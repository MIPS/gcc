/* { dg-do compile } */
/* { dg-options "-m32 -march=32r6 -mcmodel=medium -fPIC -mpcrel" } */
/* { dg-skip-if "" { *-*-* } { "-O1" "-O2" "-O3" "-Os" } { "" } } */

/* lwpc reg, %got_pcrel32(f_extern) # 1 */
/* jalrc reg # 1 */

/* lwpc reg, %got_pcrel32(f_extern) # 2 */
/* balc f_takes_fptr */

/* lwpc reg, %got_pcrel32(f_extern) # 3 */
/* lapc.b reg, f_long_takes_fptr */
/* jalrc reg # 2 */

/* { dg-final { scan-assembler-times "lwpc\t\\\$\[ast0-9\]+,%got_pcrel32\\(f_extern\\)" 3 } } */
/* { dg-final { scan-assembler "balc\tf_takes_fptr" } } */
/* { dg-final { scan-assembler "lapc.b\t\\\$\[ast0-9\]+,f_long_takes_fptr" } } */

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
