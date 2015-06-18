/* { dg-do run } */

#include <stdlib.h>

int
main (int argc, char **argv)
{
  float a, b;

  a = 2.0;
  b = 0.0;

#pragma acc parallel default (shared) /* { dg-error "expected 'none'" } */
  /* { dg-error "expected '\\\)'" "" { target c++ } 13 } */
  {
    b = a;
    a = 1.0;
    a = a + b;
  }

#pragma acc parallel default (none) /* { dg-error "enclosing OpenACC parallel" } */
  {
    b = a; /* { dg-error "in enclosing OpenACC parallel construct" } */
    a = 1.0;
    a = a + b;
  }

#pragma acc parallel create (b) default (none) /* { dg-error "enclosing OpenACC parallel" } */
  {
    b = a; /* { dg-error "in enclosing OpenACC parallel construct" } */
    a = 1.0;
    a = a + b;
  }

#pragma acc kernels default (none) /* { dg-error "enclosing OpenACC kernels" } */
  {
    b = a; /* { dg-error "in enclosing OpenACC kernels construct" } */
    a = 1.0;
    a = a + b;
  }

#pragma acc kernels create (b) default (none) /* { dg-error "enclosing OpenACC kernels construct" } */
  {
    b = a; /* { dg-error "in enclosing OpenACC kernels construct" } */
    a = 1.0;
    a = a + b;
  }

#pragma acc kernels copy (a) create (b) default () /* { dg-error "expected 'none'" } */
  {
    b = a;
    a = 1.0;
    a = a + b;
  }

  return 0;
}
