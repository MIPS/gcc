/* Value range propagation and dead branch elimination.
   Copyright (C) 2003 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

/* This file performs simple value range propagation.
   It also removes branches which can't be reached,
   all code from the following example may be removed:

   if (i < 0)
   {
     if (i > 5)
       printf ("foo");
   }

   Currently it only tracks integer registers.

   This pass performs a data-flow on value ranges.
   As usual it has IN and OUT sets for each basic block.  The set is a hash
   table, for each register there is a range which the value of the register
   can be in.  When the range is full (i.e. from min value for the type
   to the max value) it is not stored in the hash table to save memory
   (so when the range for register is not there the range is full).
   Moreover VRP has a set for each edge too because the value ranges are
   different when we go through a then edge or an else edge.

   The IN set for a BB is computed as "union bounds" of the sets on edges
   going to BB. "Union bound" means that when we have 2 intervals
   [from_1, to_1] and [from_2, to_2] the result is
   [MIN (from_1, from_2), MAX (to_1, to_2)].

   We are producing the OUT set of BB from IN set by scanning the instructions
   in BB.  When we find an insn assigning a CONST_INT or REG to REG
   we set the range according to SET_SRC (only simple cases of SET_DST and
   SET_SRC are handled, in the other cases we set the range to be full).

   Finally we update the sets on edges going from BB.  First we copy the OUT set
   to edges going out from BB. Then we update the ranges on edges according to
   a cond jump (if cond jump is in the end of BB).

   When we have computed the ranges we redirect "dead" edge for a cond jump
   to the other edge.  "Dead" edge is an edge which has an empty range for some
   register on it, i.e. we will never go through this edge because of the
   conditions.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "tree.h"
#include "basic-block.h"
#include "hard-reg-set.h"
#include "expr.h"
#include "output.h"
#include "alloc-pool.h"
#include "sbitmap.h"
#include "fibheap.h"
#include "hashtab.h"

/* Signed or unsigned integer.  */
typedef struct value_def
{
  /* Signed integer value.  */
  signed HOST_WIDE_INT si;

  /* Unsigned integer value.  */
  unsigned HOST_WIDE_INT ui;
} value;

/* Structure describing a range.  */
typedef struct range_def
{
  /* Register which contains a value from the range.  */
  rtx reg;

  /* Lower bound of the range.  */
  value from;

  /* Upper bound of the range.  */
  value to;

  /* Type of the range, see RANGE_* below.  */
  char type;

  /* Is REG_DEAD note attached to REG?  */
  bool dead;
} range_t;

/* Range types.  */
#define RANGE_SIGNED_INT	1	/* Signed integer.  */
#define RANGE_UNSIGNED_INT	2	/* Unsigned integer.  */

/* Type of operation.  */
enum oper_type
{
  CONST_INT_TO_REG,			/* REG <- CONST_INT.  */
  REG_TO_REG,				/* REG <- REG.  */
  UNKNOWN_TO_REG,			/* REG <- anything else.  */
  CLOBBER_CALL_USED_REGS,		/* CALL_INSN.  */
  REG_IS_DEAD				/* REG_DEAD(REG).  */
};

/* Information about operation.  */
typedef struct operation_def
{
  enum oper_type type;
  rtx dst;
  rtx src;
  struct operation_def *next;
} *operation;

/* Data for each edge.  */
typedef struct edge_info_def
{
  /* The list of ranges of registers.  */
  htab_t htab;
} edge_info;

/* Data for each basic block.  */
typedef struct bb_info_def
{
  /* Normalized condition (if any) for the branch in the end of BB.  */
  rtx cond;

  operation operation_list;

  /* The IN list of ranges of registers for BB.  */
  htab_t in;

  /* The OUT list of ranges of registers for BB.  */
  htab_t out;
} bb_info;

/* Get a pointer to data for a basic block BB.  */
#define BBI(BB) ((bb_info *) (BB)->aux)

/* Get a pointer to data for an edge E.  */
#define EI(E) ((edge_info *) (E)->aux)

/* Maximal unsigned integer value for register REGISTER.  */
#define MAX_UINT_VALUE(REGISTER)					\
  ((unsigned HOST_WIDE_INT) GET_MODE_MASK (GET_MODE (REGISTER)))

/* Maximal signed integer value for register REGISTER.  */
#define MAX_SINT_VALUE(REGISTER)					\
  ((HOST_WIDE_INT) ((unsigned HOST_WIDE_INT)				\
		    GET_MODE_MASK (GET_MODE (REGISTER)) >> 1))

/* Minimal signed integer value for register REGISTER.  */
#define MIN_SINT_VALUE(REGISTER) (-MAX_SINT_VALUE (REGISTER) - 1)

/* Is range *RANGE empty? Test field FIELD of struct value_def.  */
#define RANGE_EMPTY_P(RANGE, FIELD) ((RANGE)->from.FIELD > (RANGE)->to.FIELD)

/* Make range *RANGE empty, only the type which uses fielt FIELD.  */
#define RANGE_MAKE_EMPTY(RANGE, FIELD)					\
  do {									\
    (RANGE)->from.FIELD = 1;						\
    (RANGE)->to.FIELD = 0;						\
  } while (0)
  
/* Alloc pool for range_t.  */
static alloc_pool range_pool;

/* Alloc pool for operation.  */
static alloc_pool operation_pool;

/* Shall we copy randes for dead registers?  */
static bool copy_dead_registers;

/* Local function prototypes.  */
static hashval_t range_hash		PARAMS ((const void *));
static int range_eq			PARAMS ((const void *, const void *));
static void range_del			PARAMS ((void *));
static bool convert_range		PARAMS ((range_t *, int));
static void convert_ranges		PARAMS ((range_t *, range_t *));
static int union_ranges			PARAMS ((void **, void *));
static void union_all_ranges		PARAMS ((htab_t, htab_t));
static bool ranges_differ		PARAMS ((const range_t *,
						 const range_t *));
static int compare_register_tables_1	PARAMS ((void **, void *));
static bool compare_register_tables	PARAMS ((htab_t, htab_t));
static int copy_range			PARAMS ((void **, void *));
static void copy_register_table		PARAMS ((htab_t, htab_t));
static void process_ranges_eq		PARAMS ((rtx, rtx, edge));
static void process_ranges_lt		PARAMS ((rtx, rtx, edge, edge));
static void process_ranges_ltu		PARAMS ((rtx, rtx, edge, edge));
static bool update_outgoing_edges	PARAMS ((basic_block, int));
static void find_operations_1		PARAMS ((rtx, rtx, void *));
static void find_operations		PARAMS ((basic_block));
static int delete_call_used_regs	PARAMS ((void **, void *));
static bool compute_ranges_for_bb	PARAMS ((basic_block));
static void compute_ranges		PARAMS ((void));
static bool edge_is_dead		PARAMS ((edge));
static bool redirect_edges		PARAMS ((void));

static int dump_range			PARAMS ((void **, void *));
static void dump_all_ranges		PARAMS ((FILE *, htab_t));
void debug_range			PARAMS ((range_t *));
void debug_all_ranges			PARAMS ((htab_t));
static void dump_operation_list		PARAMS ((FILE *, basic_block));
void debug_operation_list		PARAMS ((basic_block));

/* Hash function for register R.  */
#define HASH_REG(R) (REGNO (R))

/* Hash function for table of ranges. It is computed from the range XX.  */

static hashval_t
range_hash (xx)
     const void *xx;
{
  const range_t *r = xx;

  return HASH_REG (r->reg);
}

/* Return true when the range XX is the range for register YY.  */

static int
range_eq (xx, yy)
     const void *xx;
     const void *yy;
{
  const range_t *r = (const range_t *) xx;
  const rtx y = (const rtx) yy;

  return REGNO (r->reg) == REGNO (y);
}

/* Free the range X.  */

static void
range_del (x)
     void *x;
{
  pool_free (range_pool, x);
}

/* Convert range *RANGE to type TYPE. Return true on success.  */

static bool
convert_range (range, type)
     range_t *range;
     int type;
{
  /* If the range already has type TYPE do nothing.  */
  if ((range->type & type) == type)
    return true;

  switch (type)
    {
      case RANGE_SIGNED_INT:
	if ((range->type & type) == RANGE_UNSIGNED_INT)
	  {
	    if (RANGE_EMPTY_P (range, ui))
	      {
		RANGE_MAKE_EMPTY (range, si);
		range->type |= RANGE_SIGNED_INT;
		return true;
	      }
	    if (range->to.ui <= ((unsigned HOST_WIDE_INT)
				MAX_SINT_VALUE (range->reg)))
	      {
		range->from.si = range->from.ui;
		range->to.si = range->to.ui;
		range->type |= RANGE_SIGNED_INT;
		return true;
	      }
	  }
	break;

      case RANGE_UNSIGNED_INT:
	if ((range->type & type) == RANGE_SIGNED_INT)
	  {
	    if (RANGE_EMPTY_P (range, si))
	      {
		RANGE_MAKE_EMPTY (range, ui);
		range->type |= RANGE_UNSIGNED_INT;
		return true;
	      }
	    if (range->from.si >= 0)
	      {
		range->from.ui = range->from.si;
		range->to.ui = range->to.si;
		range->type |= RANGE_UNSIGNED_INT;
		return true;
	      }
	  }
	break;

      default:
	abort ();
    }

  return false;
}

/* Convert ranges RANGE1 and RANGE2 to be of same types.  */

static void
convert_ranges (range1, range2)
     range_t *range1;
     range_t *range2;
{
  if ((range1->type & RANGE_SIGNED_INT) != 0)
    convert_range (range1, RANGE_UNSIGNED_INT);
  if ((range1->type & RANGE_UNSIGNED_INT) != 0)
    convert_range (range1, RANGE_SIGNED_INT);
  if ((range2->type & RANGE_SIGNED_INT) != 0)
    convert_range (range2, RANGE_UNSIGNED_INT);
  if ((range2->type & RANGE_UNSIGNED_INT) != 0)
    convert_range (range2, RANGE_SIGNED_INT);
}

/* Data passed from union_all_ranges to union_ranges.  */

typedef struct union_ranges_data_def
{
  /* The list of ranges from one and second edge.  */
  htab_t t1;
  htab_t t2;
} union_ranges_data;

/* Unify range from *SLOT and the corresponding range from DATA.T2.  */

static int
union_ranges (slot, data)
     void **slot;
     void *data;
{
  union_ranges_data *d = data;
  htab_t t1 = d->t1;
  htab_t t2 = d->t2;
  range_t *r1 = *slot;
  range_t *r2;

  if (!copy_dead_registers && r1->dead)
    return 1;

  r2 = htab_find_with_hash (t2, r1->reg, HASH_REG (r1->reg));
  if (!r2)
    {
      /* Register from second edge may have any value so the result may have
	 also any value.  */
      htab_clear_slot (t1, slot);
      return 1;
    }

  /* If the types of ranges are not the same try to convert ranges to both
     integer types.  */
  if ((r1->type & r2->type) == 0)
    convert_ranges (r1, r2);

  r1->type &= r2->type;
  if (r1->type == 0)
    {
      /* The ranges are not compatible so make the union to be a full range
	 (i.e. delete it from the table).  */
      htab_clear_slot (t1, slot);
      return 1;
    }

  /* For compatible ranges make the bounds to contain both intervals.  */
  if ((r1->type & RANGE_SIGNED_INT) != 0)
    {
      if (RANGE_EMPTY_P (r1, si))
	{
	  /* First range is empty so the union is the second range.  */
	  r1->from.si = r2->from.si;
	  r1->to.si = r2->to.si;
	}
      else if (RANGE_EMPTY_P (r2, si))
	/* Second range is empty so the union is already in the first range.  */
	;
      else
	{
	  if (r1->from.si > r2->from.si)
	    r1->from.si = r2->from.si;
	  if (r1->to.si < r2->to.si)
	    r1->to.si = r2->to.si;
	}
    }

  if ((r1->type & RANGE_UNSIGNED_INT) != 0)
    {
      if (RANGE_EMPTY_P (r1, ui))
	{
	  /* First range is empty so the union is the second range.  */
	  r1->from.ui = r2->from.ui;
	  r1->to.ui = r2->to.ui;
	}
      else if (RANGE_EMPTY_P (r2, ui))
	/* Second range is empty so the union is already in the first range.  */
	;
      else
	{
	  if (r1->from.ui > r2->from.ui)
	    r1->from.ui = r2->from.ui;
	  if (r1->to.ui < r2->to.ui)
	    r1->to.ui = r2->to.ui;
	}
    }

  if (((r1->type & RANGE_SIGNED_INT) == 0
       || (r1->from.si == MIN_SINT_VALUE (r1->reg)
	   && r1->to.si == MAX_SINT_VALUE (r1->reg)))
      && ((r1->type & RANGE_UNSIGNED_INT) == 0
	  || (r1->from.ui == 0
	      && r1->to.ui == MAX_UINT_VALUE (r1->reg))))
    {
      /* Delete the range if it is full range for all its types.  */
      htab_clear_slot (t1, slot);
    }

  return 1;
}

/* Unify all ranges in table T1 with the corresponding ranges from T2.  */

static void
union_all_ranges (t1, t2)
     htab_t t1;
     htab_t t2;
{
  union_ranges_data data;

  data.t1 = t1;
  data.t2 = t2;
  htab_traverse (t1, union_ranges, &data);
}

/* Return true if ranges R1 and R2 differ.  */

static bool
ranges_differ (r1, r2)
     const range_t *r1;
     const range_t *r2;
{
  /* R1 and R2 do not have a common type.  */
  if ((r1->type & r2->type) == 0)
    return true;

  if (!rtx_equal_p (r1->reg, r2->reg))
    return true;

  if ((r1->type & r2->type & RANGE_SIGNED_INT) != 0)
  {
    bool empty1 = RANGE_EMPTY_P (r1, si);
    bool empty2 = RANGE_EMPTY_P (r2, si);

    if (!empty1 && !empty2)
      {
	if (r1->from.si != r2->from.si)
	  return true;

	if (r1->to.si != r2->to.si)
	  return true;
      }
    else
      {
	if (empty1 && empty2)
	  return false;

	/* if (empty1 != empty2) */
	return true;
      }
  }

  if ((r1->type & r2->type & RANGE_UNSIGNED_INT) != 0)
  {
    bool empty1 = RANGE_EMPTY_P (r1, ui);
    bool empty2 = RANGE_EMPTY_P (r2, ui);

    if (!empty1 && !empty2)
      {
	if (r1->from.ui != r2->from.ui)
	  return true;

	if (r1->to.ui != r2->to.ui)
	  return true;
      }
    else
      {
	if (empty1 && empty2)
	  return false;

	/* if (empty1 != empty2) */
	return true;
      }
  }

  return false;
}

/* Variable to hold the temporary result of compare_register_tables.  */
static bool compare_register_tables_value;

/* Compare the range *SLOT with the corresponding range in table DATA.  */

static int
compare_register_tables_1 (slot, data)
     void **slot;
     void *data;
{
  range_t *r1 = *slot;
  range_t *r2;
  htab_t htab = data;

  r2 = htab_find_with_hash (htab, r1->reg, HASH_REG (r1->reg));

  if (r2 == NULL || ranges_differ (r1, r2))
    {
      compare_register_tables_value = true;

      /* Stop traversing.  */
      return 0;
    }

  /* Continue traversing.  */
  return 1;
}

/* Return true when the tables T1 and T2 of ranges differ.  */

static bool
compare_register_tables (t1, t2)
     htab_t t1;
     htab_t t2;
{
  compare_register_tables_value = false;

  htab_traverse (t1, compare_register_tables_1, t2);
  if (!compare_register_tables_value)
    htab_traverse (t2, compare_register_tables_1, t1);

  return compare_register_tables_value;
}

/* Copy range *SRC_SLOT to table DATA.  */

static int
copy_range (src_slot, data)
     void **src_slot;
     void *data;
{
  range_t *src_r = *src_slot;
  range_t *dst_r;
  htab_t dst = data;
  void **dst_slot;

  if (!copy_dead_registers && src_r->dead)
    {
      /* Do not copy this range and continue traversing the hash table.  */
      return 1;
    }

  dst_r = pool_alloc (range_pool);
  *dst_r = *src_r;

  dst_slot = htab_find_slot_with_hash (dst, src_r->reg, HASH_REG (src_r->reg),
				       INSERT);
  *dst_slot = dst_r;

  /* Continue traversing the hash table.  */
  return 1;
}

/* Copy the content of table SRC to DST.  */

static void
copy_register_table (dst, src)
     htab_t dst;
     htab_t src;
{
  htab_empty (dst);
  htab_traverse (src, copy_range, dst);
}

/* Find range for register REGISTER on edge EDGE and store it to RANGE.  */
#define FIND_RANGE(RANGE, REGISTER, EDGE)				      \
  do {									      \
    void **slot;							      \
    slot = htab_find_slot_with_hash (EI (EDGE)->htab, (REGISTER),	      \
				     HASH_REG (REGISTER), NO_INSERT);	      \
    if (slot)								      \
      (RANGE) = *slot;							      \
  } while (0)

/* Allocate RANGE for register REGISTER and store it onto edge EDGE.  */
#define CREATE_RANGE(RANGE, REGISTER, EDGE)				      \
  do {									      \
    void **slot;							      \
    slot = htab_find_slot_with_hash (EI (EDGE)->htab, (REGISTER),	      \
				     HASH_REG (REGISTER), INSERT);	      \
    *slot = (RANGE) = pool_alloc (range_pool);				      \
  } while (0)

/* Initialize the range RANGE to be a signed integer constant CONST
   and store a pointer to it to RANGEP.  */
#define INIT_CONSTANT_RANGE_SINT(RANGEP, RANGE, CONST)			      \
  do {									      \
    (RANGEP) = &(RANGE);						      \
    (RANGE).reg = NULL_RTX;						      \
    (RANGE).from.si = INTVAL (CONST);					      \
    (RANGE).to.si = INTVAL (CONST);					      \
    (RANGE).type = RANGE_UNSIGNED_INT;					      \
    (RANGE).dead = false;						      \
  } while (0)

/* Initialize the range RANGE to be a unsigned integer constant CONST
   and store a pointer to it to RANGEP.  MAX_VAL is the maximal value
   for the type.  */
#define INIT_CONSTANT_RANGE_UINT(RANGEP, RANGE, CONST, MAX_VAL)		      \
  do {									      \
    (RANGEP) = &(RANGE);						      \
    (RANGE).reg = NULL_RTX;						      \
    (RANGE).from.ui = (unsigned HOST_WIDE_INT) INTVAL (CONST) & (MAX_VAL);    \
    (RANGE).to.ui = (unsigned HOST_WIDE_INT) INTVAL (CONST) & (MAX_VAL);      \
    (RANGE).type = RANGE_UNSIGNED_INT;					      \
    (RANGE).dead = false;						      \
  } while (0)

/* Update the ranges for OP0 and OP1 on edge E according to
   result of OP0 EQ OP1.  */

static void
process_ranges_eq (op0, op1, e)
     rtx op0;
     rtx op1;
     edge e;
{
  range_t *r0 = NULL;	/* Range for OP0.  */
  range_t *r1 = NULL;	/* Range for OP1.  */
  range_t tmp;

  FIND_RANGE (r0, op0, e);

  if (GET_CODE (op1) == CONST_INT)
    {
      unsigned HOST_WIDE_INT mask = MAX_UINT_VALUE (op0);

      /* OP1 is a constant, create a temporary range for it.  */
      INIT_CONSTANT_RANGE_SINT (r1, tmp, op1);
      INIT_CONSTANT_RANGE_UINT (r1, tmp, op1, mask);
      tmp.type = RANGE_SIGNED_INT | RANGE_UNSIGNED_INT;
    }
  else if (GET_CODE (op1) == REG
	   && GET_MODE (op0) == GET_MODE (op1))
    {
      FIND_RANGE (r1, op1, e);
    }
  else
    return;

  /* If both ranges are full ranges we have nothing to do.  */
  if (!r0 && !r1)
    return;

  if (r0)
    {
      if (r1)
	{
	  /* If the types of ranges are not the same try to convert ranges to
	     be of same types.  */
	  if ((r0->type & r1->type) == 0)
	    convert_ranges (r0, r1);

	  /* If the types of ranges still are not the same do not shrink ranges
	     because we do not know how.  */
	  if ((r0->type & r1->type) == 0)
	    return;

	  if ((r0->type & r1->type & RANGE_SIGNED_INT) != 0)
	    {
		r0->type = r1->type = RANGE_SIGNED_INT;

		/* If one range is empty make the other empty too.  */
		if (RANGE_EMPTY_P (r0, si))
		  RANGE_MAKE_EMPTY (r1, si);
		else if (RANGE_EMPTY_P (r1, si))
		  RANGE_MAKE_EMPTY (r0, si);
		else
		  {
		    /* If OP1 EQ OP2 set both ranges to the intersection
		       of them.  */
		    if (r0->from.si < r1->from.si)
		      r0->from.si = r1->from.si;
		    else
		      r1->from.si = r0->from.si;

		    if (r0->to.si < r1->to.si)
		      r1->to.si = r0->to.si;
		    else
		      r0->to.si = r1->to.si;
		  }
	    }
	  if ((r0->type & r1->type & RANGE_UNSIGNED_INT) != 0)
	    {
		r0->type = r1->type = RANGE_UNSIGNED_INT;

		/* If one range is empty make the other empty too.  */
		if (RANGE_EMPTY_P (r0, ui))
		  RANGE_MAKE_EMPTY (r1, ui);
		else if (RANGE_EMPTY_P (r1, ui))
		  RANGE_MAKE_EMPTY (r0, ui);
		else
		  {
		    /* If OP1 EQ OP2 set both ranges to the intersection
		       of them.  */
		    if (r0->from.ui < r1->from.ui)
		      r0->from.ui = r1->from.ui;
		    else
		      r1->from.ui = r0->from.ui;

		    if (r0->to.ui < r1->to.ui)
		      r1->to.ui = r0->to.ui;
		    else
		      r0->to.ui = r1->to.ui;
		  }
	    }
	}
      else
	{
	  /* R1 is a full range, intersection with R0 is R0.  */
	  CREATE_RANGE (r1, op1, e);
	  *r1 = *r0;
	  r1->reg = op1;
	  r1->dead = false;
	}
    }
  else if (r1)
    {
      /* R0 is a full range, intersection with R1 is R1.  */
      CREATE_RANGE (r0, op0, e);
      *r0 = *r1;
      r0->reg = op0;
      r0->dead = false;
    }
}

/* Update the ranges for OP0 and OP1 on edges THEN_EDGE and ELSE_EDGE according
   to result of OP0 LT OP1.  */

static void
process_ranges_lt (op0, op1, then_edge, else_edge)
     rtx op0;
     rtx op1;
     edge then_edge;
     edge else_edge;
{
  range_t *r0t = NULL;	/* Range for OP0 on THEN_EDGE.  */
  range_t *r0e = NULL;	/* Range for OP0 on ELSE_EDGE.  */
  range_t *r1t = NULL;	/* Range for OP1 on THEN_EDGE.  */
  range_t *r1e = NULL;	/* Range for OP1 on ELSE_EDGE.  */
  range_t tmp0t, tmp0e, tmp1t, tmp1e;
  HOST_WIDE_INT max_val = 0;	/* Set it to something to avoid a warning.  */
  HOST_WIDE_INT min_val = 0;

  /* If the registers are not compatible leave the ranges as they are.  */
  if (GET_CODE (op0) == REG && GET_CODE (op1) == REG
      && GET_MODE (op0) != GET_MODE (op1))
    return;

  if (GET_CODE (op0) == REG)
    {
      max_val = MAX_SINT_VALUE (op0);
      min_val = MIN_SINT_VALUE (op0);

      /* Find the range for OP0 on THEN_EDGE and ELSE_EDGE.  */
      FIND_RANGE (r0t, op0, then_edge);
      FIND_RANGE (r0e, op0, else_edge);

      /* If the type of one range is not signed int try to convert it.
	 If it can't be converted leave the ranges as they are.  */
      if (r0t && !convert_range (r0t, RANGE_SIGNED_INT))
	return;
      if (r0e && !convert_range (r0e, RANGE_SIGNED_INT))
	return;
    }
  if (GET_CODE (op1) == REG)
    {
      max_val = MAX_SINT_VALUE (op1);
      min_val = MIN_SINT_VALUE (op1);

      /* Find the range for OP1 on THEN_EDGE and ELSE_EDGE.  */
      FIND_RANGE (r1t, op1, then_edge);
      FIND_RANGE (r1e, op1, else_edge);

      /* If the type of one range is not signed int try to convert it.
	 If it can't be converted leave the ranges as they are.  */
      if (r1t && !convert_range (r1t, RANGE_SIGNED_INT))
	return;
      if (r1e && !convert_range (r1e, RANGE_SIGNED_INT))
	return;
    }

  if (GET_CODE (op0) == CONST_INT)
    {
      /* OP0 is a constant, create temporary ranges for it.  */
      INIT_CONSTANT_RANGE_SINT (r0t, tmp0t, op0);
      INIT_CONSTANT_RANGE_SINT (r0e, tmp0e, op0);
    }
  else if (GET_CODE (op0) != REG)
    return;

  if (GET_CODE (op1) == CONST_INT)
    {
      /* OP1 is a constant, create temporary ranges for it.  */
      INIT_CONSTANT_RANGE_SINT (r1t, tmp1t, op1);
      INIT_CONSTANT_RANGE_SINT (r1e, tmp1e, op1);
    }
  else if (GET_CODE (op1) != REG)
    return;

  /* If both ranges on TRUE_EDGE are full (and thus on ELSE_EDGE too)
     we have nothing to do.  */
  if (!r0t && !r1t)
    return;

  /* If both operands are registers we may shorten their ranges by changing
     the bounds by 1 in some situations (PR 11559).
     So when both ranges are large (here: not empty or constant) do not shorten
     the ranges to avoid consuming too much time.  */
  if (GET_CODE (op0) == REG && GET_CODE (op1) == REG
      && (!r0t || r0t->from.si < r0t->to.si)
      && (!r1t || r1t->from.si < r1t->to.si))
    return;

  if (r0t)
    {
      if (r1t)
	{
	  r0t->type = r0e->type = RANGE_SIGNED_INT;
	  r1t->type = r1e->type = RANGE_SIGNED_INT;

	  /* If one range is empty make the other empty too.  */
	  if (RANGE_EMPTY_P (r0t, si))
	    {
	      RANGE_MAKE_EMPTY (r1t, si);
	      RANGE_MAKE_EMPTY (r1e, si);
	      return;
	    }

	  if (RANGE_EMPTY_P (r1t, si))
	    {
	      RANGE_MAKE_EMPTY (r0t, si);
	      RANGE_MAKE_EMPTY (r0e, si);
	      return;
	    }

	  if (r0t->from.si == max_val)
	    {
	      /* The condition OP0 LT OP1 may be never true. */
	      RANGE_MAKE_EMPTY (r0t, si);
	      RANGE_MAKE_EMPTY (r1t, si);
	    }
	  else if (r0t->from.si >= r1t->from.si)
	    r1t->from.si = r0t->from.si + 1;
	  else
	    r0e->from.si = r1e->from.si;

	  if (r1t->to.si == min_val)
	    {
	      /* The condition OP0 LT OP1 may be never true. */
	      RANGE_MAKE_EMPTY (r0t, si);
	      RANGE_MAKE_EMPTY (r1t, si);
	    }
	  else if (r0t->to.si >= r1t->to.si)
	    r0t->to.si = r1t->to.si - 1;
	  else
	    r1e->to.si = r0e->to.si;
	}
      else
	{
	  /* R1 is full interval, shorten it on THEN_EDGE and ELSE_EDGE.  */
	  CREATE_RANGE (r1t, op1, then_edge);
	  CREATE_RANGE (r1e, op1, else_edge);

	  r1t->reg = r1e->reg = op1;
	  r1t->dead = r1e->dead = false;
	  r1t->type = r1e->type = RANGE_SIGNED_INT;
	  r0t->type = r0e->type = RANGE_SIGNED_INT;

	  /* If one range is empty make the other empty too.  */
	  if (RANGE_EMPTY_P (r0t, si))
	    {
	      RANGE_MAKE_EMPTY (r1t, si);
	      RANGE_MAKE_EMPTY (r1e, si);
	      return;
	    }

	  if (r0t->from.si == max_val)
	    {
	      RANGE_MAKE_EMPTY (r1t, si);
	    }
	  else
	    {
	      r1t->from.si = r0t->from.si + 1;
	      r1t->to.si = max_val;
	    }

	  r1e->from.si = min_val;
	  r1e->to.si = r0e->to.si;
	}
    }
  else if (r1t)
    {
      /* R0 is full interval, shorten it on THEN_EDGE and ELSE_EDGE.  */
      CREATE_RANGE (r0t, op0, then_edge);
      CREATE_RANGE (r0e, op0, else_edge);

      r0t->reg = r0e->reg = op0;
      r0t->dead = r0e->dead = false;
      r0t->type = r0e->type = RANGE_SIGNED_INT;
      r1t->type = r1e->type = RANGE_SIGNED_INT;

      /* If one range is empty make the other empty too.  */
      if (RANGE_EMPTY_P (r1t, si))
	{
	  RANGE_MAKE_EMPTY (r0t, si);
	  RANGE_MAKE_EMPTY (r0e, si);
	  return;
	}

      if (r1t->to.si == min_val)
	{
	  RANGE_MAKE_EMPTY (r0t, si);
	}
      else
	{
	  r0t->from.si = min_val;
	  r0t->to.si = r1t->to.si - 1;
	}

      r0e->from.si = r1e->from.si;
      r0e->to.si = max_val;
    }
}

/* Update the ranges for OP0 and OP1 on edges THEN_EDGE and ELSE_EDGE according
   to result of OP0 LTU OP1.
   This function is very similar to process_ranges_lt but it is working
   with unsigned intervals.  */

static void
process_ranges_ltu (op0, op1, then_edge, else_edge)
     rtx op0;
     rtx op1;
     edge then_edge;
     edge else_edge;
{
  range_t *r0t = NULL;	/* Range for OP0 on THEN_EDGE.  */
  range_t *r0e = NULL;	/* Range for OP0 on ELSE_EDGE.  */
  range_t *r1t = NULL;	/* Range for OP1 on THEN_EDGE.  */
  range_t *r1e = NULL;	/* Range for OP1 on ELSE_EDGE.  */
  range_t tmp0t, tmp0e, tmp1t, tmp1e;
  unsigned HOST_WIDE_INT max_val = 0;
  unsigned HOST_WIDE_INT min_val = 0;

  /* If the registers are not compatible leave the ranges as they are.  */
  if (GET_CODE (op0) == REG && GET_CODE (op1) == REG
      && GET_MODE (op0) != GET_MODE (op1))
    return;

  if (GET_CODE (op0) == REG)
    {
      max_val = MAX_UINT_VALUE (op0);

      /* Find the range for OP0 on THEN_EDGE and ELSE_EDGE.  */
      FIND_RANGE (r0t, op0, then_edge);
      FIND_RANGE (r0e, op0, else_edge);

      /* If the type of one range is not unsigned int try to convert it.
	 If it can't be converted leave the ranges as they are.  */
      if (r0t && !convert_range (r0t, RANGE_UNSIGNED_INT))
	return;
      if (r0e && !convert_range (r0e, RANGE_UNSIGNED_INT))
	return;
    }
  if (GET_CODE (op1) == REG)
    {
      max_val = MAX_UINT_VALUE (op0);

      /* Find the range for OP1 on THEN_EDGE and ELSE_EDGE.  */
      FIND_RANGE (r1t, op1, then_edge);
      FIND_RANGE (r1e, op1, else_edge);

      /* If the type of one range is not unsigned int try to convert it.
	 If it can't be converted leave the ranges as they are.  */
      if (r1t && !convert_range (r1t, RANGE_UNSIGNED_INT))
	return;
      if (r1e && !convert_range (r1e, RANGE_UNSIGNED_INT))
	return;
    }

  if (GET_CODE (op0) == CONST_INT)
    {
      /* OP0 is a constant, create a temporary range for it.  */
      INIT_CONSTANT_RANGE_UINT (r0t, tmp0t, op0, max_val);
      INIT_CONSTANT_RANGE_UINT (r0e, tmp0e, op0, max_val);
    }
  else if (GET_CODE (op0) != REG)
    return;

  if (GET_CODE (op1) == CONST_INT)
    {
      /* OP1 is a constant, create a temporary range for it.  */
      INIT_CONSTANT_RANGE_UINT (r1t, tmp1t, op1, max_val);
      INIT_CONSTANT_RANGE_UINT (r1e, tmp1e, op1, max_val);
    }
  else if (GET_CODE (op0) != REG)
    return;

  /* If both ranges on TRUE_EDGE are full (and thus on ELSE_EDGE too)
     we have nothing to do.  */
  if (!r0t && !r1t)
    return;

  /* If both operands are registers we may shorten their ranges by changing
     the bounds by 1 in some situations (PR 11559).
     So when both ranges are large (here: not empty or constant) do not shorten
     the ranges to avoid consuming too much time.  */
  if (GET_CODE (op0) == REG && GET_CODE (op1) == REG
      && (!r0t || r0t->from.ui < r0t->to.ui)
      && (!r1t || r1t->from.ui < r1t->to.ui))
    return;

  if (r0t)
    {
      if (r1t)
	{
	  r0t->type = r0e->type = RANGE_UNSIGNED_INT;
	  r1t->type = r1e->type = RANGE_UNSIGNED_INT;

	  /* If one range is empty make the other empty too.  */
	  if (RANGE_EMPTY_P (r0t, ui))
	    {
	      RANGE_MAKE_EMPTY (r1t, ui);
	      RANGE_MAKE_EMPTY (r1e, ui);
	      return;
	    }

	  if (RANGE_EMPTY_P (r1t, ui))
	    {
	      RANGE_MAKE_EMPTY (r0t, ui);
	      RANGE_MAKE_EMPTY (r0e, ui);
	      return;
	    }

	  if (r0t->from.ui == max_val)
	    {
	      /* The condition OP0 LT OP1 may be never true. */
	      RANGE_MAKE_EMPTY (r0t, ui);
	      RANGE_MAKE_EMPTY (r1t, ui);
	    }
	  else if (r0t->from.ui >= r1t->from.ui)
	    r1t->from.ui = r0t->from.ui + 1;
	  else
	    r0e->from.ui = r1e->from.ui;

	  if (r1t->to.ui == min_val)
	    {
	      /* The condition OP0 LT OP1 may be never true. */
	      RANGE_MAKE_EMPTY (r0t, ui);
	      RANGE_MAKE_EMPTY (r1t, ui);
	    }
	  else if (r0t->to.ui >= r1t->to.ui)
	    r0t->to.ui = r1t->to.ui - 1;
	  else
	    r1e->to.ui = r0e->to.ui;
	}
      else
	{
	  /* R1 is full interval, shorten it on THEN_EDGE and ELSE_EDGE.  */
	  CREATE_RANGE (r1t, op1, then_edge);
	  CREATE_RANGE (r1e, op1, else_edge);

	  r1t->reg = r1e->reg = op1;
	  r1t->dead = r1e->dead = false;
	  r1t->type = r1e->type = RANGE_UNSIGNED_INT;
	  r0t->type = r0e->type = RANGE_UNSIGNED_INT;

	  /* If one range is empty make the other empty too.  */
	  if (RANGE_EMPTY_P (r0t, ui))
	    {
	      RANGE_MAKE_EMPTY (r1t, ui);
	      RANGE_MAKE_EMPTY (r1e, ui);
	      return;
	    }

	  if (r0t->from.ui == max_val)
	    {
	      RANGE_MAKE_EMPTY (r1t, ui);
	    }
	  else
	    {
	      r1t->from.ui = r0t->from.ui + 1;
	      r1t->to.ui = max_val;
	    }

	  r1e->from.ui = min_val;
	  r1e->to.ui = r0e->to.ui;
	}
    }
  else if (r1t)
    {
      /* R0 is full interval, shorten it on THEN_EDGE and ELSE_EDGE.  */
      CREATE_RANGE (r0t, op0, then_edge);
      CREATE_RANGE (r0e, op0, else_edge);

      r0t->reg = r0e->reg = op0;
      r0t->dead = r0e->dead = false;
      r0t->type = r0e->type = RANGE_UNSIGNED_INT;
      r1t->type = r1e->type = RANGE_UNSIGNED_INT;

      /* If one range is empty make the other empty too.  */
      if (RANGE_EMPTY_P (r1t, ui))
	{
	  RANGE_MAKE_EMPTY (r0t, ui);
	  RANGE_MAKE_EMPTY (r0e, ui);
	  return;
	}

      if (r1t->to.ui == min_val)
	{
	  RANGE_MAKE_EMPTY (r0t, ui);
	}
      else
	{
	  r0t->from.ui = min_val;
	  r0t->to.ui = r1t->to.ui - 1;
	}

      r0e->from.ui = r1e->from.ui;
      r0e->to.ui = max_val;
    }
}

/* Update the ranges on the edges going out from BB.  */

static bool
update_outgoing_edges (bb, changed)
     basic_block bb;
     int changed;
{
  edge e;
  htab_t old_then = NULL;	/* Set it to something to avoid a warning.  */
  htab_t old_else = NULL;

  copy_dead_registers = true;
  if (!changed && BBI (bb)->cond != NULL)
    {
      edge then_edge = BRANCH_EDGE (bb);
      edge else_edge = FALLTHRU_EDGE (bb);

      old_then = htab_create (EI (then_edge)->htab->n_elements, range_hash,
			      range_eq, range_del);
      copy_register_table (old_then, EI (then_edge)->htab);
      old_else = htab_create (EI (else_edge)->htab->n_elements, range_hash,
			      range_eq, range_del);
      copy_register_table (old_else, EI (else_edge)->htab);
    }

  /* Copy the BB's OUT set to edges.  */
  for (e = bb->succ; e; e = e->succ_next)
    copy_register_table (EI (e)->htab, BBI (bb)->out);

  /* Update outgoing edges.  */
  if (BBI (bb)->cond != NULL)
    {
      edge then_edge = BRANCH_EDGE (bb);
      edge else_edge = FALLTHRU_EDGE (bb);
      rtx op0 = XEXP (BBI (bb)->cond, 0);
      rtx op1 = XEXP (BBI (bb)->cond, 1);

      if (GET_CODE (op0) == REG
	  && (GET_CODE (op1) == REG
	      || GET_CODE (op1) == CONST_INT))
	{
	  switch (GET_CODE (BBI (bb)->cond))
	    {
	      case EQ:
		process_ranges_eq (op0, op1, then_edge);
		break;

	      case NE:
		/* OP0 NE OP1: True -> THEN_EDGE, False -> ELSE_EDGE
		   is equivalent to
		   OP0 EQ OP1: True -> ELSE_EDGE, False -> THEN_EDGE.  */
		process_ranges_eq (op0, op1, else_edge);
		break;

	      case LT:
		process_ranges_lt (op0, op1, then_edge, else_edge);
		break;

	      case GE:
		/* OP0 GE OP1: True -> THEN_EDGE, False -> ELSE_EDGE
		   is equivalent to
		   OP0 LT OP1: True -> ELSE_EDGE, False -> THEN_EDGE.  */
		process_ranges_lt (op0, op1, else_edge, then_edge);
		break;

	      case GT:
		/* OP0 GT OP1: True -> THEN_EDGE, False -> ELSE_EDGE
		   is equivalent to
		   OP1 LT OP0: True -> THEN_EDGE, False -> ELSE_EDGE.  */
		process_ranges_lt (op1, op0, then_edge, else_edge);
		break;

	      case LE:
		/* OP0 LE OP1: True -> THEN_EDGE, False -> ELSE_EDGE
		   is equivalent to
		   OP0 GT OP1: True -> ELSE_EDGE, False -> THEN_EDGE
		   is equivalent to
		   OP1 LT OP0: True -> ELSE_EDGE, False -> THEN_EDGE.  */
		process_ranges_lt (op1, op0, else_edge, then_edge);
		break;

	      case LTU:
		process_ranges_ltu (op0, op1, then_edge, else_edge);
		break;

	      case GEU:
		/* Similarly to GE.  */
		process_ranges_ltu (op0, op1, else_edge, then_edge);
		break;

	      case GTU:
		/* Similarly to GT.  */
		process_ranges_ltu (op1, op0, then_edge, else_edge);
		break;

	      case LEU:
		/* Similarly to LE.  */
		process_ranges_ltu (op1, op0, else_edge, then_edge);
		break;

	      default:
		break;
	    }
	}

      if (!changed)
	{
	  changed = compare_register_tables (old_then, EI (then_edge)->htab);
	  if (!changed)
	      changed = compare_register_tables (old_else, EI (else_edge)->htab);

	  htab_delete (old_then);
	  htab_delete (old_else);
	}
    }

  return changed;
}

/* Find operations in a single insn pattern PATTERN and insert them to the
   linked list DATA->OPERATION_LIST.  STORE is the destination of the store.  */

static void
find_operations_1 (store, pattern, data)
     rtx store;
     rtx pattern;
     void *data;
{
  bb_info *bbi = data;
  rtx set_src, set_dst;
  operation oper;

  set_dst = SET_DEST (pattern);
  if (GET_CODE (store) == REG)
    {
      if (set_dst != store || GET_CODE (pattern) == CLOBBER)
	{
	  oper = pool_alloc (operation_pool);
	  oper->next = bbi->operation_list;
	  oper->type = UNKNOWN_TO_REG;
	  oper->dst = store;
	  bbi->operation_list = oper;
	  return;
	}

      set_src = SET_SRC (pattern);
      switch (GET_CODE (set_src))
	{
	  case REG:
	    oper = pool_alloc (operation_pool);
	    oper->next = bbi->operation_list;
	    oper->type = REG_TO_REG;
	    oper->dst = set_dst;
	    oper->src = set_src;
	    bbi->operation_list = oper;
	    return;

	  case CONST_INT:
	    oper = pool_alloc (operation_pool);
	    oper->next = bbi->operation_list;
	    oper->type = CONST_INT_TO_REG;
	    oper->dst = set_dst;
	    oper->src = set_src;
	    bbi->operation_list = oper;
	    return;

	  default:
	    oper = pool_alloc (operation_pool);
	    oper->next = bbi->operation_list;
	    oper->type = UNKNOWN_TO_REG;
	    oper->dst = set_dst;
	    bbi->operation_list = oper;
	    return;
	}
    }
}

/* Find operations in basic block BB and store it to the linked list of
   operations.  */

static void
find_operations (bb)
     basic_block bb;
{
  rtx insn;
  operation oper;

  /* Scan the insns from the end of BB to the beginning of BB because we are
     adding the opeations to the head of the chain.  */
  for (insn = bb->end; insn != PREV_INSN (bb->head); insn = PREV_INSN (insn))
    {
      if (INSN_P (insn))
	{
	  rtx link;

	  for (link = REG_NOTES (insn); link; link = XEXP (link, 1))
	    if (REG_NOTE_KIND (link) == REG_DEAD)
	      {
		oper = pool_alloc (operation_pool);
		oper->next = BBI (bb)->operation_list;
		oper->type = REG_IS_DEAD;
		oper->dst = XEXP (link, 0);
		BBI (bb)->operation_list = oper;
	      }

	  note_stores (PATTERN (insn), find_operations_1, BBI (bb));
	}

      if (GET_CODE (insn) == CALL_INSN)
	{
	  /* Set the range to be a full range for the registers
	     which do not survive during the call.  */

	  oper = pool_alloc (operation_pool);
	  oper->next = BBI (bb)->operation_list;
	  oper->type = CLOBBER_CALL_USED_REGS;
	  BBI (bb)->operation_list = oper;
	}
    }
}

/* Set the range of register *SLOT to be a full range
   if the register is in CALL_USED_REG_SET (i.e. delete such a register from
   hash table DATA.  */

static int
delete_call_used_regs (slot, data)
     void **slot;
     void *data;
{
  htab_t htab = data;
  range_t *r = *slot;
  int regno;

  regno = REGNO (r->reg);
  if (regno < FIRST_PSEUDO_REGISTER
      && TEST_HARD_REG_BIT (call_used_reg_set, regno))
    {
      htab_clear_slot (htab, slot);
    }

  /* Continue traversing.  */
  return 1;
}

/* Compute the effect of insns in block BB.  Update the edges going out
   from BB.  Return true if any range changes.  */

static bool
compute_ranges_for_bb (bb)
     basic_block bb;
{
  bool changed;
  htab_t new_out;
  operation oper;
  range_t *dst_r, *src_r;
  unsigned HOST_WIDE_INT mask;
  void **slot;

  copy_dead_registers = false;
  new_out = htab_create (BBI (bb)->in->n_elements, range_hash, range_eq,
			 range_del);
  copy_register_table (new_out, BBI (bb)->in);

  for (oper = BBI (bb)->operation_list; oper; oper = oper->next)
    {
      switch (oper->type)
	{
	  case REG_IS_DEAD:
	    dst_r = htab_find_with_hash (new_out, oper->dst,
					 HASH_REG (oper->dst));
	    if (dst_r)
	      dst_r->dead = true;
	    break;

	  case CLOBBER_CALL_USED_REGS:
	    htab_traverse (new_out, delete_call_used_regs, new_out);
	    break;

	  case UNKNOWN_TO_REG:
	    slot = htab_find_slot_with_hash (new_out, oper->dst,
					     HASH_REG (oper->dst), NO_INSERT);
	    if (slot)
	      htab_clear_slot (new_out, slot);
	    break;

	  case REG_TO_REG:
	    src_r = htab_find_with_hash (new_out, oper->src,
					 HASH_REG (oper->src));
	    if (src_r)
	      {
		/* Copy the range from SET_SRC to SET_DST.  */
		slot = htab_find_slot_with_hash (new_out, oper->dst,
						 HASH_REG (oper->dst),
						 NO_INSERT);

		if (slot)
		  dst_r = *slot;
		else
		  {
		    dst_r = pool_alloc (range_pool);
		    slot = htab_find_slot_with_hash (new_out, oper->dst,
						     HASH_REG (oper->dst),
						     INSERT);
		    *slot = dst_r;
		  }

		*dst_r = *src_r;
		dst_r->reg = oper->dst;
		dst_r->dead = false;
	      }
	    else
	      {
		/* SET_SRC has a full range.  */
		slot = htab_find_slot_with_hash (new_out, oper->dst,
						 HASH_REG (oper->dst),
						 NO_INSERT);
		if (slot)
		  htab_clear_slot (new_out, slot);
	      }
	    break;

	  case CONST_INT_TO_REG:
	    /* Set the range of SET_DST to be a single value.  */
	    slot = htab_find_slot_with_hash (new_out, oper->dst,
					     HASH_REG (oper->dst), NO_INSERT);
	    if (slot)
	      dst_r = *slot;
	    else
	      {
		dst_r = pool_alloc (range_pool);
		slot = htab_find_slot_with_hash (new_out, oper->dst,
						 HASH_REG (oper->dst), INSERT);
		*slot = dst_r;
	      }

	    /* We do not know whether the constant is signed or unsigned.  */
	    mask = MAX_UINT_VALUE (oper->dst);
	    dst_r->type = RANGE_SIGNED_INT | RANGE_UNSIGNED_INT;
	    dst_r->dead = false;
	    dst_r->reg = oper->dst;
	    dst_r->from.si = INTVAL (oper->src);
	    dst_r->to.si = INTVAL (oper->src);
	    dst_r->from.ui = (unsigned HOST_WIDE_INT) INTVAL (oper->src) & mask;
	    dst_r->to.ui = (unsigned HOST_WIDE_INT) INTVAL (oper->src) & mask;
	    break;
	}
    }

  changed = compare_register_tables (new_out, BBI (bb)->out);

  htab_delete (BBI (bb)->out);
  BBI (bb)->out = new_out;

  changed |= update_outgoing_edges (bb, changed);

  return changed;
}

/* Compute ranges for the whole function.  */

static void
compute_ranges ()
{
  fibheap_t worklist, pending, fibheap_swap;
  sbitmap visited, in_worklist, in_pending, sbitmap_swap;
  basic_block bb;
  edge e;
  int *bb_order;
  int *rc_order;
  int i;

  /* Compute reverse completion order of depth first search of the CFG
     so that the data-flow runs faster.  */
  rc_order = (int *) xmalloc (n_basic_blocks * sizeof (int));
  bb_order = (int *) xmalloc (last_basic_block * sizeof (int));
  flow_depth_first_order_compute (NULL, rc_order);
  for (i = 0; i < n_basic_blocks; i++)
    bb_order[rc_order[i]] = i;
  free (rc_order);

  worklist = fibheap_new ();
  pending = fibheap_new ();
  visited = sbitmap_alloc (last_basic_block);
  in_worklist = sbitmap_alloc (last_basic_block);
  in_pending = sbitmap_alloc (last_basic_block);
  sbitmap_zero (in_worklist);
  sbitmap_zero (in_pending);

  FOR_EACH_BB (bb)
    {
      fibheap_insert (pending, bb_order[bb->index], bb);
      SET_BIT (in_pending, bb->index);
    }

  while (!fibheap_empty (pending))
    {
      fibheap_swap = pending;
      pending = worklist;
      worklist = fibheap_swap;
      sbitmap_swap = in_pending;
      in_pending = in_worklist;
      in_worklist = sbitmap_swap;

      sbitmap_zero (visited);

      while (!fibheap_empty (worklist))
	{
	  bb = fibheap_extract_min (worklist);
	  RESET_BIT (in_worklist, bb->index);
	  if (!TEST_BIT (visited, bb->index))
	    {
	      bool changed;

	      SET_BIT (visited, bb->index);

	      /* Calculate the IN set as union of predecessor OUT sets.  */
	      if ((e = bb->pred) != NULL)
		{
		  copy_dead_registers = false;
		  copy_register_table (BBI (bb)->in, EI (e)->htab);
		  for (e = e->pred_next; e; e = e->pred_next)
		    union_all_ranges (BBI (bb)->in, EI (e)->htab);
		}

	      changed = compute_ranges_for_bb (bb);
	      if (changed)
		{
		  for (e = bb->succ; e; e = e->succ_next)
		    {
		      if (e->dest == EXIT_BLOCK_PTR)
			continue;

		      if (e->dest == bb)
			continue;

		      if (TEST_BIT (visited, e->dest->index))
			{
			  if (!TEST_BIT (in_pending, e->dest->index))
			    {
			      /* Send E->DEST to next round.  */
			      SET_BIT (in_pending, e->dest->index);
			      fibheap_insert (pending, bb_order[e->dest->index],
					      e->dest);
			    }
			}
		      else if (!TEST_BIT (in_worklist, e->dest->index))
			{
			  /* Add E->DEST to current round.  */
			  SET_BIT (in_worklist, e->dest->index);
			  fibheap_insert (worklist, bb_order[e->dest->index],
					  e->dest);
			}
		    }
		}
	    }
	}
    }

  free (bb_order);
  fibheap_delete (worklist);
  fibheap_delete (pending);
  sbitmap_free (visited);
  sbitmap_free (in_worklist);
  sbitmap_free (in_pending);
}

/* Return true if edge E is dead (we will never go though this edge because
   of the conditions), i.e. (at least) one of the register ranges is empty.  */

static bool
edge_is_dead (e)
     edge e;
{
  rtx cond = BBI (e->src)->cond;
  rtx op;
  int i;

#ifdef ENABLE_CHECKING
  if (cond == NULL)
    abort ();
#endif

  for (i = 0; i < 2; i++)
    {
      op = XEXP (cond, i);
      if (GET_CODE (op) == REG)
	{
	  range_t *r;

	  r = htab_find_with_hash (EI (e)->htab, op, HASH_REG (op));
	  if (r)
	    {
	      switch (r->type)
		{
		  case RANGE_SIGNED_INT:
		    if (RANGE_EMPTY_P (r, si))
		      return true;
		    break;
		    
		  case RANGE_UNSIGNED_INT:
		    if (RANGE_EMPTY_P (r, ui))
		      return true;
		    break;
		    
		  case RANGE_SIGNED_INT | RANGE_UNSIGNED_INT:
		    if (RANGE_EMPTY_P (r, si) && RANGE_EMPTY_P (r, ui))
		      return true;
		    break;
		}
	    }
	}
    }

  return false;
}

/* Redirect dead edges to the other edges.  */

static bool
redirect_edges ()
{
  bool modified = false;
  basic_block bb;

  FOR_EACH_BB (bb)
    {
      if (any_condjump_p (bb->end)
	  && BBI (bb)->cond != NULL)
	{
	  edge then_edge = BRANCH_EDGE (bb);
	  edge else_edge = FALLTHRU_EDGE (bb);
	  bool then_dead;
	  bool else_dead;

	  if (then_edge->dest == EXIT_BLOCK_PTR
	      || else_edge->dest == EXIT_BLOCK_PTR)
	    continue;

	  /* If both edges are dead BB is dead too so it will be removed
	     and thus destinations of both edges too.
	     So perform the redirection only when there is exactly one dead
	     edge.  */

	  then_dead = edge_is_dead (then_edge);
	  else_dead = edge_is_dead (else_edge);
	  if (then_dead && !else_dead)
	    {
	      then_edge->dest->flags |= BB_DIRTY;
	      if (rtl_dump_file)
		{
		  fprintf (rtl_dump_file, "Redirecting edge %d->%d to %d\n",
			   then_edge->src->index, then_edge->dest->index,
			   else_edge->dest->index);
		}

	      if (!redirect_edge_and_branch (then_edge, else_edge->dest))
		abort ();

	      modified = true;
	    }
	  else if (!then_dead && else_dead)
	    {
	      else_edge->dest->flags |= BB_DIRTY;
	      if (rtl_dump_file)
		{
		  fprintf (rtl_dump_file, "Redirecting edge %d->%d to %d\n",
			   else_edge->src->index, else_edge->dest->index,
			   then_edge->dest->index);
		}

	      if (!redirect_edge_and_branch (else_edge, then_edge->dest))
		abort ();

	      modified = true;
	    }
	}
    }

  return modified;
}

/* Print a range *SLOT to file DATA.  */

static int
dump_range (slot, data)
     void **slot;
     void *data;
{
  FILE *file = data;
  range_t *r;

  if (!slot)
    return 1;

  r = *slot;

  print_inline_rtx (file, r->reg, 2);
  fprintf (file, "\n");

  if (r->type & RANGE_SIGNED_INT)
    {
      fprintf (file, "  signed [");
      fprintf (file, HOST_WIDE_INT_PRINT_DEC, r->from.si);
      fprintf (file, ", ");
      fprintf (file, HOST_WIDE_INT_PRINT_DEC, r->to.si);
      fprintf (file, "]\n");
    }

  if (r->type & RANGE_UNSIGNED_INT)
    {
      fprintf (file, "  unsigned [");
      fprintf (file, HOST_WIDE_INT_PRINT_UNSIGNED, r->from.ui);
      fprintf (file, ", ");
      fprintf (file, HOST_WIDE_INT_PRINT_UNSIGNED, r->to.ui);
      fprintf (file, "]\n");
    }

  if (r->dead)
    fprintf (file, "  DEAD\n");

  return 1;
}

/* Print all ranges in table HTAB to file FILE.  */

static void
dump_all_ranges (file, htab)
     FILE *file;
     htab_t htab;
{
  htab_traverse (htab, dump_range, file);
  fprintf (file, "\n");
}

/* Print a range R to STDERR.  */

void
debug_range (r)
     range_t *r;
{
  void *x = (void *) r;

  dump_range (&x, stderr);
}

/* Print all ranges in table HTAB to STDERR.  */

void
debug_all_ranges (htab)
     htab_t htab;
{
  dump_all_ranges (stderr, htab);
}

/* Print the operations in basic block BB to file FILE.  */

static void
dump_operation_list (file, bb)
     FILE *file;
     basic_block bb;
{
  operation l;

  for (l = BBI (bb)->operation_list; l; l = l->next)
    {
      switch (l->type)
	{
	  case CONST_INT_TO_REG:
	    fprintf (file, "REG %d <- ", REGNO (l->dst));
	    fprintf (file, HOST_WIDE_INT_PRINT_UNSIGNED, INTVAL (l->src));
	    fprintf (file, "\n");
	    break;

	  case REG_TO_REG:
	    fprintf (file, "REG %d <- REG %d\n", REGNO (l->dst),
		     REGNO (l->src));
	    break;

	  case UNKNOWN_TO_REG:
	    fprintf (file, "REG %d <- UNKNOWN\n", REGNO (l->dst));
	    break;

	  case CLOBBER_CALL_USED_REGS:
	    fprintf (file, "CALL\n");
	    break;

	  case REG_IS_DEAD:
	    fprintf (file, "REG %d is DEAD\n", REGNO (l->dst));
	    break;

	  default:
	    abort ();
	}
    }
}

/* Print the operations in basic block BB to STDERR.  */

void
debug_operation_list (bb)
     basic_block bb;
{
  dump_operation_list (stderr, bb);
}

/* Compute the value ranges and remove unreachable edges.
   The main entry point of this file.  */

bool
value_range_propagation ()
{
  bool modified;
  basic_block bb;
  edge e;

  /* Initialization.  */
  operation_pool = create_alloc_pool ("operation",
				      sizeof (struct operation_def), 1000);
  range_pool = create_alloc_pool ("range_t", sizeof (range_t), 250);
  alloc_aux_for_blocks (sizeof (bb_info));
  alloc_aux_for_edges (sizeof (edge_info));

  FOR_ALL_BB (bb)
    {
      BBI (bb)->operation_list = NULL;
      BBI (bb)->in = htab_create (5, range_hash, range_eq, range_del);
      BBI (bb)->out = htab_create (5, range_hash, range_eq, range_del);
      for (e = bb->succ; e; e = e->succ_next)
	EI (e)->htab = htab_create (5, range_hash, range_eq, range_del);
    }
  BBI (ENTRY_BLOCK_PTR)->cond = NULL;
  BBI (EXIT_BLOCK_PTR)->cond = NULL;
  FOR_EACH_BB (bb)
    {
      BBI (bb)->cond = get_condition (bb->end, NULL);
      find_operations (bb);
    }

  if (rtl_dump_file)
    {
      dump_flow_info (rtl_dump_file);
    }

  compute_ranges ();

  if (rtl_dump_file)
    {
      FOR_EACH_BB (bb)
	{
	  fprintf (rtl_dump_file, "Status at the end of BB %d:\n", bb->index);
	  dump_all_ranges (rtl_dump_file, BBI (bb)->out);
	}

      FOR_EACH_BB (bb)
	{
	  for (e = bb->succ; e; e = e->succ_next)
	    {
	      fprintf (rtl_dump_file, "Status on edge %d -> %d:\n",
		       e->src->index, e->dest->index);
	      dump_all_ranges (rtl_dump_file, EI (e)->htab);
	    }
	}
    }

  modified = redirect_edges ();

  /* Cleanup.  */
  FOR_ALL_BB (bb)
    {
      htab_delete (BBI (bb)->in);
      htab_delete (BBI (bb)->out);
      for (e = bb->succ; e; e = e->succ_next)
	htab_delete (EI (e)->htab);
    }

  free_aux_for_edges ();
  free_aux_for_blocks ();
  free_alloc_pool (range_pool);
  free_alloc_pool (operation_pool);

  if (modified)
    update_life_info_in_dirty_blocks (UPDATE_LIFE_GLOBAL_RM_NOTES,
				      PROP_DEATH_NOTES | PROP_SCAN_DEAD_CODE
				      | PROP_KILL_DEAD_CODE | PROP_LOG_LINKS);
  return modified;
}
