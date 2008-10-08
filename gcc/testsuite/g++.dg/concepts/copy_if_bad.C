// { dg-options "-std=c++0x" }
#include <iterator>
using namespace std;

template<InputIterator InIter, typename OutIter, typename Pred>
requires Predicate<Pred, InIter::reference> 
     , OutputIterator<OutIter, InIter::reference>
OutIter 
copy_if(InIter first, InIter last, OutIter out, Pred pred) {
  while (first < last) { // { dg-error "no match for" }
    if (pred(*first)) *out++ = *first;
    ++first;
  }
  return out;
}

// { dg-error "candidates are" "" { target *-*-* } 268 }
