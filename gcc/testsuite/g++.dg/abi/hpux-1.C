// { dg-do compile { target ia64-hpux-* } }

// Copyright (C) 2003 Free Software Foundation, Inc.
// Contributed by Nathan Sidwell 30 Nov 2003 <nathan@codesourcery.com>

// Test mangling of expressions with DECLs on hpux - it is different

extern const int L = 3;
template <int I> struct S1 {};
template <int I> void f (S1<L>) {};
template void f<7>(S1<L>);
// { dg-final { scan-assembler "\n_Z1fILi7EEv2S1ILi3EE:" } }

template <int I> void g (S1<L+I>) {};
template void g<7>(S1<L+7>);
// { dg-final { scan-assembler "\n_Z1gILi7EEv2S1IXplL1LET_EE:" } }

template <int I> void h (S1<L+1>) {};
template void h<7>(S1<L+1>);
// { dg-final { scan-assembler "\n_Z1hILi7EEv2S1ILi4EE:" } }

extern "C" void Foo ();
extern void Bar ();
template <void (*)()> struct S2 {};

void f2 (S2<Foo>){}
// { dg-final { scan-assembler "\n_Z2f22S2IXadL3FooEEE:" } }
void g2 (S2<Bar>){}
// { dg-final { scan-assembler "\n_Z2g22S2IXadL7_Z3BarvEEE:" } }

extern int N;
extern "C" int M;
template <int &> struct S3 {};

void n (S3<N>) {}
// { dg-final { scan-assembler "\n_Z1n2S3IXadL1NEEE:" } }
void m (S3<M>) {}
// { dg-final { scan-assembler "\n_Z1m2S3IXadL1MEEE:" } }

struct XXX {};
extern void Foo (XXX *, XXX *);

template <void (*)(XXX *, XXX *)> struct S4 {};

// Substitutions do not cross between nester and nestee
void f3 (XXX *, S4<Foo>){}
// { dg-final { scan-assembler "\n_Z2f3P3XXX2S4IXadL14_Z3FooP3XXXS0_EEE:" } }
void g3 (S4<Foo>, XXX *){}
// { dg-final { scan-assembler "\n_Z2g32S4IXadL14_Z3FooP3XXXS0_EEEP3XXX:" } }
