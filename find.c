#include "include/list.h"
#include <linux/string.h>
// find node
node * find ( list * lst, bdat_t * key ) { 
	node * p; //we use this node as cursor
	p = lst -> front -> next ; //so, starting from first node
	while ( p != lst -> rear ) { //find it till the end.
		if ( !memcmp( p , lst -> rear , sizeof(node) ) ) { 
			break;
		}
		if (!memcmp((int16_t)p->key->_uuid,(int16_t)key,sizeof(int16_t))) { //if same key is detected
			break; //break the loop.
		}
		p = p -> next ; //go to the next node
	}
	return p;//return the cursor; If there is no nodes with your provided data, it returns rear node.
}
node * index_node ( list * lst , ll i ) { /*find target node's index. You can use your list
					   like an array with this */
	node * n;
	ll k ;
	if ( i > lst -> size / 2) { //if its location is close to rear,
		n = lst -> rear -> prev; //it starts finding from last node.
		for ( k = 0 ; k < lst -> size - i + 1 ; k ++ ) { 
			if ( n == lst -> front ) { //find your node till the first data comes
				return lst -> rear; /*but when there is no data, it returns rear node
						      for integrated form of error report */
			}
			n = n -> prev; //go to the previous node
		}
	} else {
		n = lst -> front -> next ; /*in this case, your index is closer to front;
					     so starting from first node.*/
		for ( k = 0 ; k < i ; k ++ ) {
			if ( n == lst -> rear ) { //find till the last node comes out!
				break; //finally, last node is here. break this loop
			}
		}
		n = n -> next; //go to the next node
	}
	return n; //and return
}
MODULE_LICENSE("GPL");
