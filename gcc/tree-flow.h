/* Data and Control Flow Analysis for Trees.
   Copyright (C) 2001, 2003 Free Software Foundation, Inc.
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

  /* Nonzero if this variable has uses which may not appear
     in the IL.  This can happen in the following cases:

       1. If the variable is used in a variable length
          array declaration.

	2. If the variable is the return value in a C++
	   function where the named return value optimization
	   has been performed.  */
  unsigned has_hidden_use : 1;

  /* Nonzero if this variable was stored/written in the function.  */
  unsigned is_stored : 1;

  /* Nonzero if the variable may be modified by function calls.  */
  unsigned is_call_clobbered : 1;

  /* Used by the out of SSA pass to determine whether this variable has
     been seen yet or not.  */
  unsigned out_of_ssa_tag : 1;

  /* Used when building root_var structures in tree_ssa_live.[ch].  */
  unsigned root_var_processed : 1;

  /* Nonzero if this variable is a memory tag used to represent the memory
     pointed-to by the pointer in MEM_TAG.  */
  unsigned is_mem_tag : 1;

  /* Nonzero if this variable is an alias tag that represents references to
     other variables (i.e., this variable appears in the MAY_ALIASES array
     of other variables).  */
  unsigned is_alias_tag : 1;

  /* Nonzero if this variable was used after SSA optimizations were
     applied.  We set this when translating out of SSA form.  */
  unsigned used : 1;

  /* Nonzero if this variable is used as the argument to VA_ARG_EXPR.  This
     forces all operands to this variable to always be virtual, because
     VA_ARG_EXPR both reads and modifies its argument and it can't be
     modified by optimizations.  */
  unsigned is_in_va_arg_expr : 1;

  /* A VAR_DECL used to associated pointers with the memory location that
     they are pointing to.  If IS_MEM_TAG is nonzero, then MEM_TAG is the
     pointer associated to this memory tag.  If IS_MEM_TAG is zero, then
     MEM_TAG is the memory tag associated to this pointer (see
     compute_alias_sets).  */
  tree mem_tag;

  /* Variables that may alias this variable.  */
  varray_type may_aliases;

  /* Unique ID of this variable.  */
  size_t uid;

  /* Used when going out of SSA form to indicate which partition this
     variable represents storage for.  */
  unsigned partition;

  /* Used by the root-var object in tree-ssa-live.[ch].  */
  unsigned root_index;

  /* The BIND_EXPR in that the variable is declared.  */
  tree scope;

  /* Default definition for this symbol.  If this field is not NULL, it
     means that the first reference to this variable in the function is a
     USE or a VUSE.  In those cases, the SSA renamer creates an SSA name
     for this variable with an empty defining statement.  */
  tree default_def;
};


struct operands_d GTY(())
{
  /* LHS of assignment statements.  */
  varray_type def_ops;

  /* Array of pointers to each operand in the statement.  */
  varray_type use_ops;
};

typedef struct operands_d *operands_t;


struct voperands_d GTY(())
{
  /* List of VDEF references in this statement.  */
  varray_type vdef_ops;

  /* List of VUSE references in this statement.  */
  varray_type vuse_ops;
};

typedef struct voperands_d *voperands_t;


struct dataflow_d GTY(())
{
  /* Immediate uses.  This is a list of all the statements and PHI nodes
     that are immediately reached by the definitions made in this
     statement.  */
  varray_type immediate_uses;

  /* Use this array for very small numbers of uses instead of the varray.  */
  tree uses[2];

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

  /* Reaching definitions.  Similarly to REACHED_USES, the set
     REACHING_DEFS is the set of all the statements that make definitions
     that may reach this statement.  Notice that we don't need to have a
     similar entry for immediate definitions, as these are represented by
     the SSA_NAME nodes themselves (each SSA_NAME node contains a pointer
     to the statement that makes that definition).  */
};

typedef struct dataflow_d *dataflow_t;


struct stmt_ann_d GTY(())
{
  struct tree_ann_common_d common;

  /* Nonzero if the statement has been modified (meaning that the operands
     need to be scanned again).  */
  unsigned modified : 1;

  /* Nonzero if the statement is in the CCP worklist and has not been
     "cancelled".  If we ever need to use this bit outside CCP, then
     it should be renamed.  */
  unsigned in_ccp_worklist: 1;

  /* Nonzero if the statement makes aliased loads.  */
  unsigned makes_aliased_loads : 1;

  /* Nonzero if the statement makes aliased stores.  */
  unsigned makes_aliased_stores : 1;

  /* Nonzero if the statement makes references to volatile storage.  */
  unsigned has_volatile_ops : 1;

  /* Nonzero if the statement makes a function call that may clobber global
     and local addressable variables.  */
  unsigned makes_clobbering_call : 1;

  /* Basic block that contains this statement.  */
  basic_block GTY ((skip (""))) bb;

  /* Statement operands.  */
  operands_t ops;

  /* Virtual operands (VDEF and VUSE).  */
  voperands_t vops;

  /* Dataflow information.  */
  dataflow_t df;

  /* Control flow parent.  This is the entry statement to the control
     structure to which this statement belongs to.  */
  tree parent_stmt;

  /* Array of variables that have had their address taken in the statement.  */
  varray_type addresses_taken;

  /* The BIND_EXPR to that the statement belongs.  */
  tree scope;

  /* For BIND_EXPR, its level in the tree of BIND_EXPRs.  */
  int scope_level;
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

static inline var_ann_t var_ann (tree);
static inline var_ann_t get_var_ann (tree);
static inline stmt_ann_t stmt_ann (tree);
static inline stmt_ann_t get_stmt_ann (tree);
static inline enum tree_ann_type ann_type (tree_ann);
static inline basic_block bb_for_stmt (tree);
extern void set_bb_for_stmt (tree, basic_block);
static inline void modify_stmt (tree);
static inline void unmodify_stmt (tree);
static inline bool stmt_modified_p (tree);
static inline varray_type may_aliases (tree);
static inline void set_may_alias_global_mem (tree);
static inline bool may_alias_global_mem_p (tree);
static inline bool may_point_to_global_mem_p (tree);
static inline void set_may_point_to_global_mem (tree);
static inline int get_lineno (tree);
static inline const char *get_filename (tree);
static inline bool is_exec_stmt (tree);
static inline bool is_label_stmt (tree);
static inline varray_type vdef_ops (tree);
static inline varray_type vuse_ops (tree);
static inline varray_type use_ops (tree);
static inline varray_type def_ops (tree);
static inline varray_type addresses_taken (tree);
static inline int num_immediate_uses (dataflow_t);
static inline tree immediate_use (dataflow_t, int);
static inline dataflow_t get_immediate_uses (tree);
static inline bool has_hidden_use (tree);
static inline void set_has_hidden_use (tree);
static inline tree parent_stmt (tree);
static inline void set_default_def (tree, tree);
static inline tree default_def (tree);


/*---------------------------------------------------------------------------
		  Block annotations stored in basic_block.tree_annotations
---------------------------------------------------------------------------*/
struct bb_ann_d
{
  /* Chain of PHI nodes created in this block.  */
  tree phi_nodes;

  /* Chain of EPHI nodes created in this block.  */
  tree ephi_nodes;
  
  /* EUSE/ELEFT/EKILL/EPHI nodes created in this block.  */
  varray_type erefs;

  /* Set of blocks immediately dominated by this node.  */
  bitmap dom_children;
};

typedef struct bb_ann_d *bb_ann_t;

/* Accessors for basic block annotations.  */
static inline bb_ann_t bb_ann (basic_block);
static inline basic_block parent_block (basic_block);
static inline tree phi_nodes (basic_block);
static inline void add_dom_child (basic_block, basic_block);
static inline bitmap dom_children (basic_block);


/*---------------------------------------------------------------------------
		 Iterators for statements inside a basic block
---------------------------------------------------------------------------*/

/* Iterator object for traversing over BASIC BLOCKs.  */

typedef struct {
  tree *tp;
  tree context;		/* Stack for decending into BIND_EXPR's.  */
} block_stmt_iterator;

extern block_stmt_iterator bsi_start (basic_block);
extern block_stmt_iterator bsi_last (basic_block);
static inline bool bsi_end_p (block_stmt_iterator);
static inline void bsi_next (block_stmt_iterator *);
extern void bsi_prev (block_stmt_iterator *);
static inline tree bsi_stmt (block_stmt_iterator);
static inline tree *bsi_stmt_ptr (block_stmt_iterator);
static inline tree *bsi_container (block_stmt_iterator);

extern block_stmt_iterator bsi_from_tsi (tree_stmt_iterator);
static inline tree_stmt_iterator tsi_from_bsi (block_stmt_iterator);

extern void bsi_remove (block_stmt_iterator *);

enum bsi_iterator_update
{
  BSI_NEW_STMT,
  BSI_SAME_STMT
};

/* Single stmt insertion routines.  */

extern void bsi_insert_before (block_stmt_iterator *, tree, enum bsi_iterator_update);
extern void bsi_insert_after (block_stmt_iterator *, tree, enum bsi_iterator_update);
extern void bsi_insert_on_edge (edge, tree);
extern int bsi_commit_edge_inserts (int, int *);
extern block_stmt_iterator bsi_insert_on_edge_immediate
 (edge, tree, block_stmt_iterator *, basic_block *);

extern void bsi_replace (block_stmt_iterator, tree);

/* Stmt list insertion routines.  */

extern void bsi_insert_list_before (block_stmt_iterator *, tree_stmt_anchor);
extern void bsi_insert_list_after (block_stmt_iterator *, tree_stmt_anchor);
extern block_stmt_iterator bsi_insert_list_on_edge (edge, tree_stmt_anchor);

void bsi_next_in_bb (block_stmt_iterator *, basic_block);

/*---------------------------------------------------------------------------
			      Global declarations
---------------------------------------------------------------------------*/
/* Array of all variables referenced in the function.  */
extern GTY(()) varray_type referenced_vars;

#define num_referenced_vars VARRAY_ACTIVE_SIZE (referenced_vars)
#define referenced_var(i) VARRAY_TREE (referenced_vars, i)

/* Artificial variable used to model the effects of function calls.  */
extern GTY(()) tree global_var;

/* Array of all variables that are call clobbered in the function.  */
extern GTY(()) varray_type call_clobbered_vars;

#define num_call_clobbered_vars	VARRAY_ACTIVE_SIZE (call_clobbered_vars)
#define add_call_clobbered_var(v) VARRAY_PUSH_TREE (call_clobbered_vars, v)
#define call_clobbered_var(i) VARRAY_TREE (call_clobbered_vars, i)

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
extern void build_tree_cfg (tree);
extern void delete_tree_cfg (void);
extern bool is_ctrl_structure (tree);
extern bool is_ctrl_stmt (tree);
extern bool is_ctrl_altering_stmt (tree);
extern bool is_computed_goto (tree);
extern void dump_tree_bb (FILE *, const char *, basic_block, int);
extern void debug_tree_bb (basic_block);
extern basic_block debug_tree_bb_n (int);
extern void dump_tree_cfg (FILE *, int);
extern void debug_tree_cfg (int);
extern void dump_cfg_stats (FILE *);
extern void debug_cfg_stats (void);
extern void tree_cfg2dot (FILE *);
extern void insert_bb_before (basic_block, basic_block);
extern void cleanup_tree_cfg (void);
extern bool remove_unreachable_blocks (void);
extern void remove_phi_nodes_and_edges_for_unreachable_block (basic_block);
extern tree first_stmt (basic_block);
extern tree last_stmt (basic_block);
extern tree *last_stmt_ptr (basic_block);
extern edge find_taken_edge (basic_block, tree);
extern int call_expr_flags (tree);
extern void remove_useless_stmts_and_vars (tree *, bool);
extern basic_block tree_split_edge (edge);
extern void bsi_move_before (block_stmt_iterator, block_stmt_iterator);
extern void bsi_move_after (block_stmt_iterator, block_stmt_iterator);
extern void bsi_move_to_bb_end (block_stmt_iterator, basic_block);
extern basic_block label_to_block (tree);

/* In tree-dfa.c  */
void find_referenced_vars (tree);
extern void get_stmt_operands (tree);
extern var_ann_t create_var_ann (tree);
extern stmt_ann_t create_stmt_ann (tree);
extern tree create_phi_node (tree, basic_block);
extern void add_phi_arg (tree *, tree, edge);
extern void remove_phi_arg (tree, basic_block);
extern void remove_phi_arg_num (tree, int);
extern void remove_phi_node (tree, tree, basic_block);
extern void remove_all_phi_nodes_for (sbitmap);
extern void dump_dfa_stats (FILE *);
extern void debug_dfa_stats (void);
extern void debug_referenced_vars (void);
extern void dump_referenced_vars (FILE *);
extern void dump_variable (FILE *, tree);
extern void debug_variable (tree);
extern void dump_may_aliases_for (FILE *, tree);
extern void debug_may_aliases_for (tree);
extern void dump_immediate_uses (FILE *);
extern void debug_immediate_uses (void);
extern void dump_immediate_uses_for (FILE *, tree);
extern void debug_immediate_uses_for (tree);
extern void remove_decl (tree, tree);
extern tree *find_decl_location (tree, tree);
extern void compute_may_aliases (tree);
extern void compute_reached_uses (int);
extern void compute_immediate_uses (int, bool (*)(tree));
extern void compute_reaching_defs (int);
extern void dump_alias_info (FILE *);
extern void debug_alias_info (void);
extern tree get_virtual_var (tree);
extern void add_vuse (tree, tree, voperands_t);
extern void create_global_var (void);
extern void add_referenced_tmp_var (tree var);
extern void mark_new_vars_to_rename (tree, sbitmap);

/* Flags used when computing reaching definitions and reached uses.  */
#define TDFA_USE_OPS		1 << 0
#define TDFA_USE_VOPS		1 << 1


/* In tree-ssa.c  */
extern void init_tree_ssa (void);
extern void rewrite_into_ssa (tree, sbitmap, enum tree_dump_index);
extern void rewrite_out_of_ssa (tree, enum tree_dump_index);
extern void dump_reaching_defs (FILE *);
extern void debug_reaching_defs (void);
extern void dump_tree_ssa (FILE *);
extern void debug_tree_ssa (void);
extern void debug_def_blocks (void);
extern void dump_tree_ssa_stats (FILE *);
extern void debug_tree_ssa_stats (void);
extern void ssa_remove_edge (edge);
extern void set_is_used (tree);
extern bool tree_ssa_useless_type_conversion (tree);
extern void build_dominator_tree (dominance_info);
extern unsigned int next_ssa_version;

/* In tree-ssa-pre.c  */
extern void tree_perform_ssapre (tree, enum tree_dump_index);

/* In tree-ssa-ccp.c  */
void tree_ssa_ccp (tree, sbitmap, enum tree_dump_index);
bool fold_stmt (tree *);
tree widen_bitfield (tree, tree, tree);

/* In tree-ssa-dom.c  */
extern void tree_ssa_dominator_optimize (tree, sbitmap, enum tree_dump_index);
extern void dump_dominator_optimization_stats (FILE *);
extern void debug_dominator_optimization_stats (void);

/* In tree-ssa-dce.c  */
void tree_ssa_dce (tree, enum tree_dump_index);


/* In tree-ssa-copyprop.c  */
void tree_ssa_copyprop (tree, enum tree_dump_index);
void propagate_copy (tree *, tree, tree);
void fixup_var_scope (tree, tree);

/* In tree-flow-inline.h  */
static inline int phi_arg_from_edge (tree, edge);
static inline struct phi_arg_d *phi_element_for_edge (tree, edge);
static inline bool may_propagate_copy (tree, tree);

/* In tree-must-alias.c  */
void tree_compute_must_alias (tree, sbitmap, enum tree_dump_index);

/* In tree-eh.c  */
extern void lower_eh_constructs (tree *);
extern void make_eh_edges (tree);
extern bool tree_could_trap_p (tree);
extern bool tree_could_throw_p (tree);
extern bool tree_can_throw_internal (tree);
extern bool tree_can_throw_external (tree);

#include "tree-flow-inline.h"

#endif /* _TREE_FLOW_H  */
