/* Prefetching.
   Copyright (C) 2003 Free Software Foundation, Inc.

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "cfgloop.h"
#include "output.h"
#include "insn-config.h"
#include "recog.h"
#include "expr.h"
#include "optabs.h"

/* ??? It might be fine to take unrolling into account.  On unrolled loops
   scheduling will decrease cost of superfluous prefetches anyway, but with a
   little effort we could run decision for unrolling before prefetching and
   ignore prefetch instructions inserted here during unrolling.  */

/* Not really meaningful values, but at least something.  */
#ifndef SIMULTANEOUS_PREFETCHES
#define SIMULTANEOUS_PREFETCHES 3
#endif
#ifndef PREFETCH_BLOCK
#define PREFETCH_BLOCK 32
#endif
#ifndef HAVE_prefetch
#define HAVE_prefetch 0
#define CODE_FOR_prefetch 0
#define gen_prefetch(a,b,c) (abort(), NULL_RTX)
#endif

/* Give up the prefetch optimizations once we exceed a given threshhold.
   It is unlikely that we would be able to optimize something in a loop
   with so many detected prefetches.  */
#define MAX_PREFETCHES 100
/* The number of prefetch blocks that are beneficial to fetch at once before
   a loop with a known (and low) iteration count.  */
#define PREFETCH_BLOCKS_BEFORE_LOOP_MAX  6
/* For very tiny loops it is not worthwhile to prefetch even before the loop,
   since it is likely that the data are already in the cache.  */
#define PREFETCH_BLOCKS_BEFORE_LOOP_MIN  2

/* Parameterize some prefetch heuristics so they can be turned on and off
   easily for performance testing on new architectures.  These can be
   defined in target-dependent files.  */

/* Prefetch is worthwhile only when loads/stores are dense.  */
#ifndef PREFETCH_ONLY_DENSE_MEM
#define PREFETCH_ONLY_DENSE_MEM 1
#endif

/* Define what we mean by "dense" loads and stores; This value divided by 256
   is the minimum percentage of memory references that worth prefetching.  */
#ifndef PREFETCH_DENSE_MEM
#define PREFETCH_DENSE_MEM 220
#endif

/* Do not prefetch for a loop whose iteration count is known to be low.  */
#ifndef PREFETCH_NO_LOW_LOOPCNT
#define PREFETCH_NO_LOW_LOOPCNT 1
#endif

/* Define what we mean by a "low" iteration count.  */
#ifndef PREFETCH_LOW_LOOPCNT
#define PREFETCH_LOW_LOOPCNT 32
#endif

/* Do not prefetch for a loop that contains a function call; such a loop is
   probably not an internal loop.  */
#ifndef PREFETCH_NO_CALL
#define PREFETCH_NO_CALL 1
#endif

/* Do not prefetch accesses with an extreme stride.  */
#ifndef PREFETCH_NO_EXTREME_STRIDE
#define PREFETCH_NO_EXTREME_STRIDE 1
#endif

/* Define what we mean by an "extreme" stride.  */
#ifndef PREFETCH_EXTREME_STRIDE
#define PREFETCH_EXTREME_STRIDE 4096
#endif

/* Define a limit to how far apart indices can be and still be merged
   into a single prefetch.  */
#ifndef PREFETCH_EXTREME_DIFFERENCE
#define PREFETCH_EXTREME_DIFFERENCE 4096
#endif

/* Issue prefetch instructions before the loop to fetch data to be used
   in the first few loop iterations.  */
#ifndef PREFETCH_BEFORE_LOOP
#define PREFETCH_BEFORE_LOOP 1
#endif

/* Do not handle reversed order prefetches (negative stride).  */
#ifndef PREFETCH_NO_REVERSE_ORDER
#define PREFETCH_NO_REVERSE_ORDER 1
#endif

/* Prefetch even if the GIV is in conditional code.  */
#ifndef PREFETCH_CONDITIONAL
#define PREFETCH_CONDITIONAL 1
#endif

/* Information we collect about arrays that we might want to prefetch.  */
struct prefetch_info
{
  struct iv_occurence *iv;	/* IV this prefetch is based on.  */
  rtx base_address;		/* Start prefetching from this address plus
				   index.  The values of registers are valid
				   at the end of the loop preheader.  */
  HOST_WIDE_INT index;
  HOST_WIDE_INT stride;		/* Prefetch stride in bytes in each
				   iteration.  */
  unsigned int bytes_accessed;	/* Sum of sizes of all accesses to this
				   prefetch area in one iteration.  */
  unsigned int total_bytes;	/* Total bytes loop will access in this block.
				   This is set only for loops with known
				   iteration counts and is 0xffffffff
				   otherwise.  */
  int prefetch_in_loop;		/* Number of prefetch insns in loop.  */
  int prefetch_before_loop;	/* Number of prefetch insns before loop.  */
  unsigned int write : 1;	/* 1 for read/write prefetches.  */
};

/* Data used by check_store function.  */
struct check_store_data
{
  rtx mem;
  int mem_write;
};

static void emit_prefetch_instructions PARAMS ((struct loops *, struct loop *));
static void check_store PARAMS ((rtx, rtx, void *));

/* Set mem_write when mem is found.  Used as callback to
   note_stores.  */
static void
check_store (x, pat, data)
     rtx x, pat ATTRIBUTE_UNUSED;
     void *data;
{
  struct check_store_data *d = (struct check_store_data *) data;

  if (x == d->mem)
    d->mem_write = 1;
}

/* Attempt to identify accesses to arrays that are most likely to cause cache
   misses, and emit prefetch instructions a few prefetch blocks forward.

   To detect the arrays we use the IV information collected by loop-iv.c.

   GIVs are split into base address, stride, and constant addition values.
   GIVs with the same address, stride and close addition values are combined
   into a single prefetch.  Also writes to GIVs are detected, so that prefetch
   for write instructions can be used for the block we write to, on machines
   that support write prefetches.

   Several heuristics are used to determine when to prefetch.  They are
   controlled by defined symbols that can be overridden for each target.  */

static void
emit_prefetch_instructions (loops, loop)
     struct loops *loops;
     struct loop *loop;
{
  unsigned num_prefetches = 0;
  unsigned num_real_prefetches = 0;
  unsigned num_real_write_prefetches = 0;
  unsigned num_prefetches_before = 0;
  unsigned num_write_prefetches_before = 0;
  unsigned ahead = 0;
  unsigned i;
  struct prefetch_info info[MAX_PREFETCHES];
  struct iv_occurence_step_class *iv_sc;
  struct iv_occurence_base_class *iv_bc;
  struct iv_occurence *iv_oc;
  basic_block *body, bb;
  rtx insn;

  if (!HAVE_prefetch)
    return;

  body = get_loop_body (loop);

  /* Consider only loops w/o calls.  When a call is done, the loop is probably
     slow enough to read the memory.  */
  if (PREFETCH_NO_CALL)
    {
      for (i = 0; i < loop->num_nodes; i++)
	{
	  bb = body[i];
	  FOR_BB_INSNS (bb, insn)
	    {
	      if (GET_CODE (insn) == CALL_INSN)
		{
		  if (rtl_dump_file)
		    fprintf (rtl_dump_file,
			     "Prefetch: ignoring loop: has call.\n");
		  free (body);
		  return;
		}
	    }
	}
    }
  free (body);

  /* Don't prefetch in loops known to have few iterations.  */
  if (PREFETCH_NO_LOW_LOOPCNT
      && loop->has_desc
      && loop->simple
      && loop->desc.const_iter
      && loop->desc.niter <= PREFETCH_LOW_LOOPCNT)
    {
      if (rtl_dump_file)
	fprintf (rtl_dump_file,
		 "Prefetch: ignoring loop: not enough iterations.\n");
      return;
    }

  /* Search all induction variables and pick those interesting for the prefetch
     machinery.  */
  for (iv_sc = iv_occurences[loop->num]; iv_sc; iv_sc = iv_sc->sc_next)
    for (iv_bc = iv_sc->bc_first; iv_bc; iv_bc = iv_bc->bc_next)
      for (iv_oc = iv_bc->oc_first; iv_oc; iv_oc = iv_oc->oc_next)
	{
	  HOST_WIDE_INT stride;
	  rtx address;
	  HOST_WIDE_INT index = 0;
	  int add = 1;
	  int stride_sign = 1;
	  struct check_store_data d;
	  const char *ignore_reason = NULL;
	  int size = GET_MODE_SIZE (GET_MODE (*iv_oc->occurence));

	  /* Ignore if it is not a memory access.  */
	  if (GET_CODE (*iv_oc->occurence) != MEM)
	    {
	      if (rtl_dump_file)
      		fprintf (rtl_dump_file,
			 "Prefetch: ignoring iv at insn %d -- not a memory address\n",
			 INSN_UID (iv_oc->insn));
	      continue;
	    }

	  /* Discard non-constant additions that we can't handle well yet.  */
	  if (GET_CODE (iv_sc->step) != CONST_INT)
	    {
	      if (rtl_dump_file)
		fprintf (rtl_dump_file,
			 "Prefetch: ignoring iv at insn %d -- non-constant step\n",
			 INSN_UID (iv_oc->insn));
	      continue;
	    }

	  stride = INTVAL(iv_sc->step);
	  if (!stride)
	    {
	      if (rtl_dump_file)
		fprintf (rtl_dump_file,
			 "Prefetch: ignoring iv at insn %d -- invariant\n",
			 INSN_UID (iv_oc->insn));
	      continue;
	    }

	  if (stride < 0)
	    {
	      stride = -stride;
	      stride_sign = -1;
	    }

	  /* On some targets, reversed order prefetches are not
	     worthwhile.  */
	  if (PREFETCH_NO_REVERSE_ORDER && stride_sign < 0)
	    ignore_reason = "reversed order stride";

	  /* Prefetch of accesses with an extreme stride might not be
	     worthwhile, either.  */
	  else if (PREFETCH_NO_EXTREME_STRIDE
		   && stride > PREFETCH_EXTREME_STRIDE)
	    ignore_reason = "extreme stride";

	  if (ignore_reason != NULL)
	    {
	      if (rtl_dump_file)
		fprintf (rtl_dump_file,
			 "Prefetch: ignoring iv at insn %d -- %s\n",
			 INSN_UID (iv_oc->insn), ignore_reason);
	      continue;
	    }

	  /* Determine the pointer to the basic array we are examining.  */
	  index = INTVAL (iv_oc->delta);
	  address = iv_omit_initial_values (copy_rtx (iv_bc->base));

	  d.mem_write = 0;
	  d.mem = *iv_oc->occurence;

	  /* When the iv is not always executed, we might be better off by
	     not dirtying the cache pages.

	     TODO Here we may use profiling information to determine whether
	     it is worth considering this insn even if it is not executed
	     always.  */
	  if (PREFETCH_CONDITIONAL
	      || dominated_by_p (loops->cfg.dom, loop->latch,
				 BLOCK_FOR_INSN (iv_oc->insn)))
	    note_stores (PATTERN (iv_oc->insn), check_store, &d);
	  else
	    {
	      if (rtl_dump_file)
		fprintf (rtl_dump_file,
			 "Prefetch: ignoring iv at insn %d -- in conditional code\n",
			 INSN_UID (iv_oc->insn));
	      continue;
	    }

	  /* Attempt to find another prefetch to the same array and see if we
	     can merge this one.  */
	  for (i = 0; i < num_prefetches; i++)
	    if (rtx_equal_p (address, info[i].base_address)
		&& stride == info[i].stride)
	      {
		/* In case both access same array (same location
		   just with small difference in constant indexes), merge
		   the prefetches.  Just do the later and the earlier will
		   get prefetched from previous iteration.
		   The artificial threshold should not be too small,
		   but also not bigger than small portion of memory usually
		   traversed by single loop.  */
		if (index >= info[i].index
		    && index - info[i].index < PREFETCH_EXTREME_DIFFERENCE)
		  {
		    info[i].write |= d.mem_write;
		    info[i].bytes_accessed += size;
		    info[i].index = index;
		    info[i].iv = iv_oc;
		    add = 0;
		    break;
		  }

		if (index < info[i].index
		    && info[i].index - index < PREFETCH_EXTREME_DIFFERENCE)
		  {
		    info[i].write |= d.mem_write;
		    info[i].bytes_accessed += size;
		    add = 0;
		    break;
		  }
	      }

	  /* Merging failed.  */
	  if (add)
	    {
	      if (num_prefetches >= MAX_PREFETCHES)
		{
		  if (rtl_dump_file)
		    fprintf (rtl_dump_file,
			     "Maximal number of prefetches exceeded.\n");
		  return;
		}
	      info[num_prefetches].iv = iv_oc;
	      info[num_prefetches].index = index;
	      info[num_prefetches].stride = stride;
	      info[num_prefetches].base_address = address;
	      info[num_prefetches].write = d.mem_write;
	      info[num_prefetches].bytes_accessed = size;
	      num_prefetches++;
	    }
	}

  for (i = 0; i < num_prefetches; i++)
    {
      int density;

      /* Attempt to calculate the total number of bytes fetched by all
	 iterations of the loop.  Avoid overflow.  */
      if (loop->has_desc
	  && loop->simple
	  && loop->desc.const_iter
	  && ((unsigned HOST_WIDE_INT) (0xffffffff / info[i].stride)
	      >= loop->desc.niter))
	info[i].total_bytes = info[i].stride * loop->desc.niter;
      else
	info[i].total_bytes = 0xffffffff;

      density = info[i].bytes_accessed * 100 / info[i].stride;

      /* Prefetch might be worthwhile only when the loads/stores are dense.  */
      if (PREFETCH_ONLY_DENSE_MEM)
	if (density * 256 > PREFETCH_DENSE_MEM * 100
	    && (info[i].total_bytes / PREFETCH_BLOCK
		>= PREFETCH_BLOCKS_BEFORE_LOOP_MIN))
	  {
	    info[i].prefetch_before_loop = 1;
	    info[i].prefetch_in_loop
	      = (info[i].total_bytes / PREFETCH_BLOCK
		 > PREFETCH_BLOCKS_BEFORE_LOOP_MAX);
	  }
	else
	  {
	    info[i].prefetch_in_loop = 0, info[i].prefetch_before_loop = 0;
	    if (rtl_dump_file)
	      fprintf (rtl_dump_file,
		       "Prefetch: ignoring iv at insn %d -- %d%% density is too low\n",
		       INSN_UID (info[i].iv->insn), density);
	  }
      else
	{
	  info[i].prefetch_in_loop = 1;
	  info[i].prefetch_before_loop = 1;
	}

      /* Find how many prefetch instructions we'll use within the loop.  */
      if (info[i].prefetch_in_loop != 0)
	{
	  info[i].prefetch_in_loop = ((info[i].stride + PREFETCH_BLOCK - 1)
				      / PREFETCH_BLOCK);
	  num_real_prefetches += info[i].prefetch_in_loop;
	  if (info[i].write)
	    num_real_write_prefetches += info[i].prefetch_in_loop;
	}
    }

  /* Determine how many iterations ahead to prefetch within the loop, based
     on how many prefetches we currently expect to do within the loop.  */
  if (num_real_prefetches != 0)
    {
      if ((ahead = SIMULTANEOUS_PREFETCHES / num_real_prefetches) == 0)
	{
	  if (rtl_dump_file)
	    fprintf (rtl_dump_file,
		     "Prefetch: ignoring prefetches within loop: ahead is zero; %d < %d\n",
		     SIMULTANEOUS_PREFETCHES, num_real_prefetches);
	  num_real_prefetches = 0, num_real_write_prefetches = 0;
	}
    }
  /* We'll also use AHEAD to determine how many prefetch instructions to
     emit before a loop, so don't leave it zero.  */
  if (ahead == 0)
    ahead = PREFETCH_BLOCKS_BEFORE_LOOP_MAX;

  for (i = 0; i < num_prefetches; i++)
    {
      /* Update if we've decided not to prefetch anything within the loop.  */
      if (num_real_prefetches == 0)
	info[i].prefetch_in_loop = 0;

      /* Find how many prefetch instructions we'll use before the loop.  */
      if (info[i].prefetch_before_loop != 0)
	{
	  unsigned n = info[i].total_bytes / PREFETCH_BLOCK;
	  if (n > ahead)
	    n = ahead;
	  info[i].prefetch_before_loop = n;
	  num_prefetches_before += n;
	  if (info[i].write)
	    num_write_prefetches_before += n;
	}

      if (rtl_dump_file)
	{
	  if (info[i].prefetch_in_loop == 0
	      && info[i].prefetch_before_loop == 0)
	    continue;
	  fprintf (rtl_dump_file, "Prefetch insn: %d",
		   INSN_UID (info[i].iv->insn));
	  fprintf (rtl_dump_file,
		   "; in loop: %d; before: %d; %s\n",
		   info[i].prefetch_in_loop,
		   info[i].prefetch_before_loop,
		   info[i].write ? "read/write" : "read only");
	  fprintf (rtl_dump_file,
		   " density: %d%%; bytes_accessed: %u; total_bytes: %u\n",
		   (int) (info[i].bytes_accessed * 100 / info[i].stride),
		   info[i].bytes_accessed, info[i].total_bytes);
	  fprintf (rtl_dump_file, " index: ");
	  fprintf (rtl_dump_file, HOST_WIDE_INT_PRINT_DEC, info[i].index);
	  fprintf (rtl_dump_file, "; stride: ");
	  fprintf (rtl_dump_file, HOST_WIDE_INT_PRINT_DEC, info[i].stride);
	  fprintf (rtl_dump_file, "; address: ");
	  print_rtl (rtl_dump_file, info[i].base_address);
	  fprintf (rtl_dump_file, "\n");
	}
    }

  if (rtl_dump_file && num_real_prefetches + num_prefetches_before > 0)
    {
      fprintf (rtl_dump_file, "Real prefetches needed within loop: %d (write: %d)\n",
	       num_real_prefetches, num_real_write_prefetches);
      fprintf (rtl_dump_file, "Real prefetches needed before loop: %d (write: %d)\n",
	       num_prefetches_before, num_write_prefetches_before);
    }

  for (i = 0; i < num_prefetches; i++)
    {
      int y;
      rtx seq, loc;

      for (y = 0; y < info[i].prefetch_in_loop; y++)
	{
	  int bytes_ahead = PREFETCH_BLOCK * (ahead + y);
	  rtx before_insn = info[i].iv->insn;

	  loc = copy_rtx (XEXP (*info[i].iv->occurence, 0));
	  start_sequence ();
	  /* We can save some effort by offsetting the address on
	     architectures with offsettable memory references.  */
	  if (offsettable_address_p (0, VOIDmode, loc))
	    loc = plus_constant (loc, bytes_ahead);
	  else
	    loc = force_operand (simplify_gen_binary (PLUS, Pmode,
						      loc,
						      GEN_INT (bytes_ahead)),
				 NULL_RTX);

	  /* Make sure the address operand is valid for prefetch.  */
	  if (! (*insn_data[(int)CODE_FOR_prefetch].operand[0].predicate)
		  (loc, insn_data[(int)CODE_FOR_prefetch].operand[0].mode))
	    loc = force_reg (Pmode, loc);
	  emit_insn (gen_prefetch (loc, GEN_INT (info[i].write),
				   GEN_INT (3)));
	  seq = get_insns ();
	  end_sequence ();
	  iv_emit_insn_before (seq, before_insn);
	}

      if (PREFETCH_BEFORE_LOOP)
	{
	  /* Emit insns before the loop to fetch the first cache lines or,
	     if we're not prefetching within the loop, everything we expect
	     to need.  */
	  rtx after_insn = loop_preheader_edge (loop)->src->end;
	  for (y = 0; y < info[i].prefetch_before_loop; y++)
	    {
	      rtx init_val = info[i].base_address;
	      rtx add_val = simplify_gen_binary (PLUS, Pmode,
						 GEN_INT (info[i].index),
						 GEN_INT (y * PREFETCH_BLOCK));
	      start_sequence ();
#if 0
	      /* ??? I am not sure whether something like this is needed
		 with the new implementation.  Let's disable it for now
		 and we will see.  */
	      /* Functions called by LOOP_IV_ADD_EMIT_BEFORE expect a
		 non-constant INIT_VAL to have the same mode as REG, which
		 in this case we know to be Pmode.  */
	      if (GET_MODE (init_val) != Pmode && !CONSTANT_P (init_val))
		init_val = convert_to_mode (Pmode, init_val, 0);
#endif
	      loc = simplify_gen_binary (PLUS, Pmode, init_val, add_val);
	      loc = force_operand (loc, NULL_RTX);
	      if (! (*insn_data[(int)CODE_FOR_prefetch].operand[0].predicate)
		  (loc, insn_data[(int)CODE_FOR_prefetch].operand[0].mode))
		loc = force_reg (Pmode, loc);
	      emit_insn (gen_prefetch (loc, GEN_INT (info[i].write),
	 			       GEN_INT (3)));
	      seq = get_insns ();
	      end_sequence ();
	      iv_emit_insn_after (seq, after_insn);
	    }
	}
    }
}

/* Main entry point. Attempt to add prefetch instructions for arrays used
   in LOOPS.  */
void
prefetch_loop_arrays (loops)
     struct loops *loops;
{
  unsigned i;
  struct loop *loop;

  for (i = 1; i < loops->num; i++)
    {
      loop = loops->parray[i];
      if (!loop)
	continue;

      emit_prefetch_instructions (loops, loop);
    }
#ifdef ENABLE_CHECKING
  verify_dominators (loops->cfg.dom);
  verify_loop_structure (loops);
#endif
}
