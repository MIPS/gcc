concept DefaultConstructible<typename T>
{
  T::T();
};

concept CopyConstructible<typename T>
{
  T::T(const T&);
};

concept JustAName<typename T>
{
};

template<typename T> requires JustAName<T>
void foo()
{
  T(); // { dg-error "no matching|candidates are" }
}

template<typename T> requires DefaultConstructible<T>
void foo()
{
  T();
}
