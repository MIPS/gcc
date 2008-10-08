concept LessThanComparable<typename T>
{
  bool operator<(const T& x, const T& y);
};

template<typename T> requires LessThanComparable<T>
struct less
{
};

concept Nothing<typename T> { };

template<Nothing T> 
struct honk
{
};

template<Nothing T>
struct honk<less<T> >
{
  bool compare(const T& x, const T& y) { return x < y; }
};

