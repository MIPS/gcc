// { dg-options "-std=c++0x" }
// Contributed by Rolf Bonderer
namespace std {
 auto concept CopyConstructible<typename T> {
   T::T(const T&);
   T::~T();
 };
}

concept A<typename S, typename T>
{
  typename typeA = S::typeS;
};

concept B<typename T> 
{
  typename typeB = T::typeT;
};

concept C<typename S, typename T>
: A<S,T>, B<T>
{}; 

template <typename S, typename T>
requires C<S,T>
void test(S ss, T tt) {}

class s { public: typedef double typeS; };
class t { public: typedef double typeT; };

concept_map C<s,t> {}

int main() {
  s ss; t tt;
  test(ss,tt);
}

