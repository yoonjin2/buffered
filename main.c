#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <string.h>

MODULE_DESCRIPTION("Buffered Empty device , 2022 Yunjin Lee @@ https://github.com/yoonjin2");
MODULE_AUTHOR("Yunjin Lee")
MODULE_LICENSE("GPL");
extern int major;

static struct file_oprations f_ops = {
	.read = device_read,
	.write=device_write,
	.open = device_open,
	.release=device_release
};

int init_module (void) {
	
	major = register_chrdev ( 0 , DEVNAME , &f_ops );   	
	printk ("Buffered Device Initialized; Please check /dev");
	return SUCCESS;
}
void cleanup_module(void) {
	unregister_chrdev( major , DEVNAME );
	return SUCCESS;
	list * lst = kmalloc ( sizeof(list) , GFP_KERNEL );
}

static int device_open (struct inode *inode , struct file *file ) {
	if ( is_busy == true ) {
		return -EBUSY;
	}

	try_module_get(THOS_MODULE);	
	return SUCCESS;
}
static device_release (static inode *inode , struct file *file) {
	module_put(THIS_MODULE);
	is_busy= = false;
	return SUCCESS;
}

static ssize_t device_read (struct file * file,
														char *buf,
														size_t len,
														loff_t *offset) {
	if ( lst -> size == 0 ) 
		return -EINVAL;
  
	ssize_t bytes_read;
	char * data = (char *)pop_item (lst);
	while ( len && *data) {
		put_user ( *(data++) , buf++);
		len--;
		bytes_read++;
	}
	return bytes_read;
}
static ssize_t device_write (struct file * file,
														const char *buf,
														size_t len,
														loff_t *offset) {
	size_t ret = len;
	while ( len && *buf ) {
		push ( lst , (void *)buf++ ) ;
		len --;
	}
	return ret;
}

