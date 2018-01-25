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
#ifndef __CFG_GPIO_H__
#define __CFG_GPIO_H__

#define ENABLE_ACTIVO

#ifdef ENABLE_ACTIVO

/*------------------------------------------------------------------------------
 *
 *	(GROUP_A)
 *
 *	0 bit           8 bit                   12 bit          16 bit              20 bit
 *	| PAD_MODE_XXX  | PAD_FUNC_ALT(0,1,2,3) | PAD_LEVEL_XXX | PAD_PULL_UP,OFF | PAD_STRENGTH_0,1,2,3
 *
 -----------------------------------------------------------------------------*/
#define PAD_GPIOA0	/*NC*/      		(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN  | PAD_STRENGTH_1)     // 0: GPIO          ,1: PVCLK                  ,2:_                    ,3: TESTMODE[4]         =
#define PAD_GPIOA1  /*Boot MODE*/    	(PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[0]          ,2:_                    ,3:_                    =
#define PAD_GPIOA2  /*Boot MODE*/     	(PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[1]          ,2:_                    ,3: TESTMODE[0]         =
#define PAD_GPIOA3  /*Boot MODE*/     	(PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[2]          ,2:_                    ,3: TESTMODE[1]         =
#define PAD_GPIOA4  /*Boot MODE*/     	(PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[3]          ,2:_                    ,3: TESTMODE[2]         =
#define PAD_GPIOA5  /*Boot MODE*/     	(PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[4]          ,2:_                    ,3: TESTMODE[3]         =
#define PAD_GPIOA6  /*Boot MODE*/     	(PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[5]          ,2:_                    ,3:_                    =
#define PAD_GPIOA7  /*Boot MODE*/     	(PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[6]          ,2:_                    ,3:_                    =
#define PAD_GPIOA8  /*Boot MODE*/    	(PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[7]          ,2:_                    ,3:_                    =
#define PAD_GPIOA9  /*NC*/     		(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[8]          ,2:_                    ,3:_                    =
#define PAD_GPIOA10 /*NC*/     		(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[9]          ,2:_                    ,3:_                    =
#define PAD_GPIOA11 /*NC*/     		(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[10]         ,2:_                    ,3:_                    =
#define PAD_GPIOA12 /*NC*/    			(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[11]         ,2:_                    ,3:_                    =
#define PAD_GPIOA13 /*NC*/    			(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[12]         ,2:_                    ,3:_                    =
#define PAD_GPIOA14 /*HP_DET#*/	   	(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[13]         ,2:_                    ,3:_                    =
#define PAD_GPIOA15 /*HP_MUTE#*/    	(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[14]         ,2:_                    ,3:_                    =
#define PAD_GPIOA16 /*AMP_DC_EN*/    	(PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[15]         ,2:_                    ,3:-                    =
#define PAD_GPIOA17 /*PLL_RST*/    		(PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[16]         ,2:_                    ,3:_                    =
#define PAD_GPIOA18 /*DAC_RST*/    	(PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[17]         ,2:_                    ,3:_                    =
#define PAD_GPIOA19 /*DAC_EN*/    		(PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[18]         ,2:_                    ,3:_                    =
#define PAD_GPIOA20 /*OPAMP_PWR_EN*/  (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[19]         ,2:_                    ,3:_                    =
#define PAD_GPIOA21 /*NC*/    			(PAD_MODE_IN	| PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN  | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[20]         ,2:_                    ,3:_                    =
#define PAD_GPIOA22 /*WL_REG_ON*/    	(PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[21]         ,2:_                    ,3:_                    =
#define PAD_GPIOA23 /*BT_REG_ON*/    	(PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[22]         ,2:_                    ,3:_                    =
#define PAD_GPIOA24 /*BT_WAKE*/    	(PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: PDRGB24[23]         ,2:_                    ,3:_                    =
#define PAD_GPIOA25 /*NC*/    			(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: PDVSYNC              ,2:_                    ,3:_                    =
#define PAD_GPIOA26 /*NC*/    			(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: PDHSYNC              ,2:_                    ,3:_                    =
#define PAD_GPIOA27 /*NC*/    			(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: PDDE                     ,2:_                    ,3:_                    =
#define PAD_GPIOA28 /*NC*/    			(PAD_MODE_ALT | PAD_FUNC_ALT3 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VP0_EXTCLK         ,2: I2S2_CLK            ,3: I2S1_CLK            =
#define PAD_GPIOA29 /*EMMC_CLK*/    	(PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: SDMMC0_CCLK      ,2:_                    ,3:_                    =
#define PAD_GPIOA30 /*VID1[0]*/    		(PAD_MODE_ALT | PAD_FUNC_ALT3 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[0]              ,2: SDEX[0]             ,3: I2S1_BCLK           =
#define PAD_GPIOA31 /*EMMC_CMD*/    	(PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC0_CMD         ,2:_                    ,3:_                    =

/*------------------------------------------------------------------------------
 *	(GROUP_B)
 *
 *	0 bit           8 bit                   12 bit          16 bit              20 bit
 *	| PAD_MODE_XXX  | PAD_FUNC_ALT(0,1,2,3) | PAD_LEVEL_XXX | PAD_PULL_UP,OFF | PAD_STRENGTH_0,1,2,3
 *
 -----------------------------------------------------------------------------*/
#define PAD_GPIOB0  /*?VID1[1]*/    	(PAD_MODE_ALT | PAD_FUNC_ALT3 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[1]          ,2: SDEX[1]             ,3: I2S1_LRCLK          =
#define PAD_GPIOB1  /*EMMC_D0*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC0_CDATA[0]     ,2:_                    ,3:_                    =
#define PAD_GPIOB2  /*?VID1[2]*/    	(PAD_MODE_ALT | PAD_FUNC_ALT3 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[2]          ,2: SDEX[2]             ,3: I2S2_BCLK           =
#define PAD_GPIOB3  /*EMMC_D1*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC0_CDATA[1]     ,2:_                    ,3:_                    =
#define PAD_GPIOB4  /*NC*/    	   	(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN  | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[3]          ,2: SDEX[3]             ,3: I2S2_LRCLK          =
#define PAD_GPIOB5  /*EMMC_D2*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC0_CDATA[2]     ,2:_                    ,3:_                    =
#define PAD_GPIOB6  /*NC*/    		(PAD_MODE_IN | PAD_FUNC_ALT0  | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[4]          ,2: SDEX[4]             ,3: I2S1SDO             =
#define PAD_GPIOB7  /*EMMC_D3*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC0_CDATA[3]     ,2:_                    ,3:_                    =
#define PAD_GPIOB8  /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN  | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[5]          ,2: SDEX[5]             ,3: I2S2SDO             =
#define PAD_GPIOB9  /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[6]          ,2: SDEX[6]             ,3: I2S1SDI             =
#define PAD_GPIOB10 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP0_VD[7]          ,2: SDEX[7]             ,3: I2S2SDI             =
#define PAD_GPIOB11 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: CLE           ,1: CLE1                ,2: GPIO                ,3:_                    =
#define PAD_GPIOB12 /*NC*/   		(PAD_MODE_IN  | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: ALE           ,1: ALE1                ,2: GPIO                ,3:_                    =
#define PAD_GPIOB13 /*SD0*/    		(PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[0]    ,1: GPIO                ,2:_                    ,3:_                    =
#define PAD_GPIOB14 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_RnB      ,1: MCUS_RnB1           ,2: GPIO                ,3:_                    =
#define PAD_GPIOB15 /*SD1*/    		(PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[1]    ,1: GPIO                ,2:_                    ,3:_                    =
#define PAD_GPIOB16 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_nNOFE    ,1: MCUS_nNOFE1         ,2: GPIO                ,3:_                    =
#define PAD_GPIOB17 /*SD2*/    		(PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[2]    ,1: GPIO                ,2:_                    ,3:_                    =
#define PAD_GPIOB18 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_nNFWE    ,1: MCUS_nNFWE1         ,2: GPIO                ,3:_                    =
#define PAD_GPIOB19 /*SD3*/    		(PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[3]    ,1: GPIO                ,2:_                    ,3:_                    =
#define PAD_GPIOB20 /*SD4*/    		(PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[4]    ,1: GPIO                ,2:_                    ,3:_                    =
#define PAD_GPIOB21 /*SD5*/    		(PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[5]    ,1: GPIO                ,2:_                    ,3:_                    =
#define PAD_GPIOB22 /*SD6*/    		(PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[6]    ,1: GPIO                ,2:_                    ,3:_                    =
#define PAD_GPIOB23 /*SD7*/    		(PAD_MODE_ALT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[7]    ,1: GPIO                ,2:_                    ,3:_                    =
#define PAD_GPIOB24 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[8]    ,1: GPIO                ,2: MPEGTSI0_TDATA[0]   ,3:_                    =
#define PAD_GPIOB25 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[9]    ,1: GPIO                ,2: MPEGTSI0_TDATA[1]   ,3:_                    =
#define PAD_GPIOB26 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN  | PAD_STRENGTH_0)     // 0: MCUS_SD[10]   ,1: GPIO                ,2: MPEGTSI0_TDATA[2]   ,3: ECID_BONDING_ID[2]  =
#define PAD_GPIOB27 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN  | PAD_STRENGTH_0)     // 0: MCUS_SD[11]   ,1: GPIO                ,2: MPEGTSI0_TDATA[3]   ,3:_                    =
#define PAD_GPIOB28 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN  | PAD_STRENGTH_0)     // 0: MCUS_SD[12]   ,1: GPIO                ,2: MPEGTSI0_TDATA[4]   ,3: UART4_RXD           =
#define PAD_GPIOB29 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN  | PAD_STRENGTH_0)     // 0: MCUS_SD[13]   ,1: GPIO                ,2: MPEGTSI0_TDATA[5]   ,3: UART4_TXD           =
#define PAD_GPIOB30 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[14]   ,1: GPIO                ,2: MPEGTSI0_TDATA[6]   ,3: UART5_RXD           =
#define PAD_GPIOB31 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_SD[15]   ,1: GPIO                ,2: MPEGTSI0_TDATA[7]   ,3: UART5_TXD           =

/*------------------------------------------------------------------------------
 *	(GROUP_C)
 *
 *	0 bit           8 bit                   12 bit          16 bit              20 bit
 *	| PAD_MODE_XXX  | PAD_FUNC_ALT(0,1,2,3) | PAD_LEVEL_XXX | PAD_PULL_UP,OFF | PAD_STRENGTH_0,1,2,3
 *
 -----------------------------------------------------------------------------*/
#define PAD_GPIOC0  /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[0]  ,1: GPIO                ,2: MPEGTSI0_TSERR      ,3:_                    =
#define PAD_GPIOC1  /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[1]  ,1: GPIO                ,2: MPEGTSI1_TSERR      ,3:_                    =
#define PAD_GPIOC2  /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[2]  ,1: GPIO                ,2:_                    ,3:_                    =
#define PAD_GPIOC3  /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[3]  ,1: GPIO                ,2: HDMI_CEC            ,3: SDMMC0_nRST         =
#define PAD_GPIOC4  /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[4]  ,1: GPIO                ,2: UART1_DCD           ,3: SDMMC0_CARD_nint    =
#define PAD_GPIOC5  /*BT_UART_RTS_N*/    (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[5]  ,1: GPIO                ,2: UART1_CTS           ,3: SDMMC0_WP           =
#define PAD_GPIOC6  /*BT_UART_CTS_N*/    (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[6]  ,1: GPIO                ,2: UART1_RTS           ,3: SDMMC0_DETECT       =
#define PAD_GPIOC7  /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[7]  ,1: GPIO                ,2: UART1_DSR           ,3: SDMMC1_nRST         =
#define PAD_GPIOC8  /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[8]  ,1: GPIO                ,2: UART1_DTR           ,3: SDMMC1_CARD_nint    =
#define PAD_GPIOC9  /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[9]  ,1: GPIO                ,2: SSP2_CLK_IO         ,3: PDM_STROBE          =
#define PAD_GPIOC10 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[10] ,1: GPIO                ,2: SSP2_FSS            ,3: MCUS_nNCS[2]        =
#define PAD_GPIOC11 /*OTG_EN*/    	(PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[11] ,1: GPIO                ,2: SSP2_RXD            ,3: USB2.0OTG0_DRVBUS   =
#define PAD_GPIOC12 /*LCD_RESET*/  	(PAD_MODE_OUT | PAD_FUNC_ALT1 | PAD_LEVEL_HIGH | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[12] ,1: GPIO                ,2: SSP2_TXD            ,3: SDMMC2_nRST         =
#define PAD_GPIOC13 /*NC*/   	   	(PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[13] ,1: GPIO                ,2: PWM1_OUT            ,3: SDMMC2_CARD_nint    =
#define PAD_GPIOC14 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[14] ,1: GPIO                ,2: PWM2_OUT            ,3: VIP0_ExtCLK2        =
#define PAD_GPIOC15 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[15] ,1: GPIO                ,2: MPEGTSI0_TSCLK      ,3: VIP0_HSYNC2         =
#define PAD_GPIOC16 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[16] ,1: GPIO                ,2: MPEGTSI0_TSYNC0     ,3: VIP0_VSYNC2         =
#define PAD_GPIOC17 /*NC*/    		(PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_ADDR[17] ,1: GPIO                ,2: MPEGTSI0_TDP0       ,3: VIP0_VD2[0]         =
#define PAD_GPIOC18 /*MICRO_SD_CLK*/   (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: MCUS_ADDR[18] ,1: GPIO                ,2: SDMMC2_CCLK         ,3: VIP0_VD2[1]         =
#define PAD_GPIOC19 /*MICRO_SD_CMD*/  (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[19] ,1: GPIO                ,2: SDMMC2_CMD          ,3: VIP0_VD2[2]         =
#define PAD_GPIOC20 /*MICRO_SD_D0*/    (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[20] ,1: GPIO                ,2: SDMMC2_CDATA[0]     ,3: VIP0_VD2[3]         =
#define PAD_GPIOC21 /*MICRO_SD_D1*/    (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[21] ,1: GPIO                ,2: SDMMC2_CDATA[1]     ,3: VIP0_VD2[4]         =
#define PAD_GPIOC22 /*MICRO_SD_D2*/    (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[22] ,1: GPIO                ,2: SDMMC2_CDATA[2]     ,3: VIP0_VD2[5]         =
#define PAD_GPIOC23 /*MICRO_SD_D3*/    (PAD_MODE_ALT | PAD_FUNC_ALT2 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: MCUS_ADDR[23] ,1: GPIO                ,2: SDMMC2_CDATA[3]     ,3: VIP0_VD2[6]         =
#define PAD_GPIOC24 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_LATADDR  ,1: GPIO                ,2: SPDIFIN             ,3: VIP0_VD2[7]         =
#define PAD_GPIOC25 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_nSWAIT   ,1: GPIO                ,2: SPDIF_DATA          ,3:_                    =
#define PAD_GPIOC26 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_RDnWR    ,1: GPIO                ,2: PDM_DATA0           ,3:_                    = DM9000 (IRQ)
#define PAD_GPIOC27 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_nSDQM1   ,1: GPIO                ,2: PDM_DATA1           ,3:_                    =
#define PAD_GPIOC28 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: MCUS_nSCS[1]        ,2: UART1_TRI           ,3:_                    = DM9000 (ETHERNET)
#define PAD_GPIOC29 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: SSP0_CLKIO          ,2:_                    ,3:_                    =
#define PAD_GPIOC30 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: SSP0_FSS            ,2:_                    ,3:_                    =
#define PAD_GPIOC31 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: SSP0_TXD            ,2:_                    ,3:_                    =

/*------------------------------------------------------------------------------
 *	(GROUP_D)
 *
 *	0 bit           8 bit                   12 bit          16 bit              20 bit
 *	| PAD_MODE_XXX  | PAD_FUNC_ALT(0,1,2,3) | PAD_LEVEL_XXX | PAD_PULL_UP,OFF | PAD_STRENGTH_0,1,2,3
 *
 -----------------------------------------------------------------------------*/
#define PAD_GPIOD0  /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: SSP0_RXD            ,2: PWM3_OUT            ,3:_                    =
#define PAD_GPIOD1  /*LCD_BL_PWM*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: PWM0_OUT            ,2: MCUS_ADDR[25]       ,3:_                    =
#define PAD_GPIOD2  /*SCL0*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: I2C0_SCL            ,2: UART4_SMCAYEN       ,3:_                    =
#define PAD_GPIOD3  /*SDA0*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_1)     // 0: GPIO          ,1: I2C0_SDA            ,2: UART5_SMCAYEN       ,3:_                    =
#define PAD_GPIOD4  /*SCL1*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2C1_SCL            ,2:_                    ,3:_                    =
#define PAD_GPIOD5  /*SDA1*/   (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2C1_SDA            ,2:_                    ,3:_                    =
#define PAD_GPIOD6  /*SCL2*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2C2_SCL            ,2:_                    ,3:_                    =
#define PAD_GPIOD7  /*SDA2*/   (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2C2_SDA            ,2:_                    ,3:_                    =
#define PAD_GPIOD8  /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: PPM_IN              ,2:_                    ,3:_                    =
#define PAD_GPIOD9  /*SDOUT*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2S0_SDO            ,2: AC97_ACSDATAOUT     ,3:_                    =
#define PAD_GPIOD10 /*BCLK*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2S0_BCLK           ,2: AC97_ACBITCLK       ,3:_                    =
#define PAD_GPIOD11 /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2S0_SDI            ,2: AC97_ACSDATAIN      ,3:_                    =
#define PAD_GPIOD12 /*LRCK*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2S0_LRCLK          ,2: AC97_ACSYNC         ,3:_                    =
#define PAD_GPIOD13 /*MCLK*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: I2S0_CODCLK         ,2: AC97_nACRESET       ,3:_                    =
#define PAD_GPIOD14 /*DEBUG_RX*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART0RXD            ,2: UART1_SMCAYEN       ,3:_                    =
#define PAD_GPIOD15 /*BT_UART_TXD*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART1RXD            ,2: UART2_SMCAYEN       ,3:_                    =
#define PAD_GPIOD16 /*OTG_FAULT*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART2RXD            ,2: CAN0_TX             ,3:_                    =
#define PAD_GPIOD17 /*OTG_5V_EN*/    (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART3RXD            ,2: CAN1_TX             ,3:_                    =
#define PAD_GPIOD18 /*DEBUG_TX*/     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART0TXD            ,2:_                    ,3: SDnCD2              =
#define PAD_GPIOD19 /*BT_UART_RXD*/  (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART1TXD            ,2:_                    ,3:_                    =
#define PAD_GPIOD20 /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART2TXD            ,2:_                    ,3:_                    =
#define PAD_GPIOD21 /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: UART3TXD            ,2:_                    ,3:_                    =
#define PAD_GPIOD22 /*WL_SDIO_CLK*/    (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC1_CCLK         ,2:_                    ,3:_                    =
#define PAD_GPIOD23 /*WL_SDIO_CMD*/   (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC1_CMD          ,2:_                    ,3:_                    =
#define PAD_GPIOD24 /*WL_SDIO_D0*/     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC1_CDATA[0]     ,2:_                    ,3:_                    =
#define PAD_GPIOD25 /*WL_SDIO_D1*/     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC1_CDATA[1]     ,2:_                    ,3:_                    =
#define PAD_GPIOD26 /*WL_SDIO_D2*/     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC1_CDATA[3]     ,2:_                    ,3:_                    =
#define PAD_GPIOD27 /*WL_SDIO_D3*/     (PAD_MODE_ALT | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          ,1: SDMMC1_CDATA[3]     ,2:_                    ,3:_                    =
#define PAD_GPIOD28 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP1_VD[0]          ,2: MPEGTSI_TDATA1[0]   ,3: MCUS_ADDR[24]       =
#define PAD_GPIOD29 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP1_VD[1]          ,2: MPEGTSI_TDATA1[1]   ,3:_                    =
#define PAD_GPIOD30 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP1_VD[2]          ,2: MPEGTSI_TDATA1[2]   ,3:_                    =
#define PAD_GPIOD31 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          ,1: VIP1_VD[3]          ,2: MPEGTSI_TDATA1[3]   ,3:_                    =

/*------------------------------------------------------------------------------
 *	(GROUP_E)
 *
 *	0 bit           8 bit                   12 bit          16 bit              20 bit
 *	| PAD_MODE_XXX  | PAD_FUNC_ALT(0,1,2,3) | PAD_LEVEL_XXX | PAD_PULL_UP,OFF | PAD_STRENGTH_0,1,2,3
 * 
 -----------------------------------------------------------------------------*/
#define PAD_GPIOE0  /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: VIP1_VD[4]           ,2: MPEGTSI_TDATA1[0]   ,3:_                    =
#define PAD_GPIOE1  /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: VIP1_VD[5]           ,2: MPEGTSI_TDATA1[0]   ,3:_                    =
#define PAD_GPIOE2  /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: VIP1_VD[6]           ,2: MPEGTSI_TDATA1[0]   ,3:_                    =
#define PAD_GPIOE3  /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: VIP1_VD[7]           ,2: MPEGTSI_TDATA1[0]   ,3:_                    =
#define PAD_GPIOE4  /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: VIP1_ExtCLK          ,2: MPEGTSI_TCLK1       ,3:_                    =
#define PAD_GPIOE5  /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: VIP1_HSYNC           ,2: MPEGTSI_TSYNC1      ,3:_                    =
#define PAD_GPIOE6  /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: VIP_VSYNC            ,2: MPEGTSI_TDP1        ,3:_                    =
#define PAD_GPIOE7  /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_TXD[0]     ,2: VIP0_Ext_VSYNC      ,3:_                    =
#define PAD_GPIOE8  /*CHG_DET*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_TXD[1]     ,2:_                    ,3:_                    =
#define PAD_GPIOE9  /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_TXD[2]     ,2:_                    ,3:_                    =
#define PAD_GPIOE10 /*TOUCH_RST*/    (PAD_MODE_OUT | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_TXD[3]     ,2:_                    ,3:_                    =
#define PAD_GPIOE11 /*TOUCH_INT#*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_TXEN       ,2:_                    ,3:_                    =
#define PAD_GPIOE12 /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_TXER       ,2:_                    ,3:_                    = Backlight Enable
#define PAD_GPIOE13 /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_COL        ,2: VIP0_Ext_HSYNC      ,3:_                    =
#define PAD_GPIOE14 /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_RXD[0]     ,2: SSP1_CLKIO          ,3:_                    =
#define PAD_GPIOE15 /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_RXD[1]     ,2: SSP1_FSS            ,3:_                    =
#define PAD_GPIOE16 /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_RXD[2]     ,2:_                    ,3:_                    = VG_EN
#define PAD_GPIOE17 /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_DN  | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_RXD[3]     ,2:_                    ,3:_                    =
#define PAD_GPIOE18 /*ENCODER_-*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_CLK_RX         ,2: SSP1_RXD            ,3:_                    =
#define PAD_GPIOE19 /*ENCODER_+*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_RX_DV      ,2: SSP1_TXD            ,3:_                    =
#define PAD_GPIOE20 /*BT_SLEEP*/       (PAD_MODE_OUT  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_GMII_MDC       ,2:_                    ,3:_                    =
#define PAD_GPIOE21 /*LOW_BATT*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW | PAD_PULL_DN  | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_GMII_MDI       ,2:_                    ,3:_                    =
#define PAD_GPIOE22 /*NC*/    (PAD_MODE_IN | PAD_FUNC_ALT0 | PAD_LEVEL_LOW | PAD_PULL_DN  | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_RXER       ,2:_                    ,3:_                    =
#define PAD_GPIOE23 /*FF*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_PHY_CRS        ,2:_                    ,3:_                    =
#define PAD_GPIOE24 /*REV*/    (PAD_MODE_IN  | PAD_FUNC_ALT0 | PAD_LEVEL_LOW  | PAD_PULL_UP  | PAD_STRENGTH_0)     // 0: GPIO          1: GMAC0_GTX_CLK        ,2:_                    ,3:_                    =
#define PAD_GPIOE25 /*NTRST*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: nTRST         1: GPIO                 ,2:_                    ,3:_                    =
#define PAD_GPIOE26 /*TMS/*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: TMS           1: GPIO                 ,2:_                    ,3:_                    =
#define PAD_GPIOE27 /*TDI*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: TDI           1: GPIO                 ,2:_                    ,3:_                    =
#define PAD_GPIOE28 /*TCLK*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: TCLK          1: GPIO                 ,2:_                    ,3:_                    =
#define PAD_GPIOE29 /*TDO*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_OFF | PAD_STRENGTH_0)     // 0: TDO           1: GPIO                 ,2:_                    ,3:_                    =
#define PAD_GPIOE30 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_nSOE     1: GPIO                 ,2:_                    ,3:_                    =
#define PAD_GPIOE31 /*NC*/    (PAD_MODE_IN  | PAD_FUNC_ALT1 | PAD_LEVEL_LOW  | PAD_PULL_DN | PAD_STRENGTH_0)     // 0: MCUS_nSWE     1: GPIO                 ,2:_                    ,3:_                    =

/*------------------------------------------------------------------------------
 *	(GROUPALV)
 *	0                     4                             8        12
 *	| MODE(IN/OUT/DETECT) | ALIVE OUT or ALIVE DETMODE0 | PullUp |
 *
 -----------------------------------------------------------------------------*/
#define PAD_GPIOALV0    (PAD_MODE_IN | PAD_LEVEL_LOW  | PAD_PULL_UP )	//
#define PAD_GPIOALV1    (PAD_MODE_IN | PAD_LEVEL_LOW  | PAD_PULL_UP )	//
#define PAD_GPIOALV2    (PAD_MODE_IN | PAD_LEVEL_LOW  | PAD_PULL_UP )	//
#define PAD_GPIOALV3    (PAD_MODE_IN | PAD_LEVEL_LOW  | PAD_PULL_UP )	//
#define PAD_GPIOALV4    (PAD_MODE_IN | PAD_LEVEL_LOW  | PAD_PULL_UP )	//
#define PAD_GPIOALV5    (PAD_MODE_IN | PAD_LEVEL_LOW  | PAD_PULL_UP )	//


#else

// other lcd

#endif

#endif	/* __CFG_GPIO_H__ */

