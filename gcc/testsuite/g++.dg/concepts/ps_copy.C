concept Assignable<typename T, typename U = T>
{
  typename result_type;
  result_type T::operator=(const U&);
};

concept_map Assignable<int, int> {};
concept_map Assignable<int&, int> {};
concept_map Assignable<unsigned long, int> {};
concept_map Assignable<unsigned long&, int> {};

#ifndef NO_ERRORS
concept_map Assignable<int, float*> {}; // { dg-error "invalid conversion" }
#endif
