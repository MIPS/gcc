/* Test warning from conflicting visibility specifications. */
/* APPLE LOCAL begin mainline 4.0 2005-03-25 */
/* { dg-require-visibility "protected" } */
/* APPLE LOCAL end mainline 4.0 2005-03-25 */
/* { dg-final { scan-hidden "xyzzy" } } */

extern int 
__attribute__((visibility ("hidden")))
xyzzy; /* { dg-warning "previous declaration here" "" } */

int 
__attribute__((visibility ("protected")))
xyzzy = 5; /* { dg-warning "visibility attribute ignored" "" } */
