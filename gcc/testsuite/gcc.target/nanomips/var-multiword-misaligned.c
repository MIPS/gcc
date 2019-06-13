/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -mcmodel=auto -mpcrel -mno-gpopt" } */


long long var_di __attribute__ ((aligned (1)));

long long read_di()
{
  return var_di;
}

void write_di(long long arg)
{
  var_di = arg;
}

/* { dg-final { scan-assembler-not "%lo\\(var_di\\\+4\\)" } } */
