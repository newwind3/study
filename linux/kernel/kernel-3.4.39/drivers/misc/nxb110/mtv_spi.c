/*
 * mtv_spi.c
 *
 * NXB110 MTV spi driver.
 *
 * Copyright (C) (2015, NEXELL)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "src/nxb110tv.h"
#include "src/nxb110tv_internal.h"

#include "mtv.h"


#ifdef RTV_IF_SPI

#define SPI_DEV_NAME_0	"mtvspi"
//#define SPI_DEV_NAME_0	"mtvspi"
//#define SPI_DEV_NAME_1	"mtvspi_1"


/* Defines if cs_change of struct spi_transfer was supported in Linux SPI driver. */
//#define MTV_SPI_CS_CHANGE_FLAG_SUPPORTED

void mtv_spi_read_burst(int demod_no, unsigned char reg, unsigned char *buf, int size)
{
	int ret;
	u8 out_buf[2], read_out_buf[2];
	struct spi_message msg;
	struct spi_transfer msg_xfer0 = {
		.tx_buf = out_buf,
		.len		= 2,
#ifdef MTV_SPI_CS_CHANGE_FLAG_SUPPORTED
		.cs_change	= 1,
#else
		.cs_change	= 0,
#endif
		.delay_usecs = 0,
	};
	
	struct spi_transfer msg_xfer1 = {
		.tx_buf = read_out_buf,
		.rx_buf = buf,
		.len		= size,
		.cs_change	= 0,
		.delay_usecs = 0,
	};

	spi_message_init(&msg);
	out_buf[0] = NXB110TV_CHIP_ADDR;
	out_buf[1] = reg;	
	spi_message_add_tail(&msg_xfer0, &msg);
#ifndef MTV_SPI_CS_CHANGE_FLAG_SUPPORTED
	ret = spi_sync(mtv_cb_ptr->spi_ptr, &msg);
	if (ret)
	{
		DMBERR("error: %d\n", ret);	
	}

	spi_message_init(&msg);
#endif
	read_out_buf[0] = NXB110TV_CHIP_ADDR|0x1;
	spi_message_add_tail(&msg_xfer1, &msg);
	ret = spi_sync(mtv_cb_ptr->spi_ptr, &msg);
	if (ret)
	{
		DMBERR("error: %d\n", ret);	
	}
}

unsigned char mtv_spi_read(int demod_no, unsigned char reg)
{
	int ret;
	u8 out_buf[2], read_out_buf[2];
	u8 in_buf[2];
	struct spi_message msg;
	struct spi_transfer msg_xfer0 = {
		.tx_buf = out_buf,
		.len		= 2,
#ifdef MTV_SPI_CS_CHANGE_FLAG_SUPPORTED
		.cs_change	= 1,
#else
		.cs_change	= 0,
#endif
		.delay_usecs = 0,
	};
	
	struct spi_transfer msg_xfer1 = {
		.tx_buf = read_out_buf,
		.rx_buf = in_buf,
		.len		= 2,
		.cs_change	= 0,
		.delay_usecs = 0,
	};

	spi_message_init(&msg);
	out_buf[0] = NXB110TV_CHIP_ADDR;
	out_buf[1] = reg;	
	spi_message_add_tail(&msg_xfer0, &msg);
#ifndef MTV_SPI_CS_CHANGE_FLAG_SUPPORTED
	ret = spi_sync(mtv_cb_ptr->spi_ptr, &msg);
	if (ret)
	{
		DMBERR("error: %d\n", ret);  
		return 0xFF;
	}

	spi_message_init(&msg);
#endif
	read_out_buf[0] = NXB110TV_CHIP_ADDR|0x1;
	spi_message_add_tail(&msg_xfer1, &msg);
	ret = spi_sync(mtv_cb_ptr->spi_ptr, &msg);
	if (ret)
	{
		DMBERR("error: %d\n", ret);  
		return 0xFF;
	}

	return in_buf[1];
}


void mtv_spi_write(int demod_no, unsigned char reg, unsigned char val)
{
	u8 out_buf[3];
	u8 in_buf[3];
	struct spi_message msg;
	struct spi_transfer msg_xfer = {
		.len		= 3,
		.cs_change	= 0,
		.delay_usecs = 0,
	};
	int ret;

	spi_message_init(&msg);

	out_buf[0] = NXB110TV_CHIP_ADDR;
	out_buf[1] = reg;
	out_buf[2] = val;

	msg_xfer.tx_buf = out_buf;
	msg_xfer.rx_buf = in_buf;
	spi_message_add_tail(&msg_xfer, &msg);

	ret = spi_sync(mtv_cb_ptr->spi_ptr, &msg);
	if (ret)
	{
		DMBERR("error: %d\n", ret);  
	}
}


static int mtv_spi_probe_0(struct spi_device *spi)
{
	int ret;
	
	DMBMSG("MTV spi 0 ENTERED!!!!!!!!!!!!!!\n");
	
	spi->bits_per_word = 8;
//	spi->mode = SPI_MODE_0;
	printk(" MTV spi... entered mode(%d)!!! \n", spi->mode);
	
	ret = spi_setup(spi);
	if (ret < 0)
	       return ret;

	mtv_cb_ptr->spi_ptr = spi;

	return 0;
}


static int mtv_spi_remove(struct spi_device *spi)
{
	return 0;
}


struct spi_driver mtv_spi_driver_0 = {
	.driver = {
		.name = SPI_DEV_NAME_0,
		.owner = THIS_MODULE,
	},

	.probe    = mtv_spi_probe_0,
	.suspend	= NULL,
	.resume 	= NULL,
	.remove	= __devexit_p(mtv_spi_remove),
};

#endif /* #ifdef RTV_IF_SPI */
