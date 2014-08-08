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

* expose the statements in the API? (mostly so they can be stringified?)

* support more arithmetic ops and comparison modes

* access to a function by address::

    extern gcc_jit_function *
    gcc_jit_context_get_function (ctxt,
                                  void *); /* need type information */

  so you can access "static" fns in your code.

* ability to turn a function into a function pointer::

    gcc_jit_function_as_rvalue ()

* clarify gcc_jit_function_add_eval()::

    (void)expression;

  and, indeed, clarify all of the other operations.

* expressing branch probabilies (like __builtin_expect)::

    extern gcc_jit_rvalue *
    gcc_jit_rvalue_likely (gcc_jit_rvalue *rvalue,
                           int is_likely);

  though would:

    extern void
    gcc_jit_block_set_likelihood (gcc_jit_block *block,
                                  int hotness);

  be better?  (for expressing how hot the current location is)

* ability to give contexts names, for ease of debugging?

* can we call into GCC builtins?  What might people need?

* docs

* add a SONAME to the library (and potentially version the symbols?)

* add myself as maintainer

* do we need alternative forms of division (floor vs rounding)?

* are we missing any ops?

* error-checking:

    * gcc_jit_context_new_unary_op: various checks needed

    * gcc_jit_context_new_binary_op: various checks needed

    * gcc_jit_context_new_comparison: must be numeric or pointer types

    * gcc_jit_context_new_array_access: "index" must be of numeric type.

    * gcc_jit_lvalue_access_field: must be field of correct struct

    * gcc_jit_rvalue_access_field: must be field of correct struct

    * gcc_jit_block_add_assignment_op: check the types

* Currently each function has a single stmt_list, which is built in
  postprocessing by walking the list of blocks.  Presumably we could
  have each block have its own stmt_list, avoiding the need for this
  traversal, and having the block structure show up within tree dumps.

* Implement more kinds of casts e.g. pointers

Bugs
====
* fixing all the state issues: make it work repeatedly with optimization
  turned up to full.

* make the dirty dirty hacks less egregious...

* pkgconfig .pc file

* test under valgrind; fix memory leaks

* re-architect gcc so we don't have to reinitialize everything every time
  a context is compiled

Test suite
==========
* get DejaGnu to build and run C++ testcases

* add a multi-threaded test (perhaps based on test-combination.c, with a
  thread pool working through multiple instances of the various underlying
  tests, each thread having a separate gcc_jit_context)

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
