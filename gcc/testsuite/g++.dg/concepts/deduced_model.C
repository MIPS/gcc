concept InputIterator<typename T>
{
};

template<typename Iter>
struct reverse_iterator
{
};

template<typename Iter>
requires InputIterator<Iter>
concept_map InputIterator<reverse_iterator<Iter> >
{
};

template<typename T>
concept_map InputIterator<T*>
{
};

template<typename T> requires InputIterator<T> void foo(const T&) {}

template<typename T> requires InputIterator<T>
void bar(const T& first)
{
  reverse_iterator<T> rev;
  foo(rev);
}

void wibble()
{
  int* x = 0;
  bar(x);
}
