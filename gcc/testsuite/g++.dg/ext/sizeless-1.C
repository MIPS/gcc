typedef __SIZE_TYPE__ size_t;
inline void *operator new (size_t, void *__p) throw() { return __p; }

typedef __sizeless_1 ta;
typedef __sizeless_2 tb;

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

ta ext_produce_ta ();

// Main tests for statements and expressions.

void
statements (int n)
{
  // New and delete.

  new ta; // { dg-error {invalid use of sizeless type 'ta'} }
  new ta (); // { dg-error {invalid use of sizeless type 'ta'} }

  new (global_ta_ptr) ta; // { dg-error {invalid use of sizeless type 'ta'} }
  new (global_ta_ptr) ta (); // { dg-error {invalid use of sizeless type 'ta'} }

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
  { typedef int f[!__is_same_as (ta, int) ? 1 : -1]; }
  { typedef int f[!__is_same_as (ta, tb) ? 1 : -1]; }
  { typedef int f[__is_trivial (ta) ? 1 : -1]; }
  { typedef int f[!__is_union (ta) ? 1 : -1]; }
  { typedef int f[__is_trivially_copyable (ta) ? 1 : -1]; }
}
