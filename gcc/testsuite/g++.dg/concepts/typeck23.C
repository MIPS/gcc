// Contributed by Rolf Bonderer
// { dg-options "-std=c++0x" }
#include <concepts>

auto concept IterationControl<typename Iter>
 {
       bool Iter::first();
       //...
 };

class myiter{
public:
 myiter() : i(0){}
 bool first() {return i==0;}
private:
 int i;
};

template <typename Iter> requires IterationControl<Iter>
void check(Iter & it) {
 it.first();
}

int main() {
 myiter it;
 check(it);
}
