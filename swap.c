#include "include/list.h"
#include <linux/slab.h>
#include <linux/string.h>
void swap_void ( bdat_t * a , bdat_t * b ) {
	bdat_t * tmp=kmalloc ( sizeof(bdat_t *), SLAB_TEMPORARY); //temporary pointer
	memcpy( tmp, a , sizeof(bdat_t *)); //copy a to tmp
	memcpy( a , b , sizeof(bdat_t *)); //copy b to a
	memcpy( b , tmp , sizeof(void*)); //copy tmp(previous data of a) to b
	kfree(tmp);
} //swaps two values.

MODULE_LICENSE("GPL");
