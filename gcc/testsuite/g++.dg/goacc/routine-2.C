/* Test invalid use of the routine directive.  */

template <typename T>
extern T one_d();
#pragma acc routine (one_d) nohost seq /* { dg-error "names a set of overloads" } */

template <typename T>
T
one()
{
  return 1;
}
#pragma acc routine (one) bind(one_d) seq /* { dg-error "names a set of overloads" } */

int incr (int);
float incr (float);

#pragma acc routine (incr) seq /* { dg-error "names a set of overloads" } */


int sum (int, int);

namespace foo {
#pragma acc routine (sum) seq
  int sub (int, int);
}

#pragma acc routine (foo::sub) seq

/* It's strange to apply a routine directive to subset of overloaded
   functions, but that is permissible in OpenACC 2.x.  */

int decr (int a);

#pragma acc routine seq
float decr (float a);

/* Bind clause.  */

#pragma acc routine seq
float
mult (float a, float b)
{
  return a * b;
}

#pragma acc routine bind(mult) seq /* { dg-error "bind identifier 'mult' is not compatible with function 'broken_mult1'" } */
float
broken_mult1 (int a, int b)
{
  return a + b;
}

/* This should result in a link error, but it's valid for a compile test.  */
#pragma acc routine bind("mult") seq
float
broken_mult2 (float a, float b)
{
  return a + b;
}
#pragma acc routine (broken_mult2) bind("mult") seq
#pragma acc routine bind("mult") seq
extern float broken_mult2 (float, float);

#pragma acc routine bind(sum2) seq /* { dg-error "'sum2' has not been declared" } */
int broken_mult3 (int a, int b);

#pragma acc routine bind(foo::sub) seq
int broken_mult4 (int a, int b);
#pragma acc routine (broken_mult4) bind(foo::sub) seq
#pragma acc routine bind(foo::sub) seq
extern int broken_mult4 (int a, int b);

namespace bar
{
#pragma acc routine bind (mult) seq
  float working_mult (float a, float b)
  {
    return a * b;
  }
#pragma acc routine (working_mult) bind (mult) seq
#pragma acc routine bind (mult) seq
  float working_mult (float, float);
}

#pragma acc routine seq
int div (int, int);

#pragma acc routine seq
float div (float, float);

#pragma acc routine bind (div) seq /* { dg-error "'div' names a set of overloads" } */
float
my_div (float a, float b)
{
  return a / b;
}

#pragma acc routine bind (other_div) seq /* { dg-error "'other_div' has not been declared" } */
float
my_div2 (float a, float b)
{
  return a / b;
}

int div_var;

#pragma acc routine bind (div_var) seq /* { dg-error "'div_var' does not refer to a function" } */
float my_div3 (float, float);

#pragma acc routine bind (div_var) seq /* { dg-error "'div_var' does not refer to a function" } */
float my_div4 (float, float);

#pragma acc routine bind (%) seq /* { dg-error "expected identifier or character string literal" } */
float my_div5 (float, float);

#pragma acc routine bind ("") seq /* { dg-error "bind argument must not be an empty string" } */
float my_div6 (float, float);

struct astruct
{
  #pragma acc routine seq /* { dg-error ".#pragma acc routine. must be at file scope" } */
  int sum (int a, int b)
  {
    return a + b;
  }
};

class aclass
{
  #pragma acc routine seq /* { dg-error ".#pragma acc routine. must be at file scope" } */
  int sum (int a, int b)
  {
    return a + b;
  }
};
