/* Global common subexpression elimination/Partial redundancy elimination
   and global constant/copy propagation for GNU compiler.
   Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002, 2003
   Free Software Foundation, Inc.

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

/* TODO
   - reordering of memory allocation and freeing to be more space efficient
   - do rough calc of how many regs are needed in each block, and a rough
     calc of how many regs are available in each class and use that to
     throttle back the code in cases where RTX_COST is minimal.
   - a store to the same address as a load does not kill the load if the
     source of the store is also the destination of the load.  Handling this
     allows more load motion, particularly out of loops.
   - ability to realloc sbitmap vectors would allow one initial computation
     of reg_set_in_block with only subsequent additions, rather than
     recomputing it for each pass

*/

/* References searched while implementing this.

   Compilers Principles, Techniques and Tools
   Aho, Sethi, Ullman
   Addison-Wesley, 1988

   Global Optimization by Suppression of Partial Redundancies
   E. Morel, C. Renvoise
   communications of the acm, Vol. 22, Num. 2, Feb. 1979

   A Portable Machine-Independent Global Optimizer - Design and Measurements
   Frederick Chow
   Stanford Ph.D. thesis, Dec. 1983

   A Fast Algorithm for Code Movement Optimization
   D.M. Dhamdhere
   SIGPLAN Notices, Vol. 23, Num. 10, Oct. 1988

   A Solution to a Problem with Morel and Renvoise's
   Global Optimization by Suppression of Partial Redundancies
   K-H Drechsler, M.P. Stadel
   ACM TOPLAS, Vol. 10, Num. 4, Oct. 1988

   Practical Adaptation of the Global Optimization
   Algorithm of Morel and Renvoise
   D.M. Dhamdhere
   ACM TOPLAS, Vol. 13, Num. 2. Apr. 1991

   Efficiently Computing Static Single Assignment Form and the Control
   Dependence Graph
   R. Cytron, J. Ferrante, B.K. Rosen, M.N. Wegman, and F.K. Zadeck
   ACM TOPLAS, Vol. 13, Num. 4, Oct. 1991

   Lazy Code Motion
   J. Knoop, O. Ruthing, B. Steffen
   ACM SIGPLAN Notices Vol. 27, Num. 7, Jul. 1992, '92 Conference on PLDI

   What's In a Region?  Or Computing Control Dependence Regions in Near-Linear
   Time for Reducible Flow Control
   Thomas Ball
   ACM Letters on Programming Languages and Systems,
   Vol. 2, Num. 1-4, Mar-Dec 1993

   An Efficient Representation for Sparse Sets
   Preston Briggs, Linda Torczon
   ACM Letters on Programming Languages and Systems,
   Vol. 2, Num. 1-4, Mar-Dec 1993

   A Variation of Knoop, Ruthing, and Steffen's Lazy Code Motion
   K-H Drechsler, M.P. Stadel
   ACM SIGPLAN Notices, Vol. 28, Num. 5, May 1993

   Partial Dead Code Elimination
   J. Knoop, O. Ruthing, B. Steffen
   ACM SIGPLAN Notices, Vol. 29, Num. 6, Jun. 1994

   Effective Partial Redundancy Elimination
   P. Briggs, K.D. Cooper
   ACM SIGPLAN Notices, Vol. 29, Num. 6, Jun. 1994

   The Program Structure Tree: Computing Control Regions in Linear Time
   R. Johnson, D. Pearson, K. Pingali
   ACM SIGPLAN Notices, Vol. 29, Num. 6, Jun. 1994

   Optimal Code Motion: Theory and Practice
   J. Knoop, O. Ruthing, B. Steffen
   ACM TOPLAS, Vol. 16, Num. 4, Jul. 1994

   The power of assignment motion
   J. Knoop, O. Ruthing, B. Steffen
   ACM SIGPLAN Notices Vol. 30, Num. 6, Jun. 1995, '95 Conference on PLDI

   Global code motion / global value numbering
   C. Click
   ACM SIGPLAN Notices Vol. 30, Num. 6, Jun. 1995, '95 Conference on PLDI

   Value Driven Redundancy Elimination
   L.T. Simpson
   Rice University Ph.D. thesis, Apr. 1996

   Value Numbering
   L.T. Simpson
   Massively Scalar Compiler Project, Rice University, Sep. 1996

   High Performance Compilers for Parallel Computing
   Michael Wolfe
   Addison-Wesley, 1996

   Advanced Compiler Design and Implementation
   Steven Muchnick
   Morgan Kaufmann, 1997

   Building an Optimizing Compiler
   Robert Morgan
   Digital Press, 1998

   People wishing to speed up the code here should read:
     Elimination Algorithms for Data Flow Analysis
     B.G. Ryder, M.C. Paull
     ACM Computing Surveys, Vol. 18, Num. 3, Sep. 1986

     How to Analyze Large Programs Efficiently and Informatively
     D.M. Dhamdhere, B.K. Rosen, F.K. Zadeck
     ACM SIGPLAN Notices Vol. 27, Num. 7, Jul. 1992, '92 Conference on PLDI

   People wishing to do something different can find various possibilities
   in the above papers and elsewhere.
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "toplev.h"

#include "rtl.h"
#include "tm_p.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "flags.h"
#include "real.h"
#include "insn-config.h"
#include "recog.h"
#include "basic-block.h"
#include "output.h"
#include "function.h"
#include "expr.h"
#include "except.h"
#include "ggc.h"
#include "params.h"
#include "cselib.h"

#include "obstack.h"

#include "gcse-globals.h"
#include "gcse-utils.h"

/* Propagate flow information through back edges and thus enable PRE's
   moving loop invariant calculations out of loops.

   Originally this tended to create worse overall code, but several
   improvements during the development of PRE seem to have made following
   back edges generally a win.

   Note much of the loop invariant code motion done here would normally
   be done by loop.c, which has more heuristics for when to move invariants
   out of loops.  At some point we might need to move some of those
   heuristics into gcse.c.  */

/* We support GCSE via Partial Redundancy Elimination.  PRE optimizations
   are a superset of those done by GCSE.

   We perform the following steps:

   1) Compute basic block information.

   2) Compute table of places where registers are set.

   3) Perform copy/constant propagation.

   4) Perform global cse.

   5) Perform another pass of copy/constant propagation.

   Two passes of copy/constant propagation are done because the first one
   enables more GCSE and the second one helps to clean up the copies that
   GCSE creates.  This is needed more for PRE than for Classic because Classic
   GCSE will try to use an existing register containing the common
   subexpression rather than create a new one.  This is harder to do for PRE
   because of the code motion (which Classic GCSE doesn't do).

   Expressions we are interested in GCSE-ing are of the form
   (set (pseudo-reg) (expression)).
   Function want_to_gcse_p says what these are.

   PRE handles moving invariant expressions out of loops (by treating them as
   partially redundant).

   Eventually it would be nice to replace cse.c/gcse.c with SSA (static single
   assignment) based GVN (global value numbering).  L. T. Simpson's paper
   (Rice University) on value numbering is a useful reference for this.

   **********************

   We used to support multiple passes but there are diminishing returns in
   doing so.  The first pass usually makes 90% of the changes that are doable.
   A second pass can make a few more changes made possible by the first pass.
   Experiments show any further passes don't make enough changes to justify
   the expense.

   A study of spec92 using an unlimited number of passes:
   [1 pass] = 1208 substitutions, [2] = 577, [3] = 202, [4] = 192, [5] = 83,
   [6] = 34, [7] = 17, [8] = 9, [9] = 4, [10] = 4, [11] = 2,
   [12] = 2, [13] = 1, [15] = 1, [16] = 2, [41] = 1

   It was found doing copy propagation between each pass enables further
   substitutions.

   PRE is quite expensive in complicated functions because the DFA can take
   awhile to converge.  Hence we only perform one pass.  The parameter max-gcse-passes can
   be modified if one wants to experiment.

   **********************

   The steps for PRE are:

   1) Build the hash table of expressions we wish to GCSE (expr_hash_table).

   2) Perform the data flow analysis for PRE.

   3) Delete the redundant instructions

   4) Insert the required copies [if any] that make the partially
      redundant instructions fully redundant.

   5) For other reaching expressions, insert an instruction to copy the value
      to a newly created pseudo that will reach the redundant instruction.

   The deletion is done first so that when we do insertions we
   know which pseudo reg to use.

   Various papers have argued that PRE DFA is expensive (O(n^2)) and others
   argue it is not.  The number of iterations for the algorithm to converge
   is typically 2-4 so I don't view it as that expensive (relatively speaking).

   PRE GCSE depends heavily on the second CSE pass to clean up the copies
   we create.  To make an expression reach the place where it's redundant,
   the result of the expression is copied to a new register, and the redundant
   expression is deleted by replacing it with this new register.  Classic GCSE
   doesn't have this problem as much as it computes the reaching defs of
   each register in each block and thus can try to use an existing register.

   **********************

   A fair bit of simplicity is created by creating small functions for simple
   tasks, even when the function is only called in one place.  This may
   measurably slow things down [or may not] by creating more function call
   overhead than is necessary.  The source is laid out so that it's trivial
   to make the affected functions inline so that one can measure what speed
   up, if any, can be achieved, and maybe later when things settle things can
   be rearranged.

   Help stamp out big monolithic functions!  */

/* GCSE global vars.  */

/* -dG dump file.  */
FILE *gcse_file;

/* Bitmaps are normally not included in debugging dumps.
   However it's useful to be able to print them from GDB.
   We could create special functions for this, but it's simpler to
   just allow passing stderr to the dump_foo fns.  Since stderr can
   be a macro, we store a copy here.  */
FILE *debug_stderr;

/* An obstack for our working variables.  */
struct obstack gcse_obstack;

/* Mapping of uids to cuids.
   Only real insns get cuids.  */
int *uid_cuid;

/* Highest UID in UID_CUID.  */
int max_uid;

/* Number of cuids.  */
int max_cuid;

/* Mapping of cuids to insns.  */
rtx *cuid_insn;

/* Maximum register number in function prior to doing gcse + 1.
   Registers created during this pass have regno >= max_gcse_regno.
   This is named with "gcse" to not collide with global of same name.  */
unsigned int max_gcse_regno;

/* Table of registers that are modified.  */
struct reg_set_table reg_set_table;

/* This is a list of expressions which are MEMs and will be used by load
   or store motion.
   Load motion tracks MEMs which aren't killed by
   anything except itself. (ie, loads and stores to a single location).
   We can then allow movement of these MEM refs with a little special
   allowance. (all stores copy the same value to the reaching reg used
   for the loads).  This means all values used to store into memory must have
   no side effects so we can re-issue the setter value.
   Store Motion uses this structure as an expression table to track stores
   which look interesting, and might be moveable towards the exit block.  */

/* Head of the list of load/store memory refs.  */
struct ls_expr * pre_ldst_mems = NULL;

/* Bitmap containing one bit for each register in the program.
   Used when performing GCSE to track which registers have been set since
   the start of the basic block.  */
regset reg_set_bitmap;

/* For each block, a bitmap of registers set in the block.
   This is used by expr_killed_p and compute_transp.
   It is computed during hash table computation and not by compute_sets
   as it includes registers added since the last pass (or between cprop and
   gcse) and it's currently not easy to realloc sbitmap vectors.  */
sbitmap *reg_set_in_block;

/* Array, indexed by basic block number for a list of insns which modify
   memory within that block.  */
rtx * modify_mem_list;
bitmap modify_mem_list_set;

/* This array parallels modify_mem_list, but is kept canonicalized.  */
rtx * canon_modify_mem_list;
bitmap canon_modify_mem_list_set;

/* Various variables for statistics gathering.  */

/* Memory used in a pass.
   This isn't intended to be absolutely precise.  Its intent is only
   to keep an eye on memory usage.  */
int bytes_used;

/* GCSE substitutions made.  */
int gcse_subst_count;
/* Number of copy instructions created.  */
int gcse_create_count;
/* Number of constants propagated.  */
int const_prop_count;
/* Number of copys propagated.  */
int copy_prop_count;

static void alloc_gcse_mem	PARAMS ((rtx));
static void free_gcse_mem	PARAMS ((void));

/* Entry point for global common subexpression elimination.
   F is the first instruction in the function.  */

int
gcse_main (f, file)
     rtx f;
     FILE *file;
{
  int changed, pass;
  /* Bytes used at start of pass.  */
  int initial_bytes_used;
  /* Maximum number of bytes used by a pass.  */
  int max_pass_bytes;
  /* Point to release obstack data from for each pass.  */
  char *gcse_obstack_bottom;

  /* Note whether or not we should run jump optimization after gcse.  We
     want to do this for two cases.

      * If we changed any jumps via cprop.

      * If we added any labels via edge splitting.  */
  int run_jump_opt_after_gcse;

  /* We do not construct an accurate cfg in functions which call
     setjmp, so just punt to be safe.  */
  if (current_function_calls_setjmp)
    return 0;

  /* Assume that we do not need to run jump optimizations after gcse.  */
  run_jump_opt_after_gcse = 0;

  /* For calling dump_foo fns from gdb.  */
  debug_stderr = stderr;
  gcse_file = file;

  /* Identify the basic block information for this function, including
     successors and predecessors.  */
  max_gcse_regno = max_reg_num ();

  if (file)
    dump_flow_info (file);

  /* Return if there's nothing to do.  */
  if (n_basic_blocks <= 1)
    return 0;

  /* Trying to perform global optimizations on flow graphs which have
     a high connectivity will take a long time and is unlikely to be
     particularly useful.

     In normal circumstances a cfg should have about twice as many edges
     as blocks.  But we do not want to punish small functions which have
     a couple switch statements.  So we require a relatively large number
     of basic blocks and the ratio of edges to blocks to be high.  */
  if (n_basic_blocks > 1000 && n_edges / n_basic_blocks >= 20)
    {
      if (warn_disabled_optimization)
	warning ("GCSE disabled: %d > 1000 basic blocks and %d >= 20 edges/basic block",
		 n_basic_blocks, n_edges / n_basic_blocks);
      return 0;
    }

  /* If allocating memory for the cprop bitmap would take up too much
     storage it's better just to disable the optimization.  */
  if ((n_basic_blocks
       * SBITMAP_SET_SIZE (max_gcse_regno)
       * sizeof (SBITMAP_ELT_TYPE)) > MAX_GCSE_MEMORY)
    {
      if (warn_disabled_optimization)
	warning ("GCSE disabled: %d basic blocks and %d registers",
		 n_basic_blocks, max_gcse_regno);

      return 0;
    }

  gcc_obstack_init (&gcse_obstack);
  bytes_used = 0;

  /* We need alias.  */
  init_alias_analysis ();
  /* Record where pseudo-registers are set.  This data is kept accurate
     during each pass.  ??? We could also record hard-reg information here
     [since it's unchanging], however it is currently done during hash table
     computation.

     It may be tempting to compute MEM set information here too, but MEM sets
     will be subject to code motion one day and thus we need to compute
     information about memory sets when we build the hash tables.  */

  alloc_reg_set_mem (max_gcse_regno);
  compute_sets (f);

  pass = 0;
  initial_bytes_used = bytes_used;
  max_pass_bytes = 0;
  gcse_obstack_bottom = gcse_alloc (1);
  changed = 1;
  while (changed && pass < MAX_GCSE_PASSES)
    {
      changed = 0;
      if (file)
	fprintf (file, "GCSE pass %d\n\n", pass + 1);

      /* Initialize bytes_used to the space for the pred/succ lists,
	 and the reg_set_table data.  */
      bytes_used = initial_bytes_used;

      /* Each pass may create new registers, so recalculate each time.  */
      max_gcse_regno = max_reg_num ();

      alloc_gcse_mem (f);

      /* Don't allow constant propagation to modify jumps
	 during this pass.  */
      changed = one_cprop_pass (pass + 1, 0, &run_jump_opt_after_gcse, 0);

      if (optimize_size)
	changed |= one_classic_gcse_pass (pass + 1);
      else
	{
	  changed |= one_pre_gcse_pass (pass + 1);
	  /* We may have just created new basic blocks.  Release and
	     recompute various things which are sized on the number of
	     basic blocks.  */
	  if (changed)
	    {
	      free_modify_mem_tables ();
	      modify_mem_list
		= (rtx *) gmalloc (last_basic_block * sizeof (rtx));
	      canon_modify_mem_list
		= (rtx *) gmalloc (last_basic_block * sizeof (rtx));
	      memset ((char *) modify_mem_list, 0, last_basic_block * sizeof (rtx));
	      memset ((char *) canon_modify_mem_list, 0, last_basic_block * sizeof (rtx));
	    }
	  free_reg_set_mem ();
	  alloc_reg_set_mem (max_reg_num ());
	  compute_sets (f);
	  run_jump_opt_after_gcse = 1;
	}

      if (max_pass_bytes < bytes_used)
	max_pass_bytes = bytes_used;

      /* Free up memory, then reallocate for code hoisting.  We can
	 not re-use the existing allocated memory because the tables
	 will not have info for the insns or registers created by
	 partial redundancy elimination.  */
      free_gcse_mem ();

      /* It does not make sense to run code hoisting unless we optimizing
	 for code size -- it rarely makes programs faster, and can make
	 them bigger if we did partial redundancy elimination (when optimizing
	 for space, we use a classic gcse algorithm instead of partial
	 redundancy algorithms).  */
      if (optimize_size)
	{
	  max_gcse_regno = max_reg_num ();
	  alloc_gcse_mem (f);
	  changed |= one_code_hoisting_pass ();
	  free_gcse_mem ();

	  if (max_pass_bytes < bytes_used)
	    max_pass_bytes = bytes_used;
	}

      if (file)
	{
	  fprintf (file, "\n");
	  fflush (file);
	}

      obstack_free (&gcse_obstack, gcse_obstack_bottom);
      pass++;
    }

  /* Do one last pass of copy propagation, including cprop into
     conditional jumps.  */

  max_gcse_regno = max_reg_num ();
  alloc_gcse_mem (f);
  /* This time, go ahead and allow cprop to alter jumps.  */
  one_cprop_pass (pass + 1, 1, &run_jump_opt_after_gcse, 0);
  free_gcse_mem ();

  if (file)
    {
      fprintf (file, "GCSE of %s: %d basic blocks, ",
	       current_function_name, n_basic_blocks);
      fprintf (file, "%d pass%s, %d bytes\n\n",
	       pass, pass > 1 ? "es" : "", max_pass_bytes);
    }

  obstack_free (&gcse_obstack, NULL);
  free_reg_set_mem ();
  /* We are finished with alias.  */
  end_alias_analysis ();
  allocate_reg_info (max_reg_num (), FALSE, FALSE);

  /* Store motion disabled until it is fixed.  */
  if (!optimize_size && flag_gcse_sm)
    store_motion ();
  /* Record where pseudo-registers are set.  */
  return run_jump_opt_after_gcse;
}

/* Misc. utilities.  */

/* Allocate memory for the cuid mapping array,
   and reg/memory set tracking tables.

   This is called at the start of each pass.  */

static void
alloc_gcse_mem (f)
     rtx f;
{
  int i, n;
  rtx insn;

  /* Find the largest UID and create a mapping from UIDs to CUIDs.
     CUIDs are like UIDs except they increase monotonically, have no gaps,
     and only apply to real insns.  */

  max_uid = get_max_uid ();
  n = (max_uid + 1) * sizeof (int);
  uid_cuid = (int *) gmalloc (n);
  memset ((char *) uid_cuid, 0, n);
  for (insn = f, i = 0; insn; insn = NEXT_INSN (insn))
    {
      if (INSN_P (insn))
	uid_cuid[INSN_UID (insn)] = i++;
      else
	uid_cuid[INSN_UID (insn)] = i;
    }

  /* Create a table mapping cuids to insns.  */

  max_cuid = i;
  n = (max_cuid + 1) * sizeof (rtx);
  cuid_insn = (rtx *) gmalloc (n);
  memset ((char *) cuid_insn, 0, n);
  for (insn = f, i = 0; insn; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      CUID_INSN (i++) = insn;

  /* Allocate vars to track sets of regs.  */
  reg_set_bitmap = BITMAP_XMALLOC ();

  /* Allocate vars to track sets of regs, memory per block.  */
  reg_set_in_block = (sbitmap *) sbitmap_vector_alloc (last_basic_block,
						       max_gcse_regno);
  /* Allocate array to keep a list of insns which modify memory in each
     basic block.  */
  modify_mem_list = (rtx *) gmalloc (last_basic_block * sizeof (rtx));
  canon_modify_mem_list = (rtx *) gmalloc (last_basic_block * sizeof (rtx));
  memset ((char *) modify_mem_list, 0, last_basic_block * sizeof (rtx));
  memset ((char *) canon_modify_mem_list, 0, last_basic_block * sizeof (rtx));
  modify_mem_list_set = BITMAP_XMALLOC ();
  canon_modify_mem_list_set = BITMAP_XMALLOC ();
}

/* Free memory allocated by alloc_gcse_mem.  */

static void
free_gcse_mem ()
{
  free (uid_cuid);
  free (cuid_insn);

  BITMAP_XFREE (reg_set_bitmap);

  sbitmap_vector_free (reg_set_in_block);
  free_modify_mem_tables ();
  BITMAP_XFREE (modify_mem_list_set);
  BITMAP_XFREE (canon_modify_mem_list_set);
}

/* Scan the function and record each set of each pseudo-register.

   This is called once, at the start of the gcse pass.  See the comments for
   `reg_set_table' for further documentation.  */

void
compute_sets (f)
     rtx f;
{
  rtx insn;

  for (insn = f; insn != 0; insn = NEXT_INSN (insn))
    if (INSN_P (insn))
      note_stores (PATTERN (insn), record_set_info, insn);
}

/* Entry point for jump bypassing optimization pass.  */

int
bypass_jumps (file)
     FILE *file;
{
  int changed;
  int jump_changed;

  /* We do not construct an accurate cfg in functions which call
     setjmp, so just punt to be safe.  */
  if (current_function_calls_setjmp)
    return 0;

  /* For calling dump_foo fns from gdb.  */
  debug_stderr = stderr;
  gcse_file = file;

  /* Identify the basic block information for this function, including
     successors and predecessors.  */
  max_gcse_regno = max_reg_num ();

  if (file)
    dump_flow_info (file);

  /* Return if there's nothing to do.  */
  if (n_basic_blocks <= 1)
    return 0;

  /* Trying to perform global optimizations on flow graphs which have
     a high connectivity will take a long time and is unlikely to be
     particularly useful.

     In normal circumstances a cfg should have about twice as many edges
     as blocks.  But we do not want to punish small functions which have
     a couple switch statements.  So we require a relatively large number
     of basic blocks and the ratio of edges to blocks to be high.  */
  if (n_basic_blocks > 1000 && n_edges / n_basic_blocks >= 20)
    {
      if (warn_disabled_optimization)
        warning ("BYPASS disabled: %d > 1000 basic blocks and %d >= 20 edges/basic block",
                 n_basic_blocks, n_edges / n_basic_blocks);
      return 0;
    }

  /* If allocating memory for the cprop bitmap would take up too much
     storage it's better just to disable the optimization.  */
  if ((n_basic_blocks
       * SBITMAP_SET_SIZE (max_gcse_regno)
       * sizeof (SBITMAP_ELT_TYPE)) > MAX_GCSE_MEMORY)
    {
      if (warn_disabled_optimization)
        warning ("GCSE disabled: %d basic blocks and %d registers",
                 n_basic_blocks, max_gcse_regno);

      return 0;
    }


  gcc_obstack_init (&gcse_obstack);
  bytes_used = 0;

  /* We need alias.  */
  init_alias_analysis ();

  /* Record where pseudo-registers are set.  This data is kept accurate
     during each pass.  ??? We could also record hard-reg information here
     [since it's unchanging], however it is currently done during hash table
     computation.

     It may be tempting to compute MEM set information here too, but MEM sets
     will be subject to code motion one day and thus we need to compute
     information about memory sets when we build the hash tables.  */

  alloc_reg_set_mem (max_gcse_regno);
  compute_sets (get_insns ());

  max_gcse_regno = max_reg_num ();
  alloc_gcse_mem (get_insns ());
  changed = one_cprop_pass (1, 1, &jump_changed, 1);
  free_gcse_mem ();

  if (file)
    {
      fprintf (file, "BYPASS of %s: %d basic blocks, ",
	       current_function_name, n_basic_blocks);
      fprintf (file, "%d bytes\n\n", bytes_used);
    }

  obstack_free (&gcse_obstack, NULL);
  free_reg_set_mem ();

  /* We are finished with alias.  */
  end_alias_analysis ();
  allocate_reg_info (max_reg_num (), FALSE, FALSE);

  return changed;
}
