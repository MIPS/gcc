/* {{{ Data and Control Flow Analysis for Trees.
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

/* }}} */

#ifndef _TREE_FLOW_H
#define _TREE_FLOW_H 1

/* {{{ Types of references.  */

enum varref_type { VARDEF, VARUSE, VARPHI };

struct varref_list_def;
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
  struct varref_list_def *imm_uses;

  /* Saved definition chain.  */
  union varref_def *save_chain;
};

#define VARDEF_IMM_USES(r) (r)->def.imm_uses
#define VARDEF_SAVE_CHAIN(r) (r)->def.save_chain

/* }}} */

/* {{{ Variable uses.  */

struct varuse
{
  struct varref_common common;

  /* Definition chain.  */
  union varref_def *chain;
};

#define VARUSE_CHAIN(r) (r)->use.chain

/* }}} */

/* {{{ PHI terms.  */

struct varphi
{
  struct varref_common common;

  /* PHI arguments.  */
  struct varref_list_def *phi_chain;
};

#define VARPHI_CHAIN(r) (r)->phi.phi_chain

/* }}} */


/* {{{ Generic variable reference structure.  */

union varref_def
{
  struct varref_common common;
  struct vardef def;
  struct varuse use;
  struct varphi phi;
};

typedef union varref_def *varref;

#define VARREF_TYPE(r) (r)->common.type
#define VARREF_BLOCK(r) (r)->common.bb
#define VARREF_EXPR(r) (r)->common.expr
#define VARREF_STMT(r) (r)->common.stmt
#define VARREF_SYM(r) (r)->common.sym
#define VARREF_NEXT(r) (r)->common.next
#define VARREF_PREV(r) (r)->common.prev

/* }}} */

/* {{{ Container types for variable references.  */

struct varref_node_def
{
  varref elem;
  struct varref_node_def *next;
  struct varref_node_def *prev;
};

typedef struct varref_node_def *varref_node;

#define VARREF_NODE_ELEM(n) ((n) ? (n)->elem : NULL)
#define VARREF_NODE_NEXT(n) ((n) ? (n)->next : NULL)
#define VARREF_NODE_PREV(n) ((n) ? (n)->prev : NULL)


struct varref_list_def
{
  varref_node first;
  varref_node last;
};

typedef struct varref_list_def *varref_list;


#define VARREF_LIST_FIRST(l) ((l) ? (l)->first : NULL)
#define VARREF_LIST_LAST(l) ((l) ? (l)->last : NULL)

/* }}} */

/* {{{ Tree annotations stored in tree_common.aux.  */

struct tree_ann_def
{
  /* Basic block that contains this tree.  */
  basic_block bb;

  /* For _DECL trees, list of references made to this variable.  */
  varref_list refs;

  /* Most recent definition for this symbol. Used when placing FUD
     chains.  */
  varref currdef;
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

/* }}} */
  
/* {{{ Block annotations stored in basic_block.aux.  */

struct basic_block_ann_def
{
  /* Control flow parent.  */
  basic_block parent;

  /* List of references made in this block.  */
  varref_list refs;

  /* Control flow altering statement found in the block (CONTINUE, BREAK
     RETURN, or GOTO).  NOTE: there can be only one.  */
  tree exit_stmt;

  /* Loop header flag.  If set, this block belongs to the head of a loop
     construct (e.g., the block for a FOR_COND expression).  */
  int is_loop_header;
};

typedef struct basic_block_ann_def *basic_block_ann;

#define BB_ANN(NODE)		\
    ((basic_block_ann)((NODE)->aux))

#define BB_PARENT(NODE)		\
    ((BB_ANN (NODE)) ? BB_ANN (NODE)->parent : NULL)

#define BB_REFS(NODE)		\
    ((BB_ANN (NODE)) ? BB_ANN (NODE)->refs : NULL)

#define BB_EXIT_STMT(NODE)	\
    ((BB_ANN (NODE)) ? BB_ANN (NODE)->exit_stmt : NULL)

#define BB_IS_LOOP_HEADER(NODE)	\
    ((BB_ANN (NODE)) ? BB_ANN (NODE)->is_loop_header : 0)

/* }}} */

/* {{{ Global symbols.  */

/* List of all symbols referenced in the function.  */

extern tree ref_symbols_list;


/* Nonzero to warn about code which is never reached.  */

extern int warn_notreached;

/* }}} */


/* {{{ Functions in tree-cfg.c  */

extern void tree_find_basic_blocks PARAMS ((tree, int, FILE *));
extern int is_last_block_of_loop PARAMS ((basic_block));
extern int is_ctrl_stmt PARAMS ((tree));
extern int is_ctrl_altering_stmt PARAMS ((tree));
extern int is_loop_stmt PARAMS ((tree));
extern int stmt_ends_bb_p PARAMS ((tree));
extern int stmt_starts_bb_p PARAMS ((tree));
extern void delete_cfg PARAMS ((void));
extern basic_block_ann get_bb_ann PARAMS ((basic_block));
extern void tree_dump_bb PARAMS ((FILE *, const char *, basic_block, int));
extern void tree_debug_bb PARAMS ((basic_block));
extern void tree_debug_cfg PARAMS ((void));
extern void tree_cfg2dot PARAMS ((char *fname));
extern basic_block find_loop_parent PARAMS ((basic_block));
extern basic_block get_condition_block PARAMS ((basic_block));

/* }}} */

/* {{{ Functions in tree-dfa.c  */

extern void tree_find_varrefs PARAMS ((tree, tree));
extern void add_ref_to_sym PARAMS ((varref, tree sym, int));
extern void add_ref_to_bb PARAMS ((varref, basic_block));
extern int add_ref_symbol PARAMS ((tree, tree *));
extern void remove_ann_from_sym PARAMS ((tree));
extern tree_ann get_tree_ann PARAMS ((tree));
extern varref create_varref PARAMS ((tree, enum varref_type,
				     basic_block, tree, tree));
extern varref_list create_varref_list PARAMS ((varref));
extern void push_ref PARAMS ((varref_list, varref));
extern void delete_varref_list PARAMS ((tree *));
extern void debug_varref PARAMS ((varref));
extern void dump_varref PARAMS ((FILE *, const char *, varref, int, int));
extern void debug_varref_list PARAMS ((varref_list));
extern void dump_varref_list PARAMS ((FILE *, const char *, varref_list, int, int));

/* }}} */

/* {{{ Functions in tree-ssa.c  */

extern void tree_build_ssa PARAMS ((void));
extern void delete_ssa PARAMS ((void));

/* }}} */

#endif	/* _TREE_FLOW_H  */
