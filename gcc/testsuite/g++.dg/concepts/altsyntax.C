concept Concept<typename T>
{
  typename assoc_type;
};

template<Concept C> C::assoc_type* foo();

concept ConceptDefault<typename T, typename U = T>
{
  typename assoc_type_default;
};

template<ConceptDefault C> C::assoc_type_default* foo();

concept Concept2<typename T, typename U>
{
  typename assoc_type2;
};

template<Concept2 C> void foo(); // { dg-error "wrong number of template arguments" }
// { dg-error "provided for" "" { target *-*-* } 15 }

concept HasRequirement<typename T>
{
  requires Concept<T>;
};


