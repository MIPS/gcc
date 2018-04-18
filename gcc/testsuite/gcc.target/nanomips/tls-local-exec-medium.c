/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -mcmodel=medium -fpic -ftls-model=local-exec" } */

/* { dg-final { scan-assembler-times "rdhwr\t\\\$\[ast0-9\]+,\\\$29" 3 } } */

__thread int tls_var;

/* { dg-final { scan-assembler "f_addr:.*\taddiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%tprel\\(tls_var\\).*f_addr\n" } } */
int *
f_addr ()
{
  int *x;
  x = &tls_var;
  return x;
}

/* { dg-final { scan-assembler "f_load:.*\taddiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%tprel\\(tls_var\\).*f_load\n" } } */
/* { dg-final { scan-assembler "f_load:.*\tlw\t\\\$\[ast0-9\]+,0\\(\\\$\[ast0-9\]+\\).*f_load\n" } } */
int
f_load ()
{
  int x;
  x = tls_var;
  return x;
}

/* { dg-final { scan-assembler "f_store:.*\taddiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%tprel\\(tls_var\\).*f_store\n" } } */
/* { dg-final { scan-assembler "f_store:.*\tsw\t\\\$\[ast0-9\]+,0\\(\\\$\[ast0-9\]+\\).*f_store\n" } } */
int
f_store ()
{
  tls_var = 11;
  return 0;
}
