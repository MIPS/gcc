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

* docs

* fixing all the state issues

* make the dirty dirty hacks less egregious...

* try porting llvmpipe to gcc

* pkgconfig .pc file

* add a SONAME to the library (and potentially version the symbols?)

etc etc
