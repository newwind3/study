/*
 * ir_ioctl.
 *
 * Copyright 2010 iriver
 * 2013-5-22 tcc8930
 * 2016-10-25 exynos7420
 * 2017-3 version 2.
 * jhlim.
 */

 

#ifndef _ITOOLS_IOCTL_
#define _ITOOLS_IOCTL_


//

#define SIO_ID(a,b,c,d)     (((a)<<24) | ((b)<<16) | ((c)<<8) | (d))

#define MAKE_ID(a,b,c,d)     (((a)<<24) | ((b)<<16) | ((c)<<8) | (d))


#define  IR_IOCTL_DEV_NAME            "ir_ioctl"  

#define  IR_IOCTL2_DEV_NAME            "ir_ioctl2"  

#define IR_IOCTL (0)
#define IR_IOCTL2 (2)

#define MAX_SIO_FIELDS  (100)
#define MAX_SIO_LIST 30
#define MAX_SIO_STR (64)

#define MAX_SIO_BUFFER (128+32)

#define SIO_DATA_START (2)

#define ARGV_VA_INT int

struct io_buffer_t
{ 
	unsigned char io_buffer[MAX_SIO_BUFFER];
	int ioctl_fd;
	int rw_offset;
	int sr_mode;   //send , receive mode
} __attribute__ ((packed)) ; 

//
// 
//


enum MSG_ARGV_TYPE{
    SIO_ARGV_START = 0,
	SIO_ARGV_INT8,
	SIO_ARGV_INT16,
	SIO_ARGV_INT32,
	SIO_ARGV_STR,
	SIO_ARGV_DATA,	
	SIO_ARGV_END
};

/*
NOTE:
			32bit       64bit
-----------------------------
sizeof(int)   : 4        4
sizeof(char*) : 4        8

*/
unsigned short read_data_16(struct io_buffer_t *pio_mgr);
void ir_set_debug(int debug);
int ir_get_debug(void);

int sio_init(struct io_buffer_t *pio_mgr,int rwmode);
int sio_tell(struct io_buffer_t *ps_mgr);
int sio_seek(struct io_buffer_t *pio_mgr,int offset);
int sio_set_argv(struct io_buffer_t *ps_mgr,...);
int sio_write_int(struct io_buffer_t *ps_mgr,int Value,int size);
int sio_write_data(struct io_buffer_t *ps_mgr,char *data,int len);
int sio_write_string(struct io_buffer_t *ps_mgr,char *string);
int sio_read_int(struct io_buffer_t *ps_mgr);
int sio_read_string(struct io_buffer_t *ps_mgr,char *string);
int sio_read_data(struct io_buffer_t *ps_mgr,char *data);


void ir_hexdump(unsigned char *buf, unsigned int len);

int ir_ioctl_cmd(struct io_buffer_t *pio_mgr);
int ir_ioctl2_cmd(struct io_buffer_t *pio_mgr);


#endif
