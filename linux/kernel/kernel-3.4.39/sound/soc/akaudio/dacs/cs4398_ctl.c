/*
 * cs4398_ctl.c  --  
 *
 * Copyright 2010 Iriver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
 
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/firmware.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <asm/unaligned.h>
#include <asm/gpio.h>

#include <sound/pcm.h>

#include <mach/platform.h>
#include <mach/gpio.h>

#include <akaudio.h>

#include "cs4398_ctl.h"

#define CHANNEL_RIGHT	0
#define CHANNEL_LEFT	1

#define CS4398_NAME "cs4398"

#define REG_CHIPID		0x1
#define REG_MODCTRL		0x2
#define REG_VOLMIX		0x3
#define REG_MUTECTRL	0x4
#define REG_CH_A_VOL	0x5
#define REG_CH_B_VOL	0x6
#define REG_RAMPFILT	0x7
#define REG_MISC1		0x8
#define REG_MISC2		0x9

static struct i2c_device_id *cs4398_idtable;
static struct i2c_driver *cs4398_i2c_driver;
static struct i2c_client **cs4398_i2c_client;

/* The order of addresses is very important */
static unsigned short address_list[] = { 0b1001100, 0b1001111, I2C_CLIENT_END }; 

static struct akaudio_reg g_cs4398_regs[] = {
	{REG_CHIPID, 0},
	{REG_MODCTRL, 0},
	{REG_VOLMIX, 0},
	{REG_MUTECTRL, 0},
	{REG_CH_A_VOL, 0},
	{REG_CH_B_VOL, 0},
	{REG_RAMPFILT, 0},
	{REG_MISC1, 0},
	{REG_MISC2, 0},
};

struct akaudio_reg* cs4398_regs(uint32_t *count)
{
	*count = sizeof(g_cs4398_regs) / sizeof(struct akaudio_reg);
	return g_cs4398_regs;
}

static int write_reg (struct i2c_client *client, u8 reg, u8 data)
{
	int ret = 0;
	uint8_t buf[2];
	buf[0] = reg;
	buf[1] = data;

	ret = i2c_master_send(client, buf, 2);
	if (ret < 0)  {
		printk ("Write Error addr:%x\n", client->addr);
		return -1;
	}

	return 0;
}

static int read_reg (struct i2c_client *client, u8 reg)
{
	int ret = 0;
	uint8_t buf[2];
	buf[0] = reg;
	ret = i2c_master_send(client, buf, 1);
	if (ret < 0)  {
		printk ("Read Error addr:%x\n", client->addr);
		return -1;
	}

	client->flags = I2C_M_RD;
	ret = i2c_master_recv(client, buf, 1);
	if (ret < 0) {
		printk ("Read Error addr:%x\n", client->addr);
		return -1;
	} 

	return buf[0];
}

static int cs4398_write(int ch, u8 reg, u8 val)
{
	int ret = -1;

	if (cs4398_i2c_client && cs4398_i2c_client[ch]) 
		ret = write_reg(cs4398_i2c_client[ch], reg, val);

	return ret;
}

static int cs4398_read(int ch, u8 reg)
{
	int ret = -1;

	if (cs4398_i2c_client && cs4398_i2c_client[ch]) 
		ret = read_reg(cs4398_i2c_client[ch], reg);

	return ret;
}

void read_reg_all(void)
{
	unsigned char add;

	for(add = 0; add <= 0x9; add++){
		printk("CS4398_1[0x%x]: 0x%02x\n", add, cs4398_read(0, add));
		printk("CS4398_2[0x%x]: 0x%02x\n", add, cs4398_read(1, add));
	}
}

//#define PRINT_MSG2

#ifdef USE_VOLUME_WHEEL_RAMP

#ifdef USE_VWR_TYPE1

struct timer_list g_cs4398_timer;

static int g_volume_ramp_counter = -1;

#define TIMER_INTERVAL msecs_to_jiffies(200)
	
static void cs4398_volume_ramp_work(unsigned long data)    
{

	if(g_volume_ramp_counter++==3) {
		#ifdef PRINT_MSG2
		CPRINT("K RAMP OFF\n");
		#endif
		
		cs4398_write(1,0x7,0b10110000);
	}
	
    mod_timer(&g_cs4398_timer, jiffies + TIMER_INTERVAL);
}
void cs4398_volume_ramp_reset(void)
{
	g_volume_ramp_counter = 0;
}

extern int is_touch_pressed(void);
#endif

void cs4398_ramp_mode(int mode)
{
	//CPRINT("RAMP %d\n",mode);

	if(mode == CS4398_RAMP_SR) {
		cs4398_write(0,0x7,0b10110000);
		cs4398_write(1,0x7,0b10110000);
	} else {
		cs4398_write(0,0x7,0b11110000);
		cs4398_write(1,0x7,0b11110000);
	}

}
#endif

static void cs4398_set_volume(int lvol, int rvol)
{
	unsigned short codec_lvol,codec_rvol;
	int ch;

	if(lvol <0) lvol = 0;
	if(lvol > MAX_VOLUME_IDX) lvol= MAX_VOLUME_IDX;

	if(rvol <0) rvol = 0;
	if(rvol > MAX_VOLUME_IDX) rvol= MAX_VOLUME_IDX;

	if(lvol==0) {
		codec_lvol = 0xff;
	} else {
		codec_lvol = (MAX_VOLUME_IDX-lvol);
	}

	if(rvol==0) {
		codec_rvol = 0xff;
	} else {
		codec_rvol = (MAX_VOLUME_IDX-rvol);
	}

	if(lvol != 0) { 
		codec_lvol += MAX_VOLUME_OFFSET;
	}

	if(rvol !=0) {
		codec_rvol += MAX_VOLUME_OFFSET;
	}

	#ifdef PRINT_VOLUME_VALUE
	CPRINT("(lvol:%d(%x) rvol:%d(%x))\n", lvol,codec_lvol,rvol,codec_rvol); 		
	#endif

	for(ch = 0; ch < akaudio_get_dac_count(); ch++) {
		cs4398_write(ch, REG_CH_A_VOL, codec_lvol);
		cs4398_write(ch, REG_CH_B_VOL, codec_rvol);
	}

#ifdef USE_VWR_TYPE1
	{

		if(is_touch_pressed()) {
			g_volume_ramp_counter = -1;
			#ifdef PRINT_MSG2	
			CPRINT("T RAMP ON\n");
			#endif
			
			cs4398_write(1,0x7,0b11110000);
		} else {
			if(g_volume_ramp_counter==0) {
				#ifdef PRINT_MSG2
				CPRINT("K RAMP ON \n");		
				#endif
				
				cs4398_write(1,0x7,0b11110000);
			} else {
				#ifdef PRINT_MSG2
				CPRINT("T RAMP OFF 2\n");
				#endif
				
				cs4398_write(1,0x7,0b10110000);
			}
		}
	}
#endif
}

static void cs4398_volume_mute(void)
{
	int ch;

	for (ch = 0; ch < akaudio_get_dac_count(); ch++) {
		cs4398_write(ch, REG_CH_A_VOL, 255);
		cs4398_write(ch, REG_CH_B_VOL, 255);
	}
}

int cs4398_init_reg(int ch)
{
	int dac_count = akaudio_get_dac_count();

	if (ch < dac_count) {
		if (cs4398_i2c_client && cs4398_i2c_client[ch]) {
			write_reg(cs4398_i2c_client[ch], REG_MODCTRL, 0b00010000);
			if (dac_count == 1)
				write_reg(cs4398_i2c_client[ch], REG_VOLMIX, 0b00001001);
			else
				if (ch == CHANNEL_RIGHT)
					write_reg(cs4398_i2c_client[ch], REG_VOLMIX, 0b00000101);
				else if (ch == CHANNEL_LEFT)
					write_reg(cs4398_i2c_client[ch], REG_VOLMIX, 0b00001010);

			write_reg(cs4398_i2c_client[ch], REG_MUTECTRL, 0b00000000);
			write_reg(cs4398_i2c_client[ch], REG_CH_A_VOL, 0b10000000);
			write_reg(cs4398_i2c_client[ch], REG_CH_B_VOL, 0b10000000);
			write_reg(cs4398_i2c_client[ch], REG_RAMPFILT, 0b10110000);
			write_reg(cs4398_i2c_client[ch], REG_MISC1, 0b01000000);
			write_reg(cs4398_i2c_client[ch], REG_MISC2, 0b00001000);
		}

		return 0;
	}

	return -ENODEV;
}

static void cs4398_freq(uint32_t freq, uint32_t format)
{
	int ch;
	u32 mode;
	u32 misc = 0b01000000;

	printk("FM: freq %d format %d\n", freq, format);

	if (freq <= 50000) mode = 0b00010000;
	else if (freq > 50000 && freq <= 100000) {
		mode = 0b00010001;
		if (format == SNDRV_PCM_FORMAT_S24_LE) {
			misc = 0b01010000;
		}
	} else {
		mode = 0b00010010;
		if (format == SNDRV_PCM_FORMAT_S24_LE) {
			misc = 0b01010000;
		}
	}

	for (ch = 0; ch < akaudio_get_dac_count(); ch++) {
		cs4398_write(ch, REG_MODCTRL, mode);
		cs4398_write(ch, REG_MISC1, misc);
	}
}

static void cs4398_mute(int onoff)
{
	int freq = akaudio_get_samplerate();
	int format = akaudio_get_format();
	int ch;
	u32 mute = 0;
	u32 misc = 0;
	u32 mclkdiv = 0;

	if (freq > 50000) {
		if (format == SNDRV_PCM_FORMAT_S24_LE) 
			mclkdiv = 0b00010000;
	}

	if (onoff == 1) {
		mute = 0b00011000;
		misc = 0b11000000 | mclkdiv;
	} else {
		mute = 0b00000000;
		misc = 0b01000000 | mclkdiv;
	}

	for(ch = 0; ch < akaudio_get_dac_count(); ch++) {
		cs4398_write(ch, REG_MUTECTRL, mute);
		cs4398_write(ch, REG_MISC1, misc);
	}
}

void cs4398_shutdown(void)
{
	int ch;
	
	/* enter to a low-power state mode */
	for(ch = 0; ch < akaudio_get_dac_count(); ch++) 
		cs4398_write(ch, REG_MISC1, 0b11000000);
}

static void cs4398_reg_init(void)
{
	int ch;
	int dac_count = akaudio_get_dac_count();
	int l, r;

	for (ch = 0; ch < dac_count; ch++) cs4398_init_reg(ch);

	akaudio_get_volume(&l, &r);
	cs4398_set_volume(l, r);
}

static int cs4398_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;
	int chipid;
	int i;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA)) 
		return -ENODEV;

	chipid = i2c_smbus_read_byte_data(client, 0x01);

	if((chipid >> 3) != 0b01110) 
		return -ENODEV;

	for (i = 0; address_list[i] != I2C_CLIENT_END; i++) {
		if (address_list[i] == client->addr) {
			snprintf(info->type, I2C_NAME_SIZE, "%s.%d", CS4398_NAME, i);
			break;
		}
	}
	
	printk("CS4398: %s (i2c_add: 0x%x 0x%x)\n", info->type, client->addr, chipid);

	return 0;
}

static int __devinit cs4398_probe(struct i2c_client *client, const struct i2c_device_id * id)
{
	int idx = id->driver_data;

	if (idx >= akaudio_get_dac_count()) {
		printk("error: index value is to big\n");
		return -ENODEV;
	}
	
	printk ("i2c device: %s(%ld)\n", id->name, id->driver_data);

	cs4398_i2c_client[idx] = client;
	i2c_set_clientdata(client, NULL);

	return cs4398_init_reg(idx);
}

static int __devexit cs4398_remove(struct i2c_client *client)
{
	return 0;
}

static const struct dev_pm_ops cs4398_pm_ops = {
	.suspend	= NULL,
	.resume		= NULL,
};

int __init cs4398_init(int dacs, struct akaudio_dac_ops *ops)
{
	int i;
	int dac_count = dacs;

	cs4398_i2c_driver = kmalloc(sizeof(struct i2c_driver) * dac_count, GFP_KERNEL);
	if (!cs4398_i2c_driver) goto error;
	memset(cs4398_i2c_driver, 0, sizeof(struct i2c_driver) * dac_count);

	cs4398_idtable = kmalloc(sizeof(struct i2c_device_id) * dac_count, GFP_KERNEL);
	if (!cs4398_idtable) goto error;
	memset(cs4398_idtable, 0, sizeof(struct i2c_device_id) * dac_count);

	cs4398_i2c_client = kmalloc(sizeof(struct i2c_client*) * dac_count, GFP_KERNEL);
	if (!cs4398_i2c_client) goto error;
	memset(cs4398_i2c_client, 0, sizeof(struct i2c_client*) * dac_count);

	for (i = 0; i < dac_count; i++) {
		sprintf(cs4398_idtable[i].name, "%s.%d", CS4398_NAME, i);
		cs4398_idtable[i].driver_data = i;
		printk("cs4398 name : %s\n", cs4398_idtable[i].name);
		
		cs4398_i2c_driver[i].id_table 		= &cs4398_idtable[i];
		cs4398_i2c_driver[i].probe 			= cs4398_probe;
		cs4398_i2c_driver[i].remove			= __devexit_p(cs4398_remove);
		cs4398_i2c_driver[i].address_list	= address_list;
		cs4398_i2c_driver[i].detect			= cs4398_detect;
		cs4398_i2c_driver[i].class			= I2C_CLASS_HWMON,
		cs4398_i2c_driver[i].driver.name 	= cs4398_idtable[i].name;
		cs4398_i2c_driver[i].driver.pm 		= &cs4398_pm_ops;

		if (i2c_add_driver(&cs4398_i2c_driver[i]) < 0)
			goto error;
	}

	ops->init_dac 		= cs4398_reg_init;
	ops->set_volume 	= cs4398_set_volume;
	ops->set_vol_mute 	= cs4398_volume_mute;
	ops->set_dac_mute 	= cs4398_mute;
	ops->set_freq 		= cs4398_freq;
	ops->set_shutdown 	= cs4398_shutdown;
	ops->read_reg 		= cs4398_read;
	ops->write_reg 		= cs4398_write;

#ifdef USE_VWR_TYPE1
	init_timer(&g_cs4398_timer);
	g_cs4398_timer.function = cs4398_volume_ramp_work;
	g_cs4398_timer.expires = jiffies + TIMER_INTERVAL;	// 1 secs.
	add_timer(&g_cs4398_timer);
#endif
	
    return 0;

error:
	printk("Error on initializing cs4398\n");
	if (cs4398_i2c_driver) kfree(cs4398_i2c_driver);
	if (cs4398_idtable) kfree(cs4398_idtable);
	if (cs4398_i2c_client) kfree(cs4398_i2c_client);

	return -1;
}

void __exit cs4398_exit(void)
{
	int i;
	int dac_count = akaudio_get_dac_count();
	
	for (i = 0; i < dac_count; i++) 
		i2c_del_driver(&cs4398_i2c_driver[i]);
	
	if (cs4398_i2c_driver) kfree(cs4398_i2c_driver);
	if (cs4398_idtable) kfree(cs4398_idtable);
	if (cs4398_i2c_client) kfree(cs4398_i2c_client);
}

MODULE_DESCRIPTION("CS4398 DAC for Astell&Kern audio module");
MODULE_LICENSE("GPL");
