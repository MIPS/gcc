// FIXME lto.  We should really use 'compile' instead of 'link', but
// -S confuses lto1.
// { dg-do link }
// { dg-options "-c -flto-single -O2" }
struct Foo { void func (); }; Foo & bar () { } struct Baz { Baz (Baz &); };
Baz dummy() { bar().func(); }
