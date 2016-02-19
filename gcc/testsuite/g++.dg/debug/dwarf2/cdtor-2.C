// { dg-options "-gdwarf-2 -dA" }
/* { dg-require-alias "" } */
// { dg-do compile }

struct foo {
  foo ();
  ~foo ();
};

foo::foo () {}
foo::~foo () {}

// { dg-final { scan-assembler-times " DW_AT_import" 2 } }
