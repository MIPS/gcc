TODOs
-----

Initial Release
===============
* error-handling:
    * have a client-provided error-handling callback for the context, and
      call it, rather than asserting/crashing etc, to make the API resilient and helpful

* segfault seen in libbacktrace, when an ICE occurs

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

* the C unary prefix "&" operator::

    extern gcc_jit_rvalue *
    gcc_jit_lvalue_get_address (gcc_jit_lvalue *lvalue);

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
