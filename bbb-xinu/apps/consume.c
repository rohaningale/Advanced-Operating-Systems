#include "prodcons.h"

void consumer(int count)
{
   while(n<=count)
   {
     wait(produced);
     printf("\n\n Consumer consumed element: %d",n);
     signal(consumed);
   }
   semdelete(produced);
   semdelete(consumed);
}


