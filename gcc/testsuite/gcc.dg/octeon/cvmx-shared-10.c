/* { dg-do compile } */
__attribute__((cvmx_shared)) void *ptr; /* { dg-message "" } */
extern void *ptr; /* { dg-warning "CVMX_SHARED" } */
