// Iterator concept definitions -*- C++ -*- .

// Copyright (C) 2005, 2006, 2007, 2008 Free Software Foundation, Inc.
// Authors: Douglas Gregor, Jeremiah Willcock, and Andrew Lumsdaine
//

// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License. */

/** @file iterator_concepts.h
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */
#ifndef _ITERATOR_CONCEPTS_H
#define _ITERATOR_CONCEPTS_H

#pragma GCC system_header

#include <bits/concepts.h>

#ifndef _GLIBCXX_NO_CONCEPTS
namespace std 
{
  concept Iterator<typename X> : Semiregular<X> 
  {
    // FIXME: should be MoveConstructible?
    CopyConstructible reference = typename X::reference; 
    reference operator*(X&&);
    
    // FIXME: We want to remove this default value, but it won't fully
    // work until all of ConceptGCC's concept maps are constrained.
    CopyConstructible postincrement_result = X;
    requires Dereferenceable<postincrement_result>;

    X& operator++(X&);
    postincrement_result operator++(X&, int);   
  }

  // FIXME: Work on removing IteratorBase entirely. Also, bring the
  // requirements of value_type, reference, and pointer in sync with
  // the current proposal.
  concept IteratorBase<typename X> : Iterator<X>
  {
    typename value_type = typename X::value_type;
    typename pointer = typename X::pointer;
  };

  // 24.1.1 (Table 73)
  concept InputIterator<typename X> : IteratorBase<X>, EqualityComparable<X>
  {
    SignedIntegralLike difference_type = typename X::difference_type;

    // FIXME: requires IntegralType<difference_type>

    requires Convertible<reference, const value_type&>;
    requires Convertible<pointer, const value_type*>;

    requires Convertible<Dereferenceable<postincrement_result>::reference,
                         const value_type&>;

    pointer operator->(const X&);
  };

  auto concept OutputIterator<typename X, typename Value>
  {
    requires Iterator<X>;
    typename reference = Iterator<X>::reference;
    typename postincrement_result = Iterator<X>::postincrement_result;
    requires SameType<reference, Iterator<X>::reference>
          && SameType<postincrement_result, Iterator<X>::postincrement_result>
          && Convertible<postincrement_result, const X&>
          && HasAssign<reference, Value>
          && HasAssign<Dereferenceable<postincrement_result>::reference, Value>;
  }

  concept BasicOutputIterator<typename X> : Iterator<X>
  {
    requires Convertible<postincrement_result, const X&>;

    typename value_type;
    requires HasAssign<reference, value_type&&>;

    requires HasAssign<Dereferenceable<postincrement_result>::reference,
                       value_type&&>;
  }

  // 24.1.3 (Table 75)

  // Note: This table actually specifies both a mutable and a
  // non-mutable forward iterator concepts. We have to split these into
  // two concepts. The mutable concept refines the non-mutable version.
  concept ForwardIterator<typename X>
    : InputIterator<X>, Regular<X>
  {
    requires Convertible<postincrement_result, const X&>;
  };

  // 24.1.4 (Table 76)

  // Note: This table actually specifies both a mutable and a
  // non-mutable bidirectional iterator concepts. We have to split these
  // into two concepts. The mutable concept refines the non-mutable
  // version.
  concept BidirectionalIterator<typename X>
    : ForwardIterator<X>
  {
    // FIXME: MoveConstructible<postdecrement_result>, remove default
    Dereferenceable postdecrement_result = X;
    requires Convertible<Dereferenceable<postdecrement_result>::reference, 
                         const value_type&>
          && Convertible<postdecrement_result, const X&>;

    X& operator--(X&);
    postdecrement_result operator--(X&, int);
  };

  // 24.1.5 (Table 77)

  // Note: This table actually specifies both a mutable and a
  // non-mutable bidirectional iterator concepts. We have to split these
  // into two concepts. The mutable concept refines the non-mutable
  // version.
  concept RandomAccessIterator<typename X>
   : BidirectionalIterator<X>,
     LessThanComparable<X>
  {
    X& operator+=(X&, difference_type);
    X  operator+ (const X& x, difference_type n) { X tmp(x); x += n; return x; }
    X  operator+ (difference_type n, const X& x) { X tmp(x); x += n; return x; }
    X& operator-=(X&, difference_type);
    X  operator- (const X& x, difference_type n) { X tmp(x); x -= n; return x; }
    difference_type operator-(const X&, const X&);
    reference operator[](const X& x, difference_type n) { return *(x + n); }
  };

  auto concept ShuffleIterator<typename X> {
    requires InputIterator<X>
          && NothrowDestructible<InputIterator<X>::value_type>
          && HasStdMove<InputIterator<X>::value_type>
          && HasStdMove<InputIterator<X>::reference>
          && OutputIterator<X, HasStdMove<InputIterator<X>::value_type>::result_type>
          && OutputIterator<X, HasStdMove<InputIterator<X>::reference>::result_type>
          && HasConstructor1<InputIterator<X>::value_type, HasStdMove<InputIterator<X>::reference>::result_type>
          && HasConstructor1<InputIterator<X>::value_type, HasStdMove<InputIterator<X>::value_type>::result_type>
          && HasAssign<InputIterator<X>::value_type, HasStdMove<InputIterator<X>::reference>::result_type>
          && HasSwap<InputIterator<X>::reference, InputIterator<X>::reference>;
  }

  // 23.1 (Table 66)
  concept Container<typename X>
    : DefaultConstructible<X>,
      CopyConstructible<X>,
      EqualityComparable<X>,
      LessThanComparable<X>
  {
    requires CopyAssignable<X>, SameType<CopyAssignable<X>::result_type, X&>;

    typename value_type = typename X::value_type;

    // Note: The standard says "lvalue of T", but the term isn't
    // defined. We assume that means that it needs to be convertible to
    // a non-const reference to the value_type.
    typename reference = typename X::reference;
    requires Convertible<reference, value_type&>;

    // Note: The standard says "const lvalue of T", but the term isn't
    // defined. We assume that means that it needs to be convertible to
    // a const reference to the value_type.
    typename const_reference = typename X::const_reference;
    requires Convertible<const_reference, const value_type&>;

    // Note: The standard says "any category except output iterator."
    // This does not mean that the iterator cannot model the
    // OutputIterator concept; it means that we're requiring an
    // InputIterator.
    // Note: The standard says "iterator type" instead of "const
    // iterator type", implying that there may be some mutability
    // here. However, it also states that the output iterator category
    // is not allowed. We interpret this to mean that the true
    // requirement is just InputIterator, but one could equally well
    // argue for both InputIterator and OutputIterator.
    ForwardIterator iterator = typename X::iterator;
    requires SameType<InputIterator<iterator>::value_type, value_type>;

    // Note: The standard says "any category except output iterator."
    // This does not mean that the iterator cannot model the
    // OutputIterator concept; it means that we're requiring an
    // InputIterator.
    ForwardIterator const_iterator = typename X::const_iterator;
    requires SameType<InputIterator<const_iterator>::value_type, value_type>,
            Convertible<iterator, const_iterator>;

    SignedIntegralLike difference_type = typename X::difference_type;
    requires SameType<difference_type, 
                     InputIterator<iterator>::difference_type>,
            SameType<difference_type, 
            InputIterator<const_iterator>::difference_type>;

    // Note: We use sizeof() to try to describe the (partially semantic)
    // requirement that "size_type can represent any non-negative value
    // of difference_type". This may or may not be legitimate.
    UnsignedIntegralLike size_type = typename X::size_type;
    requires UnsignedIntegralLike<size_type>,
            std::True<sizeof(size_type) >= sizeof(difference_type)>;

    iterator       X::begin();
    const_iterator X::begin() const;
    iterator       X::end();
    const_iterator X::end() const;

    void X::swap(X& other);

    size_type X::size() const;
    size_type X::max_size() const;
    bool X::empty() const;
  };

  template<typename X>
  concept_map RandomAccessIterator<const X*>
  {
    typedef X value_type;
    typedef std::ptrdiff_t difference_type;
    typedef const X& reference;
    typedef const X* pointer;
  };

  template<typename X>
  concept_map RandomAccessIterator<X*>
  {
    typedef X value_type;
    typedef std::ptrdiff_t difference_type;
    typedef X& reference;
    typedef X* pointer;
  };

  /**
   * The @c Range concept is used by the compiler to iterate over the
   * values in a container with the C++0x "for" loop. To enable
   * iteration over a new container-like entity, create a new concept
   * map for the Range concept that provides begin and end iterators.
   */
  concept Range<typename X> {
    InputIterator iterator;
    iterator begin(X&);
    iterator end(X&);
  }
  
  /**
   * This concept map enables iteration over all values in an array.
   */
  template<typename T, size_t N>
  concept_map Range<T[N]> {
    typedef T* iterator;
    T* begin(T (&array)[N]) { return array; }
    T* end(T (&array)[N])   { return array + N; }
  }
   
  /**
   * This concept map enables iteration over all values in a container.
   */
  template<Container X>
  concept_map Range<X> {
    typedef X::iterator iterator;
    iterator begin(X& x) { return x.begin(); }
    iterator end(X& x) { return x.end(); }
  }
  
  /**
   * This concept map enables iteration over all values in a constant
   * container.
   */
  template<Container X>
  concept_map Range<const X> {
    typedef X::const_iterator iterator;
    iterator begin(const X& x) { return x.begin(); }
    iterator end(const X& x) { return x.end(); }
  }
}  // namespace std
#endif // ndef _GLIBCXX_NO_CONCEPTS

#endif // _ITERATOR_CONCEPTS_H
