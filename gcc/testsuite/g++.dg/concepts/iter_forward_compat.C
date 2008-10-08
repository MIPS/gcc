// { dg-options "-std=c++0x" }
// { dg-do "run" }
#include <iterator>

namespace std {
  template<InputIterator Iter>
  struct iterator_traits<Iter> {
    typedef input_iterator_tag    iterator_category;
    typedef Iter::value_type      value_type;
    typedef Iter::reference       reference;
    typedef Iter::pointer         pointer;
    typedef Iter::difference_type difference_type;
  };
}

using namespace std;

template<InputIterator Iter, typename T>
requires EqualityComparable<typename iterator_traits<Iter>::value_type, T>
typename iterator_traits<Iter>::difference_type
count(Iter first, Iter last, const T& value) 
{
  typename iterator_traits<Iter>::difference_type n = 0;
  for (; first != last; ++first)
    if (*first == value)
      ++n;

  return n;
}

class counting_iterator {
public:
  counting_iterator(int value = 0) : value(value) { }

  int operator*() const { return value; }
  
  const int* operator->() const { return &value; }

  counting_iterator& operator++() { ++value; return *this; }

  counting_iterator operator++(int) { 
    counting_iterator tmp(*this);
    ++(*this);
    return tmp;
  }

  int value;
};

namespace std {
  concept_map InputIterator<counting_iterator> {
    typedef int value_type;
    typedef int reference;
    typedef const int* pointer;
    typedef int difference_type;

    bool operator==(const counting_iterator& x, const counting_iterator& y) {
      return x.value == y.value;
    }
    bool operator!=(const counting_iterator& x, const counting_iterator& y) {
      return x.value != y.value;
    }
  };
}

template<typename InputIterator, typename T>
typename iterator_traits<InputIterator>::difference_type
count2(InputIterator first, InputIterator last, const T& value) 
{
  typename iterator_traits<InputIterator>::difference_type n = 0;
  for (; first != last; ++first)
    if (*first == value)
      ++n;

  return n;
}

class counting_iterator2 {
public:
  counting_iterator2(int value = 0) : value(value) { }

  int operator*() const { return value; }
  
  const int* operator->() const { return &value; }

  counting_iterator2& operator++() { ++value; return *this; }

  counting_iterator2 operator++(int) { 
    counting_iterator2 tmp(*this);
    ++(*this);
    return tmp;
  }

  int value;
};

bool operator==(const counting_iterator2& x, const counting_iterator2& y) {
  return x.value == y.value;
}
bool operator!=(const counting_iterator2& x, const counting_iterator2& y) {
  return x.value != y.value;
}

namespace std {
  concept_map InputIterator<counting_iterator2> {
    typedef int value_type;
    typedef int reference;
    typedef const int* pointer;
    typedef int difference_type;
  };
}

extern "C" void abort();

int main()
{
  if (::count(counting_iterator(5), counting_iterator(10), 7) != 1)
    abort();
  if (::count2(counting_iterator2(5), counting_iterator2(10), 7) != 1)
    abort();
  return 0;
}
