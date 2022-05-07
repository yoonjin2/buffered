#include "list.h"
//show list
void show( list *lst ) {
    node * t;
    for ( t = lst -> front -> next ; t != lst -> rear ; t = t -> next ) {
			printk ( "%c" , *(char *)t->key );
    }
}
