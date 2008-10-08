// { dg-do compile }
// { dg-options "-std=c++0x" }
// Contributed by Rolf Bonderer
#include <concepts>

auto concept Assignable<typename T, typename U = T>
{
 T& operator=(T&, const U&);
};

//No error message when Addable is defined as follows:
#if 0
auto concept Addable<typename T, typename U = T>
{
 T operator+(const T& t, const U& u);
};
#else
auto concept Addable<typename T, typename U = T>
{
  typename result_type = T;
  result_type operator+(const T& t, const U& u);
};
#endif

auto concept Scalar<typename S>
{
 requires Addable<S>;
 requires Assignable<S>;
  requires Assignable<S, Addable<S,S>::result_type>;
};


template <Scalar S>
void Assign(S& t, S& s, S&v) {
  v=s+t;
 }

int main() {
 double t(1.),s(2.),v(3.);
 Assign(t,s,v);
}

#if 0
//End of program

concept_map Addable<double> {
 typedef double result_type;
};
#endif
