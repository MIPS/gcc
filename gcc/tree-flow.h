/* Data and Control Flow Analysis for Trees.
   Copyright (C) 2001 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef _TREE_FLOW_H
#define _TREE_FLOW_H 1

#include "bitmap.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "hashtab.h"
#include "tree-simple.h"

/* Forward declare structures for the garbage collector GTY markers.  */
#ifndef GCC_BASIC_BLOCK_H
struct edge_def;
typedef struct edge_def *edge;
struct basic_block_def;
typedef struct basic_block_def *basic_block;
#endif

/*---------------------------------------------------------------------------
		   Tree annotations stored in tree_common.ann
---------------------------------------------------------------------------*/
enum tree_ann_type { TREE_ANN_COMMON, VAR_ANN, STMT_ANN };

struct tree_ann_common_d GTY(())
{
  /* Annotation type.  */
  enum tree_ann_type type;
};


struct var_ann_d GTY(())
{
  struct tree_ann_common_d common;

  /* Nonzero if this variable may alias global memory.  */
  unsigned may_alias_global_mem : 1;

  /* Nonzero if this pointer may point to global memory.  */
  unsigned may_point_to_global_mem : 1;

  /* Nonzero if this variable is used to declare a VLA (see
     find_vla_decl_r).  */
  unsigned is_vla_decl : 1;

  /* An INDIRECT_REF expression representing all the dereferences of this
     pointer.  Used to store aliasing information for pointer dereferences
     (see add_stmt_operand and find_vars_r).  */
  tree indirect_ref;

  /* Variables that may alias this variable.  */
  varray_type may_aliases;
  
  /* Unique ID of this variable.  */
  int uid;
};


struct operands_d GTY(())
{
  /* LHS of assignment statements.  */
  tree  * GTY ((length ("1"))) def_op;

  /* Array of pointers to each operand in the statement.  */
  varray_type use_ops;
};

typedef struct operands_d *operands_t;


struct voperands_d GTY(())
{
  /* List of V_DEF references in this statement.  */
  varray_type vdef_ops;

  /* List of V_USE references in this statement.  */
  varray_type vuse_ops;
};

typedef struct voperands_d *voperands_t;


struct dataflow_d GTY(())
{
  /* Immediate uses.  This is a list of all the statements and PHI nodes
     that are immediately reached by the definitions made in this
     statement.  */
  varray_type immediate_uses;

  /* Reached uses.  This is a list of all the possible program statements
     that may be reached directly or indirectly by definitions made in this
     statement.  Notice that this is a superset of IMMEDIATE_USES. 
     For instance, given the following piece of code:

	    1	a1 = 10;
	    2	if (a1 > 3)
	    3	  a2 = a1 + 5;
	    4	a3 = PHI (a1, a2)
	    5	b1 = a3 - 2;

     IMMEDIATE_USES for statement #1 are all those statements that use a1
     directly (i.e., #2, #3 and #4).  REACHED_USES for statement #1 also
     includes statement #5 because 'a1' could reach 'a3' via the PHI node
     at statement #4.  The set of REACHED_USES is then the transitive
     closure over all the PHI nodes in the IMMEDIATE_USES set.  */
  varray_type reached_uses;

  /* Reaching definitions.  Similarly to REACHED_USES, the set
     REACHING_DEFS is the set of all the statements that make definitions
     that may reach this statement.  Notice that we don't need to have a
     similar entry for immediate definitions, as these are represented by
     the SSA_NAME nodes themselves (each SSA_NAME node contains a pointer
     to the statement that makes that definition).  */
  varray_type reaching_defs;
};

typedef struct dataflow_d *dataflow_t;


struct stmt_ann_d GTY(())
{
  struct tree_ann_common_d common;

  /* Nonzero if the statement has been modified (meaning that the operands
     need to be scanned again).  */
  unsigned modified : 1;

  /* Basic block that contains this statement.  */
  basic_block GTY ((skip (""))) bb;

  /* Statement operands.  */
  operands_t ops;

  /* Virtual operands (VDEF and VUSE).  */
  voperands_t vops;

  /* Dataflow information.  */
  dataflow_t df;
};


union tree_ann_d GTY((desc ("ann_type ((tree_ann)&%h)")))
{
  struct tree_ann_common_d GTY((tag ("TREE_ANN_COMMON"))) common;
  struct var_ann_d GTY((tag ("VAR_ANN"))) decl;
  struct stmt_ann_d GTY((tag ("STMT_ANN"))) stmt;
};

typedef union tree_ann_d *tree_ann;
typedef struct var_ann_d *var_ann_t;
typedef struct stmt_ann_d *stmt_ann_t;

static inline var_ann_t var_ann			PARAMS ((tree));
static inline stmt_ann_t stmt_ann		PARAMS ((tree));
static inline enum tree_ann_type ann_type	PARAMS ((tree_ann));
static inline basic_block bb_for_stmt		PARAMS ((tree));
extern void set_bb_for_stmt      		PARAMS ((tree, basic_block));
static inline void modify_stmt			PARAMS ((tree));
static inline void unmodify_stmt		PARAMS ((tree));
static inline bool stmt_modified_p		PARAMS ((tree));
static inline tree create_indirect_ref		PARAMS ((tree));
static inline varray_type may_aliases		PARAMS ((tree));
static inline void set_may_alias_global_mem	PARAMS ((tree));
static inline bool may_alias_global_mem_p 	PARAMS ((tree));
static inline bool may_point_to_global_mem_p 	PARAMS ((tree));
static inline void set_may_point_to_global_mem	PARAMS ((tree));
static inline void set_indirect_ref		PARAMS ((tree, tree));
static inline tree indirect_ref			PARAMS ((tree));
static inline int get_lineno			PARAMS ((tree));
static inline const char *get_filename		PARAMS ((tree));
static inline bool is_exec_stmt			PARAMS ((tree));
static inline varray_type vdef_ops		PARAMS ((tree));
static inline varray_type vuse_ops		PARAMS ((tree));
static inline varray_type use_ops		PARAMS ((tree));
static inline tree *def_op			PARAMS ((tree));
static inline varray_type immediate_uses	PARAMS ((tree));
static inline varray_type reaching_defs		PARAMS ((tree));
static inline bool is_vla_decl			PARAMS ((tree));
static inline void set_vla_decl			PARAMS ((tree));


/*---------------------------------------------------------------------------
		  Block annotations stored in basic_block.aux
---------------------------------------------------------------------------*/
struct bb_ann_d
{
  /* Control flow parent.  This is the entry block to the control structure
     to which this block belongs to.  */
  basic_block parent_block;

  /* Chain of PHI nodes created in this block.  */
  tree phi_nodes;

  /* Set of blocks immediately dominated by this node.  */
  bitmap dom_children;
};

typedef struct bb_ann_d *bb_ann_t;

/* Accessors for basic block annotations.  */
static inline bb_ann_t bb_ann		PARAMS ((basic_block));
static inline basic_block parent_block	PARAMS ((basic_block));
static inline void set_parent_block	PARAMS ((basic_block, basic_block));
static inline tree phi_nodes		PARAMS ((basic_block));
static inline void add_dom_child	PARAMS ((basic_block, basic_block));
static inline bitmap dom_children	PARAMS ((basic_block));

/* Some basic blocks are nothing but markers used to give structure to the
   flow graph (see make_while_stmt_blocks).  They contain no useful
   instructions.  */
static inline bool bb_empty_p		PARAMS ((basic_block));
static inline bool
bb_empty_p (b)
     basic_block b;
{
  return *(b->head_tree_p) == empty_stmt_node;
}


/*---------------------------------------------------------------------------
		 Iterators for statements inside a basic block
---------------------------------------------------------------------------*/

/* Iterator object for traversing over BASIC BLOCKs.  */

typedef struct {
  tree *tp;
  tree context;		/* Stack for decending into BIND_EXPR's.  */
} block_stmt_iterator;

extern block_stmt_iterator bsi_start 	PARAMS ((basic_block));
static inline bool bsi_end_p		PARAMS ((block_stmt_iterator));
static inline void bsi_next		PARAMS ((block_stmt_iterator *));
static inline void bsi_prev		PARAMS ((block_stmt_iterator *));
static inline tree bsi_stmt		PARAMS ((block_stmt_iterator));
static inline tree *bsi_stmt_ptr	PARAMS ((block_stmt_iterator));
static inline tree *bsi_container	PARAMS ((block_stmt_iterator));

extern void bsi_remove			PARAMS ((block_stmt_iterator));

void bsi_next_in_bb			PARAMS ((block_stmt_iterator *, basic_block));

/*---------------------------------------------------------------------------
			      Global declarations
---------------------------------------------------------------------------*/
/* Nonzero to warn about variables used before they are initialized.  */
extern int tree_warn_uninitialized;

/* Array of all variables referenced in the function.  */
extern GTY(()) varray_type referenced_vars;

/* Next unique reference ID to be assigned by create_ref().  */
extern unsigned long next_tree_ref_id;

/* Artificial variable used to model the effects of function calls.  */
extern GTY(()) tree global_var;

/* Accessors for the referenced_vars array.  */
extern unsigned long num_referenced_vars;

static inline tree referenced_var PARAMS ((size_t));
static inline tree
referenced_var (i)
     size_t i;
{
  return VARRAY_TREE (referenced_vars, i);
}

/* Macros for showing usage statistics.  */
#define SCALE(x) ((unsigned long) ((x) < 1024*10	\
		  ? (x)					\
		  : ((x) < 1024*1024*10			\
		     ? (x) / 1024			\
		     : (x) / (1024*1024))))

#define LABEL(x) ((x) < 1024*10 ? 'b' : ((x) < 1024*1024*10 ? 'k' : 'M'))

#define PERCENT(x,y) ((float)(x) * 100.0 / (float)(y))


/*---------------------------------------------------------------------------
			      Function prototypes
---------------------------------------------------------------------------*/
/* In tree-cfg.c  */
extern void build_tree_cfg		PARAMS ((tree));
extern void delete_tree_cfg		PARAMS ((void));
extern bool is_ctrl_stmt		PARAMS ((tree));
extern bool is_ctrl_altering_stmt	PARAMS ((tree));
extern bool is_loop_stmt		PARAMS ((tree));
extern bool is_computed_goto		PARAMS ((tree));
extern tree loop_body			PARAMS ((tree));
extern void set_loop_body		PARAMS ((tree, tree));
extern void dump_tree_bb		PARAMS ((FILE *, const char *,
	                			 basic_block, int));
extern void debug_tree_bb		PARAMS ((basic_block));
extern void dump_tree_cfg		PARAMS ((FILE *, int));
extern void debug_tree_cfg		PARAMS ((int));
extern void dump_cfg_stats		PARAMS ((FILE *));
extern void debug_cfg_stats		PARAMS ((void));
extern void tree_cfg2dot		PARAMS ((FILE *));
extern void insert_bb_before		PARAMS ((basic_block, basic_block));
extern void cleanup_tree_cfg		PARAMS ((void));
extern tree first_stmt			PARAMS ((basic_block));
extern tree last_stmt			PARAMS ((basic_block));
extern tree *last_stmt_ptr		PARAMS ((basic_block));
extern basic_block latch_block		PARAMS ((basic_block));
extern bool is_latch_block		PARAMS ((basic_block));
extern edge find_taken_edge		PARAMS ((basic_block, tree));
extern int call_expr_flags		PARAMS ((tree));


/* In tree-dfa.c  */
extern void get_stmt_operands		PARAMS ((tree));
extern var_ann_t create_var_ann 	PARAMS ((tree));
extern stmt_ann_t create_stmt_ann 	PARAMS ((tree));
extern tree create_phi_node		PARAMS ((tree, basic_block));
extern void add_phi_arg			PARAMS ((tree, tree, edge));
extern void dump_dfa_stats		PARAMS ((FILE *));
extern void debug_dfa_stats		PARAMS ((void));
extern void debug_referenced_vars	PARAMS ((void));
extern void dump_referenced_vars	PARAMS ((FILE *));
extern void dump_variable		PARAMS ((FILE *, tree));
extern void debug_variable		PARAMS ((tree));
extern void dump_immediate_uses		PARAMS ((FILE *));
extern void debug_immediate_uses	PARAMS ((void));
extern void dump_immediate_uses_for	PARAMS ((FILE *, tree));
extern void debug_immediate_uses_for	PARAMS ((tree));
extern void remove_decl			PARAMS ((tree));
extern tree *find_decl_location		PARAMS ((tree, tree));
extern void compute_may_aliases		PARAMS ((void));
extern void compute_reached_uses	PARAMS ((int));
extern void compute_immediate_uses	PARAMS ((int));
extern void compute_reaching_defs	PARAMS ((int));
extern tree copy_stmt			PARAMS ((tree));
extern void dump_alias_info		PARAMS ((FILE *));
extern void debug_alias_info		PARAMS ((void));
extern tree get_virtual_var		PARAMS ((tree));

/* Flags used when computing reaching definitions and reached uses.  */
#define TDFA_USE_OPS		1 << 0
#define TDFA_USE_VOPS		1 << 1


/* In tree-ssa.c  */
extern void rewrite_into_ssa		PARAMS ((tree));
extern void rewrite_out_of_ssa		PARAMS ((tree));
extern void remove_phi_arg		PARAMS ((tree, basic_block));
extern void remove_phi_node		PARAMS ((tree, tree, basic_block));
extern void dump_reaching_defs		PARAMS ((FILE *));
extern void debug_reaching_defs		PARAMS ((void));
extern void dump_tree_ssa		PARAMS ((FILE *));
extern void debug_tree_ssa		PARAMS ((void));
extern void debug_def_blocks		PARAMS ((void));

/* In tree-ssa-pre.c  */
extern void tree_perform_ssapre		PARAMS ((tree));


/* In tree-ssa-ccp.c  */
void tree_ssa_ccp			PARAMS ((tree));


/* In tree-ssa-dce.c  */
void tree_ssa_dce			PARAMS ((tree));

#include "tree-flow-inline.h"

#endif /* _TREE_FLOW_H  */
