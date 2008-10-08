// Ticket #6
#include <concepts>

template <typename T> 
requires std::Convertible<int, T>,
         std::CopyConstructible<T> 
T test() 
{
  T t = 1;
  return t;
}

int k = test<int>();
