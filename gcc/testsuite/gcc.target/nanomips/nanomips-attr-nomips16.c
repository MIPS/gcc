/* Verify that we accept "nomips16" flag for nanoMIPS target.  */
/* { dg-do compile } */
void __attribute__((nomips16)) foo (void) {}
