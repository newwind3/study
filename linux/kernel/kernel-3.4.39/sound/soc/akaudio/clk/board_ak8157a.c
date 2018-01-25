/*
 * Copyright 2017 Iriver
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
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/slab.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>

#include <mach/irqs.h>
#include <asm/mach-types.h>

#include <akaudio.h>

//#include "../codecs/ak8157a.h"
//#include "tcc-pcm.h"
//#include "tcc-i2s.h"
//#include <mach/board_astell_kern.h>

static int ak_startup(struct snd_pcm_substream *substream)
{
	return 0;
}

/* we need to unmute the HP at shutdown as the mute burns power on tcc83x */
static void ak_shutdown(struct snd_pcm_substream *substream)
{
}

static int ak_hw_params(struct snd_pcm_substream *substream, struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	unsigned int clk = 0;
	int ret = 0;

	switch (params_rate(params)) {
    case 22000:
        clk = 22050 * 256;
        break;
    case 11000:
        clk = 11025 * 256;
        break;

	case 48000:
	case 96000:
		clk = 12288000;
		break;

	case 88200:
	case 44100:
		clk = 11289600;
		break;

    default:
        clk = params_rate(params) * 256;
        break;
	}

	/* set codec DAI configuration */
	ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S |
	SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBM_CFM);
	if (ret < 0)
		return ret;

	/* set cpu DAI configuration */
	ret = snd_soc_dai_set_fmt(cpu_dai, SND_SOC_DAIFMT_I2S |
		SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0)
		return ret;

	/* set the codec system clock for DAC and ADC */
   	ret = snd_soc_dai_set_pll(codec_dai, 0, 0, params_rate(params), 0);
	if (ret < 0)
		return ret;

	/* set the I2S system clock as input (unused) */
	ret = snd_soc_dai_set_sysclk(cpu_dai, 0, params_rate(params), SND_SOC_CLOCK_IN);
	if (ret < 0)
		return ret;
 
	return 0;
}

static struct snd_soc_ops ak_ops = {
	.startup 	= ak_startup,
	.hw_params 	= ak_hw_params,
	.shutdown 	= ak_shutdown,
};

static int ak_ak8157a_init(struct snd_soc_pcm_runtime *rtd)
{
    return 0;
}

static struct snd_soc_dai_link ak_dai[] = {
    {
        .name 			= "ak8157a",
        .stream_name 	= "ak8157a",
        .platform_name  = "nxp-pcm",
        .cpu_dai_name   = "nxp-i2s",

        .codec_name 	= "ak8157a.0-0010",
        .codec_dai_name = "ak8157a",
        .init 			= &ak_ak8157a_init,
        .ops 			= &ak_ops,
    },
};

static struct snd_soc_card ak_soc_card = {
    .name 			= "AK Audio",
	.driver_name    = "AK Audio",
    .long_name 		= "Astell&Kern Hi-Fi Audio",
	.dai_link  		= ak_dai,
	.num_links 		= ARRAY_SIZE(ak_dai),
};

static struct platform_device *ak_snd_device;

int __init init_ak8157a(void)
{
    int ret;
	int i;

    printk("Astell&Kern clock generator probe [%s]\n", __FUNCTION__);

	for (i = 0; i < ARRAY_SIZE(ak_dai); i++) {
		ak_dai[i].platform_name = akaudio_get_dai_platform_name();
		ak_dai[i].cpu_dai_name = akaudio_get_dai_cpu_dai_name();
	}

	ak_snd_device = platform_device_alloc("soc-audio", -1);
	if (!ak_snd_device)
		return -ENOMEM;

	platform_set_drvdata(ak_snd_device, &ak_soc_card);

	ret = platform_device_add(ak_snd_device);
	if (ret) {
        printk(KERN_ERR "Unable to add platform device\n");
        platform_device_put(ak_snd_device);
    }

	return ret;
}

void __exit exit_ak8157a(void)
{
	platform_device_unregister(ak_snd_device);
}

//module_init(init_ak8157a);
//module_exit(exit_ak8157a);

/* Module information */
MODULE_DESCRIPTION("ALSA SoC Astell&Kern audio module");
MODULE_LICENSE("GPL");
