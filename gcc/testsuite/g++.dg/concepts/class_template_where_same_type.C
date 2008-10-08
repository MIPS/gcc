// { dg-do compile }
#include <concepts>
late_check template<typename Metafunction, typename T>
  requires std::SameType<typename Metafunction::template apply<T>::type, T>
class requires_identity_function {};

struct add_pointer
{
  template<typename T>
  struct apply
  {
    typedef T* type;
  };

  template<typename T>
  struct apply<T&>
  {
    typedef T& type;
  };
};


int foo()
{
  requires_identity_function<add_pointer, int&> ok;
  requires_identity_function<add_pointer, int*> triggers_error; // { dg-error "do not meet the requirements of the" }
  // { dg-error "same-type constraint" "" { target *-*-* } 26 }
  // { dg-error "incomplete" "" { target *-*-* } 26 }
}
