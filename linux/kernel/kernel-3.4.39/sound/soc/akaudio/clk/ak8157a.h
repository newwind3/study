#ifndef __AK8157A_H__
#define __AK8157A_H__

struct akaudio_reg*ak8157a_regs(uint32_t *count);
struct snd_soc_dai_link* ak8157a_get_dai_link(int *count);
struct i2c_driver* ak8157a_get_i2c_driver(void);
struct snd_soc_codec_driver* ak8157a_get_codec_driver(void);
struct snd_soc_dai_driver* ak8157a_get_dai_driver(void);
void ak8157a_set_ops(struct akaudio_clk_ops*ops);

#endif
