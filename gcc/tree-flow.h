/* Data and Control Flow Analysis for Trees.
   Copyright (C) 2001 Free Software Foundation, Inc.
   Contributed by Diego Novillo <dnovillo@redhat.com>

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef _TREE_FLOW_H
#define _TREE_FLOW_H 1

#include "bitmap.h"

/* Types of references.  */

enum treeref_type { VARDEF, VARUSE, VARPHI, 
		    EXPRPHI, EXPRUSE, EXPRKILL, EXPRINJ };

union varref_def;


/* Common features of every variable reference.  */
struct treeref_common
{
  /* Base symbol.  */
  tree sym;

  /* Statement containing the reference.  */
  tree stmt;

  /* Expression tree containing the reference.  */
  tree expr;

  /* Pointer to operand of EXPR containing the reference.  */
  tree *operand_p;

  /* Basic block containing the reference.  */
  basic_block bb;

  /* Reference type.  */
  enum treeref_type type;

  /* Reference ID.  Unique within a single function.  */
  unsigned int id;
};


/* Stuff common to all expression references.  */
struct exprref_common
{
  struct treeref_common common;

  /* SSAPRE: True if expression needs to be saved to a temporary. */
  unsigned int save:1;
  
  /* SSAPRE: True if expression needs to be reloaded from a
     temporary.  */
  unsigned int reload:1;

  /* SSAPRE: True if expression was inserted as a PHI operand
     occurrence.  */
  unsigned int inserted:1;
  
  /* SSAPRE: Redundancy class of expression.  */
  unsigned int class;
};


/* Expression PHI's */

struct exprphi
{
  struct exprref_common common;
  
  /* Expression PHI operands.  This is an array of size
     last_basic_block, with operands ordered by pre-order index of the
     BB it's from.  */
  varray_type phi_chain;

  /* SSAPRE: True if PHI is downsafe.  */
  unsigned int downsafe:1;
  
  /* SSAPRE: True if PHI is can_be_avail.  */
  unsigned int can_be_avail:1;

  /* SSAPRE: True if PHI is later.  */
  unsigned int later:1;

  /* SSAPRE: True if PHI is expression.  */
  unsigned int extraneous:1;

  /* SSAPRE: Bitmap to track which operands we've processed during
     various algorithms.  */
  bitmap processed;
};

#define EXPRPHI_PHI_CHAIN(r) (r)->ephi.phi_chain
#define EXPRPHI_DOWNSAFE(r) (r)->ephi.downsafe
#define EXPRPHI_CANBEAVAIL(r) (r)->ephi.can_be_avail
#define EXPRPHI_LATER(r) (r)->ephi.later
#define EXPRPHI_EXTRANEOUS(r) (r)->ephi.extraneous
#define EXPRPHI_PROCESSED(r) (r)->ephi.processed
#define EXPRPHI_WILLBEAVAIL(r) (EXPRPHI_CANBEAVAIL ((r)) && !EXPRPHI_LATER ((r)))


/* Doubly linked list of variable references.  */
struct ref_list_node
{
  union varref_def *ref;
  struct ref_list_node *prev;
  struct ref_list_node *next;
};

struct ref_list_priv
{
  struct ref_list_node *first;
  struct ref_list_node *last;
};
typedef struct ref_list_priv *ref_list;


/* Variable definitions.  */
struct vardef
{
  struct treeref_common common;

  /* Immediate uses for this definition.  */
  ref_list imm_uses;

  /* Saved definition chain.  */
  union varref_def *save_chain;

  /* Uses reached by this definition.  */
  ref_list ruses;

  /* Visited mark.  Used when computing reaching definitions.  */
  union varref_def *marked;

  /* PHI arguments (not used with real definitions).  The number of
     arguments to a PHI node is the number of incoming edges to the basic
     block where that PHI node resides.  Each argument is of type phi_arg.  */
  varray_type phi_args;
};

#define VARDEF_IMM_USES(r) (r)->def.imm_uses
#define VARDEF_SAVE_CHAIN(r) (r)->def.save_chain
#define VARDEF_RUSES(r) (r)->def.ruses
#define VARDEF_MARKED(r) (r)->def.marked
#define VARDEF_PHI_ARGS(r) (r)->def.phi_args

/* Variable uses.  */
struct varuse
{
  struct exprref_common common;

  /* Immediate reaching definition for this use.  */
  union varref_def *imm_rdef;

  /* Definitions reaching this use.  */
  ref_list rdefs;
};

#define VARUSE_IMM_RDEF(r) (r)->use.imm_rdef
#define VARUSE_RDEFS(r) (r)->use.rdefs


/* Expressions uses.  */
struct expruse
{
  struct exprref_common common;

  /* Definition chain.  */
  union varref_def *def;
  
  /* True if this use is a phi operand occurrence. */
  int op_occurrence;

  /* SSAPRE: True if this is an operand, and it has a real use. */
  int has_real_use;
};

#define EXPRUSE_DEF(r) (r)->euse.def
#define EXPRUSE_PHIOP(r) (r)->euse.op_occurrence
#define EXPRUSE_HAS_REAL_USE(r) (r)->euse.has_real_use



/* Generic variable reference structure.  */
union varref_def
{
  struct treeref_common common;
  struct exprref_common ecommon;
  struct vardef def;
  struct varuse use;
  struct expruse euse;
  struct exprphi ephi;
};

typedef union varref_def *varref;

#define VARREF_ID(r) (r)->common.id
#define VARREF_TYPE(r) (r)->common.type
#define VARREF_BB(r) (r)->common.bb
#define VARREF_EXPR(r) (r)->common.expr
#define VARREF_OPERAND_P(r) (r)->common.operand_p
#define VARREF_STMT(r) (r)->common.stmt
#define VARREF_SYM(r) (r)->common.sym

#define EXPRREF_TYPE(r) (r)->common.type
#define EXPRREF_BB(r) (r)->common.bb
#define EXPRREF_EXPR(r) (r)->common.expr
#define EXPRREF_STMT(r) (r)->common.stmt
#define EXPRREF_SYM(r) (r)->common.sym
#define EXPRREF_CLASS(r) (r)->ecommon.class
#define EXPRREF_INSERTED(r) (r)->ecommon.inserted
#define EXPRREF_SAVE(r) (r)->ecommon.save
#define EXPRREF_RELOAD(r) (r)->ecommon.reload


/* PHI node arguments.  */
typedef struct
{
  /* Definition reaching this argument.  */
  varref def;

  /* Incoming edge where DEF is coming from.  */
  edge e;
} *phi_arg;

static inline unsigned get_num_phi_args PARAMS ((varref));
static inline phi_arg get_phi_arg PARAMS ((varref, unsigned int));
static inline void set_phi_arg PARAMS ((varref, unsigned int, phi_arg));
extern void add_phi_arg PARAMS ((varref, varref, edge));


/* Return nonzero if R is a default definition.  Default definitions are
   artificially created in the first basic block of the program.  They
   provide a convenient way of checking if a variable is used without being
   assigned a value first.  Their presence is not required, but they save
   the code from having to consider special cases like nil PHI node
   arguments.  */
#define IS_DEFAULT_DEF(R)		\
    (R					\
     && VARREF_TYPE (R) == VARDEF	\
     && VARREF_EXPR (R) == NULL_TREE	\
     && VARREF_STMT (R) == NULL_TREE	\
     && VARREF_BB (R)->index == 0)

/* Return nonzero if R is an artificial definition (currently, a PHI term
   or a default definition).  */
#define IS_ARTIFICIAL_REF(R)	\
    (IS_DEFAULT_DEF (R) || VARREF_TYPE (R) == VARPHI)


/* Tree annotations stored in tree_common.aux.  */
struct tree_ann_def
{
  /* Basic block that contains this tree.  */
  basic_block bb;

  /* For _DECL trees, list of references made to this variable.  For _STMT
     trees, list of references made in this statement.  For first-level
     SIMPLE expressions (i.e., the topmost expression of a _STMT node),
     list of references made in this expression.  */
  ref_list refs;

  /* For _DECL trees this is the most recent definition for this symbol.
     Used when placing FUD chains.  For assignment expressions, this is the
     VARDEF being defined on the LHS of the assignment.  Note that
     assignments may define more than one symbol (e.g., a.b = 5 defines
     'a.b' and 'a').  However, in this case currdef only holds the VARDEF
     D for which VARREF_SYM (D) == TREE_OPERAND (assignment, 0).  */
  varref currdef;

  /* Immediately enclosing compound statement to which this tree belongs.  */
  tree compound_stmt;

  /* Flags used to mark optimization-dependent state.  See TF_* below.  */
  int flags;
};

typedef struct tree_ann_def *tree_ann;

#define TREE_ANN(NODE)		((tree_ann)((NODE)->common.aux))
#define BB_FOR_STMT(NODE)	TREE_ANN (NODE)->bb
#define TREE_CURRDEF(NODE) 	TREE_ANN (NODE)->currdef
#define TREE_REFS(NODE)		TREE_ANN (NODE)->refs
#define TREE_COMPOUND_STMT(NODE) TREE_ANN (NODE)->compound_stmt
#define TREE_FLAGS(NODE)	TREE_ANN (NODE)->flags

/* Tree flags.  */
#define TF_FOLD		1	/* Expression tree should be folded.  */
#define TF_NOT_SIMPLE	2	/* The expression is not in SIMPLE form.  */


/* Block annotations stored in basic_block.aux.  */
struct for_header_blocks
{
  basic_block for_init_stmt_bb;
  basic_block for_cond_bb;
  basic_block for_expr_bb;
};

union header_blocks
{
  struct for_header_blocks for_hdr;
  basic_block end_while_bb;
  basic_block do_cond_bb;
};

/* Functions to manipulate lists of references (ref_list).  */
ref_list create_ref_list PARAMS ((void));
void empty_ref_list PARAMS ((ref_list));
void delete_ref_list PARAMS ((ref_list));
void add_ref_to_list_end PARAMS ((ref_list, varref));
void add_ref_to_list_begin PARAMS ((ref_list, varref));
void remove_ref_from_list PARAMS ((ref_list, varref));


#define FOR_REF_BETWEEN(REF, TMP, FROM, TO, DIR)		\
  if (FROM) \
  for (TMP = FROM, REF = TMP->ref;  TMP != TO; TMP = TMP->DIR, REF = (TMP ? TMP->ref : NULL))

#define FOR_EACH_REF(REF, TMP, LIST)				\
  FOR_REF_BETWEEN (REF, TMP, LIST->first, LIST->last->next, next)

#define FOR_EACH_REF_REV(REF, TMP, LIST)			\
  FOR_REF_BETWEEN (REF, TMP, LIST->last, LIST->first->prev, prev)

struct bb_ann_def
{
  /* Control flow parent.  */
  basic_block parent;

  /* List of references made in this block.  */
  ref_list refs;

  /* Address into the tree preceeding bb->head_tree that contains a pointer
     to bb->head_tree.  Used when we need to insert statements before the
     first statement of the block (see insert_stmt_tree_before).  */
  tree *prev_chain_p;

  /* Block that starts the enclosing binding scope for this block.  */
  basic_block binding_scope;

  /* For the entry block of a control structure, header_blocks is a
     structure containing all the associated header blocks.  */
  union header_blocks *loop_hdr;
};

typedef struct bb_ann_def *bb_ann;

/* Accessors for basic block annotations.  */
#define BB_ANN(BLOCK)		((bb_ann)((BLOCK)->aux))
#define BB_PARENT(BLOCK)	BB_ANN (BLOCK)->parent
#define BB_REFS(BLOCK)		BB_ANN (BLOCK)->refs
#define BB_PREV_CHAIN_P(BLOCK)	BB_ANN (BLOCK)->prev_chain_p
#define BB_BINDING_SCOPE(BLOCK)	BB_ANN (BLOCK)->binding_scope
#define BB_LOOP_HDR(BLOCK)	BB_ANN (BLOCK)->loop_hdr

/* Accessors for obtaining header blocks of loop statements.  */
#define FOR_INIT_STMT_BB(BLOCK)	BB_LOOP_HDR (BLOCK)->for_hdr.for_init_stmt_bb
#define FOR_COND_BB(BLOCK)	BB_LOOP_HDR (BLOCK)->for_hdr.for_cond_bb
#define FOR_EXPR_BB(BLOCK)	BB_LOOP_HDR (BLOCK)->for_hdr.for_expr_bb
#define END_WHILE_BB(BLOCK)	BB_LOOP_HDR (BLOCK)->end_while_bb
#define DO_COND_BB(BLOCK)	BB_LOOP_HDR (BLOCK)->do_cond_bb

/* Some basic blocks are nothing but markers used to give structure to the
   flow graph (see make_while_stmt_blocks).  They contain no useful
   instructions.  */
#define BB_EMPTY_P(BLOCK)	((BLOCK)->head_tree == integer_one_node \
                                 && (BLOCK)->end_tree == integer_one_node)

/* Global declarations.  */

/* Nonzero to warn about variables used before they are initialized.  */
extern int tree_warn_uninitialized;

/* Array of all symbols referenced in the function.  */
extern varray_type referenced_symbols;

/* Next unique reference ID to be assigned by create_ref().  */
extern unsigned int next_varref_id;


/* Accessor macros for referenced_symbols array.  */
#define NREF_SYMBOLS		VARRAY_ACTIVE_SIZE (referenced_symbols)
#define REF_SYMBOL(N)		VARRAY_TREE (referenced_symbols, (N))
#define ADD_REF_SYMBOL(T)	VARRAY_PUSH_TREE (referenced_symbols, (T));

/* Bitmasks to select which function calls to return in get_fcalls().  */
#define FCALL_NON_PURE	(1 << 0)
#define FCALL_PURE	(1 << 1)
#define FCALL_BUILT_IN	(1 << 2)



/* Functions in tree-cfg.c  */

extern void tree_find_basic_blocks PARAMS ((tree));
extern int is_ctrl_stmt PARAMS ((tree));
extern int is_ctrl_altering_stmt PARAMS ((tree));
extern int is_loop_stmt PARAMS ((tree));
extern tree loop_body PARAMS ((tree));
extern void set_loop_body PARAMS ((tree, tree));
extern int stmt_ends_bb_p PARAMS ((tree));
extern int stmt_starts_bb_p PARAMS ((tree));
extern void delete_cfg PARAMS ((void));
extern bb_ann get_bb_ann PARAMS ((basic_block));
extern void tree_dump_bb PARAMS ((FILE *, const char *, basic_block, int));
extern void tree_debug_bb PARAMS ((basic_block));
extern void tree_dump_cfg PARAMS ((FILE *));
extern void tree_debug_cfg PARAMS ((void));
extern void tree_cfg2dot PARAMS ((FILE *));
extern basic_block loop_parent PARAMS ((basic_block));
extern basic_block latch_block PARAMS ((basic_block));
extern basic_block switch_parent PARAMS ((basic_block));
extern tree first_exec_stmt PARAMS ((tree));
extern tree last_exec_stmt PARAMS ((tree));
extern int is_exec_stmt PARAMS ((tree));
extern int is_statement_expression PARAMS ((tree));
extern void validate_loops PARAMS ((struct loops *));
extern tree first_non_decl_stmt PARAMS ((tree));
extern tree first_decl_stmt PARAMS ((tree));
extern tree first_non_label_in_bb PARAMS ((basic_block));
extern void insert_stmt_tree_before PARAMS ((tree, tree, basic_block));
extern void insert_stmt_tree_after PARAMS ((tree, tree, basic_block));
extern void replace_expr_in_tree PARAMS ((tree, tree, tree));
extern tree *find_expr_in_tree PARAMS ((tree, tree));
extern void insert_bb_before PARAMS ((basic_block, basic_block));
extern void tree_cleanup_cfg PARAMS ((void));
extern basic_block tree_split_bb PARAMS ((basic_block, tree));


/* Functions in tree-dfa.c  */

extern void tree_find_varrefs PARAMS ((void));
extern tree_ann get_tree_ann PARAMS ((tree));
extern void remove_tree_ann PARAMS ((tree));
extern varref create_ref PARAMS ((tree, enum treeref_type,
				  basic_block, tree, tree, tree *));
extern void debug_varref PARAMS ((varref));
extern void dump_varref PARAMS ((FILE *, const char *, varref, int, int));
extern void debug_varref_list PARAMS ((ref_list));
extern void debug_varref_array PARAMS ((varray_type));
extern void debug_phi_args PARAMS ((varray_type));
extern void dump_varref_list PARAMS ((FILE *, const char *, ref_list, int,
                                      int));
extern void dump_varref_array PARAMS ((FILE *, const char *, varray_type, int,
                                      int));
extern void dump_phi_args PARAMS ((FILE *, const char *, varray_type, int,
                                      int));
extern int function_may_recurse_p PARAMS ((void));
extern void get_fcalls PARAMS ((varray_type *, unsigned));
extern basic_block find_declaration PARAMS ((tree));


/* Functions in tree-ssa.c  */

extern void tree_build_ssa PARAMS ((void));
extern void tree_compute_rdefs PARAMS ((void));
extern void analyze_rdefs PARAMS ((void));
extern int is_upward_exposed PARAMS ((tree, sbitmap, int));
extern void delete_ssa PARAMS ((void));
extern void tree_ssa_remove_phi_alternative PARAMS ((varref, basic_block));

/* Functions in tree-alias-steen.c  */
extern void create_alias_vars PARAMS ((void));


/* Inline functions.  */

/* Return the number of arguments for PHI.  */

static inline unsigned
get_num_phi_args (phi)
     varref phi;
{
  return VARRAY_ACTIVE_SIZE (VARDEF_PHI_ARGS (phi));
}


/* Return the Ith argument of PHI.  Each PHI argument contains a VARDEF and
   an edge where that VARDEF is coming from.  */

static inline phi_arg
get_phi_arg (phi, i)
     varref phi;
     unsigned int i;
{
  return (phi_arg)(VARRAY_GENERIC_PTR (VARDEF_PHI_ARGS (phi), i));
}


/* Set the Ith argument of PHI to be ARG.  */

static inline void
set_phi_arg (phi, i, arg)
     varref phi;
     unsigned int i;
     phi_arg arg;
{
  VARRAY_GENERIC_PTR (VARDEF_PHI_ARGS (phi), i) = (PTR)arg;
}

#endif /* _TREE_FLOW_H  */
