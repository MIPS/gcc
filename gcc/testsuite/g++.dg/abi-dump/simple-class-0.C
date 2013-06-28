// { dg-do compile { target { x86_64*-*-* } } }
// { dg-options "-fdump-abi=2" }
// { dg-skip-if "" { *-*-* } { "-flto" } { "" } }

struct S
{
  int m;

  S();
  S(const S&);
};

S::S()
{
  m = 0;
}

S::S(const S&o)
{
  m = o.m;
}

void
foo(S& a)
{
  S s = a;
}

// { dg-final { compare-abi-dump-with "simple-class-0.C.bi" } }
// { dg-final { cleanup-dump "bi" } }
