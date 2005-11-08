/* ToDO: We need to go back and add the warning messages about code
   moved across setjmp.  */


/* Dataflow support routines.
   Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005
   Free Software Foundation, Inc.
   Contributed by Michael P. Hayes (m.hayes@elec.canterbury.ac.nz,
                                    mhayes@redhat.com)

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
02110-1301, USA.


OVERVIEW:

This file provides some dataflow routines for computing reaching defs,
upward exposed uses, live variables, def-use chains, and use-def
chains.  The global dataflow is performed using simple iterative
methods with a worklist and could be sped up by ordering the blocks
with a depth first search order.

A `struct ref' data structure (ref) is allocated for every register
reference (def or use) and this records the insn and bb the ref is
found within.  The refs are linked together in chains of uses and defs
for each insn and for each register.  Each ref also has a chain field
that links all the use refs for a def or all the def refs for a use.
This is used to create use-def or def-use chains.


USAGE:

Here's an example of using the dataflow routines.

      struct df *df;

      df = df_init ();

      df_analyze (df, 0, DF_ALL);

      df_dump (df, stderr);

      df_finish (df);


df_init simply creates a poor man's object (df) that needs to be
passed to all the dataflow routines.  df_finish destroys this
object and frees up any allocated memory.   DF_ALL says to analyze
everything.

One instance of df is stored in the cfun structure.  This instance is
used to manage persistent dataflow in the backend.  This support is
currently primitive.

There are two types of information that df can provide:

1. Bitmaps, that are attached to each basic blocks.  
2. Chains, that link the uses and defs.

Different optimizations have different needs.  Ultimately, only
register allocation and schedulers should be using the bitmaps and the
rest of the backend should be upgraded to using and maintaining the
linked information.  

BITMAP PROBLEMS:

There are two interfaces for producing the bitmaps: DF_ANALYZE and
DF_ANALYZE_SUBCFG.  Both of these interfaces take a bitmap of basic
blocks.  However, each function does something completely different
with these blocks.

  In DF_ANALYZE, only the blocks in the bitmap are scanned.  However,
  the iteration that produces the persistent output bitmaps is run
  over the entire function from scratch.

  In DF_ANALYZE_SUBCFG, the scanning and the analysis is run only over
  the set of blocks that are passed in.  The blocks not mentioned are
  COMPLETELY ignored.  The typical usage of this function is that the
  set of blocks passed in are those nodes that make up an inner loop.
  This interface will produce information about the uses and defs
  completely contained within the loop but will ignore any definitions
  that reach into the loop from the outside or reach from the inside
  the loop to uses outside the loop.  BEWARE.

Neither of these interfaces provide a true incremental interface for
bitmap problems and it should not be expected that one will surface.
The incremental problems are well understood and the algorithms are
generally not worth implementing.

There are some special cases.  Simple manipulations of the control
flow graph such as splitting edges or basic blocks can be easily
accomodated and they are thru DF_ANALYZE_SIMPLE_CHANGE_SOME_BLOCKS and 
DF_ANALYZE_SIMPLE_CHANGE_ONE_BLOCK


LINKAGE PROBLEMS:

While incremental bitmaps are not worthwhile to maintian, incremental
chains are perfectly reasonable.  The fastest way to build chains from
scratch or after significant modifications is to build reaching
definitions (RD) and reaching uses (RU) and build the chains from this.  

However, these are not suitable datastructures to maintain
incrementally so the bitmaps are destroyed after the chains are built
and the incremental calls should be used as instructions are modified,
added or deleted.


df_analyze performs the following:

1. Records defs and uses by scanning the insns in each basic block
   or by scanning the insns queued by df_insn_modify.
2. Links defs and uses into insn-def and insn-use chains.
3. Links defs and uses into reg-def and reg-use chains.
4. Assigns LUIDs to each insn (for modified blocks).
5. Calculates local reaching definitions.
6. Calculates global reaching definitions.
7. Creates use-def chains.
8. Calculates local reaching uses (upwards exposed uses).
9. Calculates global reaching uses.
10. Creates def-use chains.
11. Calculates local live registers.
12. Calculates global live registers.
13. Calculates register lifetimes and determines local registers.


PHILOSOPHY:

Note that the dataflow information is not updated for every newly
deleted or created insn.  If the dataflow information requires
updating then all the changed, new, or deleted insns needs to be
marked with df_insn_modify (or df_insns_modify) either directly or
indirectly (say through calling df_insn_delete).  df_insn_modify
marks all the modified insns to get processed the next time df_analyze
 is called.

Beware that tinkering with insns may invalidate the dataflow information.
The philosophy behind these routines is that once the dataflow
information has been gathered, the user should store what they require
before they tinker with any insn.  Once a reg is replaced, for example,
then the reg-def/reg-use chains will point to the wrong place.  Once a
whole lot of changes have been made, df_analyze can be called again
to update the dataflow information.  Currently, this is not very smart
with regard to propagating changes to the dataflow so it should not
be called very often.

DATA STRUCTURES:

The basic object is a REF (reference) and this may either be a DEF
(definition) or a USE of a register.

These are linked into a variety of lists; namely reg-def, reg-use,
  insn-def, insn-use, def-use, and use-def lists.  For example,
the reg-def lists contain all the refs that define a given register
while the insn-use lists contain all the refs used by an insn.

Note that the reg-def and reg-use chains are generally short (except for
the hard registers) and thus it is much faster to search these chains
rather than searching the def or use bitmaps.

If the insns are in SSA form then the reg-def and use-def lists
should only contain the single defining ref.


TODO:

1) Incremental dataflow analysis.

Note that if a loop invariant insn is hoisted (or sunk), we do not
need to change the def-use or use-def chains.  All we have to do is to
change the bb field for all the associated defs and uses and to
renumber the LUIDs for the original and new basic blocks of the insn.

When shadowing loop mems we create new uses and defs for new pseudos
so we do not affect the existing dataflow information.

My current strategy is to queue up all modified, created, or deleted
insns so when df_analyze is called we can easily determine all the new
or deleted refs.  Currently the global dataflow information is
recomputed from scratch but this could be propagated more efficiently.

2) Reduced memory requirements.

We could operate a pool of ref structures.  When a ref is deleted it
gets returned to the pool (say by linking on to a chain of free refs).
This will require a pair of bitmaps for defs and uses so that we can
tell which ones have been changed.  Alternatively, we could
periodically squeeze the def and use tables and associated bitmaps and
renumber the def and use ids.

3) Ordering of reg-def and reg-use lists.

Should the first entry in the def list be the first def (within a BB)?
Similarly, should the first entry in the use list be the last use
(within a BB)?


NOTES:

Embedded addressing side-effects, such as POST_INC or PRE_INC, generate
both a use and a def.  These are both marked read/write to show that they
are dependent. For example, (set (reg 40) (mem (post_inc (reg 42))))
will generate a use of reg 42 followed by a def of reg 42 (both marked
read/write).  Similarly, (set (reg 40) (mem (pre_dec (reg 41))))
generates a use of reg 41 then a def of reg 41 (both marked read/write),
even though reg 41 is decremented before it is used for the memory
address in this second example.

A set to a REG inside a ZERO_EXTRACT, or a set to a non-paradoxical SUBREG
for which the number of word_mode units covered by the outer mode is
smaller than that covered by the inner mode, invokes a read-modify-write.
operation.  We generate both a use and a def and again mark them
read/write.
Paradoxical subreg writes don't leave a trace of the old content, so they
are write-only operations.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tm_p.h"
#include "insn-config.h"
#include "recog.h"
#include "function.h"
#include "regs.h"
#include "output.h"
#include "alloc-pool.h"
#include "flags.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "sbitmap.h"
#include "bitmap.h"
#include "timevar.h"
#include "df.h"
#ifndef HAVE_epilogue
#define HAVE_epilogue 0
#endif
#ifndef HAVE_prologue
#define HAVE_prologue 0
#endif
#ifndef HAVE_sibcall_epilogue
#define HAVE_sibcall_epilogue 0
#endif

#ifndef EPILOGUE_USES
#define EPILOGUE_USES(REGNO)  0
#endif

/* The bitmap_obstack is used to hold some static variables that
should not be reset after each function is compiled.  */

static bitmap_obstack persistent_obstack;

static HARD_REG_SET elim_reg_set;

/* This is a bitmap copy of regs_invalidated_by_call so that we can
easily add it into bitmaps, etc. */ 

static bitmap invalidated_by_call = NULL;

/* Initialize ur_in and ur_out as if all hard registers were partially
available.  */

static bitmap all_hard_regs = NULL;

/* In lr, exception edges are may use these registers.  */
#ifdef EH_USES
static bitmap eh_uses = NULL;
#endif

/*  The number of def allowed for any pseudo before sparce techniques
are used. */

#define DF_SPARSE_THRESHOLD 32

#define FOR_EACH_BB_IN_BITMAP(BITMAP, MIN, BB, CODE)	\
  do							\
    {							\
      unsigned int node_;				\
      bitmap_iterator bi;				\
      EXECUTE_IF_SET_IN_BITMAP (BITMAP, MIN, node_, bi)	\
	{						\
	  (BB) = BASIC_BLOCK (node_);			\
	  CODE;						\
	}						\
    }							\
  while (0)

static struct df *ddf;

static void df_reg_table_realloc (struct df *, int);
static void df_insn_table_realloc (struct df *, unsigned int);
static void df_bb_table_realloc (struct df *, unsigned int);
static void df_bitmaps_alloc (struct df *, bitmap, int);
static void df_bitmaps_free (struct df *, int);
static struct df_reach * df_alloc_reach_bitmaps (struct df *);
static void df_free_reach_bitmaps (struct df *, struct df_reach *);
static void df_clear_reach_bitmaps (struct df *, struct df_reach *);

static void df_free (struct df *);
static void df_alloc (struct df *, int);
static void df_bb_modify (struct df *, basic_block);

static struct df_link *df_ref_unlink (struct df_link **, struct ref *);
static void df_def_unlink (struct df *, struct ref *);
static void df_use_unlink (struct df *, struct ref *);
static void df_insn_refs_unlink (struct df *, basic_block, rtx);
#if 0
static void df_bb_refs_unlink (struct df *, basic_block);
static void df_refs_unlink (struct df *, bitmap);
#endif

static void df_ref_record_1 (struct df *, rtx, rtx *, rtx, enum df_ref_type,
			     enum df_ref_flags);
static void df_ref_record (struct df *, rtx, rtx *, rtx, enum df_ref_type,
			   enum df_ref_flags, bool record_live);
static void df_def_record_1 (struct df *, rtx, basic_block, rtx,
			     enum df_ref_flags, bool record_live);
static void df_defs_record (struct df *, rtx, basic_block, rtx);
static void df_uses_record (struct df *, rtx *, enum df_ref_type,
			    basic_block, rtx, enum df_ref_flags);
static void df_insn_refs_record (struct df *, basic_block, rtx);
static void df_bb_refs_record (struct df *, basic_block);
static void df_refs_record (struct df *, bitmap);

static void df_bb_reg_def_chain_create (struct df *, basic_block);
static void df_reg_def_chain_create (struct df *, bitmap, bool, bool);
static void df_bb_reg_use_chain_create (struct df *, basic_block);
static void df_reg_use_chain_create (struct df *, bitmap, bool, bool);
static void df_bb_chain_create (struct df *, int, basic_block);
static void df_chain_create (struct df *, int, bitmap);
static void df_rd_init (struct dataflow *, int);
static void df_rd_confluence_n (struct dataflow *, edge e);
static bool df_rd_transfer_function (struct dataflow *, int);
static void df_rd_local_compute (struct df *, bitmap);
static void df_ru_init (struct dataflow *, int);
static void df_ru_confluence_n (struct dataflow *, edge e);
static bool df_ru_transfer_function (struct dataflow *, int);
static void df_ru_local_compute (struct df *, bitmap);
static void df_lr_init (struct dataflow *, int);
static void df_bb_lr_local_compute (struct df *, basic_block);
static void df_lr_confluence_0 (struct dataflow *, basic_block);
static void df_lr_confluence_n (struct dataflow *, edge e);
static bool df_lr_transfer_function (struct dataflow *, int);
static void df_lr_local_compute (struct df *, bitmap, bool);
static void df_ur_init (struct dataflow *, int);
static void df_bb_ur_local_compute (struct df *, basic_block);
static void df_ur_local_compute (struct df *, bitmap);
static void df_ur_confluence_n (struct dataflow *, edge e);
static bool df_ur_transfer_function (struct dataflow *, int);
static void df_ur_local_finalize (struct df *, bitmap);

static void df_bb_reg_info_compute (struct df *, basic_block, bitmap);
static void df_reg_info_compute (struct df *, bitmap);

static int df_bb_luids_set (struct df *df, basic_block);
static int df_luids_set (struct df *df, bitmap);

static int df_refs_queue (struct df *);
static int df_refs_process (struct df *);
static int df_bb_refs_update (struct df *, basic_block);
static int df_refs_update (struct df *, bitmap);

static void df_insns_modify (struct df *, basic_block, rtx, rtx);
static int df_rtx_mem_replace (rtx *, void *);
static int df_rtx_reg_replace (rtx *, void *);
void df_refs_reg_replace (struct df *, bitmap, struct df_link *, rtx, rtx);

static int df_def_dominates_all_uses_p (struct df *, struct ref *def);
static int df_def_dominates_uses_p (struct df *, struct ref *def, bitmap);
static struct ref *df_bb_insn_regno_last_use_find (struct df *, basic_block,
						   rtx, unsigned int);
static struct ref *df_bb_insn_regno_first_def_find (struct df *, basic_block,
						    rtx, unsigned int);

static void df_chain_dump (struct df_link *, FILE *file);
static void df_chain_dump_regno (struct df_link *, FILE *file);
static void df_regno_debug (struct df *, unsigned int, FILE *);
static void df_ref_debug (struct df *, struct ref *, FILE *);
static void hybrid_search_forward (basic_block, struct dataflow *);
static void hybrid_search_backward (basic_block, struct dataflow *);


static struct df_problem problem_RD =
{
   DF_FORWARD,                 /* Direction.  */
   df_rd_init,                 /* Init function.  */
   NULL,                       /* Confluence operator 0.  */ 
   df_rd_confluence_n,         /* Confluence operator n.  */ 
   df_rd_transfer_function,    /* Transfer function.  */
   NULL                        /* Finalize function.  */
};

static struct df_problem problem_RU =
{
   DF_BACKWARD,                /* Direction.  */
   df_ru_init,                 /* Init function.  */
   NULL,                       /* Confluence operator 0.  */ 
   df_ru_confluence_n,         /* Confluence operator n.  */ 
   df_ru_transfer_function,    /* Transfer function.  */
   NULL                        /* Finalize function.  */
};

static struct df_problem problem_LR =
{
   DF_BACKWARD,                /* Direction.  */
   df_lr_init,                 /* Init function.  */
   df_lr_confluence_0,         /* Confluence operator 0.  */ 
   df_lr_confluence_n,         /* Confluence operator n.  */ 
   df_lr_transfer_function,    /* Transfer function.  */
   NULL                        /* Finalize function.  */
};

static struct df_problem problem_UR =
{
   DF_FORWARD,                 /* Direction.  */
   df_ur_init,                 /* Init function.  */
   NULL,                       /* Confluence operator 0.  */ 
   df_ur_confluence_n,         /* Confluence operator n.  */ 
   df_ur_transfer_function,    /* Transfer function.  */
   df_ur_local_finalize        /* Finalize function.  */
};


/* Local memory allocation/deallocation routines.  */


/* Increase the insn info table to have space for at least SIZE + 1
   elements.  */
static void
df_insn_table_realloc (struct df *df, unsigned int size)
{
  size++;
  if (size <= df->insn_size)
    return;

  /* Make the table a little larger than requested, so we do not need
     to enlarge it so often.  */
  size += df->insn_size / 4;

  df->insns = xrealloc (df->insns, size * sizeof (struct insn_info));

  memset (df->insns + df->insn_size, 0,
	  (size - df->insn_size) * sizeof (struct insn_info));

  df->insn_size = size;

  if (! df->insns_modified)
    {
      df->insns_modified = BITMAP_ALLOC (NULL);
      bitmap_zero (df->insns_modified);
    }
}

/* Increase the bb info table to have space for at least SIZE + 1
   elements.  */

static void
df_bb_table_realloc (struct df *df, unsigned int size)
{
  if (size < df->n_bbs)
    return;

  /* Make the table a little larger than requested, so we do not need
     to enlarge it so often.  */
  size += df->n_bbs;

  df->bbs = xrealloc (df->bbs, size * sizeof (struct df_bb_info));

  memset (df->bbs + df->n_bbs, 0, (size - df->n_bbs) * sizeof (struct df_bb_info));

  df->n_bbs = size;
}

/* Increase the reg info table by SIZE more elements.  */
static void
df_reg_table_realloc (struct df *df, int size)
{
  /* Make table 25 percent larger by default.  */
  if (! size)
    size = df->reg_size / 4;

  size += df->reg_size;
  if (size < max_reg_num ())
    size = max_reg_num ();

  df->regs = xrealloc (df->regs, size * sizeof (struct reg_info));
  /* Zero the new entries.  */
  memset (df->regs + df->reg_size, 0,
	  (size - df->reg_size) * sizeof (struct reg_info));

  df->reg_size = size;
}


/* Allocate bitmaps for each basic block.  */

static void
df_bitmaps_alloc (struct df *df, bitmap blocks, int flags)
{
  basic_block bb;

  df->n_defs = df->def_id;
  df->n_uses = df->use_id;

  gcc_assert (blocks);

  if (flags & DF_RD)
    {
      if (df->rd_reach)
	df_clear_reach_bitmaps (df, df->rd_reach);
      else
	df->rd_reach = df_alloc_reach_bitmaps (df);
    }

  if (flags & DF_RU)
    {
      if (df->ru_reach)
	df_clear_reach_bitmaps (df, df->ru_reach);
      else
	df->ru_reach = df_alloc_reach_bitmaps (df);
    }

  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
    {
      struct df_bb_info *bb_info = DF_BB_INFO (df, bb);

      if (flags & DF_RD)
	{
	  if (!bb_info->rd_in)
	    {
	      /* Allocate bitmaps for reaching definitions.  */
	      bb_info->rd_kill = BITMAP_ALLOC (NULL);
	      bb_info->rd_sparse_kill = BITMAP_ALLOC (NULL);
	      bb_info->rd_gen = BITMAP_ALLOC (NULL);
	      bb_info->rd_in = BITMAP_ALLOC (NULL);
	      bb_info->rd_out = BITMAP_ALLOC (NULL);
	    }
	  else
	    {
	      bitmap_clear (bb_info->rd_kill);
	      bitmap_clear (bb_info->rd_sparse_kill);
	      bitmap_clear (bb_info->rd_gen);
	      bitmap_clear (bb_info->rd_in);
	      bitmap_clear (bb_info->rd_out);
	    }
	}

      if (flags & DF_RU)
	{
	  if (!bb_info->ru_in)
	    {
	      /* Allocate bitmaps for upward exposed uses.  */
	      bb_info->ru_kill = BITMAP_ALLOC (NULL);
	      bb_info->ru_sparse_kill = BITMAP_ALLOC (NULL);
	      bb_info->ru_gen = BITMAP_ALLOC (NULL);
	      bb_info->ru_in = BITMAP_ALLOC (NULL);
	      bb_info->ru_out = BITMAP_ALLOC (NULL);
	    }
	  else
	    {
	      bitmap_clear (bb_info->ru_kill);
	      bitmap_clear (bb_info->ru_sparse_kill);
	      bitmap_clear (bb_info->ru_gen);
	      bitmap_clear (bb_info->ru_in);
	      bitmap_clear (bb_info->ru_out);
	    }
	}

      if (flags & DF_LR)
	{
	  if (!bb_info->lr_in)
	    {
	      /* Allocate bitmaps for live registers.  */
	      bb_info->lr_def = BITMAP_ALLOC (NULL);
	      bb_info->lr_use = BITMAP_ALLOC (NULL);
	      bb_info->lr_in = BITMAP_ALLOC (NULL);
	      bb_info->lr_out = BITMAP_ALLOC (NULL);
	    }
	  else
	    {
	      bitmap_clear (bb_info->lr_def);
	      bitmap_clear (bb_info->lr_use);
	      bitmap_clear (bb_info->lr_in);
	      bitmap_clear (bb_info->lr_out);
	    }
	}

      if (flags & DF_UR)
	{
	  if (!bb_info->ur_in)
	    {
	      /* Allocate bitmaps for uninitialized registers.  */
	      bb_info->ur_earlyclobber = BITMAP_ALLOC (NULL);
	      bb_info->ur_kill = BITMAP_ALLOC (NULL);
	      bb_info->ur_gen = BITMAP_ALLOC (NULL);
	      bb_info->ur_in = BITMAP_ALLOC (NULL);
	      bb_info->ur_out = BITMAP_ALLOC (NULL);
	    }
	  else
	    {
	      bitmap_clear (bb_info->ur_earlyclobber);
	      bitmap_clear (bb_info->ur_kill);
	      bitmap_clear (bb_info->ur_gen);
	      bitmap_clear (bb_info->ur_in);
	      bitmap_clear (bb_info->ur_out);
	    }
	}
    });
}


/* Free bitmaps for each basic block.  */
static void
df_bitmaps_free (struct df *df, int flags)
{
  basic_block bb;

  if ((flags & DF_RD) && df->rd_reach)
    {
      df_free_reach_bitmaps (df, df->rd_reach);
      df->rd_reach = NULL;
    }

  if ((flags & DF_RD) && df->ru_reach)
    {
      df_free_reach_bitmaps (df, df->ru_reach);
      df->ru_reach = NULL;
    }

  FOR_ALL_BB (bb)
    {
      struct df_bb_info *bb_info = DF_BB_INFO (df, bb);

      if (!bb_info)
	continue;

      if ((flags & DF_RD) && bb_info->rd_in)
	{
	  /* Free bitmaps for reaching definitions.  */
	  BITMAP_FREE (bb_info->rd_kill);
	  BITMAP_FREE (bb_info->rd_sparse_kill);
	  BITMAP_FREE (bb_info->rd_gen);
	  BITMAP_FREE (bb_info->rd_in);
	  BITMAP_FREE (bb_info->rd_out);
	}

      if ((flags & DF_RU) && bb_info->ru_in)
	{
	  /* Free bitmaps for upward exposed uses.  */
	  BITMAP_FREE (bb_info->ru_kill);
	  BITMAP_FREE (bb_info->ru_sparse_kill);
	  BITMAP_FREE (bb_info->ru_gen);
	  BITMAP_FREE (bb_info->ru_in);
	  BITMAP_FREE (bb_info->ru_out);
	}

      if ((flags & DF_LR) && bb_info->lr_in)
	{
	  /* Free bitmaps for live variables.  */
	  BITMAP_FREE (bb_info->lr_def);
	  BITMAP_FREE (bb_info->lr_use);
	  BITMAP_FREE (bb_info->lr_in);
	  BITMAP_FREE (bb_info->lr_out);
	}

      if ((flags & DF_UR) && bb_info->ur_in)
	{
	  /* Free bitmaps for live variables.  */
	  BITMAP_FREE (bb_info->ur_earlyclobber);
	  BITMAP_FREE (bb_info->ur_kill);
	  BITMAP_FREE (bb_info->ur_gen);
	  BITMAP_FREE (bb_info->ur_in);
	  BITMAP_FREE (bb_info->ur_out);
	}
    }
  df->flags &= ~(flags & (DF_RD | DF_RU | DF_LR| DF_UR));
}


/* Allocate and initialize dataflow memory.  */
static void
df_alloc (struct df *df, int n_regs)
{
  int n_insns;
  df->link_pool = create_alloc_pool ("df_link pool", sizeof (struct df_link),
				    100);
  df->ref_pool  = create_alloc_pool ("df_ref pool", sizeof (struct ref), 100);

  /* Perhaps we should use LUIDs to save memory for the insn_refs
     table.  This is only a small saving; a few pointers.  */
  n_insns = get_max_uid () + 1;

  df->def_id = 0;
  df->n_defs = 0;
  /* Approximate number of defs by number of insns.  */
  df->def_size = n_insns;
  df->defs = xmalloc (df->def_size * sizeof (*df->defs));

  df->use_id = 0;
  df->n_uses = 0;
  /* Approximate number of uses by twice number of insns.  */
  df->use_size = n_insns * 2;
  df->uses = xmalloc (df->use_size * sizeof (*df->uses));

  df->n_regs = n_regs;
  df->n_bbs = last_basic_block * 2;

  /* Allocate temporary working array used during local dataflow analysis.  */
  df_insn_table_realloc (df, n_insns);

  df_reg_table_realloc (df, df->n_regs);

  df->bbs_modified = BITMAP_ALLOC (NULL);
  bitmap_zero (df->bbs_modified);

  df->hardware_regs_used = BITMAP_ALLOC (NULL);
  df->exit_block_uses = BITMAP_ALLOC (NULL);

  df->flags = 0;

  df->bbs = xcalloc (df->n_bbs, sizeof (struct df_bb_info));

}


/* Free all the dataflow info.  */
static void
df_free (struct df *df)
{
  df_bitmaps_free (df, DF_ALL);

  if (df->bbs)
    free (df->bbs);
  df->bbs = 0;

  if (df->insns)
    free (df->insns);
  df->insns = 0;
  df->insn_size = 0;

  if (df->defs)
    free (df->defs);
  df->defs = 0;
  df->def_size = 0;
  df->def_id = 0;

  if (df->uses)
    free (df->uses);
  df->uses = 0;
  df->use_size = 0;
  df->use_id = 0;

  if (df->regs)
    free (df->regs);
  df->regs = 0;
  df->reg_size = 0;

  BITMAP_FREE (df->bbs_modified);
  BITMAP_FREE (df->insns_modified);
  BITMAP_FREE (df->hardware_regs_used);
  BITMAP_FREE (df->exit_block_uses);

  free_alloc_pool (df->ref_pool);
  free_alloc_pool (df->link_pool);
}

/* Local miscellaneous routines.  */

/* Return a USE for register REGNO.  */
static rtx df_reg_use_gen (unsigned int regno)
{
  rtx reg;
  rtx use;

  reg = regno_reg_rtx[regno];

  use = gen_rtx_USE (GET_MODE (reg), reg);
  return use;
}
/* Return a DEF for register REGNO.  */
static rtx df_reg_def_gen (unsigned int regno)
{
  rtx reg;
  rtx def;

  reg = regno_reg_rtx[regno];

  def = gen_rtx_CLOBBER (GET_MODE (reg), reg);
  return def;
}

/* Local chain manipulation routines.  */

/* Create a link in a def-use or use-def chain.  */
static inline struct df_link *
df_link_create (struct df *df, struct ref *ref, struct df_link *next)
{
  struct df_link *link;

  link = pool_alloc (df->link_pool);
  link->next = next;
  link->ref = ref;
  return link;
}

/* Copies all the members of CHAIN into a new list.  */

struct df_link *
df_chain_copy (struct df *df, struct df_link *chain)
{
  struct df_link *head = NULL, **p = &head, *act;

  for (act = chain; act; act = act->next)
    {
      struct df_link *copy = df_link_create (df, act->ref, NULL);
      *p = copy;
      p = &copy->next;
    }

  return head;
}

/* Releases members of the CHAIN.  */

static void
free_reg_ref_chain (struct df *df, struct df_link **chain)
{
  struct df_link *act, *next;

  for (act = *chain; act; act = next)
    {
      next = act->next;
      pool_free (df->link_pool, act);
    }

  *chain = NULL;
}

/* Add REF to chain head pointed to by PHEAD.  */
static struct df_link *
df_ref_unlink (struct df_link **phead, struct ref *ref)
{
  struct df_link *link = *phead;

  if (link)
    {
      if (! link->next)
	{
	  /* Only a single ref.  It must be the one we want.
	     If not, the def-use and use-def chains are likely to
	     be inconsistent.  */
	  gcc_assert (link->ref == ref);
	  
	  /* Now have an empty chain.  */
	  *phead = NULL;
	}
      else
	{
	  /* Multiple refs.  One of them must be us.  */
	  if (link->ref == ref)
	    *phead = link->next;
	  else
	    {
	      /* Follow chain.  */
	      for (; link->next; link = link->next)
		{
		  if (link->next->ref == ref)
		    {
		      /* Unlink from list.  */
		      link->next = link->next->next;
		      return link->next;
		    }
		}
	    }
	}
    }
  return link;
}


/* Unlink REF from all def-use/use-def chains, etc.  */
int
df_ref_remove (struct df *df, struct ref *ref)
{
  if (DF_REF_REG_DEF_P (ref))
    {
      df_def_unlink (df, ref);
      if (!(DF_REF_FLAGS (ref) & DF_REF_ARTIFICIAL))
	df_ref_unlink (&df->insns[DF_REF_INSN_UID (ref)].defs, ref);
    }
  else
    {
      df_use_unlink (df, ref);
      if (!(DF_REF_FLAGS (ref) & DF_REF_ARTIFICIAL))
	df_ref_unlink (&df->insns[DF_REF_INSN_UID (ref)].uses, ref);
    }
  return 1;
}


/* Unlink DEF from use-def and reg-def chains.  */
static void
df_def_unlink (struct df *df ATTRIBUTE_UNUSED, struct ref *def)
{
  struct df_link *du_link;
  unsigned int dregno = DF_REF_REGNO (def);

  /* Follow def-use chain to find all the uses of this def.  */
  for (du_link = DF_REF_CHAIN (def); du_link; du_link = du_link->next)
    {
      struct ref *use = du_link->ref;

      /* Unlink this def from the use-def chain.  */
      df_ref_unlink (&DF_REF_CHAIN (use), def);
    }
  DF_REF_CHAIN (def) = 0;

  /* Unlink def from reg-def chain.  */
  df_ref_unlink (&df->regs[dregno].defs, def);

  df->defs[DF_REF_ID (def)] = 0;
}


/* Unlink use from def-use and reg-use chains.  */
static void
df_use_unlink (struct df *df ATTRIBUTE_UNUSED, struct ref *use)
{
  struct df_link *ud_link;
  unsigned int uregno = DF_REF_REGNO (use);

  /* Follow use-def chain to find all the defs of this use.  */
  for (ud_link = DF_REF_CHAIN (use); ud_link; ud_link = ud_link->next)
    {
      struct ref *def = ud_link->ref;

      /* Unlink this use from the def-use chain.  */
      df_ref_unlink (&DF_REF_CHAIN (def), use);
    }
  DF_REF_CHAIN (use) = 0;

  /* Unlink use from reg-use chain.  */
  df_ref_unlink (&df->regs[uregno].uses, use);

  df->uses[DF_REF_ID (use)] = 0;
}

/* Local routines for recording refs.  */


/* Create a new ref of type DF_REF_TYPE for register REG at address
   LOC within INSN of BB.  */
struct ref *
df_ref_create (struct df *df, rtx reg, rtx *loc, rtx insn, basic_block bb,
	       enum df_ref_type ref_type, enum df_ref_flags ref_flags)
{
  struct ref *this_ref;

  this_ref = pool_alloc (df->ref_pool);
  DF_REF_REG (this_ref) = reg;
  DF_REF_REGNO (this_ref) = REGNO (GET_CODE (reg) == SUBREG ? SUBREG_REG (reg) : reg); 
  DF_REF_LOC (this_ref) = loc;
  DF_REF_INSN (this_ref) = insn;
  DF_REF_CHAIN (this_ref) = 0;
  DF_REF_TYPE (this_ref) = ref_type;
  DF_REF_FLAGS (this_ref) = ref_flags;
  DF_REF_DATA (this_ref) = NULL;
  DF_REF_BB (this_ref) = bb;
  if (ref_type == DF_REF_REG_DEF)
    {
      if (df->def_id >= df->def_size)
	{
	  /* Make table 25 percent larger.  */
	  df->def_size += (df->def_size / 4);
	  df->defs = xrealloc (df->defs,
			       df->def_size * sizeof (*df->defs));
	}
      DF_REF_ID (this_ref) = df->def_id;
      df->defs[df->def_id++] = this_ref;
    }
  else
    {
      if (df->use_id >= df->use_size)
	{
	  /* Make table 25 percent larger.  */
	  df->use_size += (df->use_size / 4);
	  df->uses = xrealloc (df->uses,
			       df->use_size * sizeof (*df->uses));
	}
      DF_REF_ID (this_ref) = df->use_id;
      df->uses[df->use_id++] = this_ref;
    }
  return this_ref;
}


/* Create a new reference of type DF_REF_TYPE for a single register REG,
   used inside the LOC rtx of INSN.  */
static void
df_ref_record_1 (struct df *df, rtx reg, rtx *loc, rtx insn,
		 enum df_ref_type ref_type, enum df_ref_flags ref_flags)
{
  df_ref_create (df, reg, loc, insn, BLOCK_FOR_INSN (insn), ref_type, ref_flags);
}


/* Create new references of type DF_REF_TYPE for each part of register REG
   at address LOC within INSN of BB.  */
static void
df_ref_record (struct df *df, rtx reg, rtx *loc, rtx insn,
	       enum df_ref_type ref_type, enum df_ref_flags ref_flags, bool record_live)
{
  unsigned int regno;

  gcc_assert (REG_P (reg) || GET_CODE (reg) == SUBREG);

  /* For the reg allocator we are interested in some SUBREG rtx's, but not
     all.  Notably only those representing a word extraction from a multi-word
     reg.  As written in the docu those should have the form
     (subreg:SI (reg:M A) N), with size(SImode) > size(Mmode).
     XXX Is that true?  We could also use the global word_mode variable.  */
  if ((df->flags & DF_SUBREGS) == 0
      && GET_CODE (reg) == SUBREG
      && (GET_MODE_SIZE (GET_MODE (reg)) < GET_MODE_SIZE (word_mode)
	  || GET_MODE_SIZE (GET_MODE (reg))
	       >= GET_MODE_SIZE (GET_MODE (SUBREG_REG (reg)))))
    {
      loc = &SUBREG_REG (reg);
      reg = *loc;
      ref_flags |= DF_REF_STRIPPED;
    }

  regno = REGNO (GET_CODE (reg) == SUBREG ? SUBREG_REG (reg) : reg);
  if (regno < FIRST_PSEUDO_REGISTER)
    {
      int i;
      int endregno;

      if (! (df->flags & DF_HARD_REGS))
	return;

      /* GET_MODE (reg) is correct here.  We do not want to go into a SUBREG
         for the mode, because we only want to add references to regs, which
	 are really referenced.  E.g., a (subreg:SI (reg:DI 0) 0) does _not_
	 reference the whole reg 0 in DI mode (which would also include
	 reg 1, at least, if 0 and 1 are SImode registers).  */
      endregno = hard_regno_nregs[regno][GET_MODE (reg)];
      if (GET_CODE (reg) == SUBREG)
        regno += subreg_regno_offset (regno, GET_MODE (SUBREG_REG (reg)),
				      SUBREG_BYTE (reg), GET_MODE (reg));
      endregno += regno;

      for (i = regno; i < endregno; i++)
	{
	  /* Calls are handled at call site because regs_ever_live doesn't include 
	     clobbered regs, only used ones.  */
	  if (ref_type == DF_REF_REG_DEF && record_live)
	    regs_ever_live[i] = 1;
	  else if ((ref_type == DF_REF_REG_USE 
		   || ref_type == DF_REF_REG_MEM_STORE
		   || ref_type == DF_REF_REG_MEM_LOAD)
		   && ((ref_flags & DF_REF_ARTIFICIAL) == 0) )
	    {
	      /* Set regs_ever live on uses of non-eliminable frame
		 pointers and arg pointers.  */
	      if (! (TEST_HARD_REG_BIT (elim_reg_set, regno)
		     && (regno == FRAME_POINTER_REGNUM || regno == ARG_POINTER_REGNUM)))
		regs_ever_live[i] = 1;
	    }

	  df_ref_record_1 (df, regno_reg_rtx[i], loc, insn, ref_type, ref_flags);
	}
    }
  else
    {
      df_ref_record_1 (df, reg, loc, insn, ref_type, ref_flags);
    }
}


/* A set to a non-paradoxical SUBREG for which the number of word_mode units
   covered by the outer mode is smaller than that covered by the inner mode,
   is a read-modify-write operation.
   This function returns true iff the SUBREG X is such a SUBREG.  */
bool
read_modify_subreg_p (rtx x)
{
  unsigned int isize, osize;
  if (GET_CODE (x) != SUBREG)
    return false;
  isize = GET_MODE_SIZE (GET_MODE (SUBREG_REG (x)));
  osize = GET_MODE_SIZE (GET_MODE (x));
  return (isize > osize && isize > UNITS_PER_WORD);
}


/* Process all the registers defined in the rtx, X.  */
static void
df_def_record_1 (struct df *df, rtx x, basic_block bb, rtx insn, 
		 enum df_ref_flags flags, bool record_live)
{
  rtx *loc;
  rtx dst;

 /* We may recursively call ourselves on EXPR_LIST when dealing with PARALLEL
     construct.  */
  if (GET_CODE (x) == EXPR_LIST || GET_CODE (x) == CLOBBER)
    loc = &XEXP (x, 0);
  else
    loc = &SET_DEST (x);
  dst = *loc;

  /* Some targets place small structures in registers for
     return values of functions.  */
  if (GET_CODE (dst) == PARALLEL && GET_MODE (dst) == BLKmode)
    {
      int i;

      for (i = XVECLEN (dst, 0) - 1; i >= 0; i--)
	{
	  rtx temp = XVECEXP (dst, 0, i);
	  if (GET_CODE (temp) == EXPR_LIST || GET_CODE (temp) == CLOBBER
	      || GET_CODE (temp) == SET)
	    df_def_record_1 (df, temp, bb, insn, GET_CODE (temp) == CLOBBER ? flags | DF_REF_CLOBBER : flags, record_live);
	}
      return;
    }

  /* Maybe, we should flag the use of STRICT_LOW_PART somehow.  It might
     be handy for the reg allocator.  */
  while (GET_CODE (dst) == STRICT_LOW_PART
	 || GET_CODE (dst) == ZERO_EXTRACT
	 || read_modify_subreg_p (dst))
    {
#if 0
      /* Strict low part always contains SUBREG, but we do not want to make
	 it appear outside, as whole register is always considered.  */
      if (GET_CODE (dst) == STRICT_LOW_PART)
	{
	  loc = &XEXP (dst, 0);
	  dst = *loc;
	}
#endif
      loc = &XEXP (dst, 0);
      dst = *loc;
      flags |= DF_REF_READ_WRITE;
    }

  if (REG_P (dst)
      || (GET_CODE (dst) == SUBREG && REG_P (SUBREG_REG (dst))))
    df_ref_record (df, dst, loc, insn, DF_REF_REG_DEF, flags, record_live);
}


/* Process all the registers defined in the pattern rtx, X.  */
static void
df_defs_record (struct df *df, rtx x, basic_block bb, rtx insn)
{
  RTX_CODE code = GET_CODE (x);

  if (code == SET || code == CLOBBER)
    {
      /* Mark the single def within the pattern.  */
      df_def_record_1 (df, x, bb, insn, code == CLOBBER ? DF_REF_CLOBBER : 0, true);
    }
  else if (code == COND_EXEC)
    {
      df_defs_record  (df, COND_EXEC_CODE (x), bb, insn);
    }
  else if (code == PARALLEL)
    {
      int i;

      /* Mark the multiple defs within the pattern.  */
      for (i = XVECLEN (x, 0) - 1; i >= 0; i--)
	 df_defs_record (df, XVECEXP (x, 0, i), bb, insn);
    }
}


/* Process all the registers used in the rtx at address LOC.  */
static void
df_uses_record (struct df *df, rtx *loc, enum df_ref_type ref_type,
		basic_block bb, rtx insn, enum df_ref_flags flags)
{
  RTX_CODE code;
  rtx x;
 retry:
  x = *loc;
  if (!x)
    return;
  code = GET_CODE (x);
  switch (code)
    {
    case LABEL_REF:
    case SYMBOL_REF:
    case CONST_INT:
    case CONST:
    case CONST_DOUBLE:
    case CONST_VECTOR:
    case PC:
    case CC0:
    case ADDR_VEC:
    case ADDR_DIFF_VEC:
      return;

    case CLOBBER:
      /* If we are clobbering a MEM, mark any registers inside the address
	 as being used.  */
      if (MEM_P (XEXP (x, 0)))
	df_uses_record (df, &XEXP (XEXP (x, 0), 0),
			DF_REF_REG_MEM_STORE, bb, insn, flags);

      /* If we're clobbering a REG then we have a def so ignore.  */
      return;

    case MEM:
      df_uses_record (df, &XEXP (x, 0), DF_REF_REG_MEM_LOAD, bb, insn,
		      flags & DF_REF_IN_NOTE);
      return;

    case SUBREG:
      /* While we're here, optimize this case.  */

      /* In case the SUBREG is not of a REG, do not optimize.  */
      if (!REG_P (SUBREG_REG (x)))
	{
	  loc = &SUBREG_REG (x);
	  df_uses_record (df, loc, ref_type, bb, insn, flags);
	  return;
	}
      /* ... Fall through ...  */

    case REG:
      df_ref_record (df, x, loc, insn, ref_type, flags, true);
      return;

    case SET:
      {
	rtx dst = SET_DEST (x);
	gcc_assert (!(flags & DF_REF_IN_NOTE));
	df_uses_record (df, &SET_SRC (x), DF_REF_REG_USE, bb, insn, 0);

	switch (GET_CODE (dst))
	  {
	    case SUBREG:
	      if (read_modify_subreg_p (dst))
		{
		  df_uses_record (df, &SUBREG_REG (dst), DF_REF_REG_USE, bb,
				  insn, DF_REF_READ_WRITE);
		  break;
		}
	      /* Fall through.  */
	    case REG:
	    case PARALLEL:
	    case SCRATCH:
	    case PC:
	    case CC0:
		break;
	    case MEM:
	      df_uses_record (df, &XEXP (dst, 0),
			      DF_REF_REG_MEM_STORE,
			      bb, insn, 0);
	      break;
	    case STRICT_LOW_PART:
	      {
		rtx *temp = &XEXP (dst, 0);
		/* A strict_low_part uses the whole REG and not just the
		 SUBREG.  */
		dst = XEXP (dst, 0);
	      df_uses_record (df, (GET_CODE (dst) == SUBREG) ? &SUBREG_REG (dst) : temp, DF_REF_REG_USE, bb,
			     insn, DF_REF_READ_WRITE);
	      }
	      break;
	    case ZERO_EXTRACT:
	    case SIGN_EXTRACT:
	      df_uses_record (df, &XEXP (dst, 0), DF_REF_REG_USE, bb, insn,
			      DF_REF_READ_WRITE);
	      df_uses_record (df, &XEXP (dst, 1), DF_REF_REG_USE, bb, insn, 0);
	      df_uses_record (df, &XEXP (dst, 2), DF_REF_REG_USE, bb, insn, 0);
	      dst = XEXP (dst, 0);
	      break;
	    default:
	      gcc_unreachable ();
	  }
	return;
      }

    case RETURN:
      break;

    case ASM_OPERANDS:
    case UNSPEC_VOLATILE:
    case TRAP_IF:
    case ASM_INPUT:
      {
	/* Traditional and volatile asm instructions must be considered to use
	   and clobber all hard registers, all pseudo-registers and all of
	   memory.  So must TRAP_IF and UNSPEC_VOLATILE operations.

	   Consider for instance a volatile asm that changes the fpu rounding
	   mode.  An insn should not be moved across this even if it only uses
	   pseudo-regs because it might give an incorrectly rounded result.

	   For now, just mark any regs we can find in ASM_OPERANDS as
	   used.  */

	/* For all ASM_OPERANDS, we must traverse the vector of input operands.
	   We can not just fall through here since then we would be confused
	   by the ASM_INPUT rtx inside ASM_OPERANDS, which do not indicate
	   traditional asms unlike their normal usage.  */
	if (code == ASM_OPERANDS)
	  {
	    int j;

	    for (j = 0; j < ASM_OPERANDS_INPUT_LENGTH (x); j++)
	      df_uses_record (df, &ASM_OPERANDS_INPUT (x, j),
			      DF_REF_REG_USE, bb, insn, 0);
	    return;
	  }
	break;
      }

    case PRE_DEC:
    case POST_DEC:
    case PRE_INC:
    case POST_INC:
    case PRE_MODIFY:
    case POST_MODIFY:
      /* Catch the def of the register being modified.  */
      df_ref_record (df, XEXP (x, 0), &XEXP (x, 0), insn, DF_REF_REG_DEF, DF_REF_READ_WRITE, true);

      /* ... Fall through to handle uses ...  */

    default:
      break;
    }

  /* Recursively scan the operands of this expression.  */
  {
    const char *fmt = GET_RTX_FORMAT (code);
    int i;

    for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
      {
	if (fmt[i] == 'e')
	  {
	    /* Tail recursive case: save a function call level.  */
	    if (i == 0)
	      {
		loc = &XEXP (x, 0);
		goto retry;
	      }
	    df_uses_record (df, &XEXP (x, i), ref_type, bb, insn, flags);
	  }
	else if (fmt[i] == 'E')
	  {
	    int j;
	    for (j = 0; j < XVECLEN (x, i); j++)
	      df_uses_record (df, &XVECEXP (x, i, j), ref_type,
			      bb, insn, flags);
	  }
      }
  }
}

/* Process INSN recursively, and return true if it contains an asm
   instruction.  */
static bool
df_insn_contains_asm (rtx *loc, rtx insn)
{
  RTX_CODE code;
  bool saw_asm = false;
  
  rtx x;
 retry:
  x = *loc;
  if (!x)
    return false;
  code = GET_CODE (x);
  switch (code)
    {
    case LABEL_REF:
    case SYMBOL_REF:
    case CONST_INT:
    case CONST:
    case CONST_DOUBLE:
    case CONST_VECTOR:
    case PC:
    case CC0:
    case ADDR_VEC:
    case ADDR_DIFF_VEC:
      return saw_asm;

    case CLOBBER:
      /* If we are clobbering a MEM, mark any registers inside the address
	 as being used.  */
      if (MEM_P (XEXP (x, 0)))
	saw_asm |= df_insn_contains_asm (&XEXP (XEXP (x, 0), 0), insn);

      /* If we're clobbering a REG then we have a def so ignore.  */
      return saw_asm;

    case MEM:
      saw_asm |= df_insn_contains_asm (&XEXP (x, 0), insn);
      
      return saw_asm;

    case SUBREG:
      /* While we're here, optimize this case.  */

      /* In case the SUBREG is not of a REG, do not optimize.  */
      if (!REG_P (SUBREG_REG (x)))
	{
	  loc = &SUBREG_REG (x);
	  saw_asm |= df_insn_contains_asm (loc, insn);
	  return saw_asm;
	}
      /* ... Fall through ...  */

    case REG:
      return saw_asm;

    case SET:
      {
	rtx dst = SET_DEST (x);
	saw_asm |= df_insn_contains_asm (&SET_SRC (x), insn);
	
	switch (GET_CODE (dst))
	  {
	  case SUBREG:
	  case REG:
	  case PARALLEL:
	  case SCRATCH:
	  case PC:
	  case CC0:
	  case MEM:
	  case STRICT_LOW_PART:
	  case ZERO_EXTRACT:
	  case SIGN_EXTRACT:
	    break;
	  default:
	    gcc_unreachable ();
	  }
	return saw_asm;
      }

    case RETURN:
      break;

    case ASM_OPERANDS:
    case UNSPEC_VOLATILE:
    case TRAP_IF:
    case ASM_INPUT:
      {
	/* Traditional and volatile asm instructions must be considered to use
	   and clobber all hard registers, all pseudo-registers and all of
	   memory.  So must TRAP_IF and UNSPEC_VOLATILE operations.

	   Consider for instance a volatile asm that changes the fpu rounding
	   mode.  An insn should not be moved across this even if it only uses
	   pseudo-regs because it might give an incorrectly rounded result.

	   For now, just mark any regs we can find in ASM_OPERANDS as
	   used.  */

	/* For all ASM_OPERANDS, we must traverse the vector of input operands.
	   We can not just fall through here since then we would be confused
	   by the ASM_INPUT rtx inside ASM_OPERANDS, which do not indicate
	   traditional asms unlike their normal usage.  */
	if (code == ASM_OPERANDS)
	  return true;
	break;
      }

    case PRE_DEC:
    case POST_DEC:
    case PRE_INC:
    case POST_INC:
    case PRE_MODIFY:
    case POST_MODIFY:
      return saw_asm;

      /* ... Fall through to handle uses ...  */

    default:
      break;
    }

  /* Recursively scan the operands of this expression.  */
  {
    const char *fmt = GET_RTX_FORMAT (code);
    int i;

    for (i = GET_RTX_LENGTH (code) - 1; i >= 0; i--)
      {
	if (fmt[i] == 'e')
	  {
	    /* Tail recursive case: save a function call level.  */
	    if (i == 0)
	      {
		loc = &XEXP (x, 0);
		goto retry;
	      }
	    saw_asm |= df_insn_contains_asm (&XEXP (x, i), insn);
	  }
	else if (fmt[i] == 'E')
	  {
	    int j;
	    for (j = 0; j < XVECLEN (x, i); j++)
	      saw_asm |= df_insn_contains_asm (&XVECEXP (x, i, j), insn);
	  }
      }
  }
  return saw_asm;
}

/* Mark a register in SET.  Hard registers in large modes get all
   of their component registers set as well.  */

static void
mark_reg (rtx reg, void * vset)
{
  bitmap set = (bitmap) vset;
  int regno = REGNO (reg);

  gcc_assert (GET_MODE (reg) != BLKmode);

  bitmap_set_bit (set, regno);
  if (regno < FIRST_PSEUDO_REGISTER)
    {
      int n = hard_regno_nregs[regno][GET_MODE (reg)];
      while (--n > 0)
	bitmap_set_bit  (set, regno + n);
    }
}

/* Record the set of hard registers that are used in the exit block.  */
static void
df_record_exit_block_uses (struct df * df)
{
  unsigned int i; 
  bitmap_iterator bi;

  bitmap_clear (df->exit_block_uses);
  
  /* If exiting needs the right stack value, consider the stack
     pointer live at the end of the function.  */
  if ((HAVE_epilogue && epilogue_completed)
      || ! EXIT_IGNORE_STACK
      || (! FRAME_POINTER_REQUIRED
	  && ! current_function_calls_alloca
	  && flag_omit_frame_pointer)
      || current_function_sp_is_unchanging)
    {
      bitmap_set_bit (df->exit_block_uses, STACK_POINTER_REGNUM);
    }
  
  /* Mark the frame pointer if needed at the end of the function.
     If we end up eliminating it, it will be removed from the live
     list of each basic block by reload.  */
  
  if (! reload_completed || frame_pointer_needed)
    {
      bitmap_set_bit (df->exit_block_uses, FRAME_POINTER_REGNUM);
#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
      /* If they are different, also mark the hard frame pointer as live.  */
      if (! LOCAL_REGNO (HARD_FRAME_POINTER_REGNUM))
	bitmap_set_bit (df->exit_block_uses, HARD_FRAME_POINTER_REGNUM);
#endif
    }

#ifndef PIC_OFFSET_TABLE_REG_CALL_CLOBBERED
  /* Many architectures have a GP register even without flag_pic.
     Assume the pic register is not in use, or will be handled by
     other means, if it is not fixed.  */
  if ((unsigned) PIC_OFFSET_TABLE_REGNUM != INVALID_REGNUM
      && fixed_regs[PIC_OFFSET_TABLE_REGNUM])
    bitmap_set_bit (df->exit_block_uses, PIC_OFFSET_TABLE_REGNUM);
#endif
  
  /* Mark all global registers, and all registers used by the
     epilogue as being live at the end of the function since they
     may be referenced by our caller.  */
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (global_regs[i] || EPILOGUE_USES (i))
      bitmap_set_bit (df->exit_block_uses, i);
  
  if (HAVE_epilogue && epilogue_completed)
    {
      /* Mark all call-saved registers that we actually used.  */
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	if (regs_ever_live[i] && ! LOCAL_REGNO (i)
	    && ! TEST_HARD_REG_BIT (regs_invalidated_by_call, i))
	  bitmap_set_bit (df->exit_block_uses, i);
    }
  
#ifdef EH_RETURN_DATA_REGNO
  /* Mark the registers that will contain data for the handler.  */
  if (reload_completed && current_function_calls_eh_return)
    for (i = 0; ; ++i)
      {
	unsigned regno = EH_RETURN_DATA_REGNO(i);
	if (regno == INVALID_REGNUM)
	  break;
	bitmap_set_bit (df->exit_block_uses, regno);
      }
#endif

#ifdef EH_RETURN_STACKADJ_RTX
  if ((! HAVE_epilogue || ! epilogue_completed)
      && current_function_calls_eh_return)
    {
      rtx tmp = EH_RETURN_STACKADJ_RTX;
      if (tmp && REG_P (tmp))
	mark_reg (tmp, df->exit_block_uses);
    }
#endif

#ifdef EH_RETURN_HANDLER_RTX
  if ((! HAVE_epilogue || ! epilogue_completed)
      && current_function_calls_eh_return)
    {
      rtx tmp = EH_RETURN_HANDLER_RTX;
      if (tmp && REG_P (tmp))
	mark_reg (tmp, df->exit_block_uses);
    }
#endif 
  
  /* Mark function return value.  */
  diddle_return_value (mark_reg, (void*)df->exit_block_uses);

  if (df->flags & DF_HARD_REGS)
    EXECUTE_IF_SET_IN_BITMAP (df->exit_block_uses, 0, i, bi)
      {
	rtx use = df_reg_use_gen (i);
	df_ref_create (df, regno_reg_rtx[i], &XEXP (use, 0), NULL, EXIT_BLOCK_PTR,
		       DF_REF_REG_USE, DF_REF_ARTIFICIAL);
      }
}


/* Record all the df within INSN of basic block BB.  */
static void
df_insn_refs_record (struct df *df, basic_block bb, rtx insn)
{
  int i;

  if (INSN_P (insn))
    {
      rtx note;
      
      if (df_insn_contains_asm (&PATTERN (insn), insn))
	DF_INSN_CONTAINS_ASM (df, insn) = true;
      
      /* Record register defs.  */
      df_defs_record (df, PATTERN (insn), bb, insn);

      if (df->flags & DF_EQUIV_NOTES)
	for (note = REG_NOTES (insn); note;
	     note = XEXP (note, 1))
	  {
	    switch (REG_NOTE_KIND (note))
	      {
	      case REG_EQUIV:
	      case REG_EQUAL:
		df_uses_record (df, &XEXP (note, 0), DF_REF_REG_USE,
				bb, insn, DF_REF_IN_NOTE);
	      default:
		break;
	      }
	  }

      if (CALL_P (insn))
	{
	  rtx note;
	  rtx x;

	  /* Record the registers used to pass arguments.  */
	  for (note = CALL_INSN_FUNCTION_USAGE (insn); note;
	       note = XEXP (note, 1))
	    {
	      if (GET_CODE (XEXP (note, 0)) == USE)
		df_uses_record (df, &XEXP (XEXP (note, 0), 0), DF_REF_REG_USE,
				bb, insn, 0);
              else if (GET_CODE (XEXP (note, 0)) == CLOBBER)
		{
		  if (REG_P (XEXP (XEXP (note, 0), 0)))
		    {
		      rtx reg = XEXP (XEXP (note, 0), 0);
		      int regno_last;
		      int regno_first;
		      int i;
		
		      regno_last = regno_first = REGNO (reg);
		      if (regno_first < FIRST_PSEUDO_REGISTER)
			regno_last += hard_regno_nregs[regno_first][GET_MODE (reg)] - 1;
		      for (i = regno_first; i <= regno_last; i++)
			regs_ever_live[i] = 1;
		    }
		}
	    }

	  /* The stack ptr is used (honorarily) by a CALL insn.  */
	  x = df_reg_use_gen (STACK_POINTER_REGNUM);
	  df_uses_record (df, &XEXP (x, 0), DF_REF_REG_USE, bb, insn, 
			  0);

	  if (df->flags & DF_HARD_REGS)
	    {
	      bitmap_iterator bi;
	      unsigned int ui;
	      /* Calls may also reference any of the global registers,
		 so they are recorded as used.  */
	      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
		if (global_regs[i])
		  {
		    x = df_reg_use_gen (i);
		    df_uses_record (df, &XEXP (x, 0),
				    DF_REF_REG_USE, bb, insn, 
				    0);
		  }
	      EXECUTE_IF_SET_IN_BITMAP (invalidated_by_call, 0, ui, bi)
		{
		  x = df_reg_def_gen (ui);
		  df_def_record_1 (df, x, bb, insn, DF_REF_CLOBBER, false);
		}
	    }
	}

      /* Record the register uses.  */
      df_uses_record (df, &PATTERN (insn),
		      DF_REF_REG_USE, bb, insn, 0);

      if (CALL_P (insn))
	{
	  rtx note;

	  /* We do not record hard registers clobbered by the call,
	     since there are awfully many of them and "defs" created
	     through them are not interesting (since no use can be legally
	     reached by them).  So we must just make sure we include them when
	     computing kill bitmaps.  */

	  /* There may be extra registers to be clobbered.  */
	  for (note = CALL_INSN_FUNCTION_USAGE (insn);
	       note;
	       note = XEXP (note, 1))
	    if (GET_CODE (XEXP (note, 0)) == CLOBBER)
	      df_defs_record (df, XEXP (note, 0), bb, insn);
	}
    }
}


/* Record all the refs within the basic block BB.  */
static void
df_bb_refs_record (struct df *df, basic_block bb)
{
  rtx insn;
  rtx x;

  /* Scan the block an insn at a time from beginning to end.  */
  FOR_BB_INSNS (bb, insn)
    {
      if (INSN_P (insn))
	{
	  /* Record defs within INSN.  */
	  df_insn_refs_record (df, bb, insn);
	}

    }
  if ((df->flags & DF_HARD_REGS) 
      && (df->flags & DF_ARTIFICIAL_USES)
      && bb->index > NUM_FIXED_BLOCKS)
    {
      /* Before reload, there are a few registers that must be forced
	 live everywhere -- which might not already be the case for
	 blocks within infinite loops.  */
      if (! reload_completed)
	{
	  
	  /* Any reference to any pseudo before reload is a potential
	     reference of the frame pointer.  */
	  x = df_reg_use_gen (FRAME_POINTER_REGNUM);
	  df_uses_record (df, &XEXP (x, 0),
			  DF_REF_REG_USE, bb, BB_END (bb), DF_REF_ARTIFICIAL);
	  
#if FRAME_POINTER_REGNUM != ARG_POINTER_REGNUM
	  /* Pseudos with argument area equivalences may require
	     reloading via the argument pointer.  */
	  if (fixed_regs[ARG_POINTER_REGNUM])
	    {
	      x = df_reg_use_gen (ARG_POINTER_REGNUM);
	      df_uses_record (df, &XEXP (x, 0),
			      DF_REF_REG_USE, bb, BB_END (bb), 
			      DF_REF_ARTIFICIAL);
	    }
#endif
	  
	  /* Any constant, or pseudo with constant equivalences, may
	     require reloading from memory using the pic register.  */
	  if ((unsigned) PIC_OFFSET_TABLE_REGNUM != INVALID_REGNUM
	      && fixed_regs[PIC_OFFSET_TABLE_REGNUM])
	    {
	      x = df_reg_use_gen (PIC_OFFSET_TABLE_REGNUM);
	      df_uses_record (df, &XEXP (x, 0),
			      DF_REF_REG_USE, bb, BB_END (bb), 
			      DF_REF_ARTIFICIAL);
	    }
	}
      /* The all-important stack pointer must always be live.  */
      x = df_reg_use_gen (STACK_POINTER_REGNUM);
      df_uses_record (df, &XEXP (x, 0),
		      DF_REF_REG_USE, bb, BB_END (bb), DF_REF_ARTIFICIAL);

    }
}


/* Record all the refs in the basic blocks specified by BLOCKS.  */
static void
df_refs_record (struct df *df, bitmap blocks)
{
  basic_block bb;

  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
    {
      df_bb_refs_record (df, bb);
    });
  if (bitmap_bit_p (blocks, EXIT_BLOCK))
    df_record_exit_block_uses (df);
}

/* Dataflow analysis routines.  */

/* Create reg-def chains for basic block BB.  These are a list of
   definitions for each register.  */

static void
df_bb_reg_def_chain_create (struct df *df, basic_block bb)
{
  rtx insn;

  /* Perhaps the defs should be sorted using a depth first search
     of the CFG (or possibly a breadth first search).  */
  bitmap seen = BITMAP_ALLOC (NULL);

  FOR_BB_INSNS_REVERSE (bb, insn)
    {
      struct df_link *link;
      unsigned int uid = INSN_UID (insn);

      if (! INSN_P (insn))
	continue;

      for (link = df->insns[uid].defs; link; link = link->next)
	{
	  struct ref *def = link->ref;
	  unsigned int dregno = DF_REF_REGNO (def);

	  if (bitmap_bit_p (seen, dregno))
	    def->flags |= DF_HIDDEN;
	  bitmap_set_bit (seen, dregno);

          /* Do not add ref's to the chain twice, i.e., only add new
             refs.  XXX the same could be done by testing if the
             current insn is a modified (or a new) one.  This would be
             faster.  */
          if (DF_REF_ID (def) < df->def_id_save)
            continue;

	  df->regs[dregno].defs = df_link_create (df, def, 
						  df->regs[dregno].defs);
	}
    }
  BITMAP_FREE (seen);
}


/* Create reg-def chains for each basic block within BLOCKS.  These
   are a list of definitions for each register.  If REDO is true, add
   all defs, otherwise just add the new defs.  */

static void
df_reg_def_chain_create (struct df *df, bitmap blocks, bool redo, bool renumber)
{
  basic_block bb;
#ifdef ENABLE_CHECKING
  unsigned regno;
#endif
  unsigned old_def_id_save = df->def_id_save;

  if (redo)
    {
#ifdef ENABLE_CHECKING
      for (regno = 0; regno < df->n_regs; regno++)
	gcc_assert (!df->regs[regno].defs);
#endif

      /* Pretend that all defs are new.  */
      df->def_id_save = 0;
    }

  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
    {
      df_bb_reg_def_chain_create (df, bb);
    });

  /* Renumber the def sites so that all of the defs for a single
     variable are contigious.  */
  if (renumber)
    {
      int m = max_reg_num ();
      int dregno;
      int offset = 0;
      struct ref **dfdefstemp = xmalloc (df->def_size * sizeof (*df->defs));
      
      dfdefstemp = memcpy (&dfdefstemp[0], &df->defs[0], df->def_size * sizeof (*df->defs));
      
      df->rd_reach->begin = xrealloc (df->rd_reach->begin, m * sizeof (int));
      df->rd_reach->count = xrealloc (df->rd_reach->count, m * sizeof (int));

      for (dregno=0; dregno<m; dregno++)
	{
	  int count = 0;
	  struct df_link *l = df->regs[dregno].defs;
	  df->rd_reach->begin[dregno] = offset;
	  while (l) 
	    {
	      unsigned int oldid = DF_REF_ID (l->ref);
	      DF_REF_ID (l->ref) = offset++;
	      df->defs[DF_REF_ID (l->ref)] = dfdefstemp [oldid];
	      l = l->next;
	      count++;
	    }
	  df->rd_reach->count[dregno] = count;
	}
      free (dfdefstemp);
    }

  df->def_id_save = old_def_id_save;
}

/* Remove all reg-def chains stored in the dataflow object DF.  */

static void
df_reg_def_chain_clean (struct df *df)
{
  unsigned regno;

  for (regno = 0; regno < df->n_regs; regno++)
    free_reg_ref_chain (df, &df->regs[regno].defs);
}

/* Create reg-use chains for basic block BB.  These are a list of uses
   for each register.  */

static void
df_bb_reg_use_chain_create (struct df *df, basic_block bb)
{
  rtx insn;

  /* Scan in forward order so that the last uses appear at the start
     of the chain.  */

  FOR_BB_INSNS (bb, insn)
    {
      struct df_link *link;
      unsigned int uid = INSN_UID (insn);

      if (! INSN_P (insn))
	continue;

      for (link = df->insns[uid].uses; link; link = link->next)
	{
	  struct ref *use = link->ref;
	  unsigned int uregno = DF_REF_REGNO (use);

          /* Do not add ref's to the chain twice, i.e., only add new
             refs.  XXX the same could be done by testing if the
             current insn is a modified (or a new) one.  This would be
             faster.  */
          if (DF_REF_ID (use) < df->use_id_save)
            continue;

	  df->regs[uregno].uses
	    = df_link_create (df, use, df->regs[uregno].uses);
	}
    }
}

/* Link in the artifical hardware register uses at the exit block.  */
static void
df_exit_bb_reg_use_chain_create (struct df *df)
{
  struct df_link *link;
  for (link = DF_BB_INFO (df, EXIT_BLOCK_PTR)->artificial_uses; 
       link; 
       link = link->next)
    {
      struct ref *use = link->ref;
      unsigned int uregno = DF_REF_REGNO (use);
      
      /* Do not add ref's to the chain twice, i.e., only add new
	 refs.  XXX the same could be done by testing if the
	 current insn is a modified (or a new) one.  This would be
	 faster.  */
      if (DF_REF_ID (use) < df->use_id_save)
	continue;
      
      df->regs[uregno].uses
	= df_link_create (df, use, df->regs[uregno].uses);
    }
}

/* Create reg-use chains for each basic block within BLOCKS.  These
   are a list of uses for each register.  If REDO is true, remove the
   old reg-use chains first, otherwise just add new uses to them.  */

static void
df_reg_use_chain_create (struct df *df, bitmap blocks, bool redo, bool renumber)
{
  basic_block bb;
#ifdef ENABLE_CHECKING
  unsigned regno;
#endif
  unsigned old_use_id_save = df->use_id_save;

  if (redo)
    {
#ifdef ENABLE_CHECKING
      for (regno = 0; regno < df->n_regs; regno++)
	gcc_assert (!df->regs[regno].uses);
#endif

      /* Pretend that all uses are new.  */
      df->use_id_save = 0;
    }

  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
    {
      df_bb_reg_use_chain_create (df, bb);
    });

  if (bitmap_bit_p (blocks, EXIT_BLOCK))
    df_exit_bb_reg_use_chain_create (df);

  /* Renumber the def sites so that all of the defs for a single
     variable are contigious.  */
  if (renumber)
    {
      int m = max_reg_num ();
      int dregno;
      int offset = 0;
      struct ref **dfusestemp = xmalloc (df->use_size * sizeof (*df->uses));
      
      dfusestemp = memcpy (&dfusestemp[0], &df->uses[0], df->use_size * sizeof (*df->uses));
    
      df->ru_reach->begin = xrealloc (df->ru_reach->begin, m * sizeof (int));
      df->ru_reach->count = xrealloc (df->ru_reach->count, m * sizeof (int));

      for (dregno=0; dregno<m; dregno++)
	{
	  int count = 0;
	  struct df_link *l = df->regs[dregno].uses;
	  df->ru_reach->begin[dregno] = offset;
	  while (l) 
	    {
	      unsigned int oldid = DF_REF_ID (l->ref);
	      DF_REF_ID (l->ref) = offset++;
	      df->uses[DF_REF_ID (l->ref)] = dfusestemp[oldid];
	      l = l->next;
	      count++;
	    }
	  df->ru_reach->count[dregno] = count;
	}
      free (dfusestemp);
    }
  
  df->use_id_save = old_use_id_save;
}

/* Remove all reg-use chains stored in the dataflow object DF.  */

static void
df_reg_use_chain_clean (struct df *df)
{
  unsigned regno;

  for (regno = 0; regno < df->n_regs; regno++)
    free_reg_ref_chain (df, &df->regs[regno].uses);
}

/* Create def-use chains from reaching use bitmaps for basic block BB.  */
static void
df_bb_chain_create (struct df *df, int flags, basic_block bb)
{
  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);
  rtx insn;
  bitmap cpy = BITMAP_ALLOC (NULL);

  bitmap_copy (cpy, bb_info->rd_in);

  /* For each def in BB create a linked list (chain) of uses
     reached from the def.  */
  FOR_BB_INSNS (bb, insn)
    {
      struct df_link *def_link;
      struct df_link *use_link;
      unsigned int uid = INSN_UID (insn);

      if (! INSN_P (insn))
	continue;

      /* Now scan the uses and link them up with the defs that remain
	 in the cpy vector.  */
      for (use_link = df->insns[uid].uses; use_link; use_link = use_link->next)
	{
	  struct ref *use = use_link->ref;
	  unsigned int uregno = DF_REF_REGNO (use);
	  bitmap_iterator bi;
	  unsigned int def_index;
	  int count = df->rd_reach->count[uregno];
	  if (count)
	    {
	      unsigned int first_index = df->rd_reach->begin[uregno];
	      unsigned int last_index = first_index + count - 1;
	      
	      EXECUTE_IF_SET_IN_BITMAP (cpy, first_index, def_index, bi)
		{
		  struct ref *def;
		  if (def_index > last_index) 
		    break;
		  
		  def = df->defs[def_index];
		  if (flags & DF_DU_CHAIN)
		    DF_REF_CHAIN (def)
		      = df_link_create (df, use, DF_REF_CHAIN (def));
		  if (flags & DF_UD_CHAIN)
		    DF_REF_CHAIN (use)
		      = df_link_create (df, def, DF_REF_CHAIN (use));
		}
	    }
	}

      /* Since we are going forwards, process the defs second.  */
      for (def_link = df->insns[uid].defs; def_link; def_link = def_link->next)
	{
	  struct ref *def = def_link->ref;
	  unsigned int dregno = DF_REF_REGNO (def);
	  bitmap_clear_range (cpy, 
			      df->rd_reach->begin[dregno], 
			      df->rd_reach->count[dregno]);
	  if (! (DF_REF_FLAGS (def) & DF_REF_CLOBBER))
	    bitmap_set_bit (cpy, DF_REF_ID (def));
	}
    }
}

/* Create def-use chains from reaching use bitmaps for the artifical
   uses in the exit block.  */
static void
df_exit_bb_chain_create (struct df *df, int flags)
{
  struct df_bb_info *bb_info = DF_BB_INFO_INDEX (df, EXIT_BLOCK);
  struct df_link *use_link;
  for (use_link = DF_BB_INFO (df, EXIT_BLOCK_PTR)->artificial_uses; 
       use_link; 
       use_link = use_link->next)
    {
      struct ref *use = use_link->ref;
      unsigned int uregno = DF_REF_REGNO (use);
      bitmap_iterator bi;
      unsigned int def_index;
      int count = df->rd_reach->count[uregno];
      if (count)
	{
	  unsigned int first_index = df->rd_reach->begin[uregno];
	  unsigned int last_index = first_index + count - 1;
	  
	  EXECUTE_IF_SET_IN_BITMAP (bb_info->rd_in, first_index, def_index, bi)
	    {
	      struct ref *def;
	      if (def_index > last_index) 
		break;
	      
	      def = df->defs[def_index];
	      if (flags & DF_DU_CHAIN)
		DF_REF_CHAIN (def)
		  = df_link_create (df, use, DF_REF_CHAIN (def));
	      if (flags & DF_UD_CHAIN)
		DF_REF_CHAIN (use)
		  = df_link_create (df, def, DF_REF_CHAIN (use));
	    }
	}
    }
}


/* Create def-use chains from reaching use bitmaps for basic blocks
   in BLOCKS.  */
static void
df_chain_create (struct df *df, int flags, bitmap blocks)
{
#ifdef ENABLE_CHECKING
  basic_block bb;
  rtx insn;

  /* Clear out the existing chains.  This was in the original code.
     God only knows where the storage is going.  */
  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
    {
      FOR_BB_INSNS (bb, insn)
	{
	  struct df_link *def_link;
	  struct df_link *use_link;
	  unsigned int uid = INSN_UID (insn);
	  
	  if (! INSN_P (insn))
	    continue;
	
	  if (flags & DF_DU_CHAIN)
	    for (def_link = df->insns[uid].defs; def_link; def_link = def_link->next)
	      {
		struct ref *def = def_link->ref;
		gcc_assert (DF_REF_CHAIN (def) == 0);
	 
	      }
 
	  if (flags & DF_UD_CHAIN)
	    for (use_link = df->insns[uid].uses; use_link; use_link = use_link->next)
	      {
		struct ref *use = use_link->ref;
		gcc_assert (DF_REF_CHAIN (use) == 0);
	      }
	}
    });
#endif

  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
    {
      df_bb_chain_create (df, flags, bb);
    });

  if (bitmap_bit_p (blocks, EXIT_BLOCK))
    df_exit_bb_chain_create (df, flags);
  
}


/* Local functions for REACHING DEFINITIONS.  */

static void 
df_rd_init (struct dataflow * dataflow, int bb_index)
{
  struct df * df = dataflow->df;
  struct df_bb_info *bb_info = DF_BB_INFO_INDEX (df, bb_index);

  bitmap_copy (bb_info->rd_out, bb_info->rd_gen);
}

/* In of target gets or of out of source.  */

static void
df_rd_confluence_n (struct dataflow * dataflow, edge e)
{
  struct df * df = dataflow->df;
  bitmap op1 = DF_BB_INFO (df, e->dest)->rd_in;
  bitmap op2 = DF_BB_INFO (df, e->src)->rd_out;

  if (e->flags & EDGE_EH)
    {
      bitmap_ior_and_compl_into (op1, op2, df->rd_reach->invalidated_by_call);
#ifdef EH_USES
      /* If a target saves one register in another (instead of on
	 the stack) the save register will need to be live for EH.  */
      bitmap_ior_into (op1, df->rd_reach->eh_uses);
#endif
    }
  else
    bitmap_ior_into (op1, op2);
}


/* Transfer function.  */
static bool
df_rd_transfer_function (struct dataflow * dataflow, int bb_index)
{
  struct df * df = dataflow->df;
  struct df_bb_info *bb_info = DF_BB_INFO_INDEX (df, bb_index);
  unsigned int regno;
  bitmap_iterator bi;
  bitmap in = bb_info->rd_in;
  bitmap out = bb_info->rd_out;
  bitmap gen = bb_info->rd_gen;
  bitmap kill = bb_info->rd_kill;
  bitmap sparse_kill = bb_info->rd_sparse_kill;

  if (bitmap_empty_p (sparse_kill))
    return  bitmap_ior_and_compl (out, gen, in, kill);
  else 
    {
      bool changed = false;
      bitmap tmp = BITMAP_ALLOC (NULL);
      bitmap_copy (tmp, in);
      EXECUTE_IF_SET_IN_BITMAP (sparse_kill, 0, regno, bi)
	{
	  bitmap_clear_range (tmp, 
			      df->rd_reach->begin[regno], 
			      df->rd_reach->count[regno]);
	}
      bitmap_and_compl_into (tmp, kill);
      bitmap_ior_into (tmp, gen);
      changed = !bitmap_equal_p (tmp, out);
      if (changed)
	{
	  BITMAP_FREE (out);
	  bb_info->rd_out = tmp;
	}
      else 
	  BITMAP_FREE (tmp);
      return changed;
    }
}


/* Set the Reaching definitions problem in DFLOW.  */
static void 
df_rd_set_dflow(struct dataflow *dflow, struct df * df, 
		bitmap viewing, bitmap iterating, 
		int * postorder, int n_blocks, bool single_pass)
{
  dflow->df = df;
  dflow->viewing = viewing;
  dflow->iterating = iterating;
  dflow->postorder = postorder;
  dflow->n_blocks = n_blocks;
  dflow->single_pass = single_pass;

  dflow->problem = &problem_RD;
  dflow->data = NULL;
}

/* Return an array of unallocated bitmaps, one for each register in DF.  */
static struct df_reach *
df_alloc_reach_bitmaps (struct df *df)
{
  struct df_reach * reach = xcalloc (1, sizeof (struct df_reach));

  reach->begin = xcalloc (df->n_regs, sizeof (int));
  reach->count = xcalloc (df->n_regs, sizeof (int));
  reach->regs = XCNEWVEC (bitmap, df->n_regs);
  reach->invalidated_by_call = BITMAP_ALLOC (NULL);
#ifdef EH_USES
  reach->eh_uses = BITMAP_ALLOC (NULL); 
#endif
  return reach;
}

/* Free an array allocated by df_alloc_reach_bitmaps.  */
static void
df_free_reach_bitmaps (struct df *df, struct df_reach *reach)
{
  unsigned int i;

  for (i = 0; i < df->n_regs; i++)
    if (reach->regs[i])
      BITMAP_FREE (reach->regs[i]);
  XDELETE (reach->regs);

  BITMAP_FREE (reach->invalidated_by_call);
#ifdef EH_USES
  BITMAP_FREE (reach->eh_uses);
#endif
  free (reach->begin);
  free (reach->count);
  free (reach);
}

/* Clear all of the bitmaps from the array allocated by
   df_alloc_reach_bitmaps.  */ 
static void
df_clear_reach_bitmaps (struct df *df, struct df_reach *reach)
{
  unsigned int i;

  for (i = 0; i < df->n_regs; i++)
    if (reach->regs[i])
      {
	BITMAP_FREE (reach->regs[i]);
	reach->regs[i] = NULL;
      }
  bitmap_clear (reach->invalidated_by_call);
#ifdef EH_USES
  bitmap_clear (reach->eh_uses);
#endif
}

/* Return the set of reference ids in CHAIN, caching the result in *BMAP.  */
static inline bitmap
df_ref_bitmap (bitmap *bmap, unsigned int regno, struct df_link *chain)
{
  struct df_link *link;
  bitmap ids;

  ids = bmap[regno];
  if (ids == NULL)
    {
      bmap[regno] = ids = BITMAP_ALLOC (NULL);
      for (link = chain; link; link = link->next)
	bitmap_set_bit (ids, DF_REF_ID (link->ref));
    }
  return ids;
}

/* Compute local reaching def info for basic block BB.  RD_REACH->REGS[R]
   caches the set of definitions for register R.  */
static void
df_bb_rd_local_compute (struct df *df, basic_block bb)
{
  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);
  rtx insn;

  FOR_BB_INSNS (bb, insn)
    {
      unsigned int uid = INSN_UID (insn);
      struct df_link *def_link;

      if (! INSN_P (insn))
	continue;

      for (def_link = df->insns[uid].defs; def_link; def_link = def_link->next)
	{
	  struct ref *def = def_link->ref;
	  unsigned int regno = DF_REF_REGNO (def);
	  bitmap defs = df_ref_bitmap (df->rd_reach->regs, regno, 
				       df->regs[regno].defs);
	  if (! (def->flags & DF_HIDDEN))
	    {
	      if (df->rd_reach->count[regno] > DF_SPARSE_THRESHOLD)
		bitmap_set_bit (bb_info->rd_sparse_kill, regno);
	      else
		{
		  bitmap_ior_into (bb_info->rd_kill, defs);
		  bitmap_and_compl_into (bb_info->rd_gen, defs);
		}
	    }
	  if (! (DF_REF_FLAGS (def) & DF_REF_CLOBBER))
	    bitmap_set_bit (bb_info->rd_gen, DF_REF_ID (def));
	}
    }
}


/* Compute local reaching def info for each basic block within BLOCKS.  */
static void
df_rd_local_compute (struct df *df, bitmap blocks)
{
  basic_block bb;

  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
  {
    df_bb_rd_local_compute (df, bb);
  });
}


/* Local functions for REACHING USES.  */


static void 
df_ru_init (struct dataflow * dataflow, int bb_index)
{
  struct df * df = dataflow->df;
  struct df_bb_info *bb_info = DF_BB_INFO_INDEX (df, bb_index);

  bitmap_copy (bb_info->ru_in, bb_info->ru_gen);
}

/* Out of target gets or of in of source.  */

static void
df_ru_confluence_n (struct dataflow * dataflow, edge e)
{
  struct df * df = dataflow->df;
  bitmap op1 = DF_BB_INFO (df, e->src)->ru_out;
  bitmap op2 = DF_BB_INFO (df, e->dest)->ru_in;

  if (e->flags & EDGE_EH)
    {
      bitmap_ior_and_compl_into (op1, op2, df->ru_reach->invalidated_by_call);

#ifdef EH_USES
      /* If a target saves one register in another (instead of on
	 the stack) the save register will need to be live for EH.  */
      bitmap_ior_into (op1, df->ru_reach->eh_uses);
#endif
    }
  else
    bitmap_ior_into (op1, op2);
}

/* Transfer function.  */
static bool
df_ru_transfer_function (struct dataflow * dataflow, int bb_index)
{
  struct df * df = dataflow->df;
  struct df_bb_info *bb_info = DF_BB_INFO_INDEX (df, bb_index);
  bool changed = false;
  unsigned int regno;
  bitmap_iterator bi;
  bitmap in = bb_info->ru_in;
  bitmap out = bb_info->ru_out;
  bitmap gen = bb_info->ru_gen;
  bitmap kill = bb_info->ru_kill;
  bitmap sparse_kill = bb_info->ru_sparse_kill;

  EXECUTE_IF_SET_IN_BITMAP (sparse_kill, 0, regno, bi)
    {
      changed |= bitmap_ior_and_compl (in, gen, out, df->ru_reach->regs[regno]);
    }
  
  changed |= bitmap_ior_and_compl (in, gen, out, kill);
  return changed;
}


/* Set the reaching uses problem in DFLOW.  */
static void 
df_ru_set_dflow(struct dataflow *dflow, struct df * df, 
		bitmap viewing, bitmap iterating, 
		int * postorder, int n_blocks, bool single_pass)
{
  dflow->df = df;
  dflow->viewing = viewing;
  dflow->iterating = iterating;
  dflow->postorder = postorder;
  dflow->n_blocks = n_blocks;
  dflow->single_pass = single_pass;

  dflow->problem = &problem_RU;
  dflow->data = NULL;
}


/* Compute local reaching use (upward exposed use) info for basic
   block BB.  RU_REACH->REGS[R] caches the set of uses for register R.  */
static void
df_bb_ru_local_compute (struct df *df, basic_block bb)
{
  /* This is much more tricky than computing reaching defs.  With
     reaching defs, defs get killed by other defs.  With upwards
     exposed uses, these get killed by defs with the same regno.  */

  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);
  rtx insn;


  FOR_BB_INSNS_REVERSE (bb, insn)
    {
      unsigned int uid = INSN_UID (insn);
      struct df_link *def_link;
      struct df_link *use_link;

      if (! INSN_P (insn))
	continue;

      for (def_link = df->insns[uid].defs; def_link; def_link = def_link->next)
	{
	  struct ref *def = def_link->ref;
	  unsigned int regno = DF_REF_REGNO (def);
	  bitmap uses = df_ref_bitmap (df->ru_reach->regs, regno, 
				       df->regs[regno].uses);
	  bitmap_ior_into (bb_info->ru_kill, uses);
	  bitmap_and_compl_into (bb_info->ru_gen, uses);
	}

      for (use_link = df->insns[uid].uses; use_link; use_link = use_link->next)
	{
	  struct ref *use = use_link->ref;
	  /* Add use to set of gens in this BB.  */
	  bitmap_set_bit (bb_info->ru_gen, DF_REF_ID (use));
	}
    }
}

/* Compute local reaching use (upward exposed use) info for each basic
   block within BLOCKS.  */
static void
df_ru_local_compute (struct df *df, bitmap blocks)
{
  basic_block bb;

  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
  {
    df_bb_ru_local_compute (df, bb);
  });
}


/* Local functions for LIVE REGISTERS.  */

static void 
df_lr_init (struct dataflow * dataflow, int idx)
{
  struct df * df = dataflow->df;
  struct df_bb_info *bb_info = DF_BB_INFO_INDEX (df, idx);

  bitmap_copy (bb_info->lr_in, bb_info->lr_use);
}

/* Transfer function.  */
static bool
df_lr_transfer_function (struct dataflow * dataflow, int bb_index)
{
  struct df * df = dataflow->df;
  struct df_bb_info *bb_info = DF_BB_INFO_INDEX (df, bb_index);
  bitmap in = bb_info->lr_in;
  bitmap out = bb_info->lr_out;
  bitmap use = bb_info->lr_use;
  bitmap def = bb_info->lr_def;

  return bitmap_ior_and_compl (in, use, out, def);
}


/* Set the live registers problem in DFLOW.  */
static void 
df_lr_set_dflow(struct dataflow *dflow, struct df * df, 
		bitmap viewing, bitmap iterating, 
		int * postorder, int n_blocks, bool single_pass)
{
  dflow->df = df;
  dflow->viewing = viewing;
  dflow->iterating = iterating;
  dflow->postorder = postorder;
  dflow->n_blocks = n_blocks;
  dflow->single_pass = single_pass;

  dflow->problem = &problem_LR;
  dflow->data = NULL;
}

/* Confluence function that processes infinite loops.  This might be a
   noreturn function that throws.  And even if it isn't, getting the
   unwind info right helps debugging.  */
static void
df_lr_confluence_0 (struct dataflow * dataflow, basic_block bb)
{
  struct df * df = dataflow->df;
  bitmap op1 = DF_BB_INFO (df, bb)->lr_out;
  
  if (bb != EXIT_BLOCK_PTR)
    {
      bitmap_copy (op1, df->hardware_regs_used);
#ifdef EH_USES
      bitmap_ior_into (op1, eh_uses);
#endif
    }
} 


/* Confluence function that ignores fake edges.  */
static void
df_lr_confluence_n (struct dataflow * dataflow, edge e)
{
  struct df * df = dataflow->df;
  bitmap op1 = DF_BB_INFO (df, e->src)->lr_out;
  bitmap op2 = DF_BB_INFO (df, e->dest)->lr_in;
 
  /* Call-clobbered registers die across exception and call edges.  */
  /* ??? Abnormal call edges ignored for the moment, as this gets
     confused by sibling call edges, which crashes reg-stack.  */
  if (e->flags & EDGE_EH)
    {
      bitmap_ior_and_compl_into (op1, op2, invalidated_by_call);

#ifdef EH_USES
      /* If a target saves one register in another (instead of on
	 the stack) the save register will need to be live for EH.  */
      bitmap_ior_into (op1, eh_uses);
#endif
    }
  else
    bitmap_ior_into (op1, op2);

  bitmap_ior_into (op1, df->hardware_regs_used);
} 



/* Compute local live variable info for the exit block.  */

static void
df_exit_lr_local_compute (struct df *df)
{
  struct df_bb_info *bb_info = DF_BB_INFO (df, EXIT_BLOCK_PTR);
  bitmap_copy (bb_info->lr_use, df->exit_block_uses);
}

/* Compute local live register info for basic block BB.  */
static void
df_bb_lr_local_compute (struct df *df, basic_block bb)
{
  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);
  rtx insn;
  struct df_link *link;

  FOR_BB_INSNS_REVERSE (bb, insn)
    {
      unsigned int uid = INSN_UID (insn);

      if (! INSN_P (insn))
	continue;	

      for (link = df->insns[uid].defs; link; link = link->next)
	{
	  struct ref *def = link->ref;
	  unsigned int dregno = DF_REF_REGNO (def);
	  
	  if (!CALL_P (insn))
	    {
	      if (dregno < FIRST_PSEUDO_REGISTER)
		{
		  unsigned int i;
		  for (i = dregno; 
		       i <= dregno + hard_regno_nregs[dregno][GET_MODE (DF_REF_REG (def))] - 1;
		       ++i)
		    {
		      if (DF_INSN_CONTAINS_ASM (df, insn))
			regs_asm_clobbered[i] = 1;
		    }
		}
	      /* Add def to set of defs in this BB.  */
	      bitmap_set_bit (bb_info->lr_def, dregno);
	      bitmap_clear_bit (bb_info->lr_use, dregno);
	    }
	  else
	    {
	      struct ref *def = link->ref;
	      unsigned int dregno = DF_REF_REGNO (def);
	      
	      if (dregno >= FIRST_PSEUDO_REGISTER
		  || !(SIBLING_CALL_P (insn)
		       && bitmap_bit_p (DF_BB_INFO (df, EXIT_BLOCK_PTR)->lr_use, dregno)
		       && !refers_to_regno_p (dregno, dregno+1,
					      current_function_return_rtx,
					      (rtx *)0)))
		{
		  /* Add def to set of defs in this BB.  */
		  bitmap_set_bit (bb_info->lr_def, dregno);
		  bitmap_clear_bit (bb_info->lr_use, dregno);
		}
	    }
	}

      for (link = df->insns[uid].uses; link; link = link->next)
	{
	  struct ref *use = link->ref;
	  unsigned int dregno = DF_REF_REGNO (use);
	  if (dregno < FIRST_PSEUDO_REGISTER)
	    {
#if 0
	      if (DF_INSN_CONTAINS_ASM (df, insn))
		regs_asm_clobbered[dregno] = 1;
#endif
	    }
	  /* Add use to set of uses in this BB.  */
	  bitmap_set_bit (bb_info->lr_use, DF_REF_REGNO (use));
	}
    }

  for (link = DF_BB_INFO (df, bb)->artificial_uses; link; link = link->next)
    {
      struct ref *use = link->ref;
      /* Add use to set of uses in this BB.  */
      bitmap_set_bit (bb_info->lr_use, DF_REF_REGNO (use));
    }
  
}


/* Compute local live register info for each basic block within BLOCKS.  */

static void
df_lr_local_compute (struct df *df, bitmap blocks, bool everything)
{
  basic_block bb;

  /* Assume that the stack pointer is unchanging if alloca hasn't
     been used.  */
  if (everything)
    {
#if 0
      memset (regs_ever_live, 0, sizeof (regs_ever_live));
#endif
      memset (regs_asm_clobbered, 0, sizeof (regs_asm_clobbered));
    }

  bitmap_clear (df->hardware_regs_used);

  /* The all-important stack pointer must always be live.  */
  bitmap_set_bit (df->hardware_regs_used, STACK_POINTER_REGNUM);

  /* Before reload, there are a few registers that must be forced
     live everywhere -- which might not already be the case for
     blocks within infinite loops.  */
  if (! reload_completed)
    {
      /* Any reference to any pseudo before reload is a potential
	 reference of the frame pointer.  */
      bitmap_set_bit (df->hardware_regs_used, FRAME_POINTER_REGNUM);
      
#if FRAME_POINTER_REGNUM != ARG_POINTER_REGNUM
      /* Pseudos with argument area equivalences may require
	 reloading via the argument pointer.  */
      if (fixed_regs[ARG_POINTER_REGNUM])
	bitmap_set_bit (df->hardware_regs_used, ARG_POINTER_REGNUM);
#endif
      
      /* Any constant, or pseudo with constant equivalences, may
	 require reloading from memory using the pic register.  */
      if ((unsigned) PIC_OFFSET_TABLE_REGNUM != INVALID_REGNUM
	  && fixed_regs[PIC_OFFSET_TABLE_REGNUM])
	bitmap_set_bit (df->hardware_regs_used, PIC_OFFSET_TABLE_REGNUM);
    }

  if (bitmap_bit_p (blocks, EXIT_BLOCK))
    {
      /* The exit block is special for this problem and its bits are
	 computed from thin air.  */
      df_exit_lr_local_compute (df);
    }
  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
  {
    if (bb == EXIT_BLOCK_PTR)
      continue;
    df_bb_lr_local_compute (df, bb);
  });

}



/* Local functions for UNINITIALIZED REGISTERS.  */

static void 
df_ur_init (struct dataflow * dataflow, int idx)
{
  struct df * df = dataflow->df;
  struct df_bb_info *bb_info = DF_BB_INFO_INDEX (df, idx);

  /* FIXME: This is a hack, it has been copied over from
     make_accurate_live_analysis by Vlad.  Most likely it is necessary
     because the generation of gen and kill information for hardware
     registers in ur is a subset of what is really necessary and what
     is done for the lr problem.  */

  /* Inside the register allocator, partial availability is only
     allowed for the psuedo registers.  To implement this, the rr is
     initially iored with a mask ones for the hard registers and zeros
     for the pseudos before being iterated.  This means that each
     hardware register will be live unless explicitly killed by some
     statement.  Eventually most of these bit will die because the
     results of rr are anded with the results of lr before being used.
     Outside of register allocation, a more conservative strategy of
     completely ignoring the unintialized registers is imployed in the
     finalizer function.  */
  if (df->state & DF_SCAN_GLOBAL)
    {
      bitmap_ior (bb_info->ur_out, bb_info->ur_gen, all_hard_regs);
      bitmap_copy (bb_info->ur_in, all_hard_regs);
    }
  else
    bitmap_copy (bb_info->ur_out, bb_info->ur_gen);
}

/* Transfer function.  */
static bool
df_ur_transfer_function (struct dataflow * dataflow, int bb_index)
{
  struct df * df = dataflow->df;
  struct df_bb_info *bb_info = DF_BB_INFO_INDEX (df, bb_index);
  bitmap in = bb_info->ur_in;
  bitmap out = bb_info->ur_out;
  bitmap gen = bb_info->ur_gen;
  bitmap kill = bb_info->ur_kill;

  return bitmap_ior_and_compl (out, gen, in, kill);
}


/* Set the live registers problem in DFLOW.  */
static void 
df_ur_set_dflow(struct dataflow *dflow, struct df * df, 
		bitmap viewing, bitmap iterating, 
		int * postorder, int n_blocks, bool single_pass)
{
  dflow->df = df;
  dflow->viewing = viewing;
  dflow->iterating = iterating;
  dflow->postorder = postorder;
  dflow->n_blocks = n_blocks;
  dflow->single_pass = single_pass;

  dflow->problem = &problem_UR;
  dflow->data = NULL;
}

/* Confluence function that ignores fake edges.  */
static void
df_ur_confluence_n (struct dataflow * dataflow, edge e)
{
  struct df * df = dataflow->df;
  bitmap op1 = DF_BB_INFO (df, e->dest)->ur_in;
  bitmap op2 = DF_BB_INFO (df, e->src)->ur_out;
 
  if (e->flags & EDGE_FAKE) 
    return;

  bitmap_ior_into (op1, op2);
} 

/* The function modifies local info for register REG being changed in
   SETTER.  DATA is used to pass the current basic block info.  */

static void
mark_reg_change (rtx reg, rtx setter, void *data)
{
  int regno;
  int endregno;
  int i;
  struct df_bb_info *bb_info = (struct df_bb_info*) data;

  if (GET_CODE (reg) == SUBREG)
    reg = SUBREG_REG (reg);

  if (!REG_P (reg))
    return;
  
  
  endregno = regno = REGNO (reg);
  if (regno < FIRST_PSEUDO_REGISTER)
    {
      endregno +=hard_regno_nregs[regno][GET_MODE (reg)];
      
      for (i = regno; i < endregno; i++)
	{
	  bitmap_set_bit (bb_info->ur_kill, i);
	  
	  if (GET_CODE (setter) != CLOBBER)
	    bitmap_set_bit (bb_info->ur_gen, i);
	  else
	    bitmap_clear_bit (bb_info->ur_gen, i);
	}
    }
  else
    {
      bitmap_set_bit (bb_info->ur_kill, regno);
      
      if (GET_CODE (setter) != CLOBBER)
	bitmap_set_bit (bb_info->ur_gen, regno);
      else
	bitmap_clear_bit (bb_info->ur_gen, regno);
    }
}
/* Classes of registers which could be early clobbered in the current
   insn.  */

DEF_VEC_I(int);
DEF_VEC_ALLOC_I(int,heap);

static VEC(int,heap) *earlyclobber_regclass;

/* This function finds and stores register classes that could be early
   clobbered in INSN.  If any earlyclobber classes are found, the function
   returns TRUE, in all other cases it returns FALSE.  */

static bool
check_earlyclobber (rtx insn)
{
  int opno;
  bool found = false;

  extract_insn (insn);

  VEC_truncate (int, earlyclobber_regclass, 0);
  for (opno = 0; opno < recog_data.n_operands; opno++)
    {
      char c;
      bool amp_p;
      int i;
      enum reg_class class;
      const char *p = recog_data.constraints[opno];

      class = NO_REGS;
      amp_p = false;
      for (;;)
	{
	  c = *p;
	  switch (c)
	    {
	    case '=':  case '+':  case '?':
	    case '#':  case '!':
	    case '*':  case '%':
	    case 'm':  case '<':  case '>':  case 'V':  case 'o':
	    case 'E':  case 'F':  case 'G':  case 'H':
	    case 's':  case 'i':  case 'n':
	    case 'I':  case 'J':  case 'K':  case 'L':
	    case 'M':  case 'N':  case 'O':  case 'P':
	    case 'X':
	    case '0': case '1':  case '2':  case '3':  case '4':
	    case '5': case '6':  case '7':  case '8':  case '9':
	      /* These don't say anything we care about.  */
	      break;

	    case '&':
	      amp_p = true;
	      break;
	    case '\0':
	    case ',':
	      if (amp_p && class != NO_REGS)
		{
		  int rc;

		  found = true;
		  for (i = 0;
		       VEC_iterate (int, earlyclobber_regclass, i, rc);
		       i++)
		    {
		      if (rc == (int) class)
			goto found_rc;
		    }

		  /* We use VEC_quick_push here because
		     earlyclobber_regclass holds no more than
		     N_REG_CLASSES elements. */
		  VEC_quick_push (int, earlyclobber_regclass, (int) class);
		found_rc:
		  ;
		}
	      
	      amp_p = false;
	      class = NO_REGS;
	      break;

	    case 'r':
	      class = GENERAL_REGS;
	      break;

	    default:
	      class = REG_CLASS_FROM_CONSTRAINT (c, p);
	      break;
	    }
	  if (c == '\0')
	    break;
	  p += CONSTRAINT_LEN (c, p);
	}
    }

  return found;
}

/* The function checks that pseudo-register *X has a class
   intersecting with the class of pseudo-register could be early
   clobbered in the same insn.

   This function is a no-op if earlyclobber_regclass is empty. 

   Reload can assign the same hard register to uninitialized
   pseudo-register and early clobbered pseudo-register in an insn if
   the pseudo-register is used first time in given BB and not lived at
   the BB start.  To prevent this we don't change life information for
   such pseudo-registers.  */

static int
mark_reg_use_for_earlyclobber (rtx *x, void *data)
{
  enum reg_class pref_class, alt_class;
  int i, regno;
  struct df_bb_info *bb_info = (struct df_bb_info*) data;

  if (REG_P (*x) && REGNO (*x) >= FIRST_PSEUDO_REGISTER)
    {
      int rc;

      regno = REGNO (*x);
      if (bitmap_bit_p (bb_info->ur_kill, regno)
	  || bitmap_bit_p (bb_info->ur_gen, regno))
	return 0;
      pref_class = reg_preferred_class (regno);
      alt_class = reg_alternate_class (regno);
      for (i = 0; VEC_iterate (int, earlyclobber_regclass, i, rc); i++)
	{
	  if (reg_classes_intersect_p (rc, pref_class)
	      || (rc != NO_REGS
		  && reg_classes_intersect_p (rc, alt_class)))
	    {
	      bitmap_set_bit (bb_info->ur_earlyclobber, regno);
	      break;
	    }
	}
    }
  return 0;
}

/* The function processes all pseudo-registers in *X with the aid of
   previous function.  */

static void
mark_reg_use_for_earlyclobber_1 (rtx *x, void *data)
{
  for_each_rtx (x, mark_reg_use_for_earlyclobber, data);
}


/* Compute local uninitialized register info for basic block BB.  */
static void
df_bb_ur_local_compute (struct df *df, basic_block bb)
{
  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);
  rtx insn;

  FOR_BB_INSNS (bb, insn)
    {
      if (INSN_P (insn))
	{
	  note_stores (PATTERN (insn), mark_reg_change, bb_info);
	  if (df->state & (DF_SCAN_GLOBAL | DF_SCAN_POST_ALLOC) 
	      && check_earlyclobber (insn))
	    {
	      df->earlyclobbers_found = true;
	      note_uses (&PATTERN (insn), 
			 mark_reg_use_for_earlyclobber_1, bb_info);
	    }
	}
    }
}


/* Compute local uninitialized register info.  */

static void
df_ur_local_compute (struct df *df, bitmap blocks)
{
  basic_block bb;
#ifdef STACK_REGS
  int i;
  HARD_REG_SET zero, stack_hard_regs, used;
  
  /* Any register that MAY be allocated to a register stack (like the
     387) is treated poorly.  Each such register is marked as being
     live everywhere.  This keeps the register allocator and the
     subsequent passes from doing anything useful with these values.

     FIXME: This seems like an incredibly poor idea.  */

  CLEAR_HARD_REG_SET (zero);
  CLEAR_HARD_REG_SET (stack_hard_regs);
  for (i = FIRST_STACK_REG; i <= LAST_STACK_REG; i++)
    SET_HARD_REG_BIT(stack_hard_regs, i);
  df->stack_regs = BITMAP_ALLOC (NULL);
  for (i = FIRST_PSEUDO_REGISTER; i < max_regno; i++)
    {
      COPY_HARD_REG_SET (used, reg_class_contents[reg_preferred_class (i)]);
      IOR_HARD_REG_SET (used, reg_class_contents[reg_alternate_class (i)]);
      AND_HARD_REG_SET (used, stack_hard_regs);
      GO_IF_HARD_REG_EQUAL(used, zero, skip);
      bitmap_set_bit (df->stack_regs, i);
    skip:
      ;
    }
#endif

  /* We know that earlyclobber_regclass holds no more than
    N_REG_CLASSES elements.  See check_earlyclobber.  */
  earlyclobber_regclass = VEC_alloc (int, heap, N_REG_CLASSES);
  df->earlyclobbers_found = false;

  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
    {
      df_bb_ur_local_compute (df, bb);
    });

  VEC_free (int, heap, earlyclobber_regclass);
}


/* Or in the stack regs, hard regs and early clobber regs into the the
   ur_in sets of all of the blocks.  */

static void
df_ur_local_finalize (struct df *df, bitmap blocks)
{
  bitmap tmp = BITMAP_ALLOC (NULL);
  bitmap_iterator bi;
  unsigned int i;

  EXECUTE_IF_SET_IN_BITMAP (blocks, 0, i, bi)
    {
      basic_block bb = BASIC_BLOCK(i);
      struct df_bb_info* bb_info = DF_BB_INFO (df, bb);

      if (bb != ENTRY_BLOCK_PTR && bb != EXIT_BLOCK_PTR)
	{
	  if (df->earlyclobbers_found)
	    bitmap_ior_into (bb_info->ur_in, bb_info->ur_earlyclobber);
	
#ifdef STACK_REGS
	  /* We can not use the same stack register for uninitialized
	     pseudo-register and another living pseudo-register
	     because if the uninitialized pseudo-register dies,
	     subsequent pass reg-stack will be confused (it will
	     believe that the other register dies).  */
	  bitmap_ior_into (bb_info->ur_in, df->stack_regs);
	  bitmap_ior_into (bb_info->ur_out, df->stack_regs);
#endif
	}

      if (!(df->state & DF_SCAN_GLOBAL))
	{
	  bitmap_ior_into (bb_info->ur_in, all_hard_regs);
	  bitmap_ior_into (bb_info->ur_out, all_hard_regs);
	}

      /* No register may reach a location where it is not used.  Thus
	 we trim the rr result to the places where it is used.  */
      bitmap_and_into (bb_info->ur_in, bb_info->lr_in);
      bitmap_and_into (bb_info->ur_out, bb_info->lr_out);
      
#if 1
      /* Hard registers may still stick in the ur_out set, but not
	 be in the ur_in set, if their only mention was in a call
	 in this block.  This is because a call kills in the lr
	 problem but does not kill in the rr problem.  To clean
	 this up, we execute the transfer function on the lr_in
	 set and then use that to knock bits out of ur_out.  */
      bitmap_ior_and_compl (tmp, bb_info->ur_gen, bb_info->lr_in, 
			    bb_info->ur_kill);
      bitmap_and_into (bb_info->ur_out, tmp);
#endif
    }
  
#ifdef STACK_REGS
  BITMAP_FREE (df->stack_regs);
#endif
  BITMAP_FREE (tmp);
}

/* Compute register info: lifetime, bb, and number of defs and uses
   for basic block BB.  */
static void
df_bb_reg_info_compute (struct df *df, basic_block bb, bitmap live)
{
  struct reg_info *reg_info = df->regs;
  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);
  rtx insn;

  bitmap_copy (live, bb_info->lr_out);

  FOR_BB_INSNS_REVERSE (bb, insn)
    {
      unsigned int uid = INSN_UID (insn);
      unsigned int regno;
      struct df_link *link;
      bitmap_iterator bi;

      if (! INSN_P (insn))
	continue;

      for (link = df->insns[uid].defs; link; link = link->next)
	{
	  struct ref *def = link->ref;
	  unsigned int dregno = DF_REF_REGNO (def);

	  /* Kill this register.  */
	  bitmap_clear_bit (live, dregno);
	  reg_info[dregno].n_defs++;
	}

      for (link = df->insns[uid].uses; link; link = link->next)
	{
	  struct ref *use = link->ref;
	  unsigned int uregno = DF_REF_REGNO (use);

	  /* This register is now live.  */
	  bitmap_set_bit (live, uregno);
	  reg_info[uregno].n_uses++;
	}

      /* Increment lifetimes of all live registers.  */
      EXECUTE_IF_SET_IN_BITMAP (live, 0, regno, bi)
	{
	  reg_info[regno].lifetime++;
	}
    }
}


/* Compute register info: lifetime, bb, and number of defs and uses.  */
static void
df_reg_info_compute (struct df *df, bitmap blocks)
{
  basic_block bb;
  bitmap live;

  live = BITMAP_ALLOC (NULL);

  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
  {
    df_bb_reg_info_compute (df, bb, live);
  });

  BITMAP_FREE (live);
}


/* Assign LUIDs for BB.  */
static int
df_bb_luids_set (struct df *df, basic_block bb)
{
  rtx insn;
  int luid = 0;

  /* The LUIDs are monotonically increasing for each basic block.  */

  FOR_BB_INSNS (bb, insn)
    {
      if (INSN_P (insn))
	DF_INSN_LUID (df, insn) = luid++;
      DF_INSN_LUID (df, insn) = luid;
    }
  return luid;
}


/* Assign LUIDs for each basic block within BLOCKS.  */
static int
df_luids_set (struct df *df, bitmap blocks)
{
  basic_block bb;
  int total = 0;

  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
    {
      total += df_bb_luids_set (df, bb);
    });
  return total;
}

/* Analyze dataflow info for the basic blocks specified by the bitmap
   BLOCKS, or for the whole CFG if BLOCKS is zero.  */

int
df_analyze (struct df *df, bitmap blocks, int flags)
{
  int aflags;
  int dflags;
  struct dataflow dflow;
  int * postorder = xmalloc (sizeof (int) * last_basic_block);
  bitmap scanning;
  bitmap iterating = BITMAP_ALLOC (NULL);
  int n_blocks;
  bool everything;
  int i;

  /* We could deal with additional basic blocks being created by
     rescanning everything again.  */

  if (!blocks)
    {
      if (df->n_bbs)
	{
	  /* Recompute everything from scratch.  */
	  df_free (df);
	}
      /* Allocate and initialize data structures.  */
      df_alloc (df, max_reg_num ());
    }

  gcc_assert (df->n_bbs);
  df_bb_table_realloc (df, last_basic_block);
  df_insn_table_realloc (df, get_max_uid () + 1);

  dflags = 0;
  aflags = flags;
  if (flags & (DF_UD_CHAIN | DF_DU_CHAIN))
    aflags |= DF_RD | DF_RD_CHAIN;

  if (flags & DF_RD)
    aflags |= DF_RD_CHAIN;

  if (flags & DF_RU)
    aflags |= DF_RU_CHAIN;

  if (flags & DF_REG_INFO)
    aflags |= DF_LR;

  if (flags & DF_UR)
    aflags |= DF_LR;

  n_blocks = post_order_compute (postorder, true);
  for (i=0; i<n_blocks; i++)
    bitmap_set_bit (iterating, postorder[i]);

  if (blocks)
    {
      scanning = blocks;
      everything = false;
    }
  else
    {
      blocks = iterating;
      scanning = iterating;
      everything = true;

      /* We can only do dataflow analysis over reachable blocks, so if
	 we find that some are not reachable, we need to get rid of
	 them because the rest of the compiler is not tolerant of
	 having some blocks with no info.  */
      if (n_blocks != n_basic_blocks)
	delete_unreachable_blocks ();
    }

  df->flags = flags;

  /* Scan the function for all register defs and uses.  */
  df_refs_queue (df);
  df_refs_record (df, blocks);
  
  /* Link all the new defs and uses to the insns.  */
  df_refs_process (df);

  /* Allocate the bitmaps now the total number of defs and uses are
     known.  If the number of defs or uses have changed, then
     these bitmaps need to be reallocated.  */
  df_bitmaps_alloc (df, blocks, aflags);

  /* Set the LUIDs for each specified basic block.  */
  df_luids_set (df, blocks);

  /* Recreate reg-def and reg-use chains from scratch so that first
     def is at the head of the reg-def chain and the last use is at
     the head of the reg-use chain.  This is only important for
     regs local to a basic block as it speeds up searching.  */
  if (aflags & DF_RD_CHAIN)
    {
      df_reg_def_chain_create (df, blocks, false, true);
    }

  if (aflags & DF_RU_CHAIN)
    {
      df_reg_use_chain_create (df, blocks, false, true);
    }

  /* Compute the sets of gens and kills for the defs of each bb.  */
  dflow.in = xmalloc (sizeof (bitmap) * last_basic_block);
  dflow.out = xmalloc (sizeof (bitmap) * last_basic_block);
  dflow.gen = xmalloc (sizeof (bitmap) * last_basic_block);
  dflow.kill = xmalloc (sizeof (bitmap) * last_basic_block);

  if (aflags & DF_RD)
    {
      df_rd_local_compute (df, scanning);
      df_rd_set_dflow (&dflow, df, iterating, 
		       iterating, postorder, n_blocks, false);
      iterative_dataflow (&dflow);
    }

  if (aflags & (DF_UD_CHAIN | DF_DU_CHAIN))
    {
      /* Create use-def chains.  */
      df_chain_create (df, aflags, iterating);

      if (! (flags & DF_RD))
	dflags |= DF_RD;
    }

  if (aflags & DF_RU)
    {
      df_ru_local_compute (df, scanning);
      df_ru_set_dflow (&dflow, df, iterating, 
		       iterating, postorder, n_blocks, false);
      iterative_dataflow (&dflow);
    }

  /* Free up bitmaps that are no longer required.  */
  if (dflags)
    df_bitmaps_free (df, dflags);

  if (aflags & DF_LR)
    {
      df_lr_local_compute (df, scanning, everything);
      df_lr_set_dflow (&dflow, df, iterating, 
		       iterating, postorder, n_blocks, false);
      iterative_dataflow (&dflow);
    }

  if (aflags & DF_REG_INFO)
    {
      df_reg_info_compute (df, iterating);
    }

  if (aflags & DF_UR)
    {
      df_ur_local_compute (df, scanning);
      df_ur_set_dflow (&dflow, df, iterating, 
		       iterating, postorder, n_blocks, false);
      iterative_dataflow (&dflow);
    }

  if (aflags & DF_REG_INFO)
    {
      df_reg_info_compute (df, iterating);
    }

  free (dflow.in);
  free (dflow.out);
  free (dflow.gen);
  free (dflow.kill);
  free (postorder);

  BITMAP_FREE (iterating);
  bitmap_zero (df->bbs_modified);
  bitmap_zero (df->insns_modified);
/*   df_dump (df, stderr); */
  return 1;
}

static bool initialized = false;

/* Initialize dataflow analysis.  */
struct df *
df_init (void)
{
  struct df *df;
#ifdef ELIMINABLE_REGS
  int i;
  static const struct {const int from, to; } eliminables[] = ELIMINABLE_REGS;
#endif
  /* After reload, some ports add certain bits to regs_ever_live so
     this cannot be reset.  */
  if (!reload_completed)
    memset (regs_ever_live, 0, sizeof (regs_ever_live));
  if (!initialized)
    {
      bitmap_obstack_initialize (&persistent_obstack);

#ifdef EH_USES
      /* If a target saves one register in another (instead of on
	 the stack) the save register will need to be live for EH.  */
      eh_uses = BITMAP_ALLOC (&persistent_obstack);
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	if (EH_USES (i))
	  bitmap_set_bit (eh_uses, i);
#endif
      /* Record which registers will be eliminated.  We use this in
	 mark_used_regs.  */
      CLEAR_HARD_REG_SET (elim_reg_set);
      
#ifdef ELIMINABLE_REGS
      for (i = 0; i < (int) ARRAY_SIZE (eliminables); i++)
	SET_HARD_REG_BIT (elim_reg_set, eliminables[i].from);
#else
      SET_HARD_REG_BIT (elim_reg_set, FRAME_POINTER_REGNUM);
#endif

      invalidated_by_call = BITMAP_ALLOC (&persistent_obstack);
      
      /* Inconveniently, this is only readily available in hard reg set
	 form.  */
      for (i = 0; i < FIRST_PSEUDO_REGISTER; ++i)
	if (TEST_HARD_REG_BIT (regs_invalidated_by_call, i))
	  bitmap_set_bit (invalidated_by_call, i);

      all_hard_regs = BITMAP_ALLOC (&persistent_obstack);

      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	bitmap_set_bit (all_hard_regs, i);

      initialized = true;
    }

  df = xcalloc (1, sizeof (struct df));
  df->state = DF_SCAN_INITIAL;

  /* Squirrel away a global for debugging.  */
  ddf = df;

  return df;
}


/* Set where we are in the compilation.  */
void 
df_set_state (struct df * df, int state)
{
  df->state = state;
}


/* Start queuing refs.  */
static int
df_refs_queue (struct df *df)
{
  df->def_id_save = df->def_id;
  df->use_id_save = df->use_id;
  /* ???? Perhaps we should save current obstack state so that we can
     unwind it.  */
  return 0;
}


/* Process queued refs.  */
static int
df_refs_process (struct df *df)
{
  unsigned int i;

  /* Build new insn-def chains.  */
  for (i = df->def_id_save; i != df->def_id; i++)
    {
      struct ref *def = df->defs[i];     
      unsigned int uid;
      uid = DF_REF_INSN_UID (def);	  
      
	  /* Add def to head of def list for INSN.  */
      df->insns[uid].defs
	    = df_link_create (df, def, df->insns[uid].defs);
    }

  /* Build new insn-use chains.  */
  for (i = df->use_id_save; i != df->use_id; i++)
    {
      struct ref *use = df->uses[i];
      unsigned int uid;
      if (DF_REF_FLAGS (use) & DF_REF_ARTIFICIAL)
	{
	  struct df_bb_info *bb_info = DF_BB_INFO (df, DF_REF_BB (use));
	  bb_info->artificial_uses = df_link_create (df, use, 
						     bb_info->artificial_uses);
	}
      else
	{     
	  uid = DF_REF_INSN_UID (use);
	  
	  /* Add use to head of use list for INSN.  */
	  df->insns[uid].uses
	    = df_link_create (df, use, df->insns[uid].uses);
	}
    }
  return 0;
}


/* Update refs for basic block BB.  */
static int
df_bb_refs_update (struct df *df, basic_block bb)
{
  rtx insn;
  int count = 0;

  /* While we have to scan the chain of insns for this BB, we do not
     need to allocate and queue a long chain of BB/INSN pairs.  Using
     a bitmap for insns_modified saves memory and avoids queuing
     duplicates.  */

  FOR_BB_INSNS (bb, insn)
    {
      unsigned int uid;

      uid = INSN_UID (insn);

     /*  if (bitmap_bit_p (df->insns_modified, uid)) */
/* 	{ */
	  /* Delete any allocated refs of this insn.  MPH,  FIXME.  */
	  df_insn_refs_unlink (df, bb, insn);

	  /* Scan the insn for refs.  */
	  df_insn_refs_record (df, bb, insn);

	  count++;
/* 	} */
    }
  return count;
}


/* Process all the modified/deleted insns that were queued.  */
static int
df_refs_update (struct df *df, bitmap blocks)
{
  basic_block bb;
  unsigned count = 0;

  df->n_regs = max_reg_num ();
  if (df->n_regs >= df->reg_size)
    df_reg_table_realloc (df, 0);

  df_refs_queue (df);

  if (!blocks)
    {
      FOR_EACH_BB_IN_BITMAP (df->bbs_modified, 0, bb,
	{
	  count += df_bb_refs_update (df, bb);
	});
    }
  else
    {
   
      FOR_EACH_BB_IN_BITMAP (blocks, 0, bb, 
	{
	  count += df_bb_refs_update (df,bb);
	});
    }

  df_refs_process (df);
  return count;
}


/* Remove the entries not in BLOCKS from the LIST of length LEN, preserving
   the order of the remaining entries.  Returns the length of the resulting
   list.  */

static unsigned
prune_to_subcfg (int list[], unsigned len, bitmap blocks)
{
  unsigned act, last;

  for (act = 0, last = 0; act < len; act++)
    if (bitmap_bit_p (blocks, list[act]))
      list[last++] = list[act];

  return last;
}

/* Alternative entry point to the analysis.  Analyze just the part of the cfg
   graph induced by BLOCKS.  BLOCKS cannot be NULL;
   
   TODO I am not quite sure how to avoid code duplication with df_analyze
   here, and simultaneously not make even greater chaos in it.  We behave
   slightly differently in some details, especially in handling modified
   insns.  */

void
df_analyze_subcfg (struct df *df, bitmap blocks, int flags)
{
  rtx insn;
  basic_block bb;
  struct dataflow dflow;
  unsigned n_blocks;
  int * postorder = xmalloc (sizeof (int) * last_basic_block);

  if (flags & (DF_UD_CHAIN | DF_DU_CHAIN))
    flags |= DF_RD | DF_RD_CHAIN;
  if (flags & DF_RU)
    flags |= DF_RU_CHAIN;
  if (flags & DF_RD)
    flags |= DF_RD_CHAIN;
  if (flags & DF_REG_INFO)
    flags |= DF_LR | DF_UR;
  if (flags & DF_UR)
    flags |= DF_LR;

  if (!df->n_bbs)
    {
      df_alloc (df, max_reg_num ());

      /* Mark all insns as modified.  */

      FOR_ALL_BB (bb)
	{
	  FOR_BB_INSNS (bb, insn)
	    {
	      df_insn_modify (df, bb, insn);
	    }
	}
    }
  
  df->flags = flags;

  df_reg_def_chain_clean (df);
  df_reg_use_chain_clean (df);
  df_refs_update (df, blocks);

  /* Clear the updated stuff from ``modified'' bitmaps.  */
  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
    {
      if (bitmap_bit_p (df->bbs_modified, bb->index))
	{
	  FOR_BB_INSNS (bb, insn)
	    {
	      bitmap_clear_bit (df->insns_modified, INSN_UID (insn));
	    }

	  bitmap_clear_bit (df->bbs_modified, bb->index);
	}
    });

  /* Allocate the bitmaps now the total number of defs and uses are
     known.  If the number of defs or uses have changed, then
     these bitmaps need to be reallocated.  */
  df_bitmaps_alloc (df, blocks, flags);

  /* Set the LUIDs for each specified basic block.  */
  df_luids_set (df, blocks);

  /* Recreate reg-def and reg-use chains from scratch so that first
     def is at the head of the reg-def chain and the last use is at
     the head of the reg-use chain.  This is only important for
     regs local to a basic block as it speeds up searching.  */
  if (flags & DF_RD_CHAIN)
    {
      df_reg_def_chain_create (df, blocks, true, true);
    }

  if (flags & DF_RU_CHAIN)
    {
      df_reg_use_chain_create (df, blocks, true, true);
    }

  n_blocks = post_order_compute (postorder, true);
  n_blocks = prune_to_subcfg (postorder, n_blocks, blocks);

  dflow.in = xmalloc (sizeof (bitmap) * last_basic_block);
  dflow.out = xmalloc (sizeof (bitmap) * last_basic_block);
  dflow.gen = xmalloc (sizeof (bitmap) * last_basic_block);
  dflow.kill = xmalloc (sizeof (bitmap) * last_basic_block);

  if (flags & DF_RD)
    {
      /* Compute the sets of gens and kills for the defs of each bb.  */
      df_rd_local_compute (df, blocks);
      df_rd_set_dflow (&dflow, df, blocks, blocks, postorder, n_blocks, false);
      iterative_dataflow (&dflow);
    }

  if (flags & (DF_UD_CHAIN | DF_DU_CHAIN))
    {
      /* Create use-def chains.  */
      df_chain_create (df, flags, blocks);
    }

  if (flags & DF_RU)
    {
      /* Compute the sets of gens and kills for the upwards exposed
	 uses in each bb.  */
      df_ru_local_compute (df, blocks);
      df_ru_set_dflow (&dflow, df, blocks, blocks, postorder, n_blocks, false);
      iterative_dataflow (&dflow);
    }

  if (flags & DF_LR)
    {
      /* Compute the sets of defs and uses of live variables.  */
      df_lr_local_compute (df, blocks, false);
      df_lr_set_dflow (&dflow, df, blocks, blocks, postorder, n_blocks, false);
      iterative_dataflow (&dflow);
    }

  if (flags & DF_REG_INFO)
    {
      df_reg_info_compute (df, blocks);
    }

  if (flags & DF_UR)
    {
      /* Compute the sets of defs and uses of live variables.  */
      df_ur_local_compute (df, blocks);
      df_ur_set_dflow (&dflow, df, blocks, blocks, postorder, n_blocks, false);
      iterative_dataflow (&dflow);
    }

  if (flags & DF_REG_INFO)
    {
      df_reg_info_compute (df, blocks);
    }

  free (dflow.in);
  free (dflow.out);
  free (dflow.gen);
  free (dflow.kill);

  free (postorder);
}

/* This is a very simple and fast incremental interface to df.  It's
   intended usage is for the routine cfg modifications like splitting
   or combining blocks or edges.  For these modifications, general
   iteration is not required, a single pass thru the blocks specified
   reading only the out-sets of the preds and the in-sets of the
   successors is all the work that is accomplished.  No flags are
   accepted, the flags used when the df was last built are used instead.  

   The only input is an post ordered (last block in the sequence is
   first) vector of blocks and the count of these blocks.
*/

void
df_analyze_simple_change_some_blocks (struct df *df, 
				      int * blocks_in_postorder, 
				      unsigned int n_blocks)
{
  struct dataflow dflow;
  bitmap blocks = BITMAP_ALLOC (NULL);
  bitmap blocks_in_fringe = BITMAP_ALLOC (NULL);
  unsigned int i;
  int flags = df->flags;

  df_bb_table_realloc (df, last_basic_block);
  df_insn_table_realloc (df, get_max_uid () + 1);
	
  if (flags & (DF_UD_CHAIN | DF_DU_CHAIN))
    flags |= DF_RD | DF_RD_CHAIN;
  if (flags & DF_RU)
    flags |= DF_RU_CHAIN;
  if (flags & DF_RD)
    flags |= DF_RD_CHAIN;
  if (flags & DF_REG_INFO)
    flags |= DF_LR | DF_UR;
  if (flags & DF_UR)
    flags |= DF_LR;


  /* Blocks_in_fringe is the set of blocks in the postorder + all of
     their preds and succs.  These are the blocks that get their
     bitmaps set up the dflow vectors and that will be either examined
     or modified by the propagation.  */
  for (i=0; i<n_blocks; i++) 
    {
      basic_block bb = BASIC_BLOCK (blocks_in_postorder[i]);
      edge e;
      edge_iterator ei;
      FOR_EACH_EDGE (e, ei, bb->preds)
	bitmap_set_bit (blocks_in_fringe, e->src->index);
      FOR_EACH_EDGE (e, ei, bb->succs)
	bitmap_set_bit (blocks_in_fringe, e->dest->index);
      bitmap_set_bit (blocks, bb->index);
      bitmap_set_bit (blocks_in_fringe, bb->index);
    }
#if 0
  /* Clear the updated stuff from ``modified'' bitmaps.  */
  FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
    {
      if (bitmap_bit_p (df->bbs_modified, bb->index))
	{
	  FOR_BB_INSNS (bb, insn)
	    {
	      bitmap_clear_bit (df->insns_modified, INSN_UID (insn));
	    }

	  bitmap_clear_bit (df->bbs_modified, bb->index);
	}
    });
#endif
  df_refs_update (df, blocks);
  
  /* Allocate the bitmaps now the total number of defs and uses are
     known.  If the number of defs or uses have changed, then
     these bitmaps need to be reallocated.  */
  df_bitmaps_alloc (df, blocks, flags);

  /* Set the LUIDs for each specified basic block.  */
  df_luids_set (df, blocks);

  /* Recreate reg-def and reg-use chains from scratch so that first
     def is at the head of the reg-def chain and the last use is at
     the head of the reg-use chain.  This is only important for
     regs local to a basic block as it speeds up searching.  */
  if (flags & DF_RD_CHAIN)
    df_reg_def_chain_create (df, blocks, true, false);

  if (flags & DF_RU_CHAIN)
    df_reg_use_chain_create (df, blocks, true, false);

  dflow.in = xmalloc (sizeof (bitmap) * last_basic_block);
  dflow.out = xmalloc (sizeof (bitmap) * last_basic_block);
  dflow.gen = xmalloc (sizeof (bitmap) * last_basic_block);
  dflow.kill = xmalloc (sizeof (bitmap) * last_basic_block);

  if (flags & DF_RD)
    {
      /* Compute the sets of gens and kills for the defs of each bb.  */
      df_rd_local_compute (df, blocks);
      df_rd_set_dflow (&dflow, df, blocks_in_fringe, blocks, 
		       blocks_in_postorder, n_blocks, true);
      iterative_dataflow (&dflow);
    }

  if (flags & (DF_UD_CHAIN | DF_DU_CHAIN))
    {
      /* Create use-def chains.  */
      df_chain_create (df, flags, blocks);
    }

  if (flags & DF_RU)
    {
      /* Compute the sets of gens and kills for the upwards exposed
	 uses in each bb.  */
      df_ru_local_compute (df, blocks);
      df_ru_set_dflow (&dflow, df, blocks_in_fringe, blocks, 
		       blocks_in_postorder, n_blocks, true);
      iterative_dataflow (&dflow);
    }

  if (flags & DF_LR)
    {
      /* Compute the sets of defs and uses of live variables.  */
      df_lr_local_compute (df, blocks, false);
      df_lr_set_dflow (&dflow, df, blocks_in_fringe, blocks, 
		       blocks_in_postorder, n_blocks, true);
      iterative_dataflow (&dflow);
    }

  if (flags & DF_REG_INFO)
    {
      df_reg_info_compute (df, blocks);
    }

  if (flags & DF_UR)
    {
      /* Compute the sets of defs and uses of live variables.  */
      df_ur_local_compute (df, blocks);
      df_ur_set_dflow (&dflow, df, blocks_in_fringe, blocks, 
		       blocks_in_postorder, n_blocks, true);
      iterative_dataflow (&dflow);
    }

  if (flags & DF_REG_INFO)
    {
      df_reg_info_compute (df, blocks);
    }

  free (dflow.in);
  free (dflow.out);
  free (dflow.gen);
  free (dflow.kill);
}

/* Convienence call to df_analyze_simple_changes_some_blocks.  */
void
df_analyze_simple_change_one_block (struct df * df, basic_block bb)
{
  int * block_list = alloca (sizeof (int));
  if (df)
    {
      block_list[0] = bb->index;
      df_analyze_simple_change_some_blocks (df, block_list, 1);
    }
}


/* Free all the dataflow info and the DF structure.  */
void
df_finish (struct df *df)
{
  df_free (df);
  free (df);
}

/* Unlink INSN from its reference information.  */
static void
df_insn_refs_unlink (struct df *df, basic_block bb ATTRIBUTE_UNUSED, rtx insn)
{
  struct df_link *link;
  unsigned int uid;

  uid = INSN_UID (insn);

  /* Unlink all refs defined by this insn.  */
  for (link = df->insns[uid].defs; link; link = link->next)
    df_def_unlink (df, link->ref);

  /* Unlink all refs used by this insn.  */
  for (link = df->insns[uid].uses; link; link = link->next)
    df_use_unlink (df, link->ref);

  df->insns[uid].defs = 0;
  df->insns[uid].uses = 0;
}


#if 0
/* Unlink all the insns within BB from their reference information.  */
static void
df_bb_refs_unlink (struct df *df, basic_block bb)
{
  rtx insn;

  /* Scan the block an insn at a time from beginning to end.  */
  for (insn = BB_HEAD (bb); ; insn = NEXT_INSN (insn))
    {
      if (INSN_P (insn))
	{
	  /* Unlink refs for INSN.  */
	  df_insn_refs_unlink (df, bb, insn);
	}
      if (insn == BB_END (bb))
	break;
    }
}


/* Unlink all the refs in the basic blocks specified by BLOCKS.
   Not currently used.  */
static void
df_refs_unlink (struct df *df, bitmap blocks)
{
  basic_block bb;

  if (blocks)
    {
      FOR_EACH_BB_IN_BITMAP (blocks, 0, bb,
      {
	df_bb_refs_unlink (df, bb);
      });
    }
  else
    {
      FOR_ALL_BB (bb)
	df_bb_refs_unlink (df, bb);
    }
}
#endif

/* Functions to modify insns.  */


/* Delete INSN and all its reference information.  */
rtx
df_insn_delete (struct df *df, basic_block bb ATTRIBUTE_UNUSED, rtx insn)
{
  /* If the insn is a jump, we should perhaps call delete_insn to
     handle the JUMP_LABEL?  */

  /* We should not be deleting the NOTE_INSN_BASIC_BLOCK or label.  */
  gcc_assert (insn != BB_HEAD (bb));

  /* Delete the insn.  */
  delete_insn (insn);

  df_insn_modify (df, bb, insn);

  return NEXT_INSN (insn);
}

/* Mark that basic block BB was modified.  */

static void
df_bb_modify (struct df *df, basic_block bb)
{
  if ((unsigned) bb->index >= df->n_bbs)
    df_bb_table_realloc (df, df->n_bbs);

  bitmap_set_bit (df->bbs_modified, bb->index);
}

/* Mark that INSN within BB may have changed  (created/modified/deleted).
   This may be called multiple times for the same insn.  There is no
   harm calling this function if the insn wasn't changed; it will just
   slow down the rescanning of refs.  */
void
df_insn_modify (struct df *df, basic_block bb, rtx insn)
{
  unsigned int uid;

  uid = INSN_UID (insn);
  if (uid >= df->insn_size)
    df_insn_table_realloc (df, uid);

  df_bb_modify (df, bb);
  bitmap_set_bit (df->insns_modified, uid);

  /* For incremental updating on the fly, perhaps we could make a copy
     of all the refs of the original insn and turn them into
     anti-refs.  When df_refs_update finds these anti-refs, it annihilates
     the original refs.  If validate_change fails then these anti-refs
     will just get ignored.  */
}

/* Check if INSN was marked as changed.  Of course the correctness of
   the information depends on whether the instruction was really modified
   at the time df_insn_modify was called.  */
bool
df_insn_modified_p (struct df *df, rtx insn)
{
  unsigned int uid;

  uid = INSN_UID (insn);
  return (df->insns_modified
	  && uid < df->insn_size
          && bitmap_bit_p (df->insns_modified, uid));
}

typedef struct replace_args
{
  rtx match;
  rtx replacement;
  rtx insn;
  int modified;
} replace_args;


/* Replace mem pointed to by PX with its associated pseudo register.
   DATA is actually a pointer to a structure describing the
   instruction currently being scanned and the MEM we are currently
   replacing.  */
static int
df_rtx_mem_replace (rtx *px, void *data)
{
  replace_args *args = (replace_args *) data;
  rtx mem = *px;

  if (mem == NULL_RTX)
    return 0;

  switch (GET_CODE (mem))
    {
    case MEM:
      break;

    case CONST_DOUBLE:
      /* We're not interested in the MEM associated with a
	 CONST_DOUBLE, so there's no need to traverse into one.  */
      return -1;

    default:
      /* This is not a MEM.  */
      return 0;
    }

  if (!rtx_equal_p (args->match, mem))
    /* This is not the MEM we are currently replacing.  */
    return 0;

  /* Actually replace the MEM.  */
  validate_change (args->insn, px, args->replacement, 1);
  args->modified++;

  return 0;
}


int
df_insn_mem_replace (struct df *df, basic_block bb, rtx insn, rtx mem, rtx reg)
{
  replace_args args;

  args.insn = insn;
  args.match = mem;
  args.replacement = reg;
  args.modified = 0;

  /* Search and replace all matching mems within insn.  */
  for_each_rtx (&insn, df_rtx_mem_replace, &args);

  if (args.modified)
    df_insn_modify (df, bb, insn);

  /* ???? FIXME.  We may have a new def or one or more new uses of REG
     in INSN.  REG should be a new pseudo so it won't affect the
     dataflow information that we currently have.  We should add
     the new uses and defs to INSN and then recreate the chains
     when df_analyze is called.  */
  return args.modified;
}


/* Replace one register with another.  Called through for_each_rtx; PX
   points to the rtx being scanned.  DATA is actually a pointer to a
   structure of arguments.  */
static int
df_rtx_reg_replace (rtx *px, void *data)
{
  rtx x = *px;
  replace_args *args = (replace_args *) data;

  if (x == NULL_RTX)
    return 0;

  if (x == args->match)
    {
      validate_change (args->insn, px, args->replacement, 1);
      args->modified++;
    }

  return 0;
}


/* Replace the reg within every ref on CHAIN that is within the set
   BLOCKS of basic blocks with NEWREG.  Also update the regs within
   REG_NOTES.  */
void
df_refs_reg_replace (struct df *df, bitmap blocks, 
		     struct df_link *chain, rtx oldreg, rtx newreg)
{
  struct df_link *link;
  replace_args args;

  args.match = oldreg;
  args.replacement = newreg;
  args.modified = 0;

  for (link = chain; link; link = link->next)
    {
      struct ref *ref = link->ref;
      rtx insn = DF_REF_INSN (ref);

      if (! INSN_P (insn))
	continue;

      if (blocks)
	gcc_assert (bitmap_bit_p (blocks, DF_REF_BBNO (ref)));
      
      df_ref_reg_replace (df, ref, oldreg, newreg);

      /* Replace occurrences of the reg within the REG_NOTES.  */
      if ((! link->next || DF_REF_INSN (ref)
	   != DF_REF_INSN (link->next->ref))
	  && REG_NOTES (insn))
	{
	  args.insn = insn;
	  for_each_rtx (&REG_NOTES (insn), df_rtx_reg_replace, &args);
	}
    }
}


/* Replace all occurrences of register OLDREG with register NEWREG in
   blocks defined by bitmap BLOCKS.  This also replaces occurrences of
   OLDREG in the REG_NOTES but only for insns containing OLDREG.  This
   routine expects the reg-use and reg-def chains to be valid.  */
int
df_reg_replace (struct df *df, bitmap blocks, rtx oldreg, rtx newreg)
{
  unsigned int oldregno = REGNO (oldreg);

  df_refs_reg_replace (df, blocks, df->regs[oldregno].defs, oldreg, newreg);
  df_refs_reg_replace (df, blocks, df->regs[oldregno].uses, oldreg, newreg);
  return 1;
}


/* Try replacing the reg within REF with NEWREG.  Do not modify
   def-use/use-def chains.  */
int
df_ref_reg_replace (struct df *df, struct ref *ref, rtx oldreg, rtx newreg)
{
  /* Check that insn was deleted by being converted into a NOTE.  If
   so ignore this insn.  */
  if (! INSN_P (DF_REF_INSN (ref)))
    return 0;

  gcc_assert (!oldreg || oldreg == DF_REF_REG (ref));

  if (! validate_change (DF_REF_INSN (ref), DF_REF_LOC (ref), newreg, 1))
    return 0;

  df_insn_modify (df, DF_REF_BB (ref), DF_REF_INSN (ref));
  return 1;
}


struct ref*
df_bb_def_use_swap (struct df *df, basic_block bb, rtx def_insn, rtx use_insn, unsigned int regno)
{
  struct ref *def;
  struct ref *use;
  int def_uid;
  int use_uid;
  struct df_link *link;

  def = df_bb_insn_regno_first_def_find (df, bb, def_insn, regno);
  if (! def)
    return 0;

  use = df_bb_insn_regno_last_use_find (df, bb, use_insn, regno);
  if (! use)
    return 0;

  /* The USE no longer exists.  */
  use_uid = INSN_UID (use_insn);
  df_use_unlink (df, use);
  df_ref_unlink (&df->insns[use_uid].uses, use);

  /* The DEF requires shifting so remove it from DEF_INSN
     and add it to USE_INSN by reusing LINK.  */
  def_uid = INSN_UID (def_insn);
  link = df_ref_unlink (&df->insns[def_uid].defs, def);
  link->ref = def;
  link->next = df->insns[use_uid].defs;
  df->insns[use_uid].defs = link;

#if 0
  link = df_ref_unlink (&df->regs[regno].defs, def);
  link->ref = def;
  link->next = df->regs[regno].defs;
  df->insns[regno].defs = link;
#endif

  DF_REF_INSN (def) = use_insn;
  return def;
}


/* Record df between FIRST_INSN and LAST_INSN inclusive.  All new
   insns must be processed by this routine.  */
static void
df_insns_modify (struct df *df, basic_block bb, rtx first_insn, rtx last_insn)
{
  rtx insn;

  for (insn = first_insn; ; insn = NEXT_INSN (insn))
    {
      unsigned int uid;

      /* A non-const call should not have slipped through the net.  If
	 it does, we need to create a new basic block.  Ouch.  The
	 same applies for a label.  */
      gcc_assert ((!CALL_P (insn) || CONST_OR_PURE_CALL_P (insn))
		  && !LABEL_P (insn));

      uid = INSN_UID (insn);

      if (uid >= df->insn_size)
	df_insn_table_realloc (df, uid);

      df_insn_modify (df, bb, insn);

      if (insn == last_insn)
	break;
    }
}


/* Emit PATTERN before INSN within BB.  */
rtx
df_pattern_emit_before (struct df *df, rtx pattern, basic_block bb, rtx insn)
{
  rtx ret_insn;
  rtx prev_insn = PREV_INSN (insn);

  /* We should not be inserting before the start of the block.  */
  gcc_assert (insn != BB_HEAD (bb));
  ret_insn = emit_insn_before (pattern, insn);
  if (ret_insn == insn)
    return ret_insn;

  df_insns_modify (df, bb, NEXT_INSN (prev_insn), ret_insn);
  return ret_insn;
}


/* Emit PATTERN after INSN within BB.  */
rtx
df_pattern_emit_after (struct df *df, rtx pattern, basic_block bb, rtx insn)
{
  rtx ret_insn;

  ret_insn = emit_insn_after (pattern, insn);
  if (ret_insn == insn)
    return ret_insn;

  df_insns_modify (df, bb, NEXT_INSN (insn), ret_insn);
  return ret_insn;
}


/* Emit jump PATTERN after INSN within BB.  */
rtx
df_jump_pattern_emit_after (struct df *df, rtx pattern, basic_block bb, rtx insn)
{
  rtx ret_insn;

  ret_insn = emit_jump_insn_after (pattern, insn);
  if (ret_insn == insn)
    return ret_insn;

  df_insns_modify (df, bb, NEXT_INSN (insn), ret_insn);
  return ret_insn;
}


/* Move INSN within BB before BEFORE_INSN within BEFORE_BB.

   This function should only be used to move loop invariant insns
   out of a loop where it has been proven that the def-use info
   will still be valid.  */
rtx
df_insn_move_before (struct df *df, basic_block bb, rtx insn, 
		     basic_block before_bb, rtx before_insn)
{
  struct df_link *link;
  unsigned int uid;

  if (! bb)
    return df_pattern_emit_before (df, insn, before_bb, before_insn);

  uid = INSN_UID (insn);

  /* Change bb for all df defined and used by this insn.  */
  for (link = df->insns[uid].defs; link; link = link->next)
    DF_REF_BB (link->ref) = before_bb;
  for (link = df->insns[uid].uses; link; link = link->next)
    DF_REF_BB (link->ref) = before_bb;

  /* The lifetimes of the registers used in this insn will be reduced
     while the lifetimes of the registers defined in this insn
     are likely to be increased.  */

  /* ???? Perhaps all the insns moved should be stored on a list
     which df_analyze removes when it recalculates data flow.  */

  return emit_insn_before (insn, before_insn);
}

/* Functions to query dataflow information.  */


int
df_insn_regno_def_p (struct df *df, basic_block bb ATTRIBUTE_UNUSED,
		     rtx insn, unsigned int regno)
{
  unsigned int uid;
  struct df_link *link;

  uid = INSN_UID (insn);

  for (link = df->insns[uid].defs; link; link = link->next)
    {
      struct ref *def = link->ref;

      if (DF_REF_REGNO (def) == regno)
	return 1;
    }

  return 0;
}

/* Finds the reference corresponding to the definition of REG in INSN.
   DF is the dataflow object.  */

struct ref *
df_find_def (struct df *df, rtx insn, rtx reg)
{
  struct df_link *defs;

  for (defs = DF_INSN_DEFS (df, insn); defs; defs = defs->next)
    if (rtx_equal_p (DF_REF_REG (defs->ref), reg))
      return defs->ref;

  return NULL;
}

/* Return 1 if REG is referenced in INSN, zero otherwise.  */ 

int
df_reg_used (struct df *df, rtx insn, rtx reg)
{
  struct df_link *uses;

  for (uses = DF_INSN_USES (df, insn); uses; uses = uses->next)
    if (rtx_equal_p (DF_REF_REG (uses->ref), reg))
      return 1; 

  return 0;
}

static int
df_def_dominates_all_uses_p (struct df *df ATTRIBUTE_UNUSED, struct ref *def)
{
  struct df_link *du_link;

  /* Follow def-use chain to find all the uses of this def.  */
  for (du_link = DF_REF_CHAIN (def); du_link; du_link = du_link->next)
    {
      struct ref *use = du_link->ref;
      struct df_link *ud_link;

      /* Follow use-def chain to check all the defs for this use.  */
      for (ud_link = DF_REF_CHAIN (use); ud_link; ud_link = ud_link->next)
	if (ud_link->ref != def)
	  return 0;
    }
  return 1;
}


int
df_insn_dominates_all_uses_p (struct df *df, basic_block bb ATTRIBUTE_UNUSED,
			      rtx insn)
{
  unsigned int uid;
  struct df_link *link;

  uid = INSN_UID (insn);

  for (link = df->insns[uid].defs; link; link = link->next)
    {
      struct ref *def = link->ref;

      if (! df_def_dominates_all_uses_p (df, def))
	return 0;
    }

  return 1;
}


/* Return nonzero if all DF dominates all the uses within the bitmap
   BLOCKS.  */
static int
df_def_dominates_uses_p (struct df *df ATTRIBUTE_UNUSED, struct ref *def,
			 bitmap blocks)
{
  struct df_link *du_link;

  /* Follow def-use chain to find all the uses of this def.  */
  for (du_link = DF_REF_CHAIN (def); du_link; du_link = du_link->next)
    {
      struct ref *use = du_link->ref;
      struct df_link *ud_link;

      /* Only worry about the uses within BLOCKS.  For example,
      consider a register defined within a loop that is live at the
      loop exits.  */
      if (bitmap_bit_p (blocks, DF_REF_BBNO (use)))
	{
	  /* Follow use-def chain to check all the defs for this use.  */
	  for (ud_link = DF_REF_CHAIN (use); ud_link; ud_link = ud_link->next)
	    if (ud_link->ref != def)
	      return 0;
	}
    }
  return 1;
}


/* Return nonzero if all the defs of INSN within BB dominates
   all the corresponding uses.  */
int
df_insn_dominates_uses_p (struct df *df, basic_block bb ATTRIBUTE_UNUSED,
			  rtx insn, bitmap blocks)
{
  unsigned int uid;
  struct df_link *link;

  uid = INSN_UID (insn);

  for (link = df->insns[uid].defs; link; link = link->next)
    {
      struct ref *def = link->ref;

      /* Only consider the defs within BLOCKS.  */
      if (bitmap_bit_p (blocks, DF_REF_BBNO (def))
	  && ! df_def_dominates_uses_p (df, def, blocks))
	return 0;
    }
  return 1;
}


/* Return the basic block that REG referenced in or NULL if referenced
   in multiple basic blocks.  */
basic_block
df_regno_bb (struct df *df, unsigned int regno)
{
  struct df_link *defs = df->regs[regno].defs;
  struct df_link *uses = df->regs[regno].uses;
  struct ref *def = defs ? defs->ref : 0;
  struct ref *use = uses ? uses->ref : 0;
  basic_block bb_def = def ? DF_REF_BB (def) : 0;
  basic_block bb_use = use ? DF_REF_BB (use) : 0;

  /* Compare blocks of first def and last use.  ???? FIXME.  What if
     the reg-def and reg-use lists are not correctly ordered.  */
  return bb_def == bb_use ? bb_def : 0;
}


/* Return nonzero if REG used in multiple basic blocks.  */
int
df_reg_global_p (struct df *df, rtx reg)
{
  return df_regno_bb (df, REGNO (reg)) != 0;
}


/* Return total lifetime (in insns) of REG.  */
int
df_reg_lifetime (struct df *df, rtx reg)
{
  return df->regs[REGNO (reg)].lifetime;
}


/* Return REG live at start of BB.  */
bitmap
df_bb_reg_live_start (struct df *df, basic_block bb)
{
  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);

  gcc_assert (bb_info->ur_in);

  return bb_info->ur_in;
}


/* Return REG live at end of BB.  */
bitmap
df_bb_reg_live_end (struct df *df, basic_block bb)
{
  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);

  gcc_assert (bb_info->ur_out);

  return bb_info->ur_out;
}

/* Return nonzero if REG live at start of BB.  */
int
df_bb_reg_live_start_p (struct df *df, basic_block bb, rtx reg)
{
  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);

  gcc_assert (bb_info->ur_in);

  return bitmap_bit_p (bb_info->ur_in, REGNO (reg));
}


/* Return nonzero if REG live at end of BB.  */
int
df_bb_reg_live_end_p (struct df *df, basic_block bb, rtx reg)
{
  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);

  gcc_assert (bb_info->ur_in);

  return bitmap_bit_p (bb_info->ur_out, REGNO (reg));
}


/* Return -1 if life of REG1 before life of REG2, 1 if life of REG1
   after life of REG2, or 0, if the lives overlap.  */
int
df_bb_regs_lives_compare (struct df *df, basic_block bb, rtx reg1, rtx reg2)
{
  unsigned int regno1 = REGNO (reg1);
  unsigned int regno2 = REGNO (reg2);
  struct ref *def1;
  struct ref *use1;
  struct ref *def2;
  struct ref *use2;


  /* The regs must be local to BB.  */
  gcc_assert (df_regno_bb (df, regno1) == bb
	      && df_regno_bb (df, regno2) == bb);

  def2 = df_bb_regno_first_def_find (df, bb, regno2);
  use1 = df_bb_regno_last_use_find (df, bb, regno1);

  if (DF_INSN_LUID (df, DF_REF_INSN (def2))
      > DF_INSN_LUID (df, DF_REF_INSN (use1)))
    return -1;

  def1 = df_bb_regno_first_def_find (df, bb, regno1);
  use2 = df_bb_regno_last_use_find (df, bb, regno2);

  if (DF_INSN_LUID (df, DF_REF_INSN (def1))
      > DF_INSN_LUID (df, DF_REF_INSN (use2)))
    return 1;

  return 0;
}


/* Return true if the definition DEF, which is in the same basic
   block as INSN, is available at INSN.  So DEF may as well be
   dead, in which case using it will extend its live range.  */
bool
df_local_def_available_p (struct df *df, struct ref *def, rtx insn)
{
  struct df_link *link;
  int def_luid = DF_INSN_LUID (df, DF_REF_INSN (def));
  int in_bb = 0;
  unsigned int regno = REGNO (def->reg);
  basic_block bb = DF_REF_BB (def);

  /* The regs must be local to BB.  */
  gcc_assert (bb == BLOCK_FOR_INSN (insn));

  /* This assumes that the reg-def list is ordered such that for any
     BB, the first def is found first.  However, since the BBs are not
     ordered, the first def in the chain is not necessarily the first
     def in the function.  */
  for (link = df->regs[regno].defs; link; link = link->next)
    {
      struct ref *this_def = link->ref;
      if (DF_REF_BB (this_def) == bb)
	{
	  int this_luid = DF_INSN_LUID (df, DF_REF_INSN (this_def));
	  /* Do nothing with defs coming before DEF.  */
	  if (this_luid > def_luid)
	    return this_luid > DF_INSN_LUID (df, insn);

	  in_bb = 1;
        }
      else if (in_bb)
	/* DEF was the last in its basic block.  */
        return 1;
    }

  /* DEF was the last in the function.  */
  return 1;
}

/* Insert DEF into DF.  USES is the def-use chain for DEF, which is needed
   to set up correctly both the def-use chain (obviously) *and* the use-def
   chain; it can be NULL if you do not need either.

   No nodes in USES should be shared with other chains.  Use df_chain_copy
   to ensure this.  */
void
df_insert_def (struct df *df, struct ref *def, struct df_link *uses)
{
  rtx insn = DF_REF_INSN (def);

  if (df->flags & DF_RD_CHAIN)
    {
      struct df_link *link = df_link_create (df, def, NULL);
      struct df_link **p;
      basic_block bb = BLOCK_FOR_INSN (insn);
      int new_luid = DF_INSN_LUID (df, insn);
      int regno = DF_REF_REGNO (def);

      for (p = &df->regs[regno].defs; *p; p = &(*p)->next)
        if (DF_REF_BB ((*p)->ref) == bb)
          break;

      /* This assumes that the reg-def list is ordered such that for any
         BB, the first use is found first.  However, since the BBs are not
         ordered, the first use in the chain is not necessarily the last
         use in the function.  */
      while (*p && DF_INSN_LUID (df, (*p)->ref->insn) < new_luid)
        p = &(*p)->next;

      link->next = *p;
      *p = link;
    }

  /* Also add to the per-insn def list.  */
  DF_INSN_DEFS (df, insn) = df_link_create (df, def, DF_INSN_DEFS (df, insn));
  
  if (df->flags & DF_DU_CHAIN)
    DF_REF_CHAIN (def) = uses;
         
  if (df->flags & DF_UD_CHAIN)
    {
      struct df_link *link;
      gcc_assert (uses);
      for (link = uses; link; link = link->next)
        DF_REF_CHAIN (link->ref) =
	   df_link_create (df, def, DF_REF_CHAIN (link->ref));
    }
}


/* Insert USE into DF.  DEFS is the use-def chain for USE, which is needed
   to set up correctly both the use-def chain (obviously) *and* the def-use
   chain; it can be NULL if you do not need either.

   No nodes in DEFS should be shared with other chains.  Use df_chain_copy
   to ensure this.  */
void
df_insert_use (struct df *df, struct ref *use, struct df_link *defs)
{
  rtx insn = DF_REF_INSN (use);

  if (df->flags & DF_RU_CHAIN)
    {
      struct df_link *link = df_link_create (df, use, NULL);
      struct df_link **p;
      basic_block bb = BLOCK_FOR_INSN (insn);
      int new_luid = DF_INSN_LUID (df, insn);
      int regno = DF_REF_REGNO (use);

      for (p = &df->regs[regno].uses; *p; p = &(*p)->next)
        if (DF_REF_BB ((*p)->ref) == bb)
          break;

      /* This assumes that the reg-use list is ordered such that for any
         BB, the last use is found first.  However, since the BBs are not
         ordered, the first use in the chain is not necessarily the last
         use in the function.  */
      while (*p && DF_INSN_LUID (df, (*p)->ref->insn) > new_luid)
        p = &(*p)->next;

      link->next = *p;
      *p = link;
    }

  /* Also add to the per-insn use list.  */
  DF_INSN_USES (df, insn) = df_link_create (df, use, DF_INSN_USES (df, insn));

  if (df->flags & DF_UD_CHAIN)
    DF_REF_CHAIN (use) = defs;

  if (df->flags & DF_DU_CHAIN)
    {
      struct df_link *link;
      gcc_assert (defs);
      for (link = defs; link; link = link->next)
        DF_REF_CHAIN (link->ref) =
	   df_link_create (df, use, DF_REF_CHAIN (link->ref));
    }
}


/* Return last use of REGNO within BB.  */
struct ref *
df_bb_regno_last_use_find (struct df *df, basic_block bb, unsigned int regno)
{
  struct df_link *link;

  /* This assumes that the reg-use list is ordered such that for any
     BB, the last use is found first.  However, since the BBs are not
     ordered, the first use in the chain is not necessarily the last
     use in the function.  */
  for (link = df->regs[regno].uses; link; link = link->next)
    {
      struct ref *use = link->ref;

      if (DF_REF_BB (use) == bb)
	return use;
    }
  return 0;
}


/* Return first def of REGNO within BB.  */
struct ref *
df_bb_regno_first_def_find (struct df *df, basic_block bb, unsigned int regno)
{
  struct df_link *link;

  /* This assumes that the reg-def list is ordered such that for any
     BB, the first def is found first.  However, since the BBs are not
     ordered, the first def in the chain is not necessarily the first
     def in the function.  */
  for (link = df->regs[regno].defs; link; link = link->next)
    {
      struct ref *def = link->ref;

      if (DF_REF_BB (def) == bb)
	return def;
    }
  return 0;
}

/* Return last def of REGNO within BB.  */
struct ref *
df_bb_regno_last_def_find (struct df *df, basic_block bb, unsigned int regno)
{
  struct df_link *link;
  struct ref *last_def = NULL;
  int in_bb = 0;

  /* This assumes that the reg-def list is ordered such that for any
     BB, the first def is found first.  However, since the BBs are not
     ordered, the first def in the chain is not necessarily the first
     def in the function.  */
  for (link = df->regs[regno].defs; link; link = link->next)
    {
      struct ref *def = link->ref;
      /* The first time in the desired block.  */ 
      if (DF_REF_BB (def) == bb)
	  in_bb = 1;
      /* The last def in the desired block.  */
      else if (in_bb)
        return last_def;
      last_def = def;
    }
  return last_def;
}

/* Return last use of REGNO inside INSN within BB.  */
static struct ref *
df_bb_insn_regno_last_use_find (struct df *df,
				basic_block bb ATTRIBUTE_UNUSED, rtx insn,
				unsigned int regno)
{
  unsigned int uid;
  struct df_link *link;

  uid = INSN_UID (insn);

  for (link = df->insns[uid].uses; link; link = link->next)
    {
      struct ref *use = link->ref;

      if (DF_REF_REGNO (use) == regno)
	return use;
    }

  return 0;
}


/* Return first def of REGNO inside INSN within BB.  */
static struct ref *
df_bb_insn_regno_first_def_find (struct df *df,
				 basic_block bb ATTRIBUTE_UNUSED, rtx insn,
				 unsigned int regno)
{
  unsigned int uid;
  struct df_link *link;

  uid = INSN_UID (insn);

  for (link = df->insns[uid].defs; link; link = link->next)
    {
      struct ref *def = link->ref;

      if (DF_REF_REGNO (def) == regno)
	return def;
    }

  return 0;
}


/* Return insn using REG if the BB contains only a single
   use and def of REG.  */
rtx
df_bb_single_def_use_insn_find (struct df *df, basic_block bb, rtx insn, rtx reg)
{
  struct ref *def;
  struct ref *use;
  struct df_link *du_link;

  def = df_bb_insn_regno_first_def_find (df, bb, insn, REGNO (reg));

  gcc_assert (def);

  du_link = DF_REF_CHAIN (def);

  if (! du_link)
    return NULL_RTX;

  use = du_link->ref;

  /* Check if def is dead.  */
  if (! use)
    return NULL_RTX;

  /* Check for multiple uses.  */
  if (du_link->next)
    return NULL_RTX;

  return DF_REF_INSN (use);
}

/* Functions for debugging/dumping dataflow information.  */


/* Dump a def-use or use-def chain for REF to FILE.  */
static void
df_chain_dump (struct df_link *link, FILE *file)
{
  fprintf (file, "{ ");
  for (; link; link = link->next)
    {
      fprintf (file, "%c%d(bb %d insn %d) ",
	       DF_REF_REG_DEF_P (link->ref) ? 'd' : 'u',
	       DF_REF_ID (link->ref),
	       DF_REF_BBNO (link->ref),
	       DF_REF_INSN (link->ref) ? DF_REF_INSN_UID (link->ref) : -1);
    }
  fprintf (file, "}");
}


/* Dump a chain of refs with the associated regno.  */
static void
df_chain_dump_regno (struct df_link *link, FILE *file)
{
  fprintf (file, "{ ");
  for (; link; link = link->next)
    {
      fprintf (file, "%c%d(%d) ",
	       DF_REF_REG_DEF_P (link->ref) ? 'd' : 'u',
	       DF_REF_ID (link->ref),
	       DF_REF_REGNO (link->ref));
    }
  fprintf (file, "}");
}

static void 
df_print_bb_index (basic_block bb, FILE *file)
{
  edge e;
  edge_iterator ei;
  fprintf (file, "( ");
    FOR_EACH_EDGE (e, ei, bb->preds)
    {
      basic_block pred = e->src;
      fprintf (file, "%d ", pred->index);
    } 
  fprintf (file, ")->[%d]->( ", bb->index);
  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      basic_block succ = e->dest;
      fprintf (file, "%d ", succ->index);
    } 
  fprintf (file, ")\n");
}

/* Dump dataflow info.  */
void
df_dump (struct df *df, FILE *file)
{
  unsigned int j;
  basic_block bb;
  int flags = df->flags;

  if (! df || ! file)
    return;

  fprintf (file, "\n\n%s\n", current_function_name());
  fprintf (file, "\nDataflow summary:\n");
  fprintf (file, "n_regs = %d, n_defs = %d, n_uses = %d, n_bbs = %d\n",
	   df->n_regs, df->n_defs, df->n_uses, df->n_bbs);

  /*
  if (flags & DF_RD)
    {
      basic_block bb;

      fprintf (file, "Reaching defs:\n");
      FOR_ALL_BB (bb)
	{
	  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);
	  df_print_bb_index (bb, file);

	  if (! bb_info->rd_in)
	    continue;

	  fprintf (file, "  in  \t");
	  dump_bitmap (file, bb_info->rd_in);
	  fprintf (file, "  gen \t");
	  dump_bitmap (file, bb_info->rd_gen);
	  fprintf (file, "  kill\t");
	  dump_bitmap (file, bb_info->rd_kill);
	  fprintf (file, "  out \t");
	  dump_bitmap (file, bb_info->rd_out);
	}
    }

  */
  if (flags & DF_DU_CHAIN)
    {
      fprintf (file, "Def-use chains:\n");
      for (j = 0; j < df->n_defs; j++)
	{
	  if (df->defs[j])
	    {
	      fprintf (file, "d%d bb %d luid %d insn %d reg %d ",
		       j, DF_REF_BBNO (df->defs[j]),
		       DF_INSN_LUID (df, DF_REF_INSN (df->defs[j])),
		       DF_REF_INSN (df->defs[j]) ? DF_REF_INSN_UID (df->defs[j]) : -1,
		       DF_REF_REGNO (df->defs[j]));
	      if (df->defs[j]->flags & DF_REF_READ_WRITE)
		fprintf (file, "read/write ");
	      df_chain_dump (DF_REF_CHAIN (df->defs[j]), file);
	      fprintf (file, "\n");
	    }
	}
    }

  if (flags & DF_RU)
    {
      fprintf (file, "Reaching uses:\n");
      FOR_ALL_BB (bb)
	{
	  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);
	  df_print_bb_index (bb, file);

	  if (! bb_info->ru_in)
	    continue;

	  fprintf (file, "  in  \t");
	  dump_bitmap (file, bb_info->ru_in);
	  fprintf (file, "  gen \t");
	  dump_bitmap (file, bb_info->ru_gen);
	  fprintf (file, "  kill\t");
	  dump_bitmap (file, bb_info->ru_kill);
	  fprintf (file, "  out \t");
	  dump_bitmap (file, bb_info->ru_out);
	}
    }

  if (flags & DF_UD_CHAIN)
    {
      fprintf (file, "Use-def chains:\n");
      for (j = 0; j < df->n_uses; j++)
	{
	  if (df->uses[j])
	    {
	      fprintf (file, "u%d bb %d luid %d insn %d reg %d ",
		       j, DF_REF_BBNO (df->uses[j]),
		       DF_REF_INSN (df->uses[j]) ? 
		       DF_INSN_LUID (df, DF_REF_INSN (df->uses[j]))
		       : -1,
		       DF_REF_INSN (df->uses[j]) ?
		       DF_REF_INSN_UID (df->uses[j])
		       : -1,
		       DF_REF_REGNO (df->uses[j]));
	      if (df->uses[j]->flags & DF_REF_READ_WRITE)
		fprintf (file, "read/write ");
	      if (df->uses[j]->flags & DF_REF_STRIPPED)
		fprintf (file, "stripped ");
	      if (df->uses[j]->flags & DF_REF_IN_NOTE)
		fprintf (file, "note ");
	      df_chain_dump (DF_REF_CHAIN (df->uses[j]), file);
	      fprintf (file, "\n");
	    }
	}
    }

  if (flags & DF_LR)
    {
      fprintf (file, "Live regs:\n");
      FOR_ALL_BB (bb)
	{
	  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);
	  df_print_bb_index (bb, file);

	  if (! bb_info->lr_in)
	    continue;

	  fprintf (file, "  in  \t");
	  dump_bitmap (file, bb_info->lr_in);
	  fprintf (file, "  use \t");
	  dump_bitmap (file, bb_info->lr_use);
	  fprintf (file, "  def \t");
	  dump_bitmap (file, bb_info->lr_def);
	  fprintf (file, "  out \t");
	  dump_bitmap (file, bb_info->lr_out);
	}
    }

  if (flags & DF_UR)
    {
      fprintf (file, "Undefined regs:\n");

      fprintf (file, "  all hard regs \t");
      dump_bitmap (file, all_hard_regs);
#ifdef EH_USES
      fprintf (file, "  eh uses \t");
      dump_bitmap (file, eh_uses);
#endif
      fprintf (file, "  invalidated by call \t");
      dump_bitmap (file, invalidated_by_call);
      fprintf (file, "  hardware regs used \t");
      dump_bitmap (file, df->hardware_regs_used);
      fprintf (file, "  exit block uses \t");
      dump_bitmap (file, df->exit_block_uses);
      FOR_ALL_BB (bb)
	{
	  struct df_bb_info *bb_info = DF_BB_INFO (df, bb);
	  df_print_bb_index (bb, file);

	  if (! bb_info->ur_in)
	    continue;

	  fprintf (file, "  in  \t");
	  dump_bitmap (file, bb_info->ur_in);
	  fprintf (file, "  gen \t");
	  dump_bitmap (file, bb_info->ur_gen);
	  fprintf (file, "  kill \t");
	  dump_bitmap (file, bb_info->ur_kill);
	  fprintf (file, "  out \t");
	  dump_bitmap (file, bb_info->ur_out);
	}
    }

  if (flags & (DF_REG_INFO | DF_RD_CHAIN | DF_RU_CHAIN))
    {
      struct reg_info *reg_info = df->regs;

      fprintf (file, "Register info:\n");
      for (j = 0; j < df->n_regs; j++)
	{
	  if (((flags & DF_REG_INFO)
	       && (reg_info[j].n_uses || reg_info[j].n_defs))
	      || ((flags & DF_RD_CHAIN) && reg_info[j].defs)
	      || ((flags & DF_RU_CHAIN) && reg_info[j].uses))
	    {
	      fprintf (file, "reg %d", j);
	      if ((flags & DF_RD_CHAIN) && (flags & DF_RU_CHAIN))
		{
		  basic_block bb = df_regno_bb (df, j);

		  if (bb)
		    fprintf (file, " bb %d", bb->index);
		  else
		    fprintf (file, " bb ?");
		}
	      if (flags & DF_REG_INFO)
		{
		  fprintf (file, " life %d", reg_info[j].lifetime);
		}

	      if ((flags & DF_REG_INFO) || (flags & DF_RD_CHAIN))
		{
		  fprintf (file, " defs ");
		  if (flags & DF_REG_INFO)
		    fprintf (file, "%d ", reg_info[j].n_defs);
		  if (flags & DF_RD_CHAIN)
		    df_chain_dump (reg_info[j].defs, file);
		}

	      if ((flags & DF_REG_INFO) || (flags & DF_RU_CHAIN))
		{
		  fprintf (file, " uses ");
		  if (flags & DF_REG_INFO)
		    fprintf (file, "%d ", reg_info[j].n_uses);
		  if (flags & DF_RU_CHAIN)
		    df_chain_dump (reg_info[j].uses, file);
		}

	      fprintf (file, "\n");
	    }
	}
    }
  fprintf (file, "\n");
}


void
df_insn_debug (struct df *df, rtx insn, FILE *file)
{
  unsigned int uid;
  int bbi;

  uid = INSN_UID (insn);
  if (uid >= df->insn_size)
    return;

  if (df->insns[uid].defs)
    bbi = DF_REF_BBNO (df->insns[uid].defs->ref);
  else if (df->insns[uid].uses)
    bbi = DF_REF_BBNO (df->insns[uid].uses->ref);
  else
    bbi = -1;

  fprintf (file, "insn %d bb %d luid %d defs ",
	   uid, bbi, DF_INSN_LUID (df, insn));
  df_chain_dump (df->insns[uid].defs, file);
  fprintf (file, " uses ");
  df_chain_dump (df->insns[uid].uses, file);
  fprintf (file, "\n");
}


void
df_insn_debug_regno (struct df *df, rtx insn, FILE *file)
{
  unsigned int uid;
  int bbi;

  uid = INSN_UID (insn);
  if (uid >= df->insn_size)
    return;

  if (df->insns[uid].defs)
    bbi = DF_REF_BBNO (df->insns[uid].defs->ref);
  else if (df->insns[uid].uses)
    bbi = DF_REF_BBNO (df->insns[uid].uses->ref);
  else
    bbi = -1;

  fprintf (file, "insn %d bb %d luid %d defs ",
	   uid, bbi, DF_INSN_LUID (df, insn));
  df_chain_dump_regno (df->insns[uid].defs, file);
  fprintf (file, " uses ");
  df_chain_dump_regno (df->insns[uid].uses, file);
  fprintf (file, "\n");
}


static void
df_regno_debug (struct df *df, unsigned int regno, FILE *file)
{
  if (regno >= df->reg_size)
    return;

  fprintf (file, "reg %d life %d defs ",
	   regno, df->regs[regno].lifetime);
  df_chain_dump (df->regs[regno].defs, file);
  fprintf (file, " uses ");
  df_chain_dump (df->regs[regno].uses, file);
  fprintf (file, "\n");
}


static void
df_ref_debug (struct df *df, struct ref *ref, FILE *file)
{
  fprintf (file, "%c%d ",
	   DF_REF_REG_DEF_P (ref) ? 'd' : 'u',
	   DF_REF_ID (ref));
  fprintf (file, "reg %d bb %d luid %d insn %d chain ",
	   DF_REF_REGNO (ref),
	   DF_REF_BBNO (ref),
	   DF_REF_INSN (ref) ? DF_INSN_LUID (df, DF_REF_INSN (ref)) : -1,
	   DF_REF_INSN (ref) ? INSN_UID (DF_REF_INSN (ref)) : -1);
  df_chain_dump (DF_REF_CHAIN (ref), file);
  fprintf (file, "\n");
}

/* Functions for debugging from GDB.  */

void
debug_df_insn (rtx insn)
{
  df_insn_debug (ddf, insn, stderr);
  debug_rtx (insn);
}


void
debug_df_reg (rtx reg)
{
  df_regno_debug (ddf, REGNO (reg), stderr);
}


void
debug_df_regno (unsigned int regno)
{
  df_regno_debug (ddf, regno, stderr);
}


void
debug_df_ref (struct ref *ref)
{
  df_ref_debug (ddf, ref, stderr);
}


void
debug_df_defno (unsigned int defno)
{
  df_ref_debug (ddf, ddf->defs[defno], stderr);
}


void
debug_df_useno (unsigned int defno)
{
  df_ref_debug (ddf, ddf->uses[defno], stderr);
}


void
debug_df_chain (struct df_link *link)
{
  df_chain_dump (link, stderr);
  fputc ('\n', stderr);
}


/* Hybrid search algorithm from "Implementation Techniques for
   Efficient Data-Flow Analysis of Large Programs".  */

static void
hybrid_search_forward (basic_block bb, struct dataflow *dataflow)
{
  int result_changed;
  int i = bb->index;
  edge e;
  edge_iterator ei;

  SET_BIT (dataflow->visited, bb->index);
  gcc_assert (TEST_BIT (dataflow->pending, bb->index));
  RESET_BIT (dataflow->pending, i);

  /*  Calculate <conf_op> of predecessor_outs.  */
  if (EDGE_COUNT (bb->preds) > 0)
    FOR_EACH_EDGE (e, ei, bb->preds)
      {
	if (!TEST_BIT (dataflow->considered, e->src->index))
	  continue;
	
	(*dataflow->problem->confun_n)(dataflow, e);
      }
  else if (*dataflow->problem->confun_0)
    (*dataflow->problem->confun_0)(dataflow, bb);
  
  result_changed = (*dataflow->problem->transfun)(dataflow, i);
  
  if (!result_changed || dataflow->single_pass)
    return;
  
  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      if (e->dest->index == i)
	continue;
      if (!TEST_BIT (dataflow->considered, e->dest->index))
	continue;
      SET_BIT (dataflow->pending, e->dest->index);
    }
  
  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      if (e->dest->index == i)
	continue;
      
      if (!TEST_BIT (dataflow->considered, e->dest->index))
	continue;
      if (!TEST_BIT (dataflow->visited, e->dest->index))
	hybrid_search_forward (e->dest, dataflow);
    }
}

static void
hybrid_search_backward (basic_block bb, struct dataflow *dataflow)
{
  int result_changed;
  int i = bb->index;
  edge e;
  edge_iterator ei;
  
  SET_BIT (dataflow->visited, bb->index);
  gcc_assert (TEST_BIT (dataflow->pending, bb->index));
  RESET_BIT (dataflow->pending, i);

  /*  Calculate <conf_op> of predecessor_outs.  */
  if (EDGE_COUNT (bb->succs) > 0)
    FOR_EACH_EDGE (e, ei, bb->succs)					
      {								
	if (!TEST_BIT (dataflow->considered, e->dest->index))		
	  continue;							
	
	(*dataflow->problem->confun_n)(dataflow, e);
      }								
  else if (*dataflow->problem->confun_0)
    (*dataflow->problem->confun_0)(dataflow, bb);

  result_changed = (*dataflow->problem->transfun)(dataflow, i);
  
  if (!result_changed || dataflow->single_pass)
    return;
  
  FOR_EACH_EDGE (e, ei, bb->preds)
    {								
      if (e->src->index == i)
	continue;
      
      if (!TEST_BIT (dataflow->considered, e->src->index))
	continue;

      SET_BIT (dataflow->pending, e->src->index);
    }								
  
  FOR_EACH_EDGE (e, ei, bb->preds)
    {
      if (e->src->index == i)
	continue;

      if (!TEST_BIT (dataflow->considered, e->src->index))
	continue;
      
      if (!TEST_BIT (dataflow->visited, e->src->index))
	hybrid_search_backward (e->src, dataflow);
    }
}

/* This function will perform iterative bitvector dataflow described by
   DATAFLOW, producing the in and out sets.  Only the part of the cfg
   induced by blocks in DATAFLOW->order is taken into account.

   SINGLE_PASS is true if you just want to make one pass over the blocks. 
 */

void
iterative_dataflow (struct dataflow *dataflow) 
{
  unsigned int idx;
  int i;
  int * blocks_in_postorder = dataflow->postorder;
  int n_blocks = dataflow->n_blocks;
  bitmap viewing = dataflow->viewing;
  sbitmap visited = sbitmap_alloc (last_basic_block);
  sbitmap pending = sbitmap_alloc (last_basic_block);
  sbitmap considered = sbitmap_alloc (last_basic_block);
  basic_block bb;

  dataflow->visited = visited;
  dataflow->pending = pending;
  dataflow->considered = considered;

  sbitmap_zero (visited);
  sbitmap_zero (pending);
  sbitmap_zero (considered);

  FOR_EACH_BB_IN_BITMAP(viewing, 0, bb, 
    {
      int idx = bb->index;
      SET_BIT (considered, idx);
    });

  for (i = 0; i < n_blocks; i++)
    {
      idx = blocks_in_postorder[i];
      SET_BIT (pending, idx);
      (*dataflow->problem->init)(dataflow, idx);
    };

  while (1)
    {

      /* For forward problems, you want to pass in reverse postorder
         and for backward problems you want postorder.  This has been
         shown to be as good as you can do by several people, the
         first being Mathew Hecht in his phd dissertation.

	 The nodes are passed into this function in postorder.  */

      if (dataflow->problem->dir == DF_FORWARD)
	{
	  for (i = n_blocks - 1 ; i >= 0 ; i--)
	    {
	      idx = blocks_in_postorder[i];
	      
	      if (TEST_BIT (pending, idx) && !TEST_BIT (visited, idx))
		hybrid_search_forward (BASIC_BLOCK (idx), dataflow);
	    }
	}
      else
	{
	  for (i = 0; i < n_blocks; i++)
	    {
	      idx = blocks_in_postorder[i];
	      
	      if (TEST_BIT (pending, idx) && !TEST_BIT (visited, idx))
		hybrid_search_backward (BASIC_BLOCK (idx), dataflow);
	    }
	}

      if (sbitmap_first_set_bit (pending) == -1)
	break;

      sbitmap_zero (visited);
    }


  if (*dataflow->problem->finalizefun)
    (*dataflow->problem->finalizefun)(dataflow->df, dataflow->iterating);

  sbitmap_free (pending);
  sbitmap_free (visited);
  sbitmap_free (considered);
}

void 
df_compact_blocks (struct df *df)
{
  int i;
  basic_block bb;
  struct df_bb_info *temp;
  
  temp = xmalloc (df->n_bbs * sizeof (struct df_bb_info));
  memcpy (temp, df->bbs, sizeof (struct df_bb_info) * df->n_bbs);

  i = NUM_FIXED_BLOCKS;
  
  df_bb_table_realloc (df, last_basic_block);

  FOR_EACH_BB (bb) 
    {
      df->bbs[i] = temp[bb->index];
      BASIC_BLOCK (i) = bb;
      bb->index = i;
      i++;
    }
  gcc_assert (i == n_basic_blocks);
  
  for (; i < last_basic_block; i++)
    BASIC_BLOCK (i) = NULL;

  df->n_bbs = n_basic_blocks;
  free (temp);
}

void 
df_bb_replace (struct df *df, int old_index, basic_block new_block)
{
  struct df_bb_info bb_info;
  if (df && new_block->index != old_index)
    {
      df_bb_table_realloc (df, last_basic_block);
      bb_info = df->bbs[old_index];
      df->bbs[old_index] = df->bbs[new_block->index];
      df->bbs[new_block->index] = bb_info;
    }
  BASIC_BLOCK (old_index) = new_block;
  new_block->index = old_index;
 }
