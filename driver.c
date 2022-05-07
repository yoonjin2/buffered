#include "list.h"
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <asm/uaccess.h>


int8_t is_busy;

list * lst;
static struct file_operations fops = {
	.read			= device_read,
	.write		=device_write,
	.open			= device_open,
	.release	=device_release,
};
dev_t dev;
static struct class *class_dev;
int major;
typedef struct cdev cdev;
static cdev cdev_data_array[MAX];
extern int __init init_device (void) {
	
	int cnt;
	alloc_chrdev_region ( &dev ,0 , 1,DEVNAME);
	major = MAJOR(dev);
	class_dev = class_create ( THIS_MODULE , DEVNAME);
	for ( cnt = 0 ; cnt < MAX; cnt ++) {
		cdev_init( &cdev_data_array[cnt] , fops);
		cdev_data_array[cnt].owner = THIS_MODULE;
		cdev_add(cdev_data_array[cnt].cdev,MAKDEV(major, cnt) , 1 );
		device_create ( cdev_class  , NULL , MKDEV(major,cnt),NULL, DEVALLOC,cnt);
	}
		
	printk ("Buffered Device Initialized; Please check /dev");
	return 0;
}
extern void __exit clean_device(void) {
	int cnt;
	for (cnt = 0; cnt < MAX ; cnt++ ) {
		device_destroy(mychardev_class,MKDEV(major,cnt));
	}
	class_unregister(cdev_class);
	class_destroy (cdev_class);
	unregister_chrdev_region( MKDEV(major,0) , MINORMASK);
	lst = kmalloc ( sizeof(list) , GFP_KERNEL );
}

extern int device_open (struct inode *inode , struct file *file ) {
	if ( is_busy == 1 ) {
		return -EBUSY;
	}

	try_module_get(THIS_MODULE);	
	return 0;
}
extern int device_release (struct inode *inode , struct file *file) {
	module_put(THIS_MODULE);
	is_busy= 0;
	return 0;
}

extern ssize_t device_read (struct file * file,
														char *buf,
														size_t len,
														loff_t *offset) {
	char * data = (char *)pop_item (lst);
	ssize_t bytes_read;
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
extern ssize_t device_write (struct file * file,
														const char *buf,
														size_t len,
														loff_t *offset) {
	size_t ret = len;
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
