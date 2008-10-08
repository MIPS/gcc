concept C<typename T> {
  void foo();
}

concept D<typename T> {
  C assoc;
}

concept_map D<int> { 
  typedef float assoc; 
} // { dg-error "does not meet the nested requirements" }
// { dg-error "no concept map for requirement" "" { target *-*-* } 11 }
