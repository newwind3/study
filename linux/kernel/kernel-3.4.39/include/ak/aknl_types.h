#ifndef __AK_NLTYPES_H__
#define __AK_NLTYPES_H__

#include <linux/types.h>

#define NETLINK_AK_IPC_BC 	29
#define NETLINK_AK_IPC 		31

#define KEY_LEN 	32

#define INT32_LEN	(sizeof(int32_t))
#define INT8_LEN	(sizeof(int8_t))

#define MAX_PAYLOAD 256

enum {
	NL_MODULE_XMOS,
	NL_MODULE_AUDIODSP,
	NL_MODULE_ANONYMOUS,
	NL_MODULE_MAX,
}; 

typedef enum {
	TYPE_INT32,
	TYPE_INT32S,
	TYPE_STRING,
	TYPE_CHUNK,
} nl_type_t;

typedef struct _message {
	char key[KEY_LEN];
	nl_type_t type;
	uint32_t length;
	uint8_t value[0];
} message_t;

typedef struct _module_msg {
	uint32_t module;
	uint32_t sztotal;
	message_t message;
} module_msg_t;

#endif
