/* { dg-do compile } */
/* { dg-require-alias "" } */
/* { dg-options "-gdwarf-2 -dA" } */

static int f1 (void) { return 0; }
extern int g1 (void) __attribute__((__alias__("f1")));

int f () { return g1 (); }

// { dg-final { scan-assembler-times " DW_AT_import" 1 } }
