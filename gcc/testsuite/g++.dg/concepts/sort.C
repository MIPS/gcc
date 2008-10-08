#include <concepts>

namespace fake_std {
  concept InputIterator<typename Iter> {};
  concept OutputIterator<typename Iter> {};
  concept ForwardIterator<typename Iter> 
    : InputIterator<Iter>, OutputIterator<Iter> {};
  concept BidirectionalIterator<typename Iter> 
    : ForwardIterator<Iter> {};
    concept RandomAccessIterator<typename Iter> 
    : BidirectionalIterator<Iter> {};

  template<typename T> struct list_iterator {};

  template<typename T>
  concept_map BidirectionalIterator<list_iterator<T> > {};

  template<typename T> struct list
  {
    typedef list_iterator<T> iterator;
    iterator begin() { return iterator(); }
    iterator end() { return iterator(); }
  };


  template<typename Iter> requires RandomAccessIterator<Iter>
  void sort(Iter first, Iter last) {}
}

int main()
{
  fake_std::list<int> l;
  fake_std::sort(l.begin(), l.end()); // { dg-error "no matching" }
  // { dg-error "candidates are" "" { target *-*-* } 27 }
  // { dg-error "no concept map for requirement" "" { target *-*-* } 33 }
  // { dg-error "syntactically matches" "" { target *-*-* } 10 }
  // { dg-error "if the concept semantics are met" "" { target *-*-* } 33 }
  // { dg-error "namespace fake_std" "" { target *-*-* } 0 }
  // { dg-error "concept_map RandomAccessIterator" "" { target *-*-* } 0 }
  // { dg-error "\}" "" { target *-*-* } 0 }
  return 0;
}
