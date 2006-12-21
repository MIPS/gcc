/* Scanning of rtl for dataflow analysis.
   Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   Originally contributed by Michael P. Hayes 
             (m.hayes@elec.canterbury.ac.nz, mhayes@redhat.com)
   Major rewrite contributed by Danny Berlin (dberlin@dberlin.org)
             and Kenneth Zadeck (zadeck@naturalbridge.com).

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
*/

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
#include "tree.h"
#include "target.h"
#include "target-def.h"
#include "df.h"
#include "tree-pass.h"

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

/* The set of hard registers in eliminables[i].from. */

static HARD_REG_SET elim_reg_set;

/* This is a bitmap copy of regs_invalidated_by_call so that we can
   easily add it into bitmaps, etc. */ 

bitmap df_invalidated_by_call = NULL;

/* Initialize ur_in and ur_out as if all hard registers were partially
   available.  */

static struct df_ref * df_ref_record (struct df_ref *,
                                      rtx, rtx *, 
			              basic_block, rtx, enum df_ref_type, 
                                      enum df_ref_flags);
static struct df_ref * df_def_record_1 (struct df_ref *,
                                        rtx, basic_block, rtx,
			                enum df_ref_flags);
static struct df_ref * df_defs_record (struct df_ref *,
                                       rtx, basic_block, rtx,
			               enum df_ref_flags);
static struct df_ref * df_uses_record (struct df_ref *,
                                       rtx *, enum df_ref_type,
			               basic_block, rtx, enum df_ref_flags);

static struct df_ref *df_ref_create_structure (rtx, rtx *, 
					       basic_block, rtx, enum df_ref_type, 
					       enum df_ref_flags);
static void df_get_regular_block_artificial_uses (bitmap);
static void df_get_eh_block_artificial_uses (bitmap);

static void df_record_entry_block_defs (bitmap);
static void df_record_exit_block_uses (bitmap);
static void df_get_exit_block_use_set (bitmap);
static void df_get_entry_block_def_set (bitmap);
static void df_grow_ref_info (struct df_ref_info *, unsigned int);
static void df_grow_insn_info (void);

/* A structure to hold multiple return values from df_ref_find_chains. */
struct df_chains
{
  struct df_ref **ref_chain;
  struct df_reg_info *reg_info;
  struct df_ref_info *ref_info;
};

static struct df_chains df_ref_find_chains (struct df_ref *);
static void df_ref_add_to_chains (struct df_ref *,
                                  struct df_mw_hardreg *);
static void df_refs_add_to_chains (rtx insn, struct df_ref *);

static bool df_insn_refs_verify (basic_block, 
				 rtx,
				 struct df_ref **,
				 bool);
static struct df_ref * df_entry_block_defs_collect (bitmap);
static struct df_ref * df_exit_block_uses_collect (bitmap);

#define DEBUG_DF_RESCAN
#ifdef DEBUG_DF_RESCAN
typedef bool (*df_ref_compare_func_t)(struct df_ref *, struct df_ref *);
static bool df_ref_is_equal (struct df_ref *, struct df_ref *);
static struct df_ref *df_ref_chain_find_ref (struct df_ref *, 
                                             struct df_ref *, 
                                             df_ref_compare_func_t);
static struct df_ref *df_reg_chain_find_ref (struct df_ref *, 
                                             struct df_ref *,
                                             df_ref_compare_func_t);
#endif /* DEBUG_DF_RESCAN */


/*----------------------------------------------------------------------------
   SCANNING DATAFLOW PROBLEM

   There are several ways in which scanning looks just like the other
   dataflow problems.  It shares the all the mechanisms for local info
   as well as basic block info.  Where it differs is when and how often
   it gets run.  It also has no need for the iterative solver.
----------------------------------------------------------------------------*/

/* Problem data for the scanning dataflow function.  */
struct df_scan_problem_data
{
  alloc_pool ref_pool;
  alloc_pool insn_pool;
  alloc_pool reg_pool;
  alloc_pool mw_reg_pool;
  alloc_pool mw_link_pool;
};

typedef struct df_scan_bb_info *df_scan_bb_info_t;

static void 
df_scan_free_internal (void)
{
  struct df_scan_problem_data *problem_data
    = (struct df_scan_problem_data *) df_scan->problem_data;

  free (df->def_info.refs);
  free (df->def_info.begin);
  free (df->def_info.count);
  memset (&df->def_info, 0, (sizeof (struct df_ref_info)));

  free (df->use_info.refs);
  free (df->use_info.begin);
  free (df->use_info.count);
  memset (&df->use_info, 0, (sizeof (struct df_ref_info)));

  free (df->def_regs);
  df->def_regs = NULL;
  free (df->use_regs);
  df->use_regs = NULL;
  free (df->eq_use_regs);
  df->eq_use_regs = NULL;
  df->regs_size = 0;
  DF_REG_SIZE(df) = 0;

  free (df->insns);
  df->insns = NULL;
  DF_INSN_SIZE () = 0;

  free (df_scan->block_info);
  df_scan->block_info = NULL;
  df_scan->block_info_size = 0;

  BITMAP_FREE (df->hardware_regs_used);
  BITMAP_FREE (df->regular_block_artificial_uses);
  BITMAP_FREE (df->eh_block_artificial_uses);
  BITMAP_FREE (df->entry_block_defs);
  BITMAP_FREE (df->exit_block_uses);
  BITMAP_FREE (df->insns_to_delete);
  BITMAP_FREE (df->insns_to_rescan);

  free_alloc_pool (df_scan->block_pool);
  free_alloc_pool (problem_data->ref_pool);
  free_alloc_pool (problem_data->insn_pool);
  free_alloc_pool (problem_data->reg_pool);
  free_alloc_pool (problem_data->mw_reg_pool);
  free_alloc_pool (problem_data->mw_link_pool);
  free (df_scan->problem_data);
}


/* Get basic block info.  */

struct df_scan_bb_info *
df_scan_get_bb_info (unsigned int index)
{
  gcc_assert (df_scan);
  gcc_assert (index < df_scan->block_info_size);
  return (struct df_scan_bb_info *) df_scan->block_info[index];
}


/* Set basic block info.  */

static void
df_scan_set_bb_info (unsigned int index, 
		     struct df_scan_bb_info *bb_info)
{
  gcc_assert (df_scan);
  df_grow_bb_info (df_scan);
  df_scan->block_info[index] = (void *) bb_info;
}


/* Free basic block info.  */

static void
df_scan_free_bb_info (basic_block bb, void *vbb_info)
{
  struct df_scan_bb_info *bb_info = (struct df_scan_bb_info *) vbb_info;
  if (bb_info)
    {
      df_bb_delete (bb->index);
      pool_free (df_scan->block_pool, bb_info);
    }
}


/* Allocate the problem data for the scanning problem.  This should be
   called when the problem is created or when the entire function is to
   be rescanned.  */
void 
df_scan_alloc (bitmap blocks_to_rescan ATTRIBUTE_UNUSED, 
	       bitmap all_blocks ATTRIBUTE_UNUSED)
{
  struct df_scan_problem_data *problem_data;
  unsigned int insn_num = get_max_uid () + 1;
  unsigned int block_size = 50;
  basic_block bb;

  /* Given the number of pools, this is really faster than tearing
     everything apart.  */
  if (df_scan->problem_data)
    df_scan_free_internal ();

  df_scan->block_pool 
    = create_alloc_pool ("df_scan_block pool", 
			 sizeof (struct df_scan_bb_info), 
			 block_size);

  problem_data = XNEW (struct df_scan_problem_data);
  df_scan->problem_data = problem_data;
  df_scan->computed = true;

  problem_data->ref_pool 
    = create_alloc_pool ("df_scan_ref pool", 
			 sizeof (struct df_ref), block_size);
  problem_data->insn_pool 
    = create_alloc_pool ("df_scan_insn pool", 
			 sizeof (struct df_insn_info), block_size);
  problem_data->reg_pool 
    = create_alloc_pool ("df_scan_reg pool", 
			 sizeof (struct df_reg_info), block_size);
  problem_data->mw_reg_pool 
    = create_alloc_pool ("df_scan_mw_reg pool", 
			 sizeof (struct df_mw_hardreg), block_size);
  problem_data->mw_link_pool 
    = create_alloc_pool ("df_scan_mw_link pool", 
			 sizeof (struct df_link), block_size);

  insn_num += insn_num / 4; 
  df_grow_reg_info ();
  df_grow_ref_info (&df->def_info, insn_num);
  df_grow_ref_info (&df->use_info, insn_num *2);

  df_grow_insn_info ();
  df_grow_bb_info (df_scan);

  FOR_ALL_BB (bb)
    {
      unsigned int bb_index = bb->index;
      struct df_scan_bb_info *bb_info = df_scan_get_bb_info (bb_index);
      if (!bb_info)
	{
	  bb_info = (struct df_scan_bb_info *) pool_alloc (df_scan->block_pool);
	  df_scan_set_bb_info (bb_index, bb_info);
	}
      bb_info->artificial_defs = NULL;
      bb_info->artificial_uses = NULL;
    }

  df->out_of_date_transfer_functions = BITMAP_ALLOC (NULL);
  df->hardware_regs_used = BITMAP_ALLOC (NULL);
  df->regular_block_artificial_uses = BITMAP_ALLOC (NULL);
  df->eh_block_artificial_uses = BITMAP_ALLOC (NULL);
  df->entry_block_defs = BITMAP_ALLOC (NULL);
  df->exit_block_uses = BITMAP_ALLOC (NULL);
  df->insns_to_delete = BITMAP_ALLOC (NULL);
  df->insns_to_rescan = BITMAP_ALLOC (NULL);
}


/* Free all of the data associated with the scan problem.  */

static void 
df_scan_free (void)
{
  if (df_scan->problem_data)
    df_scan_free_internal ();

  if (df->blocks_to_analyze)
    BITMAP_FREE (df->blocks_to_analyze);

  BITMAP_FREE (df->out_of_date_transfer_functions);

  free (df_scan);
}

/* Dump the preamble for DF_SCAN dump. */
static void 
df_scan_start_dump (FILE *file ATTRIBUTE_UNUSED)
{
  int i;

  fprintf (file, "  invalidated by call \t");
  df_print_regset (file, df_invalidated_by_call);
  fprintf (file, "  hardware regs used \t");
  df_print_regset (file, df->hardware_regs_used);
  fprintf (file, "  regular block artificial uses \t");
  df_print_regset (file, df->regular_block_artificial_uses);
  fprintf (file, "  eh block artificial uses \t");
  df_print_regset (file, df->eh_block_artificial_uses);
  fprintf (file, "  entry block defs \t");
  df_print_regset (file, df->entry_block_defs);
  fprintf (file, "  exit block uses \t");
  df_print_regset (file, df->exit_block_uses);
  fprintf (file, "  regs ever live \t");
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (regs_ever_live[i])
      {
	fprintf (file, " %d", i);
	if (i < FIRST_PSEUDO_REGISTER)
	  fprintf (file, " [%s]", reg_names[i]);
      }

  fprintf (file, "\n");
}

/* Dump the bb_info for a given basic block. */
static void 
df_scan_start_block (basic_block bb, FILE *file)
{
  struct df_scan_bb_info *bb_info
    = df_scan_get_bb_info (bb->index);

  if (bb_info)
    {
      fprintf (file, "bb %d artificial_defs: ", bb->index);
      df_refs_chain_dump (bb_info->artificial_defs, true, file);
      fprintf (file, "\nbb %d artificial_uses: ", bb->index);
      df_refs_chain_dump (bb_info->artificial_uses, true, file);
      fprintf (file, "\n");
    }
}

static struct df_problem problem_SCAN =
{
  DF_SCAN,                    /* Problem id.  */
  DF_NONE,                    /* Direction.  */
  df_scan_alloc,              /* Allocate the problem specific data.  */
  NULL,                       /* Reset global information.  */
  df_scan_free_bb_info,       /* Free basic block info.  */
  NULL,                       /* Local compute function.  */
  NULL,                       /* Init the solution specific data.  */
  NULL,                       /* Iterative solver.  */
  NULL,                       /* Confluence operator 0.  */ 
  NULL,                       /* Confluence operator n.  */ 
  NULL,                       /* Transfer function.  */
  NULL,                       /* Finalize function.  */
  df_scan_free,               /* Free all of the problem information.  */
  NULL,                       /* Remove this problem from the stack of dataflow problems.  */
  df_scan_start_dump,         /* Debugging.  */
  df_scan_start_block,        /* Debugging start block.  */
  NULL,                       /* Debugging end block.  */
  NULL                        /* Dependent problem.  */
};


/* Create a new DATAFLOW instance and add it to an existing instance
   of DF.  The returned structure is what is used to get at the
   solution.  */

void
df_scan_add_problem (void)
{
  df_add_problem (&problem_SCAN);
}


/*----------------------------------------------------------------------------
   Storage Allocation Utilities
----------------------------------------------------------------------------*/


/* First, grow the reg_info information.  If the current size is less than
   the number of psuedos, grow to 25% more than the number of
   pseudos.  

   Second, assure that all of the slots up to max_reg_num have been
   filled with reg_info structures.  */

void 
df_grow_reg_info (void)
{
  unsigned int max_reg = max_reg_num ();
  unsigned int new_size = max_reg;
  struct df_scan_problem_data *problem_data
    = (struct df_scan_problem_data *) df_scan->problem_data;
  unsigned int i;

  if (df->regs_size < new_size)
    {
      new_size += new_size / 4;
      df->def_regs = xrealloc (df->def_regs, 
			       new_size *sizeof (struct df_reg_info*));
      df->use_regs = xrealloc (df->use_regs, 
			       new_size *sizeof (struct df_reg_info*));
      df->eq_use_regs = xrealloc (df->eq_use_regs, 
				  new_size *sizeof (struct df_reg_info*));
      df->def_info.begin = xrealloc (df->def_info.begin, 
				      new_size *sizeof (int));
      df->def_info.count = xrealloc (df->def_info.count, 
				      new_size *sizeof (int));
      df->use_info.begin = xrealloc (df->use_info.begin, 
				      new_size *sizeof (int));
      df->use_info.count = xrealloc (df->use_info.count, 
				      new_size *sizeof (int));
      df->regs_size = new_size;
    }

  for (i = df->regs_inited; i < max_reg; i++)
    {
      struct df_reg_info *reg_info;

      reg_info = pool_alloc (problem_data->reg_pool);
      memset (reg_info, 0, sizeof (struct df_reg_info));
      df->def_regs[i] = reg_info;
      reg_info = pool_alloc (problem_data->reg_pool);
      memset (reg_info, 0, sizeof (struct df_reg_info));
      df->use_regs[i] = reg_info;
      reg_info = pool_alloc (problem_data->reg_pool);
      memset (reg_info, 0, sizeof (struct df_reg_info));
      df->eq_use_regs[i] = reg_info;
      df->def_info.begin[i] = 0;
      df->def_info.count[i] = 0;
      df->use_info.begin[i] = 0;
      df->use_info.count[i] = 0;
    }
  
  df->regs_inited = max_reg;
}


/* Grow the ref information.  */

static void 
df_grow_ref_info (struct df_ref_info *ref_info, unsigned int new_size)
{
  if (ref_info->refs_size < new_size)
    {
      ref_info->refs = xrealloc (ref_info->refs, 
				 new_size *sizeof (struct df_ref *));
      memset (ref_info->refs + ref_info->refs_size, 0,
	      (new_size - ref_info->refs_size) *sizeof (struct df_ref *));
      ref_info->refs_size = new_size;
    }
}


/* Check and grow the ref information if necessary.  This routine
   guarantees total_size + BITMAP_ADDEND amount of entries in refs
   array.  It updates ref_info->refs_size only and does not change
   ref_info->total_size.  */

static void
df_check_and_grow_ref_info (struct df_ref_info *ref_info, 
			    unsigned bitmap_addend)
{
  if (ref_info->refs_size < ref_info->total_size + bitmap_addend)
    {
      int new_size = ref_info->total_size + ref_info->total_size / 4;
      df_grow_ref_info (ref_info, new_size);
    }
}


/* Grow the ref information.  If the current size is less than the
   number of instructions, grow to 25% more than the number of
   instructions.  */

static void 
df_grow_insn_info (void)
{
  unsigned int new_size = get_max_uid () + 1;
  if (DF_INSN_SIZE () < new_size)
    {
      new_size += new_size / 4;
      df->insns = xrealloc (df->insns, 
			    new_size *sizeof (struct df_insn_info *));
      memset (df->insns + df->insns_size, 0,
	      (new_size - DF_INSN_SIZE ()) *sizeof (struct df_insn_info *));
      DF_INSN_SIZE () = new_size;
    }
}




/*----------------------------------------------------------------------------
   PUBLIC INTERFACES FOR SMALL GRAIN CHANGES TO SCANNING.
----------------------------------------------------------------------------*/

/* Rescan all of the block_to_analyze or all of the blocks in the
   function if df_set_blocks if blocks_to_analyze is NULL;  */

void
df_scan_blocks (void)
{
  basic_block bb;

  df->def_info.refs_organized_with_eq_uses = false;
  df->def_info.refs_organized_alone = false;
  df->use_info.refs_organized_with_eq_uses = false;
  df->use_info.refs_organized_alone = false;

  df->def_info.add_refs_inline = false;
  df->use_info.add_refs_inline = false;

  df_get_regular_block_artificial_uses (df->regular_block_artificial_uses);
  df_get_eh_block_artificial_uses (df->eh_block_artificial_uses);

  bitmap_ior_into (df->eh_block_artificial_uses, 
		   df->regular_block_artificial_uses);

  /* ENTRY and EXIT blocks have special defs/uses.  */
  df_get_entry_block_def_set (df->entry_block_defs);
  df_record_entry_block_defs (df->entry_block_defs);
  df_get_exit_block_use_set (df->exit_block_uses);
  df_record_exit_block_uses (df->exit_block_uses);

  /* Regular blocks */
  FOR_EACH_BB (bb)
    {
      df_bb_refs_record (bb->index, true);
    }
}


/* Create a new ref of type DF_REF_TYPE for register REG at address
   LOC within INSN of BB.  */

struct df_ref *
df_ref_create (rtx reg, rtx *loc, rtx insn, 
	       basic_block bb,
	       enum df_ref_type ref_type, 
	       enum df_ref_flags ref_flags)
{
  struct df_ref *ref;
  struct df_scan_bb_info *bb_info;
  
  df_grow_reg_info ();
  df_grow_bb_info (df_scan);
  
  /* Make sure there is the bb_info for this block.  */
  bb_info = df_scan_get_bb_info (bb->index);
  if (!bb_info)
    {
      bb_info = (struct df_scan_bb_info *) pool_alloc (df_scan->block_pool);
      df_scan_set_bb_info (bb->index, bb_info);
      bb_info->artificial_defs = NULL;
      bb_info->artificial_uses = NULL;
    }

  if (ref_type == DF_REF_REG_DEF)
    df->def_info.add_refs_inline = true;
  else
    df->use_info.add_refs_inline = true;
  
  ref = df_ref_create_structure (reg, loc, bb, insn,
                                 ref_type, ref_flags);

  df_ref_add_to_chains (ref, NULL);

  return ref;
}



/*----------------------------------------------------------------------------
   UTILITIES TO CREATE AND DESTROY REFS AND CHAINS.
----------------------------------------------------------------------------*/


/* Get the artificial defs for a basic block.  */

struct df_ref *
df_get_artificial_defs (unsigned int bb_index)
{
  return df_scan_get_bb_info (bb_index)->artificial_defs;
}


/* Get the artificial uses for a basic block.  */

struct df_ref *
df_get_artificial_uses (unsigned int bb_index)
{
  return df_scan_get_bb_info (bb_index)->artificial_uses;
}


/* Link REF at the front of reg_use or reg_def chain for REGNO.  */

void
df_reg_chain_create (struct df_reg_info *reg_info, 
		     struct df_ref *ref) 
{
  struct df_ref *head = reg_info->reg_chain;

  reg_info->reg_chain = ref;
  reg_info->n_refs++;

  gcc_assert (DF_REF_NEXT_REG (ref) == NULL);
  gcc_assert (DF_REF_PREV_REG (ref) == NULL);

  DF_REF_NEXT_REG (ref) = head;

  /* We cannot actually link to the head of the chain.  */
  DF_REF_PREV_REG (ref) = NULL;

  if (head)
    DF_REF_PREV_REG (head) = ref;
}


/* Remove REF from the CHAIN.  Return the head of the chain.  This
   will be CHAIN unless the REF was at the beginning of the chain.  */

static struct df_ref *
df_ref_unlink (struct df_ref *chain, struct df_ref *ref)
{
  struct df_ref *orig_chain = chain;
  struct df_ref *prev = NULL;
  while (chain)
    {
      if (chain == ref)
	{
	  if (prev)
	    {
	      prev->next_ref = ref->next_ref;
	      ref->next_ref = NULL;
	      return orig_chain;
	    }
	  else
	    {
	      chain = ref->next_ref;
	      ref->next_ref = NULL;
	      return chain;
	    }
	}

      prev = chain;
      chain = chain->next_ref;
    }

  /* Someone passed in a ref that was not in the chain.  */
  gcc_unreachable ();
  return NULL;
}


/* Unlink and delete REF at the reg_use or reg_def chain.  Also delete
   the def-use or use-def chain if it exists.  Returns the
   REF->next. This does not unlink the ref from the insn, for that
   call df_ref_remove which does that and calls this function.  */

struct df_ref *
df_reg_chain_unlink (struct df_ref *ref) 
{
  struct df_ref *next = DF_REF_NEXT_REG (ref);  
  struct df_ref *prev = DF_REF_PREV_REG (ref);
  struct df_scan_problem_data *problem_data
    = (struct df_scan_problem_data *) df_scan->problem_data;
  struct df_ref *next_ref = ref->next_ref;
  int id = DF_REF_ID (ref);
  struct df_reg_info *reg_info;

  if (DF_REF_TYPE (ref) == DF_REF_REG_DEF)
    {
      reg_info = DF_REG_DEF_GET (DF_REF_REGNO (ref));
      if (id >= 0)
	{
	  gcc_assert (id < (int)df->def_info.table_size);
	  DF_DEFS_SET (id, NULL);
	}
    }
  else 
    {
      if (DF_REF_FLAGS (ref) & DF_REF_IN_NOTE)
	reg_info = DF_REG_EQ_USE_GET (DF_REF_REGNO (ref));
      else
	reg_info = DF_REG_USE_GET (DF_REF_REGNO (ref));
      if (id >= 0)
	{
	  gcc_assert (id < (int)df->use_info.table_size);
	  DF_USES_SET (id, NULL);
	}
    }
  
  /* Delete any def-use or use-def chains that start here.  */
  if (DF_REF_CHAIN (ref))
    df_chain_unlink (ref);
  
  reg_info->n_refs--;

  /* Unlink from the reg chain.  If there is no prev, this is the
     first of the list.  If not, just join the next and prev.  */
  if (prev)
    DF_REF_NEXT_REG (prev) = next;
  else
    {
      gcc_assert (reg_info->reg_chain == ref);
      reg_info->reg_chain = next;
    }
  if (next)
    DF_REF_PREV_REG (next) = prev;

  pool_free (problem_data->ref_pool, ref);
  return next_ref;
}


/* Unlink REF from all def-use/use-def chains, etc.  */

void
df_ref_remove (struct df_ref *ref)
{
  if (DF_REF_REG_DEF_P (ref))
    {
      if (DF_REF_IS_ARTIFICIAL (ref))
	{
	  struct df_scan_bb_info *bb_info 
	    = df_scan_get_bb_info (DF_REF_BB (ref)->index);
	  bb_info->artificial_defs 
	    = df_ref_unlink (bb_info->artificial_defs, ref);
	}
      else
	{
	  unsigned int uid = DF_REF_INSN_UID (ref);
	  struct df_insn_info *insn_rec = DF_INSN_UID_GET (uid);
	  insn_rec->defs = df_ref_unlink (insn_rec->defs, ref);
	}
    }
  else
    {
      if (DF_REF_IS_ARTIFICIAL (ref))
	{
	  struct df_scan_bb_info *bb_info 
	    = df_scan_get_bb_info (DF_REF_BB (ref)->index);
	  bb_info->artificial_uses 
	    = df_ref_unlink (bb_info->artificial_uses, ref);
	}
      else 
	{
	  unsigned int uid = DF_REF_INSN_UID (ref);
	  struct df_insn_info *insn_rec = DF_INSN_UID_GET (uid);

	  if (DF_REF_FLAGS (ref) & DF_REF_IN_NOTE)
	    insn_rec->eq_uses = df_ref_unlink (insn_rec->eq_uses, ref);
	  else
	    insn_rec->uses = df_ref_unlink (insn_rec->uses, ref);
	}
    }

  df_reg_chain_unlink (ref);
}


/* Create the insn record for INSN.  If there was one there, zero it out.  */

struct df_insn_info *
df_insn_create_insn_record (rtx insn)
{
  struct df_scan_problem_data *problem_data
    = (struct df_scan_problem_data *) df_scan->problem_data;
  struct df_insn_info *insn_rec;

  df_grow_insn_info ();
  insn_rec = DF_INSN_GET (insn);
  if (!insn_rec)
    {
      insn_rec = pool_alloc (problem_data->insn_pool);
      DF_INSN_SET (insn, insn_rec);
    }
  memset (insn_rec, 0, sizeof (struct df_insn_info));
  insn_rec->insn = insn;
  return insn_rec;
}


/* Delete all du chain (DF_REF_CHAIN()) of all refs in the ref chain.  */

static void
df_ref_chain_delete_du_chain (struct df_ref *ref)
{
  for (; ref; ref = DF_REF_NEXT_REF (ref)) 
    {
      /* CHAIN is allocated by DF_CHAIN. So make sure to 
         pass df_scan instance for the problem.  */
      if (DF_REF_CHAIN (ref))
        df_chain_unlink (ref);
    }
}


/* Delete all refs in the ref chain.  */

static void
df_ref_chain_delete (struct df_ref *ref)
{
  while (ref)
    ref = df_reg_chain_unlink (ref);
}


/* Delete the hardreg chain.  */

static void
df_mw_hardreg_chain_delete (struct df_mw_hardreg *hardregs)
{
  struct df_scan_problem_data *problem_data;

  problem_data = (struct df_scan_problem_data *) df_scan->problem_data;

  while (hardregs)
    {
      struct df_mw_hardreg *next_hr = hardregs->next;
      struct df_link *link = hardregs->regs;
      while (link)
        {
          struct df_link *next_l = link->next;
          pool_free (problem_data->mw_link_pool, link);
          link = next_l;
        }
      
      pool_free (problem_data->mw_reg_pool, hardregs);
      hardregs = next_hr;
    }
}


/* Delete all of the refs information from INSN.  */

void 
df_insn_delete (unsigned int uid)
{
  struct df_insn_info *insn_info = NULL;

  if (!df)
    return;

  df_grow_bb_info (df_scan);
  df_grow_reg_info ();

  insn_info = DF_INSN_UID_SAFE_GET (uid);

  /* The client has defered rescanning.  */
  if (df->changeable_flags & DF_DEFER_INSN_RESCAN)
    {
      if (insn_info)
	{
	  bitmap_clear_bit (df->insns_to_rescan, uid);
	  bitmap_set_bit (df->insns_to_delete, uid);
	}
      if (dump_file)
	fprintf (dump_file, "defering deletion of insn with uid = %d.\n", uid);
      return;
    }

  if (dump_file)
    fprintf (dump_file, "deleting insn with uid = %d.\n", uid);

  bitmap_clear_bit (df->insns_to_delete, uid);
  bitmap_clear_bit (df->insns_to_rescan, uid);
  if (insn_info)
    {
      struct df_scan_problem_data *problem_data;

      problem_data = (struct df_scan_problem_data *) df_scan->problem_data;
      df_mw_hardreg_chain_delete (insn_info->mw_hardregs);

      df_ref_chain_delete_du_chain (insn_info->defs);
      df_ref_chain_delete_du_chain (insn_info->uses);
      df_ref_chain_delete_du_chain (insn_info->eq_uses);

      df_ref_chain_delete (insn_info->defs);
      df_ref_chain_delete (insn_info->uses);
      df_ref_chain_delete (insn_info->eq_uses);

      pool_free (problem_data->insn_pool, insn_info);
      DF_INSN_UID_SET (uid, NULL);
    }
}


/* Delete all of the refs information from basic_block with BB_INDEX.  */

void
df_bb_delete (unsigned int bb_index)
{
  struct df_scan_bb_info *bb_info = NULL; 
  rtx insn;
  basic_block bb = BASIC_BLOCK (bb_index);

  if (!df)
    return;

  FOR_BB_INSNS (bb, insn)
    {
      if (INSN_P (insn))
	/* Record defs within INSN.  */
	df_insn_delete (INSN_UID (insn));
    }

  if (bb_index < df_scan->block_info_size)
    bb_info = df_scan_get_bb_info (bb_index);
  
  /* Get rid of any artificial uses or defs.  */
  if (bb_info)
    {
      df_ref_chain_delete_du_chain (bb_info->artificial_defs);
      df_ref_chain_delete_du_chain (bb_info->artificial_uses);
      df_ref_chain_delete (bb_info->artificial_defs);
      df_ref_chain_delete (bb_info->artificial_uses);
      bb_info->artificial_defs = NULL;
      bb_info->artificial_uses = NULL;
    }
}


/* Rescan INSN.  Return TRUE if the rescanning produced any changes.  */

bool 
df_insn_rescan (rtx insn)
{
  unsigned int uid = INSN_UID (insn);
  struct df_insn_info *insn_info = NULL;
  basic_block bb = BLOCK_FOR_INSN (insn);
  struct df_ref *refs = NULL;

  if ((!df) || (!INSN_P (insn)))
    return false;

  if (!bb)
    {
      if (dump_file)
	fprintf (dump_file, "no bb for insn with uid = %d.\n", uid);
      return false;
    }

  /* The client has disabled rescanning and plans to do it itself.  */
  if (df->changeable_flags & DF_NO_INSN_RESCAN)
    return false;

  df_grow_bb_info (df_scan);
  df_grow_reg_info ();

  insn_info = DF_INSN_UID_SAFE_GET (uid);

  /* The client has defered rescanning.  */
  if (df->changeable_flags & DF_DEFER_INSN_RESCAN)
    {
      if (!insn_info)
	insn_info = df_insn_create_insn_record (insn);
      if (dump_file)
	fprintf (dump_file, "defering resscan insn with uid = %d.\n", uid);
    
      bitmap_clear_bit (df->insns_to_delete, uid);
      bitmap_set_bit (df->insns_to_rescan, INSN_UID (insn));
      return false;
    }

  bitmap_clear_bit (df->insns_to_delete, uid);
  bitmap_clear_bit (df->insns_to_rescan, uid);
  if (insn_info)
    {
      bool the_same = df_insn_refs_verify (bb, insn, &refs, false);
      /* If there's no change, return false. */
      if (the_same)
	{
	  if (dump_file)
	    fprintf (dump_file, "verify found no changes in insn with uid = %d.\n", uid);
	  return false;
	}
      if (dump_file)
	fprintf (dump_file, "rescanning insn with uid = %d.\n", uid);

      /* There's change - we need to delete the existing info. */
      df_insn_delete (uid);
      df_insn_create_insn_record (insn);
    }
  else
    {
      df_insn_create_insn_record (insn);
      df_insn_refs_verify (bb, insn, &refs, false);
      if (dump_file)
	fprintf (dump_file, "scanning new insn with uid = %d.\n", uid);
    }

  df_insn_refs_record (insn, refs); 
  df_set_bb_dirty (bb);
  return true;
}


/* Rescan all of the insns in the function.  Note that the artificial
   uses and defs are not touched.  This function will destroy def-se
   or use-def chains.  */

void
df_insn_rescan_all (void)
{
  bool no_insn_rescan = false;
  bool defer_insn_rescan = false;
  basic_block bb;
  bitmap_iterator bi;
  unsigned int uid;
  bitmap tmp = BITMAP_ALLOC (NULL);
  
  if (df->changeable_flags & DF_NO_INSN_RESCAN)
    {
      df_clear_flags (DF_NO_INSN_RESCAN);
      no_insn_rescan = true;
    }
  
  if (df->changeable_flags & DF_DEFER_INSN_RESCAN)
    {
      df_clear_flags (DF_DEFER_INSN_RESCAN);
      defer_insn_rescan = true;
    }

  bitmap_copy (tmp, df->insns_to_delete);
  EXECUTE_IF_SET_IN_BITMAP (tmp, 0, uid, bi)
    {
      struct df_insn_info *insn_info = DF_INSN_UID_SAFE_GET (uid);
      if (insn_info)
	df_insn_delete (uid);
    }

  BITMAP_FREE (tmp);
  bitmap_clear (df->insns_to_delete);
  bitmap_clear (df->insns_to_rescan);

  FOR_EACH_BB (bb) 
    {
      rtx insn;
      FOR_BB_INSNS (bb, insn)
	{
	  df_insn_rescan (insn);
	}
    }

  if (no_insn_rescan)
    df_set_flags (DF_NO_INSN_RESCAN);
  if (defer_insn_rescan)
    df_set_flags (DF_DEFER_INSN_RESCAN);
}


/* Process all of the defered rescans or deletions.  */

void
df_process_deferred_rescans (void)
{
  bool no_insn_rescan = false;
  bool defer_insn_rescan = false;
  bitmap_iterator bi;
  unsigned int uid;
  bitmap tmp = BITMAP_ALLOC (NULL);
  
  if (df->changeable_flags & DF_NO_INSN_RESCAN)
    {
      df_clear_flags (DF_NO_INSN_RESCAN);
      no_insn_rescan = true;
    }
  
  if (df->changeable_flags & DF_DEFER_INSN_RESCAN)
    {
      df_clear_flags (DF_DEFER_INSN_RESCAN);
      defer_insn_rescan = true;
    }

  if (dump_file)
    fprintf (dump_file, "starting the processing of defered insns\n");

  bitmap_copy (tmp, df->insns_to_delete);
  EXECUTE_IF_SET_IN_BITMAP (tmp, 0, uid, bi)
    {
      struct df_insn_info *insn_info = DF_INSN_UID_SAFE_GET (uid);
      if (insn_info)
	df_insn_delete (uid);
    }

  bitmap_copy (tmp, df->insns_to_rescan);
  EXECUTE_IF_SET_IN_BITMAP (tmp, 0, uid, bi)
    {
      struct df_insn_info *insn_info = DF_INSN_UID_SAFE_GET (uid);
      if (insn_info)
	df_insn_rescan (insn_info->insn);
    }

  if (dump_file)
    fprintf (dump_file, "ending the processing of defered insns\n");

  BITMAP_FREE (tmp);
  bitmap_clear (df->insns_to_delete);
  bitmap_clear (df->insns_to_rescan);

  if (no_insn_rescan)
    df_set_flags (DF_NO_INSN_RESCAN);
  if (defer_insn_rescan)
    df_set_flags (DF_DEFER_INSN_RESCAN);
}


/* Take build ref table for either the uses or defs from the reg-use
   or reg-def chains.  */

static void 
df_reorganize_refs (struct df_ref_info *ref_info,
		    struct df_reg_info **reg1_info,
		    struct df_reg_info **reg2_info)
{
  unsigned int m = df->regs_inited;
  unsigned int regno;
  unsigned int offset = 0;
  unsigned int size = 0;

  df_check_and_grow_ref_info (ref_info, 1);

  for (regno = 0; regno < m; regno++)
    {
      struct df_reg_info *reg_info = reg1_info[regno];
      struct df_ref *ref = reg_info->reg_chain;
      int count = 0;
      ref_info->begin[regno] = offset;
      while (ref) 
	{
	  if ((!df->blocks_to_analyze)
	      || bitmap_bit_p (df->blocks_to_analyze, DF_REF_BB(ref)->index))
	    {
	      ref_info->refs[offset] = ref;
	      DF_REF_ID (ref) = offset++;
	      count++;
	    }
	  else
	    DF_REF_ID (ref) = -1;

	  size++;
	  ref = DF_REF_NEXT_REG (ref);
	  gcc_assert (offset < ref_info->refs_size);
	}
      if (reg2_info)
	{
	  reg_info = reg2_info[regno];
	  gcc_assert (reg_info);
	  ref = reg_info->reg_chain;
	  while (ref) 
	    {
	      if ((!df->blocks_to_analyze)
		  || bitmap_bit_p (df->blocks_to_analyze, DF_REF_BB(ref)->index))
		{
		  ref_info->refs[offset] = ref;
		  DF_REF_ID (ref) = offset++;
		  count++;
		}
	      else
		DF_REF_ID (ref) = -1;

	      size++;
	      ref = DF_REF_NEXT_REG (ref);
	      gcc_assert (offset < ref_info->refs_size);
	    }
	}
      ref_info->count[regno] = count;
    }

  /* The bitmap size is not decremented when refs are deleted.  So
     reset it now that we have squished out all of the empty
     slots.  */

  ref_info->table_size = offset;
  ref_info->total_size = size;
  if (reg2_info)
    {
      ref_info->refs_organized_with_eq_uses = true;
      ref_info->refs_organized_alone = false;
    }
  else
    {
      ref_info->refs_organized_with_eq_uses = false;
      ref_info->refs_organized_alone = true;
    }
  ref_info->add_refs_inline = true;
}


/* Change the BB of all refs in the ref chain to NEW_BB.
   Assumes that all refs in the chain have the same BB.
   If changed, return the original bb the chain belonged to
   (or .
   If no change, return NEW_BB. 
   If something's wrong, it will return NULL. */

static basic_block
df_ref_chain_change_bb (struct df_ref *ref, 
                        basic_block old_bb,
                        basic_block new_bb)
{
  for (; ref; ref = DF_REF_NEXT_REF (ref))
    if (DF_REF_BB (ref) == new_bb)
      return new_bb;
    else
      {
        gcc_assert (old_bb == NULL || DF_REF_BB (ref) == old_bb);
	old_bb = DF_REF_BB (ref);
	DF_REF_BB (ref) = new_bb;
      }

  return old_bb;
}


/* Change all of the basic block references in INSN to use the insn's
   current basic block.  This function is called from routines that move 
   instructions from one block to another.  */  

void
df_insn_change_bb (rtx insn)
{
  basic_block new_bb = BLOCK_FOR_INSN (insn);
  basic_block old_bb = NULL;
  struct df_insn_info *insn_info;
  unsigned int uid = INSN_UID (insn);

  if (!df)
    return;

  insn_info = DF_INSN_UID_SAFE_GET (uid);
  if (insn_info == NULL)
    {
      /* Should we mark the bb as dirty here ?  */
      df_insn_rescan (insn);
      return;
    }

  old_bb = df_ref_chain_change_bb (insn_info->defs, old_bb, new_bb);
  if (old_bb == new_bb) 
    return;

  old_bb = df_ref_chain_change_bb (insn_info->uses, old_bb, new_bb);
  if (old_bb == new_bb) 
    return;

  old_bb = df_ref_chain_change_bb (insn_info->eq_uses, old_bb, new_bb);
  if (old_bb == new_bb) 
    return;

  if (old_bb)
    df_set_bb_dirty (old_bb);

  df_set_bb_dirty (new_bb);
}


/* If the use refs in DF are not organized, reorganize them.  */

void 
df_maybe_reorganize_use_refs (void)
{
  if (df->changeable_flags & DF_EQ_NOTES)
    {
      if (!df->use_info.refs_organized_with_eq_uses)
	df_reorganize_refs (&df->use_info, 
			    df->use_regs, df->eq_use_regs);
    }
  else if (!df->use_info.refs_organized_alone)
    {
      unsigned int regno;
      int size;
      unsigned int m = df->regs_inited;
      df_reorganize_refs (&df->use_info, df->use_regs, NULL);

      /* The eq_uses do not go into the table but we must account for
	 them and reset their ids.  */
      size = df->use_info.total_size;
      for (regno = 0; regno < m; regno++)
	{
	  struct df_ref * ref = DF_REG_EQ_USE_CHAIN (regno);
	  while (ref) 
	    {
	      DF_REF_ID (ref) = -1;
	      size++;
	      ref = DF_REF_NEXT_REG (ref);
	    }
	}
      df->use_info.total_size = size;
    }
}


/* If the def refs in DF are not organized, reorganize them.  */

void 
df_maybe_reorganize_def_refs (void)
{
  if (!df->def_info.refs_organized_alone)
    df_reorganize_refs (&df->def_info, df->def_regs, NULL);
}

#ifdef DEBUG_DF_RESCAN

/* Return true if the contents of two df_ref's are identical. 
   It ignores DF_REF_MARKER.  */

static bool
df_ref_is_equal (struct df_ref *ref1, struct df_ref *ref2)
{
  return (ref1 == ref2) ||
    (DF_REF_REG (ref1) == DF_REF_REG (ref2)
     && DF_REF_REGNO (ref1) == DF_REF_REGNO (ref2)
     && DF_REF_LOC (ref1) == DF_REF_LOC (ref2)
     && DF_REF_INSN (ref1) == DF_REF_INSN (ref2)
     && DF_REF_TYPE (ref1) == DF_REF_TYPE (ref2)
     && (DF_REF_FLAGS (ref1) & ~(DF_REF_REF_MARKER + DF_REF_REG_MARKER)) 
     == (DF_REF_FLAGS (ref2) & ~(DF_REF_REF_MARKER + DF_REF_REG_MARKER))
     && DF_REF_BB (ref1) == DF_REF_BB (ref2));
}


/* Return true if the first ref should be recorded in regs_ever_live .  */

static bool
df_ref_is_record_live (struct df_ref *ref1, 
                       struct df_ref *ref2  __attribute__ ((__unused__)))
{
  unsigned int regno;

  /* If this ref is a def.  */
  if (DF_REF_TYPE (ref1) == DF_REF_REG_DEF)
    return !DF_REF_FLAGS_IS_SET (ref1, DF_REF_MAY_CLOBBER)
        && !DF_REF_IS_ARTIFICIAL (ref1);

  /* If this ref is not a def.  */
  regno = DF_REF_REGNO (ref1);

  return !DF_REF_IS_ARTIFICIAL (ref1) 
    && !(TEST_HARD_REG_BIT (elim_reg_set, regno)
         && (regno == FRAME_POINTER_REGNUM
             || regno == ARG_POINTER_REGNUM));
}


/* Find a matching df_ref in the ref chain. */

static struct df_ref *
df_ref_chain_find_ref (struct df_ref *chain, 
                       struct df_ref *this_ref,
                       df_ref_compare_func_t func)
{
  while (chain) 
    {
      if (func (chain, this_ref))
        return chain;
      chain = chain->next_ref;
    }

  return NULL;
}

/* Find a matching df_ref in the ref chain */

static struct df_ref *
df_ref_chain_find_ref_by_regno (struct df_ref *chain, 
				unsigned int regno)
{
  while (chain) 
    {
      if (DF_REF_REGNO (chain) == regno)
        return chain;
      chain = chain->next_ref;
    }

  return NULL;
}

/* Find a matching df_ref in the ref chain */

static struct df_ref *
df_reg_chain_find_ref (struct df_ref *chain, 
                       struct df_ref *this_ref, 
                       df_ref_compare_func_t func)
{
  while (chain) 
    {
      if (func (chain, this_ref))
        return chain;
      chain = chain->next_reg;
    }

  return NULL;
}
#endif /* DEBUG_DF_RESCAN */


/* Append a new ref ITEM to the end of existing ref chain LAST_ELEMENT. 
   LAST_ELEMENT should point to the last element of the existing chain.  */

static struct df_ref *
df_ref_chain_append (struct df_ref *last_element, struct df_ref *item)
{
  last_element->next_ref = item;
  return item;
}


/* Helper function for df_ref_change_reg_with_loc.  */

static void
df_ref_change_reg_with_loc_1 (struct df_reg_info *old, struct df_reg_info *new,
			      int new_regno, rtx loc)
{
  struct df_ref *the_ref = old->reg_chain;

  while (the_ref)
    {
      if (DF_REF_LOC(the_ref) && (*DF_REF_LOC(the_ref) == loc))
	{
	  struct df_ref *next_ref = the_ref->next_reg;
	  struct df_ref *prev_ref = the_ref->prev_reg;

	  DF_REF_REGNO (the_ref) = new_regno;
	  DF_REF_REG (the_ref) = regno_reg_rtx[new_regno];

	  /* Pull the_ref out of the old regno chain.  */
	  if (prev_ref)
	    prev_ref->next_reg = next_ref;
	  else
	    old->reg_chain = next_ref;
	  if (next_ref)
	    next_ref->prev_reg = prev_ref;
	  old->n_refs--;

	  /* Put the ref into the new regno chain.  */
	  the_ref->prev_reg = NULL;
	  the_ref->next_reg = new->reg_chain;
	  if (new->reg_chain)
	    new->reg_chain->prev_reg = the_ref;
	  new->reg_chain = the_ref;
	  new->n_refs++;

	  the_ref = next_ref;
	}
      else
	the_ref = the_ref->next_reg;
    }
}



/* Change the regno of all refs that contained LOC from OLD_REGNO to
   NEW_REGNO.  Refs that do not match LOC are not changed.  This call
   is to support the SET_REGNO macro. */

void
df_ref_change_reg_with_loc (int old_regno, int new_regno, rtx loc)
{
  if ((!df) || (old_regno == -1) || (old_regno == new_regno))
    return;

  df_grow_reg_info ();

  df_ref_change_reg_with_loc_1 (DF_REG_DEF_GET (old_regno), 
				DF_REG_DEF_GET (new_regno), new_regno, loc);
  df_ref_change_reg_with_loc_1 (DF_REG_USE_GET (old_regno), 
				DF_REG_USE_GET (new_regno), new_regno, loc);
  df_ref_change_reg_with_loc_1 (DF_REG_EQ_USE_GET (old_regno), 
				DF_REG_EQ_USE_GET (new_regno), new_regno, loc);
}


/* Delete the mw_hardregs that point into the eq_notes.  */

static void
df_mw_hardreg_chain_delete_eq_uses (struct df_insn_info *insn_info)
{
  struct df_scan_problem_data *problem_data;
  struct df_mw_hardreg *prev_hr = NULL;
  struct df_mw_hardreg *hardregs = insn_info->mw_hardregs;

  problem_data = (struct df_scan_problem_data *) df_scan->problem_data;

  while (hardregs)
    {
      struct df_mw_hardreg *next_hr = hardregs->next;
      struct df_link *link = hardregs->regs;
      struct df_ref *first_ref = link->ref;

      if (DF_REF_FLAGS_IS_SET (first_ref, DF_REF_IN_NOTE))
	{
	  while (link)
	    {
	      struct df_link *next_l = link->next;
	      pool_free (problem_data->mw_link_pool, link);
	      link = next_l;
	    }
      
	  pool_free (problem_data->mw_reg_pool, hardregs);
	  if (prev_hr)
	    prev_hr->next = next_hr;
	  else
	    insn_info->mw_hardregs = next_hr;
	}
      else
	prev_hr = hardregs;
      hardregs = next_hr;
    }
}


/* Rescan only the REG_EQUIV/REG_EQUAL notes part of INSN.  */

void
df_notes_rescan (rtx insn)
{
  struct df_insn_info *insn_info;
  if (!df)
    return;

  /* The client has disabled rescanning and plans to do it itself.  */
  if (df->changeable_flags & DF_NO_INSN_RESCAN)
    return;

  df_grow_bb_info (df_scan);
  df_grow_reg_info ();

  insn_info = DF_INSN_UID_SAFE_GET (INSN_UID(insn));

  /* The client has defered rescanning.  */
  if (df->changeable_flags & DF_DEFER_INSN_RESCAN)
    {
      if (!insn_info)
	insn_info = df_insn_create_insn_record (insn);
      
      bitmap_set_bit (df->insns_to_rescan, INSN_UID (insn));
      return;
    }

  if (insn_info)
    {
      struct df_ref dummy;
      struct df_ref *insn_refs = &dummy;
      basic_block bb = BLOCK_FOR_INSN (insn);
      rtx note;

      df_mw_hardreg_chain_delete_eq_uses (insn_info);
      df_ref_chain_delete (insn_info->eq_uses);
      insn_info->eq_uses = NULL;

      DF_REF_NEXT_REF (&dummy) = NULL;

      /* Process REG_EQUIV/REG_EQUAL notes */
      for (note = REG_NOTES (insn); note;
	   note = XEXP (note, 1))
	{
	  switch (REG_NOTE_KIND (note))
	    {
	    case REG_EQUIV:
	    case REG_EQUAL:
	      insn_refs = df_uses_record (insn_refs,
					  &XEXP (note, 0), DF_REF_REG_USE,
					  bb, insn, DF_REF_IN_NOTE);
	    default:
	      break;
	    }
	}
      df_refs_add_to_chains (insn, DF_REF_NEXT_REF (&dummy));
    }
  else
    df_insn_rescan (insn);

}

/* A convenience macro for chain append.  */
#define DF_REF_CHAIN_APPEND(last, item) (last = df_ref_chain_append ((last), (item)))


/*----------------------------------------------------------------------------
   Hard core instruction scanning code.  No external interfaces here,
   just a lot of routines that look inside insns.
----------------------------------------------------------------------------*/


/* Return the appropriate chains this_ref belongs to. */

static struct df_chains
df_ref_find_chains (struct df_ref *this_ref)
{
  struct df_chains chains;
  bool is_artificial = DF_REF_IS_ARTIFICIAL (this_ref);
  bool is_eq_use = DF_REF_FLAGS_IS_SET (this_ref, DF_REF_IN_NOTE);

  /* Find the appropriate ref_chain/reg_info/ref_info 
     that this ref belongs to. */
  switch (DF_REF_TYPE (this_ref)) 
    {
    case DF_REF_REG_DEF:
        chains.ref_chain = (is_artificial) 
            ? & df_scan_get_bb_info (DF_REF_BB (this_ref)->index)
            ->artificial_defs 
            : & DF_INSN_DEFS (DF_REF_INSN (this_ref));
        chains.reg_info = DF_REG_DEF_GET (DF_REF_REGNO (this_ref));
        chains.ref_info = & df->def_info;
        break;
    case DF_REF_REG_MEM_LOAD:
    case DF_REF_REG_MEM_STORE:
    case DF_REF_REG_USE:
        chains.ref_chain = (is_artificial) 
            ? & df_scan_get_bb_info (DF_REF_BB (this_ref)->index)
            ->artificial_uses 
            : (is_eq_use) 
	    ? & DF_INSN_EQ_USES (DF_REF_INSN (this_ref))
	    : & DF_INSN_USES (DF_REF_INSN (this_ref));
        chains.reg_info = (is_eq_use)
	  ? DF_REG_EQ_USE_GET (DF_REF_REGNO (this_ref))
	  : DF_REG_USE_GET (DF_REF_REGNO (this_ref));
        chains.ref_info = & df->use_info;
        break;
    default:
        gcc_unreachable ();
    }

  return chains;
}


/* Add the new df_ref to appropriate reg_info/ref_info chains.  
   It assumes that THIS_REF is already added to ref_chain.  */

static void
df_ref_add_to_chains (struct df_ref *this_ref,
                      struct df_mw_hardreg *hardreg)
{
  struct df_chains chains = df_ref_find_chains (this_ref);
  struct df_ref **ref_chain = chains.ref_chain;
  struct df_reg_info *reg_info = chains.reg_info;
  struct df_ref_info *ref_info = chains.ref_info;

  /* Don't allow duplicate refs to be in the ref chain. */
  if (df_ref_chain_find_ref (*ref_chain, this_ref, df_ref_is_equal))
    return;

  /* Add the ref to the head of the chains.  */
  DF_REF_NEXT_REF (this_ref) = *ref_chain;
  *ref_chain = this_ref;

  /* Add the ref to the reg_{def,use} chain.  */
  df_reg_chain_create (reg_info, this_ref);

  /* Add this_ref to the df->{use,def}_info */

  /* Do not add the notes in eq_refs unless they are asked for.  */
  if (ref_info->add_refs_inline
      && ((!DF_REF_FLAGS (this_ref) & DF_REF_IN_NOTE)
	  || ((DF_REF_FLAGS (this_ref) & DF_REF_IN_NOTE)
	      && (df->changeable_flags & DF_EQ_NOTES))))
    {
      df_check_and_grow_ref_info (ref_info, 1);
      DF_REF_ID (this_ref) = ref_info->table_size;
      /* Add the ref to the big array of defs.  */
      ref_info->refs[ref_info->table_size] = this_ref;
      ref_info->table_size++;
    }
  else
    DF_REF_ID (this_ref) = -1;

  ref_info->total_size++;
  ref_info->refs_organized_alone = false;
  ref_info->refs_organized_with_eq_uses = false;

  /* mw_hardreg has to be added to the hardreg chain as well. */
  if (DF_REF_INSN (this_ref) && DF_REF_FLAGS_IS_SET (this_ref, DF_REF_MW_HARDREG))
    {
      struct df_scan_problem_data *problem_data
        = (struct df_scan_problem_data *) df_scan->problem_data;
      struct df_link *link = pool_alloc (problem_data->mw_link_pool);

      gcc_assert (hardreg);

      link->next = hardreg->regs;
      link->ref = this_ref;
      hardreg->regs = link;
    }
}


/* Add a chain of df_refs to appropriate ref chain/reg_info/ref_info chains
   and update other necessary information */

static void
df_refs_add_to_chains (rtx insn,
                       struct df_ref *ref)
{
  struct df_mw_hardreg *hardreg = NULL;
  struct df_scan_problem_data *problem_data
    = (struct df_scan_problem_data *) df_scan->problem_data;

  while (ref)
    {
      struct df_ref *nref = DF_REF_NEXT_REF (ref);

      if (DF_REF_FLAGS_IS_SET (ref, DF_REF_MW_HARDREG_GROUP)) 
        {
          /* A beginning of a group of mw hardregs */
          struct df_insn_info *insn_info = DF_INSN_GET (insn);

#if 0
          hardreg = df_mw_hardreg_find_hardreg (insn_info->mw_hardregs, ref);
	  /* Matching hardreg group not found. Create one. */
#endif
	  hardreg = pool_alloc (problem_data->mw_reg_pool);
	  hardreg->next = insn_info->mw_hardregs;
	  insn_info->mw_hardregs = hardreg;
	  hardreg->type = DF_REF_TYPE (ref);
	  hardreg->flags = DF_REF_FLAGS (ref) & ~DF_REF_MW_HARDREG_GROUP;
	  hardreg->mw_reg = DF_REF_REG (ref);
	  hardreg->regs = NULL;

          /* MW_HARDREG_GROUP ref is just a placeholder, so free the memory. */
          pool_free (problem_data->ref_pool, ref);
        }
      else 
        {
	  /* All of the regs following a DF_REF_MW_HARDREG_GROUP
	     belong to the group until one occurs that does not have
	     DF_REF_MW_HARDREG set.  */
	  if (!DF_REF_FLAGS_IS_SET (ref, DF_REF_MW_HARDREG)) 
	    hardreg = NULL;
          DF_REF_NEXT_REF (ref) = NULL;
          df_ref_add_to_chains (ref, hardreg);
        }
      ref = nref;
    }
}


/* Allocate a ref and initialize its fields. */

static struct df_ref *
df_ref_create_structure (rtx reg, rtx *loc, 
			 basic_block bb, rtx insn, 
			 enum df_ref_type ref_type, 
			 enum df_ref_flags ref_flags)
{
  struct df_ref *this_ref;
  int regno = REGNO (GET_CODE (reg) == SUBREG ? SUBREG_REG (reg) : reg);
  struct df_scan_problem_data *problem_data
    = (struct df_scan_problem_data *) df_scan->problem_data;

  this_ref = pool_alloc (problem_data->ref_pool);
  DF_REF_ID (this_ref) = -1;
  DF_REF_REG (this_ref) = reg;
  DF_REF_REGNO (this_ref) =  regno;
  DF_REF_LOC (this_ref) = loc;
  DF_REF_INSN (this_ref) = insn;
  DF_REF_CHAIN (this_ref) = NULL;
  DF_REF_TYPE (this_ref) = ref_type;
  DF_REF_FLAGS (this_ref) = ref_flags;
  DF_REF_BB (this_ref) = bb;
  DF_REF_NEXT_REF (this_ref) = NULL;
  DF_REF_NEXT_REG (this_ref) = NULL;
  DF_REF_PREV_REG (this_ref) = NULL;
  
  return this_ref;
}


/* Create new references of type DF_REF_TYPE for each part of register REG
   at address LOC within INSN of BB.  */

static struct df_ref *
df_ref_record (struct df_ref *insn_refs,
               rtx reg, rtx *loc, 
	       basic_block bb, rtx insn, 
	       enum df_ref_type ref_type, 
	       enum df_ref_flags ref_flags) 
{
  rtx oldreg = reg;
  unsigned int regno;

  gcc_assert (REG_P (reg) || GET_CODE (reg) == SUBREG);

  regno = REGNO (GET_CODE (reg) == SUBREG ? SUBREG_REG (reg) : reg);
  if (regno < FIRST_PSEUDO_REGISTER)
    {
      unsigned int i;
      unsigned int endregno;
      struct df_ref *ref;

      if (GET_CODE (reg) == SUBREG)
	{
	  regno += subreg_regno_offset (regno, GET_MODE (SUBREG_REG (reg)),
					SUBREG_BYTE (reg), GET_MODE (reg));
	  endregno = subreg_nregs (reg);
	}
      else
	endregno = hard_regno_nregs[regno][GET_MODE (reg)];
      endregno += regno;

      /*  If this is a multiword hardreg, we create some extra
	  datastructures that will enable us to easily build REG_DEAD
	  and REG_UNUSED notes.  There is an implicit trick here, the
	  mw record must be inserted into the list right before the
	  members of the mw group.  */
      if ((endregno != regno + 1) && insn)
	{
	  /* Sets to a subreg of a multiword register are partial. 
	     Sets to a non-subreg of a multiword register are not.  */
	  if (GET_CODE (oldreg) == SUBREG)
	    ref_flags |= DF_REF_PARTIAL;
	  ref_flags |= DF_REF_MW_HARDREG;

          /* This is a placeholder for grouping mw_hardreg sequence. */
          ref = df_ref_create_structure (reg, loc,
                                         bb, insn, ref_type, 
                                         ref_flags | DF_REF_MW_HARDREG_GROUP);
          DF_REF_REGNO (ref) = regno;
          DF_REF_CHAIN_APPEND (insn_refs, ref);
	}

      for (i = regno; i < endregno; i++)
	{
	  ref = df_ref_create_structure (regno_reg_rtx[i], loc, 
					 bb, insn, ref_type, ref_flags);
          gcc_assert ( ORIGINAL_REGNO (DF_REF_REG (ref)) == i);
          DF_REF_CHAIN_APPEND (insn_refs, ref);
	}
    }
  else
    {
      struct df_ref *ref;
      ref = df_ref_create_structure (reg, loc, bb, insn, 
                                     ref_type, ref_flags);
      DF_REF_CHAIN_APPEND (insn_refs, ref);
    }

  return insn_refs;
}


/* A set to a non-paradoxical SUBREG for which the number of word_mode units
   covered by the outer mode is smaller than that covered by the inner mode,
   is a read-modify-write operation.
   This function returns true iff the SUBREG X is such a SUBREG.  */

bool
df_read_modify_subreg_p (rtx x)
{
  unsigned int isize, osize;
  if (GET_CODE (x) != SUBREG)
    return false;
  isize = GET_MODE_SIZE (GET_MODE (SUBREG_REG (x)));
  osize = GET_MODE_SIZE (GET_MODE (x));
  return (isize > osize && isize > UNITS_PER_WORD);
}


/* Process all the registers defined in the rtx, X.
   Autoincrement/decrement definitions will be picked up by
   df_uses_record.  */

static struct df_ref *
df_def_record_1 (struct df_ref *insn_refs,
                 rtx x, basic_block bb, rtx insn, 
		 enum df_ref_flags flags)
{
  rtx *loc;
  rtx dst;
  bool dst_in_strict_lowpart = false;

 /* We may recursively call ourselves on EXPR_LIST when dealing with PARALLEL
     construct.  */
  if (GET_CODE (x) == EXPR_LIST || GET_CODE (x) == CLOBBER)
    loc = &XEXP (x, 0);
  else
    loc = &SET_DEST (x);
  dst = *loc;

  /* It is legal to have a set destination be a parallel. */
  if (GET_CODE (dst) == PARALLEL)
    {
      int i;

      for (i = XVECLEN (dst, 0) - 1; i >= 0; i--)
	{
	  rtx temp = XVECEXP (dst, 0, i);
	  if (GET_CODE (temp) == EXPR_LIST || GET_CODE (temp) == CLOBBER
	      || GET_CODE (temp) == SET)
	    insn_refs = df_def_record_1 (insn_refs,
                             temp, bb, insn, 
			     GET_CODE (temp) == CLOBBER 
			     ? flags | DF_REF_MUST_CLOBBER : flags);
	}
      return insn_refs;
    }

  /* Maybe, we should flag the use of STRICT_LOW_PART somehow.  It might
     be handy for the reg allocator.  */
  while (GET_CODE (dst) == STRICT_LOW_PART
	 || GET_CODE (dst) == ZERO_EXTRACT
	 || df_read_modify_subreg_p (dst))
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
      if (GET_CODE (dst) == STRICT_LOW_PART)
	dst_in_strict_lowpart = true;
      dst = *loc;
      flags |= DF_REF_READ_WRITE;

    }

  /* Sets to a subreg of a single word register are partial sets if
     they are wrapped in a strict lowpart, and not partial otherwise.
  */
  if (GET_CODE (dst) == SUBREG && REG_P (SUBREG_REG (dst))
      && dst_in_strict_lowpart)
    flags |= DF_REF_PARTIAL;
    
  if (REG_P (dst)
      || (GET_CODE (dst) == SUBREG && REG_P (SUBREG_REG (dst))))
    insn_refs = df_ref_record (insn_refs, 
                   dst, loc, bb, insn, DF_REF_REG_DEF, flags);

  return insn_refs;
}


/* Process all the registers defined in the pattern rtx, X.  */

static struct df_ref *
df_defs_record (struct df_ref *insn_refs, 
                rtx x, basic_block bb, rtx insn, enum df_ref_flags flags)
{
  RTX_CODE code = GET_CODE (x);

  if (code == SET || code == CLOBBER)
    {
      /* Mark the single def within the pattern.  */
      enum df_ref_flags clobber_flags = flags;
      clobber_flags |= (code == CLOBBER) ? DF_REF_MUST_CLOBBER : 0;
      insn_refs = df_def_record_1 (insn_refs, 
                                   x, bb, insn, clobber_flags);
    }
  else if (code == COND_EXEC)
    {
      insn_refs = df_defs_record  (insn_refs, 
                                   COND_EXEC_CODE (x), 
                                   bb, insn, DF_REF_CONDITIONAL);
    }
  else if (code == PARALLEL)
    {
      int i;

      /* Mark the multiple defs within the pattern.  */
      for (i = XVECLEN (x, 0) - 1; i >= 0; i--)
	 insn_refs = df_defs_record (insn_refs, 
                                     XVECEXP (x, 0, i), 
                                     bb, insn, flags);
    }

  return insn_refs;
}


/* Process all the registers used in the rtx at address LOC.  */

static struct df_ref *
df_uses_record (struct df_ref *insn_refs,
                rtx *loc, enum df_ref_type ref_type,
		basic_block bb, rtx insn, enum df_ref_flags flags)
{
  RTX_CODE code;
  rtx x;
 retry:
  x = *loc;
  if (!x)
    return insn_refs;
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
      return insn_refs;

    case CLOBBER:
      /* If we are clobbering a MEM, mark any registers inside the address
	 as being used.  */
      if (MEM_P (XEXP (x, 0)))
	insn_refs = df_uses_record (insn_refs,
                                    &XEXP (XEXP (x, 0), 0),
			            DF_REF_REG_MEM_STORE, bb, insn, flags);

      /* If we're clobbering a REG then we have a def so ignore.  */
      return insn_refs;

    case MEM:
      return df_uses_record (insn_refs,
                             &XEXP (x, 0), DF_REF_REG_MEM_LOAD, 
                             bb, insn, flags & DF_REF_IN_NOTE);

    case SUBREG:
      /* While we're here, optimize this case.  */
      flags |= DF_REF_PARTIAL;
      /* In case the SUBREG is not of a REG, do not optimize.  */
      if (!REG_P (SUBREG_REG (x)))
	{
	  loc = &SUBREG_REG (x);
	  return df_uses_record (insn_refs, 
                                 loc, ref_type, bb, insn, flags);
	}
      /* ... Fall through ...  */

    case REG:
      return df_ref_record (insn_refs, 
                            x, loc, bb, insn, ref_type, flags);

    case SET:
      {
	rtx dst = SET_DEST (x);
	gcc_assert (!(flags & DF_REF_IN_NOTE));
	insn_refs = df_uses_record (insn_refs,
                                    &SET_SRC (x), DF_REF_REG_USE, 
                                    bb, insn, flags);

	switch (GET_CODE (dst))
	  {
	    case SUBREG:
	      if (df_read_modify_subreg_p (dst))
		{
		  insn_refs = df_uses_record (insn_refs,
                                              &SUBREG_REG (dst), 
				              DF_REF_REG_USE, bb, insn, 
                                              flags | DF_REF_READ_WRITE);
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
	      insn_refs = df_uses_record (insn_refs, 
                                          &XEXP (dst, 0),
			                  DF_REF_REG_MEM_STORE,
			                  bb, insn, flags);
	      break;
	    case STRICT_LOW_PART:
	      {
		rtx *temp = &XEXP (dst, 0);
		/* A strict_low_part uses the whole REG and not just the
		 SUBREG.  */
		dst = XEXP (dst, 0);
		insn_refs = df_uses_record (insn_refs,
				            (GET_CODE (dst) == SUBREG) 
				             ? &SUBREG_REG (dst) : temp, 
				            DF_REF_REG_USE, bb,
				            insn, DF_REF_READ_WRITE);
	      }
	      break;
	    case ZERO_EXTRACT:
	    case SIGN_EXTRACT:
	      insn_refs = df_uses_record (insn_refs,
                              &XEXP (dst, 0), 
			      DF_REF_REG_USE, bb, insn,
			      DF_REF_READ_WRITE);
	      insn_refs = df_uses_record (insn_refs,
                              &XEXP (dst, 1), 
			      DF_REF_REG_USE, bb, insn, flags);
	      insn_refs = df_uses_record (insn_refs,
                              &XEXP (dst, 2), 
			      DF_REF_REG_USE, bb, insn, flags);
	      dst = XEXP (dst, 0);
	      break;
	    default:
	      gcc_unreachable ();
	  }
	return insn_refs;
      }

    case RETURN:
      break;

    case ASM_OPERANDS:
    case UNSPEC_VOLATILE:
    case TRAP_IF:
    case ASM_INPUT:
      {
	/* Traditional and volatile asm instructions must be
	   considered to use and clobber all hard registers, all
	   pseudo-registers and all of memory.  So must TRAP_IF and
	   UNSPEC_VOLATILE operations.

	   Consider for instance a volatile asm that changes the fpu
	   rounding mode.  An insn should not be moved across this
	   even if it only uses pseudo-regs because it might give an
	   incorrectly rounded result.

	   However, flow.c's liveness computation did *not* do this,
	   giving the reasoning as " ?!? Unfortunately, marking all
	   hard registers as live causes massive problems for the
	   register allocator and marking all pseudos as live creates
	   mountains of uninitialized variable warnings."

	   In order to maintain the status quo with regard to liveness
	   and uses, we do what flow.c did and just mark any regs we
	   can find in ASM_OPERANDS as used.  Later on, when liveness
	   is computed, asm insns are scanned and regs_asm_clobbered
	   is filled out.  

	   For all ASM_OPERANDS, we must traverse the vector of input
	   operands.  We can not just fall through here since then we
	   would be confused by the ASM_INPUT rtx inside ASM_OPERANDS,
	   which do not indicate traditional asms unlike their normal
	   usage.  */
	if (code == ASM_OPERANDS)
	  {
	    int j;

	    for (j = 0; j < ASM_OPERANDS_INPUT_LENGTH (x); j++)
	      insn_refs = df_uses_record (insn_refs,
                                          &ASM_OPERANDS_INPUT (x, j),
			                  DF_REF_REG_USE, bb, insn, flags);
	    return insn_refs;
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
      flags |= DF_REF_READ_WRITE | DF_REF_PRE_POST_MODIFY;
      insn_refs = df_ref_record (insn_refs,
                                 XEXP (x, 0), &XEXP (x, 0), bb, insn, 
		                 DF_REF_REG_DEF, flags);

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
	    insn_refs = df_uses_record (insn_refs, 
                                        &XEXP (x, i), ref_type, bb, insn, flags);
	  }
	else if (fmt[i] == 'E')
	  {
	    int j;
	    for (j = 0; j < XVECLEN (x, i); j++)
	      insn_refs = df_uses_record (insn_refs,
                                          &XVECEXP (x, i, j), ref_type,
			                  bb, insn, flags);
	  }
      }
  }

  return insn_refs;
}

/* Return true if *LOC contains an asm.  */

static int
df_insn_contains_asm_1 (rtx *loc, void *data ATTRIBUTE_UNUSED)
{
  if ( !*loc)
    return 0;
  if (GET_CODE (*loc) == ASM_OPERANDS)
    return 1;
  return 0;
}


/* Return true if INSN contains an ASM.  */

static int
df_insn_contains_asm (rtx insn)
{
  return for_each_rtx (&insn, df_insn_contains_asm_1, NULL);
}



/* For all DF_REF_CONDITIONAL defs, add a corresponding uses.  */

static struct df_ref *
df_get_conditional_uses (struct df_ref *ref)
{
  struct df_ref dummy;
  struct df_ref *uses = &dummy;

  DF_REF_NEXT_REF (uses) = NULL;

  for (; ref; ref = DF_REF_NEXT_REF (ref))
    {
      if (DF_REF_FLAGS_IS_SET (ref, DF_REF_CONDITIONAL))
        {
          struct df_ref *use;
          enum df_ref_type t = DF_REF_TYPE (ref);
          switch (t)
            {
              case DF_REF_REG_DEF:
                t = DF_REF_REG_USE;
                break;
              case DF_REF_REG_MEM_STORE:
                t = DF_REF_REG_MEM_LOAD;
                break;
              default:
                /* Ignore non-defs. */
                continue;
            }
          use = df_ref_create_structure (DF_REF_REG (ref),
                                         DF_REF_LOC (ref),
                                         DF_REF_BB (ref),
                                         DF_REF_INSN (ref),
                                         t,
                                         DF_REF_FLAGS (ref) & ~DF_REF_CONDITIONAL);
          DF_REF_REGNO (use) = DF_REF_REGNO (ref);
          DF_REF_CHAIN_APPEND (uses, use);
        }
    }

  if (DF_REF_NEXT_REF (&dummy) == NULL) 
    return NULL;


  DF_REF_NEXT_REF (uses) = DF_REF_NEXT_REF (&dummy);
  return uses;
}


/* Get call's extra defs and uses. */

static struct df_ref *
df_get_call_refs (struct df_ref *insn_refs, 
                  basic_block bb, 
                  rtx insn,
                  enum df_ref_flags flags)
{
  rtx note;
  bitmap_iterator bi;
  unsigned int ui;
  bool is_sibling_call;
  unsigned int i;

  /* Record the registers used to pass arguments, and explicitly
     noted as clobbered.  */
  for (note = CALL_INSN_FUNCTION_USAGE (insn); note;
       note = XEXP (note, 1))
    {
      if (GET_CODE (XEXP (note, 0)) == USE)
        insn_refs = df_uses_record (insn_refs,
                                    &XEXP (XEXP (note, 0), 0),
                                    DF_REF_REG_USE,
                                    bb, insn, flags);
      else if (GET_CODE (XEXP (note, 0)) == CLOBBER)
        insn_refs = df_defs_record (insn_refs,
                                    XEXP (note, 0), bb, insn, flags);
    }

  /* The stack ptr is used (honorarily) by a CALL insn.  */
  insn_refs = df_ref_record (insn_refs,
                             regno_reg_rtx[STACK_POINTER_REGNUM],
                             NULL,
                             bb, insn, 
                             DF_REF_REG_USE, 
                             DF_REF_CALL_STACK_USAGE | flags);

  /* Calls may also reference any of the global registers,
     so they are recorded as used.  */
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (global_regs[i])
      insn_refs = df_ref_record (insn_refs,
                                 regno_reg_rtx[i],
                                 NULL,
                                 bb, insn, 
                                 DF_REF_REG_USE, 
                                 flags);

  is_sibling_call = SIBLING_CALL_P (insn);
  EXECUTE_IF_SET_IN_BITMAP (df_invalidated_by_call, 0, ui, bi)
    {
      if (!is_sibling_call
          || !bitmap_bit_p (df->exit_block_uses, ui)
          || refers_to_regno_p (ui, ui+1, 
                                current_function_return_rtx, NULL))
        insn_refs = df_ref_record (insn_refs,
                                   regno_reg_rtx[ui], 
                                   NULL,
                                   bb, insn, 
                                   DF_REF_REG_DEF, 
                                   DF_REF_MAY_CLOBBER | flags);
    }

  return insn_refs;
}

/* Collect all refs in the INSN. 
   This function is free of any side-effect - 
   it will create and return a list of df_ref's (chained through next_ref) 
   without putting those refs into existing ref chains and reg chains. */

static struct df_ref *
df_insn_refs_collect (basic_block bb, rtx insn) 
{
  struct df_ref dummy;
  struct df_ref *insn_refs = &dummy;
  rtx note;
  bool is_cond_exec = (GET_CODE (PATTERN (insn)) == COND_EXEC);

  DF_REF_NEXT_REF (insn_refs) = NULL;
  
  /* Record register defs.  */
  insn_refs = df_defs_record (insn_refs, PATTERN (insn), bb, insn, 0);

  /* Process REG_EQUIV/REG_EQUAL notes */
  for (note = REG_NOTES (insn); note;
       note = XEXP (note, 1))
    {
      switch (REG_NOTE_KIND (note))
        {
        case REG_EQUIV:
        case REG_EQUAL:
          insn_refs = df_uses_record (insn_refs,
                                      &XEXP (note, 0), DF_REF_REG_USE,
                                      bb, insn, DF_REF_IN_NOTE);
        default:
          break;
        }
    }

  if (CALL_P (insn))
    insn_refs = df_get_call_refs (insn_refs, bb, insn, 
                                  (is_cond_exec) ? DF_REF_CONDITIONAL : 0);

  /* Record the register uses.  */
  insn_refs = df_uses_record (insn_refs,
                              &PATTERN (insn), DF_REF_REG_USE, bb, insn, 0);

  /* DF_REF_CONDITIONAL needs corresponding USES. */
  if (is_cond_exec)
    {
      struct df_ref *cond_uses;
      cond_uses = df_get_conditional_uses (DF_REF_NEXT_REF (&dummy));
      if (cond_uses) 
	{
	  DF_REF_NEXT_REF (insn_refs) = DF_REF_NEXT_REF (cond_uses);
	  DF_REF_NEXT_REF (cond_uses) = NULL;
	  insn_refs = cond_uses;
	}
    }

  return DF_REF_NEXT_REF (&dummy);
}


/* Record all the refs for DF within INSN of basic block BB.  */

void
df_insn_refs_record (rtx insn, struct df_ref *insn_refs)
{
  gcc_assert (INSN_P (insn));

  /* Process the collected insn_refs chain here */
  df_refs_add_to_chains (insn, insn_refs);

  if (df_insn_contains_asm (insn))
    DF_INSN_CONTAINS_ASM (insn) = true;
}


/* Return true if any pred of BB is an eh.  */

bool
df_has_eh_preds (basic_block bb)
{
  edge e;
  edge_iterator ei;

  FOR_EACH_EDGE (e, ei, bb->preds)
    {
      if (e->flags & EDGE_EH)
	return true;
    }
  return false;
}


/* Recompute the luids for the insns in BB.  */

void
df_recompute_luids (basic_block bb)
{
  rtx insn;
  int luid = 0;

  df_grow_insn_info ();

  /* Scan the block an insn at a time from beginning to end.  */
  FOR_BB_INSNS (bb, insn)
    {
      struct df_insn_info *insn_info = DF_INSN_GET (insn);
      /* Inserting labels does not always trigger the incremental
	 rescanning.  */
      if (!insn_info)
	df_insn_create_insn_record (insn);

      if (INSN_P (insn))
	DF_INSN_LUID (insn) = luid++;
      DF_INSN_LUID (insn) = luid;
    }
}


/* Returns true if the function entry needs to 
   define the static chain register.  */

static bool
df_need_static_chain_reg (struct function *fun)
{
  tree fun_context = decl_function_context (fun->decl);
  return fun_context
         && DECL_NO_STATIC_CHAIN (fun_context) == false;
}


/* Collect all artificial refs at the block level
   and return them as a newly created ref chain.  */

static struct df_ref *
df_bb_refs_collect (basic_block bb)
{
  struct df_ref dummy;
  struct df_ref *bb_refs = &dummy;

  DF_REF_NEXT_REF (bb_refs) = NULL;

  if (bb->index == ENTRY_BLOCK)
    return df_entry_block_defs_collect (df->entry_block_defs);
  else if (bb->index == EXIT_BLOCK)
    return df_exit_block_uses_collect (df->exit_block_uses);

#ifdef EH_RETURN_DATA_REGNO
  if (df_has_eh_preds (bb))
    {
      unsigned int i;
      /* Mark the registers that will contain data for the handler.  */
      for (i = 0; ; ++i)
	{
	  unsigned regno = EH_RETURN_DATA_REGNO (i);
	  if (regno == INVALID_REGNUM)
	    break;
	  bb_refs = df_ref_record (bb_refs,
                                   regno_reg_rtx[regno], NULL,
			           bb, NULL,
			           DF_REF_REG_DEF, DF_REF_AT_TOP);
	}
    }
#endif


#ifdef EH_USES
  if (df_has_eh_preds (bb))
    {
      unsigned int i;
      /* This code is putting in a artificial ref for the use at the
	 TOP of the block that receives the exception.  It is too
	 cumbersome to actually put the ref on the edge.  We could
	 either model this at the top of the receiver block or the
	 bottom of the sender block.

         The bottom of the sender block is problematic because not all
         out-edges of the a block are eh-edges.  However, it is true
         that all edges into a block are either eh-edges or none of
         them are eh-edges.  Thus, we can model this at the top of the
         eh-receiver for all of the edges at once. */
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	if (EH_USES (i))
	  bb_refs = df_ref_record (bb_refs,
				   regno_reg_rtx[i], NULL,
				   bb, NULL, DF_REF_REG_USE, 
				   DF_REF_AT_TOP);
    }
#endif

  if (bb->index >= NUM_FIXED_BLOCKS)
    {
      bitmap_iterator bi;
      unsigned int regno;
      bitmap au = df_has_eh_preds (bb) 
	? df->eh_block_artificial_uses 
	: df->regular_block_artificial_uses;

      EXECUTE_IF_SET_IN_BITMAP (au, 0, regno, bi)
	{
	  bb_refs = df_ref_record (bb_refs,
				   regno_reg_rtx[regno], NULL,
				   bb, NULL, DF_REF_REG_USE, 0);
	}
    }

  return DF_REF_NEXT_REF (&dummy);
}


/* Record all the refs within the basic block BB_INDEX and scan the instructions if SCAN_INSNS.  */

void
df_bb_refs_record (int bb_index, bool scan_insns)
{
  basic_block bb = BASIC_BLOCK (bb_index);
  rtx insn;
  int luid = 0;
  struct df_ref *bb_refs;
  struct df_scan_bb_info *bb_info;

  if (!df)
    return;

  bb_info = df_scan_get_bb_info (bb_index);

  /* Need to make sure that there is a record in the basic block info. */  
  if (!bb_info)
    {
      bb_info = (struct df_scan_bb_info *) pool_alloc (df_scan->block_pool);
      df_scan_set_bb_info (bb_index, bb_info);
      bb_info->artificial_defs = NULL;
      bb_info->artificial_uses = NULL;
    }

  if (scan_insns)
    /* Scan the block an insn at a time from beginning to end.  */
    FOR_BB_INSNS (bb, insn)
      {
	struct df_insn_info *insn_info = DF_INSN_GET (insn);
	gcc_assert (!insn_info);

	df_insn_create_insn_record (insn);
	if (INSN_P (insn))
	  {
	    /* Record refs within INSN.  */
	    DF_INSN_LUID (insn) = luid++;
	    df_insn_refs_record (insn, df_insn_refs_collect (bb, insn));
	  }
	DF_INSN_LUID (insn) = luid;
      }

  /* Other block level artificial refs */
  bb_refs = df_bb_refs_collect (bb);

  if (bb_refs)
    df_refs_add_to_chains (NULL, bb_refs);
}


/* Get the artificial use set for 
   a regular (i.e. non-exit/non-entry) block. */

static void
df_get_regular_block_artificial_uses (bitmap regular_block_artificial_uses)
{
  bitmap_clear (regular_block_artificial_uses);

  /* Before reload, there are a few registers that must be forced
     live everywhere -- which might not already be the case for
     blocks within infinite loops.  */
  if (!reload_completed)
    {
      /* Any reference to any pseudo before reload is a potential
	 reference of the frame pointer.  */
      bitmap_set_bit (regular_block_artificial_uses, FRAME_POINTER_REGNUM);
      
#if FRAME_POINTER_REGNUM != ARG_POINTER_REGNUM
      /* Pseudos with argument area equivalences may require
	 reloading via the argument pointer.  */
      if (fixed_regs[ARG_POINTER_REGNUM])
	bitmap_set_bit (regular_block_artificial_uses, ARG_POINTER_REGNUM);
#endif
      
      /* Any constant, or pseudo with constant equivalences, may
	 require reloading from memory using the pic register.  */
      if ((unsigned) PIC_OFFSET_TABLE_REGNUM != INVALID_REGNUM
	  && fixed_regs[PIC_OFFSET_TABLE_REGNUM])
	bitmap_set_bit (regular_block_artificial_uses, PIC_OFFSET_TABLE_REGNUM);
    }
  /* The all-important stack pointer must always be live.  */
  bitmap_set_bit (regular_block_artificial_uses, STACK_POINTER_REGNUM);
}


/* Get the artificial use set for an eh block. */

static void
df_get_eh_block_artificial_uses (bitmap eh_block_artificial_uses)
{
  bitmap_clear (eh_block_artificial_uses);

  /* The following code (down thru the arg_pointer seting APPEARS
     to be necessary because there is nothing that actually
     describes what the exception handling code may actually need
     to keep alive.  */
  if (reload_completed)
    {
      if (frame_pointer_needed)
	{
	  bitmap_set_bit (eh_block_artificial_uses, FRAME_POINTER_REGNUM);
#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
	  bitmap_set_bit (eh_block_artificial_uses, HARD_FRAME_POINTER_REGNUM);
#endif
	}
#if FRAME_POINTER_REGNUM != ARG_POINTER_REGNUM
      if (fixed_regs[ARG_POINTER_REGNUM])
	bitmap_set_bit (eh_block_artificial_uses, ARG_POINTER_REGNUM);
#endif
    }
}



/*----------------------------------------------------------------------------
   Specialized hard register scanning functions.
----------------------------------------------------------------------------*/


/* Mark a register in SET.  Hard registers in large modes get all
   of their component registers set as well.  */

static void
df_mark_reg (rtx reg, void *vset)
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




/* Set the bit for regs that are considered being defined at the entry. */

static void
df_get_entry_block_def_set (bitmap entry_block_defs)
{
  rtx r;
  int i;

  bitmap_clear (entry_block_defs);

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (FUNCTION_ARG_REGNO_P (i))
#ifdef INCOMING_REGNO
	bitmap_set_bit (entry_block_defs, INCOMING_REGNO (i));
#else
	bitmap_set_bit (entry_block_defs, i);
#endif
    }
      
  /* Once the prologue has been generated, all of these registers
     should just show up in the first regular block.  */
  if (HAVE_prologue && epilogue_completed)
    {
      /* Defs for the callee saved registers are inserted so that the
	 pushes have some defining location.  */
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	if ((call_used_regs[i] == 0) && (regs_ever_live[i]))
	  bitmap_set_bit (entry_block_defs, i);
    }
  else
    {
      /* The always important stack pointer.  */
      bitmap_set_bit (entry_block_defs, STACK_POINTER_REGNUM);

#ifdef INCOMING_RETURN_ADDR_RTX
      if (REG_P (INCOMING_RETURN_ADDR_RTX))
	bitmap_set_bit (entry_block_defs, REGNO (INCOMING_RETURN_ADDR_RTX));
#endif
            
      /* If STATIC_CHAIN_INCOMING_REGNUM == STATIC_CHAIN_REGNUM
	 only STATIC_CHAIN_REGNUM is defined.  If they are different,
	 we only care about the STATIC_CHAIN_INCOMING_REGNUM.  */
#ifdef STATIC_CHAIN_INCOMING_REGNUM
      bitmap_set_bit (entry_block_defs, STATIC_CHAIN_INCOMING_REGNUM);
#else 
#ifdef STATIC_CHAIN_REGNUM
      bitmap_set_bit (entry_block_defs, STATIC_CHAIN_REGNUM);
#endif
#endif
      
      r = TARGET_STRUCT_VALUE_RTX (current_function_decl, true);
      if (r && REG_P (r))
	bitmap_set_bit (entry_block_defs, REGNO (r));
    }

  if ((!reload_completed) || frame_pointer_needed)
    {
      /* Any reference to any pseudo before reload is a potential
	 reference of the frame pointer.  */
      bitmap_set_bit (entry_block_defs, FRAME_POINTER_REGNUM);
#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
      /* If they are different, also mark the hard frame pointer as live.  */
      if (!LOCAL_REGNO (HARD_FRAME_POINTER_REGNUM))
	bitmap_set_bit (entry_block_defs, HARD_FRAME_POINTER_REGNUM);
#endif
    }

  /* These registers are live everywhere.  */
  if (!reload_completed)
    {
#ifdef EH_USES
      /* The ia-64, the only machine that uses this, does not define these 
	 until after reload.  */
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	if (EH_USES (i))
	  {
	    bitmap_set_bit (entry_block_defs, i);
	  }
#endif
      
#if FRAME_POINTER_REGNUM != ARG_POINTER_REGNUM
      /* Pseudos with argument area equivalences may require
	 reloading via the argument pointer.  */
      if (fixed_regs[ARG_POINTER_REGNUM])
	bitmap_set_bit (entry_block_defs, ARG_POINTER_REGNUM);
#endif
	  
#ifdef PIC_OFFSET_TABLE_REGNUM
      /* Any constant, or pseudo with constant equivalences, may
	 require reloading from memory using the pic register.  */
      if ((unsigned) PIC_OFFSET_TABLE_REGNUM != INVALID_REGNUM
	  && fixed_regs[PIC_OFFSET_TABLE_REGNUM])
	bitmap_set_bit (entry_block_defs, PIC_OFFSET_TABLE_REGNUM);
#endif
    }

  targetm.live_on_entry (entry_block_defs);

  /* If the function has an incoming STATIC_CHAIN,
     it has to show up in the entry def set.  */
  if (df_need_static_chain_reg (cfun))
    {
#if !defined (STATIC_CHAIN_INCOMING_REGNUM) \
      || STATIC_CHAIN_REGNUM == STATIC_CHAIN_INCOMING_REGNUM
      bitmap_set_bit (entry_block_defs, STATIC_CHAIN_REGNUM);
#else 
      bitmap_set_bit (entry_block_defs, STATIC_CHAIN_INCOMING_REGNUM);
#endif
    }
}


/* Return the (conservative) set of hard registers that are defined on
   entry to the function.  
   It uses df->entry_block_defs to determine which regster 
   reference to include.  */

static struct df_ref *
df_entry_block_defs_collect (bitmap entry_block_defs)
{
  unsigned int i; 
  bitmap_iterator bi;
  struct df_ref dummy;
  struct df_ref *bb_refs = &dummy;

  DF_REF_NEXT_REF (bb_refs) = NULL;

  EXECUTE_IF_SET_IN_BITMAP (entry_block_defs, 0, i, bi)
    {
      bb_refs = df_ref_record (bb_refs, 
                               regno_reg_rtx[i], NULL, 
                               ENTRY_BLOCK_PTR, NULL, 
                               DF_REF_REG_DEF, 0);
    }

  return DF_REF_NEXT_REF (&dummy);
}


/* Record the (conservative) set of hard registers that are defined on
   entry to the function.  */

static void
df_record_entry_block_defs (bitmap entry_block_defs)
{
  struct df_ref *refs = df_entry_block_defs_collect (entry_block_defs);

  /* Process bb_refs chain */
  df_refs_add_to_chains (NULL, refs);
}


/* Update the defs in the entry bolck.  */

void
df_update_entry_block_defs (void)
{
  bitmap refs = BITMAP_ALLOC (NULL);

  df_get_entry_block_def_set (refs);
  if (df->entry_block_defs)
    {
      bitmap_iterator bi;
      bitmap diff = BITMAP_ALLOC (NULL);
      unsigned int regno;

      bitmap_and_compl (diff, df->entry_block_defs, refs);
      EXECUTE_IF_SET_IN_BITMAP (diff, 0, regno, bi)
	{
	  struct df_ref *ref 
	    = df_ref_chain_find_ref_by_regno (df_get_artificial_defs (ENTRY_BLOCK), 
					      regno);
	  gcc_assert (ref);
	  df_ref_remove (ref);
	}

      bitmap_and_compl (diff, refs, df->entry_block_defs);
      df_record_entry_block_defs (diff);
      BITMAP_FREE (diff);
    }
  else
    {
      df->entry_block_defs = BITMAP_ALLOC (NULL);
      df_record_entry_block_defs (refs);
    }

  bitmap_copy (df->entry_block_defs, refs);
  BITMAP_FREE (refs);
}


/* Set the bit for regs that are considered being used at the exit. */

static void
df_get_exit_block_use_set (bitmap exit_block_uses)
{
  unsigned int i; 

  bitmap_clear (exit_block_uses);
  
  /* If exiting needs the right stack value, consider the stack
     pointer live at the end of the function.  */
  if ((HAVE_epilogue && epilogue_completed)
      || !EXIT_IGNORE_STACK
      || (!FRAME_POINTER_REQUIRED
	  && !current_function_calls_alloca
	  && flag_omit_frame_pointer)
      || current_function_sp_is_unchanging)
    {
      bitmap_set_bit (exit_block_uses, STACK_POINTER_REGNUM);
    }
  
  /* Mark the frame pointer if needed at the end of the function.
     If we end up eliminating it, it will be removed from the live
     list of each basic block by reload.  */
  
  if ((!reload_completed) || frame_pointer_needed)
    {
      bitmap_set_bit (exit_block_uses, FRAME_POINTER_REGNUM);
#if FRAME_POINTER_REGNUM != HARD_FRAME_POINTER_REGNUM
      /* If they are different, also mark the hard frame pointer as live.  */
      if (!LOCAL_REGNO (HARD_FRAME_POINTER_REGNUM))
	bitmap_set_bit (exit_block_uses, HARD_FRAME_POINTER_REGNUM);
#endif
    }

#ifndef PIC_OFFSET_TABLE_REG_CALL_CLOBBERED
  /* Many architectures have a GP register even without flag_pic.
     Assume the pic register is not in use, or will be handled by
     other means, if it is not fixed.  */
  if ((unsigned) PIC_OFFSET_TABLE_REGNUM != INVALID_REGNUM
      && fixed_regs[PIC_OFFSET_TABLE_REGNUM])
    bitmap_set_bit (exit_block_uses, PIC_OFFSET_TABLE_REGNUM);
#endif
  
  /* Mark all global registers, and all registers used by the
     epilogue as being live at the end of the function since they
     may be referenced by our caller.  */
  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (global_regs[i] || EPILOGUE_USES (i))
      bitmap_set_bit (exit_block_uses, i);
  
  if (HAVE_epilogue && epilogue_completed)
    {
      /* Mark all call-saved registers that we actually used.  */
      for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
	if (regs_ever_live[i] && !LOCAL_REGNO (i)
	    && !TEST_HARD_REG_BIT (regs_invalidated_by_call, i))
	  bitmap_set_bit (exit_block_uses, i);
    }
  
#ifdef EH_RETURN_DATA_REGNO
  /* Mark the registers that will contain data for the handler.  */
  if (reload_completed && current_function_calls_eh_return)
    for (i = 0; ; ++i)
      {
	unsigned regno = EH_RETURN_DATA_REGNO (i);
	if (regno == INVALID_REGNUM)
	  break;
	bitmap_set_bit (exit_block_uses, regno);
      }
#endif

#ifdef EH_RETURN_STACKADJ_RTX
  if ((!HAVE_epilogue || ! epilogue_completed)
      && current_function_calls_eh_return)
    {
      rtx tmp = EH_RETURN_STACKADJ_RTX;
      if (tmp && REG_P (tmp))
	df_mark_reg (tmp, exit_block_uses);
    }
#endif

#ifdef EH_RETURN_HANDLER_RTX
  if ((!HAVE_epilogue || ! epilogue_completed)
      && current_function_calls_eh_return)
    {
      rtx tmp = EH_RETURN_HANDLER_RTX;
      if (tmp && REG_P (tmp))
	df_mark_reg (tmp, exit_block_uses);
    }
#endif 

  /* Mark function return value.  */
  diddle_return_value (df_mark_reg, (void*) exit_block_uses);
}


/* Return the refs of hard registers that are used in the exit block.  
   It uses df->exit_block_uses to determine register to include.  */

static struct df_ref *
df_exit_block_uses_collect (bitmap exit_block_uses)
{
  unsigned int i; 
  bitmap_iterator bi;
  struct df_ref dummy;
  struct df_ref *bb_refs = &dummy;

  DF_REF_NEXT_REF (bb_refs) = NULL;

  EXECUTE_IF_SET_IN_BITMAP (exit_block_uses, 0, i, bi)
    bb_refs = df_ref_record (bb_refs, regno_reg_rtx[i], NULL,
			     EXIT_BLOCK_PTR, NULL, DF_REF_REG_USE,
			     0);

#if FRAME_POINTER_REGNUM != ARG_POINTER_REGNUM
  /* It is deliberate that this is not put in the exit block uses but
     I do not know why.  */
  if (reload_completed 
      && !bitmap_bit_p (exit_block_uses, ARG_POINTER_REGNUM)
      && df_has_eh_preds (EXIT_BLOCK_PTR)
      && fixed_regs[ARG_POINTER_REGNUM])
    bb_refs = df_ref_record (bb_refs,
			     regno_reg_rtx[ARG_POINTER_REGNUM], NULL,
			     EXIT_BLOCK_PTR, NULL, DF_REF_REG_USE, 
			     0);
#endif

  return DF_REF_NEXT_REF (&dummy);
}


/* Record the set of hard registers that are used in the exit block.  
   It uses df->exit_block_uses to determine which bit to include.  */

static void
df_record_exit_block_uses (bitmap exit_block_uses)
{
  struct df_ref *refs = df_exit_block_uses_collect (exit_block_uses);
  /* Process bb_refs chain */
  df_refs_add_to_chains (NULL, refs);
}


/* Update the uses in the exit block.  */

void
df_update_exit_block_uses (void)
{
  bitmap refs = BITMAP_ALLOC (NULL);

  df_get_exit_block_use_set (refs);
  if (df->exit_block_uses)
    {
      bitmap_iterator bi;
      bitmap diff = BITMAP_ALLOC (NULL);
      unsigned int regno;

      bitmap_and_compl (diff, df->exit_block_uses, refs);
      EXECUTE_IF_SET_IN_BITMAP (diff, 0, regno, bi)
	{
	  struct df_ref *ref 
	    = df_ref_chain_find_ref_by_regno (df_get_artificial_uses (EXIT_BLOCK), 
					      regno);
	  gcc_assert (ref);
	  df_ref_remove (ref);
	}

      bitmap_and_compl (diff, refs, df->exit_block_uses);
      df_record_exit_block_uses (diff);
      BITMAP_FREE (diff);
    }
  else
    {
      df->exit_block_uses = BITMAP_ALLOC (NULL);
      df_record_exit_block_uses (refs);
    }

  bitmap_copy (df->exit_block_uses, refs);
  BITMAP_FREE (refs);
}

static bool initialized = false;


/* Initialize some platform specific structures.  */

void 
df_hard_reg_init (void)
{
  int i;
#ifdef ELIMINABLE_REGS
  static const struct {const int from, to; } eliminables[] = ELIMINABLE_REGS;
#endif
  if (initialized)
    return;

  bitmap_obstack_initialize (&persistent_obstack);

  /* Record which registers will be eliminated.  We use this in
     mark_used_regs.  */
  CLEAR_HARD_REG_SET (elim_reg_set);
  
#ifdef ELIMINABLE_REGS
  for (i = 0; i < (int) ARRAY_SIZE (eliminables); i++)
    SET_HARD_REG_BIT (elim_reg_set, eliminables[i].from);
#else
  SET_HARD_REG_BIT (elim_reg_set, FRAME_POINTER_REGNUM);
#endif
  
  df_invalidated_by_call = BITMAP_ALLOC (&persistent_obstack);
  
  /* Inconveniently, this is only readily available in hard reg set
     form.  */
  for (i = 0; i < FIRST_PSEUDO_REGISTER; ++i)
    if (TEST_HARD_REG_BIT (regs_invalidated_by_call, i))
      bitmap_set_bit (df_invalidated_by_call, i);
  
  initialized = true;
}


/* Recompute the parts of scanning that are based on regs_ever_live
   because something changed in that array.  */ 

void 
df_update_entry_exit_and_calls (void)
{
  basic_block bb;

  df_update_entry_block_defs ();
  df_update_exit_block_uses ();

  /* The call insns need to be rescanned because there may be changes
     in the set of registers clobbered across the call.  */
  FOR_EACH_BB (bb) 
    {
      rtx insn;
      FOR_BB_INSNS (bb, insn)
	{
	  if (INSN_P (insn) && CALL_P (insn))
	    df_insn_rescan (insn);
	}
    }
}


/* Return true if hard reg I is actually used in the some
   instruction.  */

bool 
df_hard_reg_used_p (unsigned int i)
{
  gcc_assert (df);

  return (df_reg_chain_find_ref (DF_REG_DEF_CHAIN (i), NULL,
				 df_ref_is_record_live) != NULL
	  || df_reg_chain_find_ref (DF_REG_USE_CHAIN (i), NULL,
				    df_ref_is_record_live) != NULL
	  || df_reg_chain_find_ref (DF_REG_EQ_USE_CHAIN (i), NULL,
				    df_ref_is_record_live) != NULL);
}


/* Compute "regs_ever_live" information.  */

void
df_compute_regs_ever_live (void)
{
  unsigned int i;
  bool changed = false;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (df_hard_reg_used_p (i) & (regs_ever_live[i] == 0))
      {
	regs_ever_live[i] = 1;
	changed = true;
      }
  if (changed)
    df_update_entry_exit_and_calls ();
}


/*----------------------------------------------------------------------------
  Dataflow ref information verification functions.

  df_reg_chain_mark (refs, regno, is_def, is_eq_use)
  df_ref_chain_verify_and_clear_marks (refs, mask)
  df_ref_verify (ref, bool)
  df_ref_chain_mark_duplicate (src_refs, dest_refs)
  df_ref_chain_free (refs)
  df_insn_refs_verify (bb, insn, refs, bool)
  df_bb_refs_verify (bb, refs, bool)
  df_bb_verify (bb)
  df_exit_block_bitmap_verify (bool)
  df_entry_block_bitmap_verify (bool)
  df_verify_blocks ()
----------------------------------------------------------------------------*/


/* Mark all refs in the reg chain.  Verify that all of the registers
are in the correct chain. */ 

static unsigned int
df_reg_chain_mark (struct df_ref *refs, unsigned int regno, 
		   bool is_def, bool is_eq_use)
{
  unsigned int count = 0;
  struct df_ref *ref;
  for (ref = refs; ref; ref = DF_REF_NEXT_REG (ref))
    {
      gcc_assert (!DF_REF_IS_REF_MARKED (ref));
      gcc_assert (!DF_REF_IS_REG_MARKED (ref));

      /* If there are no def-use or use-def chains, make sure that all
	 of the chains are clear.  */
      if (!df_chain)
	gcc_assert (!DF_REF_CHAIN (ref));

      /* Check to make sure the ref is in the correct chain.  */
      gcc_assert (DF_REF_REGNO (ref) == regno);
      if (is_def)
	gcc_assert (DF_REF_TYPE(ref) == DF_REF_REG_DEF);
      else
	gcc_assert (DF_REF_TYPE(ref) != DF_REF_REG_DEF);

      if (is_eq_use)
	gcc_assert ((DF_REF_FLAGS (ref) & DF_REF_IN_NOTE));
      else
	gcc_assert ((DF_REF_FLAGS (ref) & DF_REF_IN_NOTE) == 0);

      if (ref->next_reg)
	gcc_assert (ref->next_reg->prev_reg == ref);
      count++;
      DF_REF_REG_MARK (ref);
    }
  return count;
}


/* Unmark all refs in the ref chain 
   and return true if all refs are DF_REF_IS_MARKED. */

static bool
df_ref_chain_verify_and_unmark (struct df_ref *refs, unsigned int mask, bool abort_if_fail)
{
  bool fail = false;
  struct df_ref *ref;
  for (ref = refs; ref; ref = DF_REF_NEXT_REF (ref))
    {
      if ((DF_REF_FLAGS (ref) & (DF_REF_REF_MARKER + DF_REF_REG_MARKER)) != mask)
	{
	  if (abort_if_fail)
	    gcc_assert (0);
	  fail = true;
	}
      DF_REF_FLAGS_CLEAR (ref, DF_REF_REF_MARKER + DF_REF_REG_MARKER);
    }
  return !fail;
}


/* Free all refs in the chain.  */
static void
df_ref_chain_free (struct df_ref *ref)
{
  struct df_scan_problem_data *problem_data
    = (struct df_scan_problem_data *) df_scan->problem_data;

  while (ref)
    {
      struct df_ref *next_ref = DF_REF_NEXT_REF (ref);
      pool_free (problem_data->ref_pool, ref);
      ref = next_ref;
    }
}


/* Find the matching identical ref in the existing ref chain,
   and return it if it is in various other chains/places that 
   it's supposed to be in. Otherwise return NULL.
   Mark the original ref with DF_REF_MARK() if it was found. */

static struct df_ref *
df_ref_verify (struct df_ref *this_ref,
	       bool abort_if_fail)
{
  struct df_chains chains = df_ref_find_chains (this_ref);
  struct df_ref **ref_chain = chains.ref_chain;
  struct df_ref_info *ref_info = chains.ref_info;
  struct df_ref *old_ref;

  /* Verify the ref chain.  */
  old_ref = df_ref_chain_find_ref (*ref_chain, this_ref, df_ref_is_equal);
  if (!old_ref)
    {
      if (abort_if_fail)
	gcc_assert (0);
      else
	return NULL;
    }

  /* Collected ref chain may contain duplicate entries,
     in which case the reference is already verified. */
  if (DF_REF_IS_REF_MARKED (old_ref))
    return old_ref;
  DF_REF_REF_MARK (old_ref);

  /* Abort_if_fail is used for the whole function verifier. In that
     mode we want to see that this ref is in a reg chain exactly once.
     All entries in the reg chains were marked at the start of verification.  
     Now we clear them one at a time.  */
  if (abort_if_fail)
    {
      gcc_assert (DF_REF_IS_REG_MARKED (old_ref));
      DF_REF_REG_UNMARK (old_ref);
    }

  /* Verify ref_info->refs array.  */
  if ((DF_REF_ID (old_ref) >= 0)
      && (ref_info->refs[DF_REF_ID (old_ref)] != old_ref))
    {
      if (abort_if_fail)
	gcc_assert (0);
      else
	return NULL;
    }

  return old_ref;
}


/* Find the hardreg that contains ref.  If checkref is true, the list
   of refs is actually checked to make sure that ref is really in
   it.  */

static struct df_mw_hardreg *
df_mw_hardreg_find_hardreg (struct df_mw_hardreg *hardreg, 
			    struct df_ref *ref)
{
  enum df_ref_flags flags = DF_REF_FLAGS (ref);
  flags &= ~(DF_REF_REF_MARKER + DF_REF_REG_MARKER);
 
  for (; hardreg; hardreg = hardreg->next)
    {
      if (hardreg->type == DF_REF_TYPE (ref) 
	  && ((hardreg->flags & ~(DF_REF_REF_MARKER + DF_REF_REG_MARKER)) == flags))
	{
	  /* There could be several hardregs that look the same but
	     have different regs inside.  */
	  struct df_link * link = hardreg->regs;
	  while (link)
	    {
	      if (df_ref_is_equal (link->ref, ref))
		return hardreg;
	      link = link->next;
	    }
	}
    }
  return NULL;
}


/* Return true if the existing insn refs information is complete and
   correct. Otherwise (i.e. if there's any missing or extra refs),
   return the correct df_ref chain in REFS_RETURN.  

   If ABORT_IF_FAIL, leave the refs that are verified (already in the
   ref chain) as DF_REF_MARKED(). If it's false, then it's a per-insn
   verification mode instead of the whole function, so unmark
   everything.

   If ABORT_IF_FAIL is set, this function never returns false.  */

static bool
df_insn_refs_verify (basic_block bb, 
                     rtx insn,
		     struct df_ref **refs_return,
		     bool abort_if_fail)
{
  struct df_ref *refs = df_insn_refs_collect (bb, insn);
  struct df_ref *ref;
  bool ret1, ret2, ret3;
  struct df_mw_hardreg *hardreg = NULL;
 
  for (ref = refs; ref; ref = DF_REF_NEXT_REF (ref))
    {
      struct df_ref *oref; 
      /* Just skip the place group place holder.  */
      if (DF_REF_FLAGS_IS_SET (ref, DF_REF_MW_HARDREG_GROUP))
	continue;

      /* If the register is in a hardware group look up that group to
	 pass it to the ref verifier.  */
      if (DF_REF_FLAGS_IS_SET (ref, DF_REF_MW_HARDREG))
        {
          hardreg = df_mw_hardreg_find_hardreg (DF_INSN_GET (insn)->mw_hardregs, ref);
          if (hardreg == NULL)
	    {
	      if (abort_if_fail)
		gcc_assert (0);
	      else 
		{
		  *refs_return = refs;
		  return false;
		}
	    }
	  hardreg->flags |= DF_REF_REF_MARKER;
    	}

        oref = df_ref_verify (ref, abort_if_fail);
	if (oref == NULL)
	  {
	    *refs_return = refs;
	    return false;
	  }
    }

  /* Verify that all of the hardregs are actually being used.  */
  hardreg = DF_INSN_GET (insn)->mw_hardregs;
  while (hardreg)
    {
      if (!(hardreg->flags & DF_REF_REF_MARKER))
	{
	  if (abort_if_fail)
	    gcc_assert (0);
	  else
	    {
	      *refs_return = refs;
	      return false;
	    }
	}
	    
      hardreg->flags &= ~DF_REF_REF_MARKER;
      hardreg = hardreg->next;
    }

  ret1 = df_ref_chain_verify_and_unmark (DF_INSN_DEFS (insn), 
					 DF_REF_REF_MARKER, abort_if_fail);
  ret2 = df_ref_chain_verify_and_unmark (DF_INSN_USES (insn), 
					 DF_REF_REF_MARKER, abort_if_fail);
  ret3 = df_ref_chain_verify_and_unmark (DF_INSN_EQ_USES (insn), 
					 DF_REF_REF_MARKER, abort_if_fail);

  if (!ret1 || !ret2 || !ret3)
    {
      *refs_return = refs;
      return false;
    }

  *refs_return = NULL;
  df_ref_chain_free (refs);
  return true;
}


/* Return true if all refs in the basic block are correct and complete.
   Due to df_ref_chain_verify, it will cause all refs
   that are verified to have DF_REF_MARK bit set.  */

static bool
df_bb_verify (basic_block bb)
{
  rtx insn;
  struct df_scan_bb_info *bb_info = df_scan_get_bb_info (bb->index);
  struct df_ref *refs;
  struct df_ref *ref;

  gcc_assert (bb_info);

  /* Scan the block an insn at a time from beginning to end.  */
  FOR_BB_INSNS_REVERSE (bb, insn)
    {
      if (!INSN_P (insn))
        continue;
      df_insn_refs_verify (bb, insn, &refs, true);
    }

  /* Do the artificial defs and uses.  */
  refs = df_bb_refs_collect (bb);

  for (ref = refs; ref; ref = DF_REF_NEXT_REF (ref))
    df_ref_verify (ref, true);

  df_ref_chain_verify_and_unmark (df_get_artificial_defs (bb->index), 
				  DF_REF_REF_MARKER, true);
  df_ref_chain_verify_and_unmark (df_get_artificial_uses (bb->index), 
				  DF_REF_REF_MARKER, true);

  df_ref_chain_free (refs);
  return true;
}


/* Returns true if the entry block has correct and complete df_ref set.  
   If not it either aborts if ABORT_IF_FAIL is true or returns false.  */

static bool
df_entry_block_bitmap_verify (bool abort_if_fail)
{
  bitmap entry_block_defs = BITMAP_ALLOC (NULL);
  bool is_eq;

  df_get_entry_block_def_set (entry_block_defs);

  is_eq = bitmap_equal_p (entry_block_defs, df->entry_block_defs);

  if (!is_eq && abort_if_fail)
    {
      print_current_pass (stderr);
      fprintf (stderr, "entry_block_defs = ");
      df_print_regset (stderr, entry_block_defs);
      fprintf (stderr, "df->entry_block_defs = ");
      df_print_regset (stderr, df->entry_block_defs);
      gcc_assert (0);
    }

  BITMAP_FREE (entry_block_defs);

  return is_eq;
}


/* Returns true if the exit block has correct and complete df_ref set.  
   If not it either aborts if ABORT_IF_FAIL is true or returns false. */

static bool
df_exit_block_bitmap_verify (bool abort_if_fail)
{
  bitmap exit_block_uses = BITMAP_ALLOC (NULL);
  bool is_eq;

  df_get_exit_block_use_set (exit_block_uses);

  is_eq = bitmap_equal_p (exit_block_uses, df->exit_block_uses);

  if (!is_eq && abort_if_fail)
    {
      print_current_pass (stderr);
      fprintf (stderr, "exit_block_uses = ");
      df_print_regset (stderr, exit_block_uses);
      fprintf (stderr, "df->exit_block_uses = ");
      df_print_regset (stderr, df->exit_block_uses);
      gcc_assert (0);
    }

  BITMAP_FREE (exit_block_uses);

  return is_eq;
}


/* Return true if df_ref information for all insns in all BLOCKS are
   correct and complete.  If BLOCKS is null, all blocks are
   checked.  */

void
df_verify_blocks (void)
{
  unsigned int i;
  basic_block bb;
  bitmap regular_block_artificial_uses;
  bitmap eh_block_artificial_uses;

  if (!df)
    return;

  /* This is a hack, but a necessary one.  If you do not do this,
     insn_attrtab can never be compiled in a bootstrap.  This
     verification is just too expensive.  */
  if (n_basic_blocks > 250)
    return;

  for (i = 0; i < DF_REG_SIZE (df); i++)
    {
      gcc_assert (df_reg_chain_mark (DF_REG_DEF_CHAIN (i), i, true, false) 
		  == DF_REG_DEF_COUNT(i));
      gcc_assert (df_reg_chain_mark (DF_REG_USE_CHAIN (i), i, false, false) 
		  == DF_REG_USE_COUNT(i));
      gcc_assert (df_reg_chain_mark (DF_REG_EQ_USE_CHAIN (i), i, false, true) 
		  == DF_REG_EQ_USE_COUNT(i));
    }

  regular_block_artificial_uses = BITMAP_ALLOC (NULL);
  eh_block_artificial_uses = BITMAP_ALLOC (NULL);

  df_get_regular_block_artificial_uses (regular_block_artificial_uses);
  df_get_eh_block_artificial_uses (eh_block_artificial_uses);

  bitmap_ior_into (eh_block_artificial_uses, 
		   regular_block_artificial_uses);

  /* Check artificial_uses bitmaps didn't change. */
  gcc_assert (bitmap_equal_p (regular_block_artificial_uses, 
			      df->regular_block_artificial_uses));
  gcc_assert (bitmap_equal_p (eh_block_artificial_uses, 
			      df->eh_block_artificial_uses));

  BITMAP_FREE (regular_block_artificial_uses);
  BITMAP_FREE (eh_block_artificial_uses);

  /* Verify entry block and exit block. */
  df_entry_block_bitmap_verify (true);
  df_exit_block_bitmap_verify (true);
    
  FOR_ALL_BB (bb)
    {
      df_bb_verify (bb);
    }

  /* See if all reg chains are verified,
     and also clear the DF_REF_MARKED bit. */

  for (i = 0; i < DF_REG_SIZE (df); i++)
    {
      df_ref_chain_verify_and_unmark (DF_REG_DEF_CHAIN (i), 0, true);
      df_ref_chain_verify_and_unmark (DF_REG_USE_CHAIN (i), 0, true);
      df_ref_chain_verify_and_unmark (DF_REG_EQ_USE_CHAIN (i), 0, true);
    }
}
