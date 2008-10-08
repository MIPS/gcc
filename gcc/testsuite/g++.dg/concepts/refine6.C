namespace n1 {
  concept C1<typename T> { 
    bool operator==(const T&, const T&);
  };
}

namespace n2 {
  concept C2<typename T> : n1::C1<T> { };
  template<typename T> concept_map C2<T*> { };
}
