/* { dg-do compile { target *-octeon-elf* } } */
/* { dg-octeon-make-options "OCTEON_DISABLE_BACKTRACE=1" } */

int main (void) { return 0; }

/* { dg-final { scan-assembler-not "\.eh_frame" } } */
