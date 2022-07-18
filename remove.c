#include "include/list.h"
#include <linux/slab.h>
// function that removes the item
void * remove_item ( list * lst , node * element )
{	
	void * val = (void *)NULL;
	if ( element == lst->front ) {
		return val;
		
	}	else if ( element == lst->rear ) {
		return val;
  } else if ( lst -> size == 0 ) {
	  return val;
  } else {
		node * x = element -> next;
		node * y = element -> prev;
        x -> prev = y;
		y -> next = x; 
		val = element -> key ;
    kfree ( element ) ;
    lst -> size -- ;
    }
	return val ;
}
void * pop_item ( list * lst ) {
	void * i = remove_item ( lst , lst -> rear -> prev ) ;
	return i;
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yunjin Lee");
MODULE_VERSION("0.01");
