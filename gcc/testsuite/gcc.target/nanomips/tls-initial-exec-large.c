/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -mcmodel=large -fPIC -ftls-model=initial-exec" } */
/* { dg-skip-if "indexed load/store is not generated for -O0" { *-*-* } { "-O0" } { "" } } */

/* { dg-final { scan-assembler-not "lapc\t\\\$gp,_gp" } } */
/* { dg-final { scan-assembler-times "rdhwr\t\\\$\[ast0-9\]+,\\\$29" 3 } } */

__thread int tls_var;

/* { dg-final { scan-assembler "f_addr:.*\tlwpc\t\\\$\[ast0-9\]+,%gottprel_pc32\\(tls_var\\).*f_addr\n" } } */
/* { dg-final { scan-assembler "f_addr:.*\taddu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,\\\$\[ast0-9\]+.*f_addr\n" } } */
int *
f_addr ()
{
  int *x;
  x = &tls_var;
  return x;
}

/* { dg-final { scan-assembler "f_load:.*\tlwpc\t\\\$\[ast0-9\]+,%gottprel_pc32\\(tls_var\\).*f_load\n" } } */
/* { dg-final { scan-assembler "f_load:.*\tlwx\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+\\(\\\$\[ast0-9\]+\\).*f_load\n" } } */
int
f_load ()
{
  int x;
  x = tls_var;
  return x;
}

/* { dg-final { scan-assembler "f_store:.*\tlwpc\t\\\$\[ast0-9\]+,%gottprel_pc32\\(tls_var\\).*f_store\n" } } */
/* { dg-final { scan-assembler "f_store:.*\tswx\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+\\(\\\$\[ast0-9\]+\\).*f_store\n" } } */
int
f_store ()
{
  tls_var = 11;
  return 0;
}
