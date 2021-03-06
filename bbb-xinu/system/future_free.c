#include<xinu.h>
#include "future.h"
#include "fut_queue.h"

syscall future_free(future *f)
{
   syscall i;
   intmask mask;
   mask = disable();
   /*if(freemem((void *)f, sizeof(future))==SYSERR)
   {
	fprintf(stderr,"\nMemory free operation failed!!!");
        restore(mask);
   }*/
   i = freemem((void *)f, sizeof(future));

   int qtype;
   switch(f->flag){
		case FUTURE_SHARED:
			qtype = 0;	
			free_queue(f, qtype);			
			break; // FUTURE_SHARED mode
		case FUTURE_QUEUE:
			qtype = 2;
			free_queue(f, qtype);			
			break; // FUTURE_QUEUE mode
   }
   
   
   restore(mask);
   return i;
}

int free_queue(future *f, int qtype){ 
	// cleaning up the queues (set and get)
   futQ[qtype][f->set_queue].next = (f->set_queue)+1;

   futQ[qtype][f->set_queue+1].prev = (f->set_queue);

   futQ[qtype+1][f->get_queue].next = (f->get_queue)+1;

   futQ[qtype+1][f->get_queue+1].prev = (f->get_queue);	

   count[qtype] = count[qtype+1] = 0;

}
