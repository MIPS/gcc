#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sched.h>

static void *
func (void *p)
{
  int thr = (int) p;
  unsigned i;
  
  for (i=0; i<100; i++)
    {
      /* fprintf (stderr, "thread %d iteration %u REG\n", thr, i); */
      {
	int array[17];
	unsigned x = i % (sizeof(array)/sizeof(array[0]));
	/* VRP could prove that x is within [0,16], but until then, the
	   following access will ensure that array[] is registered to
	   libmudflap. */
	array[x] = i;
      }
      /* fprintf (stderr, "thread %d iteration %u UNREG/YIELD\n", thr, i); */
      sched_yield (); /* sleep (1); */
    }

  return (NULL);
}


int main ()
{
  int rc;
  unsigned i;
  pthread_t	threads[10];

  for (i=0; i<sizeof(threads)/sizeof(threads[0]); i++)
    {
      rc = pthread_create (& threads[i], NULL, func, (void *) i);
      if (rc) abort();
    }

  for (i=0; i<sizeof(threads)/sizeof(threads[0]); i++)
    {
      rc = pthread_join (threads[i], NULL);
      if (rc) abort();      
    }

  return 0;
}
