// Contributed by Jaakko Jarvi

concept C1<typename T> {
  typename type;
};



concept C2<typename T, typename U> : C1<T> { };


concept Other<typename T> { };

template<typename T> 
  requires Other<T> 
  concept_map C1<T> { typedef int type; }

template<typename T, typename U>
  requires Other<T>, Other<U>
  concept_map C2<T, U> { typedef int type; }; 
