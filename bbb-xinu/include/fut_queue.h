#include<kernel.h>

#ifndef NPRO
#define NPRO 100
#endif

#define NBOUND	NPRO + 50

#ifndef EMPTY
#define EMPTY -1
#endif

//static int count0=0, count1=0;  // shared mode
//static int count2=0, count3=0;  // queue mode
static int count[4]={0};
typedef struct futQEntry{
	int next, prev;
	pid32 pid;
}futQEnt;

extern futQEnt futQ[][NBOUND];

#define futQueueHead(q)		q
#define futQueueTail(q)		((q) + 1)
#define frontId(type,q)			(futQ[(type)][futQueueHead(q)].next)
#define rearId(type,q)			(futQ[(type)][futQueueTail(q)].prev)
#define isBadQid(q)			((q < 0) || (q > NBOUND))
//#define isEmpty(type,q)		(count[(type)] == 0)


