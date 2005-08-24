/* Promotion of static variables to ssa registers
   Copyright (C) 2004 Free Software Foundation, Inc.
   Contributed by Kenneth Zadeck <zadeck@naturalbridge.com>

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "basic-block.h"
#include "tree-flow.h"
#include "ipa-static.h"
#include "bitmap.h"
#include "tree-pass.h"
#include "flags.h"

#include "langhooks.h"

/*
The main idea is to promote some static variables from memory to ssa
registers.  This transformation is only applied to those static
variables for which the effects of subroutine calls can be understood.
Such infomation is provided by functions in cgraphunit.c.

The following table shows the actions that are taken to promote
variables.  The analysis in cgraphunit constructs information about
both local usage and the effect of any particular call.  Variables are
broken into 4 categories: only-read, only-write, read-write, and no
information.  (No information variables are never promoted.)  

All information is of the "may" variety: if a function is marked read,
it means the call may read the variable, but it also may not read the
variable.

There are two possible ways to perform the promotion: assume that the
static is live everywhere or compute the minimal live range for the
static variable.

The minimal live range path has a lot of problems:

1) live variables and upwards exposed uses must be first comuputed.
2) new machiney must be invented to prevent code motion algorithms
from floating a use of the suragate register across a register
function call that clobbers the variable, but was not in any minimal
live range at the time of this analysis.

While the first problem is simply a lot of code, the second problem
requires a new mechanism for pinning code and teaching all passes that
can move code to obey this new fenceposts.  

The maximum live range path has the problem that this technique can
create many false live ranges where the register is loaded after on
call only to be stored back right before the next call.  This will eat
a certain amount of space and requires special smarts to get rid of them.

There are really 7 situations to cover in the following table.  

action             read                    write                   read-write

             -+---------------------------------------------------------------

entry         |  load                    load                    load
              |
load          |  getfromreg              xxxxx                   getfromreg
              |
store         |  xxxx                    puttoreg                puttoreg
              |
call-read     |  noaction                store before            store before
              |
call-write    |  load after              store before            store before
              |                          load after              load after  
call-readwrite|  load after              store before            store before
              |                          load after              load after  
              |
return        |  no action               store                   store         


l-r	l-w	c-r	c-w	store-b	load-a

0	0	0	0   |	0	0
0	0	0	1   |	0	0
0	0       1      	0   |	0	0
0	0	1	1   |	0	0
0	1	0	0   |	0	0
0	1	0	1   |	1	1	 
0	1       1      	0   |	1	0
0	1	1	1   |	1	1
1	0	0	0   |	0	0
1	0	0	1   |   0       1
1	0       1      	0   |	0	0	
1	0	1	1   |	0	1
1	1	0	0   |	0	0
1	1	0	1   |	1	1
1	1       1      	0   |	1	0
1	1	1	1   |	1	1

store_before = local_written & (callee_read | callee_written)
load_after = (local_read | local_written) & callee_written


*/

static bitmap_obstack promote_obstack;

/* All of the static variables under consideration by this pass that
   do reads or writes withing this function.   */
static bitmap local_read;
static bitmap local_written;
static bitmap local_all;

/* Map indexed by the var_ann uid that contains the shadow register
   for each static variable to be promoted. */
static tree *static_map;


/* Return true if the asm STMT clobbers memory.  */
static bool
asm_clobbers_mem (tree stmt)
{
  tree link;
  for (link = ASM_CLOBBERS (stmt); link; link = TREE_CHAIN (link))
    if (simple_cst_equal(TREE_VALUE (link), memory_identifier_string) == 1) 
      return true;

  return false;
}

/* Return a INPUT_BITMAP for the asm inputs and OUTPUT_BITMAP for the
   asm outputs of static variables written by the asm STMT.  */
static void
get_asm_read_and_write (bitmap input_bitmap, bitmap output_bitmap, tree stmt) 
{
  int noutputs = list_length (ASM_OUTPUTS (stmt));
  const char **oconstraints
    = (const char **) alloca ((noutputs) * sizeof (const char *));
  int i;
  tree link;
  const char *constraint;
  bool allows_mem, allows_reg, is_inout;

  for (i=0, link = ASM_OUTPUTS (stmt); link; ++i, link = TREE_CHAIN (link))
    {
      oconstraints[i] = constraint
	= TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
      parse_output_constraint (&constraint, i, 0, 0,
	  &allows_mem, &allows_reg, &is_inout);

      /* Do not actually check if the variable is a static variable,
         since it will be anded with the set of variables we are
         interrested in later.  */
      if (!allows_reg && allows_mem)
	{
	  tree var = TREE_VALUE (link);
	  var = get_base_address (var);
	  if (TREE_CODE (var) == VAR_DECL)
	    {
	      var_ann_t va = var_ann (var);
	      if (va) 
		bitmap_set_bit(output_bitmap, va->uid);
	    }
	}
    }

  for (link = ASM_INPUTS (stmt); link; link = TREE_CHAIN (link))
    {
      constraint
	= TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
      parse_input_constraint (&constraint, 0, 0, noutputs, 0,
			      oconstraints, &allows_mem, &allows_reg);
      
      if (!allows_reg && allows_mem)
	{
	  tree var = TREE_VALUE (link);
	  var = get_base_address (var);
	  if (TREE_CODE (var) == VAR_DECL)
	    {
	      var_ann_t va = var_ann (var);
	      if (va) 
		bitmap_set_bit(input_bitmap, va->uid);
	    }
	}
    }
}

/* Generate a series of loads from the static variables pointed to by
   b1 && b2 or just b1 (if b2 is NULL) and insert them after
   bsi).  */
static void
gen_loads (bitmap b1, bitmap b2, block_stmt_iterator *bsi) 
{
  bitmap result;
  bitmap_iterator bi;
  unsigned int index;
  tree list = NULL;

  if (b2) 
    {
      result = BITMAP_ALLOC (&promote_obstack);
      bitmap_and (result, b1, b2);
    }
  else 
    result = b1;

  EXECUTE_IF_SET_IN_BITMAP(result, 0, index, bi) 
      {
	tree src = referenced_var (index);
	tree dest = static_map[index];
	tree stmt = build (MODIFY_EXPR, TREE_TYPE (src), dest, src);
	append_to_statement_list (stmt, &list);
      }

  if (list)
    sra_insert_after (bsi, list);
			   
  if (b2)
    BITMAP_FREE (result);
}

/* Generate a series of stores to the static variables pointed to by
   b1 && b2 or just b1 (if b2 is NULL) and insert them before
   bsi).  */
static void
gen_stores (bitmap b1, bitmap b2, block_stmt_iterator *bsi) 
{
  bitmap result;
  bitmap_iterator bi;
  unsigned int index;
  tree list = NULL;

  if (b2) 
    {
      result = BITMAP_ALLOC (&promote_obstack);
      bitmap_and (result, b1, b2);
    }
  else 
    result = b1;

  EXECUTE_IF_SET_IN_BITMAP(result, 0, index, bi) 
      {
	tree src = static_map[index];
	tree dest = referenced_var (index);
	tree stmt = build (MODIFY_EXPR, TREE_TYPE (src), dest, src);
	append_to_statement_list (stmt, &list);
/* 	fprintf(stderr, "    generating store for var=%s\n", get_name(dest)); */
      }

  if (list)
    sra_insert_before (bsi, list);
			   
  if (b2)
    BITMAP_FREE (result);
}

/* Replace the static references if it exists in the TPTR.  */

static void 
try_replace_operand(tree * tptr)
{
  tree t = *tptr;
  if (TREE_CODE (t) == VAR_DECL)
    {
      var_ann_t va = var_ann (t);
      tree replacement = static_map[va->uid];
      if (replacement)
	*tptr = replacement;
    }
}

/* Walk an expression TPTR replacing all of the static references.  */
static void
try_replace (tree *tptr) 
{
  tree t = *tptr;
  if ((TREE_CODE (t) == EXC_PTR_EXPR) || (TREE_CODE (t) == FILTER_EXPR))
    return;

  /* The INTEGER_CST is because some people use cute things like &0->a
     for offsetof.  */
  while (!SSA_VAR_P (t) 
	 && (!CONSTANT_CLASS_P (t)) 
	 && TREE_CODE (t) != LABEL_DECL
	 && TREE_CODE (t) != FUNCTION_DECL)
    {
      if (TREE_CODE (t) == ARRAY_REF)
	try_replace_operand (&TREE_OPERAND (t, 1));

      tptr = &TREE_OPERAND (t, 0);
      t = *tptr;
    }
  try_replace_operand (tptr);
}

/* Replace all the static references in the operand list of
   CALL_EXPR.  */

static void
try_replace_call_operands (tree call_expr) 
{
  tree operandList = TREE_OPERAND (call_expr, 1);
  tree operand;

  for (operand = operandList;
       operand != NULL_TREE;
       operand = TREE_CHAIN (operand))
 
    if (TREE_CODE(TREE_VALUE (operand)) != FUNCTION_DECL)
      try_replace (&TREE_VALUE (operand));
}

/* Generate loads and stores and replace all the static references in
   function FN using statement iterator SI. This form is used when
   there is not info available about the caller.  */
static void 
gen_dumb_call (tree fn, block_stmt_iterator si) 
{

/*   fprintf(stderr, "    promoting dumb call\n"); */

  gen_stores (local_written, NULL, &si);
  try_replace (&TREE_OPERAND (fn, 0));
  try_replace_call_operands (fn);
  gen_loads (local_all, NULL, &si);
}


/* Generate loads and stores and replace all the static references in
   function FN using statement iterator SI.  */
static void 
try_replace_call (tree fn, block_stmt_iterator si)
{
  /* Store intersection of call_read and local_written
     registers back to memory before calling.  */
  /* int call_flags = call_expr_flags (fn); */
  tree callee = get_callee_fndecl (fn);
  if (callee) 
    {
      bitmap callee_all = BITMAP_ALLOC (&promote_obstack);
      bitmap callee_written = ipa_get_statics_written_global (callee);
      if (callee_written) 
	{
	  bitmap_ior (callee_all, 
			 ipa_get_statics_read_global (callee), 
			 callee_written);
	  
	  gen_stores (local_written, callee_all, &si);
	  
	  if (TREE_CODE (callee) != FUNCTION_DECL)
	    try_replace (&TREE_OPERAND (fn, 0));
	  try_replace_call_operands (fn);
	  
	  /* This is a hack required because the call_flags are set on a
	     function by function basis during compilation.  Thus these
	     flags are only set if the callee has already been compiled.  */
	  /* if (!(call_flags & (ECF_PURE | ECF_CONST | ECF_NORETURN))) */
	  gen_loads (local_all, callee_written, &si);
	  BITMAP_FREE (callee_all);
	}
      else 
	gen_dumb_call (fn, si);
    }
  else
    gen_dumb_call (fn, si);
}


/* Walk the entire function looking uses or stores to global variables
and changing them to use ssa shadow registers.  */ 
static void
walk_function (void)
{
  basic_block bb;
  block_stmt_iterator si, ni;

  FOR_EACH_BB (bb)
    for (si = bsi_start (bb); !bsi_end_p (si); si = ni)
      {
	tree stmt = bsi_stmt (si);

	ni = si;
	bsi_next (&ni);

	switch (TREE_CODE (stmt))
	  {
	  case RETURN_EXPR:
	    /* Store all of the local_written registers back to memory
	       before returning. */
/* 	    fprintf(stderr, "    promoting return\n"); */
	    gen_stores (local_written, NULL, &si);
	    break;

	  case MODIFY_EXPR:
	    /* Change load of static to use of reg.  Change store of
	       static to store of reg.  */
	    {
	      tree t = TREE_OPERAND (stmt, 1);
	      tree *tptr = &TREE_OPERAND (stmt, 1);
	      
	      /* Check to see if the bottom of the right hand side is
		 a reference to a static variable that we care about.
		 If it is replace the static part with the ssa
		 register.  */
	      
	      /* Next check the operands on the rhs to see if they are ok. */
	      switch (TREE_CODE_CLASS (TREE_CODE (t))) 
		{
		case tcc_binary:
		  try_replace (&TREE_OPERAND (t, 0));
		  try_replace (&TREE_OPERAND (t, 1));
		  break;
		case tcc_unary:
		case tcc_reference:
		  try_replace (&TREE_OPERAND (t, 0));
		  break;
		case tcc_declaration:
		  try_replace (tptr);
		  break;
		case tcc_expression:
		  switch (TREE_CODE (t)) {
		  case ADDR_EXPR:
		    try_replace (tptr);
		    break;
		  case CALL_EXPR: 
		    try_replace_call (t, si);
		    break;
		  default:
		    break;
		  }
		  break;
		default:
		  break;
		}

	      /* Check to see if the bottom of the left hand side is a
		 reference to a static variable that we care about.
		 If it is replace the static part with the ssa
		 register.  */
	      try_replace(&TREE_OPERAND (stmt, 0));
	    }
	    break;
	  case CALL_EXPR:
	    try_replace_call (stmt, si);
   
	    break;
	  case ASM_EXPR:
	    /* If the asm clobbers memory, just store everything and
	       load it back.  */
	    if (asm_clobbers_mem (stmt)) 
	      { 
		gen_stores (local_written, NULL, &si);
		gen_loads (local_all, NULL, &si);
	      }
	    else 
	      {
		bitmap store_bitmap = BITMAP_ALLOC (&promote_obstack);
		bitmap load_bitmap = BITMAP_ALLOC (&promote_obstack);
		bitmap all_bitmap = BITMAP_ALLOC (&promote_obstack);
		/* The asm read generates a stores before, and the asm
		   write generates loads after.  */
		get_asm_read_and_write (store_bitmap, load_bitmap, stmt);
		bitmap_ior (all_bitmap, store_bitmap, load_bitmap);
		
		gen_stores (local_written, all_bitmap , &si);
		gen_loads (local_all, load_bitmap, &si);
		
		BITMAP_FREE (store_bitmap);
		BITMAP_FREE (load_bitmap);
		BITMAP_FREE (all_bitmap);
	      } 
	    break;
	    
	  default:
	    break;
	  }
      }
}

static int max_funct = -1;
static int current_funct = 0;

/* Main entry point for the promotion of statics to ssa regsisters. */

static void
execute_promote_statics (void)
{
  unsigned int index;
  bitmap_iterator bi;
  bitmap tb = ipa_get_statics_read_local (current_function_decl);

  if (0) 
    {
      if (max_funct == -1)
	{
	  FILE *ok_statics_file = fopen("/home/zadeck/ok_promote", "r");
	  char line[100];
	  fgets(line, sizeof(line), ok_statics_file);
	  sscanf(line, "%d", &max_funct);
	}
      current_funct++;
      if (current_funct > max_funct)
	return;
      else
	fprintf(stderr, "promoting in %s\n",
		lang_hooks.decl_printable_name (current_function_decl, 2));
    }

  /* There are some options that cause this pass to run even if file
     at a time is not set.  */
  if (!tb)
    return;

  bitmap_obstack_initialize (&promote_obstack);
  sra_init_cache ();

  local_read = BITMAP_ALLOC (&promote_obstack);
  bitmap_copy (local_read, tb);
  tb = ipa_get_statics_written_local (current_function_decl);
  local_written = BITMAP_ALLOC (&promote_obstack);
  bitmap_copy (local_written, tb);

  /* Create the shadow registers for each static variable that is
     either read or written in this function.  */
  static_map = xcalloc (num_referenced_vars, sizeof (tree));

  local_all = BITMAP_ALLOC (&promote_obstack);
  tb = BITMAP_ALLOC (&promote_obstack);
  bitmap_ior (local_all, local_read, local_written);

/*   fprintf(stderr, "promoting in %s\n", */
/* 	  lang_hooks.decl_printable_name (current_function_decl, 2)); */

  EXECUTE_IF_SET_IN_BITMAP (local_all, 0, index, bi) 
      {
	tree svar = referenced_var (index);
	tree type = TREE_TYPE (svar);
	/* We only promote variables that are either scalars or if
	   they are aggregrates, they must be a type that sra is
	   willing to scalarize.  Otherwise there is no reason to
	   promote it a register.  

	   We also do not promote anything that is marked READONLY
	   since there is little gain.  The optimizations should
	   generally be able to look thru the operations and find the
	   constants.  */
	if ((!TREE_READONLY(svar)) 
	    && (TREE_CODE (type) != ARRAY_TYPE)
	    && ((!AGGREGATE_TYPE_P (type))
		|| (sra_type_can_be_decomposed_p (type))))
	  {
	    tree tmp = create_tmp_var (type, get_name (svar));
	    add_referenced_tmp_var (tmp);
	    static_map[index] = tmp;

	    /* Insert loads from all read statics in the entry
	       block.  */
	    insert_edge_copies (build (MODIFY_EXPR, TREE_TYPE (svar), 
				       tmp, svar), 
				ENTRY_BLOCK_PTR);
/* 	    fprintf(stderr, "  var=%s, read=%d,write=%d\n", get_name(svar), */
/* 		    bitmap_bit_p(local_read, index),  */
/* 		    bitmap_bit_p(local_written, index)); */

	  }
	else 
	  {
	    /* There is nothing to be done with this variable.  */ 
	    bitmap_set_bit (tb, index);
	  }
      }

  /* Clear the to be ignored variables from the local maps.  */
  bitmap_and_compl_into (local_read, tb);
  bitmap_and_compl_into (local_written, tb);
  bitmap_and_compl_into (local_all, tb);

  

  walk_function ();
  bsi_commit_edge_inserts ();

  bitmap_obstack_release (&promote_obstack);
  free (static_map);
}

static bool
gate_promote_statics (void)
{
  return flag_unit_at_a_time != 0;
}

struct tree_opt_pass pass_promote_statics = 
{
  "pstatic",				/* name */
  gate_promote_statics,			/* gate */
  NULL, NULL,				/* IPA analysis */
  execute_promote_statics,		/* execute */
  NULL, NULL,				/* IPA modification */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  0,					/* tv_id */
  PROP_cfg ,                    	/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func,	                /* todo_flags_finish */
  0                                     /* letter */
};

