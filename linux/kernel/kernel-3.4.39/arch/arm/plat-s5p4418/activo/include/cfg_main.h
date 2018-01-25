/*------------------------------------------------------------------------------
 *
 *	Copyright (C) 2009 Nexell Co., Ltd All Rights Reserved
 *	Nexell Co. Proprietary & Confidential
 *
 *	NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
 *  AND	WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
 *  FOR A PARTICULAR PURPOSE.
 *
 *	Module     : System memory config
 *	Description:
 *	Author     : Platform Team
 *	Export     :
 *	History    :
 *	   2009/05/13 first implementation
 ------------------------------------------------------------------------------*/
#ifndef __CFG_MAIN_H__
#define __CFG_MAIN_H__

#include <nx_type.h>
#ifdef CONFIG_AK_GPIO_CONFIGS
#include "ak_gpios.h"
#else
#include "ak_gpio.h"
#endif

//------------------------------------------------------------------------------
// PLL input crystal
//------------------------------------------------------------------------------
#define CFG_SYS_PLLFIN							24000000UL

/*------------------------------------------------------------------------------
 * 	System Name
 */
#define	CFG_SYS_CPU_NAME						"s5p4418"
#define	CFG_SYS_BOARD_NAME						"s5p4418-activo"

/*------------------------------------------------------------------------------
 * 	BUS config
 */
#define CFG_DREX_PORT0_QOS_ENB              0
#define CFG_DREX_PORT1_QOS_ENB              1
#define CFG_BUS_RECONFIG_ENB                1       /* if want bus reconfig, select this first */

#define CFG_BUS_RECONFIG_DREXQOS            1
#define CFG_BUS_RECONFIG_TOPBUSSI           0
#define CFG_BUS_RECONFIG_TOPBUSQOS          0
#define CFG_BUS_RECONFIG_BOTTOMBUSSI        0
#define CFG_BUS_RECONFIG_BOTTOMBUSQOS       0
#define CFG_BUS_RECONFIG_DISPBUSSI          1

/*------------------------------------------------------------------------------
 * 	Uart
 */
/* For Low level debug */
#define	CFG_UART_DEBUG_BAUDRATE			115200
#define	CFG_UART_CLKGEN_CLOCK_HZ		50000000
//#define	CFG_UART_CLKGEN_CLOCK_HZ		14750000

/*------------------------------------------------------------------------------
 * 	Timer List (SYS = Source, EVT = Event, WDT = WatchDog)
 */
#define	CFG_TIMER_SYS_TICK_CH					0
#define	CFG_TIMER_EVT_TICK_CH					1

/*------------------------------------------------------------------------------
 * 	Extern Ethernet
 */
//#define CFG_ETHER_EXT_PHY_BASEADDR          	0x04000000	// DM9000: CS1
//#define	CFG_ETHER_EXT_IRQ_NUM					(IRQ_GPIO_C_START + 26)

/*------------------------------------------------------------------------------
 * 	GMAC PHY
 */

//#define CFG_ETHER_LOOPBACK_MODE					0       /* 0: disable, 1: 10M, 2: 100M(x), 3: 1000M(x) */

 /*for rtl8211*/
//#define	CFG_ETHER_GMAC_PHY_IRQ_NUM				(IRQ_GPIO_A_START + 9)
//#define	CFG_ETHER_GMAC_PHY_RST_NUM				(PAD_GPIO_A + 10)

/*------------------------------------------------------------------------------
 * 	Nand (HWECC)
 */
/* MTD */
//#define CFG_NAND_ECC_BYTES 						1024
//#define CFG_NAND_ECC_BITS               		40			/* 512 - 4,8,16,24 1024 - 24,40,60 */
//#define CFG_NAND_ECCIRQ_MODE

/* FTL */
//#define CFG_NAND_FTL_START_BLOCK				0x6000000	/* byte address, Must Be Multiple of 8MB */

/*------------------------------------------------------------------------------
 *	Nand (GPIO)
 */
//#define CFG_IO_NAND_nWP							(PAD_GPIO_C + 27)		/* GPIO */

/*------------------------------------------------------------------------------
 * 	Display (DPC and MLC)
 */ 
#ifdef ENABLE_ACTIVO
/* Primary */
#define CFG_DISP_PRI_SCREEN_LAYER               0
#define CFG_DISP_PRI_SCREEN_RGB_FORMAT          MLC_RGBFMT_R8G8B8//MLC_RGBFMT_A8R8G8B8
#define CFG_DISP_PRI_SCREEN_PIXEL_BYTE	        4
#define CFG_DISP_PRI_SCREEN_COLOR_KEY	        	0x090909

#define CFG_DISP_PRI_VIDEO_PRIORITY							2	// 0, 1, 2, 3
#define CFG_DISP_PRI_BACK_GROUND_COLOR	     		0x000000

#define CFG_DISP_PRI_MLC_INTERLACE              CFALSE

#define	CFG_DISP_PRI_LCD_WIDTH_MM								41.76			//480*0.087mm = 41.76 mm
#define	CFG_DISP_PRI_LCD_HEIGHT_MM							74.298 	//854*0.087mm =74.298 mm

#define CFG_DISP_PRI_RESOL_WIDTH								480
#define CFG_DISP_PRI_RESOL_HEIGHT								854

#define CFG_DISP_PRI_HSYNC_SYNC_WIDTH           10
#define CFG_DISP_PRI_HSYNC_BACK_PORCH           30
#define CFG_DISP_PRI_HSYNC_FRONT_PORCH          116
#define CFG_DISP_PRI_HSYNC_ACTIVE_HIGH          CTRUE
#define CFG_DISP_PRI_VSYNC_SYNC_WIDTH           1
#define CFG_DISP_PRI_VSYNC_BACK_PORCH           3
#define CFG_DISP_PRI_VSYNC_FRONT_PORCH          6
#define CFG_DISP_PRI_VSYNC_ACTIVE_HIGH					CTRUE

#define CFG_DISP_PRI_CLKGEN0_SOURCE             DPC_VCLK_SRC_PLL0
#define CFG_DISP_PRI_CLKGEN0_DIV                16 // even divide //yys:PLL0 533Mhz/33.8Mhz=15(35.5Mhz) or 16(33.3125Mhz) / PLL2 614.4/18=34.13MHz
#define CFG_DISP_PRI_CLKGEN0_DELAY              0
#define CFG_DISP_PRI_CLKGEN0_INVERT							0
#define CFG_DISP_PRI_CLKGEN1_SOURCE             DPC_VCLK_SRC_VCLK2
#define CFG_DISP_PRI_CLKGEN1_DIV                1
#define CFG_DISP_PRI_CLKGEN1_DELAY              0
#define CFG_DISP_PRI_CLKGEN1_INVERT							0
#define CFG_DISP_PRI_CLKSEL1_SELECT							0
#define CFG_DISP_PRI_PADCLKSEL                  DPC_PADCLKSEL_VCLK	/* VCLK=CLKGEN1, VCLK12=CLKGEN0 */

#define	CFG_DISP_PRI_PIXEL_CLOCK								800000000/CFG_DISP_PRI_CLKGEN0_DIV

#define	CFG_DISP_PRI_OUT_SWAPRB 								CFALSE
#define CFG_DISP_PRI_OUT_FORMAT                 DPC_FORMAT_RGB888
#define CFG_DISP_PRI_OUT_YCORDER                DPC_YCORDER_CbYCrY
#define CFG_DISP_PRI_OUT_INTERLACE              CFALSE
#define CFG_DISP_PRI_OUT_INVERT_FIELD           CFALSE
#define CFG_DISP_LCD_MPY_TYPE										0


/*------------------------------------------------------------------------------
 *	MIPI LCD
 */
#define CFG_MIPI_LCD_RESET						((PAD_GPIO_C + 12) | PAD_FUNC_ALT1)		/* GPIO MIPI LCD Reset*/

#else

// other lcd

#endif

/*------------------------------------------------------------------------------
 * 	LVDS
 */
#define CFG_DISP_LVDS_LCD_FORMAT             	LVDS_LCDFORMAT_VESA

/*------------------------------------------------------------------------------
 * 	HDMI
 */
#define CFG_DISP_PRI_HDMI_I2C_CHANNEL			0	/* HDMI Primary i2c channel */
#define CFG_HDMIPHY_TX_LEVEL						23	/* HDMIPHY TX Level : 0 ~ 31 */

/*------------------------------------------------------------------------------
 * 	PWM
 */
#define CFG_LCD_PRI_PWM_CH			0
#define CFG_LCD_PRI_PWM_FREQ			50000
#define CFG_LCD_PRI_PWM_DUTYCYCLE		50		/* (%) */

/*------------------------------------------------------------------------------
 * 	Audio I2S (0, 1, 2)
 */
#define CFG_AUDIO_I2S0_IS_MASTER
//#define CFG_AUDIO_I2S0_EXTERNAL_MASTER

#ifdef CFG_AUDIO_I2S0_IS_MASTER

#ifdef CFG_AUDIO_I2S0_EXTERNAL_MASTER
#define	CFG_AUDIO_I2S0_MASTER_MODE				CTRUE	// CTRUE:Master CFALSE:Slave
#define CFG_AUDIO_I2S0_MASTER_CLOCK_IN			1
#define	CFG_AUDIO_I2S0_TRANS_MODE				0		// 0:I2S, 1:Left 2:Right justified */
#define	CFG_AUDIO_I2S0_FRAME_BIT				32		// 32, 48
#define	CFG_AUDIO_I2S0_SAMPLE_RATE				44100
#define	CFG_AUDIO_I2S0_PRE_SUPPLY_MCLK			0
#else
#define	CFG_AUDIO_I2S0_MASTER_MODE				CTRUE	// CTRUE:Master CFALSE:Slave
#define CFG_AUDIO_I2S0_MASTER_CLOCK_IN			0
#define	CFG_AUDIO_I2S0_TRANS_MODE				0		// 0:I2S, 1:Left 2:Right justified */
#define	CFG_AUDIO_I2S0_FRAME_BIT				32		// 32, 48
#define	CFG_AUDIO_I2S0_SAMPLE_RATE				44100
#define	CFG_AUDIO_I2S0_PRE_SUPPLY_MCLK			0
#endif

#else
#define	CFG_AUDIO_I2S0_MASTER_MODE				CFALSE	// CTRUE:Master CFALSE:Slave
#define CFG_AUDIO_I2S0_MASTER_CLOCK_IN			1		// 0:Master 1:Slave
#define	CFG_AUDIO_I2S0_TRANS_MODE				0		// 0:I2S, 1:Left 2:Right justified */
#define	CFG_AUDIO_I2S0_FRAME_BIT				48		// 32, 48
#define	CFG_AUDIO_I2S0_SAMPLE_RATE				48000
#define	CFG_AUDIO_I2S0_PRE_SUPPLY_MCLK			0		// 1:Master 0:Slave
#endif

#define	CFG_AUDIO_I2S1_MASTER_MODE		CTRUE	// CTRUE
#define	CFG_AUDIO_I2S1_TRANS_MODE		0		// 0:I2S, 1:Left 2:Right justified */
#define	CFG_AUDIO_I2S1_FRAME_BIT		32		// 32, 48
#define	CFG_AUDIO_I2S1_SAMPLE_RATE		48000
#define	CFG_AUDIO_I2S1_PRE_SUPPLY_MCLK		1

#define	CFG_AUDIO_I2S2_MASTER_MODE		CTRUE	// CTRUE
#define	CFG_AUDIO_I2S2_TRANS_MODE		0		// 0:I2S, 1:Left 2:Right justified */
#define	CFG_AUDIO_I2S2_FRAME_BIT		32		// 32, 48
#define	CFG_AUDIO_I2S2_SAMPLE_RATE		48000
#define	CFG_AUDIO_I2S2_PRE_SUPPLY_MCLK		0

/*------------------------------------------------------------------------------
 * 	Audio SPDIF (TX/RX)
 */
#define	CFG_AUDIO_SPDIF_TX_HDMI_OUT					CTRUE
#define	CFG_AUDIO_SPDIF_TX_SAMPLE_RATE				48000
#define	CFG_AUDIO_SPDIF_RX_SAMPLE_RATE				48000

/*------------------------------------------------------------------------------
 * 	I2C
 */
#define CFG_I2C0_CLK				400000
#define CFG_I2C1_CLK				400000
#define CFG_I2C2_CLK				400000
#define CFG_I2C3_CLK				400000

/*------------------------------------------------------------------------------
 * 	SPI
 */
#define CFG_SPI0_CLK							10000000
#define CFG_SPI1_CLK							10000000
#define CFG_SPI2_CLK							10000000

#define CFG_SPI0_COM_MODE						0 /* available 0: INTERRUPT_TRANSFER, 1: POLLING_TRANSFER, 2: DMA_TRANSFER */
#define CFG_SPI1_COM_MODE						1 /* available 0: INTERRUPT_TRANSFER, 1: POLLING_TRANSFER, 2: DMA_TRANSFER */
#define CFG_SPI2_COM_MODE						1 /* available 0: INTERRUPT_TRANSFER, 1: POLLING_TRANSFER, 2: DMA_TRANSFER */

#define CFG_SPI0_CS_GPIO_MODE					1		/* 0 FSS CONTROL, 1: CS CONTRO GPIO MODE */
#define CFG_SPI1_CS_GPIO_MODE					1		/* 0 FSS CONTROL, 1: CS CONTRO GPIO MODE */
#define CFG_SPI2_CS_GPIO_MODE					0	/* 0 FSS CONTROL, 1: CS CONTRO GPIO MODE */

#define CFG_SPI0_CS				(-1)	/* 0 FSS CONTROL, 1: CS CONTRO GPIO MODE */
/*------------------------------------------------------------------------------
 *  MPEGTSIF
 */
#define CFG_MPEGTS_MASTER_MODE					1 /* 0: slave, 1: master */
#define CFG_MPEGTS_SLAVE_MODE					0 /* 0: slave, 1: master */
#define CFG_MPEGTS_CLOCKPOL						1 /* 0: falling, 1: rising */
#define CFG_MPEGTS_DATAPOL						1 /* 0: data is low, 1: data is high */
#define CFG_MPEGTS_SYNCPOL						1 /* 0: falling, 1: rising */
#define CFG_MPEGTS_ERRORPOL						1 /* 0: falling, 1: rising */
#define CFG_MPEGTS_DATAWIDTH					0 /* 0: 8bit, 1: 1bit */
#define CFG_MPEGTS_WORDCNT						47 /* 1 ~ 64 */

/*------------------------------------------------------------------------------
 * 	Keypad
 */

#define CFG_KEYPAD_KEY_BUTTON			{ PAD_GPIO_ALV + 3, PAD_GPIO_E + 23, PAD_GPIO_E + 24, PAD_GPIO_ALV + 0}
#define CFG_KEYPAD_KEY_CODE			{ KEY_PLAYPAUSE, KEY_PREVIOUSSONG, KEY_NEXTSONG, KEY_POWER}
#define CFG_KEYPAD_REPEAT			CFALSE /* 0: Repeat Off 1 : Repeat On */

/*------------------------------------------------------------------------------
 * 	SDHC
 */
#define	CFG_SDMMC2_DETECT_IO					(PAD_GPIO_ALV + 1)	/* external cd */

/*------------------------------------------------------------------------------
 * 	WIFI+BT COMBO
 */
#define CFG_WIFI_SDIO_ID						(1)
#if 0
#define CFG_GPIO_WF_POWER						(PAD_GPIO_A + 19)
#define CFG_GPIO_WF_REG_ON						(PAD_GPIO_A + 20)
#define CFG_GPIO_WF_DEV_WAKE					(PAD_GPIO_A + 21)
#define CFG_GPIO_WF_HOST_WAKE					(PAD_GPIO_A + 22)

#define	CFG_GPIO_BT_REG_ON						(PAD_GPIO_A + 23)
#define CFG_GPIO_BT_DEV_WAKE					(PAD_GPIO_A + 24)
#endif
/*------------------------------------------------------------------------------
 * 	NXE2000 PMIC
 */
#define CFG_SW_UBC_ENABLE						(1)

/**
 * 0 : GPIO interrupt (CFG_GPIO_PMIC_VUSB_DET)
 * 1 : PMIC interrupt (FVUSBDETSINT)
 */
#define CFG_USB_DET_FROM_PMIC_INT				(0)

#define CFG_GPIO_OTG_USBID_DET				(-1) //yys-	((PAD_GPIO_E + 6) | PAD_FUNC_ALT0)
#define CFG_GPIO_OTG_VBUS_DET					(-1) //yys- ((PAD_GPIO_B + 28) | PAD_FUNC_ALT1)
#define CFG_GPIO_OTG_VBUS_EN                                   (PAD_GPIO_C + 11)
	
#define CFG_GPIO_OTG_5V_EN                                     (PAD_GPIO_D + 17)

#define CFG_GPIO_PMIC_VUSB_DET					(PAD_GPIO_ALV + 2)		/* Choice for SW_UBC or Wake-up*/
#define CFG_GPIO_PMIC_LOWBAT_DET			(-1) //yys-	(PAD_GPIO_ALV + 3)		/* Critical low battery detect */
#define CFG_GPIO_PMIC_INTR						(PAD_GPIO_ALV + 4)
#define CFG_PMIC_BAT_CHG_SUPPORT				(1)
//#define CONFIG_ENABLE_INIT_VOLTAGE				/* Enalbe init voltage for ARM, CORE */

/*------------------------------------------------------------------------------
 *	TOUCH
 */
#define	CFG_IO_TOUCH_PENDOWN_DETECT			(PAD_GPIO_E + 11)
#define	CFG_IO_TOUCH_RESET_PIN				(PAD_GPIO_E + 10)

/*------------------------------------------------------------------------------
 * 	AUDIO JACK detect
 */
#define CFG_IO_HP_DETECT					((PAD_GPIO_E + 3) | PAD_FUNC_ALT0)

/*------------------------------------------------------------------------------
 *	AUDIO
 */
#define CFG_IO_AUDIO_RT5623_AMP_POWER		(PAD_GPIO_D + 31)
#define CFG_IO_AUDIO_RT5623_AMP_EN			(PAD_GPIO_E + 0)



#define CFG_IO_AUDIO_POWER_ON				(PAD_GPIO_D + 29)
#define CFG_IO_SPEAKER_AMP_POWER			(PAD_GPIO_E + 2)
#define CFG_IO_JACK_DET						(PAD_GPIO_E + 3)
#define CFG_IO_SPEAKER_AMP_ENABLE			(PAD_GPIO_E + 4)
#define CFG_IO_EARPHONE_MUTE				(PAD_GPIO_E + 1)

/*------------------------------------------------------------------------------
 *	CONEXANT
 */
#define CFG_IO_CONEXANT_RESET				(PAD_GPIO_E + 5)




/*
 * DWCOTG
 */
#define CFG_OTG_OVC_VALUE               		0   // OTGTUNE : -12%

/*------------------------------------------------------------------------------
 *	GPIO EEPROM
 */
#define CFG_IO_SPI_EEPROM_WP				((PAD_GPIO_C + 27) | PAD_FUNC_ALT1)		/* GPIO */

/*------------------------------------------------------------------------------
 *	CAMERA Back Power Down
 */
#define CFG_IO_CAMERA_BACK_POWER_DOWN     ((PAD_GPIO_E + 17) | PAD_FUNC_ALT0)

/*------------------------------------------------------------------------------
 *	CAMERA Front Power Down
 */
#define CFG_IO_CAMERA_FRONT_POWER_DOWN     ((PAD_GPIO_E + 20) | PAD_FUNC_ALT0)		/* GPIO */

/*------------------------------------------------------------------------------
 *	CAMERA Reset
 */
#define CFG_IO_CAMERA_RESET                 ((PAD_GPIO_E + 7) | PAD_FUNC_ALT0)		/* GPIO */

/*------------------------------------------------------------------------------
 *	GPS
 */
#define CFG_IO_GPS_POWER					((PAD_GPIO_E + 0) | PAD_FUNC_ALT0)	
#define CFG_IO_GPS_UART_PORT				3	
#define CFG_IO_GPS_UART_PORT_ALT			4

/*------------------------------------------------------------------------------
 * 	Suspend mode
 */

/* Wakeup Source : ALIVE [0~7] */
#define CFG_PWR_WAKEUP_SRC_ALIVE0			CTRUE	/* KEY */
#define CFG_PWR_WAKEUP_MOD_ALIVE0			PWR_DECT_FALLINGEDGE
#define CFG_PWR_WAKEUP_SRC_ALIVE1			CFALSE	/* ADCARD */
#define CFG_PWR_WAKEUP_MOD_ALIVE1			PWR_DECT_BOTHEDGE
#define CFG_PWR_WAKEUP_SRC_ALIVE2			CTRUE	/* PMIC - VBUS */
#define CFG_PWR_WAKEUP_MOD_ALIVE2			PWR_DECT_BOTHEDGE
#define CFG_PWR_WAKEUP_SRC_ALIVE3			CTRUE	/* PLAY/PAUSE */
#define CFG_PWR_WAKEUP_MOD_ALIVE3			PWR_DECT_FALLINGEDGE
#define CFG_PWR_WAKEUP_SRC_ALIVE4			CTRUE	/* PMIC INTR */
#define CFG_PWR_WAKEUP_MOD_ALIVE4			PWR_DECT_FALLINGEDGE
#define CFG_PWR_WAKEUP_SRC_ALIVE5			CFALSE	/* DCIN POWER */
#define CFG_PWR_WAKEUP_MOD_ALIVE5			PWR_DECT_BOTHEDGE

/*
 * Wakeup Source : RTC ALARM
 * ifndef Enable ALARM Wakeup
 */
#define	CFG_PWR_WAKEUP_SRC_ALARM				CTRUE

//------------------------------------------------------------------------------
// Static Bus #0 ~ #9, NAND, IDE configuration
//------------------------------------------------------------------------------
//	_BW	  : Staic Bus width for Static #0 ~ #9            : 8 or 16
//
//	_TACS : adress setup time before chip select          : 0 ~ 15
//	_TCOS : chip select setup time before nOE is asserted : 0 ~ 15
//	_TACC : access cycle                                  : 1 ~ 256
//	_TSACC: burst access cycle for Static #0 ~ #9 & IDE   : 1 ~ 256
//	_TOCH : chip select hold time after nOE not asserted  : 0 ~ 15
//	_TCAH : address hold time after nCS is not asserted   : 0 ~ 15
//
//	_WAITMODE : wait enable control for Static #0 ~ #9 & IDE : 1=disable, 2=Active High, 3=Active Low
//	_WBURST	  : burst write mode for Static #0 ~ #9          : 0=disable, 1=4byte, 2=8byte, 3=16byte
//	_RBURST   : burst  read mode for Static #0 ~ #9          : 0=disable, 1=4byte, 2=8byte, 3=16byte
//
//------------------------------------------------------------------------------
#define CFG_SYS_STATICBUS_CONFIG( _name_, bw, tACS, tCOS, tACC, tSACC, tCOH, tCAH, wm, rb, wb )	\
	enum {											\
		CFG_SYS_ ## _name_ ## _BW		= bw,		\
		CFG_SYS_ ## _name_ ## _TACS		= tACS,		\
		CFG_SYS_ ## _name_ ## _TCOS		= tCOS,		\
		CFG_SYS_ ## _name_ ## _TACC		= tACC,		\
		CFG_SYS_ ## _name_ ## _TSACC	= tSACC,	\
		CFG_SYS_ ## _name_ ## _TCOH		= tCOH,		\
		CFG_SYS_ ## _name_ ## _TCAH		= tCAH,		\
		CFG_SYS_ ## _name_ ## _WAITMODE	= wm, 		\
		CFG_SYS_ ## _name_ ## _RBURST	= rb, 		\
		CFG_SYS_ ## _name_ ## _WBURST	= wb		\
	};

//                      ( _name_ , bw, tACS tCOS tACC tSACC tOCH tCAH, wm, rb, wb )
CFG_SYS_STATICBUS_CONFIG( STATIC0 ,  8,    1,   1,   6,    6,   1,   1,  1,  0,  0 )		// 0x0000_0000
CFG_SYS_STATICBUS_CONFIG( STATIC1 ,  8,    6,   6,  32,   32,   6,   6,  1,  0,  0 )		// 0x0400_0000
CFG_SYS_STATICBUS_CONFIG(    NAND ,  8,    0,   3,   9,    1,   3,   0,  1,  0,  0 )		// 0x2C00_0000, tOCH, tCAH must be greter than 0

//#define DISABL_SLEEP //yys-lcd_wakelock Cfg_main.h	\arch\arm\plat-s5p4418\activo\Include

//yys-cut_off
//#define CUT_OFF_3000V
//#define CUT_OFF_3100V	
//#define CUT_OFF_3200V	
//#define CUT_OFF_3300V
//#define  CUT_OFF_3350V 
//#define CUT_OFF_LEPUS
#define CUT_OFF_3450V
#endif /* __CFG_MAIN_H__ */
