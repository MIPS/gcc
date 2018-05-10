/* { dg-do compile { target *-*-elf* *-*-gnu* } } */
/* { dg-options "-std=c99" } */
/* On nanoMIPS, PID mode will put static constants in .sdata instead of
   .rodata, which will make this test fail. We can work around this by
   disabling PID mode with the -mno-pid option.  */
/* { dg-additional-options "-mno-pid" { target nanomips*-*-* } } */
/* { dg-final { scan-assembler "rdata|rodata" } } */

struct S { const int *x; } s = { (const int[]){1, 2, 3} };
