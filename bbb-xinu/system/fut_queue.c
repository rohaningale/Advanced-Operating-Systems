#include "fut_queue.h"
#include<xinu.h>


int fut_enqueue(qid16 qId, int qtype, pid32 pId){
		int front, rear;
		int prev;
		if(isBadQid(qId) || isbadpid(pId)){
			return SYSERR;
		}
		/*
		if(qtype == 0 && count0 == NPRO){
			// if set_queue if full
			printf("\n SHARED mode: Couldn't add to set_queue");
			return SYSERR;
		}
		if(qtype == 1 && count1 == NPRO){
			// if get_queue if full
			printf("\n SHARED mode: Couldn't add to get_queue");
			return SYSERR;
		}
		if(qtype == 2 && count2 == NPRO){
			// if set_queue if full
			printf("\n QUEUE mode: Couldn't add to set_queue");
			return SYSERR;
		}
		if(qtype == 3 && count3 == NPRO){
			// if get_queue if full
			printf("\n QUEUE mode: Couldn't add to get_queue");
			return SYSERR;
		}
		*/
		if(count[qtype] == NPRO){
			printf("\n Q full.");
			return SYSERR;
		}
		rear = futQueueTail(qId);
		
		prev = futQ[qtype][rear].prev;
		
		futQ[qtype][(prev+1)%NPRO].prev = prev;
		futQ[qtype][(prev+1)%NPRO].next = rear;

		futQ[qtype][prev].next = (prev+1)%NPRO;
		futQ[qtype][rear].prev = (prev+1)%NPRO;

		futQ[qtype][(prev+1)%NPRO].pid = pId;
	/*
		futQ[qtype][curr].prev = prev;
		futQ[qtype][curr].next = rear;
		futQ[qtype][rear].prev = curr;
		futQ[qtype][prev].next = curr;
	*/
		//printf("\n Enqueuing %d.", pId);		
		count[qtype]++;
		//printQ(qtype, qId);
		
		return OK;		
}

pid32 fut_dequeue( qid16 qId, int qtype){
		int front, rear;
		int next; int32 pid;
		
		if(isBadQid(qId) || isEmpty(qtype,qId)){
			return SYSERR;
		}
		
		front = futQueueHead(qId);	// getting the index of head
		next = futQ[qtype][front].next;	// pointing to the first item
		futQ[qtype][front].next = futQ[qtype][next].next;	// stitching head to second item or to the tail if last item is dequeued
		futQ[qtype][futQ[qtype][next].next].prev = front;	// stitching the head
		// sanity cleaning
		pid = futQ[qtype][next].pid;
		futQ[qtype][next].next = futQ[qtype][next].prev = futQ[qtype][next].pid = EMPTY;
		
		//printf("\n Dequeuing %d.", next);
		count[qtype]--;		
		//printQ(qtype, qId);
		
		return (pid32) pid;
}

int isEmpty(int qtype){
	if(count[qtype] == 0)
		return 1;
	return 0;
}

void printQ(int qtype, qid16 qId){
	int tail, head;
	head = futQueueHead(qId);
	tail = futQueueTail(qId);
	while(futQ[qtype][head].next != tail){
		printf(" %d", futQ[qtype][futQ[qtype][head].next].pid);
		head = futQ[qtype][head].next;
	}
	printf("count-(%d)", count[qtype]);
}

