// { dg-options "-std=c++1y" }


// The following error is emitted without context. I'm not
// certain why that would be the case. It comes as a result
// of failing the declaration of S::f0().
//
//    cc1plus: error: expected ';' at end of member declaration


struct Base {
  template<typename T, typename U>
    bool C() const { return false; } // Not a concept!

  template<typename T>
    static concept bool D() { return __is_same_as(T, int); }

  template<typename T, typename U>
    static concept bool E() { return __is_same_as(T, U); }
};

void f1(Base::D) { }
void f2(Base::E<double> x) { }

template<typename T>
  struct S : Base {
    void f0(Base::C<float> x) { } // { dg-error "expected|type" }
    void f1(Base::D) { }
    void f2(Base::E<T> x) { }
  };

int main() { 
  f1('a'); // { dg-error "matching" }
  f2(0);   // { dg-error "matching" }

  S<int> s;
  s.f1('a'); // { dg-error "matching" }
  s.f2('a'); // { dg-error "matching" }
}

