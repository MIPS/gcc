#include <iterator>
#include <concepts>
#include <memory>

namespace std {
  template<CopyConstructible T>
  class vector
  {
  public:
    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& cont_reference;
    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;

    explicit vector() { }

    explicit vector(size_type n, const value_type& value = value_type())
    {
      start = (T*)malloc(sizeof(value_type) * n);
      finish = start + n;
      end_of_storage = start + n;
      uninitialized_fill(start, finish, value);
    }

  private:
    T* start;
    T* finish;
    T* end_of_storage;
  };
}
