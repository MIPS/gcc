/* Test for _Atomic in C11.  Test of invalid code.  */
/* { dg-do compile } */
/* { dg-options "-std=c11 -pedantic-errors" } */

/* Increment and decrement are invalid for atomic complex types and
   atomic pointers to incomplete types, just as for the corresponding
   non-atomic types.  Likewise for types on which arithmetic is
   invalid.  */
_Atomic _Complex float acf;
void *_Atomic apv;
struct s *_Atomic aps;
_Atomic struct t { char c; } as;

void
func (void)
{
  acf++; /* { dg-error "complex types" } */
  acf--; /* { dg-error "complex types" } */
  ++acf; /* { dg-error "complex types" } */
  --acf; /* { dg-error "complex types" } */
  apv++; /* { dg-error "wrong type|pointer of type" } */
  apv--; /* { dg-error "wrong type|pointer of type" } */
  ++apv; /* { dg-error "wrong type|pointer of type" } */
  --apv; /* { dg-error "wrong type|pointer of type" } */
  aps++; /* { dg-error "pointer to|invalid use of undefined type" } */
  aps--; /* { dg-error "pointer to|invalid use of undefined type" } */
  ++aps; /* { dg-error "pointer to|invalid use of undefined type" } */
  --aps; /* { dg-error "pointer to|invalid use of undefined type" } */
  as++; /* { dg-error "wrong type" } */
  as--; /* { dg-error "wrong type" } */
  ++as; /* { dg-error "wrong type" } */
  --as; /* { dg-error "wrong type" } */
}

/* Pointer subtraction and comparisons differing in _Atomic are
   invalid where such subtraction and comparisons differing in
   qualifiers are valid.  There is no special allowance for equality
   comparisons of pointers to atomic void to pointers to object
   types.  Likewise for conditional expressions.  */
int *pi;
_Atomic int *pai;
_Atomic void *pav;
int r;

void
func2 (void)
{
  r = pai - pi; /* { dg-error "invalid operands" } */
  r = pi - pai; /* { dg-error "invalid operands" } */
  r = pi < pai; /* { dg-error "distinct pointer types" } */
  r = pi > pai; /* { dg-error "distinct pointer types" } */
  r = pi <= pai; /* { dg-error "distinct pointer types" } */
  r = pi >= pai; /* { dg-error "distinct pointer types" } */
  r = pai < pi; /* { dg-error "distinct pointer types" } */
  r = pai > pi; /* { dg-error "distinct pointer types" } */
  r = pai <= pi; /* { dg-error "distinct pointer types" } */
  r = pai >= pi; /* { dg-error "distinct pointer types" } */
  r = pav == pi; /* { dg-error "distinct pointer types" } */
  r = pav != pi; /* { dg-error "distinct pointer types" } */
  r = pi == pav; /* { dg-error "distinct pointer types" } */
  r = pi != pav; /* { dg-error "distinct pointer types" } */
  (void) (r ? pai : pi); /* { dg-error "pointer type mismatch" } */
  (void) (r ? pi : pai); /* { dg-error "pointer type mismatch" } */
  (void) (r ? pai : pav); /* { dg-error "pointer type mismatch" } */
  (void) (r ? pav : pai); /* { dg-error "pointer type mismatch" } */
}
