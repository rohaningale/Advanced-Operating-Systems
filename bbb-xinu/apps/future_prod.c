#include "future.h"


int32 future_prod(future *fut) {
  int i, j;
  static int k=0;
  j = (int) fut;
  for (i=0; i<1000; i++) {
    j += i;
  }
  j+=k;
  k++;
  if(future_set(fut, &j) == SYSERR){
	printf("\n ERROR: Could not produce value.\n");
	return SYSERR;
  }
  //kprintf("\nProduced: %d \n",j);
  

  return OK;
}
