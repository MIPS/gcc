/* Header file for range analysis.
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

#ifndef GCC_RANGE_H
#define GCC_RANGE_H
#define MAX_RANGES	6

typedef class irange *irange_p;
enum irange_type { RANGE_PLAIN, RANGE_INVERT };

class GTY(()) irange
{
 private:
  bool overflow;
  size_t n;
  void prepend (wide_int x, wide_int y);
  void append (wide_int x, wide_int y);
  void remove (unsigned i, unsigned j);
  void canonicalize ();
  /* This is stupid.  These two should be private, but the GTY
     machinery can't look inside an irange.  */
 public:
  tree type;
  wide_int bounds[MAX_RANGES];

public:
  irange () { type = NULL_TREE; n = 0; }
  irange (tree t);
  irange (tree t, wide_int lbound, wide_int ubound,
	  irange_type rt = RANGE_PLAIN);
  irange (const irange &r);

  /* Convenience functions to simulate old range_info_def.  */
  enum value_range_type get_simple_min_max (wide_int *, wide_int *);
  bool anti_range_p (void);

  void set_range (tree t);
  void set_range (tree t, wide_int lbound, wide_int ubound,
		  irange_type rt = RANGE_PLAIN);

  bool overflow_p () { return overflow && !TYPE_OVERFLOW_WRAPS (type); }
  void set_overflow () { overflow = true; }
  void clear_overflow () { overflow = false; }

  unsigned num_ranges () { return n / 2; }
  wide_int lbound () const { return bounds[0]; }
  wide_int lbound (unsigned index);
  wide_int ubound () const { return bounds[n - 1]; }
  wide_int ubound (unsigned index);

  void clear () { n = 0; }
  void clear (tree t) { type = t; n = 0; overflow = false; }
  bool empty_p () const { return !n; }
  bool range_for_type_p () const;
  bool simple_range_p () const { return n == 2; }

  void dump (FILE *f = stderr);

  bool valid_p ();
  bool cast (tree type);
  bool contains (wide_int element);

  tree get_type () { return type; }

  irange& operator= (const irange &r);
  irange& operator= (tree t);

  bool operator== (const irange &r) const;
  bool operator!= (const irange &r) const { return !(*this == r); }

  void Union (wide_int x, wide_int y);
  bool Union (const irange &r);
  bool Union (const irange &r1, const irange &r2);

  void Intersect (wide_int x, wide_int y);
  bool Intersect (const irange &r);
  bool Intersect (const irange &r1, const irange &r2);

  bool Not ();
  bool Not (const irange& r);
};

bool range_zero (irange_p r, tree type);
bool range_one (irange_p r, tree type);
bool range_non_zero (irange_p r, tree type);

#endif /* GCC_RANGE_H */
