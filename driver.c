#include "list.h"
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
#define DEVALLOC "buffer%d"
#define DEVPATH  "/dev/buffer"
#define DEVNAME "buffer"
#define MAX 10



int8_t is_busy;

list * lst;
static struct file_operations fops = {
	.read			= device_read,
	.write		=device_write,
	.open			= device_open,
	.release	=device_release,
};
static struct class *cdev_class;
typedef struct cdev cdev;
cdev cdev_data_array[MAX];
dev_t dev;
extern int __init init_device (void) {
	
  int major=14132;
	int cnt;
	if (alloc_chrdev_region (&dev , 0 , MAX , "buf_dev" )<0) {
		printk(KERN_INFO "Cannot allocate major number\n");
		return -EINVAL;
	}
	printk(KERN_INFO "major: %d, minor: %d\n" , MAJOR(dev),MINOR(dev));
	char str[13]=DEVPATH;
	for ( cnt = 0 ; cnt < MAX; cnt ++) {
		cdev_init( &cdev_data_array[cnt] , &fops);
		cdev_data_array[cnt].owner = THIS_MODULE;
		if (cdev_add(&cdev_data_array[cnt],MKDEV(dev, cnt) , 1 )<0) {
				printk ("Cannot add the device to the system\n");
				return -EINVAL;
		}
		if (cdev_class = class_create ( THIS_MODULE , DEVNAME)==NULL) {
			printk (KERN_INFO "Cannot add struct class\n");
		}
		

		if (device_create ( cdev_class  , NULL , dev ,NULL, DEVALLOC, cnt)==NULL) {
			printk (KERN_INFO "Cannot create the device");
		}
		str[11]=(char)(48+cnt);
		mknod ( str , S_IRWXG , dev );
	}
		
	printk ("Buffered Device Initialized; Please check /dev");
	return 0;
}
extern void __exit clean_device(void) {
	int cnt;
	for (cnt = 0; cnt < MAX ; cnt++ ) {
		device_destroy(cdev_class,MKDEV(major,cnt));
	}
	class_unregister(cdev_class);
	class_destroy (cdev_class);
	unregister_chrdev_region( MKDEV(major,0) , MINOR(dev));
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
	return 0;
}

extern ssize_t device_read (struct file * file,
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
extern ssize_t device_write (struct file * file,
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
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yunjin Lee");
MODULE_VERSION("0.01");
