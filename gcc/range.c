/* SSA range analysis implementation.
   Copyright (C) 2017 Free Software Foundation, Inc.
   Contributed by Aldy Hernandez <aldyh@redhat.com>
   and Andrew Macleod <amacleod@redhat.com>.

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "tree.h"
#include "gimple.h"
#include "gimple-pretty-print.h"
#include "fold-const.h"
#include "ssa.h"
#include "range.h"
#include "selftest.h"


void
irange::set_range (tree t, wide_int lbound, wide_int ubound,
		      irange_type rt)
{
  gcc_assert (INTEGRAL_TYPE_P (t) || POINTER_TYPE_P (t));
  gcc_assert (TYPE_PRECISION (t) == lbound.get_precision ());
  gcc_assert (lbound.get_precision () == ubound.get_precision ());
  overflow = false;
  type = t;
  gcc_assert (wi::le_p (lbound, ubound, TYPE_SIGN (type)));
  if (rt == RANGE_INVERT)
    {
      bool ovf;
      n = 0;
      // INVERSE([I,J]) is [-MIN, I-1][J+1, +MAX]
      bounds[n++] = wi::min_value (TYPE_PRECISION (type), TYPE_SIGN (type));
      bounds[n++] = wi::sub (lbound, 1, TYPE_SIGN (type), &ovf);
      if (ovf)
	n = 0;
      bounds[n++] = wi::add (ubound, 1, TYPE_SIGN (type), &ovf);
      if (ovf)
	n--;
      else
	bounds[n++] = wi::max_value (TYPE_PRECISION (type), TYPE_SIGN (type));

      // If we get here with N==0, it means we tried to calculate the
      // inverse of [-MIN, +MAX] which is actually the empty set, and
      // N==0 maps nicely to the empty set :).
    }
  else
    {
      n = 2;
      bounds[0] = lbound;
      bounds[1] = ubound;
    }
}

void
irange::set_range (tree t)
{
  if (TYPE_P (t))
    {
      gcc_assert (INTEGRAL_TYPE_P (t) || POINTER_TYPE_P (t));
      wide_int min = wi::min_value (TYPE_PRECISION (t), TYPE_SIGN (t));
      wide_int max = wi::max_value (TYPE_PRECISION (t), TYPE_SIGN (t));
      set_range (t, min, max);
      return;
    }

  // Otherwise process as an SSA_NAME
  gcc_assert (TREE_CODE (t) == SSA_NAME);

  // Until tree-ssanames.c::get_range_info () supports pointer types....
  if (POINTER_TYPE_P (TREE_TYPE (t)))
    {
      set_range (TREE_TYPE (t));
      return;
    }
    
  wide_int min, max;
  value_range_type rtype = get_range_info (t, &min, &max);

  switch (rtype)
    {
    case VR_UNDEFINED:
    case VR_VARYING:
      set_range (TREE_TYPE (t));
      break;
    case VR_RANGE:
      set_range (TREE_TYPE (t), min, max, RANGE_PLAIN);
      break;
    case VR_ANTI_RANGE:
      set_range (TREE_TYPE (t), min, max, RANGE_INVERT);
      break;
    default:
      gcc_unreachable ();
      break;
    }
}

irange::irange (tree t, wide_int lbound, wide_int ubound,
		      irange_type rt)
{
  set_range (t, lbound, ubound, rt);
}

irange::irange (const irange &r)
{
  type = r.type;
  overflow = false;
  n = r.n;
  for (unsigned i = 0; i < n; ++i)
    bounds[i] = r.bounds[i];
}

// Constructor using the traditional GCC range info if available.  If
// no range info is available, use the range for the domain for the
// underlying type.

irange::irange (tree t)
{
  set_range (t);
}

bool
irange::operator== (const irange &r) const
{
  if (type != r.type || n != r.n || overflow != r.overflow)
    return false;
  for (unsigned i = 0; i < n; ++i)
    if (bounds[i] != r.bounds[i])
      return false;
  return true;
}

irange&
irange::operator= (const irange &r)
{
  type = r.type;
  n = r.n;
  overflow = r.overflow;
  for (unsigned i = 0; i < n; ++i)
    bounds[i] = r.bounds[i];
  return *this;
}


irange&
irange::operator= (tree t)
{
  set_range (t);
  return *this;
}

// Return true if this range is the full range for it's type
bool
irange::range_for_type_p () const
{
  irange tmp (type);
  return (*this == tmp);
}


bool
irange::valid_p ()
{
  if (!n)
    return false;
  if (n % 2)
    return false;
  if (n > MAX_RANGES)
    return false;
  // Check that the bounds are in the right order.
  //
  // So for [a,b][c,d][e,f] we must have:
  // a <= b < c <= d < e <= f
  if (wi::gt_p (bounds[0], bounds[1], TYPE_SIGN (type)))
    return false;
  for (unsigned i = 2; i < n; i += 2)
    {
      if (wi::le_p (bounds[i], bounds[i-1], TYPE_SIGN (type)))
	return false;
      if (wi::gt_p (bounds[i], bounds[i+1], TYPE_SIGN (type)))
	return false;
    }
  return true;
}

// Convert the current range in place into a range of type NEW_TYPE,
// and return TRUE if successful.  If the current range does not fit
// in the new range, return FALSE and convert the range to the maximum
// range for the new type.
//
// The type of the original range is changed to the new type.
//
// Converting between types of different signs is a hard failure.

bool
irange::cast(tree new_type)
{
  if (TYPE_SIGN (new_type) != TYPE_SIGN (type))
    return false;

  // Converting to a higher precision is easy because things always fit.
  unsigned new_precision = TYPE_PRECISION (new_type);
  if (TYPE_PRECISION (type) <= TYPE_PRECISION (new_type))
    {
      for (unsigned i = 0; i < n; ++i)
	bounds[i] = wide_int::from (bounds[i], new_precision,
				    TYPE_SIGN (type));
      type = new_type;
      return true;
    }

  // Get the extreme bounds for the new type, but within the old type,
  // so we can properly compare them.
  irange new_bounds (new_type);
  wide_int lbound = wide_int_to_tree (type, new_bounds.lbound ());
  wide_int ubound = wide_int_to_tree (type, new_bounds.ubound ());

  // If any of the old bounds are outside of the representable range
  // for the new type, conservatively default to the entire range of
  // the new type.
  if (wi::lt_p (bounds[0], lbound, TYPE_SIGN (type))
      || wi::gt_p (bounds[n - 1], ubound, TYPE_SIGN (type)))
    {
      bounds[0] = wide_int::from (lbound, new_precision, TYPE_SIGN (type));
      bounds[1] = wide_int::from (ubound, new_precision, TYPE_SIGN (type));
      n = 2;
      return false;
    }

  for (unsigned i = 0; i < n; ++i)
    bounds[i] = wide_int::from (bounds[i], new_precision, TYPE_SIGN (type));
  type = new_type;
  gcc_assert (valid_p ());
  return true;
}

// Return TRUE if the current range contains ELEMENT.

bool
irange::contains (wide_int element)
{
  for (unsigned i = 0; i < n; i += 2)
    if (wi::ge_p (element, bounds[i], TYPE_SIGN (type))
	&& wi::le_p (element, bounds[i + 1], TYPE_SIGN (type)))
      return true;
  return false;
}

// Canonicalize and clean up the current range.  Ideally we shouldn't
// need this function, and any cleanups should be done in place where
// the range was altered.

void
irange::canonicalize ()
{
  if (n < 2)
    return;

  // Merge any edges that touch.
  // [9,10][11,20] => [9,20]
  for (unsigned i = 1; i < n - 2; i += 2)
    {
      bool ovf;
      wide_int x = wi::add (bounds[i], 1, TYPE_SIGN (type), &ovf);
      if (ovf)
	continue;
      if (x == bounds[i + 1])
	{
	  bounds[i] = bounds[i + 2];
	  remove (i + 1, i + 2);
	}
    }
}

// Prepend [X,Y] into THIS.

void
irange::prepend (wide_int x, wide_int y)
{
  // If we have enough space, shift everything to the right and prepend.
  if (n < MAX_RANGES)
    {
      for (unsigned i = n; i; i -= 2)
	{
	  bounds[i] = bounds[i - 2];
	  bounds[i + 1] = bounds[i - 1];
	}
      bounds[0] = x;
      bounds[1] = y;
      n += 2;
    }
  // Otherwise, merge it with the first entry.
  else
    bounds[0] = x;
  canonicalize ();
}

// Place [X,Y] at the end of THIS.

void
irange::append (wide_int x, wide_int y)
{
  // If we have enough space, make space at the end and append.
  if (n < MAX_RANGES)
    {
      bounds[n++] = x;
      bounds[n++] = y;
    }
  // Otherwise, merge it with the last entry.
  else
    bounds[n - 1] = y;
  canonicalize ();
}

// Remove the bound entries from [i..j].

void
irange::remove (unsigned i, unsigned j)
{
  if (i >= n || i > j)
    return;
  gcc_assert (i <= j);
  unsigned dst = i;
  unsigned ndeleted = j - i + 1;
  for (++j; j < n; ++j)
    bounds[dst++] = bounds[j];
  n -= ndeleted;
}

// THIS = THIS U [X,Y]

void
irange::Union (wide_int x, wide_int y)
{
  if (empty_p ())
    {
      bounds[0] = x;
      bounds[1] = y;
      n = 2;
      return;
    }

  // If [X,Y] comes before, put it at the front.
  if (wi::lt_p (y, bounds[0], TYPE_SIGN (type)))
    {
      prepend (x, y);
      return;
    }
  // If [X,Y] comes after, put it at the end.
  if (wi::gt_p (x, bounds[n - 1], TYPE_SIGN (type)))
    {
      append (x, y);
      return;
    }
  // Handle [X,Y] swalling up all of THIS.
  if (wi::le_p (x, bounds[0], TYPE_SIGN (type))
      && wi::ge_p (y, bounds[n - 1], TYPE_SIGN (type)))
    {
      bounds[0] = x;
      bounds[1] = y;
      n = 2;
      return;
    }
  // Handle X starting before, while Y is within.
  //              Y
  // X[a,b][c,d][e,f][g,h][i,j]
  // ==> [X,Y][g,h][i,j]
  if (wi::lt_p (x, bounds[0], TYPE_SIGN (type)))
    {
      bounds[0] = x;
      for (unsigned i = 1; i < n; i += 2)
	if (wi::le_p (y, bounds[i], TYPE_SIGN (type)))
	  {
	    if (y == bounds[i])
	      bounds[1] = y;
	    else
	      bounds[1] = bounds[i];
	    remove (2, i);
	    return;
	  }
      gcc_unreachable ();
    }
  // Handle Y being outside, while X is within.
  //              X           Y
  // [a,b][c,d][e,f][g,h][i,j]
  // ==> [a,b][c,d][e,Y]
  if (wi::gt_p (y, bounds[n - 1], TYPE_SIGN (type)))
    {
      //gcc_assert (n > 2);
      for (unsigned i = 0; i < n; i += 2)
	if (wi::ge_p (bounds[i + 1], x, TYPE_SIGN (type)))
	  {
	    bounds[i + 1] = y;
	    n = i + 2;
	    return;
	  }
      gcc_unreachable ();
    }

  // At this point, [X,Y] must be completely inside.
  //  X           Y
  // [a,b][c,d][e,f][g,h]
  gcc_assert (wi::ge_p (x, bounds[0], TYPE_SIGN (type))
	      && wi::le_p (y, bounds[n - 1], TYPE_SIGN (type)));

  // Find X.
  gcc_assert (n >= 2);
  unsigned xpos = ~0U;
  unsigned i = n;
  do
    {
      i -= 2;
      if (wi::ge_p (x, bounds[i], TYPE_SIGN (type)))
	{
	  xpos = i;
	  break;
	}
    }
  while (i);
  gcc_assert (xpos != ~0U);

  // Find Y.
  unsigned ypos = ~0U;
  for (i = 1; i < n; i += 2)
    if (wi::le_p (y, bounds[i], TYPE_SIGN (type)))
      {
	ypos = i;
	break;
      }
  gcc_assert (ypos != ~0U);

  // Merge the sub-ranges in between xpos and ypos.
  y = bounds[ypos];
  remove (xpos + 2, ypos);
  bounds[xpos + 1] = y;
}

// THIS = R1 U R2

bool
irange::Union (const irange &r1, const irange &r2)
{
  gcc_assert (r1.type == r2.type);

  if (r1.empty_p ())
    {
      *this = r2;
      return true;
    }
  else if (r2.empty_p ())
    {
      *this = r1;
      return true;
    }

  *this = r1;
  for (unsigned i = 0; i < r2.n; i += 2)
    Union (r2.bounds[i], r2.bounds[i + 1]);

  overflow |= r1.overflow;
  return true;
}

// THIS = THIS U R

bool
irange::Union (const irange &r)
{
  if (*this == r)
    return true;
  return Union (*this, r);
}

// THIS = THIS ^ [X,Y]

void
irange::Intersect (wide_int x, wide_int y)
{
  unsigned pos = 0;

  for (unsigned i = 0; i < n; i += 2)
    {
      wide_int newlo = wi::max (bounds[i], x, TYPE_SIGN (type));
      wide_int newhi = wi::min (bounds[i + 1], y, TYPE_SIGN (type));
      if (wi::gt_p (newlo, newhi, TYPE_SIGN (type)))
	{
	  // If the new sub-range doesn't make sense, it's an
	  // impossible range and must be kept out of the result.
	}
      else
	{
	  bounds[pos++] = newlo;
	  bounds[pos++] = newhi;
	}
    }
  n = pos;
}

// THIS = R1 ^ R2

bool
irange::Intersect (const irange &r1, const irange &r2)
{
  gcc_assert (r1.type == r2.type);

  clear (r1.type);

  // Intertsection with an empty range is an empty range.
  if (r1.empty_p () || r2.empty_p ())
    return true;

  // The general algorithm is as follows.
  //
  // Intersect each sub-range of R2 with all of R1 one at a time, and
  // join/union the results of these intersections together.  I.e:
  //
  // [10,20][30,40][50,60] ^ [15,25][38,51][55,70]
  //
  // Step 1: [10,20][30,40][50,60] ^ [15,25] => [15,20]
  // Step 2: [10,20][30,40][50,60] ^ [38,51] => [38,40]
  // Step 3: [10,20][30,40][50,60] ^ [55,70] => [55,60]
  // Final:  [15,20] U [38,40] U [55,60] => [15,20][38,40][55,60]

  // FIXME: Eventually this needs to be rewritten more efficiently.
  // We should probably accumulate into a new range instead of joining
  // at each step, and stop making a copy of R1 at every step.

  for (unsigned i = 0; i < r2.n; i += 2)
    {
      irange chunk (r1);
      chunk.Intersect (r2.bounds[i], r2.bounds[i + 1]);
      Union (chunk);
    }

  // Overflow is sticky only if both ranges overflowed.
  overflow = (r1.overflow && r2.overflow);
  return true;
}

// THIS = THIS ^ R

bool
irange::Intersect (const irange &r)
{
  irange q = *this;
  return Intersect (q, r);
}

// THIS = NOT(R).

bool
irange::Not (const irange& r)
{
  type = r.type;
  overflow = r.overflow;

  // We always need one more set of bounds to represent a NOT, so if
  // we're at the limit, we can't properly represent things.
  //
  // For instance, to represent a NOT of a 2 sub-range set
  // [5, 10][20, 30], we would need a 3 sub-range set
  // [-MIN, 4][11, 19][31, MAX].
  //
  // In this case convert the current range to something more
  // conservative, and return the NOT of that.
  if (r.n >= MAX_RANGES)
    {
      bounds[0] = r.bounds[0];
      bounds[1] = r.bounds[n - 1];
      n = 2;
      return Not ();
    }

  // The inverse of the empty set is the entire domain.
  if (r.empty_p ())
    {
      set_range (type);
      return false;
    }

  // The algorithm is as follows.  To calculate NOT ([a,b][c,d]), we
  // generate [-MIN, a-1][b+1, c-1][d+1, MAX].
  //
  // If there is an over/underflow in the calculation for any
  // sub-range, we eliminate that subrange.  This allows us to easily
  // calculate NOT([-MIN, 5]) with: [-MIN, -MIN-1][6, MAX].  And since
  // we eliminate the underflow, only [6, MAX] remains.

  unsigned i = 0;
  bool ovf;

  // Construct leftmost range.
  n = 0;
  bounds[n++] = wi::min_value (TYPE_PRECISION (type), TYPE_SIGN (type));
  bounds[n++] = wi::sub (r.bounds[i++], 1, TYPE_SIGN (type), &ovf);
  if (ovf)
    n = 0;
  // Construct middle ranges if applicable.
  if (r.n > 2)
    {
      unsigned j = i;
      for (; j < r.n - 2; j += 2)
	{
	  bounds[n++] = wi::add (r.bounds[j], 1, TYPE_SIGN (type), &ovf);
	  bounds[n++] = wi::sub (r.bounds[j + 1], 1, TYPE_SIGN (type), &ovf);
	  if (ovf)
	    n -= 2;
	}
      i = j;
    }
  // Construct rightmost range.
  bounds[n++] = wi::add (r.bounds[i], 1, TYPE_SIGN (type), &ovf);
  bounds[n++] = wi::max_value (TYPE_PRECISION (type), TYPE_SIGN (type));
  if (ovf)
    n -= 2;

  return n != 0;
}

// THIS = Not (THIS).

bool
irange::Not ()
{
  irange tmp (*this);
  return Not (tmp);
}

wide_int
irange::lbound (unsigned index)
{
  gcc_assert (n != 0 && index <= n/2);
  return bounds[index * 2];
}

wide_int
irange::ubound (unsigned index)
{
  gcc_assert (n != 0 && index <= n/2);
  return bounds[index * 2 + 1];
}

void
irange::dump (FILE *f)
{
  if (!n)
    fprintf (f, "[]");
  else
    for (unsigned i = 0; i < n; ++i)
      {
	if (i % 2 == 0)
	  fputc ('[', f);
	tree x = wide_int_to_tree (type, bounds[i]);
	print_generic_expr (f, x, 0);
	if (i % 2 == 0)
	  fprintf (f, ",");
	else
	  fprintf (f, "]");
      }
  if (overflow)
    fprintf (f, "(ov)");
}

/* Return TRUE if the current range is an ANTI_RANGE.  This is a
   temporary measure offering backward compatibility with
   range_info_def, and will go away.  */

bool
irange::anti_range_p (void)
{
  // Remember: VR_ANTI_RANGE([3,10]) ==> [-MIN,2][11,MAX]
  unsigned int precision = TYPE_PRECISION (type);
  wide_int min = wi::min_value (precision, TYPE_SIGN (type));
  wide_int max = wi::max_value (precision, TYPE_SIGN (type));
  return (n == 4
	  && bounds[0] == min
	  && bounds[3] == max);
}

/* Convert the current range into a simple min/max.  This is a
   temporary measure while we remove all uses of range_info_def.

   Returns VR_RANGE or VR_ANTI_RANGE depending on whether it is a
   regular range, or an inverse range.  */

enum value_range_type
irange::get_simple_min_max (wide_int *min, wide_int *max)
{
  gcc_assert (!overflow); // FIXME: Maybe figure this out later.

  if (anti_range_p ())
    {
      irange tmp (*this);
      tmp.Not ();
      gcc_assert (tmp.n == 2);
      gcc_assert (!tmp.overflow);
      *min = tmp.bounds[0];
      *max = tmp.bounds[1];
      return VR_ANTI_RANGE;
    }

  /* We chop off any middle ranges, because old school range_info_def
     has no use for such granularity.  */
  *min = bounds[0];
  *max = bounds[n - 1];
  return VR_RANGE;
}

bool
make_irange (irange_p result, tree lb, tree ub, tree type)
{
  irange r (TREE_TYPE (lb), lb, ub);
  *result = r;
  if (result->valid_p ())
    {
      if (type)
        result->cast (type);
      return true;
    }
  return false;
}

bool
make_irange_not (irange_p result, tree not_exp, tree type)
{
  irange r (TREE_TYPE (not_exp), not_exp, not_exp, RANGE_INVERT);
  *result = r;
  if (result->valid_p ())
    {
      if (type)
        result->cast (type);
      return true;
    }
  return false;
}


bool
range_one (irange_p r, tree type)
{
  tree one = build_int_cst (type, 1);
  r->set_range (type, one, one);
  return true;
}

bool
range_zero (irange_p r, tree type)
{
  tree zero = build_int_cst (type, 0);
  r->set_range (type, zero, zero);
  return true;
}

bool
range_non_zero (irange_p r, tree type)
{
  tree zero = build_int_cst (type, 0);
  return make_irange_not (r, zero, type);
}

#ifdef CHECKING_P
namespace selftest {


#define INT(N) build_int_cst (integer_type_node, (N))
#define RANGE1(A,B) irange (integer_type_node, INT(A), INT(B))

#define RANGE2(A,B,C,D)					\
( i1 = irange (integer_type_node, INT (A), INT (B)),	\
  i2 = irange (integer_type_node, INT (C), INT (D)),	\
  i1.Union (i2),					\
  i1 )

#define RANGE3(A,B,C,D,E,F) 				\
( i1 = irange (integer_type_node, INT (A), INT (B)),	\
  i2 = irange (integer_type_node, INT (C), INT (D)),	\
  i3 = irange (integer_type_node, INT (E), INT (F)),	\
  i1.Union (i2),					\
  i1.Union (i3),					\
  i1 )

#define SHORT_INT(N) build_int_cst (short_integer_type_node, (N))
#define UCHAR_INT(N) build_int_cst (unsigned_char_type_node, (N))

// Run all of the selftests within this file.

void
irange_tests ()
{
  irange i1, i2, i3;
  irange r0, r1;
  ASSERT_FALSE (r0.valid_p ());

  // Test that NOT(255) is [0..254] in 8-bit land.
  irange not_255;
  make_irange_not (&not_255, UCHAR_INT(255), unsigned_char_type_node);
  ASSERT_TRUE (not_255 == irange (unsigned_char_type_node, UCHAR_INT(0), UCHAR_INT(254)));

  // Test that NOT(0) is [1..255] in 8-bit land.
  irange not_zero;
  range_non_zero (&not_zero, unsigned_char_type_node);
  ASSERT_TRUE (not_zero == irange (unsigned_char_type_node, UCHAR_INT(1), UCHAR_INT(255)));

  r0.set_range (integer_type_node);
  tree minint = wide_int_to_tree (integer_type_node, r0.lbound ());
  tree maxint = wide_int_to_tree (integer_type_node, r0.ubound ());

  r0.set_range (short_integer_type_node);
  tree minshort = wide_int_to_tree (short_integer_type_node, r0.lbound ());
  tree maxshort = wide_int_to_tree (short_integer_type_node, r0.ubound ());

  // Check that [~5] is really [-MIN,4][6,MAX]
  r0 = irange (integer_type_node, INT(5), INT(5), RANGE_INVERT);
  r1 = irange (integer_type_node, minint, INT(4));
  ASSERT_TRUE (r1.Union (irange (integer_type_node, INT(6), maxint)));

  ASSERT_TRUE (r0 == r1);

  r1.set_range (integer_type_node, INT(5), INT(5));
  ASSERT_TRUE (r1.valid_p ());
  irange r2 (r1);
  ASSERT_TRUE (r1 == r2);

  r1 = RANGE1 (5, 10);
  ASSERT_TRUE (r1.valid_p ());

  r1 = irange (integer_type_node, (wide_int) INT(5), (wide_int) INT(10));
  ASSERT_TRUE (r1.valid_p ());
  ASSERT_TRUE (r1.contains (INT (7)));

  // If a range is in any way is outside of the range for the
  // converted to range, default to the range for the new type.
  r1 = irange (integer_type_node, integer_zero_node, maxint);
  ASSERT_FALSE (r1.cast (short_integer_type_node));
  ASSERT_TRUE (r1.lbound () == minshort && r1.ubound() == maxshort);

  // NOT([10,20]) ==> [-MIN,9][21,MAX]
  r0 = r1 = irange (integer_type_node, INT(10), INT(20));
  r2 = irange (integer_type_node, minint, INT(9));
  ASSERT_TRUE (r2.Union (irange (integer_type_node, INT(21), maxint)));
  ASSERT_TRUE (r1.Not ());
  ASSERT_TRUE (r1 == r2);
  // Test that NOT(NOT(x)) == x.
  ASSERT_TRUE (r2.Not ());
  ASSERT_TRUE (r0 == r2);

  // NOT(-MIN,+MAX) is the empty set and should return false.
  r0 = irange (integer_type_node, wide_int_to_tree (integer_type_node, minint),
		  wide_int_to_tree (integer_type_node, maxint));
  ASSERT_FALSE (r0.Not ());
  r1.clear ();
  ASSERT_TRUE (r0 == r1);

  // Test that booleans and their inverse work as expected.
  range_zero (&r0, boolean_type_node);
  ASSERT_TRUE (r0 == irange (boolean_type_node,
			     build_int_cst (boolean_type_node, 0),
			     build_int_cst (boolean_type_node, 0)));
  r0.Not();
  ASSERT_TRUE (r0 == irange (boolean_type_node,
			     build_int_cst (boolean_type_node, 1),
			     build_int_cst (boolean_type_node, 1)));

  // ([10,20] U [5,8]) U [1,3] ==> [1,3][5,8][10,20]
  r0 = RANGE1 (10, 20);
  r1 = RANGE1 (5, 8);
  r0.Union (r1);
  r1 = RANGE1 (1, 3);
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE3 (1, 3, 5, 8, 10, 20));

  // [1,3][5,8][10,20] U [-5,0] => [-5,3][5,8][10,20]
  r1 = irange (integer_type_node, INT(-5), INT(0));
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE3 (-5, 3, 5, 8, 10, 20));

  // [10,20] U [30,40] ==> [10,20][30,40]
  r0 = irange (integer_type_node, INT(10), INT(20));
  r1 = irange (integer_type_node, INT(30), INT(40));
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE2 (10, 20, 30, 40));
  // [10,20][30,40] U [50,60] ==> [10,20][30,40][50,60]
  r1 = irange (integer_type_node, INT(50), INT(60));
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE3 (10, 20, 30, 40, 50, 60));
  // [10,20][30,40][50,60] U [70, 80] ==> [10,20][30,40][50,80]
  r1 = irange (integer_type_node, INT(70), INT(80));
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE3 (10, 20, 30, 40, 50, 80));

  // Make sure NULL and non-NULL of pointer types work, and that
  // inverses of them are consistent.
  tree voidp = build_pointer_type (void_type_node);
  range_zero (&r0, voidp);
  r1 = r0;
  r0.Not();
  r0.Not ();
  ASSERT_TRUE (r0 == r1);

  // [10,20][30,40][50,60] U [6,35] => [6,40][50,60]
  r0 = RANGE3 (10, 20, 30, 40, 50, 60);
  r1 = RANGE1 (6, 35);
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE2 (6,40,50,60));

  r0 = RANGE3 (10, 20, 30, 40, 50, 60);
  r1 = RANGE1 (6, 60);
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE1 (6,60));

  // [10,20][30,40][50,60] U [6,70] => [6,70]
  r0 = RANGE3 (10, 20, 30, 40, 50, 60);
  r1 = RANGE1 (6, 70);
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE1 (6,70));

  // [10,20][30,40][50,60] U [35,70] => [10,20][30,70]
  r0 = RANGE3 (10,20,30,40,50,60);
  r1 = RANGE1 (35,70);
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE2 (10,20,30,70));

  // [10,20][30,40] U [25,70] => [10,70]
  r0 = RANGE2 (10,20,30,40);
  r1 = RANGE1 (25,70);
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE2 (10,20,30,70));

  // [10,20][30,40][50,60] U [15,35] => [10,40][50,60]
  r0 = RANGE3 (10,20,30,40,50,60);
  r1 = RANGE1 (15,35);
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE2 (10,40,50,60));

  // [10,20] U [15, 30] => [10, 30]
  r0 = RANGE1 (10,20);
  r1 = RANGE1 (15,30);
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE1 (10,30));

  // [10,20] U [25,25] => [10,20][25,25]
  r0 = RANGE1 (10,20);
  r1 = RANGE1 (25,25);
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE2 (10,20,25,25));

  // [10,20][30,40][50,60] U [35,35] => [10,20][30,40][50,60]
  r0 = RANGE3 (10,20,30,40,50,60);
  r1 = RANGE1 (35,35);
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE3 (10,20,30,40,50,60));

  // [15,40] U [] => [15,40]
  r0 = RANGE1 (15,40);
  r1.clear ();
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE1 (15,40));

  // [10,20] U [10,10] => [10,20]
  r0 = RANGE1 (10,20);
  r1 = RANGE1 (10,10);
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE1 (10,20));

  // [10,20] U [9,9] => [9,20]
  r0 = RANGE1 (10,20);
  r1 = RANGE1 (9,9);
  r0.Union (r1);
  ASSERT_TRUE (r0 == RANGE1 (9,20));

  // [10,10][12,12][20,100] ^ [15,200]
  r0 = RANGE3 (10,10,12,12,20,100);
  r1 = RANGE1 (15,200);
  r0.Intersect (r1);
  ASSERT_TRUE (r0 == RANGE1 (20,100));

  // [10,20][30,40][50,60] ^ [15,25][38,51][55,70] => [15,20][38,40][50,60]
  r0 = RANGE3 (10,20,30,40,50,60);
  r1 = RANGE3 (15,25,38,51,55,70);
  r0.Intersect (r1);
  ASSERT_TRUE (r0 == RANGE3 (15,20,38,40,50,60));

  // [15,20][30,40][50,60] ^ [15,35][40,90][100,200] => [15,20][30,35][40,60]
  r0 = RANGE3 (15,20,30,40,50,60);
  r1 = RANGE3 (15,35,40,90,100,200);
  r0.Intersect (r1);
  ASSERT_TRUE (r0 == RANGE3 (15,20,30,35,40,60));

  // [10,20][30,40] U [40,50] => [40,40]
  r0 = RANGE2 (10,20,30,40);
  r1 = RANGE1 (40,50);
  r0.Intersect (r1);
  ASSERT_TRUE (r0 == RANGE1 (40,40));
}

} // namespace selftest
#endif // CHECKING_P
