#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#define CUSTOM_PRIV 0006774 
typedef struct timespec timespec;
typedef long long ll;
typedef struct tsData {
	unsigned char * data;
	timespec ts;	
} tsData;

typedef struct node {
	tsData key;
	struct node * prev;
	struct node * next;
} node;

typedef struct list {
	node *front;
	node *rear;
	ll size; 
} list;

void init_list ( list * lst );
void reset_list ( list * lst );
void between  ( list * lst , tsData item , node * r , node * element ) ;
void push ( list * lst , tsData item ) ;
void push_list ( list * lst , list * lst_target ) ;

tsData remove_item ( list * lst , node * element ) ;
tsData pop_item ( list * lst ) ;
ll size ( list *lst ) ;

void repair_size ( list *lst ) ;
_Bool empty ( list * lst ) ;
void show ( list * lst ) ;
void free_list ( list * lst ) ;
void empty_list ( list * lst ) ;
node * find ( list * lst , tsData key ) ;
node * index_node ( list * lst , ll i ) ;

void beautify(list *lst) ;
void swap_void ( tsData a, tsData b ); 

int __init init_device (void);
void __exit clean_device(void);
int device_open (struct inode * , struct file *);
int device_release (struct inode * , struct file * );
ssize_t device_read (struct file * , char * , size_t , loff_t *);
ssize_t device_write (struct file * , const char * , size_t , loff_t *);
