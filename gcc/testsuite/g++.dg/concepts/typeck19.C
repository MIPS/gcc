concept Assignable<typename T, typename U = T>
{
  T& operator=(T&, const U&); // { dg-error "candidates are" }
};

concept Nothing<typename> { };

template<typename T> requires Nothing<T>
T& assign(T& t, const T& u)
{
  return t = u; // { dg-error "no match for" }
}

template<typename T, typename U> requires Nothing<U>
T& assign(T& t, const U& u)
{
  t = t; // { dg-error "no match for" }
  return t = u; // { dg-error "no match for" }
}

template<Assignable T>
T& assign_ok(T& t, const T& u)
{
  return t = u;
}

template<typename T, typename U> requires Assignable<T, U>
T& assign(T& t, const U& u)
{
  t = t; // { dg-error "no match for" }
  return t = u;
}
