// { dg-options "-std=c++1z" }

struct Base {
  template<typename T>
    static concept bool D() { return __is_same_as(T, int); }

  template<typename T, typename U>
    static concept bool E() { return __is_same_as(T, U); }
};

void f1(Base::D) { }
void f2(Base::E<double> x) { }

template<typename T>
  struct S : Base {
    void f1(Base::D) { }
    void f2(Base::E<T> x) { }
  };

int main() { 
  f1(0);

  f2(0.0);

  S<int> s;
  s.f1(0);
  s.f2(0);
}
