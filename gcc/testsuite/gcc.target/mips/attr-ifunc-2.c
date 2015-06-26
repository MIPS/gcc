/* Test the ifunc and target attributes.  If the target supports MSA then we
   expect that the optimized version of function will be executed.  */
/* { dg-require-ifunc "" } */
/* { dg-do run } */
/* { dg-require-effective-target mips_ifunc_callback } */
/* { dg-require-effective-target mips_msa_runtime } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-fuse-linker-plugin" } { "" } } */
/* { dg-output "MSA: yes(\n|\r\n|\r)+" } */
/* { dg-output "executing add_optimized(\n|\r)+" } */
#include <stdio.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/dlifnctl.h>

#define whereami() printf("executing %s\n", __FUNCTION__)

#define print_hw(feature, hw_mask)	\
  printf("%s: %s\n", feature, getauxval (AT_HWCAP) & (hw_mask) ? "yes" : "no")

#define N 8
int a[N], b[N], c[N];
int ref[] = {0,2,4,6,8,10,12,14};

int add() __attribute__((ifunc("add_resolver")));

void add_generic()
{
  whereami();
  for (int i = 0; i < N; i++)
    c[i] = a[i] + b[i];
}

__attribute__((target("msa"),optimize("tree-vectorize")))
void add_optimized()
{
  whereami();
  for (int i = 0; i < N; i++)
    c[i] = a[i] + b[i];
}

static void *add_resolver (int hwcap, int (*callback) (int, int))
{
  /* Check if MSA is available and can switch into FR1 mode.  */
  if (hwcap & HWCAP_MIPS_MSA
      && callback (DL_IFUNC_MIPS_FR_MODE_SWITCH,
		   MIPS_FP_MODE_FR1) == MIPS_FP_MODE_FR1)
    return add_optimized;
  else
   return add_generic;
}

int main (int *argc, char *argv)
{
  print_hw("MSA", HWCAP_MIPS_MSA);

  for (int i = 0; i < N; i++)
    a[i] = b[i] = i;

  add();

  for (int i = 0; i < N; i++)
    if (c[i] != ref[i])
      abort();

  return 0;
}
