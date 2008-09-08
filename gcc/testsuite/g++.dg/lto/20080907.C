// { dg-do assemble }
struct Foo { void func (); }; Foo & bar () { } struct Baz { Baz (Baz &); };
Baz dummy() { bar().func(); }
