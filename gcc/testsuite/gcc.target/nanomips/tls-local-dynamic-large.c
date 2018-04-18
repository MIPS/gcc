/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -fPIC -mtls-dialect=trad -ftls-model=local-dynamic" } */
/* { dg-skip-if "indexed load/store is not generated for -O0" { *-*-* } { "-O0" } { "" } } */

/* { dg-final { scan-assembler-times "lapc\t\\\$gp,_gp" 3 } } */
/* { dg-final { scan-assembler-not "rdhwr\t\\\$\[ast0-9\]+,\\\$29" } } */

__thread int tls_var;

/* { dg-final { scan-assembler "f_addr:.*\taddiu\\.b32\t\\\$a0,\\\$gp,%tlsld\\(tls_var\\).*f_addr\n" } } */
/* { dg-final { scan-assembler "f_addr:.*\t__tls_get_addr.*f_addr\n" } } */
/* { dg-final { scan-assembler "f_addr:.*\tli48\t\\\$\[ast0-9\]+,%dtprel\\(tls_var\\).*f_addr\n" } } */
/* { dg-final { scan-assembler "f_addr:.*\taddu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,\\\$\[ast0-9\]+.*f_addr\n" } } */
int *
f_addr ()
{
  int *x;
  x = &tls_var;
  return x;
}

/* { dg-final { scan-assembler "f_load:.*\taddiu\\.b32\t\\\$a0,\\\$gp,%tlsld\\(tls_var\\).*f_load\n" } } */
/* { dg-final { scan-assembler "f_load:.*\t__tls_get_addr.*f_load\n" } } */
/* { dg-final { scan-assembler "f_load:.*\tli48\t\\\$\[ast0-9\]+,%dtprel\\(tls_var\\).*f_load\n" } } */
/* { dg-final { scan-assembler "f_load:.*\tlwx\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+\\(\\\$\[ast0-9\]+\\).*f_load\n" } } */
int
f_load ()
{
  int x;
  x = tls_var;
  return x;
}

/* { dg-final { scan-assembler "f_store:.*\taddiu\\.b32\t\\\$a0,\\\$gp,%tlsld\\(tls_var\\).*f_store\n" } } */
/* { dg-final { scan-assembler "f_store:.*\t__tls_get_addr.*f_store\n" } } */
/* { dg-final { scan-assembler "f_store:.*\tli48\t\\\$\[ast0-9\]+,%dtprel\\(tls_var\\).*f_store\n" } } */
/* { dg-final { scan-assembler "f_store:.*\tswx\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+\\(\\\$\[ast0-9\]+\\).*f_store\n" } } */
int
f_store ()
{
  tls_var = 11;
  return 0;
}
