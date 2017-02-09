/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -mcmodel=auto -fpic -mpcrel" } */
/* { dg-skip-if "" { *-*-* } { "-O1" "-O2" "-O3" "-Os" } { "" } } */

/* lw reg, %got_call(a_extern)($gp) */
/* .reloc 1f, R_NANOMIPS_JALR16, a_extern */
/* jalrc reg # 1 */

/* lw reg, %got_disp(a_extern)($gp) # 1 */
/* balc a_takes_fptr */

/* lw reg, %got_disp(a_extern)($gp) # 2 */
/* lapc[48] reg, a_long_takes_fptr */
/* jalrc reg # 2 */

/* { dg-final { scan-assembler "lw\t\\\$\[ast0-9\]+,%got_call\\(a_extern\\)\\(\\\$gp\\)" } } */
/* { dg-final { scan-assembler "\\\.reloc\t1f,R_NANOMIPS_JALR16,a_extern" } } */
/* { dg-final { scan-assembler-times "lw\t\\\$\[ast0-9\]+,%got_disp\\(a_extern\\)\\(\\\$gp\\)" 2 } } */
/* { dg-final { scan-assembler "balc\ta_takes_fptr" } } */
/* { dg-final { scan-assembler "lapc\\\[48\\]\t\\\$\[ast0-9\]+,a_long_takes_fptr" } } */

/* { dg-final { scan-assembler-times "jalrc\t\\\$\[ast0-9\]+" 2 } } */
/* { dg-final { scan-assembler "lapc\t\\\$gp,_gp" } } */

__attribute__ ((cmodel("auto"), noclone, noinline))
extern int a_extern(void) { asm (""); return 1; }

__attribute__ ((cmodel("auto"), noclone, noinline))
static int a_takes_fptr(int bar()) { asm (""); return 1; }

__attribute__ ((cmodel("auto"), noclone, noinline, long_call))
static int a_long_takes_fptr(int bar()) { asm (""); return 1; }

int
foo ()
{
  a_extern();
  a_takes_fptr(&a_extern);
  a_long_takes_fptr(&a_extern);

  return 0;
}
