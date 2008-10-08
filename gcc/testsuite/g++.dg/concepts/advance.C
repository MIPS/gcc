// { dg-do compile }
concept InputIterator<typename Iter> { typename difference_type; }
concept BidirectionalIterator<typename Iter> : InputIterator<Iter> { }

template<InputIterator Iter> 
  void advance(const Iter&, const Iter::difference_type&);
template<BidirectionalIterator Iter> 
  void advance(const Iter&, const Iter::difference_type&);

concept_map BidirectionalIterator<int*> 
{
  typedef int difference_type;
};

void f() 
{
  int* p = 0;
  advance(p, 17);
}
