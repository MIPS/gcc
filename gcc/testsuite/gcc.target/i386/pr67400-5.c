/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fno-pic -freloc-rw" } */

extern void foo (void);

typedef void (*func_p) (void);

const func_p p1 = &foo;

/* { dg-final { scan-assembler ".data.rel.ro" } } */
/* { dg-final { scan-assembler-not ".rodata" } } */
