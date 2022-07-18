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
#include <asm/uaccess.h>
#define DEVALLOC "buffer"
#define DEVPATH  "/dev/buffer"
#define DEVCLASS "bufclass"
#define DEVNAME "buffer"
#define MINORS 1


extern void push(list * lst , void * item);
extern void * pop_item(list * lst);
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
cdev cdev_data;
dev_t dev;

MODULE_AUTHOR("CraftX Inc.");
MODULE_LICENSE("GPL");

int __init init_device (void) {
	
        dev = MKDEV ( MAJOR ( dev ) , MINOR ( dev ) ) ;
	lst = kmalloc ( sizeof(list) , GFP_KERNEL );
        if ( ( alloc_chrdev_region ( & dev , 0 , 1 , "BUF" ) ) < 0 )  { 
		printk(KERN_INFO "Cannot allocate major number\n");
		return -EINVAL;
	}

		cdev_data.owner = THIS_MODULE;
		cdev_init( &cdev_data , &fops);

		if (cdev_add(&cdev_data, dev, 1 )<0) {
				printk ("Cannot add the device to the system\n");
                		cdev_del ( &cdev_data ) ;
				return -EINVAL;
		}

		if ((cdev_class = class_create ( THIS_MODULE , DEVCLASS))==NULL) {
			printk (KERN_INFO "Cannot add struct class\n");
                		cdev_del ( &cdev_data ) ;
				class_unregister(cdev_class);
        			class_destroy 	( cdev_class ) ;
				return -EINVAL;
		}



        if ( ( device_create ( cdev_class , NULL , dev , NULL , "buffer0" ) ) == NULL ) {
                printk ( "ERROR : CANNOT CREATE THE DEVICE \n" );
		device_destroy ( cdev_class , dev ) ; 
               	cdev_del ( &cdev_data ) ;
		class_unregister(cdev_class);
        	class_destroy 	( cdev_class ) ;
                return EEXIST ;
        }
		
	printk ("Buffered Device Initialized; Please check /dev");
	return 0;
}
void __exit clean_device(void) {
	device_destroy(cdev_class,dev);
        cdev_del ( &cdev_data ) ;
	class_unregister(cdev_class);
	class_destroy (cdev_class);
	unregister_chrdev_region( dev , MINORS);
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
	char * data = (char *)pop_item (lst);
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
		push ( lst , (void *)buf++ ) ;
		len --;
	}
	is_busy=0;
	return ret;
}
module_init(init_device);
module_exit(clean_device);

