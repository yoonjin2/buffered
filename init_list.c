#include "list.h"
#include <linux/slab.h>
void init_list ( list * lst ) { // init list
  lst -> front = ( node * ) kmalloc ( sizeof ( node ) , GFP_KERNEL ) ;
  lst -> rear = ( node * ) kmalloc( sizeof ( node ) , GFP_KERNEL );
  lst -> front -> next = lst -> rear ;
  lst -> rear -> prev = lst -> front ;
  lst -> front -> prev = lst -> front -> prev;
  lst -> rear -> next = lst ->rear ;
  lst -> size = 0 ;
}

void reset_list ( list * lst ) { // init list
  lst -> front -> next = lst -> rear ;
  lst -> front -> prev = lst -> front -> prev;
  lst -> rear -> next = lst ->rear ;
  lst -> rear -> prev = lst -> front ;
  lst -> size = 0 ;
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yunjin Lee");
MODULE_VERSION("0.01");
