/* Code for range operators.
   Copyright (C) 2017 Free Software Foundation, Inc.
   Contributed by Andrew MacLeod <amacleod@redhat.com>.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "insn-codes.h"
#include "rtl.h"
#include "tree.h"
#include "gimple.h"
#include "cfghooks.h"
#include "tree-pass.h"
#include "ssa.h"
#include "optabs-tree.h"
#include "gimple-pretty-print.h"
#include "diagnostic-core.h"
#include "flags.h"
#include "fold-const.h"
#include "stor-layout.h"
#include "calls.h"
#include "cfganal.h"
#include "gimple-fold.h"
#include "tree-eh.h"
#include "gimple-iterator.h"
#include "gimple-walk.h"
#include "tree-cfg.h"
#include "wide-int.h"
#include "range.h"
#include "range-op.h"

inline wide_int
max_limit (tree type)
{
  return wi::max_value (TYPE_PRECISION (type) , TYPE_SIGN (type));
}

inline wide_int
min_limit (tree type)
{
  return wi::min_value (TYPE_PRECISION (type) , TYPE_SIGN (type));
}

static void
set_boolean_range (irange& r)
{
  wide_int lb = wi::uhwi (0, 1);
  wide_int ub = wi::uhwi (1, 1);
  r.set_range (boolean_type_node, lb, ub);
}

static void
set_boolean_range_zero (irange& r)
{
  wide_int lb = wi::uhwi (0, 1);
  wide_int ub = wi::uhwi (0, 1);
  r.set_range (boolean_type_node, lb, ub);
}

static void
set_boolean_range_one (irange &r)
{
  wide_int lb = wi::uhwi (1, 1);
  wide_int ub = wi::uhwi (1, 1);

  r.set_range (boolean_type_node, lb, ub);
}


/* Given newly calcuclated lbound and ubound, examine the overflow bits to 
   determine where the various ranges belong.  */
static void
add_to_range (irange& r, wide_int lb, bool ov_lb, wide_int ub, bool ov_ub)
{
  if (ov_lb)
    {
      if (ov_ub)
        {
	  /* Both overflow, so result is all overflow.  */
	  if (TYPE_OVERFLOW_WRAPS (r.get_type ()))
	    r.Union (lb, ub);
	}
      else
	{
	  /* lbound overflow, ubound doesn't, must be underflow.  */
	  r.Union (min_limit (r.get_type ()), ub);
	  if (TYPE_OVERFLOW_WRAPS (r.get_type ()))
	    r.Union (lb, max_limit (r.get_type ()));
	}
      r.set_overflow ();
    }
  else
    if (ov_ub)
      {
	/* Ubound overflow, lboudn doesnt, must be an overflow.  */
	r.Union (lb, max_limit (r.get_type ()));
	if (TYPE_OVERFLOW_WRAPS (r.get_type ()))
	  r.Union (min_limit (r.get_type ()), ub);
	r.set_overflow ();
      }
    else
      r.Union (lb, ub);
}

/* Handle the first addition to a range while handling the overflow bits.  */
static void
set_range (tree type, irange& r, wide_int lb, bool ov_lb, wide_int ub,
	   bool ov_ub)
{
  r.clear (type);
  add_to_range (r, lb, ov_lb, ub, ov_ub);
}

/*  ------------------------------------------------------------------------  */

/* Defaults for all remaining operations are to return NULL. This means no
   additional range info is available beyond that of the type.  */


bool
irange_operator::fold_range (irange& r ATTRIBUTE_UNUSED,
				irange& lh ATTRIBUTE_UNUSED,
				irange& rh ATTRIBUTE_UNUSED)
{
  return false;
}


bool
irange_operator::op1_adjust (irange& r ATTRIBUTE_UNUSED,
				irange& val ATTRIBUTE_UNUSED,
				sro_truth truth ATTRIBUTE_UNUSED)
{
  return false;
}

bool
irange_operator::op1_range (irange& r ATTRIBUTE_UNUSED,
			       irange& val ATTRIBUTE_UNUSED,
			       sro_truth truth ATTRIBUTE_UNUSED)
{
  return false;
}


bool
irange_operator::op2_adjust (irange& r ATTRIBUTE_UNUSED,
				irange& val ATTRIBUTE_UNUSED,
				sro_truth truth ATTRIBUTE_UNUSED)
{
  return false;
}

bool
irange_operator::op2_range (irange& r ATTRIBUTE_UNUSED,
			       irange& val ATTRIBUTE_UNUSED,
			       sro_truth truth ATTRIBUTE_UNUSED)
{
  return false;
}


/*  -----------------------------------------------------------------------  */

/* Range operator for def = (op1 == op2). */

class operator_equal : public irange_operator
{
public:
  virtual void dump (FILE *f);

  virtual bool fold_range (irange& r, irange& lh, irange& rh);
  virtual bool op1_range (irange& r, irange& val, sro_truth truth);
  virtual bool op2_range (irange& r, irange& val, sro_truth truth);
} op_equal;

void 
operator_equal::dump (FILE *f)
{
  fprintf (f, " == ");
}

/* Fold comparison of the 2 ranges.  */
bool
operator_equal::fold_range (irange& r, irange& lh, irange& rh)
{
  /* We can be sure the values are always equal or not if both ranges
     consist of a single value, and then compare them.  */
  if (wi::eq_p (lh.lower_bound (), lh.upper_bound ())
      && wi::eq_p (rh.lower_bound (), rh.upper_bound ()))
    {
      if (wi::eq_p (lh.lower_bound (), rh.upper_bound()))
	set_boolean_range_one (r);
      else
	set_boolean_range_zero (r);
    }
  else
    {
      /* If ranges do not intersect, we know the range is not equal, otherwise
         we don;t really know anything for sure.  */
      r.Intersect (lh, rh);
      if (r.empty_p ())
	set_boolean_range_zero (r);
      else
	set_boolean_range (r);
    }

  return true;
}

/* Calculate the range of op1 being == to VAL based on TRUTH.  */
bool
operator_equal::op1_range (irange& r, irange& val, sro_truth truth)
{
  r = val;
  /* If the result is FALSE, then reverse the range of val if it is a constant,
     otherwise it's the full range of the type of val.. */
  if (truth != SRO_NON_ZERO_DEF)
    {
      if (r.lower_bound() == r.upper_bound())
	r.Not ();
      else
	r.set_range (val.get_type());
    }
  return true;
}


bool
operator_equal::op2_range (irange& r, irange& val, sro_truth truth)
{
  return operator_equal::op1_range (r, val, truth);
}


/*  -----------------------------------------------------------------------  */

/* Range operator for def = op1 != op2. */

class operator_not_equal : public irange_operator
{
public:
  virtual void dump (FILE *f);

  virtual bool fold_range (irange& r, irange& lh, irange& rh);
  virtual bool op1_range (irange& r, irange& val, sro_truth truth);
  virtual bool op2_range (irange& r, irange& val, sro_truth truth);
} op_not_equal;

void 
operator_not_equal::dump (FILE *f)
{
  fprintf (f, " != ");
}

/* Fold comparison of the 2 ranges.  */
bool
operator_not_equal::fold_range (irange& r, irange& lh, irange& rh)
{
  /* We can be sure the values are always equal or not if both ranges
     consist of a single value, and then compare them.  */
  if (wi::eq_p (lh.lower_bound (), lh.upper_bound ())
      && wi::eq_p (rh.lower_bound (), rh.upper_bound ()))
    {
      if (wi::ne_p (lh.lower_bound (), rh.upper_bound()))
	set_boolean_range_one (r);
      else
	set_boolean_range_zero (r);
    }
  else
    {
      /* If ranges do not intersect, we know the range is not equal, otherwise
         we don;t really know anything for sure.  */
      r.Intersect (lh, rh);
      if (r.empty_p ())
	set_boolean_range_one (r);
      else
	set_boolean_range (r);
    }

  return true;
}

/* Calculate the range of op1 being == to VAL based on TRUTH.  */
bool
operator_not_equal::op1_range (irange& r, irange& val, sro_truth truth)
{
  r = val;
  /* If the result is TRUE, then op1 has a different range from VAL, but
     we can only be sure of that if VAL is a constant, otherwise it is the
     entire range of the type of val..  */
  if (truth == SRO_NON_ZERO_DEF)
    {
      if (r.lower_bound() == r.upper_bound())
	r.Not ();
      else
	r.set_range (val.get_type());
    }
  return true;
}


bool
operator_not_equal::op2_range (irange& r, irange& val, sro_truth truth)
{
  return operator_not_equal::op1_range (r, val, truth);
}

/*  -----------------------------------------------------------------------  */


class operator_relational :  public irange_operator
{
  enum tree_code code;
  bool eval_true (wide_int i, wide_int j, signop s);
  void build_lt (irange& r, tree type, wide_int val);
  void build_le (irange& r, tree type, wide_int val);
  void build_gt (irange& r, tree type, wide_int val);
  void build_ge (irange& r, tree type, wide_int val);

public:
  operator_relational (enum tree_code c);
  
  virtual bool fold_range (irange& r, irange& lh, irange& rh);
  virtual bool op1_range (irange& r, irange& val, sro_truth truth);
  virtual bool op2_range (irange& r, irange& val, sro_truth truth);

  virtual void dump (FILE *f);
};


operator_relational op_lt (LT_EXPR);
operator_relational op_le (LE_EXPR);
operator_relational op_gt (GT_EXPR);
operator_relational op_ge (GE_EXPR);

operator_relational::operator_relational (enum tree_code c)
{
  gcc_checking_assert (c >= LT_EXPR && c <= GE_EXPR);
  code = c;
}

void 
operator_relational::dump (FILE *f)
{
  switch (code)
    {
      case LT_EXPR:
	fprintf (f, " < ");
	break;
      case LE_EXPR:
	fprintf (f, " <= ");
	break;
      case GT_EXPR:
	fprintf (f, " > ");
	break;
      case GE_EXPR:
	fprintf (f, " >= ");
	break;
      default:
       gcc_unreachable ();
    }
}

/* (X < VAL) produces the a range of [MIN, VAL - 1]  */
void
operator_relational::build_lt (irange& r, tree type, wide_int val)
{
  bool ov;
  wide_int lim = wi::sub (val, 1, TYPE_SIGN (type), &ov);

  /* If val - 1 underflows, check is X < MIN, which is an empty range.  */
  if (ov)
    r.clear (type);
  else
    r.set_range (type, min_limit (type), lim);
}

/* (X <= VAL) produces the a range of [MIN, VAL]  */
void
operator_relational::build_le (irange& r, tree type, wide_int val)
{
  r.set_range (type, min_limit (type), val);
}

/* (X > VAL) produces the a range of [VAL + 1, MAX]  */
void
operator_relational::build_gt (irange& r, tree type, wide_int val)
{
  bool ov;
  wide_int lim = wi::add (val, 1, TYPE_SIGN (type), &ov);
  /* If val + 1 overflows, check is for X > MAX , which is an empty range.  */
  if (ov)
    r.clear (type);
  else
    r.set_range (type, lim, max_limit (type));
}

/* (X >= val) produces the a range of [VAL, MAX]  */
void
operator_relational::build_ge (irange& r, tree type, wide_int val)
{
  r.set_range (type, val, max_limit (type));
}


bool
operator_relational::eval_true (wide_int i, wide_int j, signop s)
{
  switch (code)
    {
      case LT_EXPR:
	return wi::lt_p (i, j, s);
      case LE_EXPR:
        return wi::le_p (i, j, s);
      case GT_EXPR:
        return wi::gt_p (i, j, s);
      case GE_EXPR:
        return wi::ge_p (i, j, s);
      default:
        break;
    }
  gcc_unreachable ();
}

bool operator_relational::fold_range (irange& r, irange& lh,
				      irange& rh)
{
  signop sign = TYPE_SIGN (lh.get_type ());
  gcc_checking_assert (sign == TYPE_SIGN (rh.get_type ()));

  switch (code)
    {
      case LT_EXPR:
      case LE_EXPR:
	if (eval_true (lh.upper_bound (), rh.lower_bound (), sign))
	  set_boolean_range_one (r);
	else
	  if (!eval_true (lh.lower_bound (), rh.upper_bound (), sign))
	    set_boolean_range_zero (r);
	  else 
	    set_boolean_range (r);
	break;

      case GT_EXPR:
      case GE_EXPR:
	if (eval_true (lh.lower_bound (), rh.upper_bound (), sign))
	  set_boolean_range_one (r);
	else
	  if (!eval_true (lh.upper_bound (), rh.lower_bound (), sign))
	    set_boolean_range_zero (r);
	  else 
	    set_boolean_range (r);
	break;

      default:
        gcc_unreachable ();
    }

  return boolean_type_node;

}

bool
operator_relational::op1_range (irange& r, irange& val, sro_truth truth)
{
  if (truth == SRO_NON_ZERO_DEF)
    switch (code)
      {
	case LT_EXPR:
	  build_lt (r, val.get_type (), val.upper_bound ());
	  break;

	case LE_EXPR:
	  build_le (r, val.get_type (), val.upper_bound ());
	  break;

	case GT_EXPR:
	  build_gt (r, val.get_type (), val.lower_bound ());
	  break;

	case GE_EXPR:
	  build_ge (r, val.get_type (), val.lower_bound ());
	  break;

	default:
	  gcc_unreachable ();
      }
  else
    switch (code)
      {
	case LT_EXPR:
	  build_ge (r, val.get_type (), val.lower_bound ());
	  break;

	case LE_EXPR:
	  build_gt (r, val.get_type (), val.lower_bound ());
	  break;

	case GT_EXPR:
	  build_le (r, val.get_type (), val.upper_bound ());
	  break;

	case GE_EXPR:
	  build_lt (r, val.get_type (), val.upper_bound ());
	  break;

	default:
	  gcc_unreachable ();
      }
  return true;
}

bool
operator_relational::op2_range (irange& r, irange& val, sro_truth truth)
{
  return operator_relational::op1_range (r, val, SRO_TRUTH_NEGATE (truth));
}


/*  -----------------------------------------------------------------------  */

/*  -----------------------------------------------------------------------  */
/* Range operator for     def = op1 + op2. */

enum opm_mode { OPM_ADD, OPM_SUB };

class operator_plus_minus : public irange_operator
{
public:
  void op_ir (opm_mode mode, irange& r, wide_int lh, irange& rh);
  void op_ri (opm_mode mode, irange& r, irange& lh, wide_int rh);
  void op_rr (opm_mode mode, irange& r, irange& lh, irange& rh);
};

void
operator_plus_minus::op_ir (opm_mode mode, irange& r, wide_int lh,
			    irange& rh)
{
  bool ov_lb, ov_ub;
  unsigned x;
  wide_int lb, ub, new_lb, new_ub;
  tree type = rh.get_type ();
  signop s = TYPE_SIGN (type);

  r.clear (type);
  for (x = 0; x < rh.num_ranges () ; x++)
    {
      lb = rh.lower_bound (x);
      ub = rh.upper_bound (x);
      if (mode == OPM_ADD)
        {
	  lb = wi::add (lh, lb, s, &ov_lb);
	  ub = wi::add (lh, ub, s, &ov_ub);
	}
      else
        {
	  lb = wi::sub (lh, lb, s, &ov_lb);
	  ub = wi::sub (lh, ub, s, &ov_ub);
	}
      add_to_range (r, lb, ov_lb, ub, ov_ub);
    }
}

void
operator_plus_minus::op_ri (opm_mode mode, irange& r, irange& lh,
			    wide_int rh)
{
  bool ov_lb, ov_ub;
  unsigned x;
  wide_int lb, ub, new_lb, new_ub;
  tree type = lh.get_type ();
  signop s = TYPE_SIGN (type);

  r.clear (type);
  for (x = 0; x < lh.num_ranges () ; x++)
    {
      lb = lh.lower_bound (x);
      ub = lh.upper_bound (x);
      if (mode == OPM_ADD)
        {
	  lb = wi::add (lb, rh, s, &ov_lb);
	  ub = wi::add (ub, rh, s, &ov_ub);
	}
      else
        {
	  lb = wi::sub (lb, rh, s, &ov_lb);
	  ub = wi::sub (ub, rh, s, &ov_ub);
	}
      add_to_range (r, lb, ov_lb, ub, ov_ub);
    }
}


void
operator_plus_minus::op_rr (opm_mode mode, irange& r, irange& lh,
			    irange& rh)
{
  if (lh.upper_bound () == lh.lower_bound ())
    op_ir (mode, r, lh.upper_bound (), rh);
  else
    if (rh.upper_bound () == rh.lower_bound ())
      op_ri (mode, r, lh, rh.upper_bound ());
    else
      {
	wide_int lb, ub;
	bool ov_lb, ov_ub;
	tree type = lh.get_type ();
	signop s = TYPE_SIGN (type);

	/* Forget about complexity of any multi-ranges. */
	if (mode == OPM_ADD)
	  {
	    /* Add the 2 lower and upper bounds togther and see what we get.  */
	    lb = wi::add (lh.lower_bound (), rh.lower_bound (), s, &ov_lb);
	    ub = wi::add (lh.upper_bound (), rh.upper_bound (), s, &ov_ub);
	  }
	else
	  {
	    /* New possible range is [lb1-ub2, ub1-lb2].  */
	    lb = wi::sub (lh.lower_bound (), rh.upper_bound (), s, &ov_lb);
	    lb = wi::sub (lh.upper_bound (), rh.lower_bound (), s, &ov_ub);
	  }

	/* If both overflow, we can't be sure of the final range. */
	if (ov_lb && ov_ub)
	  r.set_range (type);
	else
	  set_range (type, r, lb, ov_lb, ub, ov_ub);
      }
}


class operator_plus : public operator_plus_minus
{
public:
  virtual void dump (FILE *f);

  virtual bool fold_range (irange& r, irange& lh, irange& rh);
  virtual bool op1_range (irange& r, irange& val, sro_truth truth);
  virtual bool op2_range (irange& r, irange& val, sro_truth truth);
  virtual bool op1_adjust (irange& r, irange& val, sro_truth truth);
  virtual bool op2_adjust (irange& r, irange& val, sro_truth truth);
} op_plus;

void 
operator_plus::dump (FILE *f)
{
  fprintf (f, " + ");
}


bool
operator_plus::fold_range (irange& r, irange& lh, irange& rh)
{
  op_rr (OPM_ADD, r, lh, rh);
  return true;
}


/* Adjust irange to be in terms of op1. 
   Given [range] = op1 + val,  op1 = [range] - val.  */
bool
operator_plus::op1_adjust (irange& r, irange& val,
			   sro_truth truth ATTRIBUTE_UNUSED)
{
  irange old_range = r;
  op_rr (OPM_SUB, r, old_range, val);
  return true;
}

/* zero = op1 + [RANGE]  , op1 = 0 - [range].  
   for non-zero we know nothing since it could be anything in RANGE as well.  */
bool
operator_plus::op1_range (irange& r, irange& val, sro_truth truth)
{
  if (truth == SRO_NON_ZERO_DEF)
    r.set_range (val.get_type ());
  else
    {
      wide_int z = wi::uhwi (0, TYPE_PRECISION (r.get_type ()));
      op_ir (OPM_SUB, r, z, val);
    }
  return true;
}

bool
operator_plus::op2_adjust (irange& r, irange& val, sro_truth truth)
{
  return operator_plus::op1_adjust (r, val, truth);
}

bool
operator_plus::op2_range (irange& r, irange& val, sro_truth truth)
{
  return operator_plus::op1_range (r, val, truth);
}

/*  ----------------------------------------------------------------------  */

class operator_cast: public irange_operator
{
public:
  virtual void dump (FILE *f);

  virtual bool fold_range (irange& r, irange& lh, irange& rh);
  virtual bool op1_range (irange& r, irange& val, sro_truth truth);
  virtual bool op1_adjust (irange& r, irange& val, sro_truth truth);
} op_cast;

/* Unary operators put the range for the type of the expression as op2.  */

void
operator_cast::dump (FILE *f)
{
  fprintf (f, "(cast)");
}

/* Return the range of lh converted to rh.  */
bool
operator_cast::fold_range (irange& r, irange& lh, irange& rh)
{
  if (lh.get_type () != rh.get_type ())
    {
      /* Handle conversion so they become the same type.  */
      lh.cast (rh.get_type ());
    }
  /* If they are the same type, the result should be the intersection of
     the two ranges.  */
  r.Intersect (lh, rh);
  return true;
}

bool
operator_cast::op1_range (irange& r, irange& val,
			  sro_truth truth ATTRIBUTE_UNUSED)
{
  if (val.empty_p ())
    return false;

  /* Unary cast is simply the range of the cast type.
     t_2 = (int) char_1   currently returns a range for (int) because we 
     don't actually see char_1.  In theory would could return the intersection
     of these 2 types, but dynamic lookup ought to resove it OK anyway.  */
  r = val;
  return true;
}

// FIXME: Always returns TRUE.

bool
operator_cast::op1_adjust (irange& r, irange& val,
			   sro_truth truth ATTRIBUTE_UNUSED)
{
  gcc_assert (!val.empty_p ());
  if (r.get_type () != val.get_type ())
    {
      r.cast (val.get_type ());
      return true;
    }

  /* If the types are the same, we shouldnt have to do anything.  */
  return true;
}

/*  ----------------------------------------------------------------------  */

// Bitwise and logical ops. 
// Implement fold_range (), but there is little use in opX_range nor opX_adjust

class logical_and_operator : public irange_operator
{
public:
  void dump (FILE *f);
  bool fold_range (irange& r, irange& lh, irange& rh);
} op_logical_and;

void 
logical_and_operator::dump (FILE *f)
{
  fprintf (f, " && ");
}


bool
logical_and_operator::fold_range (irange& r, irange& lh, irange& rh)
{
  tree zero = build_int_cst (lh.get_type (), 0);
  irange zero_range (lh.get_type (), zero, zero);
  
  // Empty ranges are viral.
  if (lh.empty_p () || rh.empty_p ())
    {
      r.clear (boolean_type_node);
      return true;
    }

  // 0 && anything is 0
  if (lh == zero_range || rh == zero_range)
    {
      r.set_range (boolean_type_node, boolean_false_node, boolean_false_node);
      return true;
    }

  // To reach this point, there must be a logical 1 on each side, and the only
  // remaining question is whether there is a zero or not.

  if (lh.contains (zero) || rh.contains (zero))
    r.set_range (boolean_type_node, boolean_false_node, boolean_true_node);
  else
    r.set_range (boolean_type_node, boolean_true_node, boolean_true_node);
  
  return true;
}



class bitwise_and_operator : public logical_and_operator
{
public:
  void dump (FILE *f);
  bool fold_range (irange& r, irange& lh, irange& rh);
} op_bitwise_and;

void 
bitwise_and_operator::dump (FILE *f)
{
  fprintf (f, " & ");
}

bool
bitwise_and_operator::fold_range (irange& r, irange& lh, irange& rh)
{
  if (lh.get_type () == boolean_type_node)
    return logical_and_operator::fold_range (r, lh, rh);

  // look for the specific cases we care about I guess.  
  // If there are any. For now, just punt.
  return false;
}

class logical_or_operator : public irange_operator
{
public:
  void dump (FILE *f);
  bool fold_range (irange& r, irange& lh, irange& rh);
} op_logical_or;

void 
logical_or_operator::dump (FILE *f)
{
  fprintf (f, " || ");
}


bool
logical_or_operator::fold_range (irange& r, irange& lh, irange& rh)
{
  tree zero = build_int_cst (lh.get_type (), 0);
  irange zero_range (lh.get_type (), zero, zero);

  bool lh_has_true = (!lh.empty_p () && lh != zero_range);
  bool lh_has_false = lh.contains (zero);
  bool rh_has_true = (!rh.empty_p () && rh != zero_range);
  bool rh_has_false = rh.contains (zero);

  // Empty ranges are viral only if both are empty.
  if (lh.empty_p () && rh.empty_p ())
    {
      r.clear (boolean_type_node);
      return true;
    }

  // 1 || anything is always 1
  if ((lh_has_true && !lh_has_false) || (rh_has_true && !rh_has_false))
    {
      r.set_range (boolean_type_node, boolean_true_node, boolean_true_node);
      return true;
    }

  // 0 || 0  is the only case which returns a 0 now.
  if (!lh_has_true && !rh_has_true)
    {
      r.set_range (boolean_type_node, boolean_false_node, boolean_false_node);
      return true;
    }

  // Otherwise we don't actually know the result.
  r.set_range (boolean_type_node, boolean_false_node, boolean_true_node);
  
  return true;
}


class bitwise_or_operator : public logical_or_operator
{
public:
  void dump (FILE *f);
  bool fold_range (irange& r, irange& lh, irange& rh);
} op_bitwise_or;

void 
bitwise_or_operator::dump (FILE *f)
{
  fprintf (f, " | ");
}

bool
bitwise_or_operator::fold_range (irange& r, irange& lh, irange& rh)
{
  if (lh.get_type () == boolean_type_node)
    return logical_or_operator::fold_range (r, lh, rh);

  // look for the specific cases we care about I guess.  
  // If there are any. For now, just punt.
  return false;
}

class logical_not_operator : public irange_operator
{
public:
  void dump (FILE *f);
  bool fold_range (irange& r, irange& lh, irange& rh);
} op_logical_not;

void 
logical_not_operator::dump (FILE *f)
{
  fprintf (f, " ! ");
}


bool
logical_not_operator::fold_range (irange& r, irange& lh,
				  irange& rh ATTRIBUTE_UNUSED)
{
  tree zero = build_int_cst (lh.get_type (), 0);
  irange zero_range (lh.get_type (), zero, zero);

  // the rhs is ignored for NOT operations.

  bool lh_has_true = (!lh.empty_p () && lh != zero_range);
  bool lh_has_false = lh.contains (zero);


  // Empty ranges are viral 
  if (lh.empty_p ())
    {
      r.clear (boolean_type_node);
      return true;
    }

  // [1,1] -> [0,0]
  if (lh_has_true && !lh_has_false) 
    {
      r.set_range (boolean_type_node, boolean_false_node, boolean_false_node);
      return true;
    }

  // [0,0] -> [1,1]
  if (!lh_has_true && lh_has_false)
    {
      r.set_range (boolean_type_node, boolean_true_node, boolean_true_node);
      return true;
    }

  // Otherwise we don't actually know the result.
  r.set_range (boolean_type_node, boolean_false_node, boolean_true_node);
  
  return true;
}


class bitwise_not_operator : public logical_not_operator
{
public:
  void dump (FILE *f);
  bool fold_range (irange& r, irange& lh, irange& rh);
} op_bitwise_not;

void 
bitwise_not_operator::dump (FILE *f)
{
  fprintf (f, " ~ ");
}

bool
bitwise_not_operator::fold_range (irange& r, irange& lh, irange& rh)
{
  if (lh.get_type () == boolean_type_node)
    return logical_not_operator::fold_range (r, lh, rh);

  // look for the specific cases we care about I guess.  
  // If there are any. For now, just punt.
  return false;
}


/*  ----------------------------------------------------------------------  */

/* Create the irange operator table as a local object in this file, and the
   constructor should be automatically called before it is used.  */

class irange_op_table
{
  irange_operator *irange_tree[MAX_TREE_CODES];
public:
  irange_op_table ();
  inline irange_operator *operator[] (enum tree_code code);
} irange_tree;

irange_operator *irange_op_table::operator[] (enum tree_code code)
{
  gcc_assert (code > 0 && code < MAX_TREE_CODES);
  return irange_tree[code];
}

irange_op_table::irange_op_table ()
{
  irange_tree[LT_EXPR] = &op_lt;
  irange_tree[LE_EXPR] = &op_le;
  irange_tree[GT_EXPR] = &op_gt;
  irange_tree[GE_EXPR] = &op_ge;
  irange_tree[NE_EXPR] = &op_not_equal;
  irange_tree[EQ_EXPR] = &op_equal;
  irange_tree[PLUS_EXPR] = &op_plus;
//  irange_tree[NOP_EXPR] = &op_cast;
//  irange_tree[CONVERT_EXPR] = &op_cast;

  irange_tree[TRUTH_AND_EXPR] = &op_logical_and;
  irange_tree[TRUTH_OR_EXPR] = &op_logical_or;
//  irange_tree[TRUTH_NOT_EXPR] = &op_logical_not;

  irange_tree[BIT_AND_EXPR] = &op_bitwise_and;
  irange_tree[BIT_IOR_EXPR] = &op_bitwise_or;
//  irange_tree[BIT_NOT_EXPR] = &op_bitwise_not;
}

/* The table is hidden and accessed via a simple extern function.  */

irange_operator *
irange_op_handler (enum tree_code code)
{
  return irange_tree[code];
}


