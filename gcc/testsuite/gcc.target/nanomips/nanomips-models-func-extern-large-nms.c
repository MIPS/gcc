/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6s -mcmodel=medium -fPIC -mpcrel" } */
/* { dg-skip-if "" { *-*-* } { "-O1" "-O2" "-O3" "-Os" } { "" } } */

/* aluipc reg, %got_pcrel_hi(f_extern) # 1 */
/* lw reg, %got_lo(f_extern)(reg) # 1 */
/* jalrc reg # 1 */

/* aluipc reg, %got_pcrel_hi(f_extern) # 2 */
/* lw reg, %got_lo(f_extern)(reg) # 2 */
/* balc f_takes_fptr */

/* aluipc reg, %got_pcrel_hi(f_extern) # 3 */
/* lw reg, %got_lo(f_extern)(reg) # 3 */
/* aluipc reg, %pcrel_hi(f_long_takes_fptr) */
/* ori reg, reg, %lo(f_long_takes_fptr) */
/* jalrc reg # 2 */

/* { dg-final { scan-assembler-times "aluipc\t\\\$\[ast0-9\]+,%got_pcrel_hi\\(f_extern\\)" 3 } } */
/* { dg-final { scan-assembler-times "lw\t\\\$\[ast0-9\]+,%got_lo\\(f_extern\\)\\(\\\$\[ast0-9\]+\\)" 3 } } */

/* { dg-final { scan-assembler "balc\tf_takes_fptr" } } */

/* { dg-final { scan-assembler "aluipc\t\\\$\[ast0-9\]+,%pcrel_hi\\(f_long_takes_fptr\\)" } } */
/* { dg-final { scan-assembler "ori\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,%lo\\(f_long_takes_fptr\\)" } } */

/* { dg-final { scan-assembler-times "jalrc\t\\\$\[ast0-9\]+" 2 } } */

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
