#include "include/list.h"
#include <linux/slab.h>
#include <linux/string.h>
void swap_void ( void * a , void * b ) {
	void * tmp=kmalloc ( sizeof(void *), GFP_KERNEL);
	memcpy( a , b , sizeof(void *));
	memcpy( b , tmp , sizeof(void*));
	kfree(tmp);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yunjin Lee");
MODULE_VERSION("0.01");
