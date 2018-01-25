
obj-y += clk/ak8157a.o
obj-y += clk/cs2100.o

obj-y += akaudio.o

ifeq ($(CONFIG_SND_USE_DYNAMIC_CONF),y)
obj-y += dacs/cs4398_ctl.o
else
obj-$(CONFIG_SND_AK_CS4398) += dacs/cs4398_ctl.o
endif

EXTRA_CFLAGS += -Isound/soc/akaudio
