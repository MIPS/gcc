// { dg-options "-std=c++1z" }

template<typename T>
concept bool NameProvider()
{
  return requires(){
    T::_name_t::template _member_t<int>;
  };
}

template<NameProvider... ColSpec>
void getTable(const ColSpec&...)
{}

int f()
{
  getTable(7, 'a'); // { dg-error "cannot call" }
};

// { dg-excess-errors "ColSpec|false" }
