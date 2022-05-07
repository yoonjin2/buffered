#include "list.h"
#include <linux/string.h>
#include <linux/slab.h>
// add number between two nodes
void between( list * lst , void * item , node * b , node * n ) {
    node * element ;
    if ( ( element = ( node * ) kmalloc ( sizeof ( node ) , GFP_KERNEL ) ) == NULL ) {
        return ;
	}
	if ( ( b != n -> prev ) ) {
		kfree( element );
		return ;
	}
    else {
        element -> key = item ;
        b -> next = element ;
        element -> prev = b;
        element -> next = n ;
	n -> prev = element ;
    	lst -> size ++ ;
    }
}
void push ( list * lst , void * item ) {
	between ( lst , item , lst -> rear -> prev , lst -> rear ) ;
}
void push_list ( list * lst , list * lst_target ) {
  list * target = kmalloc ( (size(lst_target)+2)*sizeof (node) * sizeof(ll) , GFP_KERNEL );
	node * push_node;
	node * target_node;
	memcpy ( target , lst_target , (size(lst_target)+2)*sizeof (node) * sizeof(ll) );
	push_node = lst -> rear -> prev;	
	target_node = target -> front -> next;
	push_node -> next = target_node;
	target_node -> prev = push_node;
	target_node -> next = lst -> rear;
	lst -> rear -> prev = target_node;
	lst -> size += target->size ;
	reset_list ( target );
	free_list  ( target );

}
