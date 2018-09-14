/* { dg-options "-std=gnu99 -Wshadow" } */

typedef unsigned char vta __attribute__((__vector_size__(2)));
typedef __sizeless_1 ta;
typedef __sizeless_2 tb;

/* Sizeless objects with global scope.  */

ta global_ta; /* { dg-error {sizeless variable 'global_ta' cannot have static storage duration} } */
static ta local_ta; /* { dg-error {sizeless variable 'local_ta' cannot have static storage duration} } */
extern ta ta_extern; /* { dg-error {sizeless variable 'ta_extern' cannot have static storage duration} } */
__thread ta tls_ta; /* { dg-error {sizeless variable 'tls_ta' cannot have static storage duration} } */
_Atomic ta atomic_ta; /* { dg-error {sizeless variable 'atomic_ta' cannot have static storage duration} } */

/* Sizeless arrays.  */

typedef ta array_type[2]; /* { dg-error {array type has incomplete element type 'ta'} } */
extern ta extern_array[]; /* { dg-error {array type has incomplete element type 'ta'} } */

/* Pointers to sizeless types.  */

ta *global_ta_ptr;

/* Sizeless arguments and return values.  */

void ext_consume_ta (ta);
void ext_consume_varargs (int, ...);
ta ext_produce_ta ();

/* Main tests for statements and expressions.  */

void
statements (int n)
{
  /* Local declarations.  */

  unsigned char va __attribute__((__vector_size__(2)));
  ta ta1, ta2;
  vta vta1;
  tb tb1;
  static ta local_static_ta; /* { dg-error {sizeless variable 'local_static_ta' cannot have static storage duration} } */

  /* Layout queries.  */

  sizeof (ta); /* { dg-error {invalid application of 'sizeof' to incomplete type} } */
  sizeof (ta1); /* { dg-error {invalid application of 'sizeof' to incomplete type} } */
  sizeof (ext_produce_ta ()); /* { dg-error {invalid application of 'sizeof' to incomplete type} } */
  _Alignof (ta); /* { dg-error {invalid application of '(_Alignof|__alignof__)' to incomplete type} } */
  _Alignof (ta1); /* { dg-error {invalid application of '(_Alignof|__alignof__)' to incomplete type} } */
  _Alignof (ext_produce_ta ()); /* { dg-error {invalid application of '(_Alignof|__alignof__)' to incomplete type} } */

  /* Initialization.  */

  ta init_ta1 = ta1;
  ta init_ta2 = tb1; /* { dg-error {incompatible types when initializing type 'ta' using type 'tb'} } */
  ta init_ta3 = {};

  int initi_a = ta1; /* { dg-error {incompatible types when initializing type 'int' using type 'ta'} } */
  int initi_b = { ta1 }; /* { dg-error {incompatible types when initializing type 'int' using type 'ta'} } */

  /* Compound literals.  */

  (int) { ta1 }; /* { dg-error {incompatible types when initializing type 'int' using type 'ta'} } */

  /* Arrays.  */

  ta array[2]; /* { dg-error {array type has incomplete element type 'ta'} } */
  ta zero_length_array[0]; /* { dg-error {array type has incomplete element type 'ta'} } */
  ta empty_init_array[] = {}; /* { dg-error {array type has incomplete element type 'ta'} } */
			      /* { dg-error {empty scalar initializer} "" { target *-*-* } .-1 } */
  typedef ta vla_type[n]; /* { dg-error {array type has incomplete element type 'ta'} } */

  /* Assignment.  */

  n = ta1; /* { dg-error {incompatible types when assigning to type 'int' from type 'ta'} } */

  ta1 = 0; /* { dg-error {incompatible types when assigning to type 'ta'[^\n]* from type 'int'} } */
  ta1 = ta2;
  ta1 = tb1; /* { dg-error {incompatible types when assigning to type 'ta'[^\n]* from type 'tb'} } */

  /* Casting.  */

  (void) ta1;

  /* Addressing and dereferencing.  */

  ta *ta_ptr = &ta1;
  vta *vta_ptr = &vta1;
  ta1 = *ta_ptr;

  /* Pointer assignment.  */

  vta_ptr = ta_ptr; /* { dg-warning {assignment to '[^']*' from incompatible pointer type} } */
  ta_ptr = vta_ptr; /* { dg-warning {assignment to '[^']*' from incompatible pointer type} } */

  /* Pointer arithmetic.  */

  ++ta_ptr; /* { dg-error {increment of pointer to an incomplete type 'ta'} } */
  --ta_ptr; /* { dg-error {decrement of pointer to an incomplete type 'ta'} } */
  ta_ptr++; /* { dg-error {increment of pointer to an incomplete type 'ta'} } */
  ta_ptr--; /* { dg-error {decrement of pointer to an incomplete type 'ta'} } */
  ta_ptr += 0; /* { dg-error {invalid use of sizeless typedef 'ta'} } */
  ta_ptr += 1; /* { dg-error {invalid use of sizeless typedef 'ta'} } */
  ta_ptr -= 0; /* { dg-error {invalid use of sizeless typedef 'ta'} } */
  ta_ptr -= 1; /* { dg-error {invalid use of sizeless typedef 'ta'} } */
  ta_ptr - ta_ptr; /* { dg-error {arithmetic on pointer to an incomplete type} } */
  vta_ptr - ta_ptr; /* { dg-error {invalid operands to binary -} } */
  ta_ptr - vta_ptr; /* { dg-error {invalid operands to binary -} } */
  ta1 = ta_ptr[0]; /* { dg-error {invalid use of sizeless typedef 'ta'} } */
  ta1 = ta_ptr[1]; /* { dg-error {invalid use of sizeless typedef 'ta'} } */

  /* Pointer comparison.  */

  ta_ptr == &ta1;
  ta_ptr != &ta1;
  ta_ptr < &ta1;
  ta_ptr <= &ta1;
  ta_ptr > &ta1;
  ta_ptr >= &ta1;
  vta_ptr == ta_ptr; /* { dg-warning {comparison of distinct pointer types lacks a cast} } */
  vta_ptr != ta_ptr; /* { dg-warning {comparison of distinct pointer types lacks a cast} } */
  vta_ptr < ta_ptr; /* { dg-warning {comparison of distinct pointer types lacks a cast} } */
  vta_ptr <= ta_ptr; /* { dg-warning {comparison of distinct pointer types lacks a cast} } */
  vta_ptr > ta_ptr; /* { dg-warning {comparison of distinct pointer types lacks a cast} } */
  vta_ptr >= ta_ptr; /* { dg-warning {comparison of distinct pointer types lacks a cast} } */
  ta_ptr == vta_ptr; /* { dg-warning {comparison of distinct pointer types lacks a cast} } */
  ta_ptr != vta_ptr; /* { dg-warning {comparison of distinct pointer types lacks a cast} } */
  ta_ptr < vta_ptr; /* { dg-warning {comparison of distinct pointer types lacks a cast} } */
  ta_ptr <= vta_ptr; /* { dg-warning {comparison of distinct pointer types lacks a cast} } */
  ta_ptr > vta_ptr; /* { dg-warning {comparison of distinct pointer types lacks a cast} } */
  ta_ptr >= vta_ptr; /* { dg-warning {comparison of distinct pointer types lacks a cast} } */

  /* Unary operators.  */

  !ta1; /* { dg-error {wrong type argument to unary exclamation mark} } */
  *ta1; /* { dg-error {invalid type argument of unary '\*'} } */
  __real ta1; /* { dg-error {wrong type argument to __real} } */
  __imag ta1; /* { dg-error {wrong type argument to __imag} } */

  /* Conditional expressions.  */

  0 ? ta1 : ta1;
  0 ? ta1 : tb1; /* { dg-error {type mismatch in conditional expression} } */
  0 ? ta1 : 0; /* { dg-error {type mismatch in conditional expression} } */
  0 ? 0 : ta1; /* { dg-error {type mismatch in conditional expression} } */
  0 ?: ta1; /* { dg-error {type mismatch in conditional expression} } */
  0 ? ta_ptr : ta_ptr;
  0 ? ta_ptr : vta_ptr; /* { dg-warning {pointer type mismatch in conditional expression} } */
  0 ? vta_ptr : ta_ptr; /* { dg-warning {pointer type mismatch in conditional expression} } */

  /* Generic associations.  */

  _Generic (ta1, default: 100);
  _Generic (1, ta: 10, default: 20);

  /* Function arguments.  */

  ext_consume_ta (ta1);
  ext_consume_ta (tb1); /* { dg-error {incompatible type for argument 1 of 'ext_consume_ta'} } */
  ext_consume_varargs (ta1); /* { dg-error {incompatible type for argument 1 of 'ext_consume_varargs'} } */
  ext_consume_varargs (1, ta1);

  /* Function returns.  */

  ext_produce_ta ();
  ta1 = ext_produce_ta ();
  tb1 = ext_produce_ta (); /* { dg-error {incompatible types when assigning to type 'tb'[^\n]* from type 'ta'} } */

  /* Varargs processing.  */

  __builtin_va_list valist;
  __builtin_va_arg (valist, ta);

  /* Statement expressions.  */

  ({ ta1; });
  ({ ta another_ta = *ta_ptr; another_ta; });
}

/* Function parameters in definitions.  */

void
old_style (input_ta)
     ta input_ta;
{
  ta ta1 = input_ta;
}

void
new_style_param (ta input_ta)
{
  ta ta1 = input_ta;
}

/* Function return values in definitions.  */

ta
good_return_ta (ta param)
{
  return param;
}

ta
bad_return_ta (tb param)
{
  return param; /* { dg-error {incompatible types when returning type 'tb'[^\n]* but 'ta'[^\n]* was expected} } */
}
