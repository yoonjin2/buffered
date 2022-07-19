#include "include/list.h"
#include <linux/slab.h>
#include <linux/string.h>
void swap_void ( tsData a , tsData b ) {
	tsData tmp=kmalloc ( sizeof(tsData), SLAB_TEMPORARY); //temporary pointer
	memcpy( tmp, a , sizeof(tsData)); //copy a to tmp
	memcpy( a , b , sizeof(tsData)); //copy b to a
	memcpy( b , tmp , sizeof(void*)); //copy tmp(previous data of a) to b
	kfree(tmp);
} //swaps two values.

MODULE_LICENSE("GPL");
