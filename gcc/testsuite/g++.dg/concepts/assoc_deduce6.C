namespace std {
  auto concept CopyConstructible<typename T> {
    T::T(const T&);
    T::~T();
  };
}

concept InputIterator<typename X> {
  typename postincrement_result;
  postincrement_result operator++(X&, int);
};

template<std::CopyConstructible T>
concept_map InputIterator<const T*> { };
