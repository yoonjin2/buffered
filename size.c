#include "list.h"

// return size of list
ll size ( list * lst ) {
	return lst -> size ;
}
void repair_size ( list * lst ) {
	node *nd ;
	ll sz = 0;
	for ( nd = lst -> front -> next ; nd != lst -> rear ; nd = nd -> next ) {
		sz ++;
	}
	lst -> size = sz;
		
}
// check if list is empty
bool empty ( list * lst ) {
    if ( lst -> size == 0 ) {
        return 1 ;
	}

    else {
        return 0 ;
	}
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yunjin Lee");
MODULE_VERSION("0.01");
