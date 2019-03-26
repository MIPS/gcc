// { dg-options "" }

typedef __SIZE_TYPE__ size_t;
inline void *operator new (size_t, void *__p) throw() { return __p; }

typedef unsigned char vta __attribute__((__vector_size__(2)));
typedef __sizeless_1 ta;
typedef __sizeless_2 tb;

// Sizeless objects with global scope.

ta global_ta; // { dg-error {sizeless variable 'ta global_ta' cannot have static storage duration} }
static ta local_ta; // { dg-error {sizeless variable 'ta local_ta' cannot have static storage duration} }
__thread ta tls_ta; // { dg-error {sizeless variable 'ta tls_ta' cannot have static storage duration} }

// Sizeless member variables.

struct struct1 {
  ta a; // { dg-error {field 'a' has incomplete type} }
};

union union1 {
  ta a; // { dg-error {field 'a' has incomplete type} }
};

// Sizeless member variables in templated structures.

template<typename T>
struct templated_struct1 {
  ta a; // { dg-error {field 'a' has incomplete type} }
};

// Pointers to sizeless types.

ta *global_ta_ptr;

// Sizeless arguments and return values.

void ext_consume_const_int_ref (const int &);
void ext_consume_varargs (int, ...);
ta ext_produce_ta ();

// Main tests for statements and expressions.

void
statements (int n)
{
  // Local declarations.

  ta ta1, ta2;
  vta vta1;

  // Layout queries.

  sizeof (ta); // { dg-error {invalid application of 'sizeof' to incomplete type} }
  sizeof (ta1); // { dg-error {invalid application of 'sizeof' to incomplete type} }
  __alignof (ta); // { dg-error {invalid application of '__alignof__' to incomplete type} }

  // Addressing and dereferencing.

  ta *ta_ptr = &ta1;
  vta *vta_ptr = &vta1;

  // Pointer assignment.

  vta_ptr = ta_ptr; // { dg-error {invalid conversion from 'ta\*'[^\n]* to 'vta\*'} }
  ta_ptr = vta_ptr; // { dg-error {invalid conversion from 'vta\*'[^\n]* to 'ta\*'} }

  // Pointer arithmetic.

  ta_ptr += 0; // { dg-error {invalid use of sizeless type 'ta'} }
	       // { dg-error {in evaluation of} "" { target *-*-* } .-1 }
  ta_ptr += 1; // { dg-error {invalid use of sizeless type 'ta'} }
	       // { dg-error {in evaluation of} "" { target *-*-* } .-1 }
  ta_ptr -= 0; // { dg-error {invalid use of sizeless type 'ta'} }
	       // { dg-error {in evaluation of} "" { target *-*-* } .-1 }
  ta_ptr -= 1; // { dg-error {invalid use of sizeless type 'ta'} }
	       // { dg-error {in evaluation of} "" { target *-*-* } .-1 }
  ta1 = ta_ptr[0]; // { dg-error {invalid use of sizeless type 'ta'} }
  ta1 = ta_ptr[1]; // { dg-error {invalid use of sizeless type 'ta'} }

  // Pointer comparison.

  ta_ptr == &ta1;
  ta_ptr != &ta1;
  ta_ptr < &ta1;
  ta_ptr <= &ta1;
  ta_ptr > &ta1;
  ta_ptr >= &ta1;
  vta_ptr == ta_ptr; // { dg-error {comparison between distinct pointer types [^\n]*lacks a cast} }
  vta_ptr != ta_ptr; // { dg-error {comparison between distinct pointer types [^\n]*lacks a cast} }
  vta_ptr < ta_ptr; // { dg-error {comparison between distinct pointer types [^\n]*lacks a cast} }
  vta_ptr <= ta_ptr; // { dg-error {comparison between distinct pointer types [^\n]*lacks a cast} }
  vta_ptr > ta_ptr; // { dg-error {comparison between distinct pointer types [^\n]*lacks a cast} }
  vta_ptr >= ta_ptr; // { dg-error {comparison between distinct pointer types [^\n]*lacks a cast} }
  ta_ptr == vta_ptr; // { dg-error {comparison between distinct pointer types [^\n]*lacks a cast} }
  ta_ptr != vta_ptr; // { dg-error {comparison between distinct pointer types [^\n]*lacks a cast} }
  ta_ptr < vta_ptr; // { dg-error {comparison between distinct pointer types [^\n]*lacks a cast} }
  ta_ptr <= vta_ptr; // { dg-error {comparison between distinct pointer types [^\n]*lacks a cast} }
  ta_ptr > vta_ptr; // { dg-error {comparison between distinct pointer types [^\n]*lacks a cast} }
  ta_ptr >= vta_ptr; // { dg-error {comparison between distinct pointer types [^\n]*lacks a cast} }

  // New and delete.

  new ta; // { dg-error {invalid use of sizeless type 'ta'} }
  new ta (); // { dg-error {invalid use of sizeless type 'ta'} }

  new (global_ta_ptr) ta; // { dg-error {invalid use of sizeless type 'ta'} }
  new (global_ta_ptr) ta (); // { dg-error {invalid use of sizeless type 'ta'} }

  // Unary vector arithmetic.

  __real ta1; // { dg-error {wrong type argument to __real} }
  __imag ta1; // { dg-error {wrong type argument to __imag} }

  // Conditional expressions.

  0 ? ta_ptr : ta_ptr;
  0 ? ta_ptr : vta_ptr; // { dg-error {conditional expression between distinct pointer types [^\n]*lacks a cast} }
  0 ? vta_ptr : ta_ptr; // { dg-error {conditional expression between distinct pointer types [^\n]*lacks a cast} }

  // Function arguments.

  ext_consume_const_int_ref (ta1); // { dg-error {invalid initialization of reference of type 'const int&' from expression of type 'ta'} }
  ext_consume_varargs (ta1); // { dg-error {cannot convert 'ta'[^\n]* to 'int'} }

  // Use in traits.  Doesn't use static_assert so that tests work with
  // earlier -std=s.

  { typedef int f[__has_nothrow_assign (ta) ? 1 : -1]; }
  { typedef int f[__has_trivial_assign (ta) ? 1 : -1]; }
  { typedef int f[__has_nothrow_constructor (ta) ? 1 : -1]; }
  { typedef int f[__has_trivial_constructor (ta) ? 1 : -1]; }
  { typedef int f[__has_nothrow_copy (ta) ? 1 : -1]; }
  { typedef int f[__has_trivial_copy (ta) ? 1 : -1]; }
  { typedef int f[__has_trivial_destructor (ta) ? 1 : -1]; }
  { typedef int f[__has_unique_object_representations (ta) ? 1 : -1]; }
  { typedef int f[!__has_virtual_destructor (ta) ? 1 : -1]; }
  { typedef int f[!__is_abstract (ta) ? 1 : -1]; }
  { typedef int f[__is_aggregate (ta) ? 1 : -1]; }
  { typedef int f[!__is_base_of (ta, ta) ? 1 : -1]; }
  { typedef int f[!__is_base_of (ta, tb) ? 1 : -1]; }
  { typedef int f[!__is_class (ta) ? 1 : -1]; }
  { typedef int f[!__is_empty (ta) ? 1 : -1]; }
  { typedef int f[!__is_enum (ta) ? 1 : -1]; }
  { typedef int f[!__is_final (ta) ? 1 : -1]; }
  { typedef int f[__is_pod (ta) ? 1 : -1]; }
  { typedef int f[!__is_polymorphic (ta) ? 1 : -1]; }
  { typedef int f[__is_same_as (ta, ta) ? 1 : -1]; }
  { typedef int f[!__is_same_as (ta, vta) ? 1 : -1]; }
  { typedef int f[!__is_same_as (vta, ta) ? 1 : -1]; }
  { typedef int f[__is_same_as (ta *, ta *) ? 1 : -1]; }
  { typedef int f[!__is_same_as (ta *, vta *) ? 1 : -1]; }
  { typedef int f[!__is_same_as (vta *, ta *) ? 1 : -1]; }
  { typedef int f[!__is_same_as (ta, int) ? 1 : -1]; }
  { typedef int f[!__is_same_as (ta, tb) ? 1 : -1]; }
  { typedef int f[__is_trivial (ta) ? 1 : -1]; }
  { typedef int f[!__is_union (ta) ? 1 : -1]; }
  { typedef int f[__is_trivially_copyable (ta) ? 1 : -1]; }
}
