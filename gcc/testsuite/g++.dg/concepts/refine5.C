namespace A {
  concept C<typename T> { };
}

namespace B {
  concept D<typename T> : A::C<T> { };

   concept_map D<int> { };
}

