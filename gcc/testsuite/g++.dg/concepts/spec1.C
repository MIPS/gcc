// Contributed by Niels Dekker
#include <algorithm>

class A {
  A& operator=(const A&);
  A(const A&);
};

namespace std {
  template <>    
  void swap(A&, A&); // { dg-error "does not match any template declaration" }
}

int main() {
  return 0;
}
