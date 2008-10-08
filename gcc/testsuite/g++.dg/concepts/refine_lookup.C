// { dg-do compile }
concept Base<typename X> {
  X& operator++(X&);
}

concept Refined1<typename X> : Base<X> { }
concept Refined2<typename X> : Base<X> { }

template<typename X>
requires Refined1<X> && Refined2<X> 
void foo(X& x) 
{
  ++x;
}

