#include "include/list.h"
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#define DEVALLOC "buffer"
#define DEVPATH  "/dev/buffer"
#define DEVCLASS "bufclass"
#define DEVNAME "buffer"
#define _MAJOR_ 600
#define _MINOR_ 1
#define CUSTOM_MODE 0006774
#define GET_UUID(string) do { int16_t raw; strcpy(string,#raw);}


extern int8_t enqueue(list * lst , bdat_t * item);
extern bdat_t * dequeue(list * lst);
int8_t is_busy;

list * lst;
struct file_operations fops = {
	.read			= device_read,
	.write		=device_write,
	.open			= device_open,
	.release	=device_release,
};
struct class *cdev_class;
typedef struct cdev cdev;
char * __tmp_name__;
dev_t dev;

MODULE_LICENSE("GPL");

int __init init_device (void) {
	
        dev = MKDEV (_MAJOR_,_MINOR_) ;
	lst = kmalloc ( sizeof(list) , GFP_KERNEL );
	__tmp_name__ = kmalloc (sizeof(int16_t),SLAB_TEMPORARY);
	GET_UUID(__tmp_name__);
	if(__register_chrdev( _MAJOR_,_MINOR_,num,__tmp_name__,&fops)<0 ) {
		printk(KERN_INFO "Cannot allocate major number");	
		return -ENOMEM;
	}
        if ( (register_chrdev_region( &dev , _MINOR_ , __tmp_name__ ) ) < 0 )  { 
		printk(KERN_INFO "Cannot allocate major number region\n");
		return -ENOMEM;
	}

		if ((cdev_class = class_create ( THIS_MODULE , __tmp_name__))==NULL) {
			printk (KERN_INFO "Cannot add struct class\n");
				class_unregister(cdev_class);
        			class_destroy 	( cdev_class ) ;
				return -ENOMEM;
		}



        if ( ( device_create ( cdev_class , NULL , dev , NULL , __tmp_name__  ) ) == NULL ) {
                printk ( "ERROR : CANNOT CREATE THE DEVICE \n" );
		device_destroy (cdev_class,dev); 
		class_unregister(cdev_class);
        	class_destroy 	(cdev_class);
                return -EINVAL;
        }
	mknod(__tmp_name__,0006774, dev);
		
	printk ("Buffered Device Initialized; Please check /dev");
	kfree(__tmp_name__);
	return 0;
}
void __exit clean_device(void) {
	device_destroy(cdev_class,dev);
	class_unregister(cdev_class);
	class_destroy (cdev_class);
	unregister_chrdev_region( dev , _MINOR_);
	kfree(lst);
}

int device_open (struct inode *inode , struct file *file ) {
	if ( is_busy == 1 ) {
		return -EBUSY;
	}

	try_module_get(THIS_MODULE);	
	return 0;
}
int device_release (struct inode *inode , struct file *file) {
	return 0;
}

ssize_t device_read (struct file * file,
														char *buf,
														size_t len,
														loff_t *offset) {
	char * data = (char *)dequeue(lst);
	ssize_t bytes_read;
	if (is_busy)
		return -EBUSY;
	if ( lst -> size == 0 ) 
		return -EINVAL;
  
	is_busy = 1;
	while ( len && *data) {
		put_user ( *(data++) , buf++);
		len--;
		bytes_read++;
	}
	is_busy=0;
	return bytes_read;
}
ssize_t device_write (struct file * file,
														const char *buf,
														size_t len,
														loff_t *offset) {
	size_t ret = len;
	if (is_busy)
		return -EBUSY;
	is_busy=1;
	while ( len && *buf ) {
		enqueue ( lst , (bdat_t *)buf++ ) ;
		len --;
	}
	is_busy=0;
	return ret;
}
module_init(init_device);
module_exit(clean_device);

