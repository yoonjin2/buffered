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
#define _MAJOR_ 600
#define MTX_INIT(mutex) do { static struct lock_class_key _locked_; __mutex_init((mutex),#mutex,&__key); } while 0
#define GET_UUID(string) do { int16_t raw;get_random_bytes(&raw,sizeof(int16_t));strcpy(string,#raw);}


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
int major;
char * __tmp_name__;
dev_t dev;

MODULE_LICENSE("GPL");

int __init init_device (void) {
	
	

	__tmp_name__ = kmalloc (sizeof(int16_t),GFP_TEMPORARY);
	GET_UUID(__tmp_name__);
	major = register_chrdev(_MAJOR_, __tmp_name__ , &fops);
	if(major < 0) {
		printk(KERN_ALERT "Registering Character Device failed with %d\n", major);
		kfree(__tmp_name__);
		return 0;
	} else if (major!=_MAJOR_) {
		printk(KERN_INFO "Major Number %d is unavailable;Using %d instead.\n" , _MAJOR_, major);
	}
	lst = kmalloc ( sizeof(list) , GFP_KERNEL );
	printk ("Buffered Device Initialized; Please check /dev/%s\n", __tmp_name__);
	return 0;
}
void __exit clean_device(void) {
	unregister_chrdev(major,__tmp_name__);
	kfree(__tmp_name__);
	kfree(lst);
}

int device_open (struct inode *inode , struct file *file ) {
	if ( is_busy == 1 ) {
		return -EBUSY;
	} else {
		is_busy=1;
	}

	try_module_get(THIS_MODULE);	
	return 0;
}
int device_release (struct inode *inode , struct file *file) {
	is_busy=0;
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

