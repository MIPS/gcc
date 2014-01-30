TODOs
-----

Initial Release
===============
* error-handling:
    * have a client-provided error-handling callback for the context, and
      call it, rather than asserting/crashing etc, to make the API resilient and helpful

* probably should turn off signal handlers and backtracing, leaving that to
  the client code

* enums and ABI: give enums specific numbers, in ranges, to make it
  possible to maintain a logical ordering whilst preserving ABI.

* more language features:

  * more types:
    * unions
    * function ptrs

* how do you encode "x[5]=y;"?  should gcc_jit_context_new_array_lookup()
  return an lvalue rather than an rvalue?

* explicit casts::

    extern gcc_jit_rvalue *
    gcc_jit_rvalue_cast (gcc_jit_rvalue *, gcc_jit_type *);

  e.g. (void*) to (struct foo*)

* support more arithmetic ops and comparison modes

* access to a function by address::

    extern gcc_jit_function *
    gcc_jit_context_get_function (ctxt,
                                  void *); /* need type information */

  so you can access "static" fns in your code.

* ability to call an rvalue function pointer, perhaps keeping the
  existing API to avoid needing to build a function ptr from a
  function.

* ability to turn a function into a function pointer::

    gcc_jit_function_as_rvalue ()

* clarify gcc_jit_function_add_eval()::

    (void)expression;

  and, indeed, clarify all of the other operations.

* array types, in case they're needed for structs::

    extern gcc_jit_type *
    gcc_jit_context_new_array_type (gcc_jit_context *ctxt,
                                    gcc_jit_type *type,
                                    int num_elements);

* expressing branch probabilies (like __builtin_expect)::

    extern gcc_jit_rvalue *
    gcc_jit_rvalue_likely (gcc_jit_rvalue *rvalue,
                           int is_likely);

  though would:

    extern void
    gcc_jit_function_set_likelihood (gcc_jit_function *func,
                                     int hotness);

  be better?  (for expressing how hot the current location is)

* can we call into GCC builtins?  What might people need?

* docs

* fixing all the state issues

* make the dirty dirty hacks less egregious...

* pkgconfig .pc file

* add a SONAME to the library (and potentially version the symbols?)

* add myself as maintainer

* valgrind; fix memory leaks

* do we need alternative forms of division (floor vs rounding)?

* are we missing any ops?

* error-checking:

    * gcc_jit_context_new_field: type must not be void

    * gcc_jit_context_new_param: type must not be void

    * gcc_jit_context_new_global: type must not be void

    * gcc_jit_context_new_rvalue_from_int: must be a numeric type

    * gcc_jit_context_zero: must be a numeric type

    * gcc_jit_context_one: must be a numeric type

    * gcc_jit_context_new_rvalue_from_double: must be a numeric type

    * gcc_jit_context_new_rvalue_from_ptr: must be a pointer type

    * gcc_jit_context_new_unary_op: various checks needed

    * gcc_jit_context_new_binary_op: various checks needed

    * gcc_jit_context_new_comparison: must be numeric or pointer types

    * gcc_jit_context_new_array_lookup: "ptr" must be of pointer type;
      "index" must be of numeric type.

    * gcc_jit_lvalue_access_field: must be field of correct struct

    * gcc_jit_rvalue_access_field: must be field of correct struct

    * gcc_jit_function_new_local: type must not be void

    * gcc_jit_function_place_forward_label: must not already have been
      placed

    * gcc_jit_function_add_assignment_op: check the types

    * gcc_jit_function_add_conditional: boolval must be of numeric type

    * gcc_jit_loop_end: verify that loops are validly nested?

    * verify that all used labels have been placed when attempting to
      compile

Future milestones
=================
* try porting llvmpipe to gcc

* fuzz testing

* inline assembler?

* measure code coverage in testing of libgccjit.so

* "switch" and "case" ?

* do we need unary plus?
* shift operators?
* sizeof (should this be an API hook?)  do we even need it? presumably
  client code can just do the sizeof() in its own code.

etc etc
