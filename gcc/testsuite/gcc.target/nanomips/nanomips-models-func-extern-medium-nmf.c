/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -fpic" } */
/* { dg-skip-if "" { *-*-* } { "-O1" "-O2" "-O3" "-Os" } { "" } } */

/* lw reg, %got_disp(f_extern)($gp) # 1 */
/* jalrc reg # 1 */

/* lw reg, %got_disp(f_extern)($gp) # 2 */
/* balc f_takes_fptr */

/* lw reg, %got_disp(f_extern)($gp) # 3 */
/* lapc[48] reg, f_long_takes_fptr */
/* jalrc reg # 2 */

/* { dg-final { scan-assembler-times "lw\t\\\$\[ast0-9\]+,%got_disp\\(f_extern\\)\\(\\\$gp\\)" 3 } } */
/* { dg-final { scan-assembler "balc\tf_takes_fptr" } } */
/* { dg-final { scan-assembler "lapc\\\[48\\]\t\\\$\[ast0-9\]+,f_long_takes_fptr" } } */

/* { dg-final { scan-assembler-times "jalrc\t\\\$\[ast0-9\]+" 2 } } */
/* { dg-final { scan-assembler "aluipc\t\\\$gp,%pcrel_hi\\(_gp\\)" } } */

__attribute__ ((noclone, noinline))
extern int f_extern(void) { asm (""); return 1; }

__attribute__ ((noclone, noinline))
static int f_takes_fptr(int bar()) { asm (""); return 1; }

__attribute__ ((noclone, noinline, long_call))
static int f_long_takes_fptr(int bar()) { asm (""); return 1; }

int
foo ()
{
  f_extern();
  f_takes_fptr(&f_extern);
  f_long_takes_fptr(&f_extern);

  return 0;
}
