#include <linux/init.h>
#include <linux/ktime.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#define BUFFER_MAX 4096
typedef uint32_t time_t;
typedef long long ll;
typedef struct tsData {
	unsigned char * data;
	ktime_t ts;	
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
int8_t between  ( list * lst , tsData item , node * r , node * element ) ;
int8_t enqueue ( list * lst , tsData item ) ;
void enqueue_list ( list * lst , list * lst_target ) ;

tsData remove_item ( list * lst , node * element ) ;
tsData dequeue ( list * lst ) ;
ll size ( list *lst ) ;

void repair_size ( list *lst ) ;
int8_t full ( list * lst ) ;
void show ( list * lst ) ;
void free_list ( list * lst ) ;
void empty_list ( list * lst ) ;
node * find ( list * lst , unsigned char * key ) ;
node * index_node ( list * lst , ll i ) ;

void swap_struct ( tsData *a, tsData *b ); 

int __init init_device (void);
void __exit clean_device(void);
int device_open (struct inode * , struct file *);
int device_release (struct inode * , struct file * );
ssize_t device_read (struct file * , char * , size_t , loff_t *);
char * find_uuid(char * buf) ;
int find_dev (char * uuid) ;
ssize_t device_write (struct file * , const char * , size_t , loff_t *);
int8_t GENERATE_DEVICE (int minor);
char * GET_UUID (void) ;
