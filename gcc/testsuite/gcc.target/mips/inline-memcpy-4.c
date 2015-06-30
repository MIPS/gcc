/* { dg-options "-fno-common -mips64 -mabi=64 -ffat-lto-objects" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-Os"} { "" } } */
/* { dg-final { scan-assembler-not "\tmemcpy" } } */
/* { dg-final { scan-assembler-times "sdl" 4 } } */
/* { dg-final { scan-assembler-times "sdr" 4 } } */

/* Test that inline memcpy for hardware with sdl, sdr handles subword
   alignment and produces enough sdl/sdrs on n64.  */

#include <string.h>

char c[40] __attribute__ ((aligned(2)));

void
f1 ()
{
  memcpy (c, "1234567890QWERTYUIOPASDFGHJKLZXCVBNM", 32);
}
