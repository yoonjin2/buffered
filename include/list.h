#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <asm/uaccess.h>

#define BUFFER_MAX 10000

typedef long long ll;

typedef struct bdat_t {
	int16_t _uuid;
	int32_t data;
} bdat_t;


typedef struct node {
	bdat_t * key;
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
int8_t between  ( list * lst , bdat_t * item , node * r , node * element ) ;
int8_t enqueue ( list * lst , bdat_t * item ) ;
void enqueue_list ( list * lst , list * lst_target ) ;

bdat_t * remove_item ( list * lst , node * element ) ;
bdat_t * dequeue ( list * lst ) ;
ll size ( list *lst ) ;

void repair_size ( list *lst ) ;
_Bool empty ( list * lst ) ;
void show ( list * lst ) ;
void free_list ( list * lst ) ;
void empty_list ( list * lst ) ;
node * find ( list * lst , bdat_t * key ) ;
node * index_node ( list * lst , ll i ) ;

void beautify(list *lst) ;
void swap_void ( bdat_t * a, bdat_t * b ); 

int __init init_device (void);
void __exit clean_device(void);
int device_open (struct inode * , struct file *);
int device_release (struct inode * , struct file * );
ssize_t device_read (struct file * , char * , size_t , loff_t *);
ssize_t device_write (struct file * , const char * , size_t , loff_t *);
