/* In n32 ABI rhe siginfo_t struct (si_addr) did not contain the correct 
   information when SEGV signal handler was called.  */
/* { dg-do run { target *-octeon-linux* } } */
#include <signal.h> 
#include <stdlib.h>
#include <stdio.h>
 
void saction (int n, siginfo_t *info, void *arg) 
{ 
  printf ("signo %d\nsi_addr %p\nsi_addr+4 %p\n", 
          info->si_signo, info->si_addr, ((void **)&info->si_addr)[1]); 
  if (info->si_addr == (int *)0x12345678)
    exit (0);
  abort (); 
} 

int 
main () 
{ 
  struct sigaction sa; 
  sa.sa_sigaction = saction; 
  sa.sa_flags =  SA_SIGINFO; 
 
  sigaction (SIGSEGV, &sa, 0); 
 
  *(int *)0x12345678 = 0; 

  return 0;
}
