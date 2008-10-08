// { dg-options "-std=c++0x" }
// Contributed by Peter Gottschling
#include <concepts>

using namespace std;

auto concept A<typename T>
{
  requires std::CopyAssignable<T>;
}

concept B<typename T> : A<T> {}

concept C<typename T>
{
  requires A<T>;
}

auto           // creates ambiguity
concept CW<typename T> 
{
  requires C<T>;
  requires CopyConstructible<T>;
}


concept_map B<short> {}


template <A T> int& f(T x);
template <B T> short& f(T x);
template <C T> float& f(T x);
template <CW T> double& f(T x);


void f() {
  
  int& fc = f(char(0));
  short& fs = f(short(0));
  int& fl = f((long int) 0);
}






