/*
 * ir_ioctl.
 *
 * Copyright 2010 iriver
 * 2013-5-22 tcc8930
 * 2016-10-25 exynos7420
 * 2017-3 version 2.
 * jhlim.
 */
 
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/i2c.h>
#include <linux/spi/spi.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/tlv.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>

#include <linux/io.h>
//#include <mach/gpio.h>
#include <asm/gpio.h>


#include <linux/miscdevice.h>

#include <ak/ir_ioctl.h>
#include <linux/file.h>
#include <asm/cacheflush.h>

#include <linux/string.h>

#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

#include <ak/astell_kern.h>
//
// smart byte stream IO.
//
//
int sio_init(struct io_buffer_t *pio_mgr,int sr_mode)
{
	pio_mgr->rw_offset = 0;
	pio_mgr->sr_mode = sr_mode;
	
	if(sr_mode) {
		memset(pio_mgr->io_buffer,0x0,MAX_SIO_BUFFER);
	} 
	
	return 0;
}

int sio_tell(struct io_buffer_t *pio_mgr)
{
	return pio_mgr->rw_offset;
}
int sio_seek(struct io_buffer_t *pio_mgr,int offset)
{
	pio_mgr->rw_offset = offset;
	return offset;
}

int write_data_8(struct io_buffer_t *pio_mgr,unsigned char Value)
{
	if(pio_mgr->rw_offset > MAX_SIO_BUFFER) {
		CPRINTR("error !!\n");
		return -1;
	}
	pio_mgr->io_buffer[pio_mgr->rw_offset++] = Value;
	return 0;
}

int  write_data_16(struct io_buffer_t *pio_mgr,unsigned short Value)
{
	write_data_8(pio_mgr,(unsigned char)Value);
	write_data_8(pio_mgr,(unsigned char)(Value >> 8));
    return 0;
}
int write_data_32(struct io_buffer_t *pio_mgr,int Value)
{
    write_data_8(pio_mgr, (unsigned char)Value);
    write_data_8(pio_mgr, (unsigned char)(Value >> 8));
    write_data_8(pio_mgr, (unsigned char)(Value >> 16));
    write_data_8(pio_mgr, (unsigned char)(Value >> 24));
    return 0;	 
}

int sio_write_int(struct io_buffer_t *pio_mgr,int Value,int type)
{
    write_data_8(pio_mgr,type);
    switch(type) {
	case SIO_ARGV_INT8: 
		write_data_8(pio_mgr,1);
		write_data_8(pio_mgr,Value); 
		break;
	case SIO_ARGV_INT16:
		write_data_8(pio_mgr,2);
		write_data_16(pio_mgr,Value); 
		break;
	case SIO_ARGV_INT32:  
		write_data_8(pio_mgr,4);
		write_data_32(pio_mgr,Value); 
		break;
    }
 
    return 0;
}

int  sio_write_string(struct io_buffer_t *pio_mgr,char *string)
{
	int str_len;
    write_data_8(pio_mgr,SIO_ARGV_STR);
	str_len = strlen(string);
    write_data_8(pio_mgr,str_len);

	if(pio_mgr->rw_offset > MAX_SIO_BUFFER) {
		CPRINTR("error !!\n");
		return -1;
	}	
	memcpy(&pio_mgr->io_buffer[pio_mgr->rw_offset],string,str_len);
	pio_mgr->rw_offset += str_len;

    return 0;
}

int  sio_write_data(struct io_buffer_t *pio_mgr,char *data,int len)
{
	
    write_data_8(pio_mgr,SIO_ARGV_DATA);
    write_data_32(pio_mgr,len);

	if(len > 0) {
		if(pio_mgr->rw_offset > MAX_SIO_BUFFER) {
			CPRINTR("error !!\n");
			return -1;
		}	
		 
		 memcpy(&pio_mgr->io_buffer[pio_mgr->rw_offset],data,len);
		 pio_mgr->rw_offset += len;
	}
	return  0;
}

inline int _read_data_8(struct io_buffer_t *pio_mgr)
{
	return pio_mgr->io_buffer[pio_mgr->rw_offset++];
}

int read_data_8(struct io_buffer_t *pio_mgr)
{
	return _read_data_8(pio_mgr);
}


unsigned short read_data_16(struct io_buffer_t *pio_mgr)
{
    unsigned int iValue;
    iValue = _read_data_8(pio_mgr);
    iValue |= _read_data_8(pio_mgr) << 8;
    return (unsigned short)iValue;
}

unsigned int read_data_32(struct io_buffer_t *pio_mgr)
{
    unsigned int iValue;
    iValue = _read_data_8(pio_mgr);
    iValue |= _read_data_8(pio_mgr) << 8;
    iValue |= _read_data_8(pio_mgr) << 16;
    iValue |= _read_data_8(pio_mgr) << 24;
    return iValue;
}

int  sio_read_int(struct io_buffer_t *pio_mgr)
{
	int value = 0;
	int Type,Size;

	Type = _read_data_8(pio_mgr );
	Size = _read_data_8(pio_mgr);

    switch(Type) {
    case SIO_ARGV_INT8: 
        value = _read_data_8(pio_mgr);
        break;
    case SIO_ARGV_INT16:
        value = read_data_16(pio_mgr);
        break;
    case SIO_ARGV_INT32:  
        value= read_data_32(pio_mgr);
        break;
 	}	
	
	return value;
}

int  sio_read_data(struct io_buffer_t *pio_mgr,char *data)
{
	int Type,Size;
	
	Type = read_data_8(pio_mgr );
	Size = read_data_32(pio_mgr);	
	if(Size> 0) {
		memcpy(data,&pio_mgr->io_buffer[pio_mgr->rw_offset],Size);
		pio_mgr->rw_offset += Size;
	}    
    return Size;
}
int  sio_read_string(struct io_buffer_t *pio_mgr,char *string)
{
	int Type,Size;
    
	Type = read_data_8(pio_mgr );
	Size = read_data_8(pio_mgr);	

	if(Size>= MAX_SIO_STR) {

		printk("Error !!! Size:%d %s\n" ,Size,string);
		return 0;
	}
	memcpy(string,&pio_mgr->io_buffer[pio_mgr->rw_offset],Size);
	string[Size] = 0;
	pio_mgr->rw_offset += Size;
    
	return Size;
}

int _sio_set_argv(struct io_buffer_t *pio_mgr,va_list list)
{
    int i;
    int iCountArgc;
    int ArgvType = -1;
    char *desc;
	int cmd_size;
	
    iCountArgc = 0;

	sio_init(pio_mgr,1);  // result mode
	
	sio_seek(pio_mgr,SIO_DATA_START);

    for(i=0;i < MAX_SIO_FIELDS;i++) {
        ArgvType= (int)va_arg(list,ARGV_VA_INT);
		
        if(ArgvType == SIO_ARGV_END)
            goto End;           

		desc = (char*)va_arg(list,char*);
	
        switch(ArgvType) {
        case SIO_ARGV_INT8:
        case SIO_ARGV_INT16:
        case SIO_ARGV_INT32:
        {
            int Value;
            Value = (int)va_arg(list,ARGV_VA_INT);
            sio_write_int(pio_mgr,Value,ArgvType);						
        }
        break;
        case SIO_ARGV_STR:
        {
            char *string;
            string = (char*)va_arg(list,char*);
            sio_write_string(pio_mgr,string);						
        }
        break;
        case SIO_ARGV_DATA:
        {
            char *data;
            int data_size;
					
            data = (char*)va_arg(list,char*);
            data_size = (int)va_arg(list,ARGV_VA_INT);					
            sio_write_data(pio_mgr,data,data_size);						
        }
        break;
        default:
        {
            char *string;
            string = (char*)va_arg(list,char*);
        }

        break;
        break;
        }
    }  
	
End:
	cmd_size = pio_mgr->rw_offset;
	sio_seek(pio_mgr,0);
	write_data_16(pio_mgr,cmd_size);
	sio_seek(pio_mgr,cmd_size);

    return 0;
}

int sio_set_argv(struct io_buffer_t *pio_mgr,...)
{
    va_list list;
    
    va_start(list,pio_mgr); 

	_sio_set_argv(pio_mgr,list);	

    va_end(list);
    return 0;
}

int ir_ioctl_open(struct inode *inode, struct file *filp)  
{
	//printk("ir_ioctl_open opened \n");
	
	return nonseekable_open(inode, filp);
}  
  
int ir_ioctl_release(struct inode *inode, struct file *filp)  
{  
    return 0;  
}  


// reference: http://forum.falinux.com/zbxe/?mid=lecture_tip&page=1&document_srl=553645

static DEFINE_MUTEX(ir_ioctl_mutex);

static int g_io_mgr_debug = 0;

void ir_set_debug(int debug)
{
	g_io_mgr_debug = debug;

}
int ir_get_debug(void)
{
	return g_io_mgr_debug;
}


long ir_ioctl_common(int type,struct io_buffer_t *p_io_mgr,struct file *filp, unsigned int cmd, unsigned long arg)  
{  
    int size;  
    int ret;
	int user_cmd_size;

	{
		int cmd_size;
		
	    size = cmd;
			
		ret = copy_from_user ( (void *)&p_io_mgr->io_buffer, (const unsigned char __user *) arg, size );	 

		sio_seek(p_io_mgr,0);
		user_cmd_size = read_data_16(p_io_mgr);

		if(g_io_mgr_debug)  {
			CPRINTR("[kernel]size:%d user_cmd_size:%d\n",size,user_cmd_size);				
			ir_hexdump(p_io_mgr->io_buffer,user_cmd_size);
		}
		
		if(ret) {
			CPRINTR("copy_from_user error !!\n");
		}

		sio_init(p_io_mgr,0);  // read mode

		cmd_size = read_data_16(p_io_mgr);
		
		sio_seek(p_io_mgr,SIO_DATA_START);

		if(type == IR_IOCTL)  {
			ir_ioctl_cmd(p_io_mgr);
		} 
		else if(type == IR_IOCTL2)
		{
			ir_ioctl2_cmd(p_io_mgr);
		}

		if(p_io_mgr->sr_mode == 1) {
			
			if(p_io_mgr->rw_offset > 0 && p_io_mgr->rw_offset <MAX_SIO_BUFFER) {
				if(g_io_mgr_debug)  {
					CPRINTR("[kernel]io_mgr.rw_offset:%d\n",p_io_mgr->rw_offset);				
					ir_hexdump(p_io_mgr->io_buffer,p_io_mgr->rw_offset);
				}
				ret = copy_to_user ( (void *) arg, (const void *) &p_io_mgr->io_buffer, (unsigned long ) (p_io_mgr->rw_offset) );	 

				if(ret) {
					CPRINTR("copy_to_user error !!\n");
			    }
			} else {
				CPRINTR("rw_offset:%d  result:%d\n",p_io_mgr->rw_offset,	p_io_mgr->rw_offset);			
				return -1;
			}
		} else {
		
			if(g_io_mgr_debug)	{
				CPRINTR("!\n");
			}
		}
	}
	
    return 0;  
}  


long ir_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)  
{  
    int ret;
	mutex_lock(&ir_ioctl_mutex);

	{
	 	struct io_buffer_t io_mgr;
		ret = ir_ioctl_common(IR_IOCTL,&io_mgr,filp,cmd,arg);  
	}
    mutex_unlock(&ir_ioctl_mutex);
	
    return ret;  
}  

void ir_hexdump(unsigned char *buf, unsigned int len)
{
	print_hex_dump(KERN_CONT, "", DUMP_PREFIX_OFFSET,
			16, 1,
			buf, len, false);
}


struct file_operations ir_ioctl_fops =  {  
    .owner    = THIS_MODULE,  
    .unlocked_ioctl    = ir_ioctl,  
	.compat_ioctl	= ir_ioctl,    
    .open     = ir_ioctl_open,       
    .release  = ir_ioctl_release,    
};  

static struct miscdevice ir_ioctl_misc_device = {
    .minor =MISC_DYNAMIC_MINOR,
    .name = IR_IOCTL_DEV_NAME,
    .fops = &ir_ioctl_fops,
};


/*
2017.04.17 jhlim
ir_ioctl 2 
for xmos,etc
*/
static DEFINE_MUTEX(ir_ioctl_mutex2);


long ir_ioctl2(struct file *filp, unsigned int cmd, unsigned long arg)  
{  
	int ret;
	mutex_lock(&ir_ioctl_mutex2);

	{
		struct io_buffer_t io_mgr;
		ret = ir_ioctl_common(IR_IOCTL2,&io_mgr,filp,cmd,arg);  
	}
	mutex_unlock(&ir_ioctl_mutex2);
	
	return ret;  
}  

struct file_operations ir_ioctl2_fops =  {  
    .owner    = THIS_MODULE,  
    .unlocked_ioctl    = ir_ioctl2,  
	.compat_ioctl	= ir_ioctl2,    
    .open     = ir_ioctl_open,       
    .release  = ir_ioctl_release,    
};  

static struct miscdevice ir_ioctl2_misc_device = {
    .minor =MISC_DYNAMIC_MINOR,
    .name = IR_IOCTL2_DEV_NAME,
    .fops = &ir_ioctl2_fops,
};



static int __init ir_ioctl_modinit(void)
{
	int ret;
	mutex_init(&ir_ioctl_mutex);

    if ((ret = misc_register(&ir_ioctl_misc_device)) < 0 ) {
        printk(KERN_INFO "regist ir_ioctl drv failed \n");
        misc_deregister(&ir_ioctl_misc_device);
	}


    if ((ret = misc_register(&ir_ioctl2_misc_device)) < 0 ) {
        printk(KERN_INFO "regist ir_ioctl2 drv failed \n");
        misc_deregister(&ir_ioctl2_misc_device);
	}

	printk(KERN_INFO "IR_IOCTL/IR_IOCTL2 interface driver. \n");

	return ret;
}


module_init(ir_ioctl_modinit);

static void __exit ir_ioctl_exit(void)
{

    misc_deregister(&ir_ioctl_misc_device);
}

module_exit(ir_ioctl_exit);




MODULE_DESCRIPTION("ir_ioctl driver");
MODULE_AUTHOR("iRiver");
MODULE_LICENSE("GPL");

