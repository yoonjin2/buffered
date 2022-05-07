#include "list.h"
#include <string.h>
void swap ( element_t * a , element_t * b ) {
	if ( memcmp((void *)a , (void *)b , sizeof(element_t)) == 0 ) {
		return;
	}
	element_t tmp=*a;
	*a=*b;
	*b=tmp;
}

