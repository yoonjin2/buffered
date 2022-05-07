#include <stdio.h>
#include "list.h"
//show list
void show( list *lst ) {
    node * t;
    for ( t = lst -> front -> next ; t != lst -> rear ; t = t -> next ) {
				if ( t -> key.coef > 0 ) {
				  printf ("+");
				} 
				if ( t->key.var != ' ') {
          printf ( "%d%c^%d" , t -> key.coef , t -> key.var, t -> key.expo) ;
				} else {
          printf ( "%d^%d" , t -> key.coef , t -> key.expo) ;
				}
    }
		printf ("\n");
}
