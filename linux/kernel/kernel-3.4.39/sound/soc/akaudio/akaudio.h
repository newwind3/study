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

#ifndef __AKAUDIO_H__
#define __AKAUDIO_H__

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#define OFF 	0
#define ON 		1

#define MUTE_OFF 	0
#define MUTE_ON		1

enum {
	AKCLK_AK8157A,
	AKCLK_CS2100,
	AKCLK_MAX
};

struct akaudio_dac_ops {
	void (*init_dac)(void);
	void (*set_volume)(int, int);
	void (*set_vol_mute)(void);
	void (*set_dac_mute)(int);
	void (*set_freq)(uint32_t, uint32_t);
	void (*set_shutdown)(void);
	int (*read_reg)(int, u8);
	int (*write_reg)(int, u8, u8);
};

struct akaudio_clk_ops {
	void (*init_clk)(void);
	uint32_t (*get_freq_out)(uint32_t, uint32_t);
	void (*set_samplerate)(uint32_t, uint32_t);
	void (*set_shutdown)(void);
};

struct akaudio_status {
	int dac_powered;
	int opamp_powered;
	int amp_powered;
	int rvolume, lvolume;
	int volume_muted;
	int dac_muted;
	int hp_muted;
	int freq;
	int format;
};

struct akaudio_gpios {
	unsigned int dac_rst;
	unsigned int dac_en;

	unsigned int pll_rst;

	unsigned int amp_mute;
	unsigned int amp_dcen;
	unsigned int amp_en;

	unsigned int hp_det;
};

struct akaudio_reg {
	u8 reg;
	u8 value;
};

struct akaudio_clk_device {
	struct i2c_driver *clk_i2c_driver;

	struct akaudio_reg* (*get_clk_regs)(uint32_t *);
	struct snd_soc_dai_link* (*get_clk_dai_link)(int *);
	struct i2c_driver* (*get_clk_i2c_driver)(void);
	struct snd_soc_codec_driver* (*get_clk_codec_driver)(void);
	struct snd_soc_dai_driver* (*get_clk_dai_driver)(void);
	void (*set_clk_ops)(struct akaudio_clk_ops*);
};

/* 
 *	external APIs
 */
void akaudio_init_clk(void);
void akaudio_init_dac(void);
void akaudio_set_volume(int l, int r);
void akaudio_set_vol_mute(int mute);
void akaudio_set_freq(uint32_t freq_in, uint32_t freq_out);
void akaudio_set_mclk(int clk);
void akaudio_set_power_dac(int on);
void akaudio_set_power_opamp(int on);
void akaudio_set_power_amp(int on);
void akaudio_set_mute_amp(int mute);
void akaudio_set_mute_volume(int mute);
void akaudio_set_mute_dac(int mute);
void akaudio_reset_dac(void);
void akaudio_reset_pll(void);

/*
 *	Internal APIs 
 */
int akaudio_get_samplerate(void);
int akaudio_get_format(void);
int akaudio_get_dac_count(void);
void akaudio_get_volume(int *l, int *r);
char *akaudio_get_dai_platform_name(void);
char *akaudio_get_dai_cpu_dai_name(void);
struct i2c_client *akaudio_get_i2c_client(void);

#endif
