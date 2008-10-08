namespace std {
  auto concept CopyConstructible<typename T>
  {
    T::T(const T&);
    T::~T();
  };
}

concept Arrowable<typename Ptrlike, typename Value> 
{ 
  typename arrow_result;  
  //  arrow_result Ptrlike::operator->() const; 
  arrow_result operator->(Ptrlike);
  requires Arrowable<arrow_result, Value>; // { dg-error "sorry" }
};

template<typename T>
concept_map Arrowable<T*, T*>
{
  typedef T* arrow_result;
};

struct XArrowProxy
{
  struct Proxy
  {
    int* operator->() const;
  };

  Proxy operator->() const;
};

concept_map Arrowable<XArrowProxy::Proxy, int*> { typedef int* arrow_result; };

concept_map Arrowable<XArrowProxy, int*> 
{ 
  typedef int* arrow_result; 
};
