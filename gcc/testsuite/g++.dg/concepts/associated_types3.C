concept Con<typename T>
{
  typename value_type = T::value_type;
};

concept RefinesCon<typename T> : Con<T> { };

struct IntValueType
{
  typedef int value_type;
};

concept_map Con<IntValueType> { };

Con<IntValueType>::value_type x = 7;

concept ConNoDefault<typename T>
{
  typename value_type;
};

struct NoValueType
{
};

concept_map Con<NoValueType> {}; // { dg-error "no type named" }
concept_map ConNoDefault<NoValueType> {}; // { dg-error "does not define associated type" }

template<typename T>
concept_map RefinesCon<T*>
{
  typedef T value_type;
};

RefinesCon<int*>::value_type y = 42;

template<typename X, typename Y>
struct Foo
{
  typedef X value_type;
};

template<typename X, typename Y>
concept_map RefinesCon<Foo<X, Y> > { };

RefinesCon<Foo<int, IntValueType> >::value_type z = 17;
