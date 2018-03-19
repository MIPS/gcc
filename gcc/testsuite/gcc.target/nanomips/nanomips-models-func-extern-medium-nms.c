/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6s -mcmodel=large -fpic -mpcrel" } */
/* { dg-skip-if "" { *-*-* } { "-O1" "-O2" "-O3" "-Os" } { "" } } */

/* lw reg, %got_disp(f_extern)($gp) # 1 */
/* jalrc reg # 1 */

/* lw reg, %got_disp(f_extern)($gp) # 2 */
/* aluipc reg, %pcrel_hi(f_takes_fptr) */
/* ori reg, reg, %lo(f_takes_fptr) */
/* jalrc reg # 2 */

/* lw reg, %got_disp(f_extern)($gp) # 3 */
/* aluipc reg, %pcrel_hi(f_long_takes_fptr) */
/* ori reg, reg, %lo(f_long_takes_fptr) */
/* jalrc reg # 3 */

/* { dg-final { scan-assembler-times "lw\t\\\$\[ast0-9\]+,%got_disp\\(f_extern\\)\\(\\\$gp\\)" 3 } } */
/* { dg-final { scan-assembler "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(f_takes_fptr\\)" } } */
/* { dg-final { scan-assembler "ori\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(f_takes_fptr\\)" } } */
/* { dg-final { scan-assembler "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(f_long_takes_fptr\\)" } } */
/* { dg-final { scan-assembler "ori\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(f_long_takes_fptr\\)" } } */

/* { dg-final { scan-assembler-times "jalrc\t\\\$\[ast0-9\]+" 3 } } */
/* { dg-final { scan-assembler "lapc\t\\\$gp,_gp" } } */

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
