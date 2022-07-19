#include "include/list.h"
#include <linux/slab.h>
#include <linux/string.h>
void swap_struct ( tsData * a , tsData * b ) {
	tsData tmp; // Temporary variable;
	tmp=*a; //store *a value to tmp
	*a=*b; //store *b to *a
	*b=tmp; //store tmp(previous data of a) to b
} //swaps two values.

MODULE_LICENSE("GPL");
