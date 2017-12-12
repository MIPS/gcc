/* Verify that we get an error for unsupported -mabi=32 option.  */
/* { dg-additional-options "-mabi=32" } */
/* { dg-error "not supported by this configuration" "" { target *-*-* } 0 } */
void foo (void) {}
