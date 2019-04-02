/* { dg-options "-std=gnu99 -Wshadow" } */

typedef __sizeless_1 ta;

/* Sizeless objects with global scope.  */

ta global_ta; /* { dg-error {sizeless variable 'global_ta' cannot have static storage duration} } */
static ta local_ta; /* { dg-error {sizeless variable 'local_ta' cannot have static storage duration} } */
extern ta ta_extern; /* { dg-error {sizeless variable 'ta_extern' cannot have static storage duration} } */
__thread ta tls_ta; /* { dg-error {sizeless variable 'tls_ta' cannot have static storage duration} } */
_Atomic ta atomic_ta; /* { dg-error {sizeless variable 'atomic_ta' cannot have static storage duration} } */

/* Pointers to sizeless types.  */

ta *global_ta_ptr;

/* Sizeless arguments and return values.  */

ta ext_produce_ta ();

/* Main tests for statements and expressions.  */

void
statements (int n)
{
  /* Local declarations.  */

  ta ta1;
  static ta local_static_ta; /* { dg-error {sizeless variable 'local_static_ta' cannot have static storage duration} } */

  /* Layout queries.  */

  sizeof (ta); /* { dg-error {invalid application of 'sizeof' to incomplete type} } */
  sizeof (ta1); /* { dg-error {invalid application of 'sizeof' to incomplete type} } */
  _Alignof (ta); /* { dg-error {invalid application of '(_Alignof|__alignof__)' to incomplete type} } */
  _Alignof (ta1); /* { dg-error {invalid application of '(_Alignof|__alignof__)' to incomplete type} } */

  /* Casting.  */

  (void) ta1;

  /* Addressing and dereferencing.  */

  ta *ta_ptr = &ta1;

  /* Generic associations.  */

  _Generic (ta1, default: 100);

  /* Statement expressions.  */

  ({ ta1; });
}
