#include <stddef.h>
#include <kernel.h>
#include <stdio.h>
//#include "fut_queue.h"

#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2         

/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  	1	
#define FUTURE_SHARED  		2
#define FUTURE_QUEUE  		3

extern sid32 mutex, printmutex;
extern int totalConsumers;
extern int consCount;

typedef struct futent
{
   int value;		
   int flag;		
   int state;         	
   pid32 pid;
   qid16 set_queue;
   qid16 get_queue;
} future;


/* Interface for system call */
future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);
int initQ(int flag, future *);

