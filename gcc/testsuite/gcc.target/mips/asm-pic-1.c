/* { dg-options "-mabi=n32" } */
/* { dg-final { scan-assembler-times "%gottprel\\(q\\)\\(\\\$28\\)" 2 } } */



__thread q __attribute__ ((tls_model ("initial-exec")));
f (int a) {
 register int arg1 asm ("$5");
 register void * cgp asm ("$28");
  __asm__ __volatile__ ("lw %0, %%gottprel(q)(%1)" : "=d" (arg1) : "r" (cgp));
 int j = a;
 j += q;

 return j;
}
