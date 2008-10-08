// Contributed by Jaakko Jarvi

concept C1<typename T> {
  typename type;
};



concept C2<typename T, typename U> : C1<T> { };


concept Other<typename T> { };

template<typename T, typename U>
  requires Other<T>, Other<U>
concept_map C2<T, U> { typedef int type; }; // { dg-error "cannot be generated|the following template parameters|U|you will need" }
