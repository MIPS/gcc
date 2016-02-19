/* { dg-do compile } */
/* { dg-require-weak "" } */
/* { dg-options "-gdwarf-2 -dA" } */

int i1 = 0;
static int i2 __attribute__((__weakref__("i1")));

int f () { return i2; }

// { dg-final { scan-assembler-times " DW_AT_import" 1 } }
