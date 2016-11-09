#include<xinu.h>
#include "future.h"
#include "fut_queue.h"



int initQ(int flag, future *f){
	// invoke from main like 
	// init(&f);

	/*
	* initialize head and tail of set_queue and get_queue
	*/
	int qtype;
	switch(flag){
		case FUTURE_SHARED:
			qtype = 0;	break; // FUTURE_SHARED mode
		case FUTURE_QUEUE:
			qtype = 2;	break; // FUTURE_QUEUE mode
	}	
	f->set_queue = f->get_queue = NPRO;
	futQ[qtype][f->set_queue].next = f->set_queue + 1;	// head
	futQ[qtype][f->set_queue].prev = EMPTY;
	futQ[qtype][f->set_queue].pid = EMPTY;

	futQ[qtype][f->set_queue+1].prev = f->set_queue;	// tail
	futQ[qtype][f->set_queue+1].next = EMPTY;
	futQ[qtype][f->set_queue+1].pid = EMPTY;

	futQ[qtype+1][f->get_queue].next = f->get_queue + 1;	//head
	futQ[qtype+1][f->get_queue].prev = EMPTY;
	futQ[qtype+1][f->get_queue].pid = EMPTY;

	futQ[qtype+1][f->get_queue+1].prev = f->set_queue;	// tail
	futQ[qtype+1][f->get_queue+1].next = EMPTY;
	futQ[qtype+1][f->get_queue+1].pid = EMPTY;



	/*

	futQ[qtype+1][f->get_queue]
	futQ[nextQId + 2].prev = nextQId + 1;
	futQ[nextQId + 1].prev = EMPTY;
	futQ[nextQId + 2].next = EMPTY;
	
	futQ[nextQId + 3].next = nextQId + 4;
	futQ[nextQId + 4].prev = nextQId + 3;
	futQ[nextQId + 3].prev = EMPTY;
	futQ[nextQId + 4].next = EMPTY;
	
	*/
	//kprintf("\nset_q: %d",f->set_queue);
	//kprintf("\nget_q: %d",f->get_queue);
	//nextQId += 4;
	
	return OK;	
}
