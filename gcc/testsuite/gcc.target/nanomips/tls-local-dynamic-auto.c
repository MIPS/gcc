/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -fpic -mcmodel=auto -mtls-dialect=trad -ftls-model=local-dynamic" } */

/* Local dynamic is overriden with global dynamic for nanoMIPS.  */

/* { dg-final { scan-assembler-times "lapc\t\\\$gp,_gp" 3 } } */
/* { dg-final { scan-assembler-not "rdhwr\t\\\$\[ast0-9\]+,\\\$29" } } */

__thread int tls_var;

/* { dg-final { scan-assembler "f_addr:.*\taddiu\\.w\t\\\$a0,\\\$gp,%tlsgd\\(tls_var\\).*f_addr\n" } } */
/* { dg-final { scan-assembler "f_addr:.*__tls_get_addr.*f_addr\n" } } */
int *
f_addr ()
{
  int *x;
  x = &tls_var;
  return x;
}

/* { dg-final { scan-assembler "f_load:.*\taddiu\\.w\t\\\$a0,\\\$gp,%tlsgd\\(tls_var\\).*f_load\n" } } */
/* { dg-final { scan-assembler "f_load:.*__tls_get_addr.*f_load\n" } } */
/* { dg-final { scan-assembler "f_load:.*\tlw\t\\\$\[ast0-9\]+,0\\(\\\$\[ast0-9\]+\\).*f_load\n" } } */
int
f_load ()
{
  int x;
  x = tls_var;
  return x;
}

/* { dg-final { scan-assembler "f_store:.*\taddiu\\.w\t\\\$a0,\\\$gp,%tlsgd\\(tls_var\\).*f_store\n" } } */
/* { dg-final { scan-assembler "f_store:.*__tls_get_addr.*f_store\n" } } */
/* { dg-final { scan-assembler "f_store:.*\tsw\t\\\$\[ast0-9\]+,0\\(\\\$\[ast0-9\]+\\).*f_store\n" } } */
int
f_store ()
{
  tls_var = 11;
  return 0;
}
