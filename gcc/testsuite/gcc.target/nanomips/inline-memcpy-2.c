/* { dg-options "-fno-common -march=32r6 (REQUIRES_STDLIB)" } */
/* { dg-additional-options "-mload-store-unaligned" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-Os"} { "" } } */
/* { dg-final { scan-assembler-not "\tmemcpy" } } */
/* { dg-final { scan-assembler-times "\tuasw " 8 } } */

/* Test that inline memcpy is expanded for target hardware without
   swl, swr when alignment is halfword and sufficent shs are produced.  */

#include <string.h>

char c[40] __attribute__ ((aligned(2)));

void
f1 ()
{
  memcpy (c, "1234567890QWERTYUIOPASDFGHJKLZXCVBNM", 32);
}
