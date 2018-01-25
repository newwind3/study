/*
 * akaudio.c
 *
 * Copyright 2017 Iriver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 * 
 * Author: pillshin.lee
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/i2c.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>

#include <asm/gpio.h>

#include <mach/devices.h>
#include <mach/platform.h>

#include "akaudio.h"
#include "dacs/cs4398_ctl.h"

#include "clk/ak8157a.h"
#include "clk/cs2100.h"

#ifdef CONFIG_SND_USE_DYNAMIC_CONF
#include "ak/ak_features.h"
#endif

static struct akaudio_clk_device akaudio_clks[] = {
	{
		.get_clk_regs 		  	= ak8157a_regs,
		.get_clk_dai_link 		= ak8157a_get_dai_link,
		.get_clk_i2c_driver 	= ak8157a_get_i2c_driver,
		.get_clk_codec_driver 	= ak8157a_get_codec_driver,
		.get_clk_dai_driver 	= ak8157a_get_dai_driver,
		.set_clk_ops			= ak8157a_set_ops,
	},
	{
		.get_clk_regs 		  	= cs2100_regs,
		.get_clk_dai_link 		= cs2100_get_dai_link,
		.get_clk_i2c_driver 	= cs2100_get_i2c_driver,
		.get_clk_codec_driver 	= cs2100_get_codec_driver,
		.get_clk_dai_driver 	= cs2100_get_dai_driver,
		.set_clk_ops			= cs2100_set_ops,
	}	
};
static int akaudio_clkidx = 0;

static int dac_count;
static struct akaudio_status astatus;
static struct akaudio_dac_ops dacops;
static struct akaudio_clk_ops clkops;
static struct akaudio_gpios agpios;

static char *dai_platform_name = DEV_NAME_PCM;
static char dai_cpu_dai_name[16] = DEV_NAME_I2S;

static struct i2c_client *i2c_pll_client;

static DEFINE_MUTEX(akaudio_volume_mtx);
static DEFINE_MUTEX(akaudio_power_mtx);

static inline int pll_read_reg(u8 reg)
{
	return i2c_smbus_read_byte_data(i2c_pll_client, reg);
}

static inline int pll_write_reg(u8 reg, u8 value)
{
	return i2c_smbus_write_byte_data(i2c_pll_client, reg, value);
}

/*
 * sysfs
 */
static struct kobject *akaudio_kobj;

static ssize_t show_reg_clk(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret = 0;
	int i;
	struct akaudio_reg *clkreg = NULL;
	uint32_t cnt = 0;

	clkreg = akaudio_clks[akaudio_clkidx].get_clk_regs(&cnt);

	for (i = 0; i < cnt; i++) {
		ret += sprintf(&buf[ret], "REG %02x VAL %02x\n", clkreg[i].reg, pll_read_reg(clkreg[i].reg));
	}
	return ret;
}

static ssize_t store_reg_clk(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	char sreg[16], sval[16];
	int cnt;
	int reg, val;

	if (count == 0) {
		printk("REG -> read REG\nREG VALUE -> write VALUE\n");
		return 0;
	}

	cnt = sscanf(buf, "%s %s", sreg, sval);

	if (cnt == 1) {
		reg = simple_strtol(sreg, NULL, 16);
		printk("REG %02x VAL %02x\n", reg, pll_read_reg(reg));
	} else if (cnt == 2) {
		reg = simple_strtol(sreg, NULL, 16);
		val = simple_strtol(sval, NULL, 16);
		pll_write_reg(reg, val);
	}

	return count;
}

static ssize_t show_reg_dac(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret = 0;
	int i, c;
	struct akaudio_reg *clkreg = NULL;
	uint32_t cnt = 0;

#ifdef CONFIG_SND_AK_CS4398
	clkreg = cs4398_regs(&cnt);
#endif

	if (!clkreg) return ret;

	for (c = 0; c < dac_count; c++) {
		for (i = 0; i < cnt; i++) {
			ret += sprintf(&buf[ret], "CH %d REG %02x VAL %02x\n", 
					c, 
					clkreg[i].reg, 
					dacops.read_reg(c, clkreg[i].reg));
		}
	}

	return ret;
}

static ssize_t store_reg_dac(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	char sch[16], sreg[16], sval[16];
	int cnt;
	int ch, reg, val;

	if (count == 0) {
		printk("REG -> read REG\nREG VALUE -> write VALUE\n");
		return 0;
	}

	cnt = sscanf(buf, "%s %s %s", sch, sreg, sval);

	if (cnt == 2) {
		ch = simple_strtol(sch, NULL, 16);
		if (ch > 0 && ch < dac_count) {
			reg = simple_strtol(sreg, NULL, 16);
			printk("CH %d REG %02x VAL %02x\n", ch, reg, dacops.read_reg(ch, reg));
		}
	} else if (cnt == 3) {
		ch = simple_strtol(sch, NULL, 16);
		reg = simple_strtol(sreg, NULL, 16);
		val = simple_strtol(sval, NULL, 16);
		dacops.write_reg(ch, reg, val);
	}

	return count;
}

static ssize_t show_mute_volume(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret = 0;
	ret = sprintf(buf, "Volume Mute: %s\n", astatus.volume_muted ? "On" : "Off");
	return ret;
}

static ssize_t store_mute_volume(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	if (buf[0] == '1') akaudio_set_mute_volume(1);
	else if (buf[0] == '0') akaudio_set_mute_volume(0);
	return count;
}

static ssize_t show_mute_hp(struct device *dev, struct device_attribute *attr, char *buf)
{	
	ssize_t ret = 0;
	ret = sprintf(buf, "AMP Mute: %s\n", astatus.hp_muted ? "On" : "Off");
	return ret;
}

static ssize_t store_mute_hp(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	if (buf[0] == '1') akaudio_set_mute_amp(MUTE_ON);
	else if (buf[0] == '0') akaudio_set_mute_amp(MUTE_OFF);
	return count;
}
 
static ssize_t show_mute_dac(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	ssize_t ret = 0;
	ret = sprintf(buf, "DAC Mute: %s\n", astatus.dac_muted ? "On" : "Off");
	return ret;
}

static ssize_t store_mute_dac(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	if (buf[0] == '1') akaudio_set_mute_dac(1);
	else if (buf[0] == '0') akaudio_set_mute_dac(0);
	return count;
}
 
static ssize_t show_power_dac(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	ssize_t ret = 0;
	ret = sprintf(buf, "DAC Power: %s\n", astatus.dac_powered ? "On" : "Off");
	return ret;
}

static ssize_t store_power_dac(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	if (buf[0] == '1') akaudio_set_power_dac(1);
	else if (buf[0] == '0') akaudio_set_power_dac(0);
	return count;
}
 
 static ssize_t show_power_opamp(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	ssize_t ret = 0;
	ret = sprintf(buf, "AMP Power: %s\n", astatus.opamp_powered ? "On" : "Off");
	return ret;
}

static ssize_t store_power_opamp(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	if (buf[0] == '1') akaudio_set_power_opamp(1);
	else if (buf[0] == '0') akaudio_set_power_opamp(0);
	return count;
}
 
static ssize_t show_power_amp(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	ssize_t ret = 0;
	ret = sprintf(buf, "AMP Power: %s\n", astatus.opamp_powered ? "On" : "Off");
	return ret;
}

static ssize_t store_power_amp(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	if (buf[0] == '1') akaudio_set_power_amp(1);
	else if (buf[0] == '0') akaudio_set_power_amp(0);
	return count;
}
static struct device_attribute akaudio_reg_clk = 
	__ATTR(reg_clk, S_IRUGO|S_IWUSR, show_reg_clk, store_reg_clk);
static struct device_attribute akaudio_reg_dac = 
	__ATTR(reg_dac, S_IRUGO|S_IWUSR, show_reg_dac, store_reg_dac);
static struct device_attribute akaudio_mute_volume = 
	__ATTR(mute_vol, S_IRUGO|S_IWUSR, show_mute_volume, store_mute_volume);
static struct device_attribute akaudio_mute_hp = 
	__ATTR(mute_hp, S_IRUGO|S_IWUSR, show_mute_hp, store_mute_hp);
static struct device_attribute akaudio_mute_dac =
	__ATTR(mute_dac, S_IRUGO|S_IWUSR, show_mute_dac, store_mute_dac);
static struct device_attribute akaudio_power_dac =
	__ATTR(power_dac, S_IRUGO|S_IWUSR, show_power_dac, store_power_dac);
static struct device_attribute akaudio_power_opamp = 
	__ATTR(power_opamp, S_IRUGO|S_IWUSR, show_power_opamp, store_power_opamp);
static struct device_attribute akaudio_power_amp = 
	__ATTR(power_amp, S_IRUGO|S_IWUSR, show_power_amp, store_power_amp);

static struct attribute *akaudio_attributes[] = {
	&akaudio_reg_clk.attr,
	&akaudio_reg_dac.attr,
	&akaudio_mute_volume.attr,
	&akaudio_mute_hp.attr,
	&akaudio_mute_dac.attr,
	&akaudio_power_dac.attr,
	&akaudio_power_opamp.attr,
	&akaudio_power_amp.attr,
	NULL
};

static struct attribute_group akaudio_attr_grp = {
	.attrs = akaudio_attributes,
	.name = "akaudio",
};

/* 
 *	external APIs
 */
void akaudio_init_clk(void)
{
	if (clkops.init_clk) clkops.init_clk();
}

void akaudio_init_dac(void)
{
	if (dacops.init_dac) dacops.init_dac();
}

void akaudio_set_volume(int l, int r)
{
	mutex_lock(&akaudio_volume_mtx);

	astatus.lvolume = l;
	astatus.rvolume = r;
	
	mutex_unlock(&akaudio_volume_mtx);

	if (dacops.set_volume) dacops.set_volume(l, r);
}

void akaudio_set_freq(uint32_t freq_in, uint32_t format)
{
	astatus.freq = freq_in;
	astatus.format = format;
	
	msleep(100);
	/* set DAC */
	if (dacops.set_freq) dacops.set_freq(freq_in, format);

	msleep(100);
	/* set clock generator */
	if (clkops.set_samplerate) clkops.set_samplerate(freq_in, format);
}

void akaudio_set_mclk(int clk)
{
	printk("%s\n", __func__);
}

/* control power functions */
void akaudio_set_power_dac(int on)
{
	mutex_lock(&akaudio_power_mtx);
	
	/* this will also powering on clock generator */
	printk("DAC & PLL: %s\n", on ? "On" : "Off");
	gpio_set_value(agpios.dac_en, on);
	astatus.dac_powered = on;
	
	mutex_unlock(&akaudio_power_mtx);
}

void akaudio_set_power_opamp(int on)
{
	mutex_lock(&akaudio_power_mtx);
	
	printk("AMP: %s\n", on ? "On" : "Off");
	gpio_set_value(agpios.amp_en, on);
	astatus.opamp_powered = on;
	
	mutex_unlock(&akaudio_power_mtx);
}

void akaudio_set_power_amp(int on)
{
	mutex_lock(&akaudio_power_mtx);
	
	printk("AMP DC: %s\n", on ? "On" : "Off");
	gpio_set_value(agpios.amp_dcen, on);
	astatus.amp_powered = on;
	
	mutex_unlock(&akaudio_power_mtx);
}

/* control mute functions */
void akaudio_set_mute_amp(int mute)
{
	printk("Mute AMP: %s\n", mute ? "On" : "Off");
	gpio_set_value(agpios.amp_mute, mute);
	astatus.hp_muted = mute;
}

void akaudio_set_mute_volume(int mute)
{
	printk("Mute VOL: %s\n", mute ? "On" : "Off");
	
	if (mute) { 
		if (dacops.set_vol_mute) dacops.set_vol_mute();
	} else {
		akaudio_set_volume(astatus.lvolume, astatus.rvolume);
	}
	astatus.volume_muted = mute;
}

/* control reset functions */
void akaudio_reset_dac(void)
{
	gpio_set_value(agpios.dac_rst, 1);
	mdelay(1);
	gpio_set_value(agpios.dac_rst, 0);
	mdelay(1);
	gpio_set_value(agpios.dac_rst, 1);
}

void akaudio_reset_pll(void)
{
	gpio_set_value(agpios.pll_rst, 1);
	mdelay(1);
	gpio_set_value(agpios.pll_rst, 0);
	mdelay(1);
	gpio_set_value(agpios.pll_rst, 1);
}

void akaudio_set_mute_dac(int mute)
{
	printk("Mute DAC: %s\n", mute ? "On" : "Off");
	
	if (dacops.set_dac_mute) dacops.set_dac_mute(mute);
	astatus.dac_muted = mute;
}

/*
 *	Internal APIs 
 */
int akaudio_get_samplerate(void)
{
	return astatus.freq;
}

int akaudio_get_format(void)
{
	return astatus.format;
}

int akaudio_get_dac_count(void)
{
	return dac_count;
}

void akaudio_get_volume(int *l, int *r)
{
	*l = astatus.lvolume;
	*r = astatus.rvolume;
}

char *akaudio_get_dai_platform_name(void)
{
	return dai_platform_name;
}

char *akaudio_get_dai_cpu_dai_name(void)
{
	int i2s_ch = 0;

	sprintf(dai_cpu_dai_name, "%s.%d", DEV_NAME_I2S, i2s_ch);
	
	return dai_cpu_dai_name;
}

struct i2c_client *akaudio_get_i2c_client(void)
{
	return i2c_pll_client;
}

/* 
 *	Internal audio implementation 
 */
static void akaudio_init_gpios(struct akaudio_gpios *gpios)
{
	gpios->dac_rst 	= GPIO_DAC_RST;
	gpio_request(GPIO_DAC_RST, "GPIO_DAC_RST");
	gpios->dac_en 	= GPIO_DAC_EN;
	gpio_request(GPIO_DAC_EN, "GPIO_DAC_EN");

	gpios->pll_rst	= GPIO_PLL_RST;
	gpio_request(GPIO_PLL_RST, "GPIO_PLL_RST");

	gpios->amp_mute = GPIO_AMP_MUTE;
	gpio_request(GPIO_AMP_MUTE, "GPIO_AMP_MUTE");
	gpios->amp_en	= GPIO_AMP_EN;
	gpio_request(GPIO_AMP_EN, "GPIO_AMP_EN");
	gpios->amp_dcen  = GPIO_AMP_DC_EN;
	gpio_request(GPIO_AMP_DC_EN, "GPIO_AMP_DC_EN");

	gpios->hp_det	= GPIO_HP_DET;
	gpio_request(GPIO_HP_DET, "GPIO_HP_DET");
}

static int akaudio_create_sysfs(void)
{
	int ret;
	struct kobject *kobj;
	kobj = kobject_create_and_add("akaudioctl", &platform_bus.kobj);
	if (!kobj) {
		printk(KERN_ERR "failed to create kobject for akaudio\n");
		return -1;
	}
	ret = sysfs_create_group(kobj, &akaudio_attr_grp);
	if (ret) {
		printk(KERN_ERR "failed to create sysfs group for akaudio\n");
		kobject_del(kobj);
		return -1;
	}
	akaudio_kobj = kobj;
	return 0;
}

static void akaudio_release_sysfs(void)
{
	if (akaudio_kobj) {
		sysfs_remove_group(akaudio_kobj, &akaudio_attr_grp);
		kobject_del(akaudio_kobj);
	}
	akaudio_kobj = NULL;
}

static int akaudio_probe(struct platform_device *pdev)
{
	printk("%s(%d)\n", __func__, __LINE__);

	akaudio_create_sysfs();

#ifdef CONFIG_SND_USE_DYNAMIC_CONF
	dac_count = AK_CUR_HW_DAC_NUM;
	
	if (AK_CUR_HW_DAC_CHIP == AK_CS4398)
		cs4398_init(dac_count, &dacops);
#else
	dac_count = CONFIG_SND_AK_DAC_COUNT;
	
	akaudio_reset_dac();

#ifdef CONFIG_SND_AK_CS4398
	cs4398_init(dac_count, &dacops);
#endif

#endif

	akaudio_set_power_opamp(ON);
	akaudio_set_power_amp(ON);
	akaudio_set_mute_amp(MUTE_ON);
	
	return 0;
}

static int akaudio_remove(struct platform_device *pdev)
{
	akaudio_release_sysfs();

#ifdef CONFIG_SND_USE_DYNAMIC_CONF
	if (AK_CUR_HW_DAC_CHIP == AK_CS4398)
		cs4398_exit();
#else
	#ifdef CONFIG_SND_CS4398
	cs4398_exit();
	#endif
#endif
	
	return 0;
}

static int akaudio_suspend(struct device *dev)
{
	printk ("%s\n", __func__);

	akaudio_set_mute_dac(MUTE_ON);
	akaudio_set_mute_amp(MUTE_ON);
	akaudio_set_power_opamp(OFF);
	akaudio_set_power_amp(OFF);
	akaudio_set_power_dac(OFF);
	
	return 0;
}

static int akaudio_resume(struct device *dev)
{
	printk ("%s\n", __func__);

	akaudio_set_mute_amp(MUTE_ON);
	akaudio_set_power_dac(ON);
	akaudio_set_mute_dac(MUTE_ON);
	akaudio_set_power_opamp(ON);
	akaudio_set_power_amp(ON);

	akaudio_init_clk();
	akaudio_init_dac();

	return 0;
}

static int akaudio_startup_pcm(struct snd_pcm_substream *substream)
{
	printk("%s(%d)\n", __func__, __LINE__);
	return 0;
}

static void akaudio_shutdown_pcm(struct snd_pcm_substream *substream)
{
	printk("%s(%d)\n", __func__, __LINE__);
	if (dacops.set_shutdown) dacops.set_shutdown();
	if (clkops.set_shutdown) clkops.set_shutdown();
}

static int akaudio_hw_params_pcm(struct snd_pcm_substream *substream, struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	int ret = 0;

	printk("%s rate %d  format %d\n", __func__, params_rate(params), params_format(params));

	/* set codec DAI configuration */
	ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBM_CFM);
	if (ret < 0)
		return ret;

#ifdef CONFIG_SND_NXP_DFS 
	/* set cpu DAI configuration */
	ret = snd_soc_dai_set_fmt(cpu_dai, SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0)
		return ret;
#endif
	
	/* set the codec system clock for DAC and ADC */
   	ret = snd_soc_dai_set_pll(codec_dai, 0, 0, params_rate(params), params_format(params));
	if (ret < 0)
		return ret;

#ifdef CONFIG_SND_NXP_DFS 
	/* set the I2S system clock as input (unused) */
	ret = snd_soc_dai_set_sysclk(cpu_dai, 0, params_rate(params), SND_SOC_CLOCK_IN);
	if (ret < 0)
		return ret;
#endif

	return 0;
}

const struct dev_pm_ops akaudio_pm_ops = {
	.suspend 	= akaudio_suspend,
	.resume 	= akaudio_resume,
};

static struct platform_driver akaudio_driver = {
	.driver 	= {
		.name 	= "akaudio",
		.owner	= THIS_MODULE,
		.pm		= &akaudio_pm_ops,
	},
	.probe		= akaudio_probe,
	.remove		= akaudio_remove,
};

static int akaudio_set_pll(struct snd_soc_dai *dai, int pll_id,
			  int source, unsigned int freq_in,
			  unsigned int format)
{
	printk("akaudio set pll: freq %d format %d\n", freq_in, format);

	/* do not set ak8157a's clock */
	if (akaudio_clkidx == AKCLK_AK8157A) {
		if (dacops.set_freq) dacops.set_freq(freq_in, format);
		return 0;
	}

	akaudio_set_freq(freq_in, format);

	return 0;
}

static int akaudio_set_sysclk(struct snd_soc_dai *dai,
			     int clk_id, unsigned int freq, int dir)
{
	printk("%s(%d)\n", __func__, __LINE__);
	return 0;
}

static int akaudio_set_clkdiv(struct snd_soc_dai *dai,
			     int div_id, int div)
{
	printk("%s(%d)\n", __func__, __LINE__);
	return 0;
}

static int akaudio_set_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	printk("%s(%d)\n", __func__, __LINE__);
	return 0;
}

static const struct snd_soc_dai_ops akaudio_dai_ops = {
	.set_fmt = akaudio_set_fmt,
	.set_sysclk = akaudio_set_sysclk,
	.set_clkdiv = akaudio_set_clkdiv,
	.set_pll = akaudio_set_pll
};

static struct snd_soc_dai_link *akaudio_dai_link = NULL;
static int akaudio_dai_link_cnt = 0;

static __devinit int akaudio_i2c_probe(struct i2c_client *i2c,
				      const struct i2c_device_id *id)
{
	struct snd_soc_dai_driver *dai_drv = NULL;
	struct snd_soc_codec_driver *codec_drv = NULL;

	printk("AKAudio Clock Generator Probing..(%s)\n", i2c->name);

	if (strcmp(i2c->name, "ak8157a") == 0) {
		akaudio_clkidx = AKCLK_AK8157A;
	} else if (strcmp(i2c->name, "cs2100") == 0) {
		akaudio_clkidx = AKCLK_CS2100;
	}

	dai_drv = akaudio_clks[akaudio_clkidx].get_clk_dai_driver();
	codec_drv = akaudio_clks[akaudio_clkidx].get_clk_codec_driver();
	akaudio_dai_link = akaudio_clks[akaudio_clkidx].get_clk_dai_link(&akaudio_dai_link_cnt);
	akaudio_clks[akaudio_clkidx].set_clk_ops(&clkops);

	dai_drv->ops = &akaudio_dai_ops;
	
	i2c_pll_client = i2c;

	/* disable external clock in case of ak8157a */
	if (akaudio_clkidx == AKCLK_AK8157A)
		pll_write_reg(0x0, 0xe7);

	return snd_soc_register_codec(&i2c->dev, codec_drv, dai_drv, 1);
}

static struct snd_soc_card akaudio_soc_card = {
    .name 			= "AK Audio",
	.driver_name    = "AK Audio",
    .long_name 		= "Astell&Kern Hi-Fi Audio",
};

static struct platform_device *soc_audio_device;
static struct platform_device *akaudio_device;

static struct snd_soc_ops akaudio_pcm_ops = {
	.startup 	= akaudio_startup_pcm,
	.hw_params 	= akaudio_hw_params_pcm,
	.shutdown 	= akaudio_shutdown_pcm,
};

static int __init akaudio_init(void)
{
	int ret = 0;
	int i;

    printk("Astell&Kern Audio Init...\n");

	akaudio_init_gpios(&agpios);
	
	akaudio_set_mute_amp(MUTE_ON);

	akaudio_set_power_dac(ON);
	akaudio_reset_pll();

	for (i = 0; i < AKCLK_MAX; i++) {
		akaudio_clks[i].clk_i2c_driver = akaudio_clks[i].get_clk_i2c_driver();
		akaudio_clks[i].clk_i2c_driver->probe = akaudio_i2c_probe;
		
		ret = i2c_add_driver(akaudio_clks[i].clk_i2c_driver);
		if (ret) {
			printk("Failed to register clock generator I2C driver: %s\n", 
				akaudio_clks[i].clk_i2c_driver->driver.name);
		}
	}

	akaudio_soc_card.dai_link = akaudio_dai_link;
	akaudio_soc_card.num_links = akaudio_dai_link_cnt;

	for (i = 0; i < akaudio_soc_card.num_links; i++) {
		akaudio_soc_card.dai_link[i].platform_name = akaudio_get_dai_platform_name();
		akaudio_soc_card.dai_link[i].cpu_dai_name = akaudio_get_dai_cpu_dai_name();
		akaudio_soc_card.dai_link[i].ops = &akaudio_pcm_ops;
	}

	soc_audio_device = platform_device_alloc("soc-audio", -1);
	if (!soc_audio_device)
		return -ENOMEM;

	platform_set_drvdata(soc_audio_device, &akaudio_soc_card);

	ret = platform_device_add(soc_audio_device);
	if (ret) {
        printk(KERN_ERR "Unable to add platform device\n");
        platform_device_put(soc_audio_device);
    }

	akaudio_device = platform_device_alloc("akaudio", -1);
	if (!akaudio_device) return -ENOMEM;

	ret = platform_device_add(akaudio_device);
	if (ret) {
		printk("Unable to add platform device for akaudio\n");
		platform_device_put(akaudio_device);

		return ret;
	}

	return platform_driver_register(&akaudio_driver);
}

static void __exit akaudio_exit(void)
{
	int i;

	platform_device_unregister(akaudio_device);
	platform_device_unregister(soc_audio_device);
	platform_driver_unregister(&akaudio_driver);
	for (i = 0; i < AKCLK_MAX; i++) {
		i2c_del_driver(akaudio_clks[i].clk_i2c_driver);
	}
}

module_init(akaudio_init);
module_exit(akaudio_exit);

MODULE_DESCRIPTION("Astell&Kern Audio Driver");
MODULE_LICENSE("GPL");

