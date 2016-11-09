#include "future.h"
#include "fut_queue.h"

/* future_get.c */
syscall future_get(future* f, int* value){
	intmask mask;
	int c, retVal;        
    	mask = disable();
	if(f == NULL)
	{
		restore(mask);
		
		return SYSERR;
	}

	switch(f->flag){

		case FUTURE_EXCLUSIVE:
			restore(mask);
			retVal = future_get_exclusive(f, value); 
			break;
		case FUTURE_SHARED:
			restore(mask);
			retVal = future_get_shared(f, value);
			break;
		case FUTURE_QUEUE:
			restore(mask);
			retVal = future_get_queue(f, value);
			break;
	}
		
	return retVal;
		
}

syscall future_get_exclusive(future *f, int *value)
{
	intmask mask = disable();
	if(f->state == FUTURE_WAITING)
	{
		restore(mask);
		return SYSERR;
	}

	if(f->state == FUTURE_EMPTY)
	{
		f->pid = getpid();
		f->state = FUTURE_WAITING;
		restore(mask);
		/*
		 * while the state is not FUTURE_VALID, keep the thread in wait state
		 */
		
		while(f->state != FUTURE_VALID)
		{		
			kprintf("");
		}
		//kprintf("Outside while");
		mask = disable();
		//kprintf("After disabling interrupts");
	}
			
	/* 
	 * The future state value is valid.
	 * Get the value and change the state
	 */
	*value = f->value;
	wait(printmutex);
	

	kprintf(" [X] %d Consumed %d\n", getpid(), *value);
	signal(printmutex);

	f->state = FUTURE_EMPTY;
  	wait(mutex);
	consCount--;
	//printf("\n %d finished [%d].",getpid(),consCount);
	signal(mutex);
	restore(mask);
	return OK;

}

syscall future_get_shared(future *f, int *value)
{
	intmask mask = disable();
	 if(f->state == FUTURE_VALID)
	 {
		*value = f->value;
		wait(printmutex);
		

		kprintf(" [S] %d Consumed %d\n", getpid(), *value);
		signal(printmutex);
  		wait(mutex);
		consCount--;
		//printf("\n %d finished [%d].",getpid(),consCount);
		signal(mutex);
		restore(mask);
		return OK;
	 }
	 else if(f->state == FUTURE_EMPTY)
	 {
		 f->state == FUTURE_WAITING;
	 }
	//kprintf("\n enqueue in get_queue");
	//wait(mutex);
	fut_enqueue( f->get_queue, 1, getpid());
	//signal(mutex);
	restore(mask);
	receive();
	mask = disable();
	*value = f->value;
	

	kprintf(" [S] %d Consumed %d\n", getpid(), *value);
	wait(mutex);
	consCount--;
	//printf("\n %d finished [%d].",getpid(),consCount);
	signal(mutex);
	restore(mask);
	return OK; 
}

syscall future_get_queue(future *f,int *value)
{
	intmask mask = disable();
	//kprintf("\n in futget queue.");
	if(isEmpty(2)){

		// no producer process running
		// enqueue in get_queue

		f->state = FUTURE_WAITING;
		//kprintf("\n %d enqueue in get_queue", getpid());
		//wait(mutex);
		fut_enqueue(f->get_queue, 3, getpid());
		//signal(mask);
		restore(mask);
		
		kprintf("\n [Q] Recv from %d. ",receive());
		//receive();
		
		// wait for any consumer process to access the future value
		mask = disable();
		*value = f->value;
		
		kprintf("[Q] %d Consumed %d\n", getpid(), *value);
		wait(mutex);
		consCount--;
		//printf("\n %d finished [%d].",getpid(),consCount);
		signal(mutex);
		f->state = FUTURE_EMPTY;
		//kprintf("\n cons > QMode > ");
		//kprintf("%d Consumed %d\n", getpid(), *value);
		restore(mask);
		return OK;
	}else{
		// consumer process waiting in the get_queue
		// dequeue the consumer process

		//kprintf("\n dequeue from set_queue");
		//wait(mutex);
		pid32 pid = fut_dequeue(f->set_queue, 2);
		//wait(mutex);
		send(pid, (umsg32)getpid());
		restore(mask);
		while(f->state != FUTURE_VALID)
		{		
			kprintf("");
		}
		mask = disable();
		*value = f->value;
		

		kprintf("[Q] %d Consumed %d\n", getpid(), *value);
		wait(mutex);
		consCount--;
		//printf("\n %d finished [%d].",getpid(),consCount);
		signal(mutex);
		f->state = FUTURE_EMPTY;
		//kprintf(" > QMode > ");
		//kprintf("%d Consumed %d\n", getpid(), *value);
		restore(mask);
		return OK;
	}
	restore(mask);
	return SYSERR;
}
