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

/* {{{ Types of references.  */

enum varref_type { VARDEF, VARUSE, VARPHI };

union varref_def;

/* }}} */

/* {{{ Common features of every variable reference.  */

struct varref_common
{
  /* Base symbol.  */
  tree sym;

  /* Statement containing the reference.  */
  tree stmt;

  /* Sub-tree containing the reference.  */
  tree expr;

  /* Basic block containing the reference.  */
  basic_block bb;

  /* Reference type.  */
  enum varref_type type;
};

/* }}} */

/* {{{ Variable definitions.  */

struct vardef
{
  struct varref_common common;

  /* Immediate uses for this definition.  */
  varray_type imm_uses;

  /* Saved definition chain.  */
  union varref_def *save_chain;

  /* Uses reached by this definition.  */
  varray_type ruses;

  /* Visited mark.  Used when computing reaching definitions.  */
  union varref_def *marked;

  /* PHI arguments (not used with real definitions).  */
  varray_type phi_chain;
};

#define VARDEF_IMM_USES(r) (r)->def.imm_uses
#define VARDEF_SAVE_CHAIN(r) (r)->def.save_chain
#define VARDEF_RUSES(r) (r)->def.ruses
#define VARDEF_MARKED(r) (r)->def.marked
#define VARDEF_PHI_CHAIN(r) (r)->def.phi_chain

/* }}} */

/* {{{ Variable uses.  */

struct varuse
{
  struct varref_common common;

  /* Definition chain.  */
  union varref_def *chain;

  /* Definitions reaching this use.  */
  varray_type rdefs;
};

#define VARUSE_CHAIN(r) (r)->use.chain
#define VARUSE_RDEFS(r) (r)->use.rdefs

/* }}} */

/* {{{ Generic variable reference structure.  */

union varref_def
{
  struct varref_common common;
  struct vardef def;
  struct varuse use;
};

typedef union varref_def *varref;

#define VARREF_TYPE(r) (r)->common.type
#define VARREF_BB(r) (r)->common.bb
#define VARREF_EXPR(r) (r)->common.expr
#define VARREF_STMT(r) (r)->common.stmt
#define VARREF_SYM(r) (r)->common.sym

/* Return non-zero if R is a ghost definition.  */
#define IS_GHOST_DEF(R)		\
    (R && VARREF_TYPE (R) == VARDEF && VARREF_BB (R) == ENTRY_BLOCK_PTR)

/* Return non-zero if R is an artificial definition (currently, a PHI term
   or a ghost definition).  */
#define IS_ARTIFICIAL_REF(R)	\
    (IS_GHOST_DEF (R) || VARREF_TYPE (R) == VARPHI)

/* }}} */

/* {{{ Tree annotations stored in tree_common.aux.  */

struct tree_ann_def
{
  /* Basic block that contains this tree.  */
  basic_block bb;

  /* For _DECL trees, list of references made to this variable.  */
  varray_type refs;

  /* Most recent definition for this symbol.  Used when placing FUD
     chains.  */
  varref currdef;

  /* Immediately enclosing compound statement to which this tree belongs.  */
  tree compound_stmt;
};

typedef struct tree_ann_def *tree_ann;

#define TREE_ANN(NODE)		\
    ((tree_ann)((NODE)->common.aux))

#define BB_FOR_STMT(NODE)	\
    ((TREE_ANN (NODE)) ? TREE_ANN (NODE)->bb : NULL)

#define TREE_CURRDEF(NODE) 	\
    ((TREE_ANN (NODE)) ? TREE_ANN (NODE)->currdef : NULL)

#define TREE_REFS(NODE)		\
    ((TREE_ANN (NODE)) ? TREE_ANN (NODE)->refs : NULL)

#define TREE_COMPOUND_STMT(NODE)\
    ((TREE_ANN (NODE)) ? TREE_ANN (NODE)->compound_stmt : NULL)

/* }}} */

/* {{{ Block annotations stored in basic_block.aux.  */

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

struct bb_ann_def
{
  /* Control flow parent.  */
  basic_block parent;

  /* List of references made in this block.  */
  varray_type refs;

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

/* }}} */

/* {{{ Global declarations.  */

/* Nonzero to warn about variables used before they are initialized.  */

extern int tree_warn_uninitialized;


/* Array of all symbols referenced in the function.  */

extern varray_type referenced_symbols;

/* Accessor macros for referenced_symbols array.  */

#define NREF_SYMBOLS		VARRAY_ACTIVE_SIZE (referenced_symbols)
#define REF_SYMBOL(N)		VARRAY_TREE (referenced_symbols, (N))
#define ADD_REF_SYMBOL(T)	VARRAY_PUSH_TREE (referenced_symbols, (T));

/* Bitmasks to select which function calls to return in get_fcalls().  */
#define FCALL_NON_PURE	(1 << 0)
#define FCALL_PURE	(1 << 1)
#define FCALL_BUILT_IN	(1 << 2)

/* }}} */


/* {{{ Functions in tree-cfg.c  */

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

/* }}} */

/* {{{ Functions in tree-dfa.c  */

extern void tree_find_varrefs PARAMS ((void));
extern tree_ann get_tree_ann PARAMS ((tree));
extern varref create_varref PARAMS ((tree, enum varref_type,
				     basic_block, tree, tree));
extern void debug_varref PARAMS ((varref));
extern void dump_varref PARAMS ((FILE *, const char *, varref, int, int));
extern void debug_varref_list PARAMS ((varray_type));
extern void dump_varref_list PARAMS ((FILE *, const char *, varray_type, int,
                                      int));
extern int function_may_recurse_p PARAMS ((void));
extern void get_fcalls PARAMS ((varray_type *, unsigned));
extern basic_block find_declaration PARAMS ((tree));

/* }}} */

/* {{{ Functions in tree-ssa.c  */

extern void tree_build_ssa PARAMS ((void));
extern void tree_compute_rdefs PARAMS ((void));
extern void analyze_rdefs PARAMS ((void));
extern int is_upward_exposed PARAMS ((tree, sbitmap, int));
extern void delete_ssa PARAMS ((void));

/* }}} */

#endif /* _TREE_FLOW_H  */
