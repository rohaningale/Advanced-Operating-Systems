#include<xinu.h>
#include "future.h"
#include "fut_queue.h"


syscall future_set(future *f, int *value)
{
   intmask mask;
   int retVal;
   mask = disable();
   if(f==NULL)
   {
	restore(mask);
	return SYSERR;
   }
   else
   {
		switch(f->flag){
			case FUTURE_EXCLUSIVE:
				retVal = future_set_exclusive(f, value); 
				break;
			case FUTURE_SHARED:
				retVal = future_set_shared(f, value);
				break;
			case FUTURE_QUEUE:
				restore(mask);
				retVal = future_set_queue(f, value);
				break;
		}
		restore(mask);
		return retVal;
   }
}

syscall future_set_exclusive(future *f, int *value){
	
	if(f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING)
	{
		f->value = *value;
		wait(printmutex);
		
		kprintf(" [X] %d Produced:%d\n",getpid(),f->value);
		signal(printmutex);
		f->state = FUTURE_VALID;
		
		return OK;
	}
    else
	{	
		return SYSERR;
	}
}

syscall future_set_shared(future *f, int *value){
	if(f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING)
	{
		f->value = *value;
		

		kprintf(" [S] %d Produced:%d\n",getpid(),f->value);
		
		f->state = FUTURE_VALID;

		while(!isEmpty(1))
			{	//kprintf("\n dequeue from get_queue");
				//wait(mutex);
				pid32 pid = fut_dequeue(f->get_queue, 1);
				//signal(mutex);
				// send to future_cons 
				send(pid, (umsg32)pid); 
			}
		return OK;
	}else{
		return SYSERR;
	}
		
	
}

syscall future_set_queue(future *f, int *value){

	intmask mask = disable();
	//kprintf("\n in futset queue ");
	if(!isEmpty(3)){

		pid32 pid;
		restore(mask);
		while(f->state != FUTURE_EMPTY && f->state != FUTURE_WAITING){
			// some consumer is consuming, so wait
			kprintf("");
		}
		// some consumer waiting for value from future
		/*
		*  set value and make it FUTURE_VALID
		*/
		mask = disable();
		f->value = *value;
		
		kprintf(" [Q] %d Produced:%d\n",getpid(),f->value);
		
		f->state = FUTURE_VALID;
		//kprintf("\nprod > QMode > ");
		//kprintf("\nProduced:%d\n",f->value);
		//kprintf("\n dequeue from get_queue");
		//wait(mutex);
		pid = fut_dequeue(f->get_queue, 3);
		//signal(mutex);
		// send to future_cons 
		send(pid, (umsg32)getpid());
		restore(mask);
		return OK;
	}else if(f->state == FUTURE_EMPTY || f->state == FUTURE_VALID){
		// if FUTURE_EMPTY i.e. no consumer process is running or
		// value is not set i.e. FUTURE_VALID
		// enqueue in set_queue
		//kprintf("\n %d enqueue in set_queue.",getpid());
		wait(mutex);
		fut_enqueue(f->set_queue, 2, getpid());
		signal(mutex);
		restore(mask);
		
		kprintf("\n [Q] Recv from %d. ",receive());
		//receive();
		// when receive() returns, it means consumer process is ready to consume.
		/*
		*  set value and make it FUTURE_VALID
		*/ 
		mask = disable();
		f->value = *value;
		
		kprintf(" [Q] %d Produced:%d\n",getpid(),f->value);
		f->state = FUTURE_VALID;
		//kprintf(" > QMode > ");
		//kprintf("\nProduced:%d\n",f->value);
		restore(mask);
		return OK;
	}
	return SYSERR;	
}
