// { dg-do compile }

concept C<typename T> { };

template<typename T> requires !C<T> void foo(T&); // { dg-error "candidates" }

concept_map C<int> {};

void f()
{
  float f;
  foo(f);

  int  i;
  foo(i); // { dg-error "no matching function" }
  // { dg-error "template requires" "" { target *-*-* } 15 }
}
