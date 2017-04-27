/* Polynomial integer classes.
   Copyright (C) 2014-2016 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* This file provides a representation of sizes and offsets whose exact
   values depend on certain runtime properties.  The motivating example
   is the ARM SVE ISA, in which the number of vector elements is only
   known at runtime.

   Overview
   ========

   We define indeterminates x1...xn whose values are only known at
   runtime and use polynomials of the form:

     c0 + c1 * x1 + ... + cn * xn

   to represent a size or offset whose value might depend on one
   of these indeterminates.  The coefficients c0...cn are always
   known at compile time, with the c0 term being the "constant" part
   that doesn't depend on any runtime value.

   The number of indeterminates n is a fixed property of the target
   and at the moment is usually 0 (since most targets don't have such
   runtime values).  When n is 0, the class degenerates to a single
   compile-time constant c0.

   We make the simplifying requirement that each indeterminate must be a
   nonnegative integer.  An indeterminate value of 0 should usually
   represent the minimum possible runtime value, with c0 indicating
   the value in that case.

   E.g., for SVE, the single indeterminate represents the number of
   128-bit blocks in a vector _beyond the minimum width of 128 bits_.
   Thus the number of 64-bit doublewords in a vector is 2 + 2 * x1.
   If an aggregate has a single SVE vector and 16 additional bytes,
   its total size is 32 + 16 * x1 bytes.

   Challenges
   ==========

   The two main problems with using polynomial sizes and offsets are that:

   (1) there's no total ordering at compile time, and
   (2) some operations have results that can't be expressed as a compile-time
       polynomial.

   For example, we can't tell at compile time whether:

      3 + 4x <= 1 + 5x

   since the condition is false for x <= 1 and true for x >= 2.
   Similarly we can't compute:

      (3 + 4x) * (1 + 5x)
   or
      (3 + 4x) / (1 + 5x)

   and represent the result as the same kind of polynomial.  These problems
   are addressed below.

   Ordering
   ========

   In general we need to compare sizes and offsets in two situations:
   those in which there is no definite link between the two values,
   and those in which there is a definite link.  An example of the
   former is bounds checking: we might want to check whether two
   arbitrary memory references overlap.  An example of the latter is the
   relationship between the inner and outer sizes of a subreg, where we
   must know at compile time whether the subreg is paradoxical, partial,
   or complete.

   Referring back to the example polynomials above, it makes sense to
   ask whether a memory reference of size 3 + 4x overlaps one of size
   1 + 5x, but it doesn't make sense to have a subreg in which the outer
   mode has 3 + 4x bytes and the inner mode has 1 + 5x bytes (or vice
   versa).  Such subregs are always invalid and should trigger an ICE
   if formed.

   Ordering without a definite link
   ================================

   In the case where there is no definite link between two sizes,
   we can usually make a conservatively-correct assumption.  E.g.
   for alias analysis the conservative assumption is that two references
   might alias.  For these situations the file provides routines for
   checking whether a particular relationship "may" (= might) hold:

      may_lt may_le may_eq may_ge may_gt
                    may_ne

   All relations on the first line are transitive, so for example:

      may_lt (a, b) && may_lt (b, c) implies may_lt (a, c)

   may_lt, may_gt and may_ne are irreflexive, so for example:

      !may_lt (a, a)

   is always true.  may_le, may_eq and may_ge are reflexive, so for example:

      may_le (a, a)

   is always true.  may_eq and may_ne are symmetric, so:

      may_eq (a, b) == may_eq (b, a)
      may_ne (a, b) == may_ne (b, a)

   In addition:

      may_le (a, b) == may_lt (a, b) || may_eq (a, b)
      may_ge (a, b) == may_gt (a, b) || may_eq (a, b)
      may_lt (a, b) == may_gt (b, a)
      may_le (a, b) == may_ge (b, a)

   However:

      may_le (a, b) && may_le (b, a) does not imply !may_ne (a, b)
      may_ge (a, b) && may_ge (b, a) does not imply !may_ne (a, b)

   One example where this doesn't hold is again a == 3 + 4x and
   b == 1 + 5x, where may_le (a, b), may_ge (a,b) and may_ne (a, b)
   all hold.  may_le and may_ge are therefore not antisymetric and so
   don't form a partial order.

   One way of checking whether [begin1, end1) might overlap [begin2, end2)
   using these relations is:

      may_gt (end1, begin2) && may_gt (end2, begin1)

   (but see the description of the range-checking predicates below).

   For readability, the file also provides "must" inverses of the above:

      must_lt (a, b) == !may_ge (a, b)
      must_le (a, b) == !may_gt (a, b)
      must_eq (a, b) == !may_ne (a, b)
      must_ge (a, b) == !may_lt (a, b)
      must_gt (a, b) == !may_le (a, b)
      must_ne (a, b) == !may_eq (a, b)

   An alternative way of writing the range check above is therefore:

      !(must_le (end1, begin2) || must_le (end2, begin1))

   All "must" relations except "must_ne" are transitive.  must_lt,
   must_ne and must_gt are irreflexive.  must_le, must_eq and must_ge
   are reflexive.  Also:

      must_lt (a, b) == must_gt (b, a)
      must_le (a, b) == must_ge (b, a)
      must_lt (a, b) implies !must_lt (b, a)   [asymmetry]
      must_gt (a, b) implies !must_gt (b, a)
      must_le (a, b) && must_le (b, a) == must_eq (a, b) [== !may_ne (a, b)]
      must_ge (a, b) && must_ge (b, a) == must_eq (a, b) [== !may_ne (a, b)]

   must_le and must_ge are therefore antisymmetric and are partial orders.
   However:

      must_le (a, b) does not imply must_lt (a, b) || must_eq (a, b)
      must_ge (a, b) does not imply must_gt (a, b) || must_eq (a, b)

   For example, must_le (4, 4 + 4x) holds due the requirements on x,
   but neither must_lt (4, 4 + 4x) nor must_eq (4, 4 + 4x) hold.

   Ordering with a definite link
   =============================

   In cases where there is a definite link between values, such as the
   outer and inner sizes of subregs, we usually require the sizes to be
   ordered by the must_le partial order.  ordered_p (a, b) checks
   whether this is true for a given a and b.

   For example, if a subreg has an outer mode of size OUTER and an
   inner mode of size INNER:

   - the subreg is complete if must_eq (INNER, OUTER)
   - otherwise, the subreg is paradoxical if must_le (INNER, OUTER)
   - otherwise, the subreg is partial if must_le (OUTER, INNER)
   - otherwise, the subreg is ill-formed

   If the sizes are already known to be valid then:

   - the subreg is complete if must_eq (INNER, OUTER)
   - the subreg is paradoxical if may_lt (INNER, OUTER)
   - rhe subreg is partial if may_lt (OUTER, INNER)

   The file also provides the following utility functions for
   ordered values:

   ordered_min (a, b)
      Asserts that a and b are ordered by must_le and returns the
      minimum of the two.

   ordered_max (a, b)
      Asserts that a and b are ordered by must_le and returns the
      maximum of the two.

   These routines should only be used if there is a definite link
   between the two values.  See the bounds routines below for routines
   that can help with other cases.

   Marker values
   =============

   Sometimes it's useful to have a special marker value that isn't
   supposed to be taken literally.  For example, some code uses a size
   of -1 to represent an unknown size, rather than having to carry around
   a separate boolean to say whether the size is known.

   The best way of checking whether something is a marker value
   is must_eq.  Conversely the best way of checking whether something
   _isn't_ a marker value is may_ne.

   Thus in the size example just mentioned, must_eq (size, -1) would
   check for an unknown size and may_ne (size, -1) would check for a
   known size.

   Range checks
   ============

   As well as the core comparisons described above, the file provides
   utilities for various kinds of range check.  In each case the range
   is represented by a start position and a length rather than a start
   position and an end position.  (This is because the former is used
   much more often than the latter in GCC.)  Also, the sizes can be
   -1 (or all-1s for unsigned sizes) to indicate a range with a known
   start position but an unknown size.

   ranges_may_overlap_p (pos1, size1, pos2, size2)
      Return true if the range described by pos1 and size1 might overlap
      the range described by pos2 and size2.

   known_subrange_p (pos1, size1, pos2, size2)
      Return true if the range described by pos1 and size1 is known to
      be contained in the range described by pos2 and size2.

   known_in_range_p (val, pos, size)
      Return true if value val is known to be in the range described
      by pos and size.

   maybe_in_range_p (val, pos, size)
      Return true if value val might be in the range described by pos
      and size (i.e. if it isn't known to be outside that range).

   Arithmetic
   ==========

   Addition, subtraction, negation and bit inverse work normally.
   Multiplication by a constant multiplier and left shifting by
   a constant shift amount also work normally.  General multiplication
   of two polynomials isn't supported and isn't useful in practice.

   These arithmetic operators handle integer ranks in a similar way to C.
   The main difference is that everything narrower than HOST_WIDE_INT
   promotes to HOST_WIDE_INT, whereas in C everything narrower than int
   promotes to int.  For example:

       poly_uint16     + int          -> poly_int64  (both narrower than HWI)
       unsigned int    + poly_uint16  -> poly_int64  (likewise)
       poly_int64      + int          -> poly_int64  (highest rank wins)
       poly_int32      + poly_uint64  -> poly_uint64 (likewise)
       uint64          + poly_int64   -> poly_uint64 (likewise)
       poly_offset_int + int32        -> poly_offset_int (likewise)
       offset_int      + poly_uint16  -> poly_offset_int (likewise)

   If one of the operands is wide_int or poly_wide_int, the rules
   are the same as for wide_int arithmetic.

   Division of polynomials is possible for certain inputs.  The functions
   for division return true if the operation is possible and in most cases
   return the results by pointer.  The routines are:

   multiple_p (a, b[, &quotient])
      Return true if a is an exact multiple of b, storing the result
      in quotient if so.  There are overloads for various combinations
      of polynomial and constant a, b and quotient.

   constant_multiple_p (a, b[, &quotient])
      Test multiple_p and also test whether the multiple is a
      compile-time constant.

   can_div_trunc_p (a, b, &quotient[, &remainder])
      Return true if we can calculate trunc (a / b) at compile time,
      storing the result in quotient and remainder if so.

   can_div_away_from_zero_p (a, b, &quotient)
      Return true if we can calculate a / b at compile time,
      rounding away from zero.  Store the result in quotient if so.

      Note that this is possible iff can_div_trunc_p is possible.
      The only difference is in the way the result is rounded.

   The file also provides an asserting form of division:

   exact_div (a, b)
      Assert that a is a multiple of b and return a / b.  The result
      is a polynomial if a is a polynomial.

   The file provides similar routines for other operations:

   can_ior_p (a, b, &result):
      Return true if we can calculate a | b at compile time, storing the
      result in the given location if so.

   Please add any others that you find to be useful.

   In addition, the following overflow-checking wi:: routines are also
   supported for polynomials:

   - wi::add
   - wi::sub
   - wi::neg
   - wi::mul

   These routines just check whether overflow occurs on any individual
   coefficient; it isn't possible to know at compile time whether the
   final runtime value would overflow.

   The following miscellaneous wi:: routines are also supported:

   - wi::sext

   Alignment
   =========

   The file provides various routines for aligning values and for querying
   misalignments.  In each case the alignment must be a power of 2.

   can_align_p (value, align)
      Return true if we can align the given value to the given alignment
      boundary at compile time.

   can_align_up (value, align, &aligned)
      Return true if we can align the given value upwards at compile time,
      storing the result in aligned if so.

   can_align_down (value, align, &aligned)
      Return true if we can align the given value downwards at compile time,
      storing the result in aligned if so.

   known_equal_after_align_up (a, b, align)
      Return true if we can align a and b up at compile time and if the
      two results are equal.

   known_equal_after_align_down (a, b, align)
      Return true if we can align a and b down at compile time and if the
      two results are equal.

   aligned_lower_bound (value, align)
      Return a result that is no greater than value and that is aligned
      to the given alignment boundary.  The result will the closest
      aligned value for some indeterminate values but not necessarily
      for all.

      For example, this function can be used to calculate a new stack
      offset for a downward-growing stack after allocating an object
      with value bytes that needs to be aligned to align bytes.

   aligned_upper_bound (value, align)
      Likewise return a result that is no less than value and that is
      aligned to the given alignment boundary.  This is the routine
      that would be used for upward-growing stacks in the scenario
      described.

   known_misalignment (value, align, &misalign)
      Return true if we can calculate the misalignment of value
      with respect to align at compile time, storing the result in
      misalign if so.

   known_alignment (value)
      Return the minimum alignment that the given value is known to have
      (in other words, the largest alignment that can be guaranteed
      whatever the values of the indeterminates turn out to be).
      Return 0 if value is known to be 0.

   force_align_up (value, align)
      Assert that value can be aligned up at compile time and return the
      result.  When using this routine, please add a comment explaining
      why the assertion is known to hold.

   force_align_down (value, align)
      As above, but aligning down.

   force_align_up_and_div (value, align)
      Divide the result of force_align_up by align.  Again, please add
      a comment explaining why the assertion in force_align_up is known
      to hold.

   force_align_down_and_div (value, align)
      Likewise for force_align_down.

   force_get_misalignment (value, align)
      Assert that we can calculate the misalignment of value with
      respect to align at compile time and return the misalignment.
      When using this function, please add a comment explaining why
      the assertion is known to hold.

   Computing bounds
   ================

   The file also provides routines for calculating lower and upper bounds:

   constant_lower_bound (a)
      Assert that a is nonnegative and return the smallest value it can have.

   upper_bound (a, b)
      Return a value that must be greater than or equal to both a and b.
      It will be the least such value for some indeterminate values
      but necessarily for all.

   Other utilities
   ===============

   common_multiple (a, b)
      Return a value that is a multiple of both a and b.  It will be
      the least common multiple for some indeterminate values but not
      necessarily for all.

   compare_sizes_for_sort (a, b)
      Compare a and b in reverse lexicographical order.  This can be
      useful when sorting data structures, since it has the effect of
      separating constant and non-constant values.  The values do not
      necessarily end up in numerical order; for example, 1 + 1x
      would come after 100 in the sort order, but may well be less
      than 100 at run time.

   print_dec (value, file, sgn)
      A polynomial version of the wide-int routine.  */

#ifndef HAVE_POLY_INT_H
#define HAVE_POLY_INT_H

/* int_traits<T1>::rank is less than int_traits<T2>::rank if T1 can
   promote to T2.

   For C-like types the rank is:

     (2 * number of bytes) + (unsigned ? 1 : 0)

   wide_ints don't have a normal rank and so use a value of INT_MAX.
   Any fixed-width integer should be promoted to wide_int if possible
   and lead to an error otherwise.

   int_traits<T>::precision is the number of bits that T can hold.

   int_traits<T>::signedness is:
      0 if T1 is unsigned
      1 if T1 is signed
     -1 if T1 has no inherent sign (as for wide_int).

   int_traits<T>::result is a type that can hold results of operations
   on T.  This is different from T itself in cases where T is the result
   of an accessor like wi::to_offset.  */
template<typename T, wi::precision_type = wi::int_traits<T>::precision_type>
struct int_traits;

template<typename T>
struct int_traits<T, wi::FLEXIBLE_PRECISION>
{
  typedef T result;
  static const int signedness = (T (0) >= T (-1));
  static const int precision = sizeof (T) * CHAR_BIT;
  static const int rank = sizeof (T) * 2 + !signedness;
};

template<typename T>
struct int_traits<T, wi::VAR_PRECISION>
{
  typedef T result;
  static const int signedness = -1;
  static const int precision = WIDE_INT_MAX_PRECISION;
  static const int rank = INT_MAX;
};

template<typename T>
struct int_traits<T, wi::CONST_PRECISION>
{
  typedef WI_UNARY_RESULT (T) result;
  static const int signedness = 1;
  static const int precision = wi::int_traits<T>::precision;
  /* These types are always signed.  */
  static const int rank = precision * 2 / CHAR_BIT;
};

/* SFINAE class that leads to substitution failure if T2 can't represent
   all the values in T1.  Either:

   - T2 should be a type with the same signedness as T1 and no less precision.
     This allows things like int16_t -> int16_t and uint32_t -> uint64_t.

   - T1 should be unsigned, T2 should be signed, and T1 should be
     narrower than T2.  This allows things like uint16_t -> int32_t.

   This rules out cases where T2 has less precision than T1 or where
   the conversion would reinterpret the top bit.  E.g. int16_t -> uint32_t
   can be dangerous and should have an explicit cast if deliberate.  */
template<typename T1, typename T2,
	  bool good = (int_traits<T1>::signedness
		       == int_traits<T2>::signedness
		       ? (int_traits<T1>::precision
			  <= int_traits<T2>::precision)
		       : (int_traits<T1>::signedness == 0
			  && int_traits<T2>::signedness == 1
			  && (int_traits<T1>::precision
			      < int_traits<T2>::precision)))>
struct if_lossless;

template<typename T1, typename T2>
struct if_lossless<T1, T2, true>
{
  typedef bool bool_type;
};

/* A base POD class for polynomial integers.  The polynomial has N
   coefficients of type C.

   Most of these functions are ALWAYS_INLINE to speed up compilers
   built at -O0.  The functions are heavily used and not interesting
   as function calls even in debug builds.  */
template<unsigned int N, typename C>
class poly_int_pod
{
public:
  typedef C t;

  template<typename Ca>
  poly_int_pod &operator = (const poly_int_pod<N, Ca> &);
  poly_int_pod &operator = (const C &);

  template<typename Ca>
  poly_int_pod &operator += (const poly_int_pod<N, Ca> &);
  poly_int_pod &operator += (const C &);

  template<typename Ca>
  poly_int_pod &operator -= (const poly_int_pod<N, Ca> &);
  poly_int_pod &operator -= (const C &);

  poly_int_pod &operator *= (const C &);

  poly_int_pod &operator <<= (unsigned int);

  bool is_constant () const;

  template<typename T>
  typename if_lossless<C, T>::bool_type is_constant (T *) const;

  C to_constant () const;

  template<typename Ca>
  static poly_int_pod from (const poly_int_pod<N, Ca> &,
			    unsigned int, signop);
  template<typename Ca>
  static poly_int_pod from (const poly_int_pod<N, Ca> &, signop);
  bool to_shwi (poly_int_pod<N, HOST_WIDE_INT> *) const;
  bool to_uhwi (poly_int_pod<N, unsigned HOST_WIDE_INT> *) const;
  poly_int_pod<N, HOST_WIDE_INT> force_shwi () const;

#if POLY_INT_CONVERSION
  operator C () const;
#endif

  C coeffs[N];
};

template<unsigned int N, typename C>
template<typename Ca>
ALWAYS_INLINE poly_int_pod<N, C>&
poly_int_pod<N, C>::operator = (const poly_int_pod<N, Ca> &a)
{
  STATIC_ASSERT (N <= 2);
  this->coeffs[0] = a.coeffs[0];
  if (N == 2)
    this->coeffs[1] = a.coeffs[1];
  return *this;
}

template<unsigned int N, typename C>
ALWAYS_INLINE poly_int_pod<N, C>&
poly_int_pod<N, C>::operator = (const C &a)
{
  STATIC_ASSERT (N <= 2);
  this->coeffs[0] = a;
  if (N == 2)
    /* Easy way of propagating the precision of a wide_int to the
       second coefficient.  */
    this->coeffs[1] = this->coeffs[0] & 0;
  return *this;
}

template<unsigned int N, typename C>
template<typename Ca>
ALWAYS_INLINE poly_int_pod<N, C>&
poly_int_pod<N, C>::operator += (const poly_int_pod<N, Ca> &a)
{
  STATIC_ASSERT (N <= 2);
  this->coeffs[0] += a.coeffs[0];
  if (N == 2)
    this->coeffs[1] += a.coeffs[1];
  return *this;
}

template<unsigned int N, typename C>
ALWAYS_INLINE poly_int_pod<N, C>&
poly_int_pod<N, C>::operator += (const C &a)
{
  this->coeffs[0] += a;
  return *this;
}

template<unsigned int N, typename C>
template<typename Ca>
ALWAYS_INLINE poly_int_pod<N, C>&
poly_int_pod<N, C>::operator -= (const poly_int_pod<N, Ca> &a)
{
  STATIC_ASSERT (N <= 2);
  this->coeffs[0] -= a.coeffs[0];
  if (N == 2)
    this->coeffs[1] -= a.coeffs[1];
  return *this;
}

template<unsigned int N, typename C>
ALWAYS_INLINE poly_int_pod<N, C>&
poly_int_pod<N, C>::operator -= (const C &a)
{
  this->coeffs[0] -= a;
  return *this;
}

template<unsigned int N, typename C>
ALWAYS_INLINE poly_int_pod<N, C>&
poly_int_pod<N, C>::operator *= (const C &a)
{
  STATIC_ASSERT (N <= 2);
  this->coeffs[0] *= a;
  if (N == 2)
    this->coeffs[1] *= a;
  return *this;
}

template<unsigned int N, typename C>
ALWAYS_INLINE poly_int_pod<N, C>&
poly_int_pod<N, C>::operator <<= (unsigned int a)
{
  STATIC_ASSERT (N <= 2);
  this->coeffs[0] = wi::lshift (this->coeffs[0], a);
  if (N == 2)
    this->coeffs[1] = wi::lshift (this->coeffs[1], a);
  return *this;
}

/* Return true if the polynomial value is a compile-time constant.  */

template<unsigned int N, typename C>
ALWAYS_INLINE bool
poly_int_pod<N, C>::is_constant () const
{
  STATIC_ASSERT (N <= 2);
  return N == 1 || this->coeffs[1] == 0;
}

/* Return true if the polynomial value is a compile-time constant,
   storing its value in CONST_VALUE if so.  */

template<unsigned int N, typename C>
template<typename T>
ALWAYS_INLINE typename if_lossless<C, T>::bool_type
poly_int_pod<N, C>::is_constant (T *const_value) const
{
  if (is_constant ())
    {
      *const_value = this->coeffs[0];
      return true;
    }
  return false;
}

/* Return the value of a polynomial that is already known to be a
   compile-time constant.

   NOTE: When using this function, please add a comment above the call
   explaining why we know the value is constant in that context.  */

template<unsigned int N, typename C>
ALWAYS_INLINE C
poly_int_pod<N, C>::to_constant () const
{
  gcc_checking_assert (is_constant ());
  return this->coeffs[0];
}

/* Convert X to a wide-int-based polynomial in which each coefficient
   has BITSIZE bits.  If X's coefficients are smaller than BITSIZE,
   extend them according to SGN.  */

template<unsigned int N, typename C>
template<typename Ca>
inline poly_int_pod<N, C>
poly_int_pod<N, C>::from (const poly_int_pod<N, Ca> &a,
			  unsigned int bitsize, signop sgn)
{
  poly_int_pod<N, C> r;
  for (unsigned int i = 0; i < N; i++)
    r.coeffs[i] = C::from (a.coeffs[i], bitsize, sgn);
  return r;
}

/* Convert X to a fixed-wide-int-based polynomial, extending according
   to SGN.  */

template<unsigned int N, typename C>
template<typename Ca>
inline poly_int_pod<N, C>
poly_int_pod<N, C>::from (const poly_int_pod<N, Ca> &a, signop sgn)
{
  poly_int_pod<N, C> r;
  for (unsigned int i = 0; i < N; i++)
    r.coeffs[i] = C::from (a.coeffs[i], sgn);
  return r;
}

/* Return true if the coefficients of this wide-int-based polynomial can
   be represented as signed HOST_WIDE_INTs.  Store the HOST_WIDE_INT
   representation in *R if so.

   If this polynomial is conceptually wider than HOST_WIDE_INT bits,
   the coefficients in the returned value should be sign-extended
   to the full width.  */

template<unsigned int N, typename C>
inline bool
poly_int_pod<N, C>::to_shwi (poly_int_pod<N, HOST_WIDE_INT> *r) const
{
  for (unsigned int i = 0; i < N; i++)
    if (!wi::fits_shwi_p (this->coeffs[i]))
      return false;
  for (unsigned int i = 0; i < N; i++)
    r->coeffs[i] = this->coeffs[i].to_shwi ();
  return true;
}

/* Return true if the coefficients of this wide-int-based polynomial can
   be represented as unsigned HOST_WIDE_INTs.  Store the unsigned
   HOST_WIDE_INT representation in *R if so.

   If this polynomial is conceptually wider than unsigned HOST_WIDE_INT
   bits, the coefficients in the returned value should be zero-extended
   to the full width.  */

template<unsigned int N, typename C>
inline bool
poly_int_pod<N, C>::to_uhwi (poly_int_pod<N, unsigned HOST_WIDE_INT> *r) const
{
  for (unsigned int i = 0; i < N; i++)
    if (!wi::fits_uhwi_p (this->coeffs[i]))
      return false;
  for (unsigned int i = 0; i < N; i++)
    r->coeffs[i] = this->coeffs[i].to_uhwi ();
  return true;
}

/* Force a wide-int based constant to HOST_WIDE_INT precision,
   truncating if necessary.  */

template<unsigned int N, typename C>
poly_int_pod<N, HOST_WIDE_INT>
poly_int_pod<N, C>::force_shwi () const
{
  poly_int_pod<N, HOST_WIDE_INT> r;
  for (unsigned int i = 0; i < N; i++)
    r.coeffs[i] = this->coeffs[i].to_shwi ();
  return r;
}

#if POLY_INT_CONVERSION
/* Provide a conversion operator to constants.  */

template<unsigned int N, typename C>
ALWAYS_INLINE
poly_int_pod<N, C>::operator C () const
{
  STATIC_ASSERT (N == 1);
  return this->coeffs[0];
}
#endif

/* The main class for polynomial integers.  The class provides
   constructors that are necessarily missing from the POD base.  */
template<unsigned int N, typename C>
class poly_int : public poly_int_pod<N, C>
{
public:
  ALWAYS_INLINE poly_int () {}

  template<typename Ca>
  poly_int (const poly_int<N, Ca> &);
  template<typename Ca>
  poly_int (const poly_int_pod<N, Ca> &);
  template<typename C0>
  poly_int (const C0 &);
  template<typename C0, typename C1>
  poly_int (const C0 &, const C1 &);

  template<typename Ca>
  poly_int &operator += (const poly_int_pod<N, Ca> &);
  poly_int &operator += (const C &);

  template<typename Ca>
  poly_int &operator -= (const poly_int_pod<N, Ca> &);
  poly_int &operator -= (const C &);

  poly_int &operator *= (const C &);

  poly_int &operator <<= (unsigned int);
};

template<unsigned int N, typename C>
template<typename Ca>
ALWAYS_INLINE
poly_int<N, C>::poly_int (const poly_int<N, Ca> &a)
{
  STATIC_ASSERT (N <= 2);
  this->coeffs[0] = a.coeffs[0];
  if (N == 2)
    this->coeffs[1] = a.coeffs[1];
}

template<unsigned int N, typename C>
template<typename Ca>
ALWAYS_INLINE
poly_int<N, C>::poly_int (const poly_int_pod<N, Ca> &a)
{
  STATIC_ASSERT (N <= 2);
  this->coeffs[0] = a.coeffs[0];
  if (N == 2)
    this->coeffs[1] = a.coeffs[1];
}

template<unsigned int N, typename C>
template<typename C0>
ALWAYS_INLINE
poly_int<N, C>::poly_int (const C0 &c0)
{
  STATIC_ASSERT (N <= 2);
  this->coeffs[0] = c0;
  if (N == 2)
    /* Easy way of propagating the precision of a wide_int to the
       second coefficient.  */
    this->coeffs[1] = this->coeffs[0] & 0;
}

template<unsigned int N, typename C>
template<typename C0, typename C1>
ALWAYS_INLINE
poly_int<N, C>::poly_int (const C0 &c0, const C1 &c1)
{
  STATIC_ASSERT (N == 2);
  this->coeffs[0] = c0;
  this->coeffs[1] = c1;
}

template<unsigned int N, typename C>
template<typename Ca>
ALWAYS_INLINE poly_int<N, C>&
poly_int<N, C>::operator += (const poly_int_pod<N, Ca> &a)
{
  STATIC_ASSERT (N <= 2);
  this->coeffs[0] += a.coeffs[0];
  if (N == 2)
    this->coeffs[1] += a.coeffs[1];
  return *this;
}

template<unsigned int N, typename C>
ALWAYS_INLINE poly_int<N, C>&
poly_int<N, C>::operator += (const C &a)
{
  this->coeffs[0] += a;
  return *this;
}

template<unsigned int N, typename C>
template<typename Ca>
ALWAYS_INLINE poly_int<N, C>&
poly_int<N, C>::operator -= (const poly_int_pod<N, Ca> &a)
{
  STATIC_ASSERT (N <= 2);
  this->coeffs[0] -= a.coeffs[0];
  if (N == 2)
    this->coeffs[1] -= a.coeffs[1];
  return *this;
}

template<unsigned int N, typename C>
ALWAYS_INLINE poly_int<N, C>&
poly_int<N, C>::operator -= (const C &a)
{
  this->coeffs[0] -= a;
  return *this;
}

template<unsigned int N, typename C>
ALWAYS_INLINE poly_int<N, C>&
poly_int<N, C>::operator *= (const C &a)
{
  STATIC_ASSERT (N <= 2);
  this->coeffs[0] *= a;
  if (N == 2)
    this->coeffs[1] *= a;
  return *this;
}

template<unsigned int N, typename C>
ALWAYS_INLINE poly_int<N, C>&
poly_int<N, C>::operator <<= (unsigned int a)
{
  STATIC_ASSERT (N <= 2);
  this->coeffs[0] = wi::lshift (this->coeffs[0], a);
  if (N == 2)
    this->coeffs[1] = wi::lshift (this->coeffs[1], a);
  return *this;
}

/* SFINAE class to force T to be a non-polynomial arithmetic type.  */
template<typename T>
struct if_nonpoly
{
  typedef bool bool_type;
  typedef T t;
};
template<unsigned int N, typename T> struct if_nonpoly<poly_int_pod<N, T> > {};
template<unsigned int N, typename T> struct if_nonpoly<poly_int<N, T> > {};

/* Likewise for two types T1 and T2.  */
template<typename T1, typename T2,
	 typename T3 = typename if_nonpoly<T1>::t,
	 typename T4 = typename if_nonpoly<T2>::t>
struct if_nonpoly2
{
  typedef bool bool_type;
};

/* SFINAE class to force T to be a polynomial type.  */
template<typename T> struct if_poly {};
template<unsigned int N, typename T>
struct if_poly<poly_int_pod<N, T> >
{
  typedef bool bool_type;
  typedef poly_int_pod<N, T> t;
};
template<unsigned int N, typename T>
struct if_poly<poly_int<N, T> >
{
  typedef bool bool_type;
  typedef poly_int<N, T> t;
};

/* poly_result<T1, T2>::t gives the result type for T1 + T2.  The intention
   is to provide normal C-like rules for integer ranks, except that
   everything smaller than HOST_WIDE_INT promotes to HOST_WIDE_INT.  */
#define RANK(X) int_traits<X>::rank
template<unsigned int N, typename T1, typename T2 = T1,
	  int sel = ((RANK (T1) < RANK (HOST_WIDE_INT)
		      && RANK (T2) < RANK (HOST_WIDE_INT))
		     ? 0 : RANK (T1) < RANK (T2) ? 1 : 2)>
struct poly_result;
#undef RANK

/* Promote pair to HOST_WIDE_INT.  */
template<unsigned int N, typename T1, typename T2>
struct poly_result<N, T1, T2, 0>
{
  typedef poly_int<N, HOST_WIDE_INT> t;
};

/* T1 promotes to T2.  */
template<unsigned int N, typename T1, typename T2>
struct poly_result<N, T1, T2, 1>
{
  typedef poly_int<N, typename int_traits <T2>::result> t;
};

/* T2 promotes to T1.  */
template<unsigned int N, typename T1, typename T2>
struct poly_result<N, T1, T2, 2>
{
  typedef poly_int<N, typename int_traits <T1>::result> t;
};

#define POLY_POLY_RESULT(N, T1, T2) typename poly_result<N, T1, T2>::t
#define POLY_SCALAR_RESULT(N, T1, T2) \
  typename poly_result<N, T1, typename if_nonpoly<T2>::t>::t
#define SCALAR_POLY_RESULT(N, T1, T2) \
  typename poly_result<N, typename if_nonpoly<T1>::t, T2>::t

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE POLY_POLY_RESULT (N, Ca, Cb)
operator + (const poly_int_pod<N, Ca> &a, const poly_int_pod<N, Cb> &b)
{
  typedef POLY_POLY_RESULT (N, Ca, Cb)::t C;
  STATIC_ASSERT (N <= 2);
  poly_int<N, C> r;
  r.coeffs[0] = C (a.coeffs[0]) + b.coeffs[0];
  if (N == 2)
    r.coeffs[1] = C (a.coeffs[1]) + b.coeffs[1];
  return r;
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE POLY_SCALAR_RESULT (N, Ca, Cb)
operator + (const poly_int_pod<N, Ca> &a, const Cb &b)
{
  typedef POLY_SCALAR_RESULT (N, Ca, Cb)::t C;
  STATIC_ASSERT (N <= 2);
  poly_int<N, C> r;
  r.coeffs[0] = C (a.coeffs[0]) + b;
  if (N == 2)
    r.coeffs[1] = C (a.coeffs[1]);
  return r;
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE SCALAR_POLY_RESULT (N, Ca, Cb)
operator + (const Ca &a, const poly_int_pod<N, Cb> &b)
{
  typedef SCALAR_POLY_RESULT (N, Ca, Cb)::t C;
  STATIC_ASSERT (N <= 2);
  poly_int<N, C> r;
  r.coeffs[0] = C (a) + b.coeffs[0];
  if (N == 2)
    r.coeffs[1] = C (b.coeffs[1]);
  return r;
}

namespace wi {
/* Poly version of wi::add, with the same interface.  */

template<unsigned int N, typename C>
poly_int<N, C>
add (const poly_int_pod<N, C> &a, const poly_int_pod<N, C> &b,
     signop sgn, bool *overflow)
{
  poly_int_pod<N, C> r;
  *overflow = false;
  bool suboverflow;
  for (unsigned int i = 0; i < N; ++i)
    {
      r.coeffs[i] = wi::add (a.coeffs[i], b.coeffs[i], sgn, &suboverflow);
      *overflow |= suboverflow;
    }
  return r;
}
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE POLY_POLY_RESULT (N, Ca, Cb)
operator - (const poly_int_pod<N, Ca> &a, const poly_int_pod<N, Cb> &b)
{
  typedef POLY_POLY_RESULT (N, Ca, Cb)::t C;
  STATIC_ASSERT (N <= 2);
  poly_int<N, C> r;
  r.coeffs[0] = C (a.coeffs[0]) - b.coeffs[0];
  if (N == 2)
    r.coeffs[1] = C (a.coeffs[1]) - b.coeffs[1];
  return r;
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE POLY_SCALAR_RESULT (N, Ca, Cb)
operator - (const poly_int_pod<N, Ca> &a, const Cb &b)
{
  typedef POLY_SCALAR_RESULT (N, Ca, Cb)::t C;
  STATIC_ASSERT (N <= 2);
  poly_int<N, C> r;
  r.coeffs[0] = C (a.coeffs[0]) - b;
  if (N == 2)
    r.coeffs[1] = C (a.coeffs[1]);
  return r;
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE SCALAR_POLY_RESULT (N, Ca, Cb)
operator - (const Ca &a, const poly_int_pod<N, Cb> &b)
{
  typedef SCALAR_POLY_RESULT (N, Ca, Cb)::t C;
  STATIC_ASSERT (N <= 2);
  poly_int<N, C> r;
  r.coeffs[0] = C (a) - b.coeffs[0];
  if (N == 2)
    r.coeffs[1] = -C (b.coeffs[1]);
  return r;
}

namespace wi {
/* Poly version of wi::sub, with the same interface.  */

template<unsigned int N, typename C>
poly_int<N, C>
sub (const poly_int_pod<N, C> &a, const poly_int_pod<N, C> &b,
     signop sgn, bool *overflow)
{
  poly_int<N, C> r;
  *overflow = false;
  bool suboverflow;
  for (unsigned int i = 0; i < N; ++i)
    {
      r.coeffs[i] = wi::sub (a.coeffs[i], b.coeffs[i], sgn, &suboverflow);
      *overflow |= suboverflow;
    }
  return r;
}
}

template<unsigned int N, typename Ca>
ALWAYS_INLINE POLY_POLY_RESULT (N, Ca, Ca)
operator - (const poly_int_pod<N, Ca> &a)
{
  typedef POLY_POLY_RESULT (N, Ca, Ca)::t C;
  poly_int<N, C> r;
  for (unsigned int i = 0; i < N; ++i)
    r.coeffs[i] = -C (a.coeffs[i]);
  return r;
}

namespace wi {
/* Poly version of wi::neg, with the same interface.  */

template<unsigned int N, typename C>
poly_int<N, C>
neg (const poly_int_pod<N, C> &a, bool *overflow)
{
  poly_int<N, C> r;
  *overflow = false;
  bool suboverflow;
  for (unsigned int i = 0; i < N; ++i)
    {
      r.coeffs[i] = wi::neg (a.coeffs[i], &suboverflow);
      *overflow |= suboverflow;
    }
  return r;
}

/* Poly version of wi::sext, with the same interface.  */

template<unsigned int N, typename C>
inline POLY_POLY_RESULT (N, C, C)
sext (const poly_int_pod<N, C> &a, unsigned int precision)
{
  POLY_POLY_RESULT (N, C, C) r;
  for (unsigned int i = 0; i < N; ++i)
    r.coeffs[i] = wi::sext (a.coeffs[i], precision);
  return r;
}
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE POLY_SCALAR_RESULT (N, Ca, Cb)
operator * (const poly_int_pod<N, Ca> &a, const Cb &b)
{
  typedef POLY_SCALAR_RESULT (N, Ca, Cb)::t C;
  STATIC_ASSERT (N <= 2);
  poly_int<N, C> r;
  r.coeffs[0] = C (a.coeffs[0]) * b;
  if (N == 2)
    r.coeffs[1] = C (a.coeffs[1]) * b;
  return r;
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE SCALAR_POLY_RESULT (N, Ca, Cb)
operator * (const Ca &a, const poly_int_pod<N, Cb> &b)
{
  typedef SCALAR_POLY_RESULT (N, Ca, Cb)::t C;
  STATIC_ASSERT (N <= 2);
  poly_int<N, C> r;
  r.coeffs[0] = C (a) * b.coeffs[0];
  if (N == 2)
    r.coeffs[1] = C (a) * b.coeffs[1];
  return r;
}

namespace wi {
/* Poly version of wi::mul, with the same interface.  */

template<unsigned int N, typename C>
poly_int<N, C>
mul (const poly_int_pod<N, C> &a, const C &b,
     signop sgn, bool *overflow)
{
  poly_int<N, C> r;
  *overflow = false;
  bool suboverflow;
  for (unsigned int i = 0; i < N; ++i)
    {
      r.coeffs[i] = wi::mul (a.coeffs[i], b, sgn, &suboverflow);
      *overflow |= suboverflow;
    }
  return r;
}
}

template<unsigned int N, typename Ca>
ALWAYS_INLINE POLY_POLY_RESULT (N, Ca, Ca)
operator << (const poly_int_pod<N, Ca> &a, unsigned int b)
{
  typedef POLY_POLY_RESULT (N, Ca, Ca)::t C;
  STATIC_ASSERT (N <= 2);
  poly_int<N, C> r;
  r.coeffs[0] = wi::lshift (C (a.coeffs[0]), b);
  if (N == 2)
    r.coeffs[1] = wi::lshift (C (a.coeffs[1]), b);
  return r;
}

/* Return true if a0 + a1 * x might equal b0 + b1 * x for some nonnegative
   integer x.  */

template<typename Ca, typename Cb>
inline bool
may_eq_2 (const Ca &a0, const Ca &a1, const Cb &b0, const Cb &b1)
{
  if (a1 != b1)
     /*      a0 + a1 * x == b0 + b1 * x
       ==> (a1 - b1) * x == b0 - a0
       ==>             x == (b0 - a0) / (a1 - b1)

       We need to test whether that's a valid value of x.
       (b0 - a0) and (a1 - b1) must not have opposite signs
       and the result must be integral.  */
    return ((a1 < b1 ? b0 <= a0 : b0 >= a0)
	    && (b0 - a0) % (a1 - b1) == 0);
  return a0 == b0;
}

/* Return true if a0 + a1 * x might equal b for some nonnegative
   integer x.  */

template<typename Ca, typename Cb>
inline bool
may_eq_2 (const Ca &a0, const Ca &a1, const Cb &b)
{
  if (a1 != 0)
     /*      a0 + a1 * x == b
       ==>             x == (b - a0) / a1

       We need to test whether that's a valid value of x.
       (b - a0) and a1 must not have opposite signs and the
       result must be integral.  For the latter test we use
       "a0 - b" rather than "b - a0" in order to cope with
       cases in which a0 is a wide_int.  */
    return ((a1 < 0 ? b <= a0 : b >= a0)
	    && (a0 - b) % a1 == 0);
  return a0 == b;
}

/* Return true if A might equal B for some indeterminate values.  */

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE bool
may_eq (const poly_int_pod<N, Ca> &a, const poly_int_pod<N, Cb> &b)
{
  STATIC_ASSERT (N <= 2);
  if (N == 2)
    return may_eq_2 (a.coeffs[0], a.coeffs[1], b.coeffs[0], b.coeffs[1]);
  return a.coeffs[0] == b.coeffs[0];
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE typename if_nonpoly<Cb>::bool_type
may_eq (const poly_int_pod<N, Ca> &a, const Cb &b)
{
  STATIC_ASSERT (N <= 2);
  if (N == 2)
    return may_eq_2 (a.coeffs[0], a.coeffs[1], b);
  return a.coeffs[0] == b;
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE typename if_nonpoly<Ca>::bool_type
may_eq (const Ca &a, const poly_int_pod<N, Cb> &b)
{
  STATIC_ASSERT (N <= 2);
  if (N == 2)
    return may_eq_2 (b.coeffs[0], b.coeffs[1], a);
  return a == b.coeffs[0];
}

template<typename Ca, typename Cb>
ALWAYS_INLINE typename if_nonpoly2<Ca, Cb>::bool_type
may_eq (const Ca &a, const Cb &b)
{
  return a == b;
}

/* Return true if A might not equal B for some indeterminate values.  */

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE bool
may_ne (const poly_int_pod<N, Ca> &a, const poly_int_pod<N, Cb> &b)
{
  STATIC_ASSERT (N <= 2);
  if (N == 2 && a.coeffs[1] != b.coeffs[1])
    return true;
  return a.coeffs[0] != b.coeffs[0];
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE typename if_nonpoly<Cb>::bool_type
may_ne (const poly_int_pod<N, Ca> &a, const Cb &b)
{
  STATIC_ASSERT (N <= 2);
  if (N == 2 && a.coeffs[1] != 0)
    return true;
  return a.coeffs[0] != b;
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE typename if_nonpoly<Ca>::bool_type
may_ne (const Ca &a, const poly_int_pod<N, Cb> &b)
{
  STATIC_ASSERT (N <= 2);
  if (N == 2 && 0 != b.coeffs[1])
    return true;
  return a != b.coeffs[0];
}

template<typename Ca, typename Cb>
ALWAYS_INLINE typename if_nonpoly2<Ca, Cb>::bool_type
may_ne (const Ca &a, const Cb &b)
{
  return a != b;
}

/* Return true if A must be equal to B.  */
#define must_eq(A, B) (!may_ne (A, B))

/* Return true if A must be unequal to B.  */
#define must_ne(A, B) (!may_eq (A, B))

/* Return true if A might be less than or equal to B for some
   indeterminate values.  */

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE bool
may_le (const poly_int_pod<N, Ca> &a, const poly_int_pod<N, Cb> &b)
{
  STATIC_ASSERT (N <= 2);
  if (N == 2 && a.coeffs[1] < b.coeffs[1])
    return true;
  return a.coeffs[0] <= b.coeffs[0];
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE typename if_nonpoly<Cb>::bool_type
may_le (const poly_int_pod<N, Ca> &a, const Cb &b)
{
  STATIC_ASSERT (N <= 2);
  if (N == 2 && a.coeffs[1] < 0)
    return true;
  return a.coeffs[0] <= b;
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE typename if_nonpoly<Ca>::bool_type
may_le (const Ca &a, const poly_int_pod<N, Cb> &b)
{
  STATIC_ASSERT (N <= 2);
  if (N == 2 && 0 < b.coeffs[1])
    return true;
  return a <= b.coeffs[0];
}

template<typename Ca, typename Cb>
ALWAYS_INLINE typename if_nonpoly2<Ca, Cb>::bool_type
may_le (const Ca &a, const Cb &b)
{
  return a <= b;
}

/* Return true if A might be less than B for some indeterminate values.  */

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE bool
may_lt (const poly_int_pod<N, Ca> &a, const poly_int_pod<N, Cb> &b)
{
  STATIC_ASSERT (N <= 2);
  if (N == 2 && a.coeffs[1] < b.coeffs[1])
    return true;
  return a.coeffs[0] < b.coeffs[0];
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE typename if_nonpoly<Cb>::bool_type
may_lt (const poly_int_pod<N, Ca> &a, const Cb &b)
{
  STATIC_ASSERT (N <= 2);
  if (N == 2 && a.coeffs[1] < 0)
    return true;
  return a.coeffs[0] < b;
}

template<unsigned int N, typename Ca, typename Cb>
ALWAYS_INLINE typename if_nonpoly<Ca>::bool_type
may_lt (const Ca &a, const poly_int_pod<N, Cb> &b)
{
  STATIC_ASSERT (N <= 2);
  if (N == 2 && 0 < b.coeffs[1])
    return true;
  return a < b.coeffs[0];
}

template<typename Ca, typename Cb>
ALWAYS_INLINE typename if_nonpoly2<Ca, Cb>::bool_type
may_lt (const Ca &a, const Cb &b)
{
  return a < b;
}

/* Return true if A may be greater than or equal to B.  */
#define may_ge(A, B) may_le (B, A)

/* Return true if A may be greater than B.  */
#define may_gt(A, B) may_lt (B, A)

/* Return true if A must be less than or equal to B.  */
#define must_le(A, B) (!may_gt (A, B))

/* Return true if A must be less than B.  */
#define must_lt(A, B) (!may_ge (A, B))

/* Return true if A must be greater than B.  */
#define must_gt(A, B) (!may_le (A, B))

/* Return true if A must be greater than or equal to B.  */
#define must_ge(A, B) (!may_lt (A, B))

/* Return true if A and B are ordered by the partial ordering must_le.  */

template<typename T1, typename T2>
inline bool
ordered_p (const T1 &a, const T2 &b)
{
  return must_le (a, b) || must_le (b, a);
}

/* Given that A and B are known to be ordered, return the minimum
   of the two.  */

template<unsigned int N, typename Ca, typename Cb>
inline POLY_POLY_RESULT (N, Ca, Cb)
ordered_min (const poly_int_pod<N, Ca> &a, const poly_int_pod<N, Cb> &b)
{
  if (must_le (a, b))
    return a;
  else
    {
      gcc_checking_assert (must_le (b, a));
      return b;
    }
}

template<unsigned int N, typename Ca, typename Cb>
inline SCALAR_POLY_RESULT (N, Ca, Cb)
ordered_min (const Ca &a, const poly_int_pod<N, Cb> &b)
{
  if (must_le (a, b))
    return a;
  else
    {
      gcc_checking_assert (must_le (b, a));
      return b;
    }
}

template<unsigned int N, typename Ca, typename Cb>
inline POLY_SCALAR_RESULT (N, Ca, Cb)
ordered_min (const poly_int_pod<N, Ca> &a, const Cb &b)
{
  if (must_le (a, b))
    return a;
  else
    {
      gcc_checking_assert (must_le (b, a));
      return b;
    }
}

/* Given that A and B are known to be ordered, return the maximum
   of the two.  */

template<unsigned int N, typename Ca, typename Cb>
inline POLY_POLY_RESULT (N, Ca, Cb)
ordered_max (const poly_int_pod<N, Ca> &a, const poly_int_pod<N, Cb> &b)
{
  if (must_le (a, b))
    return b;
  else
    {
      gcc_checking_assert (must_le (b, a));
      return a;
    }
}

template<unsigned int N, typename Ca, typename Cb>
inline SCALAR_POLY_RESULT (N, Ca, Cb)
ordered_max (const Ca &a, const poly_int_pod<N, Cb> &b)
{
  if (must_le (a, b))
    return b;
  else
    {
      gcc_checking_assert (must_le (b, a));
      return a;
    }
}

template<unsigned int N, typename Ca, typename Cb>
inline POLY_SCALAR_RESULT (N, Ca, Cb)
ordered_max (const poly_int_pod<N, Ca> &a, const Cb &b)
{
  if (must_le (a, b))
    return b;
  else
    {
      gcc_checking_assert (must_le (b, a));
      return a;
    }
}

/* Return a constant lower bound on the value of A, which is known
   to be nonnegative.  */

template<unsigned int N, typename Ca>
inline Ca
constant_lower_bound (const poly_int_pod<N, Ca> &a)
{
  gcc_checking_assert (must_ge (a, Ca (0)));
  return a.coeffs[0];
}

/* Return a value that is known to be no less than A and B, both of
   which are known to be nonnegative.  This will be the least upper
   bound for some indeterminate values but not necessarily for all.  */

template<unsigned int N, typename Ca, typename Cb>
inline POLY_SCALAR_RESULT (N, Ca, Cb)
upper_bound (const poly_int_pod<N, Ca> &a, const Cb &b)
{
  typedef POLY_SCALAR_RESULT (N, Ca, Cb)::t C;
  gcc_checking_assert (must_ge (a, Ca (0)));
  gcc_checking_assert (b >= Cb (0));
  poly_int<N, C> r;
  r.coeffs[0] = MAX (C (a.coeffs[0]), C (b));
  for (unsigned int i = 1; i < N; ++i)
    r.coeffs[1] = C (a.coeffs[i]);
  return r;
}

/* Return a value that is known to be no less than A and B, both of
   which are known to be nonnegative.  This will be the least upper
   bound for some indeterminate values but not necessarily for all.  */

template<unsigned int N, typename Ca, typename Cb>
inline POLY_POLY_RESULT (N, Ca, Cb)
upper_bound (const poly_int_pod<N, Ca> &a, const poly_int_pod<N, Cb> &b)
{
  typedef POLY_POLY_RESULT (N, Ca, Cb)::t C;
  gcc_checking_assert (must_ge (a, Ca (0)));
  gcc_checking_assert (must_ge (b, Cb (0)));
  poly_int<N, C> r;
  for (unsigned int i = 0; i < N; ++i)
    r.coeffs[i] = MAX (C (a.coeffs[i]), C (b.coeffs[i]));
  return r;
}

/* Return the greatest common divisor of all nonzero coefficients, or zero
   if all coefficients are zero.  */

template<unsigned int N, typename Ca>
inline Ca
coeff_gcd (const poly_int_pod<N, Ca> &a)
{
  /* Find the first nonzero coefficient, stopping at 0 whatever happens.  */
  unsigned int i;
  for (i = N - 1; i > 0; --i)
    if (a.coeffs[i] != 0)
      break;
  Ca r = a.coeffs[i];
  for (unsigned int j = 0; j < i; ++j)
    if (a.coeffs[j] != 0)
      r = gcd (r, a.coeffs[j]);
  return r;
}

/* Return a value that is a multiple of both A and B.  This will be the
   least common multiple for some indeterminte values but necessarily
   for all.  */

template<unsigned int N, typename Ca, typename Cb>
POLY_SCALAR_RESULT (N, Ca, Cb)
common_multiple (const poly_int_pod<N, Ca> &a, Cb b)
{
  Ca xgcd = coeff_gcd (a);
  return a * (least_common_multiple (xgcd, b) / xgcd);
}

/* Likewise, but for two polynomial values.  */

template<unsigned int N, typename Ca, typename Cb>
POLY_POLY_RESULT (N, Ca, Cb)
common_multiple (const poly_int_pod<N, Ca> &a, const poly_int_pod<N, Cb> &b)
{
  STATIC_ASSERT (N <= 2);

  if (b.is_constant ())
    return common_multiple (a, b.coeffs[0]);

  if (a.is_constant ())
    return common_multiple (b, a.coeffs[0]);

  typedef POLY_POLY_RESULT (N, Ca, Cb)::t C;

  C lcm = least_common_multiple (a.coeffs[1], b.coeffs[1]);
  C amul = lcm / a.coeffs[1];
  C bmul = lcm / b.coeffs[1];
  gcc_checking_assert (a.coeffs[0] * amul == b.coeffs[0] * bmul);

  return a * amul;
}

/* Compare A and B for sorting purposes, returning -1 if A should come
   before B, 0 if A and B are identical, and 1 if A should come after B.
   This is a lexicographical compare of the coefficients in reverse order.

   A consequence of this is that all constant sizes come before all
   non-constant ones, regardless of magnitude.  This is what most
   callers want.  E.g. when laying data out on the stack, it's better to
   keep all the constant-sized data together so that it can be accessed
   as a constant offset from a single base.  */

template<unsigned int N, typename Ca, typename Cb>
inline int
compare_sizes_for_sort (const poly_int_pod<N, Ca> &a,
			const poly_int_pod<N, Cb> &b)
{
  for (unsigned int i = N; i-- > 0; )
    if (a.coeffs[i] != b.coeffs[i])
      return a.coeffs[i] < b.coeffs[i] ? -1 : 1;
  return 0;
}

/* Return true if we can calculate VALUE & (ALIGN - 1) at compile time.  */

template<unsigned int N, typename Ca, typename Cb>
inline bool
can_align_p (const poly_int_pod<N, Ca> &value, Cb align)
{
  for (unsigned int i = 1; i < N; i++)
    if ((value.coeffs[i] & (align - 1)) != 0)
      return false;
  return true;
}

/* Return true if we can align VALUE up to the smallest multiple of
   ALIGN that is >= VALUE.  Store the aligned value in *ALIGNED if so.  */

template<unsigned int N, typename Ca, typename Cb>
inline bool
can_align_up (const poly_int_pod<N, Ca> &value, Cb align,
	      poly_int<N, Ca> *aligned)
{
  if (!can_align_p (value, align))
    return false;
  *aligned = value + (-value.coeffs[0] & (align - 1));
  return true;
}

/* Return true if we can align VALUE down to the largest multiple of
   ALIGN that is <= VALUE.  Store the aligned value in *ALIGNED if so.  */

template<unsigned int N, typename Ca, typename Cb>
inline bool
can_align_down (const poly_int_pod<N, Ca> &value, Cb align,
		poly_int<N, Ca> *aligned)
{
  if (!can_align_p (value, align))
    return false;
  *aligned = value - (value.coeffs[0] & (align - 1));
  return true;
}

/* Return true if we can align A and B to the smallest multiples of
   ALIGN that are >= A and B respectively, and if doing so gives the
   same value.  */

template<unsigned int N, typename Ca, typename Cb, typename Cc>
inline bool
known_equal_after_align_up (const poly_int_pod<N, Ca> &a,
			    const poly_int_pod<N, Cb> &b,
			    Cc align)
{
  poly_int<N, Ca> aligned_a;
  poly_int<N, Cb> aligned_b;
  return (can_align_up (a, align, &aligned_a)
	  && can_align_up (b, align, &aligned_b)
	  && must_eq (aligned_a, aligned_b));
}

/* Return true if we can align A and B to the largest multiples of
   ALIGN that are <= A and B respectively, and if doing so gives the
   same value.  */

template<unsigned int N, typename Ca, typename Cb, typename Cc>
inline bool
known_equal_after_align_down (const poly_int_pod<N, Ca> &a,
			      const poly_int_pod<N, Cb> &b,
			      Cc align)
{
  poly_int<N, Ca> aligned_a;
  poly_int<N, Cb> aligned_b;
  return (can_align_down (a, align, &aligned_a)
	  && can_align_down (b, align, &aligned_b)
	  && must_eq (aligned_a, aligned_b));
}

/* Align VALUE up to the smallest multiple of ALIGN that is >= VALUE.

   NOTE: When using this function, please add a comment above the call
   explaining why we know the non-constant coefficients must already
   be a multiple of ALIGN.  */

template<unsigned int N, typename Ca, typename Cb>
inline poly_int<N, Ca>
force_align_up (const poly_int_pod<N, Ca> &value, Cb align)
{
  gcc_checking_assert (can_align_p (value, align));
  return value + (-value.coeffs[0] & (align - 1));
}

/* Align VALUE down to the largest multiple of ALIGN that is <= VALUE.

   NOTE: When using this function, please add a comment above the call
   explaining why we know the non-constant coefficients must already
   be a multiple of ALIGN.  */

template<unsigned int N, typename Ca, typename Cb>
inline poly_int<N, Ca>
force_align_down (const poly_int_pod<N, Ca> &value, Cb align)
{
  gcc_checking_assert (can_align_p (value, align));
  return value - (value.coeffs[0] & (align - 1));
}

/* Return a value <= VALUE that is a multiple of ALIGN.  It will be the
   greatest such value for some indeterminate values but not necessarily
   for all.  */

template<unsigned int N, typename Ca, typename Cb>
inline poly_int<N, Ca>
aligned_lower_bound (const poly_int_pod<N, Ca> &value, Cb align)
{
  gcc_checking_assert (ordered_p (value, Ca (0)));
  poly_int<N, Ca> r;
  for (unsigned int i = 0; i < N; ++i)
    r.coeffs[i] = value.coeffs[i] & -Ca (align);
  return r;
}

/* Return a value >= VALUE that is a multiple of ALIGN.  It will be the
   least such value for some indeterminate values but not necessarily
   for all.  */

template<unsigned int N, typename Ca, typename Cb>
inline poly_int<N, Ca>
aligned_upper_bound (const poly_int_pod<N, Ca> &value, Cb align)
{
  gcc_checking_assert (ordered_p (value, Ca (0)));
  poly_int<N, Ca> r;
  for (unsigned int i = 0; i < N; ++i)
    r.coeffs[i] = value.coeffs[i] + (-value.coeffs[i] & Ca (align - 1));
  return r;
}

/* Align VALUE down to the largest multiple of ALIGN that is <= VALUE,
   then divide by ALIGN.

   NOTE: When using this function, please add a comment above the call
   explaining why we know the non-constant coefficients must already
   be a multiple of ALIGN.  */

template<unsigned int N, typename Ca, typename Cb>
inline poly_int<N, Ca>
force_align_down_and_div (const poly_int_pod<N, Ca> &value, Cb align)
{
  gcc_checking_assert (can_align_p (value, align));
  poly_int<N, Ca> r;
  r.coeffs[0] = (value.coeffs[0] - (value.coeffs[0] & (align - 1))) / align;
  for (unsigned int i = 1; i < N; ++i)
    r.coeffs[i] = value.coeffs[i] / align;
  return r;
}

/* Align VALUE up to the smallest multiple of ALIGN that is >= VALUE,
   then divide by ALIGN.

   NOTE: When using this function, please add a comment above the call
   explaining why we know the non-constant coefficients must already
   be a multiple of ALIGN.  */

template<unsigned int N, typename Ca, typename Cb>
inline poly_int<N, Ca>
force_align_up_and_div (const poly_int_pod<N, Ca> &value, Cb align)
{
  gcc_checking_assert (can_align_p (value, align));
  poly_int<N, Ca> r;
  r.coeffs[0] = (value.coeffs[0] + (-value.coeffs[0] & (align - 1))) / align;
  for (unsigned int i = 1; i < N; ++i)
    r.coeffs[i] = value.coeffs[i] / align;
  return r;
}

/* Return true if we know at compile time the difference between VALUE
   and the equal or preceding multiple of ALIGN.  Store the value in
   *MISALIGN if so.  */

template<unsigned int N, typename Ca, typename Cb, typename Cm>
inline bool
known_misalignment (const poly_int_pod<N, Ca> &value, Cb align, Cm *misalign)
{
  gcc_checking_assert (align != 0);
  for (unsigned int i = 1; i < N; ++i)
    if ((value.coeffs[i] & (align - 1)) != 0)
      return false;
  *misalign = value.coeffs[0] & (align - 1);
  return true;
}

/* Return X & (Y - 1), asserting that this value is known.  Please add
   an a comment above callers to this function to explain why the condition
   is known to hold.  */

template<unsigned int N, typename Ca, typename Cb>
inline Ca
force_get_misalignment (const poly_int_pod<N, Ca> &a, Cb align)
{
  gcc_checking_assert (can_align_p (a, align));
  return a.coeffs[0] & (align - 1);
}

/* Return the maximum alignment that A is known to have.  Return 0
   if A is known to be zero.  */

template<unsigned int N, typename Ca>
inline Ca
known_alignment (const poly_int_pod<N, Ca> &a)
{
  /* Calculate the minimum alignment for each coefficient individually.
     Take the minimum of the nonzero values, all of which will be powers
     of 2.  */
  Ca r = a.coeffs[0] & -a.coeffs[0];
  for (unsigned int i = 1; i < N; ++i)
    {
      Ca part = a.coeffs[i] & -a.coeffs[i];
      r = (r & (part - 1)) | (part & (r - 1));
    }
  return r;
}

/* Return true if we can compute A | B at compile time, storing the
   result in RES if so.  */

template<unsigned int N, typename Ca, typename Cb, typename Cr>
inline typename if_nonpoly<Cb>::bool_type
can_ior_p (const poly_int_pod<N, Ca> &a, Cb b, Cr *result)
{
  STATIC_ASSERT (N <= 2);
  /* Coefficient 1 must be a multiple of something greater than B.  */
  if (N == 2
      && a.coeffs[1] != 0
      && (a.coeffs[1] & -a.coeffs[1]) < b)
    return false;
  *result = a;
  result->coeffs[0] |= b;
  return true;
}

/* Return true if A is a constant multiple of B, storing the
   multiple in *MULTIPLE if so.  */

template<unsigned int N, typename Ca, typename Cb, typename Cm>
inline typename if_nonpoly<Cb>::bool_type
constant_multiple_p (const poly_int_pod<N, Ca> &a, Cb b, Cm *multiple)
{
  /* Do the modulus before the constant check, to catch divide by
     zero errors.  */
  if (a.coeffs[0] % b != 0 || !a.is_constant ())
    return false;
  *multiple = a.coeffs[0] / b;
  return true;
}

/* Return true if A is a constant multiple of B, storing the multiple
   in *MULTIPLE if so.  */

template<unsigned int N, typename Ca, typename Cb, typename Cm>
inline bool
constant_multiple_p (const poly_int_pod<N, Ca> &a,
		     const poly_int_pod<N, Cb> &b, Cm *multiple)
{
  STATIC_ASSERT (N <= 2);
  if (b.is_constant ())
    return constant_multiple_p (a, b.coeffs[0], multiple);

  typedef POLY_POLY_RESULT (N, Ca, Cb)::t C;

  if (a.coeffs[1] % b.coeffs[1] != 0)
    return false;

  C r = a.coeffs[1] / b.coeffs[1];
  if (a.coeffs[0] % b.coeffs[0] != 0 || a.coeffs[0] / b.coeffs[0] != r)
    return false;

  *multiple = r;
  return true;
}

/* Return true if A is a (polynomial) multiple of B.  */

template<unsigned int N, typename Ca, typename Cb>
inline typename if_nonpoly<Cb>::bool_type
multiple_p (const poly_int_pod<N, Ca> &a, Cb b)
{
  for (unsigned int i = 0; i < N; ++i)
    if (a.coeffs[i] % b != 0)
      return false;
  return true;
}

/* Return true if B is a constant and A is a (constant) multiple of B.  */

template<unsigned int N, typename Ca, typename Cb>
inline typename if_nonpoly<Ca>::bool_type
multiple_p (Ca a, const poly_int_pod<N, Cb> &b)
{
  /* Do the modulus before the constant check, to catch divide by
     potential zeros.  */
  return a % b.coeffs[0] == 0 && b.is_constant ();
}

/* Return true if A is a (polynomial) multiple of B.  This handles cases
   where either B is constant or the multiple is constant.  */

template<unsigned int N, typename Ca, typename Cb>
inline bool
multiple_p (const poly_int_pod<N, Ca> &a, const poly_int_pod<N, Cb> &b)
{
  if (b.is_constant ())
    return multiple_p (a, b.coeffs[0]);
  Ca tmp;
  return constant_multiple_p (a, b, &tmp);
}

/* Return true if A is a (constant) multiple of B, storing the
   multiple in *MULTIPLE if so.  */

template<typename Ca, typename Cb, typename Cm>
inline typename if_nonpoly2<Ca, Cb>::bool_type
multiple_p (Ca a, Cb b, Cm *multiple)
{
  if (a % b != 0)
    return false;
  *multiple = a / b;
  return true;
}

/* Return true if A is a (polynomial) multiple of B, storing the
   multiple in *MULTIPLE if so.  */

template<unsigned int N, typename Ca, typename Cb, typename Cm>
inline typename if_nonpoly<Cb>::bool_type
multiple_p (const poly_int_pod<N, Ca> &a, Cb b, poly_int_pod<N, Cm> *multiple)
{
  if (!multiple_p (a, b))
    return false;
  for (unsigned int i = 0; i < N; ++i)
    multiple->coeffs[i] = a.coeffs[i] / b;
  return true;
}

/* Return true if B is a constant and A is a (constant) multiple of B,
   storing the multiple in *MULTIPLE if so.  */

template<unsigned int N, typename Ca, typename Cb, typename Cm>
inline typename if_nonpoly<Ca>::bool_type
multiple_p (Ca a, const poly_int_pod<N, Cb> &b, Cm *multiple)
{
  /* Do the modulus before the constant check, to catch divide by
     potential zeros.  */
  if (a % b.coeffs[0] != 0 || !b.is_constant ())
    return false;
  *multiple = a / b.coeffs[0];
  return true;
}

/* Return true if A is a (polynomial) multiple of B, storing the
   multiple in *MULTIPLE if so.  This handles cases where either
   B is constant or the multiple is constant.  */

template<unsigned int N, typename Ca, typename Cb, typename Cm>
inline bool
multiple_p (const poly_int_pod<N, Ca> &a, const poly_int_pod<N, Cb> &b,
	    poly_int<N, Cm> *multiple)
{
  if (b.is_constant ())
    return multiple_p (a, b.coeffs[0], multiple);
  return constant_multiple_p (a, b, multiple);
}

/* Return A / B, given that A is known to be a multiple of B.  */

template<unsigned int N, typename Ca, typename Cb>
inline POLY_SCALAR_RESULT (N, Ca, Cb)
exact_div (const poly_int_pod<N, Ca> &a, Cb b)
{
  typedef POLY_POLY_RESULT (N, Ca, Cb)::t C;
  poly_int<N, C> r;
  for (unsigned int i = 0; i < N; ++i)
    {
      gcc_checking_assert (a.coeffs[i] % b == 0);
      r.coeffs[i] = a.coeffs[i] / b;
    }
  return r;
}

/* Return A / B, given that A is known to be a multiple of B.  */

template<unsigned int N, typename Ca, typename Cb>
inline typename if_nonpoly<Ca>::t
exact_div (const Ca &a, const poly_int_pod<N, Cb> &b)
{
  gcc_checking_assert (b.is_constant () && a % b.coeffs[0] == 0);
  return a / b.coeffs[0];
}

/* Return A / B, given that A is known to be a multiple of B.  */

template<unsigned int N, typename Ca, typename Cb>
inline POLY_POLY_RESULT (N, Ca, Cb)
exact_div (const poly_int_pod<N, Ca> &a, const poly_int_pod<N, Cb> &b)
{
  if (a.is_constant ())
    return exact_div (a.coeffs[0], b);

  if (b.is_constant ())
    return exact_div (a, b.coeffs[0]);

  typedef POLY_POLY_RESULT (N, Ca, Cb)::t C;

  gcc_checking_assert (a.coeffs[0] % b.coeffs[0] == 0);
  C r = C (a.coeffs[0]) / b.coeffs[0];
  for (unsigned int i = 1; i < N; ++i)
    gcc_checking_assert (a.coeffs[i] % b.coeffs[i] == 0
			 && a.coeffs[i] / b.coeffs[i] == r);
  return r;
}

/* Return true if there is some constant Q and polynomial r such that:

     (1) a = b * Q + r
     (2) 0 <= |b * Q| <= |a|
     (3) |r| < |b|

   Store the value Q in *QUOTIENT if so.  */

template<unsigned int N, typename Ca, typename Cb, typename Cq>
inline typename if_nonpoly2<Cb, Cq>::bool_type
can_div_trunc_p (const poly_int_pod<N, Ca> &a, Cb b, Cq *quotient)
{
  /* Do the modulus before the constant check, to catch divide by
     zero errors.  */
  Cq q = a.coeffs[0] / b;
  if (!a.is_constant ())
    return false;
  *quotient = q;
  return true;
}

/* Return true if there is some constant Q and polynomial r such that:

     (1) a = b * Q + r
     (2) 0 <= |b * Q| <= |a|
     (3) |r| < |b|

   Store the value Q in *QUOTIENT if so.  */

template<unsigned int N, typename Ca, typename Cb, typename Cq>
inline typename if_nonpoly<Cq>::bool_type
can_div_trunc_p (const poly_int_pod<N, Ca> &a,
		 const poly_int_pod<N, Cb> &b,
		 Cq *quotient)
{
  STATIC_ASSERT (N <= 2);
  if (b.is_constant ())
    return can_div_trunc_p (a, b.coeffs[0], quotient);

  /* For simplicity we only handle B that are always < 0 or always > 0
     (i.e. are ordered wrt 0).  This means that both coefficients of B
     are > 0 or both coefficients are < 0.

     If trunc (ai / bi) is the same for both cofficient indices i,
     the conditions hold with Q equal to that value.

     Sketch of a proof:

     If Q = trunc (ai / bi) then the three conditions then hold for
     ai and bi:

       (1a) ai = bi * Q + ri
       (2a) 0 <= |bi * Q| <= |ai|
       (3a) |ri| < |bi|

     where ri == ai % bi.  From this we need to derive:

       (1z) (a0 + a1 * x) = (b0 + b1 * x) * Q + (r0 + r1 * x)
       (2z) 0 <= |(b0 + b1 * x) * Q| <= |a0 + a1 * x|
       (3z) |r0 + r1 * x| < |b0 + b1 * x|

     (1z) is a simple consequence of (1a).  In general:

       (a) |y + z| <= |y| + |z|

     And if y and z are known not to have opposite signs:

       (b) |y + z| = |y| + |z|

     Since x >= 0:

       (c) A * x <= B * x  if  A < B
       (d) x = |x| (and so |A| * x = |A * x| from distribution)

     so (3a) gives:

                    |r1| * x <= |b1| * x                 (from c)
             |r0| + |r1| * x <  |b0| + |b1| * x          (combining 3a)
             |r0| + |r1 * x| <  |b0| + |b1 * x|          (from d)
             |r0| + |r1 * x| <  |b0 + b1 * x|            (from b)
       |r0 + r1 * x| <= "  " <  |b0 + b1 * x|            (from a)

     for (3z).  If Q is zero then (2z) is trivially true.  If it's nonzero
     then ai must have the same sign as bi, otherwise (1a) and (2a) cannot
     both be true.  This means that a0 and a1 cannot have opposite signs,
     since b0 and b1 don't.  Then (2b) gives (2z) from similar steps:

                  0 <= |a1 * Q| * x <= |b1| * x          (from c)
       0 <= |a0 * Q| + |a1 * Q| * x <= |b0| + |b1| * x   (combining 2a)
       0 <= |a0 * Q| + |a1 * Q * x| <= |b0| + |b1 * x|   (from d)
         0 <= |a0 * Q + a1 * Q * x| <= |b0 + b1 * x|     (from b)  */
  typedef POLY_POLY_RESULT (N, Ca, Cb)::t C;

  C q = a.coeffs[1] / b.coeffs[1];
  if (a.coeffs[0] / b.coeffs[0] != q)
    return false;

  if (!ordered_p (b, Cb (0)))
    return false;

  *quotient = q;
  return true;
}

/* Likewise, but also store r in *REMAINDER.  */

template<unsigned int N, typename Ca, typename Cb, typename Cq, typename Cr>
inline typename if_nonpoly<Cq>::bool_type
can_div_trunc_p (const poly_int_pod<N, Ca> &a,
		 const poly_int_pod<N, Cb> &b,
		 Cq *quotient, Cr *remainder)
{
  if (!can_div_trunc_p (a, b, quotient))
    return false;
  *remainder = a - *quotient * b;
  return true;
}

/* Return true if there is some polynomial q and constant R such that:

     (1) a = B * q + R
     (2) 0 <= |B * q| <= |a|
     (3) |R| < |B|

   Store the value q in *QUOTIENT if so.  */

template<unsigned int N, typename Ca, typename Cb, typename Cq>
inline typename if_nonpoly<Cb>::bool_type
can_div_trunc_p (const poly_int_pod<N, Ca> &a, Cb b,
		 poly_int_pod<N, Cq> *quotient)
{
  /* The remainder must be constant.  */
  for (unsigned int i = 1; i < N; ++i)
    if (a.coeffs[i] % b != 0)
      return false;
  for (unsigned int i = 0; i < N; ++i)
    quotient->coeffs[i] = a.coeffs[i] / b;
  return true;
}

/* Likewise, but also store R in *REMAINDER.  */

template<unsigned int N, typename Ca, typename Cb, typename Cq, typename Cr>
inline typename if_nonpoly<Cb>::bool_type
can_div_trunc_p (const poly_int_pod<N, Ca> &a, Cb b,
		 poly_int_pod<N, Cq> *quotient, Cr *remainder)
{
  if (!can_div_trunc_p (a, b, quotient))
    return false;
  *remainder = a.coeffs[0] % b;
  return true;
}

/* Return true if there is some constant Q and polynomial r such that:

     (1) a = b * Q + r
     (2) 0 <= |a| <= |b * Q|
     (3) |r| < |b|

   Store the value Q in *QUOTIENT if so.  */

template<unsigned int N, typename Ca, typename Cb, typename Cq>
inline typename if_nonpoly<Cq>::bool_type
can_div_away_from_zero_p (const poly_int_pod<N, Ca> &a,
			  const poly_int_pod<N, Cb> &b,
			  Cq *quotient)
{
  if (!can_div_trunc_p (a, b, quotient))
    return false;
  if (may_ne (*quotient * b, a))
    *quotient += (*quotient < 0 ? -1 : 1);
  return true;
}

/* Use print_dec to print VALUE to FILE, where SGN is the sign
   of the values.  */

template<unsigned int N, typename C>
void
print_dec (const poly_int_pod<N, C> &value, FILE *file, signop sgn)
{
  if (value.is_constant ())
    print_dec (value.coeffs[0], file, sgn);
  else
    {
      fprintf (file, "[");
      for (unsigned int i = 0; i < N; ++i)
	{
	  print_dec (value.coeffs[i], file, sgn);
	  fputc (i == N - 1 ? ']' : ',', file);
	}
    }
}

#undef POLY_SCALAR_RESULT
#undef SCALAR_POLY_RESULT
#undef POLY_POLY_RESULT

/* Return true if the two ranges [POS1, POS1 + SIZE1] and [POS2, POS2 + SIZE2]
   might overlap.  SIZE1 and/or SIZE2 can be the special value -1, in which
   case the range is open-ended.  */

template<typename T1, typename T2, typename T3, typename T4>
static inline bool
ranges_may_overlap_p (const T1 &pos1, const T2 &size1,
		      const T3 &pos2, const T4 &size2)
{
  /* The checks are written this way so that we can cope with signed
     offsets and unsigned sizes.  The "+ (x - x)"s avoid warnings about
     comparisons between signed and unsigned in that case.  */
  if (may_ge (pos1, pos2)
      && (must_eq (size2, (size2 - size2) - 1)
	  || may_lt (pos1 - pos2 + (size2 - size2), size2)))
    return true;
  if (may_ge (pos2, pos1)
      && (must_eq (size1, (size1 - size1) - 1)
	  || may_lt (pos2 - pos1 + (size1 - size1), size1)))
    return true;

  return false;
}

/* Return true if range [POS1, POS1 + SIZE1) is known to be a subrange of
   [POS2, POS2 + SIZE2).  SIZE1 and/or SIZE2 can be the special value -1,
   in which case the range is open-ended.  */

template<typename T1, typename T2, typename T3, typename T4>
static inline bool
known_subrange_p (const T1 &pos1, const T2 &size1,
		  const T3 &pos2, const T4 &size2)
{
  return (may_ne (size1, (size1 - size1) - 1)
	  && may_ne (size2, (size2 - size2) - 1)
	  && must_ge (pos1, pos2)
	  && must_le (pos1 - pos2 + size1 + (size2 - size2), size2));
}

/* Return true if range [POS, POS + SIZE) is known to include VAL.
   SIZE can be the special value -1, in which case the range is
   open-ended.  */

template<typename T1, typename T2, typename T3>
static inline bool
known_in_range_p (const T1 &val, const T2 &pos, const T3 &size)
{
  return (may_ne (size, (size - size) - 1)
	  && must_ge (val, pos)
	  && must_lt (val - pos + (size - size), size));
}

/* Return true if range [POS, POS + SIZE) might include VAL.
   SIZE can be the special value -1, in which case the range is
   open-ended.  */

template<typename T1, typename T2, typename T3>
static inline bool
maybe_in_range_p (const T1 &val, const T2 &pos, const T3 &size)
{
  return (must_eq (size, (size - size) - 1)
	  || (may_ge (val, pos) && may_lt (val, pos + size)));
}

template<unsigned int N, typename C>
void
gt_ggc_mx (poly_int_pod<N, C> *)
{
}

template<unsigned int N, typename C>
void
gt_pch_nx (poly_int_pod<N, C> *)
{
}

template<unsigned int N, typename C>
void
gt_pch_nx (poly_int_pod<N, C> *, void (*) (void *, void *), void *)
{
}

#endif
