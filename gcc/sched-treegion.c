/* Instruction scheduling pass.
   Copyright (C) 1992, 1993, 1994, 1995, 1996, 1997, 1998,
   1999, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.
   Contributed by Michael Tiemann (tiemann@cygnus.com) Enhanced by,
   and currently maintained by, Jim Wilson (wilson@cygnus.com)

   Treegion instruction scheduling pass additions contributed
   by Chad Rosier (mcrosier@unity.ncsu.edu) 

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

/* This pass implements treegion-based list scheduling.  It is
   run after flow analysis, but before register allocation.

   This pass performs interblock scheduling, moving insns between
   different blocks in the same "treegion."

   Interblock motions performed are useful motions and speculative
   motions, including speculative loads.  The identification of motion 
   type and the check for validity of speculative motions requires
   construction and analysis of the function's control flow graph.

   The main entry point for this pass is schedule_treegions(), called 
   for each function.  The work of the scheduler is organized in three
   levels: (1) function level: insns are subject to splitting,
   control-flow-graph is constructed, regions are computed, (2) region 
   level: control flow graph attributes required for interblock 
   scheduling are computed (dominators, reachability, etc.), data 
   dependences and priorities are computed, and (3) block level: 
   insns in the block are actually scheduled.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "toplev.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "regs.h"
#include "function.h"
#include "flags.h"
#include "insn-config.h"
#include "insn-attr.h"
#include "except.h"
#include "toplev.h"
#include "recog.h"
#include "cfglayout.h"
#include "params.h"
#include "sched-int.h"
#include "target.h"
#include "timevar.h"
#include "tree-pass.h"

/* Define when we want to do count REG_DEAD notes before and after scheduling
   for sanity checking.  We can't do that when conditional execution is used,
   as REG_DEAD exist only for unconditional deaths.  */

#if !defined (HAVE_conditional_execution) && defined (ENABLE_CHECKING)
#define CHECK_DEAD_NOTES 1
#else
#define CHECK_DEAD_NOTES 0
#endif


#ifdef INSN_SCHEDULING

/* Some accessor macros for h_i_d members only used within this file.  */
#define INSN_REF_COUNT(INSN)	(h_i_d[INSN_UID (INSN)].ref_count)
#define FED_BY_SPEC_LOAD(insn)	(h_i_d[INSN_UID (insn)].fed_by_spec_load)
#define IS_LOAD_INSN(insn)	(h_i_d[INSN_UID (insn)].is_load_insn)

/* nr_inter/spec counts interblock/speculative motion for the function.  */
static int nr_inter, nr_spec;

static bool sched_is_disabled_for_current_region_p (void);

/* A region is the main entity for interblock scheduling: insns
   are allowed to move between blocks in the same region, along
   control flow graph edges, in the 'up' direction.  */
typedef struct
{
  int rgn_nr_blocks;		/* Number of blocks in region.  */
  int rgn_nr_insns;		/* Number of instructions in region.  */
  int *rgn_blocks;		/* Region blocks */
}
region;

/* Number of regions in the procedure.  */
static int nr_regions;

/* Table of region descriptions.  */
static region *rgn_table;

/* Array of lists of regions' blocks.  */
static int *rgn_bb_table;

/* Topological order of blocks in the region (if b2 is reachable from
   b1, block_to_bb[b2] > block_to_bb[b1]).  Note: A basic block is
   always referred to by either block or b, while its topological
   order name (in the region) is referred to by bb.  */
static int *block_to_bb;

/* The number of the region containing a block.  */
static int *containing_rgn;

#define RGN_NR_BLOCKS(rgn) (rgn_table[rgn].rgn_nr_blocks)
#define RGN_NR_INSNS(rgn) (rgn_table[rgn].rgn_nr_insns)
#define RGN_BLOCKS(rgn) (rgn_table[rgn].rgn_blocks)
#define BLOCK_TO_BB(block) (block_to_bb[block])
#define CONTAINING_RGN(block) (containing_rgn[block])

void debug_treegions (void);
void debug_treegion (int trgn);
static void find_treegions (void);

extern void debug_live (int, int);

/* Blocks of the current region being scheduled.  */
static int current_nr_blocks;
static int *current_blocks;

/* The mapping from bb to block.  */
#define BB_TO_BLOCK(bb) (BASIC_BLOCK(current_blocks[bb])->index)

/* Target info declarations.

   The block currently being scheduled is referred to as the "target" block,
   while other blocks in the region from which insns can be moved to the
   target are called "source" blocks.  The candidate structure holds info
   about such sources: are they valid?  Speculative?  Etc.  */
typedef struct
{
  basic_block *first_member;
  int nr_members;
}
bblst;

typedef struct
{
  char is_valid;
  char is_speculative;
  int src_prob;
  bblst split_bbs;
  bblst update_bbs;
}
candidate;

static candidate *candidate_table;

/* A speculative motion requires checking live information on the path
   from 'source' to 'target'.  The split blocks are those to be checked.
   After a speculative motion, live information should be modified in
   the 'update' blocks.

   Lists of split and update blocks for each candidate of the current
   target are in array bblst_table.  */
static basic_block *bblst_table;
static int bblst_size, bblst_last;

#define IS_VALID(src) ( candidate_table[src].is_valid )
#define IS_SPECULATIVE(src) ( candidate_table[src].is_speculative )
#define SRC_PROB(src) ( candidate_table[src].src_prob )

/* The bb being currently scheduled.  */
static int target_bb;

/* List of edges.  */
typedef struct
{
  edge *first_member;
  int nr_members;
}
edgelst;

static edge *edgelst_table;
static int edgelst_last;

static void extract_edgelst (sbitmap, edgelst *);


/* Target info functions.  */
static void split_edges (int, int, edgelst *);
static void compute_trg_info (int);
void trgn_debug_candidate (int);
void trgn_debug_candidates (int);

/* Dominators array: dom[i] contains the sbitmap of dominators of
   bb i in the region.  */
static sbitmap *dom;

/* bb 0 is the only region entry.  */
#define IS_RGN_ENTRY(bb) (!bb)

/* Is bb_src dominated by bb_trg.  */
#define IS_DOMINATED(bb_src, bb_trg)                                 \
( TEST_BIT (dom[bb_src], bb_trg) )

/* Probability: Prob[i] is a float in [0, 1] which is the probability
   of bb i relative to the region entry.  */
static float *prob;

/* The probability of bb_src, relative to bb_trg.  Note, that while the
   'prob[bb]' is a float in [0, 1], this macro returns an integer
   in [0, 100].  */
#define GET_SRC_PROB(bb_src, bb_trg) ((int) (100.0 * (prob[bb_src] / \
						      prob[bb_trg])))

/* Bit-set of edges, where bit i stands for edge i.  */
typedef sbitmap edgeset;

/* Number of edges in the region.  */
static int rgn_nr_edges;

/* Array of size rgn_nr_edges.  */
static edge *rgn_edges;

/* Mapping from each edge in the graph to its number in the rgn.  */
#define EDGE_TO_BIT(edge) ((int)(size_t)(edge)->aux)
#define SET_EDGE_TO_BIT(edge,nr) ((edge)->aux = (void *)(size_t)(nr))

/* The split edges of a source bb is different for each target
   bb.  In order to compute this efficiently, the 'potential-split edges'
   are computed for each bb prior to scheduling a region.  This is actually
   the split edges of each bb relative to the region entry.

   pot_split[bb] is the set of potential split edges of bb.  */
static edgeset *pot_split;

/* For every bb, a set of its ancestor edges.  */
static edgeset *ancestor_edges;

static void compute_dom_prob_ps (int);

#define INSN_PROBABILITY(INSN) (SRC_PROB (BLOCK_TO_BB (BLOCK_NUM (INSN))))
#define IS_SPECULATIVE_INSN(INSN) (IS_SPECULATIVE (BLOCK_TO_BB (BLOCK_NUM (INSN))))
#define INSN_BB(INSN) (BLOCK_TO_BB (BLOCK_NUM (INSN)))

/* Speculative scheduling functions.  */
static int check_live_1 (int, rtx);
static void update_live_1 (int, rtx);
static int check_live (rtx, int);
static void update_live (rtx, int);
static void set_spec_fed (rtx);
static int is_pfree (rtx, int, int);
static int find_conditional_protection (rtx, int);
static int is_conditionally_protected (rtx, int, int);
static int is_prisky (rtx, int, int);
static int is_exception_free (rtx, int, int);

static bool sets_likely_spilled (rtx);
static void sets_likely_spilled_1 (rtx, rtx, void *);
static void add_branch_dependences (rtx, rtx);
static void compute_block_backward_dependences (int);
void trgn_debug_dependencies (void);

static void init_treegions (void);
static void schedule_treegion (int);
static rtx concat_INSN_LIST (rtx, rtx);
static void concat_insn_mem_list (rtx, rtx, rtx *, rtx *);
static void propagate_deps (int, struct deps *);
static void free_pending_lists (void);

/* Tail duplication data structures */


/* Types of tail duplication candidates

  We define 4 types of possible tail duplicate candidates
   1: parent tree doms cand tree and cand tree has 2 predecessors
   2: parent tree doms cand tree and cand tree has 3 or more predecessors
   3: parent tree doesn't dom cand tree and cand tree has 2 predecessors
   4: parent tree doesn't dom cand tree and cand tree has 3 or more predecessors
*/
typedef enum{td_type_undef = 0, td_type_1, td_type_2, td_type_3, td_type_4} 
td_type_t;

typedef struct
{
  td_type_t type;           /* tail duplication type */
  int ctrgn, ptrgn, ptrgn2; /* candidate and parent treegions */
  edge pc_edge;             /* edge between parent and candidate */
  edge p2c_edge;            /* edge between parent2 and candidate */
  float efficiency;         /* instantaneous code size efficiency */
}
td_candidate;

td_candidate *td_candidates;
int max_td_candidates, nr_td_candidates;
sbitmap free_candidate_bitmap;

/* Tail duplication functions by Chad Rosier */
static void td_treegions (void);
static int td_treegion (td_candidate c);
static void td_reform_treegions (td_candidate c);
static int td_candidate_p (int trgn);
static int td_init_candidates (void);
static int td_add_candidates (int ctrgn, int best);
static void td_free_candidates (void);
static void td_classify_candidate (td_candidate *c);
static int td_update_candidates (int dup);
static void td_candidate_efficiency (td_candidate *c);
static void td_build_DDG (int trgn);
static void td_free_DDG (int trgn);
static float td_treegion_exec_time (int trgn, edge *false_edges, int nr_false_edges);
static float td_path_res_bound (basic_block *blocks, int nr_blocks, float prob);
static float td_path_dd_bound (basic_block *blocks, int nr_blocks, float prob);
static bool sched_treegion_too_large_p (int trgn);

/* Variables used to limit code expansion due to tail duplication */
static float max_code_growth;
static float orig_insn_count;
static float curr_insn_count;

static float nr_insns_function (void);
static float nr_insns_region (int trgn);

/* Number of instructions in function */
static float
nr_insns_function(void)
{
  float ret = 0;
  basic_block bb;
  rtx insn;
  
  FOR_EACH_BB(bb)
    {
      FOR_BB_INSNS(bb, insn)
	{
	  if(INSN_P(insn))
	    ret += 1.0;
	}
    }
  return ret;
}

/* Number of instructions in a treegion */
static float
nr_insns_region(int trgn)
{
  int i;
  float ret = 0;

  for(i = 0; i < RGN_NR_BLOCKS (trgn); i++)
    ret += RGN_NR_INSNS (trgn);

  return ret;
}
 
/* Extract list of edges from a bitmap containing EDGE_TO_BIT bits.  */

static void
extract_edgelst (sbitmap set, edgelst *el)
{
  unsigned int i = 0;
  sbitmap_iterator sbi;

  /* edgelst table space is reused in each call to extract_edgelst.  */
  edgelst_last = 0;

  el->first_member = &edgelst_table[edgelst_last];
  el->nr_members = 0;

  /* Iterate over each word in the bitset.  */
  EXECUTE_IF_SET_IN_SBITMAP (set, 0, i, sbi)
    {
      edgelst_table[edgelst_last++] = rgn_edges[i];
      el->nr_members++;
    }
}

/* Functions for the construction of regions.  */

/* Print the regions, for debugging purposes.  Callable from debugger.  */

void
debug_treegions (void)
{
  int rgn, bb;

  fprintf (sched_dump, "\n;;   ------------ REGIONS ----------\n\n");
  for (rgn = 0; rgn < nr_regions; rgn++)
    {
      fprintf (sched_dump, ";;\trgn %d nr_blocks %d:\n", rgn,
	       rgn_table[rgn].rgn_nr_blocks);
      fprintf (sched_dump, ";;\tbb/block: ");

      for (bb = 0; bb < rgn_table[rgn].rgn_nr_blocks; bb++)
	{
	  current_blocks = RGN_BLOCKS (rgn);

	  gcc_assert (bb == BLOCK_TO_BB (BB_TO_BLOCK (bb)));
	  fprintf (sched_dump, " %d/%d ", bb, BB_TO_BLOCK (bb));
	}

      fprintf (sched_dump, "\n\n");
    }
}

void
debug_treegion (int rgn)
{
  int bb;
  int *rgn_blocks;

  edge e;
  edge_iterator ei;

  fprintf (sched_dump, ";;\trgn %d nr_blocks %d:\n", rgn,
	   rgn_table[rgn].rgn_nr_blocks);

  fprintf (sched_dump, ";;\tbb/block: ");

  rgn_blocks = RGN_BLOCKS (rgn);  
  for (bb = 0; bb < rgn_table[rgn].rgn_nr_blocks; bb++)
    fprintf (sched_dump, " %d/%d ", bb, BASIC_BLOCK(rgn_blocks[bb])->index);

  putc ('\n', sched_dump);

  for (bb = 0; bb < rgn_table[rgn].rgn_nr_blocks; bb++)
    {
      fprintf (sched_dump, ";; basic block %d, loop depth %d\n",
	       BASIC_BLOCK(rgn_blocks[bb])->index, 
	       BASIC_BLOCK(rgn_blocks[bb])->loop_depth);

      fprintf (sched_dump, ";; pred:      ");
      FOR_EACH_EDGE (e, ei, BASIC_BLOCK(rgn_blocks[bb])->preds)
	dump_edge_info (sched_dump, e, 0);
      putc ('\n', sched_dump);

      fprintf (sched_dump, ";; succ:      ");
      FOR_EACH_EDGE (e, ei, BASIC_BLOCK(rgn_blocks[bb])->succs)
	dump_edge_info (sched_dump, e, 1);
      putc ('\n', sched_dump);
    }

  fprintf (sched_dump, "\n\n");
}

/* Limit region size based on the number of blocks/insns in a region.
   If true is returned then this region is not scheduled. */

static bool
sched_treegion_too_large_p (int trgn)
{
  /* Region includes too many blocks */
  if(RGN_NR_BLOCKS (trgn) > PARAM_VALUE (PARAM_MAX_SCHED_REGION_BLOCKS))
    return true;

  /* Region includes too many insns */
  if(RGN_NR_INSNS (trgn) > PARAM_VALUE (PARAM_MAX_SCHED_REGION_INSNS))
    return true;
      
  return false;
}

/* Update_loop_relations(blk, hdr): Check if the loop headed by max_hdr[blk]
   is still an inner loop.  Put in max_hdr[blk] the header of the most inner
   loop containing blk.  */
#define UPDATE_LOOP_RELATIONS(blk, hdr)		\
{						\
  if (max_hdr[blk] == -1)			\
    max_hdr[blk] = hdr;				\
  else if (dfs_nr[max_hdr[blk]] > dfs_nr[hdr])	\
    RESET_BIT (inner, hdr);			\
  else if (dfs_nr[max_hdr[blk]] < dfs_nr[hdr])	\
    {						\
      RESET_BIT (inner,max_hdr[blk]);		\
      max_hdr[blk] = hdr;			\
    }						\
}

/* Functions for regions scheduling information.  */

/* Compute dominators, probability, and potential-split-edges of bb.
   Assume that these values were already computed for bb's predecessors.  */

static void
compute_dom_prob_ps (int bb)
{
  int pred_bb;
  int nr_out_edges, nr_rgn_out_edges;
  edge_iterator in_ei, out_ei;
  edge in_edge, out_edge;

  prob[bb] = 0.0;
  if (IS_RGN_ENTRY (bb))
    {
      SET_BIT (dom[bb], 0);
      prob[bb] = 1.0;
      return;
    }

  /* Initialize dom[bb] to '111..1'.  */
  sbitmap_ones (dom[bb]);

  FOR_EACH_EDGE (in_edge, in_ei, BASIC_BLOCK (BB_TO_BLOCK (bb))->preds)
    {
      if (in_edge->src == ENTRY_BLOCK_PTR)
	continue;

      pred_bb = BLOCK_TO_BB (in_edge->src->index);
      sbitmap_a_and_b (dom[bb], dom[bb], dom[pred_bb]);
      sbitmap_a_or_b (ancestor_edges[bb],
		      ancestor_edges[bb], ancestor_edges[pred_bb]);

      SET_BIT (ancestor_edges[bb], EDGE_TO_BIT (in_edge));

      sbitmap_a_or_b (pot_split[bb], pot_split[bb], pot_split[pred_bb]);

      nr_out_edges = 0;
      nr_rgn_out_edges = 0;

      FOR_EACH_EDGE (out_edge, out_ei, in_edge->src->succs)
	{
	  ++nr_out_edges;

	  /* The successor doesn't belong in the region?  */
	  if (out_edge->dest != EXIT_BLOCK_PTR
	      && CONTAINING_RGN (out_edge->dest->index)
		 != CONTAINING_RGN (BB_TO_BLOCK (bb)))
	    ++nr_rgn_out_edges;

	  SET_BIT (pot_split[bb], EDGE_TO_BIT (out_edge));
	}

      /* Now nr_rgn_out_edges is the number of region-exit edges from
         pred, and nr_out_edges will be the number of pred out edges
         not leaving the region.  */
      nr_out_edges -= nr_rgn_out_edges;
      if (nr_rgn_out_edges > 0)
	prob[bb] += 0.9 * prob[pred_bb] / nr_out_edges;
      else
	prob[bb] += prob[pred_bb] / nr_out_edges;
    }

  SET_BIT (dom[bb], bb);
  sbitmap_difference (pot_split[bb], pot_split[bb], ancestor_edges[bb]);

  if (sched_verbose >= 2)
    fprintf (sched_dump, ";;  bb_prob(%d, %d) = %3d\n", bb, BB_TO_BLOCK (bb),
	     (int) (100.0 * prob[bb]));
}

/* Functions for target info.  */

/* Compute in BL the list of split-edges of bb_src relatively to bb_trg.
   Note that bb_trg dominates bb_src.  */

static void
split_edges (int bb_src, int bb_trg, edgelst *bl)
{
  sbitmap src = sbitmap_alloc (pot_split[bb_src]->n_bits);
  sbitmap_copy (src, pot_split[bb_src]);

  sbitmap_difference (src, src, pot_split[bb_trg]);
  extract_edgelst (src, bl);
  sbitmap_free (src);
}

/* Find the valid candidate-source-blocks for the target block TRG, compute
   their probability, and check if they are speculative or not.
   For speculative sources, compute their update-blocks and split-blocks.  */

static void
compute_trg_info (int trg)
{
  candidate *sp;
  edgelst el;
  int i, j, k, update_idx;
  basic_block block;
  sbitmap visited;
  edge_iterator ei;
  edge e;

  /* Define some of the fields for the target bb as well.  */
  sp = candidate_table + trg;
  sp->is_valid = 1;
  sp->is_speculative = 0;
  sp->src_prob = 100;

  visited = sbitmap_alloc (last_basic_block);

  for (i = trg + 1; i < current_nr_blocks; i++)
    {
      sp = candidate_table + i;

      sp->is_valid = IS_DOMINATED (i, trg);
      if (sp->is_valid)
	{
	  sp->src_prob = GET_SRC_PROB (i, trg);
	  sp->is_valid = (sp->src_prob >= PARAM_VALUE (PARAM_MIN_SPEC_PROB));
	}

      if (sp->is_valid)
	{
	  split_edges (i, trg, &el);
	  sp->is_speculative = (el.nr_members) ? 1 : 0;
	  if (sp->is_speculative && !flag_schedule_speculative)
	    sp->is_valid = 0;
	}

      if (sp->is_valid)
	{
	  /* Compute split blocks and store them in bblst_table.
	     The TO block of every split edge is a split block.  */
	  sp->split_bbs.first_member = &bblst_table[bblst_last];
	  sp->split_bbs.nr_members = el.nr_members;
	  for (j = 0; j < el.nr_members; bblst_last++, j++)
	    bblst_table[bblst_last] = el.first_member[j]->dest;
	  sp->update_bbs.first_member = &bblst_table[bblst_last];

	  /* Compute update blocks and store them in bblst_table.
	     For every split edge, look at the FROM block, and check
	     all out edges.  For each out edge that is not a split edge,
	     add the TO block to the update block list.  This list can end
	     up with a lot of duplicates.  We need to weed them out to avoid
	     overrunning the end of the bblst_table.  */

	  update_idx = 0;
	  sbitmap_zero (visited);
	  for (j = 0; j < el.nr_members; j++)
	    {
	      block = el.first_member[j]->src;
	      FOR_EACH_EDGE (e, ei, block->succs)
		{
		  if (!TEST_BIT (visited, e->dest->index))
		    {
		      for (k = 0; k < el.nr_members; k++)
			if (e == el.first_member[k])
			  break;

		      if (k >= el.nr_members)
			{
			  bblst_table[bblst_last++] = e->dest;
			  SET_BIT (visited, e->dest->index);
			  update_idx++;
			}
		    }
		}
	    }
	  sp->update_bbs.nr_members = update_idx;

	  /* Make sure we didn't overrun the end of bblst_table.  */
	  gcc_assert (bblst_last <= bblst_size);
	}
      else
	{
	  sp->split_bbs.nr_members = sp->update_bbs.nr_members = 0;

	  sp->is_speculative = 0;
	  sp->src_prob = 0;
	}
    }

  sbitmap_free (visited);
}

/* Print candidates info, for debugging purposes.  Callable from debugger.  */

void
trgn_debug_candidate (int i)
{
  if (!candidate_table[i].is_valid)
    return;

  if (candidate_table[i].is_speculative)
    {
      int j;
      fprintf (sched_dump, "src b %d bb %d speculative \n", BB_TO_BLOCK (i), i);

      fprintf (sched_dump, "split path: ");
      for (j = 0; j < candidate_table[i].split_bbs.nr_members; j++)
	{
	  int b = candidate_table[i].split_bbs.first_member[j]->index;

	  fprintf (sched_dump, " %d ", b);
	}
      fprintf (sched_dump, "\n");

      fprintf (sched_dump, "update path: ");
      for (j = 0; j < candidate_table[i].update_bbs.nr_members; j++)
	{
	  int b = candidate_table[i].update_bbs.first_member[j]->index;

	  fprintf (sched_dump, " %d ", b);
	}
      fprintf (sched_dump, "\n");
    }
  else
    {
      fprintf (sched_dump, " src %d equivalent\n", BB_TO_BLOCK (i));
    }
}

/* Print candidates info, for debugging purposes.  Callable from debugger.  */

void
trgn_debug_candidates (int trg)
{
  int i;

  fprintf (sched_dump, "----------- candidate table: target: b=%d bb=%d ---\n",
	   BB_TO_BLOCK (trg), trg);
  for (i = trg + 1; i < current_nr_blocks; i++)
    trgn_debug_candidate (i);
}

/* Functions for speculative scheduling.  */

/* Return 0 if x is a set of a register alive in the beginning of one
   of the split-blocks of src, otherwise return 1.  */

static int
check_live_1 (int src, rtx x)
{
  int i;
  int regno;
  rtx reg = SET_DEST (x);

  if (reg == 0)
    return 1;

  while (GET_CODE (reg) == SUBREG
	 || GET_CODE (reg) == ZERO_EXTRACT
	 || GET_CODE (reg) == STRICT_LOW_PART)
    reg = XEXP (reg, 0);

  if (GET_CODE (reg) == PARALLEL)
    {
      int i;

      for (i = XVECLEN (reg, 0) - 1; i >= 0; i--)
	if (XEXP (XVECEXP (reg, 0, i), 0) != 0)
	  if (check_live_1 (src, XEXP (XVECEXP (reg, 0, i), 0)))
	    return 1;

      return 0;
    }

  if (!REG_P (reg))
    return 1;

  regno = REGNO (reg);

  if (regno < FIRST_PSEUDO_REGISTER && global_regs[regno])
    {
      /* Global registers are assumed live.  */
      return 0;
    }
  else
    {
      if (regno < FIRST_PSEUDO_REGISTER)
	{
	  /* Check for hard registers.  */
	  int j = hard_regno_nregs[regno][GET_MODE (reg)];
	  while (--j >= 0)
	    {
	      for (i = 0; i < candidate_table[src].split_bbs.nr_members; i++)
		{
		  basic_block b = candidate_table[src].split_bbs.first_member[i];

		  if (REGNO_REG_SET_P (b->il.rtl->global_live_at_start,
				       regno + j))
		    {
		      return 0;
		    }
		}
	    }
	}
      else
	{
	  /* Check for pseudo registers.  */
	  for (i = 0; i < candidate_table[src].split_bbs.nr_members; i++)
	    {
	      basic_block b = candidate_table[src].split_bbs.first_member[i];

	      if (REGNO_REG_SET_P (b->il.rtl->global_live_at_start, regno))
		{
		  return 0;
		}
	    }
	}
    }

  return 1;
}

/* If x is a set of a register R, mark that R is alive in the beginning
   of every update-block of src.  */

static void
update_live_1 (int src, rtx x)
{
  int i;
  int regno;
  rtx reg = SET_DEST (x);

  if (reg == 0)
    return;

  while (GET_CODE (reg) == SUBREG
	 || GET_CODE (reg) == ZERO_EXTRACT
	 || GET_CODE (reg) == STRICT_LOW_PART)
    reg = XEXP (reg, 0);

  if (GET_CODE (reg) == PARALLEL)
    {
      int i;

      for (i = XVECLEN (reg, 0) - 1; i >= 0; i--)
	if (XEXP (XVECEXP (reg, 0, i), 0) != 0)
	  update_live_1 (src, XEXP (XVECEXP (reg, 0, i), 0));

      return;
    }

  if (!REG_P (reg))
    return;

  /* Global registers are always live, so the code below does not apply
     to them.  */

  regno = REGNO (reg);

  if (regno >= FIRST_PSEUDO_REGISTER || !global_regs[regno])
    {
      if (regno < FIRST_PSEUDO_REGISTER)
	{
	  int j = hard_regno_nregs[regno][GET_MODE (reg)];
	  while (--j >= 0)
	    {
	      for (i = 0; i < candidate_table[src].update_bbs.nr_members; i++)
		{
		  basic_block b = candidate_table[src].update_bbs.first_member[i];

		  SET_REGNO_REG_SET (b->il.rtl->global_live_at_start,
				     regno + j);
		}
	    }
	}
      else
	{
	  for (i = 0; i < candidate_table[src].update_bbs.nr_members; i++)
	    {
	      basic_block b = candidate_table[src].update_bbs.first_member[i];

	      SET_REGNO_REG_SET (b->il.rtl->global_live_at_start, regno);
	    }
	}
    }
}

/* Return 1 if insn can be speculatively moved from block src to trg,
   otherwise return 0.  Called before first insertion of insn to
   ready-list or before the scheduling.  */

static int
check_live (rtx insn, int src)
{
  /* Find the registers set by instruction.  */
  if (GET_CODE (PATTERN (insn)) == SET
      || GET_CODE (PATTERN (insn)) == CLOBBER)
    return check_live_1 (src, PATTERN (insn));
  else if (GET_CODE (PATTERN (insn)) == PARALLEL)
    {
      int j;
      for (j = XVECLEN (PATTERN (insn), 0) - 1; j >= 0; j--)
	if ((GET_CODE (XVECEXP (PATTERN (insn), 0, j)) == SET
	     || GET_CODE (XVECEXP (PATTERN (insn), 0, j)) == CLOBBER)
	    && !check_live_1 (src, XVECEXP (PATTERN (insn), 0, j)))
	  return 0;

      return 1;
    }

  return 1;
}

/* Update the live registers info after insn was moved speculatively from
   block src to trg.  */

static void
update_live (rtx insn, int src)
{
  /* Find the registers set by instruction.  */
  if (GET_CODE (PATTERN (insn)) == SET
      || GET_CODE (PATTERN (insn)) == CLOBBER)
    update_live_1 (src, PATTERN (insn));
  else if (GET_CODE (PATTERN (insn)) == PARALLEL)
    {
      int j;
      for (j = XVECLEN (PATTERN (insn), 0) - 1; j >= 0; j--)
	if (GET_CODE (XVECEXP (PATTERN (insn), 0, j)) == SET
	    || GET_CODE (XVECEXP (PATTERN (insn), 0, j)) == CLOBBER)
	  update_live_1 (src, XVECEXP (PATTERN (insn), 0, j));
    }
}

/* Nonzero if block bb_to is equal to, or reachable from block bb_from.  */
#define IS_REACHABLE(bb_from, bb_to)					\
  (bb_from == bb_to							\
   || IS_RGN_ENTRY (bb_from)						\
   || (TEST_BIT (ancestor_edges[bb_to],					\
	 EDGE_TO_BIT (single_pred_edge (BASIC_BLOCK (BB_TO_BLOCK (bb_from)))))))

/* Turns on the fed_by_spec_load flag for insns fed by load_insn.  */

static void
set_spec_fed (rtx load_insn)
{
  rtx link;

  for (link = INSN_DEPEND (load_insn); link; link = XEXP (link, 1))
    if (GET_MODE (link) == VOIDmode)
      FED_BY_SPEC_LOAD (XEXP (link, 0)) = 1;
}				/* set_spec_fed */

/* On the path from the insn to load_insn_bb, find a conditional
branch depending on insn, that guards the speculative load.  */

static int
find_conditional_protection (rtx insn, int load_insn_bb)
{
  rtx link;

  /* Iterate through DEF-USE forward dependences.  */
  for (link = INSN_DEPEND (insn); link; link = XEXP (link, 1))
    {
      rtx next = XEXP (link, 0);
      if ((CONTAINING_RGN (BLOCK_NUM (next)) ==
	   CONTAINING_RGN (BB_TO_BLOCK (load_insn_bb)))
	  && IS_REACHABLE (INSN_BB (next), load_insn_bb)
	  && load_insn_bb != INSN_BB (next)
	  && GET_MODE (link) == VOIDmode
	  && (JUMP_P (next)
	      || find_conditional_protection (next, load_insn_bb)))
	return 1;
    }
  return 0;
}				/* find_conditional_protection */

/* Returns 1 if the same insn1 that participates in the computation
   of load_insn's address is feeding a conditional branch that is
   guarding on load_insn. This is true if we find a the two DEF-USE
   chains:
   insn1 -> ... -> conditional-branch
   insn1 -> ... -> load_insn,
   and if a flow path exist:
   insn1 -> ... -> conditional-branch -> ... -> load_insn,
   and if insn1 is on the path
   region-entry -> ... -> bb_trg -> ... load_insn.

   Locate insn1 by climbing on LOG_LINKS from load_insn.
   Locate the branch by following INSN_DEPEND from insn1.  */

static int
is_conditionally_protected (rtx load_insn, int bb_src, int bb_trg)
{
  rtx link;

  for (link = LOG_LINKS (load_insn); link; link = XEXP (link, 1))
    {
      rtx insn1 = XEXP (link, 0);

      /* Must be a DEF-USE dependence upon non-branch.  */
      if (GET_MODE (link) != VOIDmode
	  || JUMP_P (insn1))
	continue;

      /* Must exist a path: region-entry -> ... -> bb_trg -> ... load_insn.  */
      if (INSN_BB (insn1) == bb_src
	  || (CONTAINING_RGN (BLOCK_NUM (insn1))
	      != CONTAINING_RGN (BB_TO_BLOCK (bb_src)))
	  || (!IS_REACHABLE (bb_trg, INSN_BB (insn1))
	      && !IS_REACHABLE (INSN_BB (insn1), bb_trg)))
	continue;

      /* Now search for the conditional-branch.  */
      if (find_conditional_protection (insn1, bb_src))
	return 1;

      /* Recursive step: search another insn1, "above" current insn1.  */
      return is_conditionally_protected (insn1, bb_src, bb_trg);
    }

  /* The chain does not exist.  */
  return 0;
}				/* is_conditionally_protected */

/* Returns 1 if a clue for "similar load" 'insn2' is found, and hence
   load_insn can move speculatively from bb_src to bb_trg.  All the
   following must hold:

   (1) both loads have 1 base register (PFREE_CANDIDATEs).
   (2) load_insn and load1 have a def-use dependence upon
   the same insn 'insn1'.
   (3) either load2 is in bb_trg, or:
   - there's only one split-block, and
   - load1 is on the escape path, and

   From all these we can conclude that the two loads access memory
   addresses that differ at most by a constant, and hence if moving
   load_insn would cause an exception, it would have been caused by
   load2 anyhow.  */

static int
is_pfree (rtx load_insn, int bb_src, int bb_trg)
{
  rtx back_link;
  candidate *candp = candidate_table + bb_src;

  if (candp->split_bbs.nr_members != 1)
    /* Must have exactly one escape block.  */
    return 0;

  for (back_link = LOG_LINKS (load_insn);
       back_link; back_link = XEXP (back_link, 1))
    {
      rtx insn1 = XEXP (back_link, 0);

      if (GET_MODE (back_link) == VOIDmode)
	{
	  /* Found a DEF-USE dependence (insn1, load_insn).  */
	  rtx fore_link;

	  for (fore_link = INSN_DEPEND (insn1);
	       fore_link; fore_link = XEXP (fore_link, 1))
	    {
	      rtx insn2 = XEXP (fore_link, 0);
	      if (GET_MODE (fore_link) == VOIDmode)
		{
		  /* Found a DEF-USE dependence (insn1, insn2).  */
		  if (haifa_classify_insn (insn2) != PFREE_CANDIDATE)
		    /* insn2 not guaranteed to be a 1 base reg load.  */
		    continue;

		  if (INSN_BB (insn2) == bb_trg)
		    /* insn2 is the similar load, in the target block.  */
		    return 1;

		  if (*(candp->split_bbs.first_member) == BLOCK_FOR_INSN (insn2))
		    /* insn2 is a similar load, in a split-block.  */
		    return 1;
		}
	    }
	}
    }

  /* Couldn't find a similar load.  */
  return 0;
}				/* is_pfree */

/* Return 1 if load_insn is prisky (i.e. if load_insn is fed by
   a load moved speculatively, or if load_insn is protected by
   a compare on load_insn's address).  */

static int
is_prisky (rtx load_insn, int bb_src, int bb_trg)
{
  if (FED_BY_SPEC_LOAD (load_insn))
    return 1;

  if (LOG_LINKS (load_insn) == NULL)
    /* Dependence may 'hide' out of the region.  */
    return 1;

  if (is_conditionally_protected (load_insn, bb_src, bb_trg))
    return 1;

  return 0;
}

/* Insn is a candidate to be moved speculatively from bb_src to bb_trg.
   Return 1 if insn is exception-free (and the motion is valid)
   and 0 otherwise.  */

static int
is_exception_free (rtx insn, int bb_src, int bb_trg)
{
  int insn_class = haifa_classify_insn (insn);

  /* Handle non-load insns.  */
  switch (insn_class)
    {
    case TRAP_FREE:
      return 1;
    case TRAP_RISKY:
      return 0;
    default:;
    }

  /* Handle loads.  */
  if (!flag_schedule_speculative_load)
    return 0;
  IS_LOAD_INSN (insn) = 1;
  switch (insn_class)
    {
    case IFREE:
      return (1);
    case IRISKY:
      return 0;
    case PFREE_CANDIDATE:
      if (is_pfree (insn, bb_src, bb_trg))
	return 1;
      /* Don't 'break' here: PFREE-candidate is also PRISKY-candidate.  */
    case PRISKY_CANDIDATE:
      if (!flag_schedule_speculative_load_dangerous
	  || is_prisky (insn, bb_src, bb_trg))
	return 0;
      break;
    default:;
    }

  return flag_schedule_speculative_load_dangerous;
}

/* The number of insns from the current block scheduled so far.  */
static int sched_target_n_insns;
/* The number of insns from the current block to be scheduled in total.  */
static int target_n_insns;
/* The number of insns from the entire region scheduled so far.  */
static int sched_n_insns;
/* Nonzero if the last scheduled insn was a jump.  */
static int last_was_jump;


/* Implementations of the sched_info functions for region scheduling.  */
static void init_ready_list (struct ready_list *);
static int can_schedule_ready_p (rtx);
static int new_ready (rtx);
static int schedule_more_p (void);
static const char *rgn_print_insn (rtx, int);
static int rgn_rank (rtx, rtx);
static int contributes_to_priority (rtx, rtx);
static void compute_jump_reg_dependencies (rtx, regset, regset, regset);

/* Return nonzero if there are more insns that should be scheduled.  */

static int
schedule_more_p (void)
{
  return ! last_was_jump && sched_target_n_insns < target_n_insns;
}

/* Add all insns that are initially ready to the ready list READY.  Called
   once before scheduling a set of insns.  */

static void
init_ready_list (struct ready_list *ready)
{
  rtx prev_head = current_sched_info->prev_head;
  rtx next_tail = current_sched_info->next_tail;
  int bb_src;
  rtx insn;

  target_n_insns = 0;
  sched_target_n_insns = 0;
  sched_n_insns = 0;
  last_was_jump = 0;

  /* Print debugging information.  */
  if (sched_verbose >= 5)
    trgn_debug_dependencies ();

  /* Prepare current target block info.  */
  if (current_nr_blocks > 1)
    {
      candidate_table = xmalloc (current_nr_blocks * sizeof (candidate));

      bblst_last = 0;
      /* bblst_table holds split blocks and update blocks for each block after
	 the current one in the region.  split blocks and update blocks are
	 the TO blocks of region edges, so there can be at most rgn_nr_edges
	 of them.  */
      bblst_size = (current_nr_blocks - target_bb) * rgn_nr_edges;
      bblst_table = xmalloc (bblst_size * sizeof (basic_block));

      edgelst_last = 0;
      edgelst_table = xmalloc (rgn_nr_edges * sizeof (edge));

      compute_trg_info (target_bb);
    }

  /* Initialize ready list with all 'ready' insns in target block.
     Count number of insns in the target block being scheduled.  */
  for (insn = NEXT_INSN (prev_head); insn != next_tail; insn = NEXT_INSN (insn))
    {
      if (INSN_DEP_COUNT (insn) == 0)
	{
	  ready_add (ready, insn);

	  if (targetm.sched.adjust_priority)
	    INSN_PRIORITY (insn) =
	      targetm.sched.adjust_priority (insn, INSN_PRIORITY (insn));
	}
      target_n_insns++;
    }

  /* Add to ready list all 'ready' insns in valid source blocks.
     For speculative insns, check-live, exception-free, and
     issue-delay.  */
  for (bb_src = target_bb + 1; bb_src < current_nr_blocks; bb_src++)
    if (IS_VALID (bb_src))
      {
	rtx src_head;
	rtx src_next_tail;
	rtx tail, head;

	get_block_head_tail (BB_TO_BLOCK (bb_src), &head, &tail);
	src_next_tail = NEXT_INSN (tail);
	src_head = head;

	for (insn = src_head; insn != src_next_tail; insn = NEXT_INSN (insn))
	  {
	    if (! INSN_P (insn))
	      continue;

	    if (!CANT_MOVE (insn)
		&& (!IS_SPECULATIVE_INSN (insn)
		    || ((recog_memoized (insn) < 0
			 || min_insn_conflict_delay (curr_state,
						     insn, insn) <= 3)
			&& check_live (insn, bb_src)
			&& is_exception_free (insn, bb_src, target_bb))))
	      if (INSN_DEP_COUNT (insn) == 0)
		{
		  ready_add (ready, insn); 

		  if (targetm.sched.adjust_priority)
		    INSN_PRIORITY (insn) =
		      targetm.sched.adjust_priority (insn, INSN_PRIORITY (insn));
		}
	  }
      }
}

/* Called after taking INSN from the ready list.  Returns nonzero if this
   insn can be scheduled, nonzero if we should silently discard it.  */

static int
can_schedule_ready_p (rtx insn)
{
  if (JUMP_P (insn))
    last_was_jump = 1;

  /* An interblock motion?  */
  if (INSN_BB (insn) != target_bb)
    {
      basic_block b1;

      if (IS_SPECULATIVE_INSN (insn))
	{
	  if (!check_live (insn, INSN_BB (insn)))
	    return 0;
	  update_live (insn, INSN_BB (insn));

	  /* For speculative load, mark insns fed by it.  */
	  if (IS_LOAD_INSN (insn) || FED_BY_SPEC_LOAD (insn))
	    set_spec_fed (insn);

	  nr_spec++;
	}
      nr_inter++;

      /* Update source block boundaries.  */
      b1 = BLOCK_FOR_INSN (insn);
      if (insn == BB_HEAD (b1) && insn == BB_END (b1))
	{
	  /* We moved all the insns in the basic block.
	     Emit a note after the last insn and update the
	     begin/end boundaries to point to the note.  */
	  rtx note = emit_note_after (NOTE_INSN_DELETED, insn);
	  BB_HEAD (b1) = note;
	  BB_END (b1) = note;
	}
      else if (insn == BB_END (b1))
	{
	  /* We took insns from the end of the basic block,
	     so update the end of block boundary so that it
	     points to the first insn we did not move.  */
	  BB_END (b1) = PREV_INSN (insn);
	}
      else if (insn == BB_HEAD (b1))
	{
	  /* We took insns from the start of the basic block,
	     so update the start of block boundary so that
	     it points to the first insn we did not move.  */
	  BB_HEAD (b1) = NEXT_INSN (insn);
	}
    }
  else
    {
      /* In block motion.  */
      sched_target_n_insns++;
    }
  sched_n_insns++;

  return 1;
}

/* Called after INSN has all its dependencies resolved.  Return nonzero
   if it should be moved to the ready list or the queue, or zero if we
   should silently discard it.  */
static int
new_ready (rtx next)
{
  /* For speculative insns, before inserting to ready/queue,
     check live, exception-free, and issue-delay.  */
  if (INSN_BB (next) != target_bb
      && (!IS_VALID (INSN_BB (next))
	  || CANT_MOVE (next)
	  || (IS_SPECULATIVE_INSN (next)
	      && ((recog_memoized (next) >= 0
		   && min_insn_conflict_delay (curr_state, next, next) > 3)
		  || !check_live (next, INSN_BB (next))
		  || !is_exception_free (next, INSN_BB (next), target_bb)))))
    return 0;
  return 1;
}

/* Return a string that contains the insn uid and optionally anything else
   necessary to identify this insn in an output.  It's valid to use a
   static buffer for this.  The ALIGNED parameter should cause the string
   to be formatted so that multiple output lines will line up nicely.  */

static const char *
rgn_print_insn (rtx insn, int aligned)
{
  static char tmp[80];

  if (aligned)
    sprintf (tmp, "b%3d: i%4d", INSN_BB (insn), INSN_UID (insn));
  else
    {
      if (current_nr_blocks > 1 && INSN_BB (insn) != target_bb)
	sprintf (tmp, "%d/b%d", INSN_UID (insn), INSN_BB (insn));
      else
	sprintf (tmp, "%d", INSN_UID (insn));
    }
  return tmp;
}

/* Compare priority of two insns.  Return a positive number if the second
   insn is to be preferred for scheduling, and a negative one if the first
   is to be preferred.  Zero if they are equally good.  */

static int
rgn_rank (rtx insn1, rtx insn2)
{
  /* Some comparison make sense in interblock scheduling only.  */
  if (INSN_BB (insn1) != INSN_BB (insn2))
    {
      int spec_val, prob_val;

      /* Prefer an inblock motion on an interblock motion.  */
      if ((INSN_BB (insn2) == target_bb) && (INSN_BB (insn1) != target_bb))
	return 1;
      if ((INSN_BB (insn1) == target_bb) && (INSN_BB (insn2) != target_bb))
	return -1;

      /* Prefer a useful motion on a speculative one.  */
      spec_val = IS_SPECULATIVE_INSN (insn1) - IS_SPECULATIVE_INSN (insn2);
      if (spec_val)
	return spec_val;

      /* Prefer a more probable (speculative) insn.  */
      prob_val = INSN_PROBABILITY (insn2) - INSN_PROBABILITY (insn1);
      if (prob_val)
	return prob_val;
    }
  return 0;
}

/* NEXT is an instruction that depends on INSN (a backward dependence);
   return nonzero if we should include this dependence in priority
   calculations.  */

static int
contributes_to_priority (rtx next, rtx insn)
{
  return BLOCK_NUM (next) == BLOCK_NUM (insn);
}

/* INSN is a JUMP_INSN, COND_SET is the set of registers that are
   conditionally set before INSN.  Store the set of registers that
   must be considered as used by this jump in USED and that of
   registers that must be considered as set in SET.  */

static void
compute_jump_reg_dependencies (rtx insn ATTRIBUTE_UNUSED,
			       regset cond_exec ATTRIBUTE_UNUSED,
			       regset used ATTRIBUTE_UNUSED,
			       regset set ATTRIBUTE_UNUSED)
{
  /* Nothing to do here, since we postprocess jumps in
     add_branch_dependences.  */
}


/* Used in schedule_treegions to initialize current_sched_info for 
   scheduling treegions .  */

static struct sched_info treegion_sched_info =
{
  init_ready_list,
  can_schedule_ready_p,
  schedule_more_p,
  new_ready,
  rgn_rank,
  rgn_print_insn,
  contributes_to_priority,
  compute_jump_reg_dependencies,
  NULL, NULL,
  NULL, NULL,
  0, 0, 0
};



/* Determine if PAT sets a CLASS_LIKELY_SPILLED_P register.  */

static bool
sets_likely_spilled (rtx pat)
{
  bool ret = false;
  note_stores (pat, sets_likely_spilled_1, &ret);
  return ret;
}

static void
sets_likely_spilled_1 (rtx x, rtx pat, void *data)
{
  bool *ret = (bool *) data;

  if (GET_CODE (pat) == SET
      && REG_P (x)
      && REGNO (x) < FIRST_PSEUDO_REGISTER
      && CLASS_LIKELY_SPILLED_P (REGNO_REG_CLASS (REGNO (x))))
    *ret = true;
}

/* Add dependences so that branches are scheduled to run last in their
   block.  */

static void
add_branch_dependences (rtx head, rtx tail)
{
  rtx insn, last;

  /* For all branches, calls, uses, clobbers, cc0 setters, and instructions
     that can throw exceptions, force them to remain in order at the end of
     the block by adding dependencies and giving the last a high priority.
     There may be notes present, and prev_head may also be a note.

     Branches must obviously remain at the end.  Calls should remain at the
     end since moving them results in worse register allocation.  Uses remain
     at the end to ensure proper register allocation.

     cc0 setters remain at the end because they can't be moved away from
     their cc0 user.

     COND_EXEC insns cannot be moved past a branch (see e.g. PR17808).

     Insns setting CLASS_LIKELY_SPILLED_P registers (usually return values)
     are not moved before reload because we can wind up with register
     allocation failures.  */

  insn = tail;
  last = 0;
  while (CALL_P (insn)
	 || JUMP_P (insn)
	 || (NONJUMP_INSN_P (insn)
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER
		 || can_throw_internal (insn)
#ifdef HAVE_cc0
		 || sets_cc0_p (PATTERN (insn))
#endif
		 || (!reload_completed
		     && sets_likely_spilled (PATTERN (insn)))))
	 || NOTE_P (insn))
    {
      if (!NOTE_P (insn))
	{
	  if (last != 0 && !find_insn_list (insn, LOG_LINKS (last)))
	    {
	      if (! sched_insns_conditions_mutex_p (last, insn))
		add_dependence (last, insn, REG_DEP_ANTI);
	      INSN_REF_COUNT (insn)++;
	    }

	  CANT_MOVE (insn) = 1;

	  last = insn;
	}

      /* Don't overrun the bounds of the basic block.  */
      if (insn == head)
	break;

      insn = PREV_INSN (insn);
    }

  /* Make sure these insns are scheduled last in their block.  */
  insn = last;
  if (insn != 0)
    while (insn != head)
      {
	insn = prev_nonnote_insn (insn);

	if (INSN_REF_COUNT (insn) != 0)
	  continue;

	if (! sched_insns_conditions_mutex_p (last, insn))
	  add_dependence (last, insn, REG_DEP_ANTI);
	INSN_REF_COUNT (insn) = 1;
      }

#ifdef HAVE_conditional_execution
  /* Finally, if the block ends in a jump, and we are doing intra-block
     scheduling, make sure that the branch depends on any COND_EXEC insns
     inside the block to avoid moving the COND_EXECs past the branch insn.

     We only have to do this after reload, because (1) before reload there
     are no COND_EXEC insns, and (2) the region scheduler is an intra-block
     scheduler after reload.

     FIXME: We could in some cases move COND_EXEC insns past the branch if
     this scheduler would be a little smarter.  Consider this code:

		T = [addr]
	C  ?	addr += 4
	!C ?	X += 12
	C  ?	T += 1
	C  ?	jump foo

     On a target with a one cycle stall on a memory access the optimal
     sequence would be:

		T = [addr]
	C  ?	addr += 4
	C  ?	T += 1
	C  ?	jump foo
	!C ?	X += 12

     We don't want to put the 'X += 12' before the branch because it just
     wastes a cycle of execution time when the branch is taken.

     Note that in the example "!C" will always be true.  That is another
     possible improvement for handling COND_EXECs in this scheduler: it
     could remove always-true predicates.  */

  if (!reload_completed || ! JUMP_P (tail))
    return;

  insn = tail;
  while (insn != head)
    {
      insn = PREV_INSN (insn);

      /* Note that we want to add this dependency even when
	 sched_insns_conditions_mutex_p returns true.  The whole point
	 is that we _want_ this dependency, even if these insns really
	 are independent.  */
      if (INSN_P (insn) && GET_CODE (PATTERN (insn)) == COND_EXEC)
	add_dependence (tail, insn, REG_DEP_ANTI);
    }
#endif
}

/* Data structures for the computation of data dependences in a regions.  We
   keep one `deps' structure for every basic block.  Before analyzing the
   data dependences for a bb, its variables are initialized as a function of
   the variables of its predecessors.  When the analysis for a bb completes,
   we save the contents to the corresponding bb_deps[bb] variable.  */

static struct deps *bb_deps;

/* Duplicate the INSN_LIST elements of COPY and prepend them to OLD.  */

static rtx
concat_INSN_LIST (rtx copy, rtx old)
{
  rtx new = old;
  for (; copy ; copy = XEXP (copy, 1))
    new = alloc_INSN_LIST (XEXP (copy, 0), new);
  return new;
}

static void
concat_insn_mem_list (rtx copy_insns, rtx copy_mems, rtx *old_insns_p,
		      rtx *old_mems_p)
{
  rtx new_insns = *old_insns_p;
  rtx new_mems = *old_mems_p;

  while (copy_insns)
    {
      new_insns = alloc_INSN_LIST (XEXP (copy_insns, 0), new_insns);
      new_mems = alloc_EXPR_LIST (VOIDmode, XEXP (copy_mems, 0), new_mems);
      copy_insns = XEXP (copy_insns, 1);
      copy_mems = XEXP (copy_mems, 1);
    }

  *old_insns_p = new_insns;
  *old_mems_p = new_mems;
}

/* After computing the dependencies for block BB, propagate the dependencies
   found in TMP_DEPS to the successors of the block.  */
static void
propagate_deps (int bb, struct deps *pred_deps)
{
  basic_block block = BASIC_BLOCK (BB_TO_BLOCK (bb));
  edge_iterator ei;
  edge e;

  /* bb's structures are inherited by its successors.  */
  FOR_EACH_EDGE (e, ei, block->succs)
    {
      struct deps *succ_deps;
      unsigned reg;
      reg_set_iterator rsi;

      /* Only bbs "below" bb, in the same region, are interesting.  */
      if (e->dest == EXIT_BLOCK_PTR
	  || CONTAINING_RGN (block->index) != CONTAINING_RGN (e->dest->index)
	  || BLOCK_TO_BB (e->dest->index) <= bb)
	continue;

      succ_deps = bb_deps + BLOCK_TO_BB (e->dest->index);

      /* The reg_last lists are inherited by successor.  */
      EXECUTE_IF_SET_IN_REG_SET (&pred_deps->reg_last_in_use, 0, reg, rsi)
	{
	  struct deps_reg *pred_rl = &pred_deps->reg_last[reg];
	  struct deps_reg *succ_rl = &succ_deps->reg_last[reg];

	  succ_rl->uses = concat_INSN_LIST (pred_rl->uses, succ_rl->uses);
	  succ_rl->sets = concat_INSN_LIST (pred_rl->sets, succ_rl->sets);
	  succ_rl->clobbers = concat_INSN_LIST (pred_rl->clobbers,
						succ_rl->clobbers);
	  succ_rl->uses_length += pred_rl->uses_length;
	  succ_rl->clobbers_length += pred_rl->clobbers_length;
	}
      IOR_REG_SET (&succ_deps->reg_last_in_use, &pred_deps->reg_last_in_use);

      /* Mem read/write lists are inherited by successor.  */
      concat_insn_mem_list (pred_deps->pending_read_insns,
			    pred_deps->pending_read_mems,
			    &succ_deps->pending_read_insns,
			    &succ_deps->pending_read_mems);
      concat_insn_mem_list (pred_deps->pending_write_insns,
			    pred_deps->pending_write_mems,
			    &succ_deps->pending_write_insns,
			    &succ_deps->pending_write_mems);

      succ_deps->last_pending_memory_flush
	= concat_INSN_LIST (pred_deps->last_pending_memory_flush,
			    succ_deps->last_pending_memory_flush);

      succ_deps->pending_lists_length += pred_deps->pending_lists_length;
      succ_deps->pending_flush_length += pred_deps->pending_flush_length;

      /* last_function_call is inherited by successor.  */
      succ_deps->last_function_call
	= concat_INSN_LIST (pred_deps->last_function_call,
			      succ_deps->last_function_call);

      /* sched_before_next_call is inherited by successor.  */
      succ_deps->sched_before_next_call
	= concat_INSN_LIST (pred_deps->sched_before_next_call,
			    succ_deps->sched_before_next_call);
    }

  /* These lists should point to the right place, for correct
     freeing later.  */
  bb_deps[bb].pending_read_insns = pred_deps->pending_read_insns;
  bb_deps[bb].pending_read_mems = pred_deps->pending_read_mems;
  bb_deps[bb].pending_write_insns = pred_deps->pending_write_insns;
  bb_deps[bb].pending_write_mems = pred_deps->pending_write_mems;

  /* Can't allow these to be freed twice.  */
  pred_deps->pending_read_insns = 0;
  pred_deps->pending_read_mems = 0;
  pred_deps->pending_write_insns = 0;
  pred_deps->pending_write_mems = 0;
}

/* Compute backward dependences inside bb.  In a multiple blocks region:
   (1) a bb is analyzed after its predecessors, and (2) the lists in
   effect at the end of bb (after analyzing for bb) are inherited by
   bb's successors.

   Specifically for reg-reg data dependences, the block insns are
   scanned by sched_analyze () top-to-bottom.  Two lists are
   maintained by sched_analyze (): reg_last[].sets for register DEFs,
   and reg_last[].uses for register USEs.

   When analysis is completed for bb, we update for its successors:
   ;  - DEFS[succ] = Union (DEFS [succ], DEFS [bb])
   ;  - USES[succ] = Union (USES [succ], DEFS [bb])

   The mechanism for computing mem-mem data dependence is very
   similar, and the result is interblock dependences in the region.  */

static void
compute_block_backward_dependences (int bb)
{
  rtx head, tail;
  struct deps tmp_deps;

  tmp_deps = bb_deps[bb];

  /* Do the analysis for this block.  */
  get_block_head_tail (BB_TO_BLOCK (bb), &head, &tail);
  sched_analyze (&tmp_deps, head, tail);
  add_branch_dependences (head, tail);

  if (current_nr_blocks > 1)
    propagate_deps (bb, &tmp_deps);

  /* Free up the INSN_LISTs.  */
  free_deps (&tmp_deps);
}

/* Remove all INSN_LISTs and EXPR_LISTs from the pending lists and add
   them to the unused_*_list variables, so that they can be reused.  */

static void
free_pending_lists (void)
{
  int bb;

  for (bb = 0; bb < current_nr_blocks; bb++)
    {
      free_INSN_LIST_list (&bb_deps[bb].pending_read_insns);
      free_INSN_LIST_list (&bb_deps[bb].pending_write_insns);
      free_EXPR_LIST_list (&bb_deps[bb].pending_read_mems);
      free_EXPR_LIST_list (&bb_deps[bb].pending_write_mems);
    }
}

/* Print dependences for debugging, callable from debugger.  */

void
trgn_debug_dependencies (void)
{
  int bb;

  fprintf (sched_dump, ";;   --------------- forward dependences: ------------ \n");
  for (bb = 0; bb < current_nr_blocks; bb++)
    {
      rtx head, tail;
      rtx next_tail;
      rtx insn;

      get_block_head_tail (BB_TO_BLOCK (bb), &head, &tail);
      next_tail = NEXT_INSN (tail);
      fprintf (sched_dump, "\n;;   --- Region Dependences --- b %d bb %d \n",
	       BB_TO_BLOCK (bb), bb);

      fprintf (sched_dump, ";;   %7s%6s%6s%6s%6s%6s%14s\n",
	       "insn", "code", "bb", "dep", "prio", "cost",
	       "reservation");
      fprintf (sched_dump, ";;   %7s%6s%6s%6s%6s%6s%14s\n",
	       "----", "----", "--", "---", "----", "----",
	       "-----------");

      for (insn = head; insn != next_tail; insn = NEXT_INSN (insn))
	{
	  rtx link;

	  if (! INSN_P (insn))
	    {
	      int n;
	      fprintf (sched_dump, ";;   %6d ", INSN_UID (insn));
	      if (NOTE_P (insn))
		{
		  n = NOTE_LINE_NUMBER (insn);
		  if (n < 0)
		    fprintf (sched_dump, "%s\n", GET_NOTE_INSN_NAME (n));
		  else
		    {
		      expanded_location xloc;
		      NOTE_EXPANDED_LOCATION (xloc, insn);
		      fprintf (sched_dump, "line %d, file %s\n",
			       xloc.line, xloc.file);
		    }
		}
	      else
		fprintf (sched_dump, " {%s}\n", GET_RTX_NAME (GET_CODE (insn)));
	      continue;
	    }

	  fprintf (sched_dump,
		   ";;   %s%5d%6d%6d%6d%6d%6d   ",
		   (SCHED_GROUP_P (insn) ? "+" : " "),
		   INSN_UID (insn),
		   INSN_CODE (insn),
		   INSN_BB (insn),
		   INSN_DEP_COUNT (insn),
		   INSN_PRIORITY (insn),
		   insn_cost (insn, 0, 0));

	  if (recog_memoized (insn) < 0)
	    fprintf (sched_dump, "nothing");
	  else
	    print_reservation (sched_dump, insn);

	  fprintf (sched_dump, "\t: ");
	  for (link = INSN_DEPEND (insn); link; link = XEXP (link, 1))
	    fprintf (sched_dump, "%d ", INSN_UID (XEXP (link, 0)));
	  fprintf (sched_dump, "\n");
	}
    }
  fprintf (sched_dump, "\n");
}

/* Returns true if all the basic blocks of the current region have
   NOTE_DISABLE_SCHED_OF_BLOCK which means not to schedule that region.  */
static bool
sched_is_disabled_for_current_region_p (void)
{
  int bb;

  for (bb = 0; bb < current_nr_blocks; bb++)
    if (!(BASIC_BLOCK (BB_TO_BLOCK (bb))->flags & BB_DISABLE_SCHEDULE))
      return false;

  return true;
}

/* Schedule a treegion.  A region is an acyclic subgraph of the cfg. 
   Each bb in the treegion is scheduled after its flow predecessors. */

static void
schedule_treegion (int rgn)
{
  basic_block block;
  edge_iterator ei;
  edge e;
  int bb;
  int rgn_n_insns = 0;
  int sched_rgn_n_insns = 0;

  /* Set variables for the current region.  */
  current_nr_blocks = RGN_NR_BLOCKS (rgn);
  current_blocks = RGN_BLOCKS (rgn);
  
  /* Don't schedule region that is too large */
  if (sched_treegion_too_large_p (rgn))
    return;
  
  /* Don't schedule region that is marked by
     NOTE_DISABLE_SCHED_OF_BLOCK.  */
  if (sched_is_disabled_for_current_region_p ())
    return;

  init_deps_global ();
  
  /* Initializations for region data dependence analysis.  */
  bb_deps = xmalloc (sizeof (struct deps) * current_nr_blocks);
  for (bb = 0; bb < current_nr_blocks; bb++)
    init_deps (bb_deps + bb);
  
  /* Compute LOG_LINKS.  */
  for (bb = 0; bb < current_nr_blocks; bb++)
    compute_block_backward_dependences (bb);
  
  /* Compute INSN_DEPEND.  */
  for (bb = current_nr_blocks - 1; bb >= 0; bb--)
    {
      rtx head, tail;
      get_block_head_tail (BB_TO_BLOCK (bb), &head, &tail);
      
      compute_forward_dependences (head, tail);

      if (targetm.sched.dependencies_evaluation_hook)
	targetm.sched.dependencies_evaluation_hook (head, tail);
    }
  
  /* Set priorities.  */
  for (bb = 0; bb < current_nr_blocks; bb++)
    {
      rtx head, tail;
      get_block_head_tail (BB_TO_BLOCK (bb), &head, &tail);
      
      rgn_n_insns += set_priorities (head, tail);
    }

  /* Compute interblock info: probabilities, split-edges, dominators, etc.  */
  if (current_nr_blocks > 1)
    {
      prob = xmalloc ((current_nr_blocks) * sizeof (float));
      
      dom = sbitmap_vector_alloc (current_nr_blocks, current_nr_blocks);
      sbitmap_vector_zero (dom, current_nr_blocks);
      
      /* Use ->aux to implement EDGE_TO_BIT mapping.  */
      rgn_nr_edges = 0;
      FOR_EACH_BB (block)
	{
	  if (CONTAINING_RGN (block->index) != rgn)
	    continue;
	  FOR_EACH_EDGE (e, ei, block->succs)
	    SET_EDGE_TO_BIT (e, rgn_nr_edges++);
	}
      
      rgn_edges = xmalloc (rgn_nr_edges * sizeof (edge));
      rgn_nr_edges = 0;
      FOR_EACH_BB (block)
	{
	  if (CONTAINING_RGN (block->index) != rgn)
	    continue;
	  FOR_EACH_EDGE (e, ei, block->succs)
	    rgn_edges[rgn_nr_edges++] = e;
	}

      /* Split edges.  */
      pot_split = sbitmap_vector_alloc (current_nr_blocks, rgn_nr_edges);
      sbitmap_vector_zero (pot_split, current_nr_blocks);
      ancestor_edges = sbitmap_vector_alloc (current_nr_blocks, rgn_nr_edges);
      sbitmap_vector_zero (ancestor_edges, current_nr_blocks);

      /* Compute probabilities, dominators, split_edges.  */
      for (bb = 0; bb < current_nr_blocks; bb++)
	compute_dom_prob_ps (bb);
    }

  /* Now we can schedule all blocks.  */
  for (bb = 0; bb < current_nr_blocks; bb++)
    {
      rtx head, tail;
      int b = BB_TO_BLOCK (bb);

      get_block_head_tail (b, &head, &tail);

      if (no_real_insns_p (head, tail))
	continue;

      current_sched_info->prev_head = PREV_INSN (head);
      current_sched_info->next_tail = NEXT_INSN (tail);

      if (write_symbols != NO_DEBUG)
	{
	  save_line_notes (b, head, tail);
	  rm_line_notes (head, tail);
	}

      /* rm_other_notes only removes notes which are _inside_ the
	 block---that is, it won't remove notes before the first real insn
	 or after the last real insn of the block.  So if the first insn
	 has a REG_SAVE_NOTE which would otherwise be emitted before the
	 insn, it is redundant with the note before the start of the
	 block, and so we have to take it out.  */
      if (INSN_P (head))
	{
	  rtx note;

	  for (note = REG_NOTES (head); note; note = XEXP (note, 1))
	    if (REG_NOTE_KIND (note) == REG_SAVE_NOTE)
	      remove_note (head, note);
	}

      /* Remove remaining note insns from the block, save them in
	 note_list.  These notes are restored at the end of
	 schedule_block ().  */
      rm_other_notes (head, tail);

      target_bb = bb;

      current_sched_info->queue_must_finish_empty
	= current_nr_blocks > 1 && !flag_schedule_interblock;

      schedule_block (b, rgn_n_insns);
      sched_rgn_n_insns += sched_n_insns;

      /* Update target block boundaries.  */
      if (head == BB_HEAD (BASIC_BLOCK (b)))
	BB_HEAD (BASIC_BLOCK (b)) = current_sched_info->head;
      if (tail == BB_END (BASIC_BLOCK (b)))
	BB_END (BASIC_BLOCK (b)) = current_sched_info->tail;

      /* Clean up.  */
      if (current_nr_blocks > 1)
	{
	  free (candidate_table);
	  free (bblst_table);
	  free (edgelst_table);
	}
    }

  /* Sanity check: verify that all region insns were scheduled.  */
  gcc_assert (sched_rgn_n_insns == rgn_n_insns);

  /* Restore line notes.  */
  if (write_symbols != NO_DEBUG)
    {
      for (bb = 0; bb < current_nr_blocks; bb++)
	{
	  rtx head, tail;
	  get_block_head_tail (BB_TO_BLOCK (bb), &head, &tail);
	  restore_line_notes (head, tail);
	}
    }

  /* Done with this region.  */
  free_pending_lists ();

  finish_deps_global ();

  free (bb_deps);

  if (current_nr_blocks > 1)
    {
      /* Cleanup ->aux used for EDGE_TO_BIT mapping.  */
      FOR_EACH_BB (block)
	{
	  if (CONTAINING_RGN (block->index) != rgn)
	    continue;
	  FOR_EACH_EDGE (e, ei, block->succs)
	    e->aux = NULL;
	}

      free (prob);
      sbitmap_vector_free (dom);
      sbitmap_vector_free (pot_split);
      sbitmap_vector_free (ancestor_edges);
      free (rgn_edges);
    }
}

/* Single path data dependence bound */
static float 
td_path_dd_bound (basic_block *blocks, int nr_blocks, float prob){

  int bb;
  int idx, dep_idx;
  int nr_insns, nr_dep_insns, first_dep_insn;
  rtx insn, *insn_list;
  int height, *insn_height_list, *max_pred_height;

  if(prob < 0.001)
    return 0.0;

  /* Find the number of instructions */
  nr_insns = 0;
  first_dep_insn = 0;
  for (bb = 0; bb < nr_blocks; bb++)
    FOR_BB_INSNS(blocks[bb], insn)
      if(INSN_P(insn))
	{
	  nr_insns++;
	  if(INSN_DEP_COUNT (insn) == 0)
	    first_dep_insn++;
	}

  /* Allocate instruction list and height list */
  insn_list = xmalloc(sizeof(rtx) * nr_insns);
  insn_height_list = xmalloc(sizeof(int) * nr_insns);

  nr_dep_insns = nr_insns - first_dep_insn;
  max_pred_height = xmalloc(sizeof(int) * nr_dep_insns);

  /* Build list */
  idx = 0;
  dep_idx = first_dep_insn;
  for (bb = 0; bb < nr_blocks; bb++)
    FOR_BB_INSNS(blocks[bb], insn)
      if(INSN_P(insn))
	{
	  if(INSN_DEP_COUNT (insn) == 0)
	    {
	      gcc_assert(idx < first_dep_insn);
	      insn_list[idx] = insn;
	      insn_height_list[idx++] = 0;
	    }
	  else
	    {
	      gcc_assert(dep_idx < nr_insns);
	      insn_list[dep_idx] = insn;
	      max_pred_height[dep_idx - first_dep_insn] = 0;
	      insn_height_list[dep_idx++] = 0;
	    }
	}
  gcc_assert(idx == first_dep_insn);
  gcc_assert(dep_idx == nr_insns);

  /* Propagate height to dependent instructions */
  idx = 0;
  height = 0;
  for (idx = 0; idx < nr_insns; idx++)
    {
      rtx link;

      /* If you're a dependent instruction then make your height the same 
	 as the height of the tallest predecessor you're depended upon. */
      if(idx >= first_dep_insn)
	insn_height_list[idx] = max_pred_height[idx - first_dep_insn];

      /* Add your latency to the height */
      insn_height_list[idx] += insn_cost(insn_list[idx], 0, 0);

      /* Update the height variable */
      if(height < insn_height_list[idx])
	height = insn_height_list[idx];

      /* This instruction has a height of zero so it can't 
	 increase the height of any dependent successor */
      if(insn_height_list[idx] == 0)
	continue;

      /* Propage your height to the instructions that dependent on you. */
      for (link = INSN_DEPEND (insn_list[idx]); link; link = XEXP (link, 1))
	{
	  dep_idx = (first_dep_insn > idx) ? first_dep_insn : idx + 1;
	  for( ; dep_idx < nr_insns; dep_idx++)
	    {
	      /* Is this the dependent instruction? */
	      if(INSN_UID(insn_list[dep_idx]) == INSN_UID (XEXP (link, 0)))
		if(max_pred_height[dep_idx - first_dep_insn] < insn_height_list[idx])
		  max_pred_height[dep_idx - first_dep_insn] = insn_height_list[idx];
	    }
	}
    }
  return ((height * 1.0) * prob);
}


/* Single path resource bound */
static float
td_path_res_bound (basic_block *blocks, int nr_blocks, float prob){
  int i, issue_rate;
  float nr_insns = 0;

  if (targetm.sched.issue_rate)
    {
      int temp = reload_completed;

      reload_completed = 1;
      issue_rate = (*targetm.sched.issue_rate) ();
      reload_completed = temp;
    }
  else
    issue_rate = 1;

  for(i = 0; i < nr_blocks; i++)
    {
      rtx insn;

      FOR_BB_INSNS(blocks[i], insn)
	if(INSN_P(insn))
	  nr_insns += 1;
    }

  return ((nr_insns / issue_rate) * prob);
}


/* Build the DDG */
static void
td_build_DDG (int trgn)
{
  int bb, idx;

  current_blocks = RGN_BLOCKS (trgn);
  current_nr_blocks = RGN_NR_BLOCKS (trgn);

  sched_init (NULL);
  current_sched_info = &treegion_sched_info;

  init_deps_global ();

  /* Initializations for region data dependence analysis.  */
  bb_deps = xmalloc (sizeof (struct deps) * current_nr_blocks);
  for (idx = 0; idx < current_nr_blocks; idx++)
    init_deps (bb_deps + idx);

  /* Compute LOG_LINKS.  */
  for (bb = 0; bb < current_nr_blocks; bb++)
    compute_block_backward_dependences (bb);

  /* Compute INSN_DEPEND.  */
  for (bb = current_nr_blocks - 1; bb >= 0; bb--)
    {
      rtx head, tail;
      get_block_head_tail (BB_TO_BLOCK (bb), &head, &tail);
      
      compute_forward_dependences (head, tail);
      
      if (targetm.sched.dependencies_evaluation_hook)
	targetm.sched.dependencies_evaluation_hook (head, tail);
    }
}

/* Build the DDG */
static void
td_free_DDG (int trgn)
{
  int i;
  gcc_assert (current_blocks == RGN_BLOCKS (trgn));

  free_pending_lists ();
  finish_deps_global ();
  free (bb_deps);

  /* ia64.c assert...*/
  for(i = 0; i < RGN_NR_BLOCKS (trgn); i++)
    {
      rtx insn;
      basic_block bb = BASIC_BLOCK (BB_TO_BLOCK(i));
      
      FOR_BB_INSNS(bb, insn)
	if(INSN_P(insn))
	  SCHED_GROUP_P (insn) = 0;
    }
  sched_finish ();
}

/* Estimate the expected execution time of a region. The execution 
   time is the max of the data dependence bound (i.e., true data 
   dependence height) and the resource bound. 
*/

static float
td_treegion_exec_time (int trgn, edge *false_edges, int nr_false_edges)
{
  int i, node, child, sp;
  float res_bound, dd_bound, path_prob, bound;

  edge e;
  edge_iterator ei, current_edge, *stack;
  basic_block bb, *path_blocks;

  /* Build the data dependence graph */
  td_build_DDG (trgn);

  current_blocks = RGN_BLOCKS (trgn);
  current_nr_blocks = RGN_NR_BLOCKS (trgn);

  /* Allocate and initialize variables for DFS traversal.  */
  path_blocks = xmalloc ((current_nr_blocks) * sizeof (basic_block));
  stack = xmalloc (current_nr_blocks * sizeof (edge_iterator));

  FOR_EACH_BB (bb)
    {
      if(CONTAINING_RGN (bb->index) != trgn)
	continue;
      
      FOR_EACH_EDGE (e, ei, bb->succs)
	e->passed = 0;
    }

  /* These edges won't exist after tail duplication so
     we mark them as passed so we don't traverse them. */
  for(i = 0; i < nr_false_edges; i++)
    false_edges[i]->passed = 1;

  #define EDGE_PASSED(E) (ei_end_p ((E)) || ei_edge ((E))->passed)
  #define SET_EDGE_PASSED(E) (ei_edge ((E))->passed = 1)

  current_edge = ei_start (BASIC_BLOCK(current_blocks[0])->succs);
  sp = -1;

  /* Perform a DFS traversal of the cfg finding all possible 
     paths through the treegion. For each path calculate the 
     data dependence and resource bounds. */

  bound = 0;
  while (1)
    {
      if (EDGE_PASSED (current_edge))
	{
	  /* We have reached a leaf node. Pop edges off the stack until we find
	     an edge that has not yet been processed.  */

	  while (sp >= 0 && EDGE_PASSED (current_edge))
	    {
	      /* Pop entry off the stack.  */
	      current_edge = stack[sp--];
	      node = ei_edge (current_edge)->src->index;
	      gcc_assert (node != ENTRY_BLOCK);
	      child = ei_edge (current_edge)->dest->index;
	      gcc_assert (child != EXIT_BLOCK);
	      ei_next (&current_edge);
	    }

	  /* See if have finished the DFS tree traversal.  */
	  if (sp < 0 && EDGE_PASSED (current_edge))
	    break;

	  /* Nope, continue the traversal with the popped node.  */
	  continue;
	}

      /* Process a node.  */
      node = ei_edge (current_edge)->src->index;
      gcc_assert (node != ENTRY_BLOCK);
      gcc_assert (node != EXIT_BLOCK);

      child = ei_edge (current_edge)->dest->index;

      /* Is this the end of a path? */
      if ((CONTAINING_RGN(child) != trgn) || 
	  (ei_edge (current_edge)->flags & EDGE_DFS_BACK))
	{
	  path_prob = 1.0;
	  
	  /* Get path and path probability info from stack */
	  for(i = 0; i <= sp; i++)
	    {
	      path_blocks[i] = ei_edge (stack[i])->src;	      
	      path_prob = path_prob * 
		(ei_edge (stack[i])->probability * 1.0 / REG_BR_PROB_BASE);
	    }
	  
	  /* Add path and path probability info from current edge  */
	  path_blocks[i] = ei_edge (current_edge)->src;
	  path_prob = path_prob * 
	    (ei_edge (current_edge)->probability * 1.0 / REG_BR_PROB_BASE);

	  /* Find the resource/data dependence bounds */
	  res_bound = td_path_res_bound (path_blocks, i+1, path_prob);
	  dd_bound = td_path_dd_bound (path_blocks, i+1, path_prob);

	  if(res_bound > dd_bound)
	    bound += res_bound;
	  else
	    bound += dd_bound;
	  	  
	  /* We've reached a leaf node so don't continue down this path */
	  SET_EDGE_PASSED (current_edge);
	  ei_next (&current_edge);
	  continue;
	}

      /* Push an entry on the stack and continue DFS traversal.  */
      stack[++sp] = current_edge;
      SET_EDGE_PASSED (current_edge);
      current_edge = ei_start (ei_edge (current_edge)->dest->succs);
    }
  
  free (stack);
  free (path_blocks);

  /* Free the data dependence graph */
  td_free_DDG (trgn);

  return bound;
}

/* Determine the efficiency (ICSE) of tail duplication.
   
   ICSE = 

         (IPC_after_td - IPC_before_td)
    ------------------------------------------
    (CODE_SIZE_after_td - CODE_SIZE_before_td)
*/
static void 
td_candidate_efficiency (td_candidate *c)
{
  int i, j;
  float ave_p, ave_c, ave_p2, ave_pc, ave_p2c;

  /* Variables for making parent-candidate treegion */
  int nr_false_edges;
  edge e, *false_edges;
  edge_iterator ei;
  region orig_rgn, new_rgn;

  gcc_assert(c->type != td_type_undef);

  /* Clear expected execution times */
  ave_p = ave_c = ave_p2 = ave_pc = ave_p2c = 0;
  c->efficiency = 0;

  ave_p = td_treegion_exec_time (c->ptrgn, NULL, 0);
  ave_c = td_treegion_exec_time (c->ctrgn, NULL, 0);

  /* Form new treegion from parent and candidate */
  nr_false_edges = 0;
  false_edges = xmalloc (EDGE_COUNT(c->pc_edge->dest->preds) * sizeof (edge));
  FOR_EACH_EDGE(e, ei, c->pc_edge->dest->preds)
    {
      if(e != c->pc_edge)
	{
	  false_edges[nr_false_edges] = e;
	  nr_false_edges++;
	}
    }

  new_rgn.rgn_nr_blocks = RGN_NR_BLOCKS (c->ptrgn) + RGN_NR_BLOCKS(c->ctrgn);
  new_rgn.rgn_nr_insns = RGN_NR_INSNS (c->ptrgn) + RGN_NR_INSNS(c->ctrgn);
  new_rgn.rgn_blocks = xmalloc ((new_rgn.rgn_nr_blocks) * sizeof (int));
  
  for(i = 0; i < RGN_NR_BLOCKS (c->ptrgn); i++)
    new_rgn.rgn_blocks[i] = (RGN_BLOCKS (c->ptrgn))[i];

  for(j = 0; j < RGN_NR_BLOCKS (c->ctrgn); j++, i++)
    new_rgn.rgn_blocks[i] = (RGN_BLOCKS (c->ctrgn))[j];

  /* Update the containing and block_to_bb structures */
  for(i = 0; i < RGN_NR_BLOCKS (c->ctrgn); i++)
    {
      CONTAINING_RGN ((RGN_BLOCKS (c->ctrgn))[i]) = c->ptrgn;
      BLOCK_TO_BB ((RGN_BLOCKS (c->ctrgn))[i]) = RGN_NR_BLOCKS (c->ptrgn) + i;
    }
    
  /* Swap parent treegion with new treegion */
  orig_rgn = rgn_table[c->ptrgn];
  rgn_table[c->ptrgn] = new_rgn;

  /* Calculate estimated execution time */
  ave_pc = td_treegion_exec_time (c->ptrgn, false_edges, nr_false_edges);

  /* Repair parent treegion */
  free (false_edges);
  free(rgn_table[c->ptrgn].rgn_blocks);
  rgn_table[c->ptrgn] = orig_rgn;

  /* Repair the candidate containing_rgn and block_to_bb structures */
  for(i = 0; i < RGN_NR_BLOCKS (c->ctrgn); i++)
    {
      CONTAINING_RGN ((RGN_BLOCKS (c->ctrgn))[i]) = c->ctrgn;
      BLOCK_TO_BB ((RGN_BLOCKS (c->ctrgn))[i]) = i;
    }

  if(c->type == td_type_2 || c->type == td_type_4)
    ave_c = ave_c * (c->pc_edge->probability * 1.0 / REG_BR_PROB_BASE);

  if(c->type == td_type_3)
    {
      ave_p2 = td_treegion_exec_time (c->ptrgn2, NULL, 0);

      /* Form new treegion from parent and candidate */
      nr_false_edges = 0;
      false_edges = xmalloc (EDGE_COUNT(c->p2c_edge->dest->preds) * sizeof (edge));
      FOR_EACH_EDGE(e, ei, c->p2c_edge->dest->preds)
	{
	  if(e != c->p2c_edge)
	    {
	      false_edges[nr_false_edges] = e;
	      nr_false_edges++;
	    }
	}

      new_rgn.rgn_nr_blocks = RGN_NR_BLOCKS (c->ptrgn2) + RGN_NR_BLOCKS(c->ctrgn);
      new_rgn.rgn_nr_insns = RGN_NR_INSNS (c->ptrgn2) + RGN_NR_INSNS(c->ctrgn);
      new_rgn.rgn_blocks = xmalloc ((new_rgn.rgn_nr_blocks) * sizeof (int));
      
      for(i = 0; i < RGN_NR_BLOCKS (c->ptrgn2); i++)
	new_rgn.rgn_blocks[i] = (RGN_BLOCKS (c->ptrgn2))[i];

      for(j = 0; j < RGN_NR_BLOCKS (c->ctrgn); j++, i++)
	new_rgn.rgn_blocks[i] = (RGN_BLOCKS (c->ctrgn))[j];
    
      /* Update the containing and block_to_bb structures */
      for(i = 0; i < RGN_NR_BLOCKS (c->ctrgn); i++)
	{
	  CONTAINING_RGN ((RGN_BLOCKS (c->ctrgn))[i]) = c->ptrgn2;
	  BLOCK_TO_BB ((RGN_BLOCKS (c->ctrgn))[i]) = RGN_NR_BLOCKS (c->ptrgn2) + i;
	}
    
      /* Swap parent treegion with new treegion */
      orig_rgn = rgn_table[c->ptrgn2];
      rgn_table[c->ptrgn2] = new_rgn;
    
      ave_p2c = td_treegion_exec_time (c->ptrgn2, false_edges, nr_false_edges);
    
      /* Repair parent treegion */
      free (false_edges);
      free(rgn_table[c->ptrgn2].rgn_blocks);
      rgn_table[c->ptrgn2] = orig_rgn;
    
      /* Repair the candidate containing_rgn and block_to_bb structures */
      for(i = 0; i < RGN_NR_BLOCKS (c->ctrgn); i++)
	{
	  CONTAINING_RGN ((RGN_BLOCKS (c->ctrgn))[i]) = c->ctrgn;
	  BLOCK_TO_BB ((RGN_BLOCKS (c->ctrgn))[i]) = i;
	}
    
      ave_pc = ave_pc * (c->pc_edge->probability * 1.0 / REG_BR_PROB_BASE);
      ave_p2c = ave_p2c * (c->p2c_edge->probability * 1.0 / REG_BR_PROB_BASE);
    }
  else
    {
      ave_p2 = 0;
      ave_p2c = 0;
    }

  c->efficiency = 
    (ave_p + ave_c + ave_p2 - (ave_pc + ave_p2c)) / nr_insns_region(c->ctrgn);

  /*
  fprintf(stderr," Efficiency: %f\n", c->efficiency);
  */
}

/*
  We define 4 types of possible tail duplicate candidates
   1: parent tree doms cand tree and cand tree has 2 predecessors
   2: parent tree doms cand tree and cand tree has 3 or more predecessors
   3: parent tree doesn't dom cand tree and cand tree has 2 predecessors
   4: parent tree doesn't dom cand tree and cand tree has 3 or more predecessors
   Instantaneous code side efficiency is calculated based on type 
*/
static void 
td_classify_candidate (td_candidate *c)
{
  int single_parent;
  int *parent_rgns, edge_count;
  edge e, *parent_edges;
  edge_iterator ei;
  
  basic_block cand_root = c->pc_edge->dest;

  parent_rgns = xmalloc (EDGE_COUNT(cand_root->preds) * sizeof (int));
  parent_edges = xmalloc (EDGE_COUNT(cand_root->preds) * sizeof (edge));
  
  single_parent = 1;
  edge_count = 0;
  
  FOR_EACH_EDGE (e, ei, cand_root->preds)
    {      
      parent_rgns[edge_count] = CONTAINING_RGN(e->src->index);
      parent_edges[edge_count] = e;
      
      /* Check for multiple parents */
      if(edge_count > 0)
	if(parent_rgns[edge_count-1] != parent_rgns[edge_count])
	  single_parent = 0;
      
      edge_count++;
    }

  gcc_assert(edge_count > 1);
  
  c->type = td_type_undef;
  
  /* Type 1/3 */
  if(edge_count == 2)
    {
      if(single_parent)
	{
	  gcc_assert(c->ptrgn == parent_rgns[0]);
	  c->type = td_type_1;
	}
      else
	{
	  if(c->ptrgn == parent_rgns[0])
	    {
	      c->ptrgn2 = parent_rgns[1];
	      c->p2c_edge = parent_edges[1];
	    }
	  else
	    {
	      c->ptrgn2 = parent_rgns[0];
	      c->p2c_edge = parent_edges[0];
	    }
	  c->type = td_type_3;
	}
    }
  else  /* Type 2/4 */
    {
      if(single_parent)
	{
	  gcc_assert(c->ptrgn == parent_rgns[0]);
	  c->type = td_type_2;
	}
      else
	c->type = td_type_4;
    }
  
  free (parent_rgns);
  free (parent_edges);
}

/* Is this treegion a candidate for duplication? */

static int
td_candidate_p(int trgn)
{
  int i;
  edge e;
  edge_iterator ei;

  current_blocks = RGN_BLOCKS(trgn);
  current_nr_blocks = RGN_NR_BLOCKS(trgn); 

  if(current_nr_blocks == 0)
    return 0;

  /* Check to see if this trgn's parent is the ENTRY_BLOCK */
  e = find_edge(ENTRY_BLOCK_PTR, BASIC_BLOCK(BB_TO_BLOCK(0)));
  if(e)
    return 0;

  /* Treegion must have at least two incoming edges */
  gcc_assert(EDGE_COUNT((BASIC_BLOCK(BB_TO_BLOCK(0)))->preds) > 1);

  /* Check for backedge/complex edges into treegion */
  FOR_EACH_EDGE(e, ei, BASIC_BLOCK(BB_TO_BLOCK(0))->preds)
    if(e->flags & (EDGE_DFS_BACK | EDGE_COMPLEX))
      return 0;

  /* Check to make sure all blocks can be replicated.
     Also check for successor complex edges. */
  for(i = 0; i < current_nr_blocks; i++)
    {
      if(!can_duplicate_block_p (BASIC_BLOCK(BB_TO_BLOCK(i))))
	return 0;

      FOR_EACH_EDGE(e, ei, BASIC_BLOCK(BB_TO_BLOCK(i))->succs)
	if(e->flags & EDGE_COMPLEX)
	  return 0;
    }

  /* Don't duplicate a region that can't be scheduled */
  if(sched_is_disabled_for_current_region_p ())
    return 0;

  /* Exceeds our maximum code expansion */
  if (max_code_growth)
    {
      if ((curr_insn_count + nr_insns_region(trgn)) > 
	  (max_code_growth * orig_insn_count))
	return 0;
    }

  return 1;
}

/* Duplicate a block within a treegion */

static void
td_block(basic_block bb, edge redirect_e, int trgn)
{
  edge e;
  edge_iterator ei;
  basic_block new_bb;

  gcc_assert(EDGE_COUNT (bb->preds) > 1);
  
  new_bb = duplicate_block (bb, redirect_e, NULL);
  gcc_assert(new_bb);

  new_bb->aux = bb->aux;
  bb->aux = new_bb;

  /* Replicate successors that are in this trgn */
  FOR_EACH_EDGE (e, ei, new_bb->succs)
    if(CONTAINING_RGN(e->dest->index) == trgn)
      td_block (e->dest, e, trgn);
}

/* Duplicate candidate treegion */

static int
td_treegion(td_candidate c)
{
  int nr_blocks = n_basic_blocks;
  int nr_new_blocks = RGN_NR_BLOCKS (c.ctrgn);
#if 0
  fprintf(stderr, "\n-- td_treegion -- type: %d ptrgn: %d (%d) ctrgn: %d (%d)\n\n",
	  c.type,
	  c.ptrgn, c.pc_edge->src->index, 
	  c.ctrgn, c.pc_edge->dest->index);
#endif

  gcc_assert(c.ctrgn != c.ptrgn);
  gcc_assert(!(c.pc_edge->flags & EDGE_DFS_BACK));

  /* Duplicate candidate treegion */
  td_block (c.pc_edge->dest, c.pc_edge, c.ctrgn);

  /* Tail duplication was successful so update the code size */
  curr_insn_count += nr_insns_region (c.ctrgn);

  gcc_assert((nr_blocks + nr_new_blocks) == n_basic_blocks);

  return 1;
}

/* Update the candidate list after tail duplication.
   Return the index of the best candidate for duplication. */

static int
td_update_candidates (int dup)
{
  bool invalid;
  int i, best;

  td_candidate duped = td_candidates[dup];

  /* Invalidate the just duplicated candidate. */
  td_candidates[dup].type = td_type_undef;
  nr_td_candidates--;

  /* Remove any invalid candidates after tail duplicating candidate duped. */
  best = 0;
  for(i = 0; i < max_td_candidates; i++)
    {
      /* Skip invalid candidates. */
      if(td_candidates[i].type == td_type_undef)
	{
	  SET_BIT (free_candidate_bitmap, i);
	  continue;
	}

      invalid = false;
      if((td_candidates[i].ptrgn == duped.ptrgn) ||
	 (td_candidates[i].ctrgn == duped.ptrgn) ||
	 (td_candidates[i].type == td_type_3 &&
	  td_candidates[i].ptrgn2 == duped.ptrgn))
	invalid = true;

      if((td_candidates[i].ptrgn == duped.ctrgn) ||
	 (td_candidates[i].ctrgn == duped.ctrgn) ||
	 (td_candidates[i].type == td_type_3 &&
	  td_candidates[i].ptrgn2 == duped.ctrgn))
	invalid = true;

      if(duped.type == td_type_3)
	if((td_candidates[i].ptrgn == duped.ptrgn2) ||
	   (td_candidates[i].ctrgn == duped.ptrgn2) ||
	   (td_candidates[i].type == td_type_3 &&
	    td_candidates[i].ptrgn2 == duped.ptrgn2))
	  invalid = true;

      if(invalid)
	{
	  td_candidates[i].type = td_type_undef;
	  SET_BIT (free_candidate_bitmap, i);
	  nr_td_candidates--;
	  continue;
	}

      if((td_candidates[best].type == td_type_undef) ||
	 (td_candidates[best].efficiency < td_candidates[i].efficiency))
	best = i;
    }

  /* Add new candidates resulting from tail duplicating candidate duped. */
  best = td_add_candidates (duped.ptrgn, best);
  best = td_add_candidates (duped.ctrgn, best);

  if(duped.type == td_type_3)
    best = td_add_candidates (duped.ptrgn2, best);

  return best;
}

/* For a single treegion (trgn) add new candidates to the candidate list.  
   best is the current best candidate. Return the (new) best candidate. */

static int
td_add_candidates (int ctrgn, int best)
{
  int ptrgn, idx;
  edge e;
  edge_iterator ei;

  /* Is the candidate treegion a candidate for duplication */
  if(!td_candidate_p(ctrgn))
    return best;
	  
  current_blocks = RGN_BLOCKS (ctrgn);
  FOR_EACH_EDGE(e, ei, BASIC_BLOCK(BB_TO_BLOCK(0))->preds) 
    {
      /* Is the parent treegion a candidate for duplication */
      ptrgn = CONTAINING_RGN(e->src->index);
      if(!td_candidate_p(ptrgn))
	continue;
      
      /* The parent-child region would include too many blocks */
      if((RGN_NR_BLOCKS (ptrgn) + RGN_NR_BLOCKS (ctrgn))
	 > (PARAM_VALUE (PARAM_MAX_SCHED_REGION_BLOCKS)))
	continue;
      
      /* The parent-child region would include too many insns */
      if((RGN_NR_INSNS (ptrgn) + RGN_NR_INSNS (ctrgn))
	 > (PARAM_VALUE (PARAM_MAX_SCHED_REGION_INSNS)))
	continue;
      
      /* Found a candidate. */
      idx = sbitmap_first_set_bit(free_candidate_bitmap);
      RESET_BIT(free_candidate_bitmap, idx);

      td_candidates[idx].ctrgn = ctrgn;
      td_candidates[idx].ptrgn = ptrgn; 
      td_candidates[idx].ptrgn2 = 0;
      td_candidates[idx].pc_edge = e;
      td_candidates[idx].p2c_edge = NULL;
      td_candidates[idx].efficiency = 0;	  
      
      /* Classify the candidate */
      td_classify_candidate (&td_candidates[idx]);
      
      /* Calculate the efficiency */
      td_candidate_efficiency (&td_candidates[idx]);

      if((nr_td_candidates == 0) ||
	 (td_candidates[best].efficiency 
	  < td_candidates[idx].efficiency))
	best = idx;
      
      gcc_assert(td_candidates[best].type != td_type_undef);

      nr_td_candidates++;

      /* Candidate list if full.  Resize. */
      if(nr_td_candidates == max_td_candidates)
	{
	  sbitmap new_free_candidate_bitmap;

	  gcc_assert(0); /* REMOVE ME LATER */

	  /* Allocate new bitmap. */
	  new_free_candidate_bitmap = sbitmap_alloc (n_edges);
	  sbitmap_ones (free_candidate_bitmap);

	  /* Copy old free candidate bitmap to new bitmap. */
	  memcpy (new_free_candidate_bitmap->elms, 
		  free_candidate_bitmap->elms, 
		  sizeof (SBITMAP_ELT_TYPE) * free_candidate_bitmap->size);

	  /* Free old free candidate bitmap. */
	  sbitmap_free (free_candidate_bitmap);
	  free_candidate_bitmap = new_free_candidate_bitmap;

	  /* Reallocate candidate list. */
	  max_td_candidates = n_edges;
	  td_candidates = 
	    xrealloc (td_candidates,
		      max_td_candidates * sizeof (td_candidate));	  
	}

    }
  return best;
}


/* Initialize the tail duplication candidate list. 
   Return the index of the best candidate for duplication. */

static int
td_init_candidates (void)
{
  int i, best, ctrgn;

  best = 0;
  nr_td_candidates = 0;
  max_td_candidates = n_edges;

  /* Allocate free candidate bitmap. Set all slots as free. */
  free_candidate_bitmap = sbitmap_alloc (max_td_candidates);
  sbitmap_ones (free_candidate_bitmap);

  /* Allocate tail duplication candidate list. */
  td_candidates = xmalloc (max_td_candidates * sizeof (td_candidate));

  for(ctrgn = 0; ctrgn < nr_regions; ctrgn++)
    best = td_add_candidates (ctrgn, best);

  /* Invalidate any remaining td_candidates entries by setting 
     td_type to td_type_undef. */
  for(i = nr_td_candidates; i < max_td_candidates; i++)
    td_candidates[i].type = td_type_undef;

  return best;
}

/* Free the tail duplication candidate list. */

static void
td_free_candidates (void)
{
  gcc_assert (td_candidates);
  free(td_candidates);
}


/* Reform (update) treegion data structures after tail duplication.
   We also remove tail duplication candidate (e->aux) that no longer
   exist due to tail duplication. */

static void
td_reform_treegions (td_candidate c)
{
  rtx insn;
  basic_block bb;

  int i, j, *rgn_blocks, *copy_blocks, curr_trgn;
  int num_bbs, idx, node, head, tail;
  edge_iterator current_ei, *ei_list;
  
  sbitmap zero_bitmap; /* Zero bitmap used to check for empty queue */
  sbitmap saplings;    /* Saplings queue */

#if 0
  fprintf(stderr, "td_reform_treegions: ptrgn: %d ctrgn: %d\n",
	  c.ptrgn, c.ctrgn);
#endif  

  mark_dfs_back_edges ();
  
  zero_bitmap = sbitmap_alloc (last_basic_block);
  sbitmap_zero (zero_bitmap);
  
  saplings = sbitmap_alloc (last_basic_block);
  sbitmap_zero (saplings);
  
  /* Set the sapling bits for all treegions that need to be reformed */  
  SET_BIT (saplings, rgn_table[c.ptrgn].rgn_blocks[0]);

  if(c.type == td_type_3)
    SET_BIT (saplings, rgn_table[c.ptrgn2].rgn_blocks[0]);

  rgn_blocks = xmalloc ((n_basic_blocks) * sizeof (int));
  ei_list = xmalloc (n_edges * sizeof (edge_iterator));
  
  while(!sbitmap_equal(saplings, zero_bitmap))
    {
      
      /* Get a sapling to start a new treegion */
      node = sbitmap_first_set_bit(saplings);
      gcc_assert(node != ENTRY_BLOCK);
      RESET_BIT(saplings, node);

      /* Make a new empty treegion */
      idx = 0;
      num_bbs = 0;
      curr_trgn = CONTAINING_RGN (node);
      gcc_assert(curr_trgn < nr_regions);

      /* Move sapling node into treegion */
      rgn_blocks[idx++] = node;
      RGN_NR_BLOCKS (curr_trgn) = ++num_bbs;
      RGN_NR_INSNS (curr_trgn) = 0;
      bb = BASIC_BLOCK (node);
      FOR_BB_INSNS(bb, insn)
	{
	  if(INSN_P(insn))
	    RGN_NR_INSNS (curr_trgn) = RGN_NR_INSNS (curr_trgn) + 1;
	}

      /* BFS traversal to build treegion */
      head = tail = 0;
      if(EDGE_COUNT(BASIC_BLOCK(node)->succs) > 0)
	ei_list[tail++] = ei_start (BASIC_BLOCK(node)->succs);
  
      while(head != tail)
	{
	  current_ei = ei_list[head++];
	  
	  while(!ei_end_p(current_ei))
	    {
	      
	      /* Don't traverse backedges */
	      if(ei_edge (current_ei)->flags & EDGE_DFS_BACK)
		{
		  ei_next (&current_ei);
		  continue;
		}
	      
	      /* Skip Exit block */
	      if (ei_edge (current_ei)->dest == EXIT_BLOCK_PTR)
		{
		  ei_next (&current_ei);
		  continue;
		}
	      
	      /* Skip merge points */
	      if(EDGE_COUNT(ei_edge (current_ei)->dest->preds) > 1)
		{
		  ei_next (&current_ei);
		  continue;
		}
	      
	      /* Move successor into treegion */
	      rgn_blocks[idx++] = ei_edge (current_ei)->dest->index;
	      RGN_NR_BLOCKS (curr_trgn) = ++num_bbs;
	      bb = ei_edge (current_ei)->dest;
	      FOR_BB_INSNS(bb, insn)
		{
		  if(INSN_P(insn))
		    RGN_NR_INSNS (curr_trgn) = RGN_NR_INSNS (curr_trgn) + 1;
		}
	  
	      /* Add successor to BFS edge list */
	      if(EDGE_COUNT(ei_edge (current_ei)->dest->succs) > 0)
		ei_list[tail++] = ei_start (ei_edge (current_ei)->dest->succs);
	      
	      ei_next (&current_ei);
	    }
	}
      
      /* Allocate the blocks for the region */
      copy_blocks = xmalloc ((num_bbs) * sizeof (int));
      memcpy(copy_blocks, rgn_blocks, (num_bbs) * sizeof (int));
      gcc_assert(copy_blocks);
      
      /* Copy blocks to region */
      gcc_assert (RGN_BLOCKS (curr_trgn));
      free (RGN_BLOCKS (curr_trgn));
      RGN_BLOCKS(curr_trgn) = copy_blocks;
      copy_blocks = NULL;
      
    }

  free (ei_list);
  sbitmap_free (zero_bitmap);
  sbitmap_free (saplings);

  /* For type 1/3 tail duplication remove the candidate treegion from 
     the rgn_table. It has now been merged into the parent treegion */

  if(c.type == td_type_1 || c.type == td_type_3)
    {
      RGN_NR_BLOCKS (c.ctrgn) = 0;
      RGN_NR_INSNS (c.ctrgn) = 0;
      gcc_assert (RGN_BLOCKS (c.ctrgn));
      free (RGN_BLOCKS (c.ctrgn));
      RGN_BLOCKS (c.ctrgn) = NULL;
    }

  /* Resize and update block_to_bb and containing_rgn arrays */
  block_to_bb = xrealloc (block_to_bb, (last_basic_block) * sizeof (int));
  containing_rgn = xrealloc (containing_rgn, (last_basic_block) * sizeof (int));

  num_bbs = 0;
  for(i = 0; i < nr_regions; i++)
    {
      rgn_blocks = RGN_BLOCKS(i);
      for(j = 0; j < RGN_NR_BLOCKS (i); j++)
	{
	  BLOCK_TO_BB (rgn_blocks[j]) = j;
	  CONTAINING_RGN (rgn_blocks[j]) = i;
	}
      num_bbs += RGN_NR_BLOCKS (i);
    }

  gcc_assert(num_bbs == (n_basic_blocks - NUM_FIXED_BLOCKS));
}

/* Tail duplicate treegions */

static void
td_treegions (void)
{
  int best;

  /* Nothing to duplicate */
  if(nr_regions == 1)
    return;

  /* Initialize all candidate data structures and find all 
     possible tail duplication candidates. */
  best = td_init_candidates ();

  /* Tail duplicate the best candidate */
  while(nr_td_candidates > 0)
    {
      td_treegion (td_candidates[best]);
      td_reform_treegions (td_candidates[best]);
      best = td_update_candidates (best);
    }

  /* Free candidate structures */
  td_free_candidates ();
}

/* Find natural treegions (i.e., treegions without tail duplication) */

static void
find_treegions (void)
{
  rtx insn;
  basic_block bb;

  int *rgn_blocks, *copy_blocks;
  int num_bbs, idx, node, head, tail;
  edge_iterator current_ei, *ei_list;
  
  sbitmap zero_bitmap; /* Zero bitmap used to check for empty queue */
  sbitmap in_treegion; /* Track if block is assigned to a treegion */
  sbitmap saplings;    /* Saplings queue */
  
  mark_dfs_back_edges ();
  
  nr_regions = 0;
  rgn_blocks = xmalloc ((n_basic_blocks) * sizeof (int));
  rgn_table = xrealloc (rgn_table, (n_basic_blocks) * sizeof (region));
  block_to_bb = xrealloc (block_to_bb, (last_basic_block) * sizeof (int));
  containing_rgn = xrealloc (containing_rgn, (last_basic_block) * sizeof (int));

  ei_list = xmalloc (n_edges * sizeof (edge_iterator));

  zero_bitmap = sbitmap_alloc (last_basic_block);
  sbitmap_zero (zero_bitmap);

  in_treegion = sbitmap_alloc (last_basic_block);
  sbitmap_zero (in_treegion);

  saplings = sbitmap_alloc (last_basic_block);
  sbitmap_zero (saplings);

  /* Start the first treegion with the block after the ENTRY_BLOCK */
  node = single_succ (ENTRY_BLOCK_PTR)->index;
  SET_BIT (saplings, node);

  while(!sbitmap_equal(saplings, zero_bitmap))
    {
      /* Get a sapling to start a new treegion */
      node = sbitmap_first_set_bit(saplings);
      gcc_assert(node != ENTRY_BLOCK);
      RESET_BIT(saplings, node);

      /* Make a new empty treegion */
      idx = 0;
      num_bbs = 0;
      RGN_NR_BLOCKS (nr_regions) = 0;      

      /* Move sapling node into treegion */
      rgn_blocks[idx++] = node;
      BLOCK_TO_BB (node) = num_bbs;      
      CONTAINING_RGN (node) = nr_regions;
      SET_BIT (in_treegion, node);

      /* Set number of block/insns in region */
      RGN_NR_BLOCKS (nr_regions) = ++num_bbs;
      RGN_NR_INSNS (nr_regions) = 0;
      bb = BASIC_BLOCK (node);
      FOR_BB_INSNS(bb, insn)
	{
	  if(INSN_P(insn))
	    RGN_NR_INSNS (nr_regions) = 
	      RGN_NR_INSNS (nr_regions) + 1;
	}

      /* BFS traversal to build treegion */
      head = tail = 0;
      if(EDGE_COUNT(BASIC_BLOCK(node)->succs) > 0)
	 ei_list[tail++] = ei_start (BASIC_BLOCK(node)->succs);

      while(head != tail)
	{
	  current_ei = ei_list[head++];

	  while(!ei_end_p(current_ei))
	    {
	      
	      /* Don't traverse backedges */
	      if(ei_edge (current_ei)->flags & EDGE_DFS_BACK)
		{
		  ei_next (&current_ei);
		  continue;
		}
	      
	      /* Skip Exit block */
	      if (ei_edge (current_ei)->dest == EXIT_BLOCK_PTR)
		{
		  ei_next (&current_ei);
		  continue;
		}

	      /* Skip if already in treegion */
	      if(TEST_BIT (in_treegion, ei_edge (current_ei)->dest->index))
		{
		  ei_next (&current_ei);
		  continue;
		}
	      
	      
	      /* If a merge points then add to sapling list */
	      if(EDGE_COUNT(ei_edge (current_ei)->dest->preds) > 1)
		{
		  SET_BIT (saplings, ei_edge (current_ei)->dest->index);
		  ei_next (&current_ei);
		  continue;
		}
	      
	      /* Move successor into treegion */
	      rgn_blocks[idx++] = ei_edge (current_ei)->dest->index;
	      BLOCK_TO_BB (ei_edge (current_ei)->dest->index) = num_bbs;
	      CONTAINING_RGN (ei_edge (current_ei)->dest->index) = nr_regions;
	      SET_BIT (in_treegion, ei_edge (current_ei)->dest->index);

	      /* Clear the aux pointer. Later used during tail duplication */
	      ei_edge (current_ei)->aux = NULL;

	      /* Set number of block/insns in region */
	      RGN_NR_BLOCKS (nr_regions) = ++num_bbs;
	      bb = ei_edge (current_ei)->dest;
	      FOR_BB_INSNS(bb, insn)
		{
		  if(INSN_P(insn))
		    RGN_NR_INSNS (nr_regions) = 
		      RGN_NR_INSNS (nr_regions) + 1;
		}
	      
	      /* Add successor to BFS edge list */
	      if(EDGE_COUNT(ei_edge (current_ei)->dest->succs) > 0)
		ei_list[tail++] = ei_start (ei_edge (current_ei)->dest->succs);

	      ei_next (&current_ei);
	    }
	}

      /* Allocate the blocks for the region */
      copy_blocks = xmalloc ((num_bbs) * sizeof (int));
      memcpy(copy_blocks, rgn_blocks, (num_bbs) * sizeof (int));
      gcc_assert(copy_blocks);

      /* Copy blocks to region */
      RGN_BLOCKS(nr_regions) = copy_blocks;
      copy_blocks = NULL;
      nr_regions++;
    }
  
  free (rgn_blocks);
  free (ei_list);
  
  sbitmap_free (zero_bitmap);
  sbitmap_free (in_treegion);
  sbitmap_free (saplings);
}

/* Indexed by region, holds the number of death notes found in that region.
   Used for consistency checks.  */
static int *deaths_in_region;

/* Initialize data structures for treegion scheduling.  */

static void
init_treegions (void)
{
  int rgn;
  basic_block bb;
  sbitmap blocks;
  
  /* Clear region information */
  nr_regions = 0;
  rgn_table = (region *)NULL;
  block_to_bb = (int *)NULL;
  containing_rgn = (int *)NULL;
  td_candidates = (td_candidate *)NULL;

  max_code_growth = PARAM_VALUE (PARAM_MAX_CODE_GROWTH) / 100.0;

  /* If limiting by code expansion then initalize counters */
  if (max_code_growth)
    orig_insn_count = curr_insn_count = nr_insns_function(); 

  cfg_layout_initialize(0);

  /* Build the reorder chain for the original order of blocks.  */
  FOR_EACH_BB (bb)
    if (bb->next_bb != EXIT_BLOCK_PTR)
      bb->aux = bb->next_bb;

  /* Find natural treegions */
  find_treegions ();

  /* Tail duplication */
  td_treegions ();

  cfg_layout_finalize();
  cleanup_cfg(CLEANUP_EXPENSIVE);

  /* Reform treegions since cleanup_cfg() makes region 
     information stale */
  find_treegions ();

  if (sched_verbose >= 3)
    debug_treegions ();
  
  if (CHECK_DEAD_NOTES)
    {
      blocks = sbitmap_alloc (last_basic_block);
      deaths_in_region = xmalloc (sizeof (int) * nr_regions);
      /* Remove all death notes from the subroutine.  */
      for (rgn = 0; rgn < nr_regions; rgn++)
	{
	  int b;
	  int *rgn_blocks = RGN_BLOCKS (rgn);

	  sbitmap_zero (blocks);
	  for (b = RGN_NR_BLOCKS (rgn) - 1; b >= 0; --b)
	    SET_BIT (blocks, BASIC_BLOCK(rgn_blocks[b])->index);

	  deaths_in_region[rgn] = count_or_remove_death_notes (blocks, 1);
	}
      sbitmap_free (blocks);
    }
  else
    count_or_remove_death_notes (NULL, 1);
}

/* The one entry point in this file.  DUMP_FILE is the dump file for
   this pass. This function is a derivative of schedule_insns 
   (in sched-rgn.c). */

void
schedule_treegions (FILE *dump_file)
{
  sbitmap large_region_blocks, blocks;
  int rgn;
  int any_large_regions;
  basic_block bb;

  /* Taking care of this degenerate case makes the rest of
     this code simpler.  */
  if (n_basic_blocks == NUM_FIXED_BLOCKS)
    return;

  nr_inter = 0;
  nr_spec = 0;

  init_treegions ();

  /* We must call this after init_regions because information may
     change due to tail duplication during region formation. */

  sched_init (dump_file);

  current_sched_info = &treegion_sched_info;

  /* Schedule every region in the subroutine.  */
  for (rgn = 0; rgn < nr_regions; rgn++)
    schedule_treegion (rgn);

  /* Update life analysis for the subroutine.  Do single block regions
     first so that we can verify that live_at_start didn't change.  Then
     do all other blocks.  */
  /* ??? There is an outside possibility that update_life_info, or more
     to the point propagate_block, could get called with nonzero flags
     more than once for one basic block.  This would be kinda bad if it
     were to happen, since REG_INFO would be accumulated twice for the
     block, and we'd have twice the REG_DEAD notes.

     I'm fairly certain that this _shouldn't_ happen, since I don't think
     that live_at_start should change at region heads.  Not sure what the
     best way to test for this kind of thing...  */

  allocate_reg_life_data ();
  compute_bb_for_insn ();

  any_large_regions = 0;
  large_region_blocks = sbitmap_alloc (last_basic_block);
  sbitmap_zero (large_region_blocks);
  FOR_EACH_BB (bb)
    SET_BIT (large_region_blocks, bb->index);

  blocks = sbitmap_alloc (last_basic_block);
  sbitmap_zero (blocks);

  /* Update life information.  For regions consisting of multiple blocks
     we've possibly done interblock scheduling that affects global liveness.
     For regions consisting of single blocks we need to do only local
     liveness.  */
  for (rgn = 0; rgn < nr_regions; rgn++){
    int *rgn_blocks = RGN_BLOCKS (rgn);

    if (RGN_NR_BLOCKS (rgn) > 1)
      any_large_regions = 1;
    else
      {
	SET_BIT (blocks, BASIC_BLOCK(rgn_blocks[0])->index);
	RESET_BIT (large_region_blocks, BASIC_BLOCK(rgn_blocks[0])->index);
      }
  }

  /* Don't update reg info after reload, since that affects
     regs_ever_live, which should not change after reload.  */
  update_life_info (blocks, UPDATE_LIFE_LOCAL,
		    (reload_completed ? PROP_DEATH_NOTES
		     : PROP_DEATH_NOTES | PROP_REG_INFO));
  if (any_large_regions)
    {
      update_life_info (large_region_blocks, UPDATE_LIFE_GLOBAL,
			PROP_DEATH_NOTES | PROP_REG_INFO);
    }

  if (CHECK_DEAD_NOTES)
    {
      /* Verify the counts of basic block notes in single the basic block
         regions.  */
      for (rgn = 0; rgn < nr_regions; rgn++){
	int *rgn_blocks = RGN_BLOCKS (rgn);

	if (RGN_NR_BLOCKS (rgn) == 1)
	  {
	    sbitmap_zero (blocks);
	    SET_BIT (blocks, BASIC_BLOCK(rgn_blocks[0])->index);

	    gcc_assert (deaths_in_region[rgn]
			== count_or_remove_death_notes (blocks, 0));
	  }
      }
      free (deaths_in_region);
    }

  /* Reposition the prologue and epilogue notes in case we moved the
     prologue/epilogue insns.  */
  if (reload_completed)
    reposition_prologue_and_epilogue_notes (get_insns ());

  /* Delete redundant line notes.  */
  if (write_symbols != NO_DEBUG)
    rm_redundant_line_notes ();

  if (sched_verbose)
    {
      if (reload_completed == 0 && flag_schedule_interblock)
	{
	  fprintf (sched_dump,
		   "\n;; Procedure interblock/speculative motions == %d/%d \n",
		   nr_inter, nr_spec);
	}
      else
	gcc_assert (nr_inter <= 0);
      fprintf (sched_dump, "\n\n");
    }

  /* Clean up.  */
  free (rgn_table);
  free (block_to_bb);
  free (containing_rgn);
  free (rgn_bb_table);

  sched_finish ();

  sbitmap_free (blocks);
  sbitmap_free (large_region_blocks);
}
#endif

static bool
gate_handle_sched_treegions (void)
{
#ifdef INSN_SCHEDULING
  return flag_schedule_treegions;
#else
  return 0;
#endif
}

/* Run treegion scheduler.  */
static void
rest_of_handle_sched_treegions (void)
{
#ifdef INSN_SCHEDULING
  schedule_treegions (dump_file);
#endif
}

struct tree_opt_pass pass_sched_treegions =
{
  "sched_tree",                         /* name */
  gate_handle_sched_treegions,          /* gate */
  rest_of_handle_sched_treegions,       /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_SCHED_TREEGIONS,                   /* tv_id */
  0,                                    /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,                                    /* todo_flags_start */
  TODO_dump_func |
  TODO_ggc_collect,                     /* todo_flags_finish */
  'S'                                   /* letter */
};
