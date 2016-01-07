/* Test invalid use of the routine directive.  */

template <typename T>
extern T one_d();
#pragma acc routine (one_d) nohost /* { dg-error "names a set of overloads" } */

template <typename T>
T
one()
{
  return 1;
}
#pragma acc routine (one) bind(one_d) /* { dg-error "names a set of overloads" } */

int incr (int);
float incr (float);

#pragma acc routine (incr) /* { dg-error "names a set of overloads" } */


int sum (int, int);

namespace foo {
#pragma acc routine (sum)
  int sub (int, int);
}

#pragma acc routine (foo::sub)

/* It's strange to apply a routine directive to subset of overloaded
   functions, but that is permissible in OpenACC 2.x.  */

int decr (int a);

#pragma acc routine
float decr (float a);

/* Bind clause.  */

#pragma acc routine
float
mult (float a, float b)
{
  return a * b;
}

#pragma acc routine bind(mult) /* { dg-error "bind identifier 'mult' is not compatible with function 'broken_mult1'" } */
float
broken_mult1 (int a, int b)
{
  return a + b;
}

/* This should result in a link error, but it's valid for a compile test.  */
#pragma acc routine bind("mult")
float
broken_mult2 (float a, float b)
{
  return a + b;
}

#pragma acc routine bind(sum2) /* { dg-error "'sum2' has not been declared" } */
int broken_mult3 (int a, int b);

#pragma acc routine bind(foo::sub)
int broken_mult4 (int a, int b);

namespace bar
{
#pragma acc routine bind (mult)
  float working_mult (float a, float b)
  {
    return a * b;
  }
}

#pragma acc routine
int div (int, int);

#pragma acc routine
float div (float, float);

#pragma acc routine bind (div) /* { dg-error "'div' names a set of overloads" } */
float
my_div (float a, float b)
{
  return a / b;
}

#pragma acc routine bind (other_div) /* { dg-error "'other_div' has not been declared" } */
float
my_div2 (float a, float b)
{
  return a / b;
}

int div_var;

#pragma acc routine bind (div_var) /* { dg-error "'div_var' does not refer to a function" } */
float my_div3 (float, float);

#pragma acc routine bind (div_var) /* { dg-error "'div_var' does not refer to a function" } */
float my_div4 (float, float);

#pragma acc routine bind (%) /* { dg-error "expected identifier or character string literal" } */
float my_div5 (float, float);

#pragma acc routine bind ("") /* { dg-error "bind argument must not be an empty string" } */
float my_div6 (float, float);

struct astruct
{
  #pragma acc routine /* { dg-error ".#pragma acc routine. must be at file scope" } */
  int sum (int a, int b)
  {
    return a + b;
  }
};

class aclass
{
  #pragma acc routine /* { dg-error ".#pragma acc routine. must be at file scope" } */
  int sum (int a, int b)
  {
    return a + b;
  }
};
