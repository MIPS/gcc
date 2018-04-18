/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -fpic -ftls-model=initial-exec" } */
/* { dg-skip-if "indexed load/store is not generated for -O0" { *-*-* } { "-O0" } { "" } } */

/* Note that this test checks for the same thing as in the auto case. We need
   it to be separate because the options for medium are different.  */

/* { dg-final { scan-assembler-times "lapc\t\\\$gp,_gp" 3 } } */
/* { dg-final { scan-assembler-times "rdhwr\t\\\$\[ast0-9\]+,\\\$29" 3 } } */

__thread int tls_var;

/* { dg-final { scan-assembler "f_addr:.*\tlw\t\\\$\[ast0-9\]+,%gottprel\\(tls_var\\)\\(\\\$gp\\).*f_addr\n" } } */
/* { dg-final { scan-assembler "f_addr:.*\taddu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,\\\$\[ast0-9\]+.*f_addr\n" } } */
int *
f_addr ()
{
  int *x;
  x = &tls_var;
  return x;
}

/* { dg-final { scan-assembler "f_load:.*\tlw\t\\\$\[ast0-9\]+,%gottprel\\(tls_var\\)\\(\\\$gp\\).*f_load\n" } } */
/* { dg-final { scan-assembler "f_load:.*\tlwx\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+\\(\\\$\[ast0-9\]+\\).*f_load\n" } } */
int
f_load ()
{
  int x;
  x = tls_var;
  return x;
}

/* { dg-final { scan-assembler "f_store:.*\tlw\t\\\$\[ast0-9\]+,%gottprel\\(tls_var\\)\\(\\\$gp\\).*f_store\n" } } */
/* { dg-final { scan-assembler "f_store:.*\tswx\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+\\(\\\$\[ast0-9\]+\\).*f_store\n" } } */
int
f_store ()
{
  tls_var = 11;
  return 0;
}
