#include <stdarg.h>

#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/platform_device.h>

#include <asm/setup.h>

#include <ak/ak_models.h>
#include <ak/ak_core.h>
#include <ak/ak_features.h>

char *ak_specific_cmdline;

struct ak_model_info {
	int model;
	int swmodel;
	int hwrev;
	char smodel[16];
	char shwrev[16];
	char displayname[16];

	char wifi[24];
	char bluetooth[24];
	char ethernet[24];
};

static struct ak_model_operation *akops;

static struct ak_model_info ak_model_info;
static char __initdata features[AK_FEATURE_MAX][AK_FEATURE_STR_MAX];

/* utility functions */
int ak_register_operations(int model, struct ak_model_operation *ops)
{
	if (model == ak_get_model()) 
		akops = ops;
	
	return 0;
}

static int get_string(const char *data)
{
	int len = 0;

	while (1) {
		if (*data == ' ' || *data == '\0') break;
		len++;
		data++;
	}

	return len;
}

/* parsing ATAGs */
static int __init parse_tag_devparam1(const struct tag *tag)
{
	memcpy(&ak_model_info, tag->u.devparam1.devparam, sizeof(struct ak_model_info));

	printk("Model: %s(%d) HW Rev.: %s(%d) SW Model: %d Display Name: %s\n", 
		ak_get_model_str(), ak_get_model(), 
		ak_get_hwrev_str(), ak_get_hwrev(),
		ak_get_swmodel(), ak_get_dispname_str());
	return 0;
}
__tagtable(ATAG_DEVPARAM1, parse_tag_devparam1);

static int __init parse_tag_devparam2(const struct tag *tag)
{
	int len = 0;
	int i;
	const char *ptr = tag->u.devparam3.devparam;

	printk("Parameters for framework:\n");
	printk("%s(%d)param:%s\n", __func__, __LINE__, tag->u.devparam2.devparam);
	
	for (i = 0; i < AK_FEATURE_MAX; i++) {
		len = get_string(ptr);
		if (len == 0) break;
		
		memcpy(features[i], ptr, len);
		features[i][len] = '\0';
		ptr += (len + 1);
	}

	return 0;
}
__tagtable(ATAG_DEVPARAM2, parse_tag_devparam2);

static int __init parse_tag_devparam3(const struct tag *tag)
{
	memcpy(&ak_model_features, tag->u.devparam3.devparam, sizeof(ak_model_features));
	
	printk("Parameters for kernel:\n");
	_ak_dump_kernel_features();

	return 0;
}
__tagtable(ATAG_DEVPARAM3, parse_tag_devparam3);

static int __init parse_tag_devparam4(const struct tag *tag)
{
	return 0;
}
__tagtable(ATAG_DEVPARAM4, parse_tag_devparam4);

int ak_get_model(void)
{
	return ak_model_info.model;
}
EXPORT_SYMBOL(ak_get_model);

int ak_get_swmodel(void)
{
	return ak_model_info.swmodel;
}
EXPORT_SYMBOL(ak_get_swmodel);

int ak_get_hwrev(void)
{
	return ak_model_info.hwrev;
}
EXPORT_SYMBOL(ak_get_hwrev);

char*ak_get_model_str(void)
{
	return ak_model_info.smodel;
}
EXPORT_SYMBOL(ak_get_model_str);

char*ak_get_hwrev_str(void)
{
	return ak_model_info.shwrev;
}
EXPORT_SYMBOL(ak_get_hwrev_str);

char*ak_get_dispname_str(void)
{
	return ak_model_info.displayname;
}
EXPORT_SYMBOL(ak_get_dispname_str);

int ak_check_model(int no)
{
	return no == ak_model_info.model;
}
EXPORT_SYMBOL(ak_check_model);

int ak_check_hwrev(int no)
{
	return no == ak_model_info.hwrev;
}
EXPORT_SYMBOL(ak_check_hwrev);

/* 	
	0 : wifi mac
	1 : bluetooth mac
	2 : ethernet mac
*/
int ak_get_mac_address(int type, unsigned char *buff)
{
	char *start = NULL;
	char tmp[4] = {0};
	int idx = 0;

	if (type == 0) start = ak_model_info.wifi;
	else if (type == 1) start = ak_model_info.bluetooth;
	else if (type == 2) start = ak_model_info.ethernet;
	else return -1;

	if (strlen(start) == 0) return -1;

	while (start) {
		memset(tmp, 0, sizeof tmp);
		memcpy(tmp, start, 2);
		
		buff[idx++] = simple_strtol(tmp, NULL, 16);
		
		start = strstr(start, ":");
		if (start) start++;
	}

	return 0;	
}

struct ak_model_operation*ak_get_operation(void)
{
	return akops;
}

static int __init ak_core_init(void)
{
	int i, cnt; 
	int total_len = 0, index = 0;

	printk("Initialize properties for %s/%s-%s\n", 
			ak_model_info.smodel, ak_model_info.shwrev, ak_model_info.displayname);
	
	/* count total length */
	for (i = 0; i < AK_FEATURE_MAX; i++) {
		int len = strlen(features[i]);

		if (!len) break;
		total_len += sizeof("androidboot.") + len;
	}

	cnt = i;
	ak_specific_cmdline = kmalloc(total_len + 1, GFP_KERNEL);
	memset(ak_specific_cmdline, 0, total_len + 1);
	for (i = 0; i < cnt; i++) 
		index += sprintf(&ak_specific_cmdline[index], "androidboot.%s ", features[i]);

	return 0; 
}
pure_initcall(ak_core_init);

// 2017.1 jhlim
//
static int ak_model_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", ak_model_info.displayname);
	return 0;
}

static int ak_event_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", ak_get_hwrev_str());
	return 0;
}

static int ak_model_proc_open(struct inode *inode, struct file *file)
{
	if (strcmp(file->f_path.dentry->d_iname, "ak_model") == 0)
		return single_open(file, ak_model_proc_show, NULL);
	else 
		return single_open(file, ak_event_proc_show, NULL);
}

static const struct file_operations ak_proc_fops = {
	.open		= ak_model_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init ak_proc_init(void)
{
	proc_create("ak_model", 0, NULL, &ak_proc_fops);
	proc_create("ak_event", 0, NULL, &ak_proc_fops);

	return 0;
}
module_init(ak_proc_init);
