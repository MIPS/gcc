concept Concept<typename T> {};

template<typename T> struct X {};

template<typename T> concept_map Concept<X<T> > { };

concept_map Concept<int> {};
