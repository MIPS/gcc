/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fno-pic -fno-plt" } */

extern void foo (void);

typedef void (*func_p) (void);

const func_p p1 = &foo;

/* { dg-final { scan-assembler ".data.rel.ro" } } */
/* { dg-final { scan-assembler-not ".rodata" } } */
