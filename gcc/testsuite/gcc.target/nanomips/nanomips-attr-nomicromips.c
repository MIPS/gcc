/* Verify that we accept "nomicromips" flag for nanoMIPS target.  */
/* { dg-do compile } */
void __attribute__((nomicromips)) foo (void) {}
