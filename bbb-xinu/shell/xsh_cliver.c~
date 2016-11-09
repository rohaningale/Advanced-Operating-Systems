#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prodcons.h"

#define TRUE 1


/* Global declarations */
sid32 mutex, printmutex;
int consCount=0;

int checkArgs(char *argv){
	int val = TRUE, negVal = !TRUE;
  	int len = strlen(argv);
	int i=0;
	if(*(argv) == '-'){
		/* a possible negative number is passed. */
		negVal = TRUE;
		i++;
	}
	while(i<len){   
		if( !((*(argv+i)-48) >= 0 && *(argv+i)-48 <=9)){
			val = !TRUE; break;
		}
		i++;
  	}
	if(val && negVal){
		/* input is a number */
		/* and input is negative */
		return -1;
	}
	return val;
}

void showUsage(){
	printf("\n Usage: cliver REMOTEIP PORT");
	printf("\n Description:\n");
	printf("\n Runs the cliver command with REMOTEIP and PORT number specified as the parameters\n");
	printf(" Options:\n");
	printf("\t REMOTEIP: \t IP address in dotted decimal\n");
	printf("\t PORT: \t Port number.\n");
	printf("\t--help\t display this help and exit\.n");
}


shellcmd xsh_cliver(int nargs, char *args[])
{
	int retVal=0, i=0;
	uint32 remoteIP=0;
	uint16 remotePort=0, localPort = 6006;
	uint32 slot = 0; 
	pid32 pid[20]={0};
	int prodCount = 0;
	/* futures */
	future *fut[3];

	/*Sanity check starts*/
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		showUsage();
		return 0;
	}

	if(nargs != 3){
		printf("\n %s: invalid argument(s)\n", args[0]);
		showUsage();
		return -1;
	}
	
	if(dot2ip(args[1], &remoteIP) == SYSERR){
		printf("\n REMOTEIP: Invalid IP address.");
		return -1;
	}

	
	retVal = checkArgs(args[2]);
	if(retVal == 0){
		printf("\n Argument passed is not a valid argument");
		showUsage();
	  	return -1;
  	}
  	if(retVal == -1){
		printf("\n Negative argument value.");
		showUsage();
	  	return -1;
	}

	remotePort = atoi(args[2]);
	
	if(remotePort < 1024 || remotePort > 65535){
		printf("\n PORT: Invalid port range.");
		return -1;
	}	

	/* Sanity check ends */
	consCount=0;
	mutex = semcreate(1);
	printmutex = semcreate(1);

	/*  */
	fut[0] = future_alloc(FUTURE_EXCLUSIVE);
	fut[1] = future_alloc(FUTURE_SHARED);
	fut[2] = future_alloc(FUTURE_QUEUE);

	/* Creating the producer process. */
	/* processes operating on future_exclusive */
	
	pid[0] = create(cliver_produce, 1024, 50, "cliver_produce0", 5, remoteIP, remotePort, localPort, getpid(), fut[0]);
	resume(pid[0]);
	prodCount++;

	pid[1] = create(future_cons, 1024, 50, "cliver_consume0", 2, fut[0], getpid());
	wait(mutex);
	consCount++;
	signal(mutex);
	resume(pid[1]);
	

	/* processes operating on future_shared */
	
	pid[2] = create(future_cons, 1024, 40, "cliver_consume1", 2, fut[1], getpid());
	wait(mutex);
	consCount++;
	signal(mutex);
	resume(pid[2]);

	pid[3] = create(future_cons, 1024, 40, "cliver_consume2", 2, fut[1], getpid());
	wait(mutex);
	consCount++;
	signal(mutex);
	resume(pid[3]);

	pid[4] = create(cliver_produce, 1024, 40, "cliver_produce1", 5, remoteIP, remotePort, localPort+1, getpid(), fut[1]);
	resume(pid[4]);
	prodCount++;
	
	/* processes operating on future_queue */
	
	
	pid[6] = create(cliver_produce, 1024, 30, "cliver_produce2", 5, remoteIP, remotePort, localPort+2, getpid(), fut[2]);
	resume(pid[6]);
	prodCount++;

	pid[5] = create(future_cons, 1024, 30, "cliver_consume3", 2, fut[2], getpid());
	wait(mutex);
	consCount++;
	signal(mutex);
	resume(pid[5]);
	

	
	pid[7] = create(cliver_produce, 1024, 20, "cliver_produce3", 5, remoteIP, remotePort, localPort+3, getpid(), fut[2]);
	resume(pid[7]);
	prodCount++;

	pid[8] = create(future_cons, 1024, 20, "cliver_consume4", 2, fut[2], getpid());
	wait(mutex);
	consCount++;
	signal(mutex);
	resume(pid[8]);
	
	
	while(consCount != 0){
		printf("");
	}
	
	printf("\n");
	for(i=0; i<3; i++){
		future_free(fut[i]);
		printf(" Future %d freed.\n",i);
		fut[i] = NULL;
	}
	
	
	for(i=0; i<prodCount; i++){
		printf(" Process %d killed.\n",pid[i]);
		kill(pid[i]);
	}
		
	return 0;
}

