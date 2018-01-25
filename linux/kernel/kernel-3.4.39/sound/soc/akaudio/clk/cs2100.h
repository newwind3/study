#ifndef __CS2100_H__
#define __CS2100_H__

struct akaudio_reg*cs2100_regs(uint32_t *count);
struct snd_soc_dai_link* cs2100_get_dai_link(int *count);
struct i2c_driver* cs2100_get_i2c_driver(void);
struct snd_soc_codec_driver* cs2100_get_codec_driver(void);
struct snd_soc_dai_driver* cs2100_get_dai_driver(void);
void cs2100_set_ops(struct akaudio_clk_ops*ops);

#endif
