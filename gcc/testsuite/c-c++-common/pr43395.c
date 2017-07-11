/* PR c/43395 */
/* { dg-do compile } */
/* { dg-require-effective-target label_values } */

void *
foo (void)
{
lab:
  return &&lab;
/* { dg-warning "function returns address of label" "" { target c } 9 } */
/* { dg-warning "address of label" "" { target c++ } 8 } */
}

void *
bar (void)
{
  __label__ lab;
lab:
  return &&lab;
/* { dg-warning "function returns address of label" "" { target c } 19 } */
/* { dg-warning "address of label" "" { target c++ } 18 } */
}

void *
baz (void)
{
  int i;
  return &i;
/* { dg-warning "function returns address of local variable" "" { target c } 28 } */
/* { dg-warning "address of local variable" "" { target c++ } 27 } */
}
