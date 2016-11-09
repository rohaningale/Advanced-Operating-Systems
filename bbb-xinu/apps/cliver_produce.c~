#include <xinu.h>
#include <stdio.h>
#include "future.h"

int32 cliver_produce(uint32 remoteIP, uint16 remotePort, uint16 localPort, pid32 pid, future *f){
	int msgLen=0, ret=0;
	char msg[]="This is a message from Xinu!";
	char recvBuf[10];//='\0';
	int32 recvInt;
	int32 timeout = 2000;
	intmask mask;
	int32 slot = udp_register(remoteIP, remotePort, localPort);

	if(slot == SYSERR){
		printf("\n Unable to reserve slot in the udp table. Exiting.");
		return -1;
	}
	//kprintf("\n Slot acquired.");
	
	msgLen = strnlen(msg,1500);
	if(msgLen <=0){
		printf("\n Invalid message.");
		udp_release(slot);
		return -1;
	}
	
	ret = udp_send(slot, msg, msgLen);
	if(ret == SYSERR){
		printf("\n Error: In udp_send.");
		udp_release(slot);
		return -1;
	}	

	ret = udp_recv(slot, recvBuf, sizeof(recvBuf), timeout);
	//ret = udp_recv(slot, recvInt, sizeof(int32), timeout);

	if(ret == TIMEOUT){
		printf("\n TIMEOUT: In udp_recv.");
		udp_release(slot);
		return -1;
	}else if(ret == SYSERR){
		printf("\n Error: In udp_recv.");
		udp_release(slot);
		return -1;
	}	
	recvInt = atoi(recvBuf);
	/* received from server */
	wait(printmutex);
	mask = disable();
	kprintf("\n Resp:(%d)",recvInt);
	restore(mask);
	signal(printmutex);

	/* release the registered udp slot */
	
	if(udp_release(slot)==SYSERR){
		printf("\n Error: Couldn't release udp slot.");
		return -1;
	}

	/* Setting the future value */
	if(future_set(f, &recvInt) == SYSERR){
		printf("\n ERROR: Could not produce value.\n");
		return -1;	
	}
	
	return 0;
}
