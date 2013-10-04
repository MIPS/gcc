TODOs:

* gcc_jit_context_new_local should take a function, not the context (is
  it building a global?)

* error-handling:
    * have a client-provided error-handling callback for the context, and
      call it, rather than asserting/crashing etc, to make the API resilient and helpful

* more language features:

  * more types:
    * unions
    * function ptrs

* demo of JIT-compilation of a bytecode interpreter:
    https://github.com/davidmalcolm/jittest
  (move this into tree, as an example)

* wiring up testing into a DejaGNU testsuite, rather than testing it
  by hand.

* docs

* fixing all the state issues

* make the dirty dirty hacks less egregious...

* try porting llvmpipe to gcc

* source locations: line-numbering etc

* pkgconfig .pc file

etc etc
