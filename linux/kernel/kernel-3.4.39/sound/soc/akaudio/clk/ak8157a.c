/*
 * ak8157a.c  --  AK8157A S/PDIF transceiver driver
 *
 * Copyright 2017 Iriver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>
#include <mach/platform.h>

#include <akaudio.h>

#define AK8157A_NAME "ak8157a"

static struct akaudio_reg g_ak8157a_reg[] =
{
	{0,  0b00001111},
	{1,  0b00100010},
};

struct akaudio_reg*ak8157a_regs(uint32_t *count)
{
	*count = sizeof(g_ak8157a_reg) / sizeof(struct akaudio_reg);
	return g_ak8157a_reg;
}

static inline int read_reg(struct i2c_client *client, u8 reg)
{
	return i2c_smbus_read_byte_data(client, reg);
}

static inline int write_reg(struct i2c_client *client, u8 reg, u8 value)
{
	return i2c_smbus_write_byte_data(client, reg, value);
}

int ak8157a_write_reg(unsigned int reg, unsigned int val)
{
	if (akaudio_get_i2c_client() == NULL) return -1;
	return write_reg(akaudio_get_i2c_client(),reg,val);
}

unsigned int ak8157a_read_reg(unsigned char reg)
{
	if (akaudio_get_i2c_client() == NULL) return -1;
	return read_reg(akaudio_get_i2c_client(), reg);
}

static void ak8157a_reset(void)
{
	akaudio_reset_pll();
}

void ak8157a_set_samplerate(uint32_t rate, uint32_t format)
{
	unsigned int val;
	
	ak8157a_reset();

	ak8157a_write_reg(0x0, 0x00);

	printk("ak8157: rate %d format %d\n", rate, format);

	if(rate <= 44100)
		val = 0x01;
	else if(rate == 48000)
		val = 0x42;
	else if(rate == 88200)
		val = 0x11;
	else if(rate == 96000)
		val = 0x12;
	else if(rate == 176400)
		val = 0x21;
	else if(rate == 192000)
		val = 0x22;
	else if(rate == 352000)
		val = 0x31;
	else if(rate == 384000)
		val = 0x32;
	else
		val = 0x01;

	ak8157a_write_reg(0x1, val);
}

static unsigned int ak8157a_read(struct snd_soc_codec *codec, unsigned int reg)
{
	struct i2c_client *i2c =  codec->control_data;

	return read_reg(i2c, reg);
}


static int ak8157a_probe(struct snd_soc_codec *codec)
{
	printk("%s(%d)\n", __func__, __LINE__);

	codec->hw_read = ak8157a_read;	
	codec->hw_write = (hw_write_t)i2c_master_send;

//	ak_set_hw_mute(AMT_DAC_MUTE,1);
//	ak_set_hw_mute(AMT_HP_MUTE,1);

	return 0;
}

#define AK8157A_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | \
			SNDRV_PCM_FMTBIT_S24_LE)

#define AK8157A_RATES (SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_44100 | \
		      SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_64000 | \
		      SNDRV_PCM_RATE_88200 | SNDRV_PCM_RATE_96000 | \
		      SNDRV_PCM_RATE_176400 | SNDRV_PCM_RATE_192000)

static struct snd_soc_dai_driver ak8157a_dai_drv = {
	.name = "ak8157a",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 2,
		.channels_max = 2,
		.rates = AK8157A_RATES,
		.formats = AK8157A_FORMATS,
	},
	.capture = {
		.stream_name = "Capture",
		.channels_min = 2,
		.channels_max = 2,
		.rates = AK8157A_RATES,
		.formats = AK8157A_FORMATS,
	},
	.ops = NULL,
	.symmetric_rates = 1
};

struct snd_soc_dai_driver* ak8157a_get_dai_driver(void)
{
	return &ak8157a_dai_drv;
}

static struct snd_soc_codec_driver ak8157a_soc_codec_dev = {
	.probe = ak8157a_probe,
};

struct snd_soc_codec_driver* ak8157a_get_codec_driver(void)
{
	return &ak8157a_soc_codec_dev;
}

static const struct of_device_id ak8157a_of_match[] = {
	{ .compatible = "ak,ak8157a", },
	{ }
};
MODULE_DEVICE_TABLE(of, ak8157a_of_match);

void ak8157a_reg_init(void)
{
	int i;
	
	for(i = 0; i < ARRAY_SIZE(g_ak8157a_reg); i++) {
		ak8157a_write_reg(g_ak8157a_reg[i].reg, g_ak8157a_reg[i].value);
	}
}

static int ak8157a_detect(struct i2c_client *client,
			  struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;
	int val1, val2;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA)) {
		return -ENODEV;
	}

	val1 = i2c_smbus_read_byte_data(client, 0x00);
	val2 = i2c_smbus_read_byte_data(client, 0x01);

	if (val1 != 0xe0) {	
		return -ENODEV;
	}

	printk("%s(i2c_add:0x%x [00H]0x%x, [01H]0x%x)\n", __FUNCTION__, client->addr, val1, val2);

	strlcpy(info->type, AK8157A_NAME, I2C_NAME_SIZE);

	return 0;
}

static __devexit int ak8157a_i2c_remove(struct i2c_client *i2c)
{
	snd_soc_unregister_codec(&i2c->dev);

	return 0;
}

static const struct i2c_device_id ak8157a_i2c_id[] = {
	{ AK8157A_NAME, 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, ak8157a_i2c_id);

/* Addresses to scan */
static const unsigned short normal_i2c[] = { 0x10, 0x11, 0x12, 0x13,
						I2C_CLIENT_END };

static struct i2c_driver ak8157a_i2c_driver = {
	.driver = {
		.name = AK8157A_NAME,
		.owner = THIS_MODULE,
		.of_match_table = ak8157a_of_match,
	},
	.class			= I2C_CLASS_HWMON, //i2c_detect
	.probe 			= NULL,
	.remove 		= __devexit_p(ak8157a_i2c_remove),
	.address_list	= normal_i2c,
	.detect			= ak8157a_detect, //i2c_detect
	.id_table 		= ak8157a_i2c_id
};

struct i2c_driver *ak8157a_get_i2c_driver(void)
{
	return &ak8157a_i2c_driver;
}

static struct snd_soc_dai_link ak8157a_dai[] = {
    {
        .name 			= "ak8157a",
        .stream_name 	= "ak8157a",
        .platform_name  = "nxp-pcm",
        .cpu_dai_name   = "nxp-i2s",

        .codec_name 	= "ak8157a.0-0010",
        .codec_dai_name = "ak8157a",
    },
};

struct snd_soc_dai_link *ak8157a_get_dai_link(int *count)
{
	*count = ARRAY_SIZE(ak8157a_dai);
	return ak8157a_dai;
}

void ak8157a_set_ops(struct akaudio_clk_ops*ops)
{
	ops->set_samplerate = ak8157a_set_samplerate;
}

MODULE_DESCRIPTION("ak8157a driver for Astell&Kern audio module");
MODULE_LICENSE("GPL");
