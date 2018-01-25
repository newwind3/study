/*
 * (C) Copyright 2009
 * jung hyun kim, Nexell Co, <jhkim@nexell.co.kr>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <linux/delay.h>

//#define MIPI_BITRATE_345M		// 480 * 800   ST7701
//#define MIPI_BITRATE_330M		// 480 * 800   RM68172
#ifdef ENABLE_ACTIVO
#define	MIPI_BITRATE_396M	
#else
// other lcd
#endif

#ifdef MIPI_BITRATE_1G
#define	PLLPMS		0x33E8
#define	BANDCTL		0xF
#elif defined(MIPI_BITRATE_900M)
#define	PLLPMS		0x2258
#define	BANDCTL		0xE
#elif defined(MIPI_BITRATE_840M)
#define	PLLPMS		0x2230
#define	BANDCTL		0xD
#elif defined(MIPI_BITRATE_750M)
#define	PLLPMS		0x43E8
#define	BANDCTL		0xC
#elif defined(MIPI_BITRATE_660M)
#define	PLLPMS		0x21B8
#define	BANDCTL		0xB
#elif defined(MIPI_BITRATE_600M)
#define	PLLPMS		0x2190
#define	BANDCTL		0xA
#elif defined(MIPI_BITRATE_540M)
#define	PLLPMS		0x2168
#define	BANDCTL		0x9
#elif defined(MIPI_BITRATE_512M)
#define	PLLPMS		0x03200
#define	BANDCTL		0x9
#elif defined(MIPI_BITRATE_480M)
#define	PLLPMS		0x2281
#define	BANDCTL		0x8
#elif defined(MIPI_BITRATE_420M)
#define	PLLPMS		0x2231
#define	BANDCTL		0x7
#elif defined(MIPI_BITRATE_402M)
#define	PLLPMS		0x2219
#define	BANDCTL		0x7
#elif defined(MIPI_BITRATE_396M) //yys-
#define	PLLPMS		0x2211	//10(P) 001000010(M) 001(S) // 0x2211  PLL = (M*24Mhz)/(P*2^S) = 66*24/(2*2)=396MHz
#define	BANDCTL		0x7
#elif defined(MIPI_BITRATE_345M)
#define	PLLPMS		0x21E9
#define	BANDCTL		0x6
#elif defined(MIPI_BITRATE_330M)
#define	PLLPMS		0x21B9
#define	BANDCTL		0x6
#elif defined(MIPI_BITRATE_300M)
#define	PLLPMS		0x2191
#define	BANDCTL		0x5
#elif defined(MIPI_BITRATE_210M)
#define	PLLPMS		0x2232
#define	BANDCTL		0x4
#elif defined(MIPI_BITRATE_200M)
#define	PLLPMS		0x3643
#define	BANDCTL		0x4
#elif defined(MIPI_BITRATE_180M)
#define	PLLPMS		0x21E2
#define	BANDCTL		0x3
#elif defined(MIPI_BITRATE_170M)
#define	PLLPMS		0x6AA3
#define	BANDCTL		0x6
#elif defined(MIPI_BITRATE_150M)
#define	PLLPMS		0x2192
#define	BANDCTL		0x2
#elif defined(MIPI_BITRATE_100M)
#define	PLLPMS		0x3323
#define	BANDCTL		0x1
#elif defined(MIPI_BITRATE_80M)
#define	PLLPMS		0x3283
#define	BANDCTL		0x0
#endif

#define	PLLCTL		0
#define	DPHYCTL		0

#define MIPI_DELAY 0xFF
#define MIPI_PARAM_MAX_LENGTH		48
//#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

struct data_val{
	u8 data[MIPI_PARAM_MAX_LENGTH];
};

struct mipi_reg_val {
	u32 cmd;
	u32 addr;
	u32 cnt;
	struct data_val data;
};

static struct mipi_reg_val mipi_init_data[]=
{
		{0x15, 0x36, 1,{0x00}}, //set_addr_mode
		{0x15, 0x3A, 1,{0x70}}, //set_pixel
		{0x29, 0xB0, 1,{0x04}}, //pannel_manufacture_access		
		{0x29, 0xB3, 2,{0x00, 0x80}}, //pannel_pixel_format
		{0x29, 0xB6, 2,{0x32, 0x83}}, //pannel_dsi_control
		{0x29, 0xC0, 6,{0x01, 0x56, 0x65, 0x00, 0x00, 0x00}}, //pannel_driving_set
		{0x29, 0xC1, 5,{0x00, 0x10, 0x00, 0x01, 0x12}},	//pannel_V_timimg_set
		{0x29, 0xC3, 2,{0x00, 0x05}}, 		//pannel_control_set
		{0x29, 0xC4, 1,{0x03}}, 		//pannel_ltps_mode
		{0x29, 0xC5,14,{0x40, 0x01, 0x06, 0x01, 0x76, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x1E, 0x04, 0x00, 0x00}}, 		//pannel_H_timming_set
		{0x29, 0xC8, 7,{0x0D, 0x1B, 0x19, 0x1C, 0x20, 0x15, 0x00}}, //pannel_gamma_setA
		{0x29, 0xC9, 7,{0x0D, 0x1F, 0x1F, 0x1E, 0x22, 0x17, 0x00}}, //pannel_gamma_setB		
		{0x29, 0xCA, 7,{0xCD, 0x54, 0x4E, 0x0F, 0x17, 0x14, 0x00}}, //pannel_gamma_setC
		{0x29, 0xD0, 4,{0x22, 0x24, 0xA3, 0xB8}}, //pannel_power_set_CP
		{0x29, 0xD1, 5,{0x00, 0x00, 0x00, 0x00, 0x00}}, //pannel_test_mode1
		{0x29, 0xD2, 2,{0xB3, 0x00}}, //pannel_power_set_SA
		{0x29, 0xD3, 2,{0x33, 0x03}}, //pannel_power_set_IPSC
		{0x29, 0xD5, 1,{0x06}}, //pannel_vreg_set	
		{0x29, 0xD6, 1,{0x01}}, //pannel_test_mode2	
		{0x29, 0xD7, 7,{0x09, 0x00, 0x84, 0x81, 0x61, 0xBC, 0xB5}}, //pannel_sequencer_timming_control_power_on
		{0x29, 0xD8, 6,{0x04, 0x25, 0x90, 0x4C, 0x92, 0x00}}, //pannel_sequencer_timming_control_power_off		
		{0x29, 0xD9, 3,{0x5B, 0x7F, 0x00}}, //pannel_power_on_off
		{0x29, 0xDD, 1,{0x3C}}, //pannel_vcs_set
		{0x29, 0xDE, 1,{0x42}}, //pannel_vcomdc_set
		{0x29, 0xB0, 1,{0x03}}, //pannel_manufacture_access_protect
    {0x05, 0x00, 0,{0x11}}, // exit_sleep_mode
    {0xFF, 0x78, 0,{0x00}}, // delay 120
    {0x05, 0x00, 0,{0x29}}, // set_diaplay_on
    {0xFF, 0x05, 0,{0x00}}, // delay 5 
};

static void  mipilcd_dcs_long_write(U32 cmd, U32 ByteCount, U8* pByteData )
{
	U32 DataCount32 = (ByteCount+3)/4;
	int i = 0;
	U32 index = 0;
	volatile NX_MIPI_RegisterSet* pmipi = (volatile NX_MIPI_RegisterSet*)IO_ADDRESS(NX_MIPI_GetPhysicalAddress(index));

	NX_ASSERT( 512 >= DataCount32 );

#if 0
	printk("0x%02x %2d: ", cmd, ByteCount);
	for(i=0; i< ByteCount; i++)
		printk("%02x ", pByteData[i]);
	printk("\n");
#endif

	for( i=0; i<DataCount32; i++ )
	{
		pmipi->DSIM_PAYLOAD = (pByteData[3]<<24)|(pByteData[2]<<16)|(pByteData[1]<<8)|pByteData[0];
		pByteData += 4;
	}
	pmipi->DSIM_PKTHDR  = (cmd & 0xff) | (ByteCount<<8);
}

static void mipilcd_dcs_write( unsigned int id, unsigned int data0, unsigned int data1 )
{
	U32 index = 0;
	volatile NX_MIPI_RegisterSet* pmipi = (volatile NX_MIPI_RegisterSet*)IO_ADDRESS(NX_MIPI_GetPhysicalAddress(index));

#if 0
	switch(id)
	{
		case 0x05:
			printk("0x05  1: %02x \n", data0);
			break;
		case 0x13:
			printk("0x13  2: %02x %02x \n", data0, data1);
			break;
		case 0x15:
			printk("0x15  2: %02x %02x \n", data0, data1);
			break;
	}
#endif
    printk("0x%08x, ", id | (data0 << 8) | (data1 << 16));
	pmipi->DSIM_PKTHDR = id | (data0<<8) | (data1<<16);
}

void lcd_reset(void)
{

	NX_GPIO_SetPadFunction( PAD_GET_GROUP(CFG_MIPI_LCD_RESET), PAD_GET_BITNO(CFG_MIPI_LCD_RESET), PAD_GET_FUNC(CFG_MIPI_LCD_RESET));
	NX_GPIO_SetOutputEnable(PAD_GET_GROUP(CFG_MIPI_LCD_RESET), PAD_GET_BITNO(CFG_MIPI_LCD_RESET), 1);
	NX_GPIO_SetOutputValue( PAD_GET_GROUP(CFG_MIPI_LCD_RESET), PAD_GET_BITNO(CFG_MIPI_LCD_RESET), 0);
	mdelay(10);
	NX_GPIO_SetOutputValue( PAD_GET_GROUP(CFG_MIPI_LCD_RESET), PAD_GET_BITNO(CFG_MIPI_LCD_RESET), 1);
	mdelay(10);
}
EXPORT_SYMBOL(lcd_reset);
static int MIPI_LCD_INIT(int width, int height, void *data)
{
	int i=0;
	int size=ARRAY_SIZE(mipi_init_data);
	u32 index = 0;
	u32 value = 0;
	u8 pByteData[MIPI_PARAM_MAX_LENGTH + 32];
	u8 bitrate = BANDCTL;

	volatile NX_MIPI_RegisterSet* pmipi = (volatile NX_MIPI_RegisterSet*)IO_ADDRESS(NX_MIPI_GetPhysicalAddress(index));
	value = pmipi->DSIM_ESCMODE;
	pmipi->DSIM_ESCMODE = value | (3 << 6);
	value = pmipi->DSIM_ESCMODE;
    mdelay(10);

	//printk("DSIM_ESCMODE : 0x%x\n", value);

	// LCD RESET
	lcd_reset();

	for(i=0; i<size; i++)
	{
		switch(mipi_init_data[i].cmd)
		{
			case 0x05:
				mipilcd_dcs_write(mipi_init_data[i].cmd, mipi_init_data[i].data.data[0], 0x00);
				break;

			case 0x13:
				mipilcd_dcs_write(mipi_init_data[i].cmd, mipi_init_data[i].addr, mipi_init_data[i].data.data[0]);
				break;

			case 0x15:
				mipilcd_dcs_write(mipi_init_data[i].cmd, mipi_init_data[i].addr, mipi_init_data[i].data.data[0]);
				break;
#if 1
 			case 0x23:
				mipilcd_dcs_write(mipi_init_data[i].cmd, mipi_init_data[i].addr, mipi_init_data[i].data.data[0]);
				break;
				
 			case 0x29:
				pByteData[0] = mipi_init_data[i].addr;
				memcpy(&pByteData[1], &mipi_init_data[i].data.data[0], MIPI_PARAM_MAX_LENGTH);
				mipilcd_dcs_long_write(mipi_init_data[i].cmd, mipi_init_data[i].cnt+1, &pByteData[0]);
				break;
#endif
 			case 0x39:
				pByteData[0] = mipi_init_data[i].addr;
				memcpy(&pByteData[1], &mipi_init_data[i].data.data[0], MIPI_PARAM_MAX_LENGTH);
				mipilcd_dcs_long_write(mipi_init_data[i].cmd, mipi_init_data[i].cnt+1, &pByteData[0]);
				break;

			case MIPI_DELAY:
				//printk("delay %d\n", mipi_init_data[i].addr);
				mdelay(mipi_init_data[i].addr);
				break;
		}
		mdelay(1);
	}

	value = pmipi->DSIM_ESCMODE;
	pmipi->DSIM_ESCMODE = value & (~(3 << 6));
	value = pmipi->DSIM_ESCMODE;
	//printk("DSIM_ESCMODE : 0x%x\n", value);

	//mdelay(10);
	return 0;
}
