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

/*---------------------------------------------------------------------------
			    Types of references

  The compiler tracks references to variables (scalars, arrays and
  structures), indicated with a V_ prefix, and references to expressions
  (function calls, arithmetic expressions, etc), indicated with an E_
  prefix.

  The basic semantics of each referenc type can be altered using the
  modifiers defined below (M_* constants).  These modifiers provide more
  information for optimizers when making transformations.
---------------------------------------------------------------------------*/

/* A V_DEF reference represents a write operation to the associated
   variable.  If no modifier alters the reference, the reference represents
   a killing definition of the associated variable via an assignment
   expression (i.e., all the bits of the variable are modified).  Note that
   unmodified V_DEF references are only allowed for MODIFY_EXPR and
   INIT_EXPR expressions.
   
   In this case, this reference will represent the output value of the
   associated expression.  For instance, 'a = 3' creates a V_DEF reference
   for 'a' and calling output_ref('a = 3') returns this V_DEF.  */
extern const HOST_WIDE_INT V_DEF;

/* A V_USE reference represents a read operation from the associated
   variable.  If no modifier alters the reference, the reference represents
   a full read operation on the variable (i.e., all the bits of the
   variable are read).  */
extern const HOST_WIDE_INT V_USE;

/* A V_PHI represents an SSA PHI operation on the associated variable.  */
extern const HOST_WIDE_INT V_PHI;

/* An E_FCALL reference indicates a function call site.  Its associated
   variable is the CALL_EXPR node.  Used to insert may-def/may-use
   references when the compiler doesn't know what the called function might
   do.  */
extern const HOST_WIDE_INT E_FCALL;

/* The following references are akin to the previous types but used
   when building SSA information for expressions instead of variables
   (see tree-ssa-pre.c)  */
extern const HOST_WIDE_INT E_PHI;
extern const HOST_WIDE_INT E_USE;
extern const HOST_WIDE_INT E_KILL;

/* FIXME: This is not used anywhere.  Needed?  */
extern const HOST_WIDE_INT E_INJ;


/*---------------------------------------------------------------------------
			 Reference type modifiers

  These modifiers are OR'd with the basic reference types to alter the
  semantics of the reference.  For instance, V_DEF|M_CLOBBER means that the
  reference modifies the associated variable in ways that the compiler
  cannot determine.
---------------------------------------------------------------------------*/
/* M_DEFAULT is used to modify V_DEF references to indicate a default
   definition.  Default definitions are artificially created in the first
   basic block of the program.  They provide a convenient way of checking
   if a variable is used without being assigned a value first.  Their
   presence is not required, but they save the code from having to consider
   special cases like nil PHI node arguments.  */
extern const HOST_WIDE_INT M_DEFAULT;

/* M_CLOBBER is used to modify V_DEF references to represent an unknown
   access to the associated variable (ie, the operation may read and/or
   write to the variable).  Since the compiler does not know what the
   statement may be doing to the variable, it assumes that the statement
   might write a new value to it.

   For instance, __asm__ statements, function calls and tree nodes that
   have not been simplified use V_DEF|M_CLOBBERs to represent references.
   V_DEF|M_CLOBBER references are safety devices used when the compiler
   knows that the variable is being accessed, but it doesn't know how.  */
extern const HOST_WIDE_INT M_CLOBBER;

/* M_MAY is used to represent references that may or may not occur at
   runtime.  It is generated to model aliasing.  For instance, given

   		*ptr = 30;

   A V_DEF|M_MAY reference is created for all the variables potentially
   aliased by 'ptr'.  The M_MAY modifier may be removed using alias
   analysis.  V_USE reference may be similarly created.  */
extern const HOST_WIDE_INT M_MAY;

/* M_PARTIAL is used to model partial references to compound structures
   like arrays, structures and complex numbers.  For instance, given

   		a[4] = 10;

   The compiler creates a V_DEF for 'a[4]' and a V_DEF|M_PARTIAL for 'a'.
   Partial definitions are also known as non-killing definitions in the
   literature.  */
extern const HOST_WIDE_INT M_PARTIAL;

/* M_INITIAL modifies a V_DEF reference to indicate that the definition is
   an initial static value for the variable.  Multiple executions of this
   reference do not produce multiple definitions of the variable.  This is
   used to model static variables in C.  For instance,

   		static int counter = 0;

   An initializing definition is created for variable counter.  */
extern const HOST_WIDE_INT M_INITIAL;

/* M_INDIRECT modifies a V_DEF or V_USE reference to indicate that the
   reference is to the memory location pointed by the variable.  For
   instance, the following creates a V_DEF|M_INDIRECT to 'ptr',

   		*ptr = 5;

   FIXME This has the unpleasant side-effect that an indirect V_DEF to a
   pointer also implies a definition for the pointer, because currently the
   FUD chaining algorithm only keeps a single notion of 'current
   definition' for symbols.  In the example above, *ptr = 5 means that ptr
   itself is considered defined.  To fix this inefficiency, we should have
   the notion of 'indirect current definitions' in the FUD chaining
   algorithm (i.e., have a 'currdef' and an 'indirect_currdef' field in the
   tree_ann structure).  */
extern const HOST_WIDE_INT M_INDIRECT;

/* M_VOLATILE modifies a V_DEF or V_USE reference to indicate that it is
   accessing a volatile variable.  Therefore, optimizers should not assume
   anything about it.  For instance,

   		volatile int a = 5;
		int b = a;

   In the above code fragment, we cannot assume that 'b' is assigned the
   value 5.  */
extern const HOST_WIDE_INT M_VOLATILE;


/*---------------------------------------------------------------------------
		 Doubly linked list of variable references
---------------------------------------------------------------------------*/
union tree_ref_d;

struct ref_list_node
{
  union tree_ref_d *ref;
  struct ref_list_node *prev;
  struct ref_list_node *next;
};

struct ref_list_priv
{
  struct ref_list_node *first;
  struct ref_list_node *last;
};
typedef struct ref_list_priv *ref_list;


/* Iterators for reference lists.  */
#define FOR_REF_BETWEEN(REF, TMP, FROM, TO, DIR)		\
  if (FROM) \
  for (TMP = FROM, REF = TMP->ref;  TMP != TO; TMP = TMP->DIR, REF = (TMP ? TMP->ref : NULL))

#define FOR_EACH_REF(REF, TMP, LIST)				\
  FOR_REF_BETWEEN (REF, TMP, LIST->first, LIST->last->next, next)

#define FOR_EACH_REF_REV(REF, TMP, LIST)			\
  FOR_REF_BETWEEN (REF, TMP, LIST->last, LIST->first->prev, prev)


/*---------------------------------------------------------------------------
			      Variable references
---------------------------------------------------------------------------*/
/* Common features of every variable reference.  */
struct tree_ref_common
{
  /* Reference type.  */
  HOST_WIDE_INT type;

  /* Variable being referenced.  This is not necessarily a _DECL node.  It
     may be also be a COMPONENT_REF, ARRAY_REF or an INDIRECT_REF.  */
  tree var;

  /* Statement containing the reference.  Maybe NULL for special references
     (e.g., default definitions inserted at the start of every function).  */
  tree stmt;

  /* Expression tree containing the reference.  Maybe NULL for special
     references (e.g., default definitions inserted at the start of every
     function).  */
  tree expr;

  /* Pointer to operand of EXPR containing VAR.  Used when substituting the
     operand with some other value in transformations like constant
     propagation.  Maybe NULL for special references (e.g., default
     definitions inserted at the start of every function).  */
  tree *operand_p;

  /* Original value stored in *OPERAND_P.  Used by restore_ref_operand.  */
  tree orig_operand;

  /* Basic block containing the reference.  */
  basic_block bb;

  /* Reference ID.  Unique within a single function.  */
  HOST_WIDE_INT id;
};

/* Generic variable references.  */
struct var_ref
{
  struct tree_ref_common common;

  /* Immediate reaching definition for this reference.  This is applicable
     to both variable definitions and uses because we are interested in
     building def-def chains (for non-killing definitions).  */
  union tree_ref_d *imm_rdef;
};

/* Variable definitions.  */
struct var_def
{
  struct var_ref common;

  /* Immediate uses for this definition.  */
  ref_list imm_uses;

  /* Uses reached by this definition.  */
  ref_list reached_uses;
};

/* Variable PHIs.  */
struct var_phi
{
  struct var_def common;

  /* Array of PHI arguments.  The number of arguments to a PHI node is the
     number of incoming edges to the basic block where that PHI node
     resides.  Each member of the array is of type phi_node_arg.  */
  varray_type phi_args;
};

/* Variable uses.  */
struct var_use
{
  struct var_ref common;

  /* Definitions reaching this use.  */
  ref_list rdefs;
};

/* PHI arguments.

   NOTE: These are not regular tree_ref objects!  We used to model them as
   just another tree_ref, but the space overhead for jumpy functions with
   many PHI nodes and arguments was horrible.
   
   All yyparse() functions in the different front ends were causing cc1 to
   grow to the 100-300 Mb range.  Furthermore, the number of references
   would grow into the millions, making the optimizers waste unnecessary
   cycles when traversing all the references in the function.  */
struct phi_node_arg_d
{
  /* Immediate reaching definition for this argument.  */
  union tree_ref_d *def;

  /* Incoming edge where we are receiving imm_rdef from.  */
  edge e;
};

typedef struct phi_node_arg_d *phi_node_arg;

/*---------------------------------------------------------------------------
			     Expression references
---------------------------------------------------------------------------*/
/* Common feature of all expression references.  */
struct expr_ref_common
{
  struct tree_ref_common common;

  /* SSAPRE: True if expression needs to be saved to a temporary. */
  unsigned int save:1;
  
  /* SSAPRE: True if expression needs to be reloaded from a temporary.  */
  unsigned int reload:1;

  /* SSAPRE: True if expression was inserted as a PHI operand occurrence.  */
  unsigned int inserted:1;
  
  /* SSAPRE: Redundancy class of expression.  */
  unsigned int class;
};


/* FIXME: Get rid of these lvalue macros.  */
#define EXPRREF_CLASS(r) (r)->ecommon.class
#define EXPRREF_INSERTED(r) (r)->ecommon.inserted
#define EXPRREF_SAVE(r) (r)->ecommon.save
#define EXPRREF_RELOAD(r) (r)->ecommon.reload

/* Expression PHIs.  */
struct expr_phi
{
  struct expr_ref_common common;
  
  /* Expression PHI operands.  This is an array of size
     last_basic_block, with operands ordered by pre-order index of the
     BB it's from.  */
  varray_type phi_args;

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

#define EXPRPHI_PHI_ARGS(r) (r)->ephi.phi_args
#define EXPRPHI_DOWNSAFE(r) (r)->ephi.downsafe
#define EXPRPHI_CANBEAVAIL(r) (r)->ephi.can_be_avail
#define EXPRPHI_LATER(r) (r)->ephi.later
#define EXPRPHI_EXTRANEOUS(r) (r)->ephi.extraneous
#define EXPRPHI_PROCESSED(r) (r)->ephi.processed
#define EXPRPHI_WILLBEAVAIL(r) (EXPRPHI_CANBEAVAIL ((r)) && !EXPRPHI_LATER ((r)))


/* Expressions uses.  */
struct expr_use
{
  struct expr_ref_common common;

  /* Definition chain.  */
  union tree_ref_d *def;
  
  /* True if this use is a phi operand occurrence. */
  int op_occurrence;

  /* SSAPRE: True if this is an operand, and it has a real use. */
  int has_real_use;
};

#define EXPRUSE_DEF(r) (r)->euse.def
#define EXPRUSE_PHIOP(r) (r)->euse.op_occurrence
#define EXPRUSE_HAS_REAL_USE(r) (r)->euse.has_real_use


/* Generic variable reference structure.  */
union tree_ref_d
{
  struct tree_ref_common common;
  struct var_ref vref;
  struct var_def vdef;
  struct var_phi vphi;
  struct var_use vuse;
  struct expr_ref_common ecommon;
  struct expr_use euse;
  struct expr_phi ephi;
};

typedef union tree_ref_d *tree_ref;


/*---------------------------------------------------------------------------
		    Accessor functions for tree_ref objects.
---------------------------------------------------------------------------*/
/* For tree_ref_common.  */
static inline HOST_WIDE_INT ref_type		PARAMS ((tree_ref));
static inline tree ref_var			PARAMS ((tree_ref));
static inline tree ref_stmt			PARAMS ((tree_ref));
static inline tree ref_expr			PARAMS ((tree_ref));
static inline basic_block ref_bb		PARAMS ((tree_ref));
static inline HOST_WIDE_INT ref_id		PARAMS ((tree_ref));
static inline void replace_ref_operand_with	PARAMS ((tree_ref, tree));
static inline void restore_ref_operand		PARAMS ((tree_ref));


/* For var_ref.  */
static inline ref_list imm_uses			PARAMS ((tree_ref));
static inline ref_list reached_uses		PARAMS ((tree_ref));
static inline tree_ref imm_reaching_def		PARAMS ((tree_ref));
static inline void set_imm_reaching_def		PARAMS ((tree_ref, tree_ref));
static inline ref_list reaching_defs		PARAMS ((tree_ref));
static inline varray_type phi_args		PARAMS ((tree_ref));
static inline unsigned int num_phi_args		PARAMS ((tree_ref));
static inline phi_node_arg phi_arg		PARAMS ((tree_ref, unsigned));
static inline void set_phi_arg			PARAMS ((tree_ref, unsigned,
                                                         phi_node_arg));
extern void add_phi_arg				PARAMS ((tree_ref, tree_ref,
                                                         edge));

/* For phi_node_arg.  */
static inline edge phi_arg_edge			PARAMS ((phi_node_arg));
static inline void set_phi_arg_edge		PARAMS ((phi_node_arg, edge));
static inline tree_ref phi_arg_def		PARAMS ((phi_node_arg));
static inline void set_phi_arg_def		PARAMS ((phi_node_arg,
      							 tree_ref));


/*---------------------------------------------------------------------------
		   Tree annotations stored in tree_common.aux
---------------------------------------------------------------------------*/
/* Tree flags.  */
enum tree_flags
{
  /* Expression tree should be folded.  */
  TF_FOLD	= 1 << 0,

  /* The expression is not in SIMPLE form.  */
  TF_NOT_SIMPLE	= 1 << 1,

  /* This _DECL node has already been referenced in this function.  */
  TF_REFERENCED	= 1 << 2
};

struct tree_ann_d
{
  /* Basic block that contains this tree.  */
  basic_block bb;

  /* For _DECL trees, list of references made to this variable.  For _STMT
     trees, list of references made in this statement.  For first-level
     SIMPLE expressions (i.e., the topmost expression of a _STMT node),
     list of references made in this expression.  */
  ref_list refs;

  /* For _DECL trees this is the most recent definition for this variable.
     Used when placing FUD chains.  */
  tree_ref currdef;

  /* Immediately enclosing compound statement to which this tree belongs.  */
  tree compound_parent;

  /* Flags used to mark optimization-dependent state.  See TF_* below.  */
  HOST_WIDE_INT flags;

  /* Output reference.  This is the V_DEF reference at the LHS of
     assignments (MODIFY_EXPR, INIT_EXPR and DECL_STMTs for static
     variables).  */
  tree_ref output_ref;
};

typedef struct tree_ann_d *tree_ann;

static inline tree_ann tree_annotation	PARAMS ((tree));
static inline bool has_annotation	PARAMS ((tree));
static inline basic_block bb_for_stmt	PARAMS ((tree));
static inline void set_bb_for_stmt	PARAMS ((tree, basic_block));
static inline tree_ref currdef_for	PARAMS ((tree));
static inline void set_currdef_for	PARAMS ((tree, tree_ref));
static inline ref_list tree_refs	PARAMS ((tree));
static inline void add_tree_ref		PARAMS ((tree, tree_ref));
static inline void remove_tree_ref	PARAMS ((tree, tree_ref));
static inline tree compound_parent	PARAMS ((tree));
static inline void set_compound_parent	PARAMS ((tree, tree));
static inline void set_tree_flag	PARAMS ((tree, enum tree_flags));
static inline void clear_tree_flag	PARAMS ((tree, enum tree_flags));
static inline enum tree_flags tree_flags PARAMS ((tree));
static inline void reset_tree_flags	PARAMS ((tree));
static inline tree_ref output_ref	PARAMS ((tree));
static inline void set_output_ref	PARAMS ((tree, tree_ref));

/*---------------------------------------------------------------------------
		  Block annotations stored in basic_block.aux
---------------------------------------------------------------------------*/
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
static inline bb_ann bb_annotation	PARAMS ((basic_block));
static inline basic_block bb_parent	PARAMS ((basic_block));
static inline void set_bb_parent	PARAMS ((basic_block, basic_block));
static inline ref_list bb_refs		PARAMS ((basic_block));
static inline void remove_bb_ref	PARAMS ((basic_block, tree_ref));
static inline tree *prev_chain_p	PARAMS ((basic_block));
static inline void set_prev_chain_p	PARAMS ((basic_block, tree *));
static inline basic_block binding_scope	PARAMS ((basic_block));
static inline void set_binding_scope	PARAMS ((basic_block, basic_block));
static inline union header_blocks *loop_hdr PARAMS ((basic_block));
static inline basic_block for_init_bb	PARAMS ((basic_block));
static inline void set_for_init_bb 	PARAMS ((basic_block, basic_block));
static inline basic_block for_cond_bb	PARAMS ((basic_block));
static inline void set_for_cond_bb	PARAMS ((basic_block, basic_block));
static inline basic_block for_expr_bb	PARAMS ((basic_block));
static inline void set_for_expr_bb	PARAMS ((basic_block, basic_block));
static inline basic_block end_while_bb	PARAMS ((basic_block));
static inline void set_end_while_bb	PARAMS ((basic_block, basic_block));
static inline basic_block do_cond_bb	PARAMS ((basic_block));
static inline void set_do_cond_bb	PARAMS ((basic_block, basic_block));

/* Some basic blocks are nothing but markers used to give structure to the
   flow graph (see make_while_stmt_blocks).  They contain no useful
   instructions.  */
static inline bool bb_empty_p		PARAMS ((basic_block));
static inline bool
bb_empty_p (b)
     basic_block b;
{
  return b->head_tree == integer_one_node && b->end_tree == integer_one_node;
}


/* Global declarations.  */

/* Nonzero to warn about variables used before they are initialized.  */
extern int tree_warn_uninitialized;

/* Array of all variables referenced in the function.  */
extern varray_type referenced_vars;

/* List of all call sites in the current function.  */
extern ref_list call_sites;

/* Next unique reference ID to be assigned by create_ref().  */
extern HOST_WIDE_INT next_tree_ref_id;

/* Accessors for the referenced_vars array.  */
extern size_t num_referenced_vars;

static inline tree referenced_var PARAMS ((size_t));
static inline tree
referenced_var (i)
     size_t i;
{
  return VARRAY_TREE (referenced_vars, i);
}

/* Bitmasks to select which function calls to return in get_fcalls().  */
#define FCALL_NON_PURE	(1 << 0)
#define FCALL_PURE	(1 << 1)
#define FCALL_BUILT_IN	(1 << 2)


/* Functions in tree-cfg.c  */
extern void tree_find_basic_blocks	PARAMS ((tree));
extern int is_ctrl_stmt			PARAMS ((tree));
extern int is_ctrl_altering_stmt	PARAMS ((tree));
extern int is_loop_stmt			PARAMS ((tree));
extern bool is_computed_goto		PARAMS ((tree));
extern tree loop_body			PARAMS ((tree));
extern void set_loop_body		PARAMS ((tree, tree));
extern int stmt_ends_bb_p		PARAMS ((tree));
extern int stmt_starts_bb_p		PARAMS ((tree));
extern void delete_cfg			PARAMS ((void));
extern bb_ann create_bb_ann 		PARAMS ((basic_block));
extern void tree_dump_bb		PARAMS ((FILE *, const char *,
						 basic_block, int));
extern void tree_debug_bb		PARAMS ((basic_block));
extern void tree_dump_cfg		PARAMS ((FILE *));
extern void tree_debug_cfg		PARAMS ((void));
extern void tree_cfg2dot		PARAMS ((FILE *));
extern basic_block loop_parent		PARAMS ((basic_block));
extern basic_block latch_block		PARAMS ((basic_block));
extern basic_block switch_parent	PARAMS ((basic_block));
extern tree first_exec_stmt		PARAMS ((tree));
extern tree last_exec_stmt		PARAMS ((tree));
extern int is_exec_stmt			PARAMS ((tree));
extern int is_statement_expression	PARAMS ((tree));
extern void validate_loops		PARAMS ((struct loops *));
extern tree first_non_decl_stmt		PARAMS ((tree));
extern tree first_decl_stmt		PARAMS ((tree));
extern tree first_non_label_in_bb	PARAMS ((basic_block));
extern void insert_stmt_tree_before	PARAMS ((tree, tree, basic_block));
extern void insert_stmt_tree_after	PARAMS ((tree, tree, basic_block));
extern void replace_expr_in_tree	PARAMS ((tree, tree, tree));
extern tree *find_expr_in_tree		PARAMS ((tree, tree));
extern void insert_bb_before		PARAMS ((basic_block, basic_block));
extern void tree_cleanup_cfg		PARAMS ((void));
extern basic_block tree_split_bb	PARAMS ((basic_block, tree));


/* Functions in tree-dfa.c  */
extern void tree_find_refs		PARAMS ((void));
extern void remove_tree_ann		PARAMS ((tree));
extern tree_ann create_tree_ann 	PARAMS ((tree));
extern tree_ref create_ref		PARAMS ((tree, HOST_WIDE_INT,
						 basic_block, tree, tree,
						 tree *, bool));
extern void debug_ref			PARAMS ((tree_ref));
extern void dump_ref			PARAMS ((FILE *, const char *, tree_ref,
      						 int, int));
extern void debug_ref_list		PARAMS ((ref_list));
extern void debug_ref_array		PARAMS ((varray_type));
extern void debug_phi_args		PARAMS ((varray_type));
extern void dump_ref_list		PARAMS ((FILE *, const char *, ref_list,
      						 int, int));
extern void dump_ref_array		PARAMS ((FILE *, const char *,
                                                 varray_type, int, int));
extern void dump_phi_args		PARAMS ((FILE *, const char *,
      						 varray_type, int, int));
extern void debug_referenced_vars	PARAMS ((void));
extern void dump_referenced_vars	PARAMS ((FILE *));
extern int function_may_recurse_p	PARAMS ((void));
extern void get_fcalls			PARAMS ((varray_type *, unsigned));
extern bool is_pure_fcall		PARAMS ((tree));
extern bool fcall_takes_ref_args	PARAMS ((tree));
extern basic_block find_declaration	PARAMS ((tree));
extern ref_list create_ref_list		PARAMS ((void));
extern void empty_ref_list		PARAMS ((ref_list));
extern void delete_ref_list		PARAMS ((ref_list));
extern void add_ref_to_list_end		PARAMS ((ref_list, tree_ref));
extern void add_ref_to_list_begin	PARAMS ((ref_list, tree_ref));
extern void add_ref_to_list_after	PARAMS ((ref_list,
						 struct ref_list_node *,
					         tree_ref));
extern void remove_ref_from_list	PARAMS ((ref_list, tree_ref));
extern struct ref_list_node *find_list_node PARAMS ((ref_list, tree_ref));
extern const char *ref_type_name	PARAMS ((HOST_WIDE_INT));
extern bool validate_ref_type		PARAMS ((HOST_WIDE_INT));


/* Functions in tree-ssa.c  */
extern void tree_build_ssa		PARAMS ((void));
extern void tree_compute_rdefs		PARAMS ((void));
extern void analyze_rdefs		PARAMS ((void));
extern bool is_upward_exposed		PARAMS ((tree, sbitmap, int));
extern void delete_ssa			PARAMS ((void));
extern void tree_ssa_remove_phi_alternative PARAMS ((tree_ref, basic_block));
extern void dump_reaching_defs		PARAMS ((FILE *));
extern void debug_reaching_defs		PARAMS ((void));

/* Functions in tree-alias-steen.c  */
extern void create_alias_vars		PARAMS ((void));


/*---------------------------------------------------------------------------
				Inline functions
---------------------------------------------------------------------------*/
/* Return the reference ID.  */
static inline HOST_WIDE_INT
ref_id (ref)
     tree_ref ref;
{
  return ref->common.id;
}

/* Return the reference type.  */
static inline HOST_WIDE_INT
ref_type (ref)
     tree_ref ref;
{
  return ref->common.type;
}

/* Return the basic block where REF is located.  */
static inline basic_block
ref_bb (ref)
     tree_ref ref;
{
  return ref->common.bb;
}

/* Return the expression where REF is located.  */
static inline tree
ref_expr (ref)
     tree_ref ref;
{
  return ref->common.expr;
}

/* Return the statement where REF is located.  */
static inline tree
ref_stmt (ref)
     tree_ref ref;
{
  return ref->common.stmt;
}

/* Return the variable associated to REF.  */
static inline tree
ref_var (ref)
     tree_ref ref;
{
  return ref->common.var;
}

/* Return a list of all the uses immediately reached from DEF.  */
static inline ref_list
imm_uses (def)
     tree_ref def;
{
  return def->vdef.imm_uses;
}

/* Return a list of all the uses reached by DEF.  */
static inline ref_list
reached_uses (ref)
     tree_ref ref;
{
  return ref->vdef.reached_uses;
}

/* Return the immediately reaching definition for USE.  */
static inline tree_ref
imm_reaching_def (use)
     tree_ref use;
{
  return use->vref.imm_rdef;
}

/* Set the immediately reaching definition of USE to be DEF.  */
static inline void 
set_imm_reaching_def (use, def)
     tree_ref use;
     tree_ref def;
{
  use->vref.imm_rdef = def;
}

/* Set DEF to be the definition reaching a given PHI_ARG.  */
static inline void
set_phi_arg_def (phi_arg, def)
     phi_node_arg phi_arg;
     tree_ref def;
{
  phi_arg->def = def;
}

/* Return the definition reaching PHI_ARG.  */
static inline tree_ref
phi_arg_def (phi_arg)
     phi_node_arg phi_arg;
{
  return phi_arg->def;
}

/* Set E to be the edge where the given PHI_ARG is coming from.  */
static inline void
set_phi_arg_edge (phi_arg, e)
     phi_node_arg phi_arg;
     edge e;
{
  phi_arg->e = e;
}

/* Return the edge where the PHI argument PHI_ARG is coming from.  */
static inline edge
phi_arg_edge (phi_arg)
     phi_node_arg phi_arg;
{
  return phi_arg->e;
}

/* Return the list of all definitions that may reach USE.  This is only
   valid after calling tree-ssa.c:tree_compute_rdefs.  */
static inline ref_list
reaching_defs (use)
     tree_ref use;
{
  return use->vuse.rdefs;
}


/* Return the array with all the arguments of PHI node.  */
static inline varray_type
phi_args (phi)
     tree_ref phi;
{
  return phi->vphi.phi_args;
}

/* Return the number of arguments for the given PHI node.  */
static inline unsigned int
num_phi_args (phi)
     tree_ref phi;
{
  return VARRAY_ACTIVE_SIZE (phi->vphi.phi_args);
}

/* Return the Ith argument for the given PHI node.  */
static inline phi_node_arg
phi_arg (phi, i)
     tree_ref phi;
     unsigned int i;
{
  return (phi_node_arg)(VARRAY_GENERIC_PTR (phi->vphi.phi_args, i));
}

/* Set the Ith argument of the given PHI node to ARG.  */
static inline void
set_phi_arg (phi, i, arg)
     tree_ref phi;
     unsigned int i;
     phi_node_arg arg;
{
  VARRAY_GENERIC_PTR (phi->vphi.phi_args, i) = (PTR)arg;
}

/* Replaces the operand that REF is pointing to with a new operand OP.
   Note that this does not modify any other references that may be affected
   by the change.  */
static inline void
replace_ref_operand_with (ref, op)
     tree_ref ref;
     tree op;
{
  *(ref->common.operand_p) = op;
}

/* Restores the operand for REF with its original value (i.e., with the
   variable that REF is referring to).  */
static inline void
restore_ref_operand (ref)
     tree_ref ref;
{
  *(ref->common.operand_p) = ref->common.orig_operand;
}

/* Return the annotation attached to T.  Create a new one if necessary.  */
static inline tree_ann
tree_annotation (t)
     tree t;
{
  return ((t->common.aux) ? (tree_ann)t->common.aux : create_tree_ann (t));
}

static inline bool
has_annotation (t)
     tree t;
{
  return (t->common.aux != NULL);
}

/* Return the basic block containing statement T.  */
static inline basic_block
bb_for_stmt (t)
     tree t;
{
  return tree_annotation (t)->bb;
}

/* Set the basic block containing statement T.  */
static inline void
set_bb_for_stmt (t, bb)
     tree t;
     basic_block bb;
{
  tree_ann ann;
#if defined ENABLE_CHECKING
  if (!statement_code_p (TREE_CODE (t)))
    abort ();
#endif
  ann = tree_annotation (t);
  ann->bb = bb;
}

/* Return the current definition for variable DECL.  Used by the SSA
   builder (tree-ssa.c:search_fud_chains).  */
static inline tree_ref
currdef_for (decl)
     tree decl;
{
#if defined ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (decl)) != 'd')
    abort ();
#endif
  return tree_annotation (decl)->currdef;
}

/* Set the current definition for variable DECL to be DEF.  */
static inline void
set_currdef_for (decl, def)
     tree decl;
     tree_ref def;
{
  tree_ann ann;
#if defined ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (decl)) != 'd')
    abort ();
  if (def && !(def->common.type & (V_DEF | V_PHI)))
    abort ();
#endif
  ann = tree_annotation (decl);
  ann->currdef = def;
}

/* Return the list of variable references made by T.  For _DECL trees, this
   is the list of references made to this variable.  For _STMT trees, list
   of references made in this statement.  For first-level SIMPLE
   expressions (i.e., the topmost expression of a _STMT node), list of
   references made in this expression.  */
static inline ref_list
tree_refs (t)
     tree t;
{
#if defined ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (t)) == 'c'
      || TREE_CODE_CLASS (TREE_CODE (t)) == 't')
    abort ();
#endif
  return tree_annotation (t)->refs;
}

/* Add REF to the list of referencs for T.  */
static inline void
add_tree_ref (t, ref)
     tree t;
     tree_ref ref;
{
#if defined ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (t)) == 'c'
      || TREE_CODE_CLASS (TREE_CODE (t)) == 't')
    abort ();
#endif
  add_ref_to_list_end (tree_refs (t), ref);
}

/* Remove REF from the list of references for T.  */
static inline void
remove_tree_ref (t, ref)
     tree t;
     tree_ref ref;
{
  remove_ref_from_list (tree_refs (t), ref);
}

/* Return the COMPOUND_STMT enclosing T.  */
static inline tree
compound_parent (t)
     tree t;
{
#if defined ENABLE_CHECKING
  if (!statement_code_p (TREE_CODE (t)))
    abort ();
#endif
  return tree_annotation (t)->compound_parent;
}

/* Set PARENT to be the COMPOUND_STMT enclosing T.  */
static inline void
set_compound_parent (t, parent)
     tree t;
     tree parent;
{
  tree_ann ann;
#if defined ENABLE_CHECKING
  if (!statement_code_p (TREE_CODE (t)))
    abort ();
#endif
  ann = tree_annotation (t);
  ann->compound_parent = parent;
}

/* Return the output reference for EXPR.  This will always be NULL for
   everything except MODIFY_EXPR, INIT_EXPR and DECL_STMT for static
   variables.  */
static inline tree_ref
output_ref (t)
     tree t;
{
  return tree_annotation (t)->output_ref;
}

/* Set the output reference for EXPR to be DEF.  */
static inline void
set_output_ref (t, def)
     tree t;
     tree_ref def;
{
  tree_ann ann = tree_annotation (t);
  ann->output_ref = def;
}

/* Set T's FLAG.  */
static inline void
set_tree_flag (t, flag)
     tree t;
     enum tree_flags flag;
{
  tree_ann  ann;
#if defined ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (t)) == 'c'
      || TREE_CODE_CLASS (TREE_CODE (t)) == 't')
    abort ();
#endif
  ann = tree_annotation (t);
  ann->flags |= flag;
}

/* Clear T's FLAG.  */
static inline void
clear_tree_flag (t, flag)
     tree t;
     enum tree_flags flag;
{
  tree_ann ann;
#if defined ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (t)) == 'c'
      || TREE_CODE_CLASS (TREE_CODE (t)) == 't')
    abort ();
#endif
  ann = tree_annotation (t);
  ann->flags &= ~flag;
}

/* Reset all T's flags to zero.  */
static inline void
reset_tree_flags (t)
     tree t;
{
  tree_ann ann;
#if defined ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (t)) == 'c'
      || TREE_CODE_CLASS (TREE_CODE (t)) == 't')
    abort ();
#endif
  ann = tree_annotation (t);
  ann->flags = 0;
}

/* Return T's FLAGS.  */
static inline enum tree_flags
tree_flags (t)
     tree t;
{
#if defined ENABLE_CHECKING
  if (TREE_CODE_CLASS (TREE_CODE (t)) == 'c'
      || TREE_CODE_CLASS (TREE_CODE (t)) == 't')
    abort ();
#endif
  return tree_annotation (t)->flags;
}

/* Return the annotation for basic block B.  Create a new one if necessary.  */
static inline bb_ann
bb_annotation (b)
     basic_block b;
{
  return (b->aux) ? (bb_ann)b->aux : create_bb_ann (b);
}

/* Return the control flow parent block for B.  This is the block that
   starts the scope or control structure that B is contained in.  */
static inline basic_block
bb_parent (b)
     basic_block b;
{
  return bb_annotation (b)->parent;
}

/* Set the control flow parent block for B.  */
static inline void
set_bb_parent (b, parent)
     basic_block b;
     basic_block parent;
{
  bb_ann ann = bb_annotation (b);
  ann->parent = parent;
}

/* Return the list of references made in basic block B.  */
static inline ref_list
bb_refs (b)
     basic_block b;
{
  return bb_annotation (b)->refs;
}

/* Remove REF from the list of references made by B.  */
static inline void
remove_bb_ref (b, ref)
     basic_block b;
     tree_ref ref;
{
  remove_ref_from_list (bb_annotation (b)->refs, ref);
}

/* Return the address of the tree that is pointing to the first tree in the
   basic block.  This is used to compensate for the lack of a 'previous'
   chain in the trees.  Writing to the address returned by this function
   will make the TREE_CHAIN of the tree preceding T point somewhere else.  */
static inline tree *
prev_chain_p (b)
     basic_block b;
{
  return bb_annotation (b)->prev_chain_p;
}

/* Set the previous chain pointer for block B.  */
static inline void
set_prev_chain_p (b, tp)
     basic_block b;
     tree *tp;
{
  bb_ann ann = bb_annotation (b);
  ann->prev_chain_p = tp;
}

/* Return the block that starts the binding scope containing B.  */
static inline basic_block
binding_scope (b)
     basic_block b;
{
  return bb_annotation (b)->binding_scope;
}

/* Set the binding scope for B.  */
static inline void
set_binding_scope (b, scope)
     basic_block b;
     basic_block scope;
{
  bb_ann ann = bb_annotation (b);
  ann->binding_scope = scope;
}

/* Return the set of loop headers for the loop entry block ENTRY.  */
static inline union header_blocks *
loop_hdr (entry)
     basic_block entry;
{
  return bb_annotation (entry)->loop_hdr;
}

/* Return the block containing the FOR_INIT_STMT node of a FOR_STMT.  Only
   valid if FOR_BB is the entry block of the loop.  */
static inline basic_block
for_init_bb (for_bb)
     basic_block for_bb;
{
  return loop_hdr (for_bb)->for_hdr.for_init_stmt_bb;
}

/* Set the block containing the FOR_INIT_STMT node.  */
static inline void
set_for_init_bb (for_bb, init_bb)
     basic_block for_bb;
     basic_block init_bb;
{
  union header_blocks *hdr = loop_hdr (for_bb);
  hdr->for_hdr.for_init_stmt_bb = init_bb;
}

/* Return the block containing the FOR_COND node of a FOR_STMT.  Only
   valid if FOR_BB is the entry block of the loop.  */
static inline basic_block
for_cond_bb (for_bb)
     basic_block for_bb;
{
  return loop_hdr (for_bb)->for_hdr.for_cond_bb;
}

/* Set the block containing the FOR_COND node.  */
static inline void
set_for_cond_bb (for_bb, cond_bb)
     basic_block for_bb;
     basic_block cond_bb;
{
  union header_blocks *hdr = loop_hdr (for_bb);
  hdr->for_hdr.for_cond_bb = cond_bb;
}

/* Return the block containing the FOR_EXPR node of a FOR_STMT.  Only
   valid if FOR_BB is the entry block of the loop.  */
static inline basic_block
for_expr_bb (for_bb)
     basic_block for_bb;
{
  return loop_hdr (for_bb)->for_hdr.for_expr_bb;
}

/* Set the block containing the FOR_EXPR node.  */
static inline void
set_for_expr_bb (for_bb, expr_bb)
     basic_block for_bb;
     basic_block expr_bb;
{
  union header_blocks *hdr = loop_hdr (for_bb);
  hdr->for_hdr.for_expr_bb = expr_bb;
}

/* Return the END_WHILE block for WHILE_BB.  */
static inline basic_block
end_while_bb (while_bb)
     basic_block while_bb;
{
  return loop_hdr (while_bb)->end_while_bb;
}

/* Set the END_WHILE block for WHILE_BB.  */
static inline void
set_end_while_bb (while_bb, end_while_bb)
     basic_block while_bb;
     basic_block end_while_bb;
{
  union header_blocks *hdr = loop_hdr (while_bb);
  hdr->end_while_bb = end_while_bb;
}

/* Return the basic block for the DO_COND node of a DO_STMT entry block.  */
static inline basic_block
do_cond_bb (do_bb)
     basic_block do_bb;
{
  return loop_hdr (do_bb)->do_cond_bb;
}

/* Set the basic block for the DO_COND node of a DO_STMT entry block.  */
static inline void
set_do_cond_bb (do_bb, do_cond_bb)
     basic_block do_bb;
     basic_block do_cond_bb;
{
  union header_blocks *hdr = loop_hdr (do_bb);
  hdr->do_cond_bb = do_cond_bb;
}

#endif /* _TREE_FLOW_H  */
