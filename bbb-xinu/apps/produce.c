#include "prodcons.h"

void producer(int count)
{
   while(n<count)
   {	
     wait(consumed);
     n++;
     printf("\n\n Producer produced element: %d",n);
     signal(produced);
   }
}
