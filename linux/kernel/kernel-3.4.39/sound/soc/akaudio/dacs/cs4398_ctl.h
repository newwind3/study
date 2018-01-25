#ifndef __CS4398_CTL_H__
#define __CS4398_CTL_H__

#define MAX_VOLUME_OFFSET (3)
#define MAX_VOLUME_IDX (150)

struct akaudio_reg* cs4398_regs(uint32_t *count);
int cs4398_init(int dacs, struct akaudio_dac_ops *ops);
void cs4398_exit(void);

#endif
