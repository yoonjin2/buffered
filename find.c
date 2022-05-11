#include "list.h"
#include <linux/string.h>
// find node
node * find ( list * lst, void * key ) {
	node * p;
	p = lst -> front -> next ;
	while ( p != lst -> rear ) {
		if ( !memcmp( p , lst -> rear , sizeof(void *) ) ) {
			break;
		}
		p = p -> next ;
	}
	return p;
}
node * index_node ( list * lst , ll i ) {
	node * n;
	ll k ;
	if ( i > lst -> size / 2) {
		n = lst -> rear -> prev;
		for ( k = 0 ; k < lst -> size - i + 1 ; k ++ ) {
			if ( n == lst -> front ) {
				return lst -> rear;
			}
			n = n -> prev;
		}
	} else {
		n = lst -> front -> next ;
		for ( k = 0 ; k < i ; k ++ ) {
			if ( n == lst -> rear ) {
				break;
			}
		}
		n = n -> next;
	}
	return n;
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yunjin Lee");
MODULE_VERSION("0.01");
