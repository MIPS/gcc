/* { dg-options "-std=gnu99 -Wshadow" } */

typedef __sizeless_1 ta;

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

  /* Casting.  */

  (void) ta1;

  /* Addressing and dereferencing.  */

  ta *ta_ptr = &ta1;

  /* Generic associations.  */

  _Generic (ta1, default: 100);
}
