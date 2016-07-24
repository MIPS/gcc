/* { dg-do compile } */
/* { dg-require-weak "" } */
/* { dg-options "-gdwarf-2 -dA" } */

int f1 (void) { return 0; }
static int g1 (void) __attribute__((__weakref__("f1")));

int f () { return g1 (); }

// { dg-final { scan-assembler-times " DW_AT_import" 1 } }
