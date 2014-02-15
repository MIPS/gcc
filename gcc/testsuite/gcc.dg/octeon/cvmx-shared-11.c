/* { dg-do compile } */
extern void *ptr;  /* { dg-message "" } */
__attribute__((cvmx_shared)) void *ptr; /* { dg-warning "CVMX_SHARED" } */
