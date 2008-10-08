concept Foo<typename T>
{
  typename type;
};

template<Foo X, type Value> void foo(); // { dg-error "has not been declared" }
