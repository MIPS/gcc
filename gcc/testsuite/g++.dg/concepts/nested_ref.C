// { dg-options "-std=c++0x" }
// Contributed by Rolf Bonderer

void show(double t) {}

auto concept A<typename type> 
{
  void show(const type& t);
};

//Now comes the important part: Refinement and nested requirement simultaneously!

auto concept B<typename type> : A<type>
{
  requires A<type>;
};

template<B type>
void test(const type& t)
{
  show(t);
}

int main() {
  double t=1.;
  test(t);
}
