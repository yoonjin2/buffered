#include <stdio.h>
#define MAJOR 255
#define DEVNAME "buf"
typedef long long ll;

typedef struct node {
	void * key;
	struct node * prev;
	struct node * next;
} node;

typedef struct list {
	node *front;
	node *rear;
	ll size; 
	_Bool is_sorted;
} list;

void init_list ( list * lst );
void reset_list ( list * lst );
void between  ( list * lst , void * item , node * r , node * element ) ;
void regen_sorted ( list * lst );
void push ( list * lst , void * item ) ;
void push_list ( list * lst , list * lst_target ) ;

void * remove_item ( list * lst , node * element ) ;
void * pop_item ( list * lst ) ;
ll size ( list *lst ) ;

void repair_size ( list *lst ) ;
_Bool empty ( list * lst ) ;
void show ( list * lst ) ;
void free_list ( list * lst ) ;
void empty_list ( list * lst ) ;
node * find ( list * lst , void * key ) ;
node * index_node ( list * lst , ll i ) ;

void sort_func ( list * lst ) ;
void sortthree ( list * lst ) ;
void bubble_sort ( list * lst );
void beautify(list *lst) ;
void swap ( void * a, void * b ); 

int init_module (void);
void cleanup_module(void);
static int device_open (struct inode * , struct file *);
static int device_release (struct inode * , struct file * );
static ssize_t device_read (struct file * , char * , size_t , loff_t *);
static ssize_t device_write (struct file * , const char * , size_t , loff_t *);
int major;
