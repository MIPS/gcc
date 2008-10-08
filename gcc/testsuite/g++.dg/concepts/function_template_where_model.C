// { dg-do compile }
#include <concepts>

concept PointerLike<typename T>
{
  typename value_type = T::value_type;
};

concept SmartPointerLike<typename T> : PointerLike<T>
{

};

template<typename T> requires SmartPointerLike<T>
  SmartPointerLike<T>::value_type foo(T);

late_check template<typename T> requires SmartPointerLike<T>
  typename T::value_type bar(T);

template<typename T> requires SmartPointerLike<T>
  typename SmartPointerLike<T>::value_type wibble(T);

template<typename T> requires SmartPointerLike<T>
  SmartPointerLike<T>::value_type wonka(T);
