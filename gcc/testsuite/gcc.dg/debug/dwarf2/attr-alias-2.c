/* { dg-do compile } */
/* { dg-require-alias "" } */
/* { dg-options "-gdwarf-2 -dA" } */

static int i1 = 0;
extern int i2 __attribute__((__alias__("i1")));

int f () { return i2; }

// { dg-final { scan-assembler-times " DW_AT_import" 1 } }
