// ** Old, obsolete commentary:
// **************************************************************************
// **************************************************************************

// The above commentary is wrong.  (jason 1998/11/13)
// In fact, the lines marked OK are well-formed; the prohibition is only
// against forming array types with multiple unknown bounds.  This prohibition
// is found in 8.3.4 [dcl.array].

// It is also ill-formed to create an object of incomplete type.
extern int extern_two_d [] [];		// ERROR - invalid declaration
int (*pointer_to_two_d)[][];		// ERROR - invalid declaration
void function_0 (int arg [] []) {	// ERROR - invalid declaration
typedef int_one_d_type int_two_d_type[];// ERROR - invalid declaration
extern struct s extern_s_array [10];	// OK
struct s tenative_s_array [10];		// ERROR - object with incomplete type
static struct s static_s_array [10];	// ERROR - object with incomplete type
struct s (*pointer_to_s_array) [];	// OK
void function_1 (struct s arg []) {	// OK
typedef s_type s_one_d_type [10];	// OK
