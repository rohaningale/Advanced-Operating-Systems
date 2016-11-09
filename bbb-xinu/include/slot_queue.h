#ifndef NSLOT
#define NSLOT	16
#endif

#define	EMPTY	(-1)		/* Null value for qnext or qprev index	*/
#define	MAXKEY	0x7FFFFFFF	/* Max key that can be stored in queue	*/
#define	MINKEY	0x80000000	/* Min key that can be stored in queue	*/

struct	slotentry	{		/* One per process plus two per list	*/
	int32	timestamp;		/* Key on which the queue is ordered	*/
	qid16	qnext;		/* Index of next process or tail	*/
	qid16	qprev;		/* Index of previous process or head	*/
};

extern	struct slotentry slottab[];

/* Inline queue manipulation functions */

#define	slotqueuehead(q)	(q)
#define	slotqueuetail(q)	((q) + 1)
#define	firstslotid(q)	(slottab[slotqueuehead(q)].qnext)
#define	lastslotid(q)	(slottab[slotqueuetail(q)].qprev)
#define	isemptyslotQ(q)	(firstslotid(q) >= NSLOT)
#define	nonemptyslotQ(q)	(firstslotid(q) <  NSLOT)
#define	firstslotkey(q)	(slottab[firstslotid(q)].timestamp)
#define	lastslotkey(q)	(slottab[ lastslotid(q)].timestamp)

/* Inline to check queue id assumes interrupts are disabled */

#define	isbadslotqid(x)	(x!=16)
