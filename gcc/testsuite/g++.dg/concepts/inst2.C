template<typename T>
struct allocator
{
  typedef T type;
};

template<typename T, typename Alloc = allocator<T> >
struct vector
{
  typename allocator<T>::type value;
};

concept Nothing<typename T> { };

template<typename T> requires Nothing<T>
void f()
{
  vector<T> v;
}
