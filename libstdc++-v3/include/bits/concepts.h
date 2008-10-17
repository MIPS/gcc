// Concept definitions -*- C++ -*- .

// Copyright (C) 2005, 2006, 2007, 2008  Free Software Foundation, Inc.
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

/** @file concepts.h
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */
#ifndef _CONCEPTS_H
#define _CONCEPTS_H

#pragma GCC system_header
// So variadic macros won't generate a warning

#include <cstddef>
#include <bits/c++config.h>

// We can't have concepts if they're turned off in the compiler
#if !defined(__GXX_CONCEPTS__) && !defined(_GLIBCXX_NO_CONCEPTS)
#  define _GLIBCXX_NO_CONCEPTS
#endif

#ifdef _GLIBCXX_NO_CONCEPTS

#define _GLIBCXX_WHERE(...)
#define _GLIBCXX_LATE_CHECK
#define _GLIBCXX_REQ_PARM(Req,Name) typename Name
#define _GLIBCXX_PARM_REQ(Name, ...) typename Name
#define _GLIBCXX_ITERATOR_TRAITS(Concept,Name) \
  typename ::std::iterator_traits< Name >
#define _GLIBCXX_ITERATOR_TRAITS_NA(Name,Type) \
  typename ::std::iterator_traits< Name >::Type
#define _GLIBCXX_ITERATOR_TRAITS_NEST(Name,Type) \
  typename ::std::iterator_traits< Name >::Type
#define _GLIBCXX_CONCEPT_ALGO(Name) _GLIBCXX_CONCEPT_ALGO2( __ , Name )
#define _GLIBCXX_CONCEPT_ALGO2(X,Y) _GLIBCXX_CONCEPT_ALGO3(X,Y)
#define _GLIBCXX_CONCEPT_ALGO3(X,Y) X##Y

#else

#include <new> // for std::nothrow_t

#define _GLIBCXX_WHERE(...) requires __VA_ARGS__
#define _GLIBCXX_LATE_CHECK late_check
#define _GLIBCXX_REQ_PARM(Req,Name) Req Name
#define _GLIBCXX_PARM_REQ(Name, ...) __VA_ARGS__ Name
#define _GLIBCXX_ITERATOR_TRAITS(Concept,Name) \
  ::std::Concept< Name >
#define _GLIBCXX_ITERATOR_TRAITS_NA(Name,Type) Type
#define _GLIBCXX_ITERATOR_TRAITS_NEST(Name,Type) Name::Type
#define _GLIBCXX_CONCEPT_ALGO(Name) Name

namespace std
{

  // [concept.support]

  /// @brief Describes types that can be used as the return type of a
  /// function.
  concept Returnable<typename T> { }

  /// @brief Describes types to which a pointer can be formed.
  concept PointeeType<typename T> { }

  /// @brief Describes types to which a pointer-to-member can be formed.
  concept MemberPointeeType<typename T> : PointeeType<T> { }

  /// @brief Describes types to which a reference can be formed.
  concept ReferentType<typename T> { }

  /// @brief Describes types that can be used to declare a variable.
  concept VariableType<typename T> { }

  /// @brief Describes object types.
  concept ObjectType<typename T> : VariableType<T>, MemberPointeeType<T> { }

  /// @brief Describes class types
  concept ClassType<typename T> : ObjectType<T> { }

  /// @brief Describes classes (and structs)
  concept Class<typename T> : ClassType<T> { }

  /// @brief Describes unions
  concept Union<typename T> : ClassType<T> { }

  /// @brief Describes trivial types
  concept TrivialType<typename T> : ObjectType<T> { }

  /// @brief Describes standard layout types
  concept StandardLayoutType<typename T> : ObjectType<T> { }

  /// @brief Describes literal types. 
  concept LiteralType<typename T> : ObjectType<T> { }

  /// @brief Describes scalar types
  concept ScalarType<typename T> 
    : TrivialType<T>, LiteralType<T>, StandardLayoutType<T> { }

  /// @brief Describes types that can be used as the type of a
  /// non-type template parameter.
  concept NonTypeTemplateParameterType<typename T> : VariableType<T> { }

  /// @brief Describes the types that can be the types of an integral
  /// constant expression.
  concept IntegralConstantExpressionType<typename T> 
    : ScalarType<T>, NonTypeTemplateParameterType<T> { }

  /// @brief Describes integral types
  concept IntegralType<typename T> : IntegralConstantExpressionType<T> { }

  /// @brief Describes enumeration types
  concept EnumerationType<typename T> : IntegralConstantExpressionType<T> { }

  /// @brief The SameType concept is a compiler-supported concept that
  /// requires that the two types, T and U, be equivalent.
  concept SameType<typename T, typename U> { }

  /**
   *  @if maint
   *  This concept map is used to match the SameType constraint when
   *  needed.
   *  @endif
  */
  template<typename T> concept_map SameType<T, T> { }

  /// @brief The DerivedFrom concept is a compiler-supported concept
  /// that requires that Derived by publicly and unambiguously derived
  /// from Base.
  concept DerivedFrom<typename Derived, typename Base> { }

  // [concept.true]

  // @brief The True concept is a compiler-supported concept that
  // requires that the integral constant expression it is given
  // evaluate true.
  concept True<bool> { }

  /**
   * @if maint
   * Helper concept map implementing the semantics of concept True
   * @endif
  */
  concept_map True<true> { }

  // [concept.convertible]

  // TODO: Cannot write the ExplicitlyConvertible concept without
  // support for explicit conversion operators

  /// @brief Describes the ability to (implicitly) convert from one
  /// type to another.
  auto concept Convertible<typename T, typename U>
  {
    operator U(const T&);
  }
  
  /// TODO: Helper concept map to be removed
  template<typename T>
  concept_map Convertible<T, T> {}
  
  /// TODO: Helper concept map to be removed
  template<typename T>
  concept_map Convertible<T, T&> { }
  
  /// TODO: Helper concept map to be removed
  template<typename T>
  concept_map Convertible<T, const T&> { }

  // [concept.operator]

  /// @brief Describes types with a binary @c operator+
  auto concept HasPlus<typename T, typename U = T>
  {
    typename result_type;
    result_type operator+(T const& t, U const& u);
  }

  /// @brief Describes types with a binary @c operator-
  auto concept HasMinus<typename T, typename U = T>
  {
    typename result_type;
    result_type operator-(T const& t, U const& u);
  }

  /// @brief Describes types with a binary @c operator*
  auto concept HasMultiply<typename T, typename U = T>
  {
    typename result_type;
    result_type operator*(T const& t, U const& u);
  }

  /// @brief Describes types with a binary @c operator/
  auto concept HasDivide<typename T, typename U = T>
  {
    typename result_type;
    result_type operator/(T const& t, U const& u);
  }

  /// @brief Describes types with a binary @c operator%
  auto concept HasModulus<typename T, typename U = T>
  {
    typename result_type;
    result_type operator%(T const& t, U const& u);
  }

  /// @brief Describes types with a unary @c operator+
  auto concept HasUnaryPlus<typename T>
  {
    typename result_type;
    result_type operator+(T const& t);
  }

  /// @brief Describes types with a unary @c operator-
  auto concept HasNegate<typename T>
  {
    typename result_type;
    result_type operator-(T const& t);
  }
  
  /// @brief Describes types with a < operator.
  auto concept HasLess<typename T, typename U> {
    bool operator<(const T&, const U&);
  }

  /// @brief Describes types with a > operator.
  auto concept HasGreater<typename T, typename U> {
    bool operator>(const T&, const U&);
  }

  /// @brief Describes types with a <= operator.
  auto concept HasLessEqual<typename T, typename U> {
    bool operator<=(const T&, const U&);
  }

  /// @brief Describes types with a >= operator.
  auto concept HasGreaterEqual<typename T, typename U> {
    bool operator>=(const T&, const U&);
  }

  /// @brief Describes types with a == operator.
  auto concept HasEqualTo<typename T, typename U> {
    bool operator==(const T&, const U&);
  }

  /// @brief Describes types with a != operator.
  auto concept HasNotEqualTo<typename T, typename U> {
    bool operator!=(const T&, const U&);
  }

  /// @brief Describes types with a binary @c operator&&
  auto concept HasLogicalAnd<typename T, typename U = T>
  {
    bool operator&&(T const& t, U const& u);
  }

  /// @brief Describes types with a binary @c operator||
  auto concept HasLogicalOr<typename T, typename U = T>
  {
    bool operator||(T const& t, U const& u);
  }

  /// @brief Describes types with a unary @c operator!
  auto concept HasLogicalNot<typename T>
  {
    bool operator!(T const& t);
  }

  /// @brief Describes types with a binary @c operator&
  auto concept HasBitAnd<typename T, typename U = T>
  {
    typename result_type;
    result_type operator&(T const& t, U const& u);
  }

  /// @brief Describes types with a binary @c operator|
  auto concept HasBitOr<typename T, typename U = T>
  {
    typename result_type;
    result_type operator|(T const& t, U const& u);
  }

  /// @brief Describes types with a binary @c operator^
  auto concept HasBitXor<typename T, typename U = T>
  {
    typename result_type;
    result_type operator^(T const& t, U const& u);
  }
  
  /// @brief Describes types with a unary @c operator~
  auto concept HasComplement<typename T>
  {
    typename result_type;
    result_type operator~(T const& t);
  }

  /// @brief Describes types with a binary @c operator<<
  auto concept HasLeftShift<typename T, typename U = T>
  {
    typename result_type;
    result_type operator<<(T const& t, U const& u);
  }

  /// @brief Describes types with a binary @c operator<<
  auto concept HasRightShift<typename T, typename U = T>
  {
    typename result_type;
    result_type operator>>(T const& t, U const& u);
  }
  
  /// @brief Describes types that have a unary @c operator*
  auto concept Dereferenceable<typename T>
  {
    typename reference;
    reference operator*(T);
  };


  /// @brief Describes types that have an address-of operator
  auto concept Addressable<typename T> 
  {
    typename pointer;
    typename const_pointer;

    pointer operator&(T&);
    const_pointer operator&(T const&);
  }

  // TODO: cannot express Callable, which is variadic, so we settle
  // for Callable0, Callable1, etc.

  auto concept Callable0<typename F> {
    typename result_type;
    result_type operator()(F&);
  };
  
  auto concept Callable1<typename F, typename X> {
    typename result_type;
    result_type operator()(F&, X);
  };
  
  auto concept Callable2<typename F, typename X, typename Y> {
    typename result_type;
    result_type operator()(F&, X, Y);
  };

  auto concept HasAssign<typename T, typename U> {
    typename result_type;
    result_type T::operator=(U);
  }

  auto concept HasPlusAssign<typename T, typename U> {
    typename result_type;
    result_type operator+=(T&, U);
  }

  auto concept HasMinusAssign<typename T, typename U> {
    typename result_type;
    result_type operator-=(T&, U);
  }

  auto concept HasMultiplyAssign<typename T, typename U> {
    typename result_type;
    result_type operator*=(T&, U);
  }

  auto concept HasDivideAssign<typename T, typename U> {
    typename result_type;
    result_type operator/=(T&, U);
  }

  auto concept HasModulusAssign<typename T, typename U> {
    typename result_type;
    result_type operator%=(T&, U);
  }

  auto concept HasBitAndAssign<typename T, typename U> {
    typename result_type;
    result_type operator&=(T&, U);
  }

  auto concept HasBitOrAssign<typename T, typename U> {
    typename result_type;
    result_type operator|=(T&, U);
  }

  auto concept HasBitXorAssign<typename T, typename U> {
    typename result_type;
    result_type operator^=(T&, U);
  }

  auto concept HasLeftShiftAssign<typename T, typename U> {
    typename result_type;
    result_type operator<<=(T&, U);
  }

  auto concept HasRightShiftAssign<typename T, typename U> {
    typename result_type;
    result_type operator>>=(T&, U);
  }

  auto concept HasPreincrement<typename T> {
    typename result_type;
    result_type operator++(T&);
  }

  auto concept HasPostincrement<typename T> {
    typename result_type;
    result_type operator++(T&, int);
  }

  auto concept HasPredecrement<typename T> {
    typename result_type;
    result_type operator--(T&);
  }

  auto concept HasPostdecrement<typename T> {
    typename result_type;
    result_type operator--(T&, int);
  }

  auto concept HasComma<typename T, typename U> {
    typename result_type;
    result_type operator,(const T&, const U&);
  }

  // [concept.predicate]

  // TODO: since we can't express the variadic Predicate, we just use
  // Predicate and BinaryPredicate for now

  auto concept Predicate<typename F, typename T> : Callable1<F, const T&>
  {
    requires Convertible<result_type, bool>;
  }

  auto concept BinaryPredicate<typename F, typename T, typename U>
    : Callable2<F, const T&, const U&>
  {
    requires Convertible<result_type, bool>;
  }

  // [concept.comparison]

  /// @brief Describes an ordering between two objects (potentially of
  /// different types).
  auto concept LessThanComparable<typename T> : HasLess<T, T> {
    bool operator>(const T& a, const T& b) { return b < a; }
    bool operator<=(const T& a, const T& b) { return !(b < a); }
    bool operator>=(const T& a, const T& b) { return !(a < b); }

    axiom Consistency(T a, T b) { 
      (a > b) == (b < a); 
      (a <= b) == !(b < a); 
      (a >= b) == !(a < b); 
    } 

    axiom Irreflexivity(T a) { (a < a) == false; } 

    axiom Antisymmetry(T a, T b) { 
      if (a < b) 
        (b < a) == false; 
    } 

    axiom Transitivity(T a, T b, T c) { 
      if (a < b && b < c) 
        (a < c) == true; 
    } 

    axiom TransitivityOfEquivalence(T a, T b, T c) {
      if (!(a < b) && !(b < a) && !(b < c) && !(c < b)) 
        (!(a < c) && !(c < a)) == true; 
    }
  }

  auto concept StrictWeakOrder<typename F, typename T> 
    : BinaryPredicate<F, T, T> { 
    axiom Irreflexivity(F f, T a) { f(a, a) == false; } 

    axiom Antisymmetry(F f, T a, T b) { 
      if (f(a, b)) 
        f(b, a) == false; 
    }
 
    axiom Transitivity(F f, T a, T b, T c) { 
      if (f(a, b) && f(b, c)) 
        f(a, c) == true; 
    }
 
    axiom TransitivityOfEquivalence(F f, T a, T b, T c) { 
      if (!f(a, b) && !f(b, a) && !f(b, c) && !f(c, b)) 
        (!f(a, c) && !f(c, a)) == true; 
    } 
  } 

  /// @brief Describes comparison of two objects for equality, which
  /// may have different types.
  auto concept EqualityComparable<typename T, typename U = T>
  {
    bool operator==(const T& t, const U& u);
    bool operator!=(const T& t, const U& u) { return !(t == u); }
  }
  
  concept TriviallyEqualityComparable<typename T> : EqualityComparable<T> { }

  // [concept.destruct]

  /// @brief Describes types that can be destroyed
  // FIXME: Need support for VariableType from the compiler
  auto concept Destructible<typename T> /*: VariableType<T>*/ { 
    T::~T();
  }

  auto concept NothrowDestructible<typename T> : Destructible<T> { }

  concept TriviallyDestructible<typename T> : NothrowDestructible<T> { }
  
  // [concept.construct]

  // TODO: Can't express the variadic HasConstructor concept just yet,
  // so instead we use HasConstructor0, HasConstructor1, etc.
  auto concept HasConstructor0<typename T> { 
    T::T();
  }

  auto concept HasConstructor1<typename T, typename T1> { 
    T::T(T1);
  }

  auto concept HasConstructor2<typename T, typename T1, typename T2> { 
    T::T(T1, T2);
  }


  /// @brief Describes types that can be default-constructed
  auto concept DefaultConstructible<typename T> : HasConstructor0<T> { }
  
  concept TriviallyDefaultConstructible<typename T> : DefaultConstructible<T> {}

  // [concept.copymove]
  
  /// @brief Describes types that can be move-constructed
  auto concept MoveConstructible<typename T> : HasConstructor1<T, T&&> { }

  /// @brief Describes types that can be copy-constructed

  /// TODO: Should refine MoveConstructible, but there's a problem
  /// with the move-constructibility of references. Also, should
  /// refine HasConstructor1<T, const T&>, but we're running into an
  /// ambiguity.
  auto concept CopyConstructible<typename T>
  {
    T::T(const T&);
  }

  /// @brief Describes types that are trivially copy constructible
  concept TriviallyCopyConstructible<typename T> : CopyConstructible<T> { }

  /// @brief Describes types that are move-assignable
  auto concept MoveAssignable<typename T, typename U = T> {
    typename result_type;
    result_type T::operator=(U&&);
  }

  /// @brief Describes types that are copy-assignable
  auto concept CopyAssignable<typename T, typename U = T> : MoveAssignable<T, U>
  {
    typename result_type;
    result_type T::operator=(const U&);
  }

  /// @brief Describes types that are trivially copy-assignable,
  /// meaning that they can be copied with @c memcpy.
  concept TriviallyCopyAssignable<typename T> : CopyAssignable<T> { }

  auto concept HasSwap<typename T, typename U> {
    void swap(T, U);
  }

  /// @brief Describes types that are swappable
  auto concept Swappable<typename T> : HasSwap<T&, T&> { }
  
  // [concept.memory]
  auto concept HasPlacementNew<typename T> { 
    void* T::operator new(size_t size, void*); 
  }

  auto concept FreeStoreAllocatable<typename T> {
    void* T::operator new(size_t size);
    void  T::operator delete(void*);

    void* T::operator new(size_t size, const nothrow_t&) {
      try {
        return T::operator new(size);
      } catch(...) {
        return 0;
      }
    }

    void T::operator delete(void* ptr, const nothrow_t&)
    {
      T::operator delete(ptr);
    }

    void* T::operator new[](size_t size)
    {
      return T::operator new(size);
    }

    void T::operator delete[](void* ptr)
    {
      T::operator delete(ptr);
    }

    void* T::operator new[](size_t size, const nothrow_t&) {
      try {
          return T::operator new[](size);
      } catch(...) {
        return 0;
      }
    }

    void T::operator delete[](void* ptr, const nothrow_t&) {
      T::operator delete[](ptr);
    }
  }

  // [concept.regular]

  /// @brief Describes a semi-regular type, which can be copied and
  /// assigned.
  /// TODO: the CopyAssignable requirement should be a refinement, but
  /// that ends up breaking our concept maps for
  /// RandomAccessIterator<const T*> and
  /// RandomAccessIterator<T*>, due to problems with deduction
  /// of associated types.
  auto concept Semiregular<typename T> 
    : NothrowDestructible<T>, CopyConstructible<T>, FreeStoreAllocatable<T>
  { 
    requires CopyAssignable<T> && SameType<CopyAssignable<T>::result_type, T&>;
  }
  
  /// @brief Describes a regular type.
  auto concept Regular<typename T>
    : Semiregular<T>, DefaultConstructible<T>, EqualityComparable<T>
  {
  }
  
  // [concept.arithmetic]

  /// @brief Describes types that provide all of the operators
  /// available on arithmetic types.
  concept ArithmeticLike<typename T> : Regular<T>
  {
    // TODO: These should be refinements, but ConceptGCC handles
    // associated types in refinements in the wrong way (by combining
    // them).
    requires HasUnaryPlus<T> && HasNegate<T> && 
      HasPlus<T, T> && HasMinus<T, T> && HasMultiply<T, T> && HasDivide<T, T> && 
      HasLess<T, T> && HasGreater<T, T> && HasLessEqual<T, T> && HasGreaterEqual<T, T> && 
      HasPreincrement<T> && HasPostincrement<T> && HasPredecrement<T> && HasPostdecrement<T> && 
      HasPlusAssign<T, const T&> && HasMinusAssign<T, const T&> && 
      HasMultiplyAssign<T, const T&> && HasDivideAssign<T, const T&>;

    T::T(long long);

    // TODO: What's going on with these? It causes ambiguities...
    //    T::T(unsigned long long);
    //    T::T(long double);

    requires Convertible<HasUnaryPlus<T>::result_type, T>
      && Convertible<HasNegate<T>::result_type, T>
      && Convertible<HasPlus<T>::result_type, T>
      && Convertible<HasMinus<T>::result_type, T>
      && Convertible<HasMultiply<T>::result_type, T>
      && Convertible<HasDivide<T>::result_type, T>
      && SameType<HasPreincrement<T>::result_type, T&>
      && SameType<HasPostincrement<T>::result_type, T>
      && SameType<HasPredecrement<T>::result_type, T&>
      && SameType<HasPostdecrement<T>::result_type, T>
      && SameType<HasPlusAssign<T, const T&>::result_type, T&>
      && SameType<HasMinusAssign<T, const T&>::result_type, T&>
      && SameType<HasMultiplyAssign<T, const T&>::result_type, T&>
      && SameType<HasDivideAssign<T, const T&>::result_type, T&>;
  }
  

  /// @brief Describes types that act like integral types
  concept IntegralLike<typename T> : ArithmeticLike<T>
  {
    // TODO: these should be refinements
    requires HasComplement<T> && HasModulus<T, T> 
      && HasBitAnd<T, T> && HasBitXor<T, T> && HasBitOr<T, T> 
      && HasLeftShift<T, T> && HasRightShift<T, T> 
      && HasModulusAssign<T, const T&> && HasLeftShiftAssign<T, const T&> && HasRightShiftAssign<T, const T&> 
      && HasBitAndAssign<T, const T&> && HasBitXorAssign<T, const T&> && HasBitOrAssign<T, const T&>;

    requires Convertible<HasComplement<T>::result_type, T>
      && Convertible<HasModulus<T>::result_type, T>
      && Convertible<HasBitAnd<T>::result_type, T>
      && Convertible<HasBitOr<T>::result_type, T>
      && Convertible<HasBitXor<T>::result_type, T>
      && Convertible<HasLeftShift<T>::result_type, T>
      && Convertible<HasRightShift<T>::result_type, T>
      && SameType<HasModulusAssign<T, const T&>::result_type, T&>
      && SameType<HasLeftShiftAssign<T, const T&>::result_type, T&>
      && SameType<HasRightShiftAssign<T, const T&>::result_type, T&>
      && SameType<HasBitAndAssign<T, const T&>::result_type, T&>
      && SameType<HasBitXorAssign<T, const T&>::result_type, T&>
      && SameType<HasBitOrAssign<T, const T&>::result_type, T&>; 
  }

  /// @brief Describes types that act like signed integral types
  concept SignedIntegralLike<typename T> : IntegralLike<T> { }
  
  /// @brief Describes types that act like unsigned integral types
  concept UnsignedIntegralLike<typename T> : IntegralLike<T> { }
  
  concept_map IntegralLike<char> {};
  concept_map SignedIntegralLike<signed char> {};
  concept_map UnsignedIntegralLike<unsigned char> {};
#ifdef _GLIBCXX_USE_WCHAR_T
  concept_map IntegralLike<wchar_t> {};
#endif
  concept_map SignedIntegralLike<short> {};
  concept_map UnsignedIntegralLike<unsigned short> {};
  concept_map SignedIntegralLike<int> {};
  concept_map UnsignedIntegralLike<unsigned int> {};
  concept_map SignedIntegralLike<long> {};
  concept_map UnsignedIntegralLike<unsigned long> {};
  concept_map SignedIntegralLike<long long> {};
  concept_map UnsignedIntegralLike<unsigned long long> {};

  /// @brief Describes floating-point types
  concept FloatingPointLike<typename T> : ArithmeticLike<T> { }

  concept_map FloatingPointLike<float> { }
  concept_map FloatingPointLike<double> { }
  concept_map FloatingPointLike<long double> { }

  template<typename _Tp>
  struct __remove_reference {
    typedef _Tp type;
  };

  template<typename _Tp>
  struct __remove_reference<_Tp&> {
    typedef _Tp type;
  };

  template<typename _Tp>
  struct __remove_reference<_Tp&&> {
    typedef _Tp type;
  };

  auto concept HasStdMove<typename T> {
    typename result_type = typename __remove_reference<T>::type&&;
    result_type std_move(T&& __x) { return __x; }
  }
}
#endif // ndef _GLIBCXX_NO_CONCEPTS

#endif // _CONCEPTS_H
