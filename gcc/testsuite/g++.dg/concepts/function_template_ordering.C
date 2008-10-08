#include <concepts>

concept Superior<typename T> { };

concept Refined<typename T> : Superior<T> { };

concept UeberRefined<typename T> : Refined<T> { };

concept AlsoRefined<typename T> : Superior<T> { };

template<typename T> requires Superior<T> int& f(T); // { dg-error "candidates are" }
template<typename T> requires Refined<T> float& f(T);// { dg-error "float" }
template<typename T> requires UeberRefined<T> double& f(T);
template<typename T> requires AlsoRefined<T> bool& f(T); // { dg-error "bool" }

template<typename T> requires Superior<T> int& g(T);
template<typename T> requires Refined<T> float& g(T);
template<typename T> requires AlsoRefined<T> bool& g(T);
template<typename T> requires Refined<T>, AlsoRefined<T> double& g(T);


class S { };
class R { };
class UR { };
class AR { };
class R_AR { };

concept_map Superior<S> { };
concept_map Refined<R> { };
concept_map UeberRefined<UR> { };
concept_map AlsoRefined<AR> { };
concept_map Refined<R_AR> { };
concept_map AlsoRefined<R_AR> { };

void foo()
{
  int& an_int = f(S());
  float& a_float = f(R());
  double& a_double = f(UR());
  bool& a_bool = f(AR());

  f(R_AR()); // { dg-error "call of overloaded" }

  float& a_float2 = g(R());
  bool& a_bool2 = g(AR());
  double& a_double2 = g(R_AR());
}
