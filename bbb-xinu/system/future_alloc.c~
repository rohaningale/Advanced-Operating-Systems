#include "future.h"
//#include "fut_queue.h"

/* future_alloc.c */
future* future_alloc(int future_flags){
	future *f = NULL;
	if(future_flags == FUTURE_EXCLUSIVE || future_flags == FUTURE_SHARED || future_flags == FUTURE_QUEUE){
		/* 
		 * Allocate memory for future.
		 */
		f = (future*) getmem(sizeof(future));
		if(f != NULL){
			/* if memget doesn't return any error */
			f->value = NULL;
			//f->flag = FUTURE_EXCLUSIVE;
			f->flag = future_flags;
			f->state = FUTURE_EMPTY;
			f->pid = NULL;
			/* Initialize the set_queue and get_queue ids of f
			*/
			if(future_flags != FUTURE_EXCLUSIVE){
				if(initQ(future_flags, f) == SYSERR){
				kprintf("\n Initialization of set_queue and get_queue failed.");
				// sanity check : free the future
				future_free(f);
				return NULL;
				}
			}
		}
	}
	return f;
}
