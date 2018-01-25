/*
 * 
 *
 * Author:  <@iriver>
 * Created: May, 2013,2016
 * Description: Definitions for configurations.
 * 2016.11 jhlim.
 * Copyright (C) Iriver, Inc.
 *
 */
#ifndef __ASTELL_KERN__H__
#define __ASTELL_KERN__H__

//#include <mach/ir_ioctl.h>

enum {
	AK_PWR_ON = 1,
	AK_PWR_OFF = 0
};

enum {
	AK_NORMAL_ACT = 0,
	AK_FORCE_ACT = 1
};

enum {
	DOCK_PWR_ON = 1,
	DOCK_PWR_OFF = 0
};

typedef enum {
	AK_USB_MODE_NONE=0,
	AK_USB_MODE_DEVICE,
	AK_USB_MODE_HOST,
	AK_USB_MODE_DOCK,

	AK_USB_MODE_MAX,
} AK_USB_MODE;

typedef enum {
	AK_USB_HOST_FUNC_NOTSUPPORTED=-1,
	AK_USB_HOST_FUNC_NONE=0,
	AK_USB_HOST_FUNC_CDROM,
	AK_USB_HOST_FUNC_USBAUDIO,
	AK_USB_HOST_FUNC_STORAGE,

	AK_USB_HOST_FUNC_MAX,
} AK_USB_HOST_FUNC;

typedef enum {
	AK_USB_DEVICE_FUNC_NONE=0,
	AK_USB_DEVICE_FUNC_MTP=AK_USB_HOST_FUNC_MAX+1,
	AK_USB_DEVICE_FUNC_DAC,

	AK_USB_DEVICE_FUNC_MAX,
} AK_USB_DEVICE_FUNC;


	
typedef enum {
	ADP_NONE = -1,
	ADP_PRE_AP_I2S_PLAYBACK 	= 1,
	ADP_AP_I2S_PLAYBACK 		= 2,  // AP  --I2S-->  DAC
	ADP_AP_USB_PLAYBACK			= 4,  // AP  --USB-->  XMOS
	ADP_PC_USB_PLAYBACK			= 8,  // PC  --USB-->  XMOS
	ADP_OPTICAL_IN				= 16,
	ADP_OPTICAL_OUT				= 32,
	ADP_AP_EXTUSB_PLAYBACK			= 64, //J170630
} AUDIO_DATA_PATH_TYPE;

enum {
	AK_ADP_FAIL = -1,
	AK_ADP_OK,
	AK_ADP_SAME = 1
};


#define FORCE_SET 1
#define NORMAL_SET 0

typedef enum {
	AVT_VALUE = 0,  // value change	
	AVT_ALL,   // value & volume
	AVT_VOLUME    // volume
} AUDIO_VOLUME_TYPE;


typedef enum {
	AMT_HP_MUTE = 1,  //headphone mute  
	AMT_SW_MUTE = 2,  //switch mute
	AMT_BAL_CHG_MUTE = 4,  //switch mute
	AMT_DAC_MUTE= 8,   // DAC mute
	AMT_VOLUME_MUTE= 16,   // DAC volume mute
	AMT_DELAY	= 32,	// nodelay
	AMT_MDELAY	= 64,	// nodelay
	AMT_MUTE_ALL = AMT_HP_MUTE |AMT_SW_MUTE|AMT_DAC_MUTE | AMT_DELAY
} AUDIO_HW_MUTE_TYPE;


typedef enum snd_pwr_ctrl{
	AK_AUDIO_POWER_ALL_OFF =0,
	AK_AUDIO_POWER = 1,
	AK_AUDIO_XMOS_POWER = 2,
	AK_OPTICAL_RX_POWER=4,
	AK_OPTICAL_TX_POWER=8,
	AK_AUDIO_POWER_SUSPEND=16  /* [downer] A130827 */
}snd_pwr_ctrl_t;


typedef enum {
	EXT_DOCK_OFF = 0,	
	EXT_DOCK_ON
} AK_AUDIO_OUT_TYPE;


typedef enum {
	AUD_PATH_NONE =0,
	AUD_PATH_HP_35 = 1,
	AUD_PATH_HP_65 = 2,
	AUD_PATH_HP_25_BAL = 4,
	AUD_PATH_FIXED_UNBAL = 8,
	AUD_PATH_FIXED_BAL = 16,
	AUD_PATH_VAR_BAL = 32,
	AUD_PATH_VAR_UNBAL = 64,
	AUD_PATH_DIG_IN_OPTICAL = 128,
	AUD_PATH_DIG_OUT_OPTICAL = 256
	
} AK_SND_OUT_PATH;


#define DSD_MODE_OFF          0
#define DSD_MODE_DOP          1
#define DSD_MODE_NATIVE       2


//#define ENABLE_XMOS_EVENT   // 2017.05.16  32 /384khz 노이즈 수정위해 옵션 변경.

#ifdef ENABLE_XMOS_EVENT

#define ENABLE_MUTE_CTL_BY_AP
//#define ENABLE_XMOS_CTL_BY_AP
#endif

#define XMOS_MUTE_CTL_BY_KERNEL
/*

global veriables,macro define here.
extrn function define here.
2016.11.01 jhlim

*/
	
extern int printk2(const char *fmt, ...);
	
#include <linux/string.h>
	
#define __FILENAME__ (strrchr((__FILE__),('/'))+1)
	
#define ENABLE_DEBUG
	
	
#ifdef ENABLE_DEBUG
#define	DBG_PRINT(fmt, args...)	printk(" %-12s %4d %s  " fmt ,__FILENAME__,__LINE__, __FUNCTION__, ## args)
#define	DBG_CALLER(fmt, args...)	printk("\x1b[1;33m" " %-12s %4d %s %pS  " fmt "\x1b[0m\n",__FILENAME__,__LINE__, __FUNCTION__,__builtin_return_address(0), ## args)
#define	DBG_ERR(fmt, args...)		printk(" %-12s %4d %s " "\x1b[1;33m" fmt "\x1b[0m" ,__FILENAME__,__LINE__, __FUNCTION__, ## args)
#define	CPRINT(fmt, args...)		printk("\x1b[1;33m" fmt "\x1b[0m" , ## args)
#define	CPRINTR(fmt, args...)		printk(" %-12s %4d %s " "\x1b[1;33m" fmt "\x1b[0m" ,__FILENAME__,__LINE__, __FUNCTION__, ## args)
	
#else
#define	DBG_PRINT(tag,fmt, args...)	
#define	DBG_CALLER(fmt, args...)
#define	DBG_ERR(tag,fmt, args...)	
#define	CPRINT(fmt, args...)	
#define	CPRINTR(fmt, args...)
#endif


// jhlim 2017.02.06
//#define PRINT_PRE_KMSG

#ifdef PRINT_PRE_KMSG
#define PRINTK_CPU_STATUS
#define PRINTK_NO_LONGER_AFFINE
#define PRINTK_S2M_RTC
#endif





struct _ak_gpio
{
	int gpio_id;
	char* gpio_name;
	char* model;
	char* hwrev;
};


#define GET_GPIOS_DTSI(np,_gpio_pins,_gpio,_function,_firstvalue)				\
_gpio_pins._gpio = of_get_named_gpio(np, #_gpio , 0);	\
if (!gpio_is_valid(_gpio_pins._gpio)) {	\
      CPRINT("get %-24s FAIL.",#_gpio); 	\
} else  {		\
	int gpio_flag; \
	if(_function==1) gpio_flag = (_firstvalue==1? GPIOF_OUT_INIT_HIGH : GPIOF_OUT_INIT_LOW); \
	else gpio_flag = GPIOF_IN;\
	printk("get %-24s OK ",#_gpio);   \
	if (gpio_request_one(_gpio_pins._gpio,gpio_flag, #_gpio) < 0) {   \
		CPRINT("request FAIL.\n");	  \
	} else {  \
		g_gpio_list[g_gpio_list_count].gpio_name = #_gpio; \
		g_gpio_list[g_gpio_list_count].gpio_id = _gpio_pins._gpio; \
		g_gpio_list[g_gpio_list_count].model   =_gpio_pins.MODEL_NAME; \
		g_gpio_list[g_gpio_list_count].hwrev   =_gpio_pins.HW_TYPE; \
		g_gpio_list_count++;   \
		printk("request OK \n");	 \
	} \
}   \


#define DISPLAY_PIN_MUX


/*
2016.11.02 jhlim

GPIO functions.
*/

//#include "gpio/ak_gpio_define.h"



#define AUDIO_POWER_CTL 
#define AUDIO_AUTO_POWER_OFF_TIME (20)


/*
Astell & Kern Audio API list.
*/
void request_wm8804_set_pll(void);
void request_wm8804_reinit(void);
void wm8804_pll_enable(int enable);

void enable_audio_auto_power_off(int onoff);

void ak_xmos_power_on_process(int enable,int force_set);

extern void ak_set_pcm_audio_path(void);

void ak_set_xmos_port_select(int select);
int ak_is_select_xmos_port(void);

int  ak_set_audio_path(AUDIO_DATA_PATH_TYPE e_type,int force_set);
int ak_get_audio_path(void);
void set_audio_path_mutex(int onoff);

void ak_set_spdif_out_enable(int enable);
int ak_get_spdif_out_enable(void);


int ak_get_snd_pwr_ctl(int power_ctl_status);
int ak_set_snd_pwr_ctl(snd_pwr_ctrl_t snd_ctl,int pwr_ctl,int mode);
void ak_set_snd_pwr_ctl_request(snd_pwr_ctrl_t snd_ctl,int pwr_ctl);

void ak_set_hw_mute(int e_type,int onoff);
void ak_set_volume(int mode, int lvol,int rvol);




/*********************************************************************************************************************
*
*     Astell & Kern hifi audio control api.
*     
*     2016.11 jhlim
*
**********************************************************************************************************************/
struct ak_dac_ctl_obj {
	void		*userdata;
	struct i2c_client *single_dac;  // CH L
	struct i2c_client *dual_dac;	// CH R
	void (*_ak_dac_init)(void);
	void (*_ak_dac_fm)(int dsd_mode,int freq);
	void (*_ak_set_volume)(int mode, int lvol,int rvol);
	void (*_ak_get_volume)(int *lvol,int *rvol);
	void (*_ak_resume_volume)(void);
};


int get_audio_debug_print(void);

void select_ak_dac_ctl_obj(struct ak_dac_ctl_obj* pak_dac_control);


void ak_dac_init(void);
void ak_dac_fm(int dsd_mode,int freq);
void ak_set_volume(int mode, int lvol,int rvol);
void ak_get_volume(int *lvol,int *rvol);
void ak_resume_volume(void);

int is_enable_volume_ctl(void);

int AK_IOCTL_set_play_mode(struct io_buffer_t *pio_mgr);
void ak_play_notify(int play_status);


/*
audio out route function.
*/
int ak_get_route_audio_out(void);
void ak_route_audio_out(AK_SND_OUT_PATH sop,int enable);
int AK_IOCTL_route_audio_out(struct io_buffer_t *pio_mgr);


//J170125
int s5p_ehci_set_power_global(int on);
int usb3503_switch_mode_global(int mode);



/*
2017.4.1 jhlim
xmos usb card detect function.
*/

int ak_get_xmos_usb_card_num(void);
int ak_wait_for_xmos_usb_card_connect(void);
int ak_wait_for_xmos_usb_card_disconnect(void);
int ak_is_avaiable_xmos_card(void);


int S2MPS15_READ_REG(u8 reg1);
int S2MPS15_GET_REG(u8 reg1);
int S2MPS15_WRITE_REG(u8 reg1);
int S2MPS15_SET_REG(u8 reg1,u8 value);


void wm8804_spdif_tx_pwr(int onoff);


int AKIO_POGO_cmd(struct io_buffer_t *pio_mgr);

#endif	


