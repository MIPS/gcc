/* Form lists of pseudo register references for autoinc optimization
   for GNU compiler.  This is part of flow optimization.
   Copyright (C) 1999, 2000, 2001, 2003, 2004, 2005
   Free Software Foundation, Inc.
   Contributed by Michael P. Hayes (m.hayes@elec.canterbury.ac.nz)

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

#ifndef GCC_DF_H
#define GCC_DF_H

#include "bitmap.h"
#include "basic-block.h"
#include "alloc-pool.h"

#define DF_RD		1	/* Reaching definitions.  */
#define DF_RU		2	/* Reaching uses.  */
#define DF_LR		4	/* Live registers.  */
#define DF_UR           8	/* Uninitialized registers.  */
#define DF_DU_CHAIN	16	/* Def-use chain.  */
#define DF_UD_CHAIN	32	/* Use-def chain.  */
#define DF_REG_INFO	64	/* Register info.  */
#define DF_RD_CHAIN	128	/* Reg-def chain.  */
#define DF_RU_CHAIN	256	/* Reg-use chain.  */
#define DF_ALL		511

#define DF_HARD_REGS	2048	/* Mark hard registers.  */
#define DF_EQUIV_NOTES	4096	/* Mark uses present in EQUIV/EQUAL notes.  */
#define DF_SUBREGS	8192	/* Return subregs rather than the inner reg.  */
#define DF_ARTIFICIAL_USES  16384  /* Create artificial uses for 
				      always live registers, etc.  */

/* The way that registers are processed, especially hard registers,
   changes as the compilation proceeds. These states are passed to
   df_set_state to control this processing.  */

#define DF_SCAN_INITIAL    1    /* Processing from beginning of rtl to
				   global-alloc.  */
#define DF_SCAN_GLOBAL     2    /* Processing before global
				   allocation.  */
#define DF_SCAN_POST_ALLOC 4    /* Processing after register
				   allocation.  */

enum df_ref_type {DF_REF_REG_DEF, DF_REF_REG_USE, DF_REF_REG_MEM_LOAD,
		  DF_REF_REG_MEM_STORE};

#define DF_REF_TYPE_NAMES {"def", "use", "mem load", "mem store"}

/* Link on a def-use or use-def chain.  */
struct df_link
{
  struct df_link *next;
  struct ref *ref;
};

enum df_ref_flags
  {
    /* Read-modify-write refs generate both a use and a def and
       these are marked with this flag to show that they are not
       independent.  */
    DF_REF_READ_WRITE = 1,

    /* This flag is set, if we stripped the subreg from the reference.
       In this case we must make conservative guesses, at what the
       outer mode was.  */
    DF_REF_STRIPPED = 2,
    
    /* If this flag is set, this is not a real definition/use, but an
       artificial one created to model always live registers, eh uses, etc.  */
    DF_REF_ARTIFICIAL = 4,

    /* This flag is set if the use is inside a REG_EQUAL note.  */
    DF_REF_IN_NOTE = 8
    
  };


/* Define a register reference structure.  One of these is allocated
   for every register reference (use or def).  Note some register
   references (e.g., post_inc, subreg) generate both a def and a use.  */
struct ref
{
  rtx reg;			/* The register referenced.  */
  unsigned int regno;           /* The register number referenced.  */
  rtx insn;			/* Insn containing ref.  */
  rtx *loc;			/* The location of the reg.  */
  struct df_link *chain;	/* Head of def-use or use-def chain.  */
  unsigned int id;		/* Ref index.  */
  enum df_ref_type type;	/* Type of ref.  */
  enum df_ref_flags flags;	/* Various flags.  */
  void *data;			/* The data assigned to it by user.  */
};


/* One of these structures is allocated for every insn.  */
struct insn_info
{
  struct df_link *defs;		/* Head of insn-def chain.  */
  struct df_link *uses;		/* Head of insn-use chain.  */
  /* ???? The following luid field should be considered private so that
     we can change it on the fly to accommodate new insns?  */
  int luid;			/* Logical UID.  */
  bool contains_asm;            /* Contains an asm instruction.  */
};


/* One of these structures is allocated for every reg.  */
struct reg_info
{
  struct df_link *defs;		/* Head of reg-def chain.  */
  struct df_link *uses;		/* Head of reg-use chain.  */
  int lifetime;
  int n_defs;
  int n_uses;
};


/* One of these structures is allocated for every basic block.  */
struct df_bb_info
{
  /* Reaching def bitmaps have def_id elements.  */
  bitmap rd_kill;
  bitmap rd_gen;
  bitmap rd_in;
  bitmap rd_out;
  /* Reaching use bitmaps have use_id elements.  */
  bitmap ru_kill;
  bitmap ru_gen;
  bitmap ru_in;
  bitmap ru_out;
  /* Live register bitmaps have n_regs elements.  */
  bitmap lr_def;
  bitmap lr_use;
  bitmap lr_in;
  bitmap lr_out;
  /* Reaching register bitmaps have n_regs elements.  */
  bitmap ur_earlyclobber;
  bitmap ur_kill;
  bitmap ur_gen;
  bitmap ur_in;
  bitmap ur_out;
  int rd_valid;
  int ru_valid;
  int lr_valid;
  int ur_valid;
  struct df_link *artificial_uses;
};

struct df
{
  int flags;			/* Indicates what's recorded.  */
  int state;                    /* Indicates where we are in the compilation.  */
  struct df_bb_info *bbs;	/* Basic block table.  */
  struct ref **defs;		/* Def table, indexed by def_id.  */
  struct ref **uses;		/* Use table, indexed by use_id.  */
  struct ref **reg_def_last;	/* Indexed by regno.  */
  struct reg_info *regs;	/* Regs table, index by regno.  */
  unsigned int reg_size;	/* Size of regs table.  */
  struct insn_info *insns;	/* Insn table, indexed by insn UID.  */
  unsigned int insn_size;	/* Size of insn table.  */
  unsigned int def_id;		/* Next def ID.  */
  unsigned int def_size;	/* Size of def table.  */
  unsigned int n_defs;		/* Size of def bitmaps.  */
  unsigned int use_id;		/* Next use ID.  */
  unsigned int use_size;	/* Size of use table.  */
  unsigned int n_uses;		/* Size of use bitmaps.  */
  unsigned int n_bbs;		/* Number of basic blocks.  */
  unsigned int n_regs;		/* Number of regs.  */
  unsigned int def_id_save;	/* Saved next def ID.  */
  unsigned int use_id_save;	/* Saved next use ID.  */
  bitmap insns_modified;	/* Insns that (may) have changed.  */
  bitmap bbs_modified;		/* Blocks that (may) have changed.  */
  bitmap all_blocks;		/* All blocks in CFG.  */
  bitmap hardware_regs_used;    /* The set of hardware registers used.  */
#ifdef STACK_REGS
  bitmap stack_regs;		/* Registers that may be allocated to a STACK_REGS.  */
#endif
  int *dfs_order;		/* DFS order -> block number.  */
  int *postorder;		/* Reverse completion order -> block number.  */
  int *reverse_postorder;	/* Reverse top sort order -> block number.  */
  bool earlyclobbers_found;     /* True if any instruction contains an
				   earlyclobber.  */
  alloc_pool ref_pool;
  alloc_pool link_pool;
};



struct df_map
{
  rtx old;
  rtx new;
};


#define DF_BB_INFO(DF, BB) (&DF->bbs[(BB)->index])

/* Most transformations that wish to use live register analysis will
   use these macros.  The DF_UPWARD_LIVE* macros are only half of the
   solution.  */
#define DF_LIVE_IN(DF, BB) (DF_BB_INFO(DF, BB)->ur_in) 
#define DF_LIVE_OUT(DF, BB) (DF_BB_INFO(DF, BB)->ur_out) 

/* These macros are currently used by only reg-stack since it is not
   tolerant of uninitialized variables.  This intolerance should be
   fixed because it causes other problems.  */ 
#define DF_UPWARD_LIVE_IN(DF, BB) (DF_BB_INFO(DF, BB)->lr_in) 
#define DF_UPWARD_LIVE_OUT(DF, BB) (DF_BB_INFO(DF, BB)->lr_out) 

/* This next macro gives access to the old information before the live
   register rewrite. It should not be used except in old consisteny
   tests.  */
#define DF_LL_LIVE_IN(DF, BB) (DF_BB_INFO(DF, BB)->lr_in)


/* Macros to access the elements within the ref structure.  */


#define DF_REF_REAL_REG(REF) (GET_CODE ((REF)->reg) == SUBREG \
				? SUBREG_REG ((REF)->reg) : ((REF)->reg))
#define DF_REF_REGNO(REF) ((REF)->regno)
#define DF_REF_REAL_LOC(REF) (GET_CODE ((REF)->reg) == SUBREG \
			        ? &SUBREG_REG ((REF)->reg) : ((REF)->loc))
#define DF_REF_REG(REF) ((REF)->reg)
#define DF_REF_LOC(REF) ((REF)->loc)
#define DF_REF_BB(REF) (BLOCK_FOR_INSN ((REF)->insn))
#define DF_REF_BBNO(REF) (BLOCK_FOR_INSN ((REF)->insn)->index)
#define DF_REF_INSN(REF) ((REF)->insn)
#define DF_REF_INSN_UID(REF) (INSN_UID ((REF)->insn))
#define DF_REF_TYPE(REF) ((REF)->type)
#define DF_REF_CHAIN(REF) ((REF)->chain)
#define DF_REF_ID(REF) ((REF)->id)
#define DF_REF_FLAGS(REF) ((REF)->flags)
#define DF_REF_DATA(REF) ((REF)->data)

/* Macros to determine the reference type.  */

#define DF_REF_REG_DEF_P(REF) (DF_REF_TYPE (REF) == DF_REF_REG_DEF)
#define DF_REF_REG_USE_P(REF) ((REF) && ! DF_REF_REG_DEF_P (REF))
#define DF_REF_REG_MEM_STORE_P(REF) (DF_REF_TYPE (REF) == DF_REF_REG_MEM_STORE)
#define DF_REF_REG_MEM_LOAD_P(REF) (DF_REF_TYPE (REF) == DF_REF_REG_MEM_LOAD)
#define DF_REF_REG_MEM_P(REF) (DF_REF_REG_MEM_STORE_P (REF) \
                               || DF_REF_REG_MEM_LOAD_P (REF))


/* Macros to access the elements within the reg_info structure table.  */

#define DF_REGNO_FIRST_DEF(DF, REGNUM) \
((DF)->regs[REGNUM].defs ? (DF)->regs[REGNUM].defs->ref : 0)
#define DF_REGNO_LAST_USE(DF, REGNUM) \
((DF)->regs[REGNUM].uses ? (DF)->regs[REGNUM].uses->ref : 0)

#define DF_REGNO_FIRST_BB(DF, REGNUM) \
((DF)->regs[REGNUM].defs ? DF_REF_BB ((DF)->regs[REGNUM].defs->ref) : 0)
#define DF_REGNO_LAST_BB(DF, REGNUM) \
((DF)->regs[REGNUM].uses ? DF_REF_BB ((DF)->regs[REGNUM].uses->ref) : 0)


/* Macros to access the elements within the insn_info structure table.  */

#define DF_INSN_CONTAINS_ASM(DF, INSN) ((DF)->insns[INSN_UID (INSN)].contains_asm)
#define DF_INSN_LUID(DF, INSN) ((DF)->insns[INSN_UID (INSN)].luid)
#define DF_INSN_DEFS(DF, INSN) ((DF)->insns[INSN_UID (INSN)].defs)
#define DF_INSN_USES(DF, INSN) ((DF)->insns[INSN_UID (INSN)].uses)


/* Functions to build and analyze dataflow information.  */

extern struct df *df_init (void);
extern void df_set_state (struct df *, int);

extern int df_analyze (struct df *, bitmap, int);
extern void df_analyze_subcfg (struct df *, bitmap, int);
extern void df_analyze_simple_change_some_blocks (struct df *, int *, unsigned int);
extern void df_analyze_simple_change_one_block (struct df *, basic_block);

extern void df_finish (struct df *);

extern void df_dump (struct df *, FILE *);


/* Functions to modify insns.  */

extern bool df_insn_modified_p (struct df *, rtx);

extern void df_insn_modify (struct df *, basic_block, rtx);

extern rtx df_insn_delete (struct df *, basic_block, rtx);

extern rtx df_pattern_emit_before (struct df *, rtx, basic_block, rtx);

extern rtx df_jump_pattern_emit_after (struct df *, rtx, basic_block, rtx);

extern rtx df_pattern_emit_after (struct df *, rtx, basic_block, rtx);

extern rtx df_insn_move_before (struct df *, basic_block, rtx, basic_block,
				rtx);

extern struct df_link *df_chain_copy (struct df *, struct df_link *);

extern int df_reg_replace (struct df *, bitmap, rtx, rtx);

extern int df_ref_reg_replace (struct df *, struct ref *, rtx, rtx);

extern struct ref *df_ref_create (struct df *, rtx, rtx *, rtx,  
                                  enum df_ref_type, enum df_ref_flags);

extern int df_ref_remove (struct df *, struct ref *);

extern int df_insn_mem_replace (struct df *, basic_block, rtx, rtx, rtx);

extern struct ref *df_bb_def_use_swap (struct df *, basic_block, rtx, rtx,
				       unsigned int);


/* Functions to query dataflow information.  */

extern basic_block df_regno_bb (struct df *, unsigned int);

extern int df_reg_lifetime (struct df *, rtx);

extern int df_reg_global_p (struct df *, rtx);

extern int df_insn_regno_def_p (struct df *, basic_block, rtx, unsigned int);

extern int df_insn_dominates_all_uses_p (struct df *, basic_block, rtx);

extern int df_insn_dominates_uses_p (struct df *, basic_block, rtx, bitmap);

extern bitmap df_bb_reg_live_start (struct df *, basic_block);

extern bitmap df_bb_reg_live_end (struct df *, basic_block);

extern int df_bb_reg_live_start_p (struct df *, basic_block, rtx);

extern int df_bb_reg_live_end_p (struct df *, basic_block, rtx);

extern int df_bb_regs_lives_compare (struct df *, basic_block, rtx, rtx);

extern bool df_local_def_available_p (struct df *, struct ref *, rtx);

extern rtx df_bb_single_def_use_insn_find (struct df *, basic_block, rtx,
					   rtx);
extern struct ref *df_bb_regno_last_use_find (struct df *, basic_block, unsigned int);

extern struct ref *df_bb_regno_first_def_find (struct df *, basic_block, unsigned int);

extern struct ref *df_bb_regno_last_def_find (struct df *, basic_block, unsigned int);

extern void df_insert_def (struct df *, struct ref *, struct df_link *);
 
extern void df_insert_use (struct df *, struct ref *, struct df_link *);
 
extern struct ref *df_find_def (struct df *, rtx, rtx);

extern int df_reg_used (struct df *, rtx, rtx);

extern void df_bb_replace (struct df *, int, basic_block);
/* Functions for debugging from GDB.  */

extern void debug_df_insn (rtx);

extern void debug_df_regno (unsigned int);

extern void debug_df_reg (rtx);

extern void debug_df_defno (unsigned int);

extern void debug_df_useno (unsigned int);

extern void debug_df_ref (struct ref *);

extern void debug_df_chain (struct df_link *);

extern void df_insn_debug (struct df *, rtx, FILE *);

extern void df_insn_debug_regno (struct df *, rtx, FILE *);

/* Dataflow direction.  */
enum df_flow_dir
  {
    DF_FORWARD,
    DF_BACKWARD
  };

struct dataflow;

typedef void (*confluence_function_0) (struct df *, void *, basic_block);
typedef void (*confluence_function_n) (struct df *, void *, void *, edge);
typedef void (*setbits_function) (struct dataflow *, basic_block);
typedef void (*transfer_function) (int, int *, void *, void *,
				   void *, void *, void *);
typedef void (*finalizer_function) (struct df*, bitmap);


/* The static description of a dataflow problem to solve.  */

struct df_problem {
  enum df_flow_dir dir;			/* Dataflow direction.  */
  setbits_function setbitsfun;          /* Function to copy bitvec 
                                           pointers to dflow. */
  confluence_function_0 confun_0;       /* The function to provide
					   information when there are
					   no edges.  */
  confluence_function_n confun_n;       /* The function to merge the
					   bits from the pred blocks
					   into the current block.  */
  transfer_function transfun;		/* The transfer function.  */
  finalizer_function finalizefun;       /* The finalizer function.  */
};


/* The specific instance of the problem to solve.  */
struct dataflow
{
  /* The following arrays are indexed by block indices, so they must always
     be large enough even if we restrict ourselves just to a subset of cfg.  */
  void **gen, **kill;			/* Gen and kill sets.  */
  void **in, **out;			/* Results.  */

  void *data;				/* Data used by the transfer
					   function.  */
  struct df *df;                        /* Instance of df we are working in.  */
  bitmap viewing;                       /* Basic blocks that can be 
					   examined during iteration.  */
  bitmap iterating;                     /* Basic blocks that are to be
					   iterated over.  Must have
					   no blocks not in
					   viewing. */
  int * postorder;                      /* Basic blocks that are to be
					   iterated over for a
					   backwards problem.  */
  int n_blocks;                         /* Number of basic blocks in postorder.  */ 
  bool single_pass;                     /* Stop the iteration after 1 pass.  */
  struct df_problem * problem;          /* The problem to be solved.  */
  sbitmap visited, pending, considered; /* Communication between
                                           iterative_dataflow and
                                           hybrid_search. */
  bitmap init;                          /* Bitmap to or with gen set
					   to initialize problem.  */
};

extern void iterative_dataflow (struct dataflow *);
extern bool read_modify_subreg_p (rtx);
extern void df_compact_blocks (struct df *);

#endif /* GCC_DF_H */
