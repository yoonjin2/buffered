#include "include/list.h"
//show list
void show( list *lst ) {
    node * t;
    for ( t = lst -> front -> next ; t != lst -> rear ; t = t -> next ) {
			printk ( "%c" , *(char *)t->key );
    }
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yunjin Lee");
MODULE_VERSION("0.01");
