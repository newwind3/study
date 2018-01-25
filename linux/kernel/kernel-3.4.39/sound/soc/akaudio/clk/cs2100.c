/*
 * cs2100.c  --  CS2100 S/PDIF transceiver driver
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

#define CS2100_NAME "cs2100"

/////////////////////////////////////////////////////////////////////
// Register Addresses
//
#define CS2100_DEVID        0x01    /* Device ID and Revision */
#define CS2100_DEVCTL       0x02    /* Device Control */
#define CS2100_DEVCFG1      0x03    /* Device Configuration 1 */
#define CS2100_GBLCFG       0x05    /* Global Configuration */
#define CS2100_RATIO0       0x06    /* Ratio: Bits 24-31 */
#define CS2100_RATIO1       0x07    /* Ratio: Bits 16-23 */
#define CS2100_RATIO2       0x08    /* Ratio: Bits  8-15 */
#define CS2100_RATIO3       0x09    /* Ratio: Bits  0- 7 */
#define CS2100_FNCCFG1      0x16    /* Function Configuration 1 */
#define CS2100_FNCCFG2      0x17    /* Function Configuration 2 */
#define CS2100_FNCCFG3      0x1e    /* Function Configuration 3 */

/////////////////////////////////////////////////////////////////////
// Register Bit Field Definitions

/* Device ID and Revision */
#define CS2100_DEVID_REVISION_SHIFT        (0)       /* Bits 0-2: Device REVISION */
#define CS2100_DEVID_REVISION_MASK         (7 << CS2100_DEVID_REVISION_SHIFT)
#define CS2100_DEVID_DEVICE_SHIFT          (3)       /* Bits 3-7: Device ID */
#define CS2100_DEVID_DEVICE_MASK           (31 << CS2100_DEVID_DEVICE_SHIFT)
#define CS2100_DEVID_DEVICE                (0 << CS2100_DEVID_DEVICE_SHIFT)

/* Device Control */
#define CS2100_DEVCTL_CLKOUTDIS            (1 << 0)  /* Bit 0:  CLK_OUT disable */
#define CS2100_DEVCTL_AUXOUTDIS            (1 << 1)  /* Bit 1:  AUX_OUT disable */
#define CS2100_DEVCTL_UNLOCK               (1 << 7)  /* Bit 7:  Unlock PLL */

/* Device Configuration 1 */
#define CS2100_DEVCFG1_ENDEVCFG1           (1 << 0)  /* Bit 0: Enable 1 */
#define CS2100_DEVCFG1_AUXOUTSRC_SHIFT     (1)       /* Bits 1-2: Source of AUX_OUT signal */
#define CS2100_DEVCFG1_AUXOUTSRC_MASK      (3 << CS2100_DEVCFG1_AUXOUTSRC_SHIFT)
#define CS2100_DEVCFG1_AUXOUTSRC_REFCLK    (0 << CS2100_DEVCFG1_AUXOUTSRC_SHIFT) /* RefClk */
#define CS2100_DEVCFG1_AUXOUTSRC_CLKIN     (1 << CS2100_DEVCFG1_AUXOUTSRC_SHIFT) /* CLK_IN */
#define CS2100_DEVCFG1_AUXOUTSRC_CLKOUT    (2 << CS2100_DEVCFG1_AUXOUTSRC_SHIFT) /* CLK_OUT */
#define CS2100_DEVCFG1_AUXOUTSRC_PLLLOCK   (3 << CS2100_DEVCFG1_AUXOUTSRC_SHIFT) /* PLL Lock Status Indicator*/
#define CS2100_DEVCFG1_RMODSEL_SHIFT       (5)       /* Bit 5-7: Selects R-Mod value */
#define CS2100_DEVCFG1_RMODSEL_MASK        (7 << CS2100_DEVCFG1_RMODSEL_SHIFT)
#define CS2100_DEVCFG1_RMODSEL_NONE        (0 << CS2100_DEVCFG1_RMODSEL_SHIFT) /* Left-shift R-value by 0 (x 1) */
#define CS2100_DEVCFG1_RMODSEL_MUL2        (1 << CS2100_DEVCFG1_RMODSEL_SHIFT) /* Left-shift R-value by 1 (x 2) */
#define CS2100_DEVCFG1_RMODSEL_MUL4        (2 << CS2100_DEVCFG1_RMODSEL_SHIFT) /* Left-shift R-value by 2 (x 4) */
#define CS2100_DEVCFG1_RMODSEL_MUL8        (3 << CS2100_DEVCFG1_RMODSEL_SHIFT) /* Left-shift R-value by 3 (x 8) */
#define CS2100_DEVCFG1_RMODSEL_DIV2        (4 << CS2100_DEVCFG1_RMODSEL_SHIFT) /* Right-shift R-value by 1 (÷ 2) */
#define CS2100_DEVCFG1_RMODSEL_DIV4        (5 << CS2100_DEVCFG1_RMODSEL_SHIFT) /* Right-shift R-value by 2 (÷ 4) */
#define CS2100_DEVCFG1_RMODSEL_DIV8        (6 << CS2100_DEVCFG1_RMODSEL_SHIFT) /* Right-shift R-value by 3 (÷ 8) */
#define CS2100_DEVCFG1_RMODSEL_DIV16       (7 << CS2100_DEVCFG1_RMODSEL_SHIFT) /* Right-shift R-value by 4 (÷ 16) */

/* Global Configuration */
#define CS2100_GBLCFG_ENDEVCFG2            (1 << 0)  /* Bit 0: Enable 2 */
#define CS2100_GBLCFG_FREEZE               (1 << 3)  /* Bit 3: Mods not applied until unfrozen */

/* Ratio: Bits 0-31 (4 x 8-bit values) */

/* Function Configuration 1 */
#define CS2100_FNCCFG1_REFCLKDIV_SHIFT     (3)       /* Bits 3-4: Reference Clock Input Divider */
#define CS2100_FNCCFG1_REFCLKDIV_MASK      (3 << CS2100_FNCCFG1_REFCLKDIV_SHIFT)
#define CS2100_FNCCFG1_REFCLKDIV_DIV4      (0 << CS2100_FNCCFG1_REFCLKDIV_SHIFT) /* ÷ 4. 32 MHz to 75 MHz (50 MHz with XTI) */
#define CS2100_FNCCFG1_REFCLKDIV_DIV2      (1 << CS2100_FNCCFG1_REFCLKDIV_SHIFT) /* ÷ 2. 16 MHz to 37.5 MHz */
#define CS2100_FNCCFG1_REFCLKDIV_NONE      (2 << CS2100_FNCCFG1_REFCLKDIV_SHIFT) /* ÷ 1. 8 MHz to 18.75 MHz */

#define CS2100_FNCCFG1_AUXLOCKCFG_MASK     (1 << 6)  /* Bit 6:  AUX PLL Lock Output Configuration */
#define CS2100_FNCCFG1_AUXLOCKCFG_PP       (0 << 6)  /*   0=Push-Pull */
#define CS2100_FNCCFG1_AUXLOCKCFG_OD       (1 << 6)  /*   1=Open Drain */
#define CS2100_FNCCFG1_CLKSKIPEN           (1 << 7)  /* Bit 7:  Clock Skip Enable */

/* Function Configuration 2 */
#define CS2100_FNCCFG2_LFRATIOCFG          (1 << 3)  /* Bit 3:  Low-Frequency Ratio Configuration */
#define CS2100_FNCCFG2_CLKOUTUNL           (1 << 4)  /* Bit 4:  Enable PLL Clock Output on Unlock */

/* Function Configuration 3 */
#define CS2100_FNCCFG3_CLKINBW_SHIFT       (4)       /* Bits 4-6: Clock Input Bandwidth */
#define CS2100_FNCCFG3_CLKINBW_MASK        (7 << CS2100_FNCCFG3_CLKINBW_SHIFT)
#define CS2100_FNCCFG3_CLKINBW_1HZ         (0 << CS2100_FNCCFG3_CLKINBW_SHIFT) /* 1 Hz */
#define CS2100_FNCCFG3_CLKINBW_2HZ         (1 << CS2100_FNCCFG3_CLKINBW_SHIFT) /* 2 Hz */
#define CS2100_FNCCFG3_CLKINBW_4HZ         (2 << CS2100_FNCCFG3_CLKINBW_SHIFT) /* 4 Hz */
#define CS2100_FNCCFG3_CLKINBW_8HZ         (3 << CS2100_FNCCFG3_CLKINBW_SHIFT) /* 8 Hz */
#define CS2100_FNCCFG3_CLKINBW_16HZ        (4 << CS2100_FNCCFG3_CLKINBW_SHIFT) /* 16 Hz */
#define CS2100_FNCCFG3_CLKINBW_32HZ        (5 << CS2100_FNCCFG3_CLKINBW_SHIFT) /* 32 Hz */
#define CS2100_FNCCFG3_CLKINBW_64HZ        (6 << CS2100_FNCCFG3_CLKINBW_SHIFT) /* 64 Hz */
#define CS2100_FNCCFG3_CLKINBW_128HZ       (7 << CS2100_FNCCFG3_CLKINBW_SHIFT) /* 128 Hz */

#define CLK_IN	24000000.0

static struct akaudio_reg g_cs2100_reg[] =
{
	{CS2100_DEVID, 0},
	{CS2100_DEVCTL, 0},
	{CS2100_DEVCFG1, 0},
	{CS2100_GBLCFG, 0},
	{CS2100_RATIO0, 0},
	{CS2100_RATIO1, 0},
	{CS2100_RATIO2, 0},
	{CS2100_RATIO3, 0},
	{CS2100_FNCCFG1, 0},
	{CS2100_FNCCFG2, 0},
	{CS2100_FNCCFG3, 0},
};

struct akaudio_reg*cs2100_regs(uint32_t *count)
{
	*count = sizeof(g_cs2100_reg) / sizeof(struct akaudio_reg);
	return g_cs2100_reg;
}

static inline int read_reg(struct i2c_client *client, u8 reg)
{
	return i2c_smbus_read_byte_data(client, reg);
}

static inline int write_reg(struct i2c_client *client, u8 reg, u8 value)
{
	return i2c_smbus_write_byte_data(client, reg, value);
}

int cs2100_write_reg(u8 reg, u8 val)
{
	if (akaudio_get_i2c_client() == NULL) return -1;
	return write_reg(akaudio_get_i2c_client(),reg,val);
}

unsigned int cs2100_read_reg(u8 reg)
{
	if (akaudio_get_i2c_client() == NULL) return -1;
	return read_reg(akaudio_get_i2c_client(), reg);
}

struct clock_ratio {
	unsigned int sample_rate;
	uint32_t bits_16;
	uint32_t bits_24;
};

static struct clock_ratio clk_ratio [] = {
	{  44100, 11289600, 16934400 },
	{  48000, 12288000, 18432000 },
	{  64000, 16384000, 24576000 },
	{  88200, 22579200, 33868800 },
	{  96000, 24576000, 36864000 },
	{ 176400, 45158400, 67737600 },
	{ 192000, 49152000, 73728000 },
};

static uint32_t cs2100_get_freqout(uint32_t rate, uint32_t format)
{
	int i;
	uint32_t cnt = sizeof(clk_ratio) / sizeof(struct clock_ratio);

	for (i = 0; i < cnt; i++) {
		if (clk_ratio[i].sample_rate == rate)
			break;
	}

	switch (format) {
	case SNDRV_PCM_FORMAT_S8:
	case SNDRV_PCM_FORMAT_U8:
		return clk_ratio[i].bits_16;
	case SNDRV_PCM_FORMAT_S16_LE:
		return clk_ratio[i].bits_16;
	case SNDRV_PCM_FORMAT_S24_LE:
		return clk_ratio[i].bits_24;
	}

	return 0;
}

static void cs2100_set_samplerate(uint32_t freq_in, uint32_t format)
{
	uint32_t freq;
	uint32_t value = 0;
	uint8_t reg[4];
	int i = 0;

	freq = cs2100_get_freqout(freq_in, format);

	if (freq_in > 100000) 
		value = 0x00000800; /* x0.5 */
	else 
		value = 0x00001000; /* x1 */
	
	printk("CS2100 fs: %d clk: %d reg: %08x\n", freq_in, freq, value);

	/* disable all clock output */
	cs2100_write_reg(CS2100_DEVCTL, CS2100_DEVCTL_AUXOUTDIS | CS2100_DEVCTL_CLKOUTDIS);

	/* set user defined ratio */
	reg[0] = (u8)((value >> 24) & 0x0ff);
	reg[1] = (u8)((value >> 16) & 0x0ff);
	reg[2] = (u8)((value >>  8) & 0x0ff);
	reg[3] = (u8)((value) 		& 0x0ff);

	cs2100_write_reg(CS2100_RATIO0, reg[0]);
	cs2100_write_reg(CS2100_RATIO1, reg[1]);
	cs2100_write_reg(CS2100_RATIO2, reg[2]);
	cs2100_write_reg(CS2100_RATIO3, reg[3]);	

	/* enable clock out */
	cs2100_write_reg(CS2100_DEVCTL, CS2100_DEVCTL_AUXOUTDIS);
}

static unsigned int cs2100_read(struct snd_soc_codec *codec, unsigned int reg)
{
	struct i2c_client *i2c =  codec->control_data;

	return read_reg(i2c, reg);
}

static void cs2100_set_init(void)
{
	cs2100_write_reg(CS2100_DEVCFG1, CS2100_DEVCFG1_ENDEVCFG1);
	cs2100_write_reg(CS2100_GBLCFG, CS2100_GBLCFG_ENDEVCFG2);
    cs2100_write_reg(CS2100_FNCCFG1, CS2100_FNCCFG1_REFCLKDIV_DIV2);
	cs2100_write_reg(CS2100_FNCCFG2, CS2100_FNCCFG2_CLKOUTUNL);
	cs2100_write_reg(CS2100_FNCCFG3, CS2100_FNCCFG3_CLKINBW_1HZ);
}

static void cs2100_set_shutdown(void)
{
	cs2100_write_reg(CS2100_DEVCTL, CS2100_DEVCTL_AUXOUTDIS | CS2100_DEVCTL_CLKOUTDIS);
}

static int cs2100_probe(struct snd_soc_codec *codec)
{
	printk("%s(%d)\n", __func__, __LINE__);

	codec->hw_read = cs2100_read;	
	codec->hw_write = (hw_write_t)i2c_master_send;
	
	cs2100_set_init();
	
	return 0;
}

#define CS2100_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | \
			SNDRV_PCM_FMTBIT_S24_LE)

#define CS2100_RATES (SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_44100 | \
		      SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_64000 | \
		      SNDRV_PCM_RATE_88200 | SNDRV_PCM_RATE_96000 | \
		      SNDRV_PCM_RATE_176400 | SNDRV_PCM_RATE_192000)

static struct snd_soc_dai_driver cs2100_dai_drv = {
	.name = "cs2100",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 2,
		.channels_max = 2,
		.rates = CS2100_RATES,
		.formats = CS2100_FORMATS,
	},
	.capture = {
		.stream_name = "Capture",
		.channels_min = 2,
		.channels_max = 2,
		.rates = CS2100_RATES,
		.formats = CS2100_FORMATS,
	},
	.ops = NULL,
	.symmetric_rates = 1
};

struct snd_soc_dai_driver* cs2100_get_dai_driver(void)
{
	return &cs2100_dai_drv;
}

static struct snd_soc_codec_driver cs2100_soc_codec_dev = {
	.probe = cs2100_probe,
};

struct snd_soc_codec_driver* cs2100_get_codec_driver(void)
{
	return &cs2100_soc_codec_dev;
}

static const struct of_device_id cs2100_of_match[] = {
	{ .compatible = "ak,cs2100", },
	{ }
};
MODULE_DEVICE_TABLE(of, cs2100_of_match);

void cs2100_reg_init(void)
{
	int i;
	
	for(i = 0; i < ARRAY_SIZE(g_cs2100_reg); i++) {
		cs2100_write_reg(g_cs2100_reg[i].reg, g_cs2100_reg[i].value);
	}
}

static int cs2100_detect(struct i2c_client *client,
			  struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;
	int val;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA)) {
		return -ENODEV;
	}

	val = i2c_smbus_read_byte_data(client, 0x01);
	if (((val >> 3) & 0xff) != 0x0) {
		return -ENODEV;
	}

	printk("%s(i2c_add:0x%x [01H]0x%x)\n", __FUNCTION__, client->addr, val);

	strlcpy(info->type, CS2100_NAME, I2C_NAME_SIZE);

	return 0;
}

static __devexit int cs2100_i2c_remove(struct i2c_client *i2c)
{
	snd_soc_unregister_codec(&i2c->dev);

	return 0;
}

static const struct i2c_device_id cs2100_i2c_id[] = {
	{ CS2100_NAME, 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, cs2100_i2c_id);

/* Addresses to scan */
static const unsigned short normal_i2c[] = { 0x4e, 0x4f, I2C_CLIENT_END };

static struct i2c_driver cs2100_i2c_driver = {
	.driver = {
		.name = CS2100_NAME,
		.owner = THIS_MODULE,
		.of_match_table = cs2100_of_match,
	},
	.class			= I2C_CLASS_HWMON, //i2c_detect
	.probe 			= NULL,
	.remove 		= __devexit_p(cs2100_i2c_remove),
	.address_list	= normal_i2c,
	.detect			= cs2100_detect, //i2c_detect
	.id_table 		= cs2100_i2c_id
};

struct i2c_driver *cs2100_get_i2c_driver(void)
{
	return &cs2100_i2c_driver;
}

static struct snd_soc_dai_link cs2100_dai[] = {
    {
        .name 			= "cs2100",
        .stream_name 	= "cs2100",
        .platform_name  = "nxp-pcm",
        .cpu_dai_name   = "nxp-i2s",

        .codec_name 	= "cs2100.0-004e",
        .codec_dai_name = "cs2100",
    },
};

struct snd_soc_dai_link *cs2100_get_dai_link(int *count)
{
	*count = ARRAY_SIZE(cs2100_dai);
	return cs2100_dai;
}

void cs2100_set_ops(struct akaudio_clk_ops*ops)
{
	ops->init_clk = cs2100_set_init;
	ops->set_samplerate = cs2100_set_samplerate;
	ops->get_freq_out = cs2100_get_freqout;
	ops->set_shutdown = cs2100_set_shutdown;
}

MODULE_DESCRIPTION("cs2100 driver for Astell&Kern audio module");
MODULE_LICENSE("GPL");
