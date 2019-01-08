<<<<<<< HEAD
// { dg-do compile { target c++17 } }
// { dg-options "-fconcepts" }
=======
// needs port; error message?
// { dg-options "-std=c++17 -fconcepts" }
>>>>>>> 594131fbad3... move ported tests; note more issues and needs port after fixes

template<typename T>
concept bool NameProvider()
{
  return requires(){
    typename T::_name_t::template _member_t<int>;
  };
}

template<NameProvider... ColSpec>
void getTable(const ColSpec&...)
{}

void f()
{
  getTable(7, 'a'); // { dg-error "cannot call" }
};
