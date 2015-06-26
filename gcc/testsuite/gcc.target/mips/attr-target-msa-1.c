/* Test function specific options, check generation of MSA code.  Functions
   without target attribute should not have any MSA code.  */
/* { dg-compile } */
/* { dg-require-effective-target mips_msa } */
/* { dg-options "-mips32r2 -mno-msa -mfpxx -ftree-vectorize" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-Os" } { "" } } */

/* { dg-final { scan-assembler-times "\.set\tmsa" 4 } } */
/* { dg-final { scan-assembler-times "\.set\tfp=64" 4 } } */
/* { dg-final { scan-assembler-times "addv.w" 2 } } */
/* { dg-final { scan-assembler-times "mulv.w" 2 } } */

#define N 4

int a[N];
int b[N];
int c[N];

__attribute__ ((target("msa")))
void
test_msa ()
{
  int i;
  for (i = 0; i < N; i++)
    c[i] = a[i] + b[i];
}

typedef int v4i32 __attribute__((vector_size(16)));
v4i32 av[N], bv[N], cv[N];

__attribute__ ((target("msa")))
v4i32
test_msa2 ()
{
  cv[0] = av[0] + bv[0];
}

v4i32
test_nomsa ()
{
  cv[0] = av[0] + bv[0];
}

__attribute__ ((target("msa")))
v4i32
test_msa3 (v4i32 *a, v4i32 *b)
{
  return *a * *b;
}

void
test_nomsa2 (v4i32 *a, v4i32 *b, v4i32 *c)
{
  *c = *a * *b;
}

__attribute__ ((target("msa")))
void
test_msa4 (v4i32 *a, v4i32 *b, v4i32 *c)
{
  *c = *a * *b;
}
