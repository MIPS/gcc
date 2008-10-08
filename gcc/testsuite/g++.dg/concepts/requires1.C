concept A<typename T> { }

template<typename T>
struct X 
{
  requires A<T>; // { dg-error "requirements clause" }
};
