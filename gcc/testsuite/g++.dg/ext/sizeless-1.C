// { dg-options "-Wclass-memaccess" }

typedef __SIZE_TYPE__ size_t;
inline void *operator new (size_t, void *__p) throw() { return __p; }

typedef unsigned char vta __attribute__((__vector_size__(2)));
typedef __sizeless_1 ta;
typedef __sizeless_2 tb;

// Sizeless objects with global scope.

ta global_ta; // { dg-error {sizeless variable 'ta global_ta' cannot have static storage duration} }
static ta local_ta; // { dg-error {sizeless variable 'ta local_ta' cannot have static storage duration} }
extern ta extern_ta; // { dg-error {sizeless variable 'ta extern_ta' cannot have static storage duration} }
__thread ta tls_ta; // { dg-error {sizeless variable 'ta tls_ta' cannot have static storage duration} }

// Sizeless member variables.

struct struct1 {
  ta a; // { dg-error {field 'a' has incomplete type} }
};

union union1 {
  ta a; // { dg-error {field 'a' has incomplete type} }
};

#if __cplusplus >= 201103L
struct static_ta {
  static ta ta1 = {}; // { dg-error {in-class initialization of static data member 'ta static_ta::ta1' of incomplete type} "" { target c++11 } }
};
#endif

// Sizeless member variables in templated structures.

template<typename T>
struct templated_struct1 {
  ta a; // { dg-error {field 'a' has incomplete type} }
};

template<typename T>
struct templated_struct2 {
  T a; // { dg-error {'templated_struct2<T>::a' has incomplete type} }
       // { dg-error {invalid use of sizeless type} "" { target *-*-* } .-1 }
};

template class templated_struct2<ta>;

template<typename T>
struct templated_struct3 {
  T &a;
};

template class templated_struct3<ta>;

#if __cplusplus >= 201103L
template<typename T>
struct templated_struct4 {
  static T a = {}; // { dg-error {in-class initialization of static data member '[^']*' of incomplete type} "" { target c++11 } }
};

template class templated_struct4<ta>;
#endif

template<typename T> struct templated_struct5 : T {}; // { dg-error {base type '[^']*' fails to be a struct or class type} }
template class templated_struct5<ta>;

#if __cplusplus >= 201103L
template<int N> using typedef_sizeless1 = ta;
template<int N> using typedef_sizeless1 = ta; // { dg-error {redefinition of 'template<int N> using typedef_sizeless1 = ta'} "" { target c++11 } }
template<typename T> using array = T[2];
#endif

// Pointers to sizeless types.

ta *global_ta_ptr;

// Sizeless arguments and return values.

void ext_consume_ta (ta);
void ext_consume_const_int_ref (const int &);
void ext_consume_varargs (int, ...);
ta ext_produce_ta ();

// Sizeless types in throw specifications.

#if __cplusplus < 201103L
void thrower1 () throw (ta); // { dg-error {invalid use of sizeless type 'ta'} "" { target c++98_only } }
void thrower2 () throw (ta); // { dg-error {invalid use of sizeless type 'ta'} "" { target c++98_only } }
void thrower3 () throw (ta); // { dg-error {invalid use of sizeless type 'ta'} "" { target c++98_only } }
#endif

// Main tests for statements and expressions.

void
statements (int n)
{
  // Local declarations.

  ta ta1, ta2;
  volatile ta volatile_ta1;
  vta vta1;
  tb tb1;

  // Layout queries.

  sizeof (ta); // { dg-error {invalid application of 'sizeof' to incomplete type} }
  sizeof (ta1); // { dg-error {invalid application of 'sizeof' to incomplete type} }
  __alignof (ta); // { dg-error {invalid application of '__alignof__' to incomplete type} }

#if __cplusplus >= 201103L
  array<ta> foo = {}; // { dg-error {elements of array '[^']*' have incomplete type} "" { target c++11 } }
#endif

  // Initialization.

  int init_int1 = ta1; // { dg-error {cannot convert 'ta'[^\n]* to 'int' in initialization} }
  int init_int2 = { ta1 }; // { dg-error {cannot convert 'ta'[^\n]* to 'int' in initialization} }

  ta init_ta1 (ta1);
  ta init_ta2 (tb1); // { dg-error {cannot convert 'tb'[^\n]* to 'ta'[^\n]* in initialization} }
  ta init_ta3 = ta1;
  ta init_ta4 = tb1; // { dg-error {cannot convert 'tb'[^\n]* to 'ta'[^\n]* in initialization} }
  ta init_ta5 = {};

  // Constructor calls.

  (0, ta ());

  // Lvalue reference binding

  ta &lvalue_ref_ta1 = ta1;
  ta &lvalue_ref_ta2 = ext_produce_ta (); // { dg-error {cannot bind non-const lvalue reference of type 'ta&'[^\n]* to an rvalue of type 'ta'} }
  ta &lvalue_ref_ta3 = tb1; // { dg-error {invalid initialization of reference of type 'ta&'[^\n]* from expression of type 'tb'} }

  const ta &const_lvalue_ref_ta1 = ta1;
  const ta &const_lvalue_ref_ta2 = ext_produce_ta ();
  const ta &const_lvalue_ref_ta3 = tb1; // { dg-error {invalid initialization of reference of type 'const ta&'[^\n]* from expression of type 'tb'} }

  // Compound literals.

  (int) { ta1 }; // { dg-error {cannot convert 'ta'[^\n]* to 'int' in initialization} }

  // Assignment.

  n = ta1; // { dg-error {cannot convert 'ta'[^\n]* to 'int' in assignment} }

  ta1 = 0; // { dg-error {cannot convert 'int' to 'ta'[^\n]* in assignment} }
  ta1 = ta1;
  ta1 = vta1;
  vta1 = ta1;
  ta1 = tb1; // { dg-error {cannot convert 'tb'[^\n]* to 'ta'[^\n]* in assignment} }

  // Casting.

  (void) ta1;
  (void) volatile_ta1;
  (void) *&volatile_ta1;

  // Addressing and dereferencing.

  ta *ta_ptr = &ta1;
  vta *vta_ptr = &vta1;

  // Pointer assignment.

  vta_ptr = ta_ptr; // { dg-error {invalid conversion from 'ta\*'[^\n]* to 'vta\*'} }
  ta_ptr = vta_ptr; // { dg-error {invalid conversion from 'vta\*'[^\n]* to 'ta\*'} }

  // Pointer arithmetic.

  ++ta_ptr; // { dg-error {cannot increment a pointer to incomplete type 'ta'} }
  --ta_ptr; // { dg-error {cannot decrement a pointer to incomplete type 'ta'} }
  ta_ptr++; // { dg-error {cannot increment a pointer to incomplete type 'ta'} }
  ta_ptr--; // { dg-error {cannot decrement a pointer to incomplete type 'ta'} }
  ta_ptr += 0; // { dg-error {invalid use of sizeless type 'ta'} }
	       // { dg-error {in evaluation of} "" { target *-*-* } .-1 }
  ta_ptr += 1; // { dg-error {invalid use of sizeless type 'ta'} }
	       // { dg-error {in evaluation of} "" { target *-*-* } .-1 }
  ta_ptr -= 0; // { dg-error {invalid use of sizeless type 'ta'} }
	       // { dg-error {in evaluation of} "" { target *-*-* } .-1 }
  ta_ptr -= 1; // { dg-error {invalid use of sizeless type 'ta'} }
	       // { dg-error {in evaluation of} "" { target *-*-* } .-1 }
  ta_ptr - ta_ptr; // { dg-error {invalid use of sizeless type 'ta'} }
  vta_ptr - ta_ptr; // { dg-error {invalid operands of types 'vta\*'[^\n]* and 'ta\*'[^\n]* to binary 'operator-'} }
  ta_ptr - vta_ptr; // { dg-error {invalid operands of types 'ta\*'[^\n]* and 'vta\*'[^\n]* to binary 'operator-'} }
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

  ta1.~ta(); // { dg-error {expected class-name before '\(' token} }
  delete ta_ptr; // { dg-error {cannot delete objects of sizeless type} }
  delete[] ta_ptr; // { dg-error {cannot delete objects of sizeless type 'ta'} }

  // Unary vector arithmetic.

  __real ta1; // { dg-error {wrong type argument to __real} }
  __imag ta1; // { dg-error {wrong type argument to __imag} }

  // Conditional expressions.

  0 ? ta1 : ta1;
  0 ? ta_ptr : ta_ptr;
  0 ? ta_ptr : vta_ptr; // { dg-error {conditional expression between distinct pointer types [^\n]*lacks a cast} }
  0 ? vta_ptr : ta_ptr; // { dg-error {conditional expression between distinct pointer types [^\n]*lacks a cast} }

  // Function arguments.

  ext_consume_ta (ta1);
  ext_consume_ta (tb1); // { dg-error {cannot convert 'tb'[^\n]* to 'ta'} }
  ext_consume_const_int_ref (ta1); // { dg-error {invalid initialization of reference of type 'const int&' from expression of type 'ta'} }
  ext_consume_varargs (ta1); // { dg-error {cannot convert 'ta'[^\n]* to 'int'} }
  ext_consume_varargs (1, ta1);

  // Function returns.

  ext_produce_ta ();
  ta1 = ext_produce_ta ();
  tb1 = ext_produce_ta (); // { dg-error {cannot convert 'ta'[^\n]* to 'tb'[^\n]* in assignment} }

  // Auto

#if __cplusplus >= 201103L
  auto auto_ta1 = ta1;
  auto auto_ta1_return = ext_produce_ta ();
#endif

  // Varargs processing.

  __builtin_va_list valist;
  __builtin_va_arg (valist, ta);

  // Other built-ins

  __builtin_launder (ta1); // { dg-error {non-pointer argument to '__builtin_launder'} }
  __builtin_memcpy (&ta1, &ta2, 2);

  // Lambdas

#if __cplusplus >= 201103L
  [ta1] () {}; // { dg-error {capture by copy of incomplete type 'ta'} "" { target c++11 } }
  [=] () { &ta1; }; // { dg-error {capture by copy of incomplete type 'ta'} "" { target c++11 } }
  [&ta1] () { ta1 = ta2; }; // { dg-error {'ta2' is not captured} "" { target c++11 } }
  [&ta1, &ta2] () { ta1 = ta2; };
  [&] () { ta1 = ta2; };
  [] () { return ext_produce_ta (); } ();
#endif

  // Exceptions

  throw ta (); // { dg-error {invalid use of sizeless type 'ta'} }
  try {} catch (ta x) {} // { dg-error {invalid use of sizeless type 'ta'} }
  try {} catch (ta &x) {} // { dg-error {invalid use of sizeless type 'ta'} }
  try {} catch (ta *x) {} // { dg-error {invalid use of sizeless type 'ta'} }
#if __cplusplus < 201103L
  thrower2 ();
#endif

  // Structured bindings

#if __cplusplus >= 201703L
  ta &ta_ref = ta1;
  auto &[x1, x2] = ta_ref;
#endif

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
  /* The intention is that ta should behave like vta here.  If the behavior
     for vta changes then the behavior for ta should change in the same
     way.  */
  { typedef int f[!__is_trivially_assignable (vta, vta) ? 1 : -1]; }
  { typedef int f[!__is_trivially_assignable (ta, ta) ? 1 : -1]; }
  { typedef int f[!__is_trivially_assignable (ta, vta) ? 1 : -1]; }
  { typedef int f[!__is_trivially_assignable (vta, ta) ? 1 : -1]; }
  { typedef int f[!__is_trivially_assignable (ta, int) ? 1 : -1]; }
  { typedef int f[!__is_trivially_assignable (ta, tb) ? 1 : -1]; }
  { typedef int f[!__is_assignable (vta, vta) ? 1 : -1]; }
  { typedef int f[!__is_assignable (ta, ta) ? 1 : -1]; }
  { typedef int f[!__is_assignable (ta, vta) ? 1 : -1]; }
  { typedef int f[!__is_assignable (vta, ta) ? 1 : -1]; }
  { typedef int f[!__is_assignable (ta, int) ? 1 : -1]; }
  { typedef int f[!__is_assignable (ta, tb) ? 1 : -1]; }
  { typedef int f[__is_trivially_constructible (ta) ? 1 : -1]; }
  { typedef int f[__is_trivially_constructible (vta, vta) ? 1 : -1]; }
  { typedef int f[__is_trivially_constructible (ta, ta) ? 1 : -1]; }
  { typedef int f[__is_trivially_constructible (ta, vta) ? 1 : -1]; }
  { typedef int f[__is_trivially_constructible (vta, ta) ? 1 : -1]; }
  { typedef int f[!__is_trivially_constructible (ta, int) ? 1 : -1]; }
  { typedef int f[!__is_trivially_constructible (ta, tb) ? 1 : -1]; }
  { typedef int f[__is_constructible (ta) ? 1 : -1]; }
  { typedef int f[__is_constructible (vta, vta) ? 1 : -1]; }
  { typedef int f[__is_constructible (ta, ta) ? 1 : -1]; }
  { typedef int f[__is_constructible (ta, vta) ? 1 : -1]; }
  { typedef int f[__is_constructible (vta, ta) ? 1 : -1]; }
  { typedef int f[!__is_constructible (ta, int) ? 1 : -1]; }
  { typedef int f[!__is_constructible (ta, tb) ? 1 : -1]; }
}

// Function parameters in definitions.

void
unnamed_st1 (ta)
{
}

void
named_st1 (ta param1)
{
  ta ta1 = param1;
}

// Function return values in definitions.

ta
ret_st1 (ta param)
{
  return param;
}

ta
bad_ret_st1 (tb param)
{
  return param; // { dg-error {cannot convert 'tb'[^\n]* to 'ta'[^\n]* in return} }
}

#if __cplusplus >= 201103L
template<typename T>
void
const_to_ta (T i)
{
  constexpr ta a = (ta) i;
}
#endif

template<typename T>
int
get_x (T *a)
{
  return a->a; // { dg-error {request for member 'a' in '\* a', which is of non-class type} }
}
template int get_x<ta>(ta *);

#if __cplusplus < 201103L
void thrower3 () throw (ta) {} // { dg-error {invalid use of sizeless type 'ta'} "" { target c++98_only } }
#endif

// Using "auto" as a return type.

#if __cplusplus >= 201402L
auto auto_ret_ta (ta *ptr) { return *ptr; }
const auto &auto_ret_const_ta_ref (ta *ptr) { return *ptr; }
auto &auto_ret_ta_ref (ta *ptr) { return *ptr; }
auto &&auto_ret_ta_rvalue_ref (ta *ptr) { return *ptr; }
#endif
