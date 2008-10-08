// { dg-do compile }
class X;

class Y
{
  typedef int type;
};

concept Concept<typename T>
{
  typename nodefault;
  typename void_t = void;
  typename int_c_t = int const;
  typename c_int_t = const int;
  typename ptr = int*;
  typename func = int(float);
  typename ptr_mem = int (X::*);
  typename ptr_memfunc = int (X::*)(float);
  typename class_t = X;
  typename nested_known_type = Y::type;
  typename nested_type = T::type;
  typename nested_template = T::template foo<int>;
  typename nested_template_type = T::template foo<int>::type;
  typename c_class_t = const X;
  typename c_nested_known_type = const Y::type;
  typename c_nested_type = const T::type;
  typename c_nested_template = const T::template foo<int>;
  typename c_nested_template_type = const T::template foo<int>::type;
  typename class_c_t = X const;
  typename nested_known_type_c = Y::type const;
  typename nested_type_c = T::type const;
  typename nested_template_c = T::template foo<int> const;
  typename nested_template_type_c = T::template foo<int>::type const;

  typename error = typename T::iter;
};
