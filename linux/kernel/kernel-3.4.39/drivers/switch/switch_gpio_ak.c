/*
 *  drivers/switch/switch_gpio.c
 *
 * Copyright (C) 2008 Google, Inc.
 * Author: Mike Lockwood <lockwood@android.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/switch.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>
#include <nx_type.h>

#include <asm/gpio.h>
#include <mach/platform.h>
#include <mach/gpio.h>
#include <linux/kthread.h>
#include <linux/delay.h>



#define AK_BIT_HEADSET             (1 << 0)

struct ak_gpio_switch_data {
	struct switch_dev sdev;
	unsigned gpio;
	unsigned gpio_val;
	int connect_count;
	int disconnect_count;
	int debounce_count;
	int connected_check;
	int connected;


	char gpio_name[32];
	unsigned int state;
	unsigned int invert;
	unsigned int state_old;

	const char *name_on;
	const char *name_off;
	const char *state_on;
	const char *state_off;
	unsigned int state_val;
	unsigned int switch_on_val;


	int irq;
	struct work_struct work;
	struct task_struct *poll_task;
};

static struct ak_gpio_switch_data *switch_data_h2w;

struct task_struct *poll_task;

#define JACK_CONNECT_BOUNCE (3)  //jack bounce check time: 300ms 

static int g_hpjack_connected = -1;
static int g_hpjack_connected_count = 0;
static int g_temp_hp_connected = -1;



static void ak_gpio_switch_work_h2w(struct work_struct *work)
{
	struct ak_gpio_switch_data	*switch_data =
		container_of(work, struct ak_gpio_switch_data, work);

	if(switch_data->connected_check) {
		switch_data->state_val = switch_data->switch_on_val;
	//	printk("ak_gpio_switch_work_h2w state 1\n");
	} else {

		// NO use  HP DET  20171227 
		//switch_data->state_val = 0;
		switch_data->state_val = 1;
		
	//	printk("ak_gpio_switch_work_h2w state 0\n");

	}	
	
				
//	CPRINT("UNBAL switch %s : %d",
//		switch_data->state_val ? switch_data->name_on : switch_data->name_off,
//		switch_data->state_val);
	switch_set_state(&switch_data->sdev, switch_data->state_val);
}
static ssize_t switch_ak_gpio_print_state(struct switch_dev *sdev, char *buf)
{
	struct ak_gpio_switch_data	*switch_data =
		container_of(sdev, struct ak_gpio_switch_data, sdev);

//	CPRINT("switch print %s : %d",
//		switch_data->state_val ? switch_data->name_on : switch_data->name_off,
//		switch_data->state_val);
	
    return sprintf(buf, "%d\n", switch_data->state_val);
}



int ak_switch_gpio_scan_thread(void *data)
{
 	while(1)
	{
		// CT10 HP connect gpio 0    disconnect gpio 1  //20171031 dean	
		g_temp_hp_connected = !gpio_get_value(switch_data_h2w->gpio); 
		
		if(g_temp_hp_connected >0) {
			if(g_hpjack_connected_count < 0) g_hpjack_connected_count=0;

			g_hpjack_connected_count++;
		}
		else {
			if(g_hpjack_connected_count > 0) g_hpjack_connected_count=0;

			g_hpjack_connected_count--;
		}

		
		#ifdef DEBUG_JACK_DETECT
		//printk("HP:%s TX:%s\r",g_temp_hp_connected ? "O" : "X",g_temp_tx_connected ? "O" : "X");
		#endif

		if(g_hpjack_connected_count > JACK_CONNECT_BOUNCE)
		{
			if(g_hpjack_connected != 1) {
				g_hpjack_connected = 1;


				//CPRINT("HP CONNECTED\n"); 
				//ak_route_hp_balanced_out(AOT_HP);

				switch_data_h2w->connected_check = 1;
				ak_gpio_switch_work_h2w(&switch_data_h2w->work);			
			}
		} 
		else if(g_hpjack_connected_count < -JACK_CONNECT_BOUNCE) 
		{
			if(g_hpjack_connected == 1) {
				g_hpjack_connected = 0;
				
				//CPRINT("HP DISCONNECTED\n"); 

				switch_data_h2w->connected_check = 0;
				ak_gpio_switch_work_h2w(&switch_data_h2w->work);			
			}
		}
	
		if (kthread_should_stop())  {
			printk(KERN_ERR "jack_observe_thread exited !! \n");
			break;
		}

		msleep_interruptible(100);
	}

	return 0;
}



static int ak_gpio_switch_probe(struct platform_device *pdev)
{
	int ret = 0;
	//Headset Detect

	switch_data_h2w = kzalloc(sizeof(struct ak_gpio_switch_data), GFP_KERNEL);
	if (!switch_data_h2w) {
		return -ENOMEM;
	}

printk("ak_gpio_switch_probe\n");

	memset(switch_data_h2w,0x0,sizeof(struct ak_gpio_switch_data));
    switch_data_h2w->sdev.name = "h2w";
// NO use  HP DET  20171227
//    switch_data_h2w->state_val = 0;
    switch_data_h2w->state_val = 1;	
	switch_data_h2w->switch_on_val = AK_BIT_HEADSET;
    switch_data_h2w->name_on = "Headset connected";
    switch_data_h2w->name_off = "Headset disconnected";
    switch_data_h2w->state_on = "2";
    switch_data_h2w->state_off = "0";
	switch_data_h2w->sdev.print_state = switch_ak_gpio_print_state;

	switch_data_h2w->gpio = GPIO_HP_DET;

	switch_data_h2w->invert = 1;
	switch_data_h2w->debounce_count = 5; 
		
	sprintf(switch_data_h2w->gpio_name,"GPIO_SWITCH%d",switch_data_h2w->gpio);
	gpio_request(switch_data_h2w->gpio, switch_data_h2w->gpio_name  );
//	tcc_gpio_config(switch_data_h2w->gpio, GPIO_FN(0));
	gpio_direction_input(switch_data_h2w->gpio);


    ret = switch_dev_register(&switch_data_h2w->sdev);
	if (ret < 0) {
		goto err_switch_dev_register;
	}
	INIT_WORK(&switch_data_h2w->work, ak_gpio_switch_work_h2w);

	poll_task = kthread_create(ak_switch_gpio_scan_thread, switch_data_h2w, "headset-poll-thread");
    if (IS_ERR(poll_task)) {
        printk("\ntcc-gpio-poll-thread create error: %p\n", poll_task);
      //  kfree(switch_data_h2w);
        return (-EINVAL);
    }
    wake_up_process(poll_task);

	
    //ak_dbg("gpio_switch_probe()...out \n\n");

	return 0;

err_switch_dev_register:
	kfree(switch_data_h2w);

	return ret;
}

static int __devexit ak_gpio_switch_remove(struct platform_device *pdev)
{
	struct ak_gpio_switch_data *switch_data = platform_get_drvdata(pdev);

	cancel_work_sync(&switch_data->work);
	gpio_free(switch_data->gpio);
    switch_dev_unregister(&switch_data->sdev);
	kfree(switch_data);

	return 0;
}


static struct platform_driver ak_gpio_switch_driver = {
	.probe		= ak_gpio_switch_probe,
		.remove		= __devexit_p(ak_gpio_switch_remove),
		.driver		= {
		.name	= "ak-switch-gpio",
			.owner	= THIS_MODULE,
	},
};



static int __init gpio_switch_init(void)
{
	return platform_driver_register(&ak_gpio_switch_driver);
}

static void __exit gpio_switch_exit(void)
{
	platform_driver_unregister(&ak_gpio_switch_driver);
}

module_init(gpio_switch_init);
module_exit(gpio_switch_exit);

MODULE_AUTHOR("<iriver>");
MODULE_DESCRIPTION("GPIO Switch driver");
MODULE_LICENSE("GPL");
