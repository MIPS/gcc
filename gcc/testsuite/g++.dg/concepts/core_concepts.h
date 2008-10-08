namespace std {
auto concept CopyConstructible<typename T>
{
  T::T(const T&);
  T::~T();
};

concept SameType<typename T, typename U> { };
template<typename T> concept_map SameType<T, T> { };

concept DerivedFrom<typename Derived, typename Base> { };

concept True<bool> { };
concept_map True<true> { };
}
