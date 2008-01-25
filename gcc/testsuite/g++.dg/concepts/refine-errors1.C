// { dg-options "-std=c++0x" }
// Check for error messages regarding ill-formed refinements

concept C<typename T> : C<T*> { } // { dg-error "recursive" }
concept D<typename T> : C<int> { } // { dg-error "concept parameters" }

template<typename T> class Class { };
concept E<typename T> : Class<T> { }; // { dg-error "does not refer to a concept" }

concept F<typename... Args> : C<Args> { } // { dg-error "not expanded|Args" }
