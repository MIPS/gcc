/* Global common subexpression elimination/Partial redundancy elimination
   and global constant/copy propagation for GNU compiler.
   Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002
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

#ifndef GCC_GCSE_GLOBALS_H
#define GCC_GCSE_GLOBALS_H

/* Various types.  */

struct reg_use {rtx reg_rtx; };

/* Hash table of expressions.  */

struct expr
{
  /* The expression (SET_SRC for expressions, PATTERN for assignments).  */
  rtx expr;
  /* Index in the available expression bitmaps.  */
  int bitmap_index;
  /* Next entry with the same hash.  */
  struct expr *next_same_hash;
  /* List of anticipatable occurrences in basic blocks in the function.
     An "anticipatable occurrence" is one that is the first occurrence in the
     basic block, the operands are not modified in the basic block prior
     to the occurrence and the output is not used between the start of
     the block and the occurrence.  */
  struct occr *antic_occr;
  /* List of available occurrence in basic blocks in the function.
     An "available occurrence" is one that is the last occurrence in the
     basic block and the operands are not modified by following statements in
     the basic block [including this insn].  */
  struct occr *avail_occr;
  /* Non-null if the computation is PRE redundant.
     The value is the newly created pseudo-reg to record a copy of the
     expression in all the places that reach the redundant copy.  */
  rtx reaching_reg;
};

/* Occurrence of an expression.
   There is one per basic block.  If a pattern appears more than once the
   last appearance is used [or first for anticipatable expressions].  */

struct occr
{
  /* Next occurrence of this expression.  */
  struct occr *next;
  /* The insn that computes the expression.  */
  rtx insn;
  /* Non-zero if this [anticipatable] occurrence has been deleted.  */
  char deleted_p;
  /* Non-zero if this [available] occurrence has been copied to
     reaching_reg.  */
  /* ??? This is mutually exclusive with deleted_p, so they could share
     the same byte.  */
  char copied_p;
};

/* Expression and copy propagation hash tables.
   Each hash table is an array of buckets.
   ??? It is known that if it were an array of entries, structure elements
   `next_same_hash' and `bitmap_index' wouldn't be necessary.  However, it is
   not clear whether in the final analysis a sufficient amount of memory would
   be saved as the size of the available expression bitmaps would be larger
   [one could build a mapping table without holes afterwards though].
   Someday I'll perform the computation and figure it out.  */

struct hash_table
{
  /* The table itself.
     This is an array of `expr_hash_table_size' elements.  */
  struct expr **table;

  /* Size of the hash table, in elements.  */
  unsigned int size;

  /* Number of hash table elements.  */
  unsigned int n_elems;

  /* Whether the table is expression of copy propagation one.  */
  int set_p;
};

struct reg_avail_info
{
  basic_block last_bb;
  int first_set;
  int last_set;
};

/* Table of registers that are modified.

   For each register, each element is a list of places where the pseudo-reg
   is set.

   For simplicity, GCSE is done on sets of pseudo-regs only.  PRE GCSE only
   requires knowledge of which blocks kill which regs [and thus could use
   a bitmap instead of the lists `reg_set_table' uses].

   `reg_set_table' and could be turned into an array of bitmaps (num-bbs x
   num-regs) [however perhaps it may be useful to keep the data as is].  One
   advantage of recording things this way is that `reg_set_table' is fairly
   sparse with respect to pseudo regs but for hard regs could be fairly dense
   [relatively speaking].  And recording sets of pseudo-regs in lists speeds
   up functions like compute_transp since in the case of pseudo-regs we only
   need to iterate over the number of times a pseudo-reg is set, not over the
   number of basic blocks [clearly there is a bit of a slow down in the cases
   where a pseudo is set more than once in a block, however it is believed
   that the net effect is to speed things up].  This isn't done for hard-regs
   because recording call-clobbered hard-regs in `reg_set_table' at each
   function call can consume a fair bit of memory, and iterating over
   hard-regs stored this way in compute_transp will be more expensive.  */

typedef struct reg_set
{
  /* The next setting of this register.  */
  struct reg_set *next;
  /* The insn where it was set.  */
  rtx insn;
} reg_set;

struct reg_set_table
{
  reg_set **table;
  int size;
};

/* Amount to grow `reg_set_table' by when it's full.  */
#define REG_SET_TABLE_SLOP 100

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

struct ls_expr
{
  struct expr * expr;		/* Gcse expression reference for LM.  */
  rtx pattern;			/* Pattern of this mem.  */
  rtx pattern_regs;		/* List of registers mentioned by the mem.  */
  rtx loads;			/* INSN list of loads seen.  */
  rtx stores;			/* INSN list of stores seen.  */
  struct ls_expr * next;	/* Next in the list.  */
  int invalid;			/* Invalid for some reason.  */
  int index;			/* If it maps to a bitmap index.  */
  int hash_index;		/* Index when in a hash table.  */
  rtx reaching_reg;		/* Register to use when re-writing.  */
};

/* Each block has a bitmap of each type.
   The length of each blocks bitmap is:

       max_cuid  - for reaching definitions
       n_exprs - for available expressions

   Thus we view the bitmaps as 2 dimensional arrays.  i.e.
   rd.kill[block_num][cuid_num]
   ae.kill[block_num][expr_num]			 */

/* For reaching defs */
struct rd
{
  sbitmap *kill, *gen, *reaching_defs, *out;
};

/* for available exprs */
struct ae
{
  sbitmap *kill, *gen, *in, *out;
};

/* Global data for PRE.  */
struct pre_global
{
  /* Nonzero for expressions that are transparent in the block.  */
  sbitmap *transp;

  /* Nonzero for expressions that are computed (available) in the block.  */
  sbitmap *comp;

  /* Nonzero for expressions that are locally anticipatable in the block.  */
  sbitmap *antloc;

  /* Nonzero for expressions which should be inserted on a specific edge.  */
  sbitmap *insert_map;

  /* Nonzero for expressions which should be deleted in a specific block.  */
  sbitmap *delete_map;

  /* Contains the edge_list returned by pre_edge_lcm.  */
  struct edge_list *edge_list;

  /* Redundant insns.  */
  sbitmap redundant_insns;

  /* Available expressions.  */
  struct ae ae;

  /* Expressions hash table.  */
  struct hash_table expr_hash_table;
};

/* Global data for classic gcse.  */
struct classic_global
{
  /* Available expressions.  */
  struct ae ae;

  /* Reaching definitions.  */
  struct rd rd;

  /* Expression hash table.  */
  struct hash_table expr_hash_table;	       
};

/* Global data for hoisting.  */
struct hoist_global
{
  /* Nonzero for expressions that are transparent in the block.  */
  sbitmap *transp;

  /* Nonzero for expressions that are transparent at the end of the block.
     This is only zero for expressions killed by abnormal critical edge
     created by a calls.  */
  sbitmap *transpout;

  /* Nonzero for expressions that are computed (available) in the block.  */
  sbitmap *comp;

  /* Nonzero for expressions that are locally anticipatable in the block.  */
  sbitmap *antloc;

  /* Nonzero for expressions which should be inserted on a specific edge.  */
  sbitmap *insert_map;

  /* Nonzero for expressions which should be deleted in a specific block.  */
  sbitmap *delete_map;

  /* Available expressions.  */
  struct ae ae;

  /* Very busy expressions.  */
  sbitmap *vbein;
  sbitmap *vbeout;

  /* Hoistable expressions.  */
  sbitmap *exprs;

  /* Dominator bitmaps.  */
  dominance_info dominators;

  /* Expressions hash table.  */
  struct hash_table expr_hash_table;
};

/* Global data for store motion.  */
struct store_global
{
  /* This is used to communicate the target bitvector we want to use in the 
     reg_set_info routine when called via the note_stores mechanism.  */
  int * regvec;

  /* And current insn, for the same routine.  */
  rtx current_insn;

  /* Nonzero for expressions that are transparent in the block.  */
  sbitmap *transp;

  /* Used in computing the reverse edge graph bit vectors.  */
  sbitmap * antloc;

  /* Global holding the number of store expressions we are dealing with.  */
  int num_stores;

  /* Contains the edge_list returned by pre_edge_lcm.  */
  struct edge_list *edge_list;

  /* Available expressions.  */
  struct ae ae;

  /* Nonzero for expressions which should be inserted on a specific edge.  */
  sbitmap *insert_map;

  /* Nonzero for expressions which should be deleted in a specific block.  */
  sbitmap *delete_map;
};

/* Copy/constant propagation global data.  */

/* Maximum number of register uses in an insn that we handle.  */
#define MAX_USES 8

struct cprop_global
{
  /* Local properties of assignments.  */
  sbitmap *pavloc;
  sbitmap *absaltered;

  /* Global properties of assignments (computed from the local properties).  */
  sbitmap *avin;
  sbitmap *avout;

  /* Set hash table.  */
  struct hash_table set_hash_table;

  /* Table of uses found in an insn.  */
  struct reg_use reg_use_table[MAX_USES];

  /* Index into `reg_use_table' while building it.  */
  int reg_use_count;
};

/* Objects of this type are passed around by the null-pointer check
   removal routines.  */
struct null_pointer_info
{
  /* The basic block being processed.  */
  basic_block current_block;
  /* The first register to be handled in this pass.  */
  unsigned int min_reg;
  /* One greater than the last register to be handled in this pass.  */
  unsigned int max_reg;
  sbitmap *nonnull_local;
  sbitmap *nonnull_killed;
};

/* GCSE global vars.  */

/* -dG dump file.  */
extern FILE *gcse_file;

/* Bitmaps are normally not included in debugging dumps.
   However it's useful to be able to print them from GDB.
   We could create special functions for this, but it's simpler to
   just allow passing stderr to the dump_foo fns.  Since stderr can
   be a macro, we store a copy here.  */
extern FILE *debug_stderr;

/* An obstack for our working variables.  */
extern struct obstack gcse_obstack;

/* Mapping of uids to cuids.
   Only real insns get cuids.  */
extern int *uid_cuid;

/* Highest UID in UID_CUID.  */
extern int max_uid;

/* Get the cuid of an insn.  */
#ifdef ENABLE_CHECKING
#define INSN_CUID(INSN) (INSN_UID (INSN) > max_uid ? (abort (), 0) : uid_cuid[INSN_UID (INSN)])
#else
#define INSN_CUID(INSN) (uid_cuid[INSN_UID (INSN)])
#endif

/* Number of cuids.  */
extern int max_cuid;

/* Mapping of cuids to insns.  */
extern rtx *cuid_insn;

/* Get insn from cuid.  */
#define CUID_INSN(CUID) (cuid_insn[CUID])

/* Maximum register number in function prior to doing gcse + 1.
   Registers created during this pass have regno >= max_gcse_regno.
   This is named with "gcse" to not collide with global of same name.  */
extern unsigned int max_gcse_regno;

/* Table of registers that are modified.  */
extern struct reg_set_table reg_set_table;

/* Head of the list of load/store memory refs.  */
extern struct ls_expr * pre_ldst_mems;

/* Bitmap containing one bit for each register in the program.
   Used when performing GCSE to track which registers have been set since
   the start of the basic block.  */
extern regset reg_set_bitmap;

/* For each block, a bitmap of registers set in the block.
   This is used by expr_killed_p and compute_transp.
   It is computed during hash table computation and not by compute_sets
   as it includes registers added since the last pass (or between cprop and
   gcse) and it's currently not easy to realloc sbitmap vectors.  */
extern sbitmap *reg_set_in_block;

/* Array, indexed by basic block number for a list of insns which modify
   memory within that block.  */
extern rtx * modify_mem_list;
extern bitmap modify_mem_list_set;

/* This array parallels modify_mem_list, but is kept canonicalized.  */
extern rtx * canon_modify_mem_list;
extern bitmap canon_modify_mem_list_set;

/* Various variables for statistics gathering.  */

/* Memory used in a pass.
   This isn't intended to be absolutely precise.  Its intent is only
   to keep an eye on memory usage.  */
extern int bytes_used;

/* GCSE substitutions made.  */
extern int gcse_subst_count;
/* Number of copy instructions created.  */
extern int gcse_create_count;
/* Number of constants propagated.  */
extern int const_prop_count;
/* Number of copys propagated.  */
extern int copy_prop_count;

/* Entry point.  */
extern int gcse_main			PARAMS ((rtx, FILE *));

/* Passes.  */
extern int one_classic_gcse_pass	PARAMS ((int));
extern int one_code_hoisting_pass	PARAMS ((void));
extern int one_cprop_pass		PARAMS ((int, int, int *, int));
extern int one_pre_gcse_pass		PARAMS ((int));
extern void store_motion		PARAMS ((void));
extern int delete_null_pointer_checks	PARAMS ((rtx));

#endif
