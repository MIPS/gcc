/* Merge constant bitfield stores.
   Copyright (C) 2007 Cavium Networks.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

/* This pass merges adjacent or overlapping (in case of unions)
   constant bitfield stores.  We try to merge stores into preceding
   stores.  In case the whole structure is zeroed out initially, by
   merging backward, we can possibly merge into this
   initialization.

   Requirements:

   1. order shouldn't matter:

     x.b = 2;
     x.a = 3;
     x.c = 4;

  2. merge into initialization (with a union example):

     union { uint64_t l; struct { uint64_t a:2; uint64_t b:2 ... } s } u;
     u.l = 0;
     ...
     u.s.b = 2;

  3. merge should happen even if use is in a different bb:

     x.a = 2;
     x.b = 3;
     if ()
       use (x);

  4. don't give up a non-overlapping varying stores:

     extern int i;
     x.a = 2;
     x.c = i;
     x.b = 3;

  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "stor-layout.h"
#include "flags.h"
#include "tm_p.h"
#include "target.h"
#include "langhooks.h"
#include "basic-block.h"
#include "function.h"
#include "gimple-pretty-print.h"
#include "pointer-set.h"
#include "tree-ssa-alias.h"
#include "internal-fn.h"
#include "gimple-fold.h"
#include "gimple-expr.h"
#include "is-a.h"
#include "gimple.h"
#include "gimplify.h"
#include "gimple-iterator.h"
#include "gimple-walk.h"
#include "gimple-ssa.h"
#include "tree-phinodes.h"
#include "ssa-iterators.h"
#include "stringpool.h"
#include "tree-ssanames.h"
#include "tree-ssa-loop-manip.h"
#include "tree-into-ssa.h"
#include "tree-ssa.h"
#include "tree-inline.h"
#include "hashtab.h"
#include "tree-pass.h"
#include "diagnostic-core.h"
#include "cfgloop.h"
#include "cfgexpand.h"


#include "varasm.h"
#include "stor-layout.h"
#include "pointer-set.h"
#include "hash-table.h"
#include "basic-block.h"



/* Structure to keep track of constant bit-field stores.  */

typedef struct const_bfstore
{
  tree inner;
  HOST_WIDE_INT bitsize;
  HOST_WIDE_INT bitpos;
  tree constval;
  gimple stmt;
} const_bfstore;

static vec<const_bfstore> bfstores;

/* Build an unsigned BIT_FIELD_REF expression.  */

static tree
make_unsigned_bitfield_ref (tree inner, HOST_WIDE_INT bitsize,
			    HOST_WIDE_INT bitpos)
{
  enum machine_mode mode;
  tree exp, unsigned_type;

  mode = get_best_mode (bitsize, bitpos, 0, 0,
			TYPE_ALIGN (TREE_TYPE (inner)), word_mode, 0);
  gcc_assert (mode != VOIDmode);

  unsigned_type = build_nonstandard_integer_type (bitsize, 1);
  exp = build3 (BIT_FIELD_REF, unsigned_type, inner, size_int (bitsize),
		bitsize_int (bitpos));
  return exp;
}

/* Return true iff the bitfields are overlapping.  */

static bool
overlapping_bitfields (HOST_WIDE_INT bitsize0, HOST_WIDE_INT bitpos0,
		       HOST_WIDE_INT bitsize1, HOST_WIDE_INT bitpos1)
{
  return (bitpos0 < bitpos1
	  ? bitpos1 < bitpos0 + bitsize0
	  : bitpos0 < bitpos1 + bitsize1);
}

/* A bitfield store of OBITSIZE starting at OBITPOS is followed by a
   bitfield store of NBITSIZE at NBITPOS, compute the new *BITPOS and
   return the new bitsize or -1 if the stores aren't either adjacent
   or overlapping.  Also compute *oshift and *nshift which is the
   number bits the constants need to be shifted to the left before the
   RHS bits are combined.  */

static HOST_WIDE_INT
compute_new_bitfield_positions (HOST_WIDE_INT obitsize, HOST_WIDE_INT obitpos,
				HOST_WIDE_INT nbitsize, HOST_WIDE_INT nbitpos,
				HOST_WIDE_INT *bitpos,
				HOST_WIDE_INT *oshift, HOST_WIDE_INT *nshift)
{
  HOST_WIDE_INT bitsize;

  *bitpos = MIN (obitpos, nbitpos);
  bitsize = MAX (obitpos + obitsize, nbitpos + nbitsize) - *bitpos;

  if (BYTES_BIG_ENDIAN)
    *oshift = *bitpos + bitsize - (obitpos + obitsize);
  else
    *oshift = *bitpos + obitpos;

  if (*oshift > nbitsize)
    return -1;
    
  if (BYTES_BIG_ENDIAN)
    *nshift = *bitpos + bitsize - (nbitpos + nbitsize);
  else
    *nshift = *bitpos + nbitpos;

  if (*nshift > obitsize)
    return -1;

  return bitsize;
}

/* This is used as an early check.  Return true iff given the
   alignment ALIGN and two bitfield references with BITSIZE0/BITPOS0
   and BITSIZE1/BITPOS1 we will be able to combine the constant stores
   to these locations.  The get_best_mode call at the end makes sure
   that we never combine more than what fits in word_mode.  */

static HOST_WIDE_INT
combinable_bitfields (unsigned int align,
		      HOST_WIDE_INT bitsize0, HOST_WIDE_INT bitpos0,
		      HOST_WIDE_INT bitsize1, HOST_WIDE_INT bitpos1)
{
  HOST_WIDE_INT bitsize, bitpos, oshift, nshift;

  bitsize =
    compute_new_bitfield_positions (bitsize0, bitpos0, bitsize1, bitpos1,
				    &bitpos, &oshift, &nshift);
  return (bitsize != -1
	  && bitpos + bitsize <= GET_MODE_BITSIZE (word_mode)
	  && get_best_mode (bitsize, bitpos, 0, 0, align, word_mode, 0) != VOIDmode);
}

/* Wrap get_inner_reference to only return information relevant for
   bit-field refs on the LHS.  Return NULL_TREE iff this is not a
   bit-field reference.  */

static tree
get_lhs_bitfield_reference (tree exp, HOST_WIDE_INT *pbitsize,
			    HOST_WIDE_INT *pbitpos, int *pvolatilep)
{
  tree inner, offset;
  enum machine_mode mode;
  int unsignedp;

  *pvolatilep = 0;
  inner = get_inner_reference (exp, pbitsize, pbitpos, &offset, &mode,
			       &unsignedp, pvolatilep, false);
  if (inner == exp || *pbitsize < 0 || offset != 0
      || TREE_CODE (inner) == PLACEHOLDER_EXPR)
    return NULL_TREE;

  return inner;
}

/* Update stmt in an entry P of bfstores to include the constant
   CONSTVAL.  Note that overlapping bits should get their value from
   entry.  We can change OSTMT, OBITSIZE, OBITPOS, OCONSTVAL. */

static void
merge_one_bitfield_store (const_bfstore *p, gimple ostmt, HOST_WIDE_INT *bitsize,
			  HOST_WIDE_INT *bitpos, tree *constval)
{
  tree bitfield;
  HOST_WIDE_INT nshift, oshift, nbitpos = p->bitpos, nbitsize = p->bitsize,
    obitsize = *bitsize, obitpos = *bitpos;
  tree nconstval = p->constval, oconstval = *constval;
  gimple nstmt = p->stmt;
  tree type, oconst, nconst, mask, one, minus_one;
  gimple_stmt_iterator gsi;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "  ** REPLACING THIS **\n");
      print_gimple_stmt (dump_file, ostmt, 0, dump_flags);
      fprintf (dump_file, "\n  ** AND THIS **\n");
      print_gimple_stmt (dump_file, nstmt, 0, dump_flags);
    }

  *bitsize = compute_new_bitfield_positions (obitsize, obitpos,
					     nbitsize, nbitpos,
					     bitpos, &oshift, &nshift);
  gcc_assert (*bitsize > 0);

  bitfield = make_unsigned_bitfield_ref (p->inner, *bitsize, *bitpos);

  type = TREE_TYPE (bitfield);
  /* Always zero-extend to the new type.  */
  oconst = fold_convert (type, oconstval);
  oconst = int_const_binop (BIT_AND_EXPR, oconst,
			    build_low_bits_mask (type, obitsize));
  nconst = fold_convert (type, nconstval);
  nconst = int_const_binop (BIT_AND_EXPR, nconst,
			    build_low_bits_mask (type, nbitsize));

  one = fold_convert (type, integer_one_node);
  minus_one = fold_convert (type, integer_minus_one_node);

  if (oshift)
    oconst = int_const_binop (LSHIFT_EXPR, oconst,
			    build_int_cst (type, oshift));
  if (nshift)
    nconst = int_const_binop (LSHIFT_EXPR, nconst,
			    build_int_cst (type, nshift));

  /* Mask off the bits from old value that the new value overrides.  The mask
     is -1 ^ (((1 << NBITSIZE) - 1) << NSHIFT).  */
  mask = build_low_bits_mask (type, nbitsize);
  if (nshift)
    mask = int_const_binop (LSHIFT_EXPR, mask,
			    build_int_cst (type, nshift));
  mask = int_const_binop (BIT_XOR_EXPR, minus_one, mask);
  oconst = int_const_binop (BIT_AND_EXPR, oconst, mask);
  *constval = int_const_binop (BIT_IOR_EXPR, oconst, nconst);

  gimple_set_lhs (ostmt, bitfield);
  gimple_assign_set_rhs1 (ostmt, *constval);
  update_stmt (ostmt);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "\n  ** WITH THIS **\n");
      print_gimple_stmt (dump_file, ostmt, 0, dump_flags);
      fprintf (dump_file, "\n\n");
    }

  gsi = gsi_for_stmt (nstmt);
  unlink_stmt_vdef (nstmt);
  gsi_remove (&gsi, true);
  release_defs (nstmt);
}

/* Record a new bit-field store describe with base expression INNER,
   BITSIZE, BITPOS and the STMT where the store occurs.  */

static void
record_new_const_bfstore (tree inner, HOST_WIDE_INT bitsize,
			  HOST_WIDE_INT bitpos, gimple stmt)
{
  const_bfstore p;
      
  p.inner = inner;
  p.bitsize = bitsize;
  p.bitpos = bitpos;
  p.constval = gimple_assign_rhs1 (stmt);
  p.stmt = stmt;
  bfstores.safe_push (p);
}

/* Decide based on vops whether STMT conflicts with a later STORE.  */

static bool
vops_conflict_with_store_p (gimple stmt, const_bfstore *store)
{
  /* Use the aliasing oracle to decide if the stmt conflicts with the store. */
  return gimple_vuse (stmt) && ref_maybe_used_by_stmt_p (stmt, gimple_get_lhs (store->stmt));
}

/* If STMT is a bfstore look through existing stores and if the base
   expression matches update/combine the stores.  If the base
   expression is different we still have to invalidate entries based
   on vops.  */

static bool
merge_bitfield_stores (gimple stmt)
{
  tree lhs, rhs, inner;
  HOST_WIDE_INT bitsize, bitpos;
  int volatilep;
  unsigned i;
  const_bfstore *store;

  if (!gimple_assign_copy_p (stmt))
    return false;

  lhs = gimple_get_lhs (stmt);
  rhs = gimple_assign_rhs1 (stmt);
  if (TREE_CODE (lhs) != COMPONENT_REF
      && TREE_CODE (lhs) != BIT_FIELD_REF)
    return false;

  inner = get_lhs_bitfield_reference (lhs, &bitsize, &bitpos, &volatilep);

  if (!inner || (TREE_CODE (rhs) == INTEGER_CST && volatilep))
    return false;

  for (i = 0; bfstores.iterate (i, &store); i++)
    {
      /* If we don't know how these are related we don't combine them
	 and for invalidation we resort to vops.  Do similarly, if we
	 can potentially have a conflicting bitfield use on the RHS.
	 This is somewhat conservative but given how gimple is
	 generate this does not happen *at all* in practice, still be
	 paranoid.  */
      if (!operand_equal_p (inner, store->inner, 0)
	  || TREE_CODE (rhs) == COMPONENT_REF
	  || TREE_CODE (rhs) == BIT_FIELD_REF)
	{
	  if (vops_conflict_with_store_p (stmt, store))
	    bfstores.unordered_remove (i--);

	  continue;
	}

      if (TREE_CODE (rhs) != INTEGER_CST)
	{
	  /* Instead of invalidating the entry for a varying store we
	     could adjust the constant store entry but this does not
	     seem to be happening in our code.  */
	  if (overlapping_bitfields (bitsize, bitpos,
				     store->bitsize, store->bitpos))
	    bfstores.unordered_remove (i--);

	  continue;
	}

      if (!combinable_bitfields (TYPE_ALIGN (TREE_TYPE (inner)),
				 bitsize, bitpos,
				 store->bitsize, store->bitpos))
	  continue;

      merge_one_bitfield_store (store, stmt, &bitsize, &bitpos, &rhs);
      bfstores.unordered_remove (i--);
    }

  if (TREE_CODE (rhs) == INTEGER_CST)
    record_new_const_bfstore (inner, bitsize, bitpos, stmt);

  return true;
}

/* Invalidate stores that can't be moved before STMT.  We do this
   based on VOPS we is more conservative than it should be.  Consider
   this:

     union { uint64_t l; struct { uint64_t a:2; uint64_t b:2 } s } u;
     u.l = 0; i = u.s.a; u.s.a = 1; u.s.b = 2;

   We will not merge u.s.b = 2 into the intialization because the use
   of the vops on i = u.s.a will conflict with it.  */

static void
invalidate_bfstores (gimple stmt)
{
  unsigned i;
  const_bfstore *store;

  for (i = 0; bfstores.iterate (i, &store);)
    if (vops_conflict_with_store_p (stmt, store))
      bfstores.unordered_remove (i);
    else
      i++;
}


namespace {

const pass_data pass_data_merge_const_bfstores =
{
  GIMPLE_PASS, /* type */
  "constbfstores", /* name */
  OPTGROUP_NONE, /* optinfo_flages */
  TV_MERGE_CONST_BFSTORES, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  TODO_update_ssa_only_virtuals, /* todo_flags_finish */
};

class pass_merge_const_bfstores : public gimple_opt_pass
{
public:
  pass_merge_const_bfstores (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_merge_const_bfstores, ctxt)
  {}

  /* opt_pass methods: */
  opt_pass * clone () { return new pass_merge_const_bfstores (m_ctxt); }
  virtual bool gate (function*) { return flag_merge_const_bfstores; }
  virtual unsigned int execute (function*);

}; // classs pass_merge_const_bfstores
  
} // anon namespace
/* Main entry point of the pass.  */

unsigned int
pass_merge_const_bfstores::execute (function*)
{
  basic_block bb;

  FOR_EACH_BB_FN (bb, cfun)
    {
      gimple_stmt_iterator gsi;

      bfstores.truncate (0);

      /* Walk statements backward so that if for example the whole
	 struct is zeroed out initially we merge all the constant
	 bitfield stores into the initialization.  */
      for (gsi = gsi_last_bb (bb); !gsi_end_p (gsi); gsi_prev (&gsi))
	{
	  gimple stmt = gsi_stmt (gsi);
	  /* Look for varying stores, note constant stores and try to
	     merge them.  */
	  if (!merge_bitfield_stores (stmt))
	    /* Look for uses that would prevent moving the store.  */
	    invalidate_bfstores (stmt);
	}
    }

  bfstores.truncate (0);
  return 0;
}

gimple_opt_pass * 
make_pass_merge_const_bfstores (gcc::context *ctxt)
{
  return new pass_merge_const_bfstores (ctxt);
}


