namespace std {
  auto concept CopyConstructible<typename T> {
    T::T(const T&);
    T::~T();
  }
}

auto concept SignedIntegral<typename T> {
  T::T(int);
};

auto concept InputIterator<typename Iter> {
  typename difference_type;
  requires std::CopyConstructible<difference_type> 
         , SignedIntegral<difference_type>;
};

template<typename MaybeIter>
struct iterator_traits {
  typedef typename MaybeIter::difference_type difference_type;
};

template<InputIterator InIter>
struct iterator_traits<InIter> {
  typedef InIter::difference_type difference_type;
};

template<InputIterator FooIter>
void foo(FooIter) {
  typename iterator_traits<FooIter>::difference_type n = 0;
}
