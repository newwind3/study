/*
  ir_ioctl_cmd.c

  Copyright 2010 iriver
  2013-5-22 tcc8930
  2016-10-25 exynos7420
  jhlim.
*/


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/switch.h>
#include <linux/i2c.h>
#include <linux/spi/spi.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/tlv.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>

//#include <ak/aknl_core.h>
//#include <ak/ak_common.h>

#include <linux/io.h>
//#include <mach/gpio.h>
#include <asm/gpio.h>

#include <linux/miscdevice.h>

#include <ak/ir_ioctl.h>



#include <linux/regulator/machine.h>
#include <linux/regulator/consumer.h>

#include <linux/bitops.h>

#include <ak/astell_kern.h>
#include <akaudio.h>

/*
  AK_IO cmd function.

  argv1 : AK_IO
*/
	

#if 0	
int AKIO_XMOS_cmd(struct io_buffer_t *pio_mgr)
{
	char mode1[32] = {0};

	sio_read_string(pio_mgr, mode1);

	if (!strcmp(mode1, "FLASH_PATH")) {
		int path = sio_read_int(pio_mgr);
		CPRINT("flash PATH:%s \n",path == 0 ? "XMOS" : "AP");
		xmos_selpath(path);

	}
	else if (!strcmp(mode1, "POWER")) {
		int onoff = sio_read_int(pio_mgr);
		CPRINT("POWER:%s\n",onoff == 1? "ON" : "OFF");
		xmos_power(onoff);
	}
	else if (!strcmp(mode1, "RESET")) {
		CPRINT("RESET\n");
		xmos_reset();
	}
	else if (!strcmp(mode1, "DEBUG_LEVEL")) {		
		int debug_level = sio_read_int(pio_mgr);

		CPRINT("DEBUG_LEVEL: %d\n",debug_level);
		ak_set_xmos_debug_level(debug_level);
	}
	else if (!strcmp(mode1, "CTL_BY_HOST")) {
		int ctl_by_host = sio_read_int(pio_mgr);

		CPRINT("CTL_BY_HOST: %d\n",ctl_by_host);
		ak_set_xmos_ctl_by_host(ctl_by_host);
	}

	return 0;
}

int AKIO_XMOS_CB_cmd(struct io_buffer_t *pio_mgr)
{
	char mode1[32] = {0};
	int lvol,rvol;
	
	sio_read_string(pio_mgr, mode1);

	if (!strcmp(mode1, "FLAG_EVT_BOOTED")) {
		CPRINT("FLAG_EVT_BOOTED\n");			
		ak_set_xmos_alive(1);		

		ak_get_volume(&lvol,&rvol);

		sio_set_argv(pio_mgr,
					SIO_ARGV_INT32, "aud path",ak_get_audio_path(),
					SIO_ARGV_INT32, "ctl host",ak_get_xmos_ctl_by_host(),
					SIO_ARGV_INT32, "debug", ak_get_xmos_debug_level(),
					SIO_ARGV_INT32, "lvol", lvol,
					SIO_ARGV_INT32, "rvol", rvol,
					SIO_ARGV_END);
	}
	else if (!strcmp(mode1, "FLAG_EVT_DAC_INIT")) {
		ak_dac_init();
	}
	else if (!strcmp(mode1, "FLAG_EVT_DAC_FM")) {
		int dsd_mode = sio_read_int(pio_mgr);
		int rate 	 = sio_read_int(pio_mgr);
		//if(dsd_mode == 1)  
		{ 
			ak_dac_fm(dsd_mode,rate);
		}
	}
	else if (!strcmp(mode1, "FLAG_EVT_SET_MUTE")) {
		int mute_type = sio_read_int(pio_mgr);
		int onoff = sio_read_int(pio_mgr);

		if(onoff) {
			ak_set_hw_mute(mute_type,1);
		} 
		else {
			
			if(ak_get_audio_path() == ADP_PC_USB_PLAYBACK) {
				ak_set_hw_mute(mute_type,0);
			}
			else {
				if(ak_is_avaiable_xmos_card()) {	  // if XMOS connected 	
					ak_set_hw_mute(mute_type,0);
				}
			}
		}

		// CPRINT("FLAG_EVT_SET_MUTE: %d\n",onoff);
	}

	return 0;
}

/*
  itools cmd function.

  argv1 : ITOOLS_IO
*/
	

int ak_cpu_reg_ctl(struct io_buffer_t *pio_mgr)
{
	char control_type[32] = {0};
	sio_read_string(pio_mgr, control_type);

	if (!strcmp(control_type, "READ_REG"))  {
		/*
		  2016.11.1 : add 64bit core.
		*/
		unsigned int  reg_addr, reg_value;
		phys_addr_t physical_addr;
		void __iomem  *virtual_addr;

		reg_addr =  sio_read_int(pio_mgr);
		physical_addr = (phys_addr_t)reg_addr;

		virtual_addr = (void __iomem  *)ioremap(physical_addr, SZ_16);
		if (!virtual_addr) {

			printk("ioremap error\n");
		} else {

			reg_value =  readl(virtual_addr);

			//printk("CPU_REG_READ 0x%x %p va=%p\n",reg_addr,physical_addr,virtual_addr);

			sio_set_argv(pio_mgr,
						 SIO_ARGV_INT32, "reg_value", reg_value,
						 SIO_ARGV_END);

			iounmap((void __iomem *)virtual_addr);
		}

	}
	else if (!strcmp(control_type, "WRITE_REG")) {
		phys_addr_t physical_addr;
		unsigned int write_addr, reg_value;
		void __iomem  *virtual_addr;


		write_addr =  sio_read_int(pio_mgr);
		reg_value =  sio_read_int(pio_mgr);

		physical_addr = (phys_addr_t)write_addr;

		virtual_addr = (void __iomem  *)ioremap(physical_addr, SZ_16);

		if (virtual_addr) {
			writel(reg_value, virtual_addr);
			iounmap((void __iomem *)virtual_addr);
		}
	}

  
	return 0;
}

int ak_wm8804_ctl(struct io_buffer_t *pio_mgr)
{
	char control_type[32] = {0};
	sio_read_string(pio_mgr, control_type);

	if (!strcmp(control_type, "READ_REG")) {

		extern int WM8804_READ(unsigned char reg_addr);
		int reg_addr, reg_value;

		reg_addr =  sio_read_int(pio_mgr);
		reg_value = WM8804_READ(reg_addr);
		//      DBG_PRINT("IR_IOCTL","%x \n",reg_value);
		sio_set_argv(pio_mgr,
					 SIO_ARGV_INT8, "reg_value", reg_value,
					 SIO_ARGV_END);
	}

	else if (!strcmp(control_type, "WRITE_REG"))  {
		extern int WM8804_WRITE(unsigned int reg, unsigned int val);

		int reg_addr, reg_value;
		reg_addr =  sio_read_int(pio_mgr);
		reg_value =  sio_read_int(pio_mgr);
		WM8804_WRITE(reg_addr, reg_value);

		DBG_PRINT("WRITE:%x VALUE:%d(%x) \n", reg_addr, reg_value, reg_value);
	}
	else if (!strcmp(control_type, "wm8804_reset"))  {

		extern int WM8804_RESET(void);

		WM8804_RESET();


		CPRINTR("WM8804_RESET\n");
	}

	return 0;
}



int ak_ak4497_ctl(struct io_buffer_t *pio_mgr)
{
	char control_type[32] = {0};
	sio_read_string(pio_mgr, control_type);


	if (!strcmp(control_type, "READ_REG"))  {
		extern   int AK4497_READ(int ch, unsigned short reg);

		int ch, reg_addr, reg_value;
		ch =  sio_read_int(pio_mgr);

		reg_addr =  sio_read_int(pio_mgr);
		reg_value = AK4497_READ(ch, reg_addr);

		//DBG_PRINT("IR_IOCTL","AK4497 CH:%d READ:%x VALUE:%d(%x) \n",ch,reg_addr,reg_value,reg_value);
		sio_set_argv(pio_mgr,
					 SIO_ARGV_INT32, "reg_value", reg_value,
					 SIO_ARGV_END);
	}

	else if (!strcmp(control_type, "WRITE_REG"))
	{
		extern int AK4497_WRITE(int ch, unsigned short reg, unsigned short value);

		int ch, reg_addr, reg_value;
		ch =  sio_read_int(pio_mgr);
		reg_addr =  sio_read_int(pio_mgr);
		reg_value =  sio_read_int(pio_mgr);
		AK4497_WRITE(ch, reg_addr, reg_value);

		//DBG_PRINT("IR_IOCTL","CS4398 CH:%d WRITE:%x VALUE:%d(%x) \n",ch,reg_addr,reg_value,reg_value);
	}

	return 0;
}

int ak_gpio_ctl(struct io_buffer_t *pio_mgr)
{
	char control_type[32] = {0};
	sio_read_string(pio_mgr, control_type);

	if (!strcmp(control_type, "LIST")) {
		//char type[32] = {0};
		int gpio_list_num;
		int i;
		extern int g_gpio_list_count;
		extern struct _ak_gpio g_gpio_list[];
		gpio_list_num = g_gpio_list_count;

		CPRINT("\n");
		for (i = 0; i < gpio_list_num; i++) {

			CPRINT("%-2d %-10s %-5s  %-25s %-5s\n", i, g_gpio_list[i].model, g_gpio_list[i].hwrev, g_gpio_list[i].gpio_name, gpio_get_value(g_gpio_list[i].gpio_id) == 1 ? "HIGH" : "LOW");

		}

		sio_set_argv(pio_mgr,
					 SIO_ARGV_INT8, "max_list", gpio_list_num,
					 SIO_ARGV_END);




	}
	else if (!strcmp(control_type, "LIST_READ")) {
		int num;
		int val;
		extern int g_gpio_list_count;
		extern struct _ak_gpio g_gpio_list[];
		num = sio_read_int(pio_mgr);

		if (num >= 0 && num < g_gpio_list_count)  {
			val = gpio_get_value(g_gpio_list[num].gpio_id);
			CPRINT("\n%-2d %-25s %-5s\n", num, g_gpio_list[num].gpio_name, val ? "HIGH" : "LOW");
		} else {
			CPRINT("invalid gpio index \n");
		}

	}

	else if (!strcmp(control_type, "LIST_WRITE")) {
		int num;
		int val;
		extern int g_gpio_list_count;
		extern struct _ak_gpio g_gpio_list[];
		num = sio_read_int(pio_mgr);
		val = sio_read_int(pio_mgr);

		if (num >= 0 && num < g_gpio_list_count)  {
			gpio_set_value(g_gpio_list[num].gpio_id, val);
			CPRINT("\n%-2d %d %-25s %-5s\n", num, g_gpio_list[num].gpio_id, g_gpio_list[num].gpio_name, gpio_get_value(g_gpio_list[num].gpio_id) == 1 ? "HIGH" : "LOW");
		} else {
			CPRINT("invalid gpio index \n");
		}
	}
	else if (!strcmp(control_type, "WRITE")) {
		char type[32] = {0};
		int num;
		int val;

		sio_read_string(pio_mgr, type);
		num = sio_read_int(pio_mgr);
		val = sio_read_int(pio_mgr);

	}
	else if (!strcmp(control_type, "READ")) {

		char type[32] = {0};
		int num;
		int val = 0;

		sio_read_string(pio_mgr, type);
		num = sio_read_int(pio_mgr);

		sio_set_argv(pio_mgr,
					 SIO_ARGV_INT32, "reg_value", val,
					 SIO_ARGV_END);

	}

	else if (!strcmp(control_type, "CONF")) {
		//extern void tcc8930_gpio_set_conf(char*type, int num, int io, int func);
		char type[32] = {0};
		int num, io, func;

		sio_read_string(pio_mgr, type);
		num = sio_read_int(pio_mgr);
		io = sio_read_int(pio_mgr);
		func = sio_read_int(pio_mgr);

		//tcc8930_gpio_set_conf(type, num, io, func);
	}

	return 0;

}

int ak_pmic_ctl(struct io_buffer_t *pio_mgr)
{

	char control_type[32] = {0};
	char ldo_name[32] = {0};
	int enable = 0;


	sio_read_string(pio_mgr, control_type);

	if (!strcmp(control_type, "LDO")) {
		extern struct device *g_s2mps15_dev;

		u8 reg_addr = -1, reg_value;

		struct regulator *vdd_ldo;

		sio_read_string(pio_mgr, ldo_name);
		enable =  sio_read_int(pio_mgr);

		if (!strcmp(ldo_name, "vdd_ldo14")) {
			reg_addr = S2MPS15_REG_L14CTRL;
		}
		else if (!strcmp(ldo_name, "vdd_ldo22")) {
			reg_addr = S2MPS15_REG_L22CTRL;
		}

		if (reg_addr != -1) {
			reg_value = S2MPS15_READ_REG(reg_addr);

			if (enable) {
				reg_value |= 0b01000000;
			} else {
				reg_value &= 0b00111111;
			}
			S2MPS15_SET_REG(reg_addr, reg_value);
			S2MPS15_WRITE_REG(reg_addr);
		}

	}
	else if (!strcmp(control_type, "READ_REG"))   {
		int reg_addr, reg_value;
		reg_addr =  sio_read_int(pio_mgr);
		S2MPS15_READ_REG((u8)reg_addr);
	}

	else if (!strcmp(control_type, "WRITE_REG"))
	{
		int reg_addr, reg_value;
		reg_addr =  sio_read_int(pio_mgr);
		reg_value =  sio_read_int(pio_mgr);
		S2MPS15_WRITE_REG((u8)reg_addr);

		if (ir_get_debug())  {
			DBG_PRINT("WRITE_REG:%d VALUE:%d(%x) \n", reg_value, reg_value);
		}
	}
	else if (!strcmp(control_type, "GET_REG"))    {
		int reg_addr, reg_value;
		reg_addr =  sio_read_int(pio_mgr);

		reg_value = S2MPS15_GET_REG(reg_addr);
		//reg_value = 10;
		//CPRINTR("PMIC READ:%x VALUE:%d(%x) %d sizeof(int):%d sizeof(char*):%d\n",reg_addr,reg_value,reg_value,pio_mgr->rw_offset,sizeof(int),sizeof(char*));

		sio_set_argv(pio_mgr,
					 SIO_ARGV_INT32, "reg_value", reg_value,
					 SIO_ARGV_END);

	}
	else if (!strcmp(control_type, "SET_REG"))
	{
		int reg_addr, reg_value;
		reg_addr =  sio_read_int(pio_mgr);
		reg_value =  sio_read_int(pio_mgr);
		S2MPS15_SET_REG((u8)reg_addr, (u8)reg_value);
		if (ir_get_debug())  {
			DBG_PRINT("SET_REG:%d VALUE:%d(%x) \n", reg_value, reg_value);
		}

	}

	return 0;

}

#include <linux/mtd/mtd.h>
extern int M25P_QUAD_ENABLE(int enable);

int ak_m25p80_ctl(struct io_buffer_t *pio_mgr)
{
	extern int M25P_READ(u8 reg, u8 * data, int len);
	extern int M25P_WRITE(u8 reg, u8 * data, int len);

	char oper[32];

	struct mtd_info *fact;

	sio_read_string(pio_mgr, oper);


#if 0
	fact = get_mtd_device_nm("xmos-factory0");

	if (IS_ERR(fact)) {
		CPRINT("XMOS Updater : cannot find xmos-factory0\n");
		return -ENODEV;
	}
#endif


	if (!strcmp(oper, "READ_REG")) {
		u8 data[10];
		int reg =  sio_read_int(pio_mgr);
		int len =  sio_read_int(pio_mgr);

		M25P_READ(reg, data, len);

		sio_set_argv(pio_mgr,
					 SIO_ARGV_DATA, "data", data, len,
					 SIO_ARGV_END);
	}
	else if (!strcmp(oper, "WRITE_REG")) {
		u8 data[10];
		int reg =  sio_read_int(pio_mgr);
		int data_len = sio_read_data(pio_mgr, data);

		M25P_WRITE(reg, data, data_len);
	}
	else if (!strcmp(oper, "info")) {

		sio_set_argv(pio_mgr,
					 SIO_ARGV_STR, "name", fact->name,
					 SIO_ARGV_INT32, "size", (int)fact->size,
					 SIO_ARGV_INT32, "erasesize", (int)fact->erasesize,
					 SIO_ARGV_END);

	}
	else if (!strcmp(oper, "boot_enable")) {
		M25P_QUAD_ENABLE(1);


	}
	else if (!strcmp(oper, "boot_disable")) {
		M25P_QUAD_ENABLE(0);


	}
	else if (!strcmp(oper, "info")) {
		M25P_QUAD_ENABLE(0);

	}
	return 0;
}

int ak_test(struct io_buffer_t *pio_mgr)
{
	char mode1[32] = {0};

	sio_read_string(pio_mgr, mode1);
	
	if (!strcmp(mode1, "PC_FI_TEST")) {
		CPRINT("PC_FI_TEST !!\n");
		ak_set_audio_path(ADP_PC_USB_PLAYBACK, FORCE_SET);
	}
	else if (!strcmp(mode1, "AP_USB_TEST")) {
		CPRINT("AP_USB_TEST !!\n");
		int ret,cardnum;

		ret = ak_set_audio_path(ADP_AP_USB_PLAYBACK, NORMAL_SET);

		cardnum = ak_get_xmos_usb_card_num();
	  	
		CPRINTR("usb card num:%d \n",cardnum);
	  
		sio_set_argv(pio_mgr,
					 SIO_ARGV_INT32, "cardnum", cardnum,
					 SIO_ARGV_END);
	  
	}

	return 0;
}
#endif

#if 0
int ak_cs4398_ctl(struct io_buffer_t *pio_mgr)
{
	char control_type[32] = {0};
	sio_read_string(pio_mgr, control_type);


	if (!strcmp(control_type, "READ_REG"))   {
		extern   int cs4398_read(int ch, unsigned short reg);

		int ch, reg_addr, reg_value;
		ch =  sio_read_int(pio_mgr);
		reg_addr =  sio_read_int(pio_mgr);
		reg_value = cs4398_read(ch, reg_addr);

		//printk("CS4398 CH:%d READ:%x VALUE:%d(%x) \n",ch,reg_addr,reg_value,reg_value);
		sio_set_argv(pio_mgr,
					 SIO_ARGV_INT32, "reg_value", reg_value,
					 SIO_ARGV_END);
	} else if (!strcmp(control_type, "WRITE_REG")) {
		extern int cs4398_write(int ch, unsigned short reg, unsigned short value);

		int ch, reg_addr, reg_value;
		ch =  sio_read_int(pio_mgr);
		reg_addr =  sio_read_int(pio_mgr);
		reg_value =  sio_read_int(pio_mgr);
		cs4398_write(ch, reg_addr, reg_value);

		//printk("CS4398 CH:%d WRITE:%x VALUE:%d(%x) \n",ch,reg_addr,reg_value,reg_value);
	}

  
	return 0;
}
#endif

#if 0
int ak_ak8157a_ctl(struct io_buffer_t *pio_mgr)
{
	char control_type[32] = {0};
	sio_read_string(pio_mgr, control_type);

	if (!strcmp(control_type, "READ_REG"))   {
		extern   int ak8157a_read_reg(unsigned short reg);

		int reg_addr, reg_value;
		reg_addr =  sio_read_int(pio_mgr);
		reg_value = ak8157a_read_reg(reg_addr);

		//printk("READ:%x VALUE:%x\n", reg_addr, reg_value);
		sio_set_argv(pio_mgr,
					 SIO_ARGV_INT32, "reg_value", reg_value,
					 SIO_ARGV_END);
	} else if (!strcmp(control_type, "WRITE_REG")) {
		extern int ak8157a_write_reg(unsigned short reg, unsigned short value);

		int reg_addr, reg_value;
		reg_addr =  sio_read_int(pio_mgr);
		reg_value =  sio_read_int(pio_mgr);
		ak8157a_write_reg(reg_addr, reg_value);

		//printk("WRITE:%x VALUE:%x\n", reg_addr, reg_value);
	}
  
	return 0;
}
#endif

int ak_s5p4418_register(struct io_buffer_t *pio_mgr)
{
#if 0
	extern unsigned int i2s_register_io(int io, int reg, unsigned int val);
	char io[32] = {0};
	unsigned int reg = 0;

	sio_read_string(pio_mgr, io);
	reg = sio_read_int(pio_mgr);

	if (!strcmp(io, "READ_REG")) {
		int value = i2s_register_io(0, reg, 0);
		sio_set_argv(pio_mgr,
				 SIO_ARGV_INT32, "reg_value", value,
				 SIO_ARGV_END);
	} else if (!strcmp(io, "WRITE_REG")) {
		int value = sio_read_int(pio_mgr);
		i2s_register_io(1, reg, value);
	} 
#endif
	return 0;
}

/*

  ir_ioctl_cmd


*/

#define CMD_ARGV0(argva) ( !strcmp((cmd_argv0),(argva)) )
#define CMD_ARGV1(argva) ( !strcmp((cmd_argv1),(argva)) )
#define CMD_ARGV2(argva) ( !strcmp((cmd_argv2),(argva)) )

extern void ak_power_key_control(int mode);
extern void ak_wheel_lock(int mode);
extern void ak_set_wheel_lock(int mode);

static int g_boot_complete = 0;

int ak_get_boot_complete(void)
{
	return g_boot_complete;
}
EXPORT_SYMBOL(ak_get_boot_complete);

int ir_ioctl_cmd(struct io_buffer_t *pio_mgr)
{
	char cmd_argv0[MAX_SIO_STR] = {0,};
	char cmd_argv1[MAX_SIO_STR] = {0,};
//	char cmd_argv2[MAX_SIO_STR] = {0,};

	sio_read_string(pio_mgr, cmd_argv0); //get 1th argv

	//dump_stack();

	if (ir_get_debug())  
	{
		CPRINTR("[kernel]cmd_argv0:%s \n", cmd_argv0);
	}

	if (CMD_ARGV0("AK_IO"))  {
		sio_read_string(pio_mgr, cmd_argv1); //get 2th argv

		if (CMD_ARGV1("AK_SET_VOLUME"))  {
			int lvol, rvol;

			lvol =  sio_read_int(pio_mgr);
			rvol =  sio_read_int(pio_mgr);

			akaudio_set_volume(lvol, rvol);
		}
		else if (CMD_ARGV1("SYSTEM_BOOT_COMPLETED")) {
			int mode = sio_read_int(pio_mgr);
			g_boot_complete = mode;
			printk("\x1b[1;33m BOOT COMPLETED(%d)!\x1b[0m\n", g_boot_complete);
		}
		else if (CMD_ARGV1("KEY_LOCK")) {
			int mode = sio_read_int(pio_mgr);

			if (mode) {
				printk("\x1b[1;33m WHEEL LOCK!\x1b[0m\n");
				ak_set_wheel_lock(1);
			}
			else {
				printk("\x1b[1;33m WHEEL UNLOCK!\x1b[0m\n");
				ak_set_wheel_lock(0);
			}
		}
		else if (CMD_ARGV1("POWERKEY_LOCK")) {
			int mode = sio_read_int(pio_mgr);
			
			if (mode) {
				printk("\x1b[1;33m POWERKEY LOCK!\x1b[0m\n");
				ak_power_key_control(0);
				ak_wheel_lock(1);
			}
			else {
				printk("\x1b[1;33m POWERKEY UNLOCK!\x1b[0m\n");
				ak_power_key_control(1);				
			}

		}
		else if (CMD_ARGV1("AK_USB_HOST_START")) {
			extern int ct10_set_otg_power_en(int enable);
			mdelay(500);
			ct10_set_otg_power_en(1);
			//printk("--%s:%d--\n", __func__, __LINE__);

		}
		else if (CMD_ARGV1("AK_USB_HOST_STOP")) {
			extern int ct10_set_otg_power_en(int enable);
			extern void activo_force_id_status_change(int conidsts);
			extern int g_usb_audio_disconnect;

			ct10_set_otg_power_en(0);
			while(!g_usb_audio_disconnect)
				msleep_interruptible(100);

			activo_force_id_status_change(1);
			//printk("--%s:%d--\n", __func__, __LINE__);
		}
		else if  (CMD_ARGV1("SET_USB_AUDIO_TYPE")) {
			extern int activo_set_dma_desc_enable_global(int val);
			int mode = sio_read_int(pio_mgr);
			//printk("--%s:%d:mode[%d]--\n", __func__, __LINE__, mode);
			//activo_set_dma_desc_enable_global(mode);
		}

#if 0
		else if (CMD_ARGV1("PLAY_NOTIFY"))  {
			int paly_status;
			paly_status =	sio_read_int(pio_mgr);
			ak_play_notify(paly_status);
		}
		else if (CMD_ARGV1("AK_SPDIF_OUT"))  {
			int enable;
			enable =  sio_read_int(pio_mgr);		  
			ak_set_spdif_out_enable(enable);
		}
		else if (CMD_ARGV1("AK_SET_PLAY_MODE")) {
			AK_IOCTL_set_play_mode(pio_mgr);
		}
		else if(CMD_ARGV1("SYSTEM_BOOT_COMPLETED")) {
			extern void fusb301_set_host_type(int type);
			extern int g_ak_audio_power_off_count;

			int mode;
			mode = sio_read_int(pio_mgr);

			if(g_ak_boot_complete) 
				return 0;
			g_ak_boot_complete = mode;
			g_ak_audio_power_off_count = AUDIO_AUTO_POWER_OFF_TIME;

			CPRINT("SYSTEM_BOOT_COMPLETED....requested : %d\n",g_ak_boot_complete);

			fusb301_set_host_type(1);
		}


		else if (CMD_ARGV1("USB_DEVICE_MODE")) {
			extern struct blocking_notifier_head fusb302_notifier_list;
			extern struct blocking_notifier_head fusb301_notifier_list;
			int mode;
			mode = sio_read_int(pio_mgr);

			g_ak_usb_device_mode_setting = mode;
			CPRINT("USB_DEVICE_MODE change to %d", mode);
				
			if(gpio_get_value(g_AK400_ES_gpio_pins.DRD_VBUS_SENSE)) {

				if(ak_get_hwrev() >= AK400_HW_ES2) {

//					blocking_notifier_call_chain(&fusb301_notifier_list, USB_HOST_DETACH, NULL);
					blocking_notifier_call_chain(&fusb301_notifier_list, USB_HOST_ATTACH, NULL);
				} else {
					blocking_notifier_call_chain(&fusb302_notifier_list, USB_HOST_DETACH, NULL);
					blocking_notifier_call_chain(&fusb302_notifier_list, USB_HOST_ATTACH, NULL);
				}
			}
		}

		else if (CMD_ARGV1("AK_USB_HOST_START")) {
			extern int ak_check_support_device(void);
			extern void bq2589x_set_otg_global(int enable);
			extern int fusb302_set_dac_sel_switch(int mode);
			extern int fusb301_set_dac_sel_switch(int mode);
			extern struct blocking_notifier_head fusb301_notifier_list;

			g_ak_usb_host_func_checking = 1;
			bq2589x_set_otg_global(0);
			fusb302_set_dac_sel_switch(1);
			fusb301_set_dac_sel_switch(1);
			
			if(!ak_check_support_device()){
				bq2589x_set_otg_global(1);

				if(!ak_check_support_device()) {
					blocking_notifier_call_chain(&fusb301_notifier_list, USB_HOST_DETACH, NULL);
					return 0;
				}
			}

			if(switch_get_state(&ak_usb_func_dev) == AK_USB_HOST_FUNC_USBAUDIO) 
				msleep_interruptible(250);

			g_ak_usb_host_func_checking = 0;
		}

		else if (CMD_ARGV1("AK_USB_HOST_STOP")) {
			extern void bq2589x_set_otg_global(int enable);
			extern int fusb302_set_dac_sel_switch(int mode);
			extern int fusb301_set_dac_sel_switch(int mode);

			bq2589x_set_otg_global(0);
			fusb302_set_dac_sel_switch(0);
			fusb301_set_dac_sel_switch(0);
		}
		else if (CMD_ARGV1("AK_BALANCE_OUT"))  {

			AK_IOCTL_route_audio_out(pio_mgr);
		}
		else if (CMD_ARGV1("XMOS")) {
			AKIO_XMOS_cmd(pio_mgr);
		}	
		else if (CMD_ARGV1("XMOS_CB")) {  // from XMOS --> xmosd --> CPU
			// 	if(ak_get_xmos_alive_ready()) {
			//		CPRINT("FLAG_EVT_BOOTED IR_IOCTL\n");
			// 		AKIO_XMOS_CB_cmd(pio_mgr);
			// 	}
		}
		else if (CMD_ARGV1("KEY_LOCK")) { /* [downer/A170426] */
			extern void ak_set_keylock_mode(int mode);
			int mode;

			mode = sio_read_int(pio_mgr);
			ak_set_keylock_mode(mode);
		}
		else if (CMD_ARGV1("POGO")) {
			AKIO_POGO_cmd(pio_mgr);
		}	
		
		else {
			CPRINTR("ir_ioctl io error !!!! %s %s\n", cmd_argv0, cmd_argv1);
		}
#endif
	}
	else if (CMD_ARGV0("ITOOLS_IO"))  {

		char cmd_argv1[MAX_SIO_STR] = {0,};

		sio_read_string(pio_mgr, cmd_argv1); //get 2th argv

		if (ir_get_debug())  
		{
			CPRINTR("[kernel]cmd_argv1:%s \n", cmd_argv1);
		}


		if (CMD_ARGV1("CPU_REG")) {
//			ak_cpu_reg_ctl(pio_mgr);
		}
		else if (CMD_ARGV1("GPIO")) {
//			ak_gpio_ctl(pio_mgr);
		}
		else if (CMD_ARGV1("WM8804")) {
//			ak_wm8804_ctl(pio_mgr);
		}
		else if (CMD_ARGV1("CS4398")) {
//			ak_cs4398_ctl(pio_mgr);
		}
#if 0
		else if (CMD_ARGV1("AK8157A")) {
			ak_ak8157a_ctl(pio_mgr);
		}
#endif
		else if (CMD_ARGV1("AK4497")) {
//			ak_ak4497_ctl(pio_mgr);
		}
		else if (CMD_ARGV1("AK_TEST")) {
//			ak_test(pio_mgr);
		}
		else if (CMD_ARGV1("xmos_flash"))  {
//			ak_m25p80_ctl(pio_mgr);
		}
		else if (CMD_ARGV1( "PMIC"))   {
//			ak_pmic_ctl(pio_mgr);
		}
		else if (CMD_ARGV1("DEBUG"))  {
#if 0
			extern void set_audio_debug_print(int enable);

			int   enable =  sio_read_int(pio_mgr);
			int  ir_debug,mute_debug;

			CPRINTR("IOCTL DEBUG %s %d\n", enable ? "ON" : "OFF",enable);
			ir_debug = enable & 1;
			mute_debug = enable & 2;

			ir_set_debug(ir_debug);
			set_audio_debug_print(mute_debug);
#endif
		}
		else {
			printk("ir_ioctl io error !!!! %s \n", cmd_argv1);
		}

	}
	else if (CMD_ARGV0("AP_REGISTER"))  {
		if (CMD_ARGV1("S5P4418")) {
//			ak_s5p4418_register(pio_mgr);
		}
	}
	else
	{
		printk("ir_ioctl io error !!!! %s \n", cmd_argv0);
	}

	//CPRINTR(" pio_mgr->rw_offset:%d \n",pio_mgr->rw_offset);
	return 0;
}

int ir_ioctl2_cmd(struct io_buffer_t *pio_mgr)
{
	char cmd_argv0[MAX_SIO_STR] = {0,};
	char cmd_argv1[MAX_SIO_STR] = {0,};
//	char cmd_argv2[MAX_SIO_STR] = {0,};

	sio_read_string(pio_mgr, cmd_argv0); //get 1th argv

	//dump_stack();


	if (CMD_ARGV0("AK_IO"))  {
		sio_read_string(pio_mgr, cmd_argv1); //get 2th argv

		if (CMD_ARGV1("AK_GET_VOLUME"))  {
#if 0
			int lvol, rvol, amp_gain; 

			ak_get_volume (&lvol,&rvol);

			CPRINTR("AK_GET_VOLUME: %d %d\n",lvol,rvol);
	  
			sio_set_argv(pio_mgr,
						 SIO_ARGV_INT16, "lvol", lvol,
						 SIO_ARGV_INT16, "rvol", rvol,
						 SIO_ARGV_END);
#endif
		}
#if 0	
		else if (CMD_ARGV1("XMOS_CB")) {  // from XMOS --> xmosd --> CPU

			if(ak_get_xmos_alive_ready()) {
				AKIO_XMOS_CB_cmd(pio_mgr);
			}
		}
#endif
		else {
			CPRINTR("ir_ioctl io error !!!! %s %s\n", cmd_argv0, cmd_argv1);
		}

	}

	else
	{
		printk("ir_ioctl io error !!!! %s \n", cmd_argv0);
	}

	//CPRINTR(" pio_mgr->rw_offset:%d \n",pio_mgr->rw_offset);
	return 0;
}


