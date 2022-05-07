#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <string.h>
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
void push ( list * lst , element_t item ) {
	between ( lst , item , lst -> rear -> prev , lst -> rear ) ;
}
void push_list ( list * lst , list * lst_target ) {
	auto unsigned ll SIZE = (size(lst_target)+2)*sizeof (node) * sizeof(ll);
  list * target = kmalloc ( SIZE , GFP_USER );
	memcpy ( target , lst_target , SIZE );
	node * push_node = lst -> rear -> prev;	
	node * target_node = target -> front -> next;
	push_node -> next = target_node;
	target_node -> prev = push_node;
	target_node -> next = lst -> rear;
	lst -> rear -> prev = target_node;
	auto unsigned ll lst_new_size_t = lst -> size + target->size;
	lst -> size = lst_new_size_t ;
	reset_list ( target );
	free_list  ( target );

}
