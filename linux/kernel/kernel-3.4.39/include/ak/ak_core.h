#ifndef __AK_CORE_H__
#define __AK_CORE_H__

#include <linux/rbtree.h>

#define AK_PROP_STR_LEN 	25
#define AK_MODEL_MAX  		16
#define AK_FEATURE_MAX		128 
#define AK_FEATURE_STR_MAX	128 
#define AK_KERN_FEATURE_MAX 32

struct ak_model_operation {
	int(*test_func)(int a, char *b);
};

extern char *ak_specific_cmdline;

int __init early_init_dt_scan_ak_model(unsigned long node, const char *uname, int depth, void *data);
void __init ak_get_mac_addrs_from_chosen(unsigned long node);
int ak_register_operations(int idx, struct ak_model_operation *ops);

int ak_get_model(void);
int ak_get_swmodel(void);
int ak_get_hwrev(void);
char *ak_get_model_str(void);
char *ak_get_hwrev_str(void);
char *ak_get_dispname_str(void);
int ak_check_model(int no);
int ak_check_hwrev(int no);
int ak_get_mac_address(int type, unsigned char *buff);
struct ak_model_operation*ak_get_operation(void);

struct device_node * of_find_node_by_path_format(const char *fmt, ...);

//int ak_feature_value(char *feature);
//char *ak_feature_value_str(char *feature);
#endif
