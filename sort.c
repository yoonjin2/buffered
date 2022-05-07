#include "list.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
//sort by exponent;start from the biggest value;
void sortthree ( list * lst ) {
	node * mid = index_node ( lst , ( ll ) ( ( float ) ( lst -> size / 2 ) ) ) ;
	node * front = lst -> front -> next ;
	node * rear = lst -> rear -> prev ;
	if ( front -> key.expo < mid -> key.expo ) {
		swap ( &front -> key , &mid -> key );
	}
	if ( front -> key.expo < rear -> key.expo ) {
		swap ( &front -> key , &rear -> key );
	} 
	if (mid -> key.expo < rear -> key.expo) {
		swap ( &mid -> key , & rear -> key );
	}
}


void sort_func ( list * lst ) {

	if ( lst -> size < 2 ) {
		return;
	}

	list llst, rlst ;
	init_list ( &llst );
	init_list ( &rlst );
	sortthree( lst );
	node * piv = index_node ( lst , ( ll )(size(lst)/2) ) /*lst -> front -> next*/ , * track = lst -> front -> next ;
	element_t key = piv -> key;
	for ( ; track != lst -> rear ; track = track -> next ) {
		if ( piv  == track ) {
//		  track = track ->next;
			continue;
		}
		if ( (track -> key.expo > piv -> key.expo) ) {
			push ( &llst , track -> key ) ;
		} else {
			push ( &rlst , track -> key );
		} 
	}

	if ( llst.size > 1 ) {
		sort_func ( &llst ) ;
	}
	if ( rlst.size > 1 ) {
		sort_func ( &rlst ) ;
	}
	if ( llst.size > 0 ) { 
		node * f = llst.front -> next ;
		node * r = llst.rear -> prev ;
		node * left = lst -> front ;
		left -> next = f ;
		f -> prev = left ;
		lst -> rear -> prev = r;
		r -> next = lst -> rear ;
	}
	push( lst , key );
	if ( rlst.size > 0 ) { 
		node * f = rlst.front -> next;
		node * r = rlst.rear -> prev;
		node * right = lst -> rear -> prev;
		right -> next = f;
		f -> prev = right;
		r -> next = lst -> rear;
		lst -> rear -> prev = r;
	}
	lst -> is_sorted = true;
}
 
void beautify(list *lst) {
	node * nd_2 = lst -> front -> next; //two nodes are
	node * nd = lst -> front -> next ; //here;
  for (nd = lst -> front -> next;nd != lst -> rear ;nd=nd->next) {
    for(nd_2 = lst -> front -> next;nd_2 != lst->rear ;nd_2=nd_2->next) {
  	  if (!memcmp(nd,nd_2,sizeof(node))) {
				continue; //if two of them is same, continue
  		}
    	if ( (nd->key.var == nd_2->key.var) && (nd->key.expo == nd_2->key.expo) ) {
      	  nd->key.coef += nd_2->key.coef; //merge the coef`
					node * sav = nd_2; //Node that will be free
					nd_2 = nd_2 ->next;//go to the next node to avoid error
  				remove_item(lst,sav);//free NODE!
      }
  	}
  }
  for (nd = lst -> front -> next;nd != lst -> rear ;nd=nd->next) {
    if(nd->key.coef == 0 ){ 
		  remove_item(lst,nd); //delete if coef == 0 , it is useless
	  }
	}
}


