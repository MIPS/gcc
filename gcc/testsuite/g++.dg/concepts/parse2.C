// { dg-options "-std=c++0x" }
// Check parsing of concepts with refinement clauses
concept C1<typename T> { }
concept C2<typename T> : C1<T> { };
concept C3<typename T> { }
concept C4<typename T> : C1<T>, C2<T> { };

namespace N1 {
  concept C1<typename T> { }
  namespace Inner {
    concept C2<typename T> { }
  }
}

namespace N2 {
  namespace Inner { }

  concept D<typename T> : C1<T>, N1::C1<T>, ::N1::Inner::C2<T>, ::C3<T> { }
}
