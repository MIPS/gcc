// { dg-options "-gdwarf-2 -fdeclone-ctor-dtor -dA" }
// { dg-do compile }

struct bar {
  bar ();
  ~bar ();
};

struct foo : virtual bar {
  foo ();
  ~foo ();
};

foo::foo () {}
foo::~foo () {}

// { dg-final { scan-assembler-times " DW_AT_trampoline" 4 } }
