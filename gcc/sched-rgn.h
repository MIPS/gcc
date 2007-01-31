/* Instruction scheduling pass.  This file contains interface to 
   the regions data of the Haifa scheduler.   
   Copyright (C) 2006, 2007 Free Software Foundation, Inc.

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

#ifndef GCC_SCHED_RGN_H
#define GCC_SCHED_RGN_H

extern struct common_sched_info_def rgn_common_sched_info;
extern struct sched_deps_info_def rgn_sched_deps_info;

/* A region is the main entity for interblock scheduling: insns
   are allowed to move between blocks in the same region, along
   control flow graph edges, in the 'up' direction.  */
typedef struct
{
  /* Number of extended basic blocks in region.  */
  int rgn_nr_blocks;
  /* cblocks in the region (actually index in rgn_bb_table).  */
  int rgn_blocks;
  /* Dependencies for this region are already computed.  Basically, indicates,
     that this is a recovery block.  */
  unsigned int dont_calc_deps : 1;
  /* This region has at least one non-trivial ebb.  */
  unsigned int has_real_ebb : 1;
}
region;

extern int nr_regions;
extern region *rgn_table;
extern int *rgn_bb_table;
extern int *block_to_bb;
extern int *containing_rgn;

#define RGN_NR_BLOCKS(rgn) (rgn_table[rgn].rgn_nr_blocks)
#define RGN_BLOCKS(rgn) (rgn_table[rgn].rgn_blocks)
#define RGN_DONT_CALC_DEPS(rgn) (rgn_table[rgn].dont_calc_deps)
#define RGN_HAS_REAL_EBB(rgn) (rgn_table[rgn].has_real_ebb)
#define BLOCK_TO_BB(block) (block_to_bb[block])
#define CONTAINING_RGN(block) (containing_rgn[block])

extern int *ebb_head;

/* The mapping from ebb to block.  */
#define BB_TO_BLOCK(ebb) (rgn_bb_table[ebb_head[ebb]])
#define EBB_FIRST_BB(ebb) BASIC_BLOCK (BB_TO_BLOCK (ebb))
#define EBB_LAST_BB(ebb) BASIC_BLOCK (rgn_bb_table[ebb_head[ebb + 1] - 1])

extern int current_nr_blocks;
extern int current_blocks;

extern void sched_rgn_init (bool, bool);
extern void sched_rgn_finish (void);
extern bool sched_rgn_local_preinit (int);
extern bool sched_rgn_local_init (int, bool);
extern void sched_rgn_local_finish (void);
extern void sched_rgn_local_free (void);
extern void extend_regions (void);

extern void compute_trg_info (int);
extern void free_trg_info (void);
extern int check_live (rtx, int);
extern void update_live (rtx, int);

extern void debug_regions (void);
extern void debug_region (int);
extern void dump_region_dot (FILE *, int);
extern void dump_region_dot_file (const char *, int);

struct deps;

extern void deps_join (struct deps *, struct deps *);

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

extern candidate *candidate_table;

#define IS_VALID(src) (candidate_table[src].is_valid)
#define IS_SPECULATIVE(src) (candidate_table[src].is_speculative)

#define IS_SPECULATIVE_INSN(INSN)			\
  (IS_SPECULATIVE (BLOCK_TO_BB (BLOCK_NUM (INSN))))

#define INSN_BB(INSN) (BLOCK_TO_BB (BLOCK_NUM (INSN)))

extern int target_bb;


#endif /* GCC_SCHED_RGN_H */
