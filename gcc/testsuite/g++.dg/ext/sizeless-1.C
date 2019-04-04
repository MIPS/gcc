typedef __sizeless_1 ta;

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
}
