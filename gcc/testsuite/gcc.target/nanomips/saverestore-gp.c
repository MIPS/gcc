/* { dg-do compile } */
/* { dg-options "-march=32r6 -fpic -mcmodel=auto" } */
/* { dg-additional-options "-mrelax-saverestore-gp" } */
/* { dg-skip-if "excessive frame in fun1" { *-*-* } { "-O0" } { "" } } */

extern int val;

int fun1 (void) { return val; }

int fun2 (void) { return fun1(); }

extern __attribute__((visibility("hidden"))) int foo (void);

int fun3 (void) { return foo(); }

/* { dg-final {scan-assembler-not "\t\.reloc\t1f,R_NANOMIPS_SAVERESTORE,fun1\n" } } */
/* { dg-final {scan-assembler-times "\t\.reloc\t1f,R_NANOMIPS_SAVERESTORE,fun2\n" 2 } } */
/* { dg-final {scan-assembler-not "\t\.reloc\t1f,R_NANOMIPS_SAVERESTORE,fun3\n" } } */