#include "buffer.h"
#include <linux/random.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/device/class.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/ktime.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#define __DEV_COUNT__ 1000
#define NAME "BUFFEREDCHAR"
#define GENERATE "SIG_NEW"
int __MAJOR__;
char *UUID_TEXT="1234567890abcdefghijklmnopqrstuvwxyz";
int8_t err;
extern int8_t enqueue(list * lst , tsData item);
extern tsData dequeue(list * lst);
int16_t failflag=0;
int minor=0;
int8_t is_busy;

list * lst;
struct file_operations fops = {
	.read			= device_read,
	.write		=device_write,
	.open			= device_open,
	.release	=device_release,
};
struct cdev cdev[__DEV_COUNT__];
struct class *CLASS = NULL;
char ** __device_name__;
dev_t dev;

MODULE_LICENSE("GPL");
char * GET_UUID (void) {
	static char string[36];
	auto int num=0;
	auto int i;
	for(i=0;i<36;i++) {
		if((i==8)|(i==13)|(i==18)|(i==23)) {
			string[i]='-';
			continue;
		}
		get_random_bytes(&num,sizeof(int));
		num%=strlen(UUID_TEXT);
		string[i]=UUID_TEXT[num];
	}
	return string;
	
}

int8_t GENERATE_DEVICE (int minor) {
	int8_t err_dev;
	__device_name__[minor]=GET_UUID();
	__MAJOR__=MAJOR(dev);
	cdev_init(&cdev[minor],&fops);
	cdev[minor].owner = THIS_MODULE;
	err_dev = cdev_add(&cdev[minor],MKDEV(__MAJOR__,minor),1);
	if (err_dev<0) {
		printk(KERN_ALERT "cdev_add() failed with error code(%d)" , err_dev);
		failflag++;
		return err_dev;
	}
	device_create(CLASS,NULL,MKDEV(__MAJOR__,minor),NULL,__device_name__[minor]);

	printk (KERN_INFO "Buffered Device Initialized; Please check /dev/%s\n",__device_name__[minor] );
	return 0;
}



char *CLASSNAME;
int __init init_device (void) {
	auto int i;
	__device_name__=kmalloc(sizeof(char *)*__DEV_COUNT__,GFP_KERNEL);
	CLASSNAME=kmalloc(sizeof(char)*36,GFP_KERNEL);
	CLASSNAME=GET_UUID();
	for(minor=0;minor<__DEV_COUNT__;minor++) {
		__device_name__[minor] = kmalloc (sizeof(char)*36,GFP_KERNEL);
	}
	err = register_chrdev_region(dev,__DEV_COUNT__,NAME);
	if(err < 0) {
		printk(KERN_ERR "Registering Character Device failed with %d\n", err);
		for (i=0;i<minor;i++) {
			kfree(__device_name__[i]);
		}
		return 0;
	}
	
	CLASS=class_create(THIS_MODULE,CLASSNAME);
	if(IS_ERR(CLASS)) {
		printk(KERN_ERR "Creating Character Device Class failed");
		for(minor=0;i<__DEV_COUNT__;i++) {
			kfree(__device_name__[i]);	
		}
		kfree(__device_name__);
		return 0;
	}
	return err;


	lst=kmalloc(sizeof(list),GFP_KERNEL);
		
	minor=0;
	init_list(lst);
	err=GENERATE_DEVICE(minor);
	if(err<0) {
		printk(KERN_ALERT "Device Generation Failed: error code(%d)", err);
		return err;
	}
	return 0;
}
void __exit clean_device(void) {
	
	auto int i;
	unregister_chrdev_region(dev,__DEV_COUNT__);
	for(i=0;i<minor;i++) {
		kfree(__device_name__[i]);
	}
	kfree(__device_name__);
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
				char __user *buf,
				size_t len,
				loff_t *offset) {
	int ret1,ret2;
	tsData data = dequeue (lst);
	if (is_busy)
		return -EBUSY;
	if ( lst -> size == 0 ) 
		return -EINVAL;
  
	is_busy = 1;
	ret1=copy_to_user(buf,data.data,sizeof(char)*strlen(data.data));
	ret2=copy_to_user(buf,(char *)data.ts,sizeof(ktime_t));
	if((ret2<0)|(ret1<0)) {
		printk( KERN_ERR "Copying data failed with error codes (%d,%d)", ret1, ret2);
	}
	lst->size--;
	is_busy=0;
	return (ssize_t)(sizeof(tsData));
}
ssize_t device_write (struct file * file,
														const char *buf,
														size_t len,
														loff_t *offset) {
	tsData data;
	ktime_t tmp;
	ssize_t ret = (ssize_t)len;
	if (is_busy) {
		return -EBUSY;
	}
	if(strcmp(buf,GENERATE)==0) {
		minor++;
		err=GENERATE_DEVICE(minor);
		if (err<0) {
			printk(KERN_ALERT "Warning: Device Generation Failed with error code(%d),",err);
			minor--;
			return -ENOMEM;
		} else {
			return 0;
		}
	}
	is_busy=1;
	tmp=ktime_get_real();
	data.ts=tmp;
	strcat(data.data,buf);
	if(full(lst)) {
		dequeue(lst);
	}
	enqueue ( lst , data);
	lst->size++;
	is_busy=0;
	return ret;
}
module_init(init_device);
module_exit(clean_device);

