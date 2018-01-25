#ifndef __AK_NETLINK_H__
#define __AK_NETLINK_H__

#include <linux/skbuff.h>
#include <ak/aknl_types.h>

#define INIT_BC_INT32(_KEY, _VAL)						\
	message_t *__pmsg;									\
	int __size = sizeof(message_t) + sizeof(int32_t);	\
	int32_t *__val;										\
	__pmsg = kmalloc(__size, GFP_KERNEL);				\
	strncpy(__pmsg->key, #_KEY, strlen(#_KEY));			\
	__pmsg->type = TYPE_INT32;							\
	__pmsg->length = 1;									\
	__val = (int32_t*)__pmsg->value;			

#define INIT_BC_INT32S(_KEY, _CNT)								\
	message_t *__pmsg;											\
	int __size = sizeof(message_t) + sizeof(int32_t) * _CNT;	\
	int32_t *__val;												\
	__pmsg = kmalloc(__size, GFP_KERNEL);						\
	strncpy(__pmsg->key, #_KEY, strlen(#_KEY));					\
	__pmsg->type = TYPE_INT32S;									\
	__pmsg->length = _CNT;										\
	__val = (int32_t*)__pmsg->value;				

#define INIT_BC_STRING(_KEY, _LEN)							\
	message_t *__pmsg;										\
	int __size = sizeof(message_t) + sizeof(char) * _LEN;	\
	char *__val;											\
	__pmsg = kmalloc(__size, GFP_KERNEL);					\
	strncpy(__pmsg->key, #_KEY, strlen(#_KEY));				\
	__pmsg->type = TYPE_STRING;								\
	__pmsg->length = _LEN;									\
	__val = (char*)__pmsg->value;				

#define INIT_BC_CHUNK(_KEY, _LEN)								\
	message_t *__pmsg;											\
	int __size = sizeof(message_t) + sizeof(uint8_t) * _LEN;	\
	char *__val;												\
	__pmsg = kmalloc(__size, GFP_KERNEL);						\
	strncpy(__pmsg->key, #_KEY, strlen(#_KEY));					\
	__pmsg->type = TYPE_CHUNK;									\
	__pmsg->length = _LEN;										\
	__val = (uint8_t*)__pmsg->value;				

#define IVAL(_IDX)	__val[_IDX]
#define SVAL(_STR) 	strncpy(__val, #_STR, strlen(#_STR))
#define CVAL(_CHK, _LEN)	memcpy(__val, _CHK, _LEN)

#define START_BROADCASTING							\
	if (aknl_broadcast_msg(__pmsg, __size) < 0) {	\
		printk("aknl : broadcasting failed\n");		\
		return -1;									\
	}												\
	kfree(__pmsg);									\
	return 0;

typedef int32_t (*ak_module_func_t)(int pid, message_t *);

extern ak_module_func_t modfunctions[NL_MODULE_MAX];

void aknl_set_callbacks(void);

int32_t aknl_get_int32(message_t *pmsg);
int32_t *aknl_get_int32s(message_t *pmsg, uint32_t *cnt);
char *aknl_get_string(message_t *pmsg);
uint8_t *aknl_get_chunk(message_t *pmsg, uint32_t *len);

int32_t aknl_return(int pid, uint32_t module, int32_t retval);
int32_t aknl_return_int32(int pid, uint32_t module, char *key, int32_t value);
int32_t aknl_return_int32s(int pid, uint32_t module, char *key, int32_t *value, uint32_t len);
int32_t aknl_return_string(int pid, uint32_t module, char *key, char *value, uint32_t len);

int32_t aknl_broadcast_msg(message_t *msg, int sz_msg);

#endif
