/* Goto statement elimination for Simple.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Sebastian Pop <s.pop@laposte.net>

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "c-tree.h"
#include "c-common.h"
#include "tree-simple.h"
#include "hash.h"
#include "tree-dchain.h"
#include "diagnostic.h"

/* This implements a goto elimination pass based on the SIMPLE representation.
   The original algorithm was described by Ana M. Erosa and Laurie J. Hendren.
   This implementation follows this algorithm and does not generate breaks.
   Logical variables are declared in the function's outermost scope such that 
   they are visible in the whole function.  Thier initial value is set at false.
   When a GOTO_STMT is reached, then its logical variable switches to 'ON' and
   the flow control is switched on an "express way" to the corresponding 
   LABEL_STMT.  On this "express way" there are only comparisons like '(T != 0)'
   introduced by this pass, and no other instructions are executed.  Then once 
   the flow reached its destination, the logical variable is set to 'OFF' and the
   control flow is switched on its normal way.  The total number of such 
   comparisons depends on the number of goto statements and on the nesting level
   of the label/goto statements.  
   An interesting thing in the goto elimination is that after this pass the 
   control flow graph (CFG) becomes regular, and optimisations that were 
   disabled can be applied after this pass. 
   
   In fact this pass does nothing else than complete the implicitly specified
   control flow.  When a programmer uses a goto, he implicitly specifies that
   the code between the GOTO_STMT and the corresponding target LABEL_STMT is
   not executed.  This pass adds the necessary code to complete the 
   specification of the control flow.  The control flow becomes explicit.
   
   As described in the documentation of the McCAT compiler, the algorithm 
   uses a hash table of LABEL_STMTs and a list of GOTO_STMTs.  
      
   [SWITCH|BREAK|CONTINUE]_STMTs are not handled by the current implementation
   and have to be transformed before this pass (break_continue_elimination pass).
   

   References :

   - Taming Control Flow : A structured	Approach to Eliminate Goto Statements,
   Ana M. Erosa and Laurie J. Hendren, in the proceedings of the 1994 
   International Conference on Computer Languages (ICCL'94) pp.229-240. 
   ftp://ftp-acaps.cs.mcgill.ca/pub/doc/papers/ICCL94.ps.gz
   http://citeseer.nj.nec.com/22521.html

   - Erosa's MSc. Thesis, University McGill : 
   A goto-elimination method and its implementation for the McCAT C compiler,
   http://www.sable.mcgill.ca/~hendren/ftp/erosa/thesis.ps.gz
   http://citeseer.nj.nec.com/erosa95gotoelimination.html  */

typedef struct label_hash_entry
{
  struct hash_entry root;
  int offset;
  int level;
  tree label_stmt;
  PATH_TYPE path;
  const char *id;
} *label;

typedef struct goto_list_entry
{
  int offset;
  int level;
  tree if_stmt;
  tree label;
  tree lvar;
  PATH_TYPE path;
  struct goto_list_entry *next;
} *gotol;

enum rel
{
  SIBLINGS = 0,
  DIRECTLY_RELATED,
  INDIRECTLY_RELATED 
};

/* Static declarations.  */
static void find_labels_gotos                  PARAMS ((tree));
static void init_logical_vars                  PARAMS ((void));
static void transform_gotos                    PARAMS ((void));
static enum rel related                        PARAMS ((void));
static tree label_get_label                    PARAMS ((tree));
static PATH_TYPE outer_non_common_parent       PARAMS ((void));
static PATH_TYPE great_common_parent           PARAMS ((void));
static tree flow_in                            PARAMS ((PATH_TYPE, tree, tree));
static tree eliminate_with_do                  PARAMS ((tree, tree, tree));
static PATH_TYPE path_contain_stmt             PARAMS ((PATH_TYPE, tree));
static void update_paths_if                    PARAMS ((tree, tree, tree, tree));
static void update_paths_do                    PARAMS ((tree, tree, tree, tree));
static void path_include                       PARAMS ((PATH_TYPE, tree, tree, PATH_TYPE));
static tree tree_last_decl                     PARAMS ((tree));
static struct hash_entry * label_hash_newfunc  PARAMS ((struct hash_entry *, 
							struct hash_table *, 
							hash_table_key));
static label label_hash_lookup                 PARAMS ((const char *, int));


/* Static variables */
/* Containers.  */
static struct hash_table label_table;
static gotol goto_list;

/* Iterators.  */
static gotol g;
static label l;

/* Stack information.  */
static int offset;
static int level;
static PATH_TYPE PATH_TOP;


#define NEW(TYPE) ((TYPE *) xmalloc (sizeof (TYPE)))
#define DELETE(E) free (E)

/* Define a LABEL interface.  */
#define LABEL_ID(E)         (((label) (E))->id)
#define LABEL_LABEL_STMT(E) (((label) (E))->label_stmt)
#define LABEL_PATH(E)       (((label) (E))->path)
#define LABEL_OFFSET(E)     (((label) (E))->offset)
#define LABEL_LEVEL(E)      (((label) (E))->level)
#define LABEL_PUSH(NODE) do {                                                \
  label lab;                                                                 \
  const char *p =                                                            \
    IDENTIFIER_POINTER (DECL_NAME (TREE_OPERAND (NODE, 0)));                 \
  lab = label_hash_lookup (p, true);                                         \
  LABEL_ID (lab) = p;                                                        \
  LABEL_LABEL_STMT (lab) = NODE;                                             \
  PATH_COPY (LABEL_PATH (lab), PATH_TOP);                                    \
  LABEL_OFFSET (lab) = offset++;                                             \
  LABEL_LEVEL (lab) = level;                                                 \
} while (0)
#define LABEL_POP(NODE) 

/* Define a GOTO interface.  */
#define GOTO_FOREACH(E, L) for (E = L; E != NULL; E = GOTO_NEXT (E))
#define GOTO_IF_STMT(E) ((E)->if_stmt)
#define GOTO_LABEL(E)   ((E)->label)
#define GOTO_LVAR(E)    ((E)->lvar)
#define GOTO_PATH(E)    ((E)->path)
#define GOTO_OFFSET(E)  ((E)->offset)
#define GOTO_LEVEL(E)   ((E)->level)
#define GOTO_NEXT(E)    ((E)->next)
#define GOTO_LAST(E) do {                                               \
  while (GOTO_NEXT (E))                                                 \
    E = GOTO_NEXT (E);                                                  \
} while (0)
/* Push elements at the end of the queue.  */
#define GOTO_PUSH(NODE) do {                                            \
  PATH_TYPE tmp;                                                        \
  struct goto_list_entry *elt;                                          \
  elt = NEW (struct goto_list_entry);                                   \
  PATH_COPY (tmp, PATH_TOP);                                            \
  GOTO_PATH (elt) = tmp;                                                \
  GOTO_OFFSET (elt) = offset++;                                         \
  GOTO_LEVEL (elt) = level;                                             \
  GOTO_IF_STMT (elt) = NODE;                                            \
  GOTO_LABEL (elt) = GOTO_DESTINATION (NEXT_STMT (COMPOUND_BODY (THEN_CLAUSE (NODE))));  \
  GOTO_NEXT (elt) = NULL;                                               \
  if (goto_list == NULL)                                                \
    {  /* First element in GOTO_LIST.  */                               \
      goto_list = elt;                                                  \
    }                                                                   \
  else                                                                  \
    {                                                                   \
      struct goto_list_entry *last;                                     \
      last = goto_list;                                                 \
      GOTO_LAST (last);                                                 \
      GOTO_NEXT (last) = elt;                                           \
    }                                                                   \
  {                                                                     \
    tree decl;                                                          \
    decl = create_tmp_var (integer_type_node);                          \
    DECL_INITIAL (decl) = integer_zero_node;                            \
    GOTO_LVAR (elt) = decl;                                             \
  }                                                                     \
} while (0)
/* Pop elements from the head of the queue.  */
#define GOTO_POP() do {                                                 \
  struct goto_list_entry *elt;                                          \
  elt = goto_list;                                                      \
  goto_list = GOTO_NEXT (goto_list);                                    \
} while (0)



/* This function is the entry point for this pass.  It transforms all GOTO_STMTs
   from the current function in regular control flow statements.  FN is the 
   FUNCTION_DECL node for the function we want to transform.  */

void 
goto_elimination (fn)
     tree fn;
{
  tree fn_body;

  /* Don't bother doing anything if the program has errors.  */
  if (errorcount || sorrycount)
    return;

  fn_body = COMPOUND_BODY (DECL_SAVED_TREE (fn));
  if (fn_body == NULL)
    return;

  /* Create a new binding level for the temporaries created by the
     simplification process.  */
  pushlevel (0);

  /* Initialise the LABEL_STMT hash table.  */
  hash_table_init (&label_table, label_hash_newfunc, string_hash, 
		   string_compare);
  
  /* Reset counters.  */
  offset = 0;
  level = 0;
  goto_list = NULL;
  PATH_TOP = NULL;
  
  /* Get the list of labels and gotos for the current function.  */
  find_labels_gotos (DECL_SAVED_TREE (fn));
    
  /* Initialize logical variables.  */  
  init_logical_vars ();
  
  /* Finish the work.  */
  transform_gotos ();
  
#if 0
  /* The work is not finished, since the label variable is still 
     marked as used in the scope block;  and from here we get 
     the error in varasm.c:839...  */
  dchain_delete_labels (fn_body);
#endif
  
  /* Clean unused COMPOUND_STMTs... (optional cleaning, just for making things 
     simpler to analyze after this pass).  */
  eliminate_compound_stmts (fn_body);

  /* Declare the new temporary variables.  */
  dchain_declare_tmp_vars (getdecls(), fn_body);

  /* Restore the binding level.  */
  poplevel (1, 1, 0);
}

/* Insert LABEL_STMTs in the hash table, and GOTO_STMTs in the goto_list.  */

static void
find_labels_gotos (node)
     tree node;
{
  while (node && node != error_mark_node)
    {
      switch (TREE_CODE (node))
	{
	case COMPOUND_STMT:
	  PATH_PUSH (node);
	  ++level;
	  find_labels_gotos (COMPOUND_BODY (node));
	  --level;
	  PATH_POP ();
	  break;

	case FOR_STMT:
	  PATH_PUSH (node);
	  ++level;
	  dchain_build_scope (&FOR_BODY (node));
	  find_labels_gotos (FOR_BODY (node));
	  --level;
	  PATH_POP ();
	  break;

	case WHILE_STMT:
	  PATH_PUSH (node);
	  ++level;
	  dchain_build_scope (&WHILE_BODY (node));
	  find_labels_gotos (WHILE_BODY (node));
	  --level;
	  PATH_POP ();
	  break;

	case DO_STMT:
	  PATH_PUSH (node);
	  ++level;
	  dchain_build_scope (&DO_BODY (node));
	  find_labels_gotos (DO_BODY (node));
	  --level;
	  PATH_POP ();
	  break;

	case SWITCH_STMT:
	  /* This implementation of goto_elimination doesn't handle this type of
	     statement : it works on a subset of SIMPLE that doesn't contain 
	     SWITCH_STMTs.  This subset of SIMPLE is obtained after the 
	     break_continue_elimination.  Run this pass before the goto_elimination.
	     The main reason for this limitation is that implementating this pass
	     for SWITCH_STMTs introduces some difficult cases to handle (when the 
	     LABEL_STMT is in the default_case, flow_in has to figure out what 
	     cases belongs to the SWITCH_BODY and then choose a value that 
	     is not in the SWITCH_BODY).  This implementation could be refined
	     in order to work on SIMPLE with SWITCH_STMTs as it is described in 
	     McGill's papers.  */
	  fprintf (stderr, "Run break_continue_elimination before goto_elimination.\n");
	  abort ();
	  
	case LABEL_STMT:
	  /* Insert the label in the hash table.  */
	  LABEL_PUSH (node);
	  break;

	case GOTO_STMT:
	  /* This GOTO_STMT is unconditional.  */
	  {
	    tree if_stmt, cond, open_scope, close_scope;
	    
	    /* Replace "goto L;" by "if (1!=0) {goto L;}".  */
	    cond = build (NE_EXPR, integer_type_node, 
			  integer_one_node, integer_zero_node);
	    if_stmt = build_stmt (IF_STMT, cond, NULL_TREE, NULL_TREE);
	    dchain_build_scope (&THEN_CLAUSE (if_stmt));
	    dchain_build_scope (&ELSE_CLAUSE (if_stmt));
	    open_scope = COMPOUND_BODY (THEN_CLAUSE (if_stmt));
	    close_scope = NEXT_STMT (open_scope);

	    dchain_two_chains (open_scope, close_scope, node, node,
			       PREV_STMT (node), NEXT_STMT (node), if_stmt, if_stmt);

	    /* Add "if (1!=0) {goto L;}" to goto_list.  */
	    GOTO_PUSH (if_stmt);

	    node = if_stmt;
	    break;
	  }
	  
	case IF_STMT:
	  dchain_build_scope (&THEN_CLAUSE (node));
	  
	  if (TREE_CODE (NEXT_STMT 
			 (COMPOUND_BODY 
			  (THEN_CLAUSE (node)))) == GOTO_STMT
	      && TREE_CODE (NEXT_STMT 
			    (NEXT_STMT 
			     (COMPOUND_BODY 
			      (THEN_CLAUSE (node))))) == SCOPE_STMT)
	    /* That's a conditionnal goto : "if (cond) {goto L;}".  */
	    GOTO_PUSH (node);
	  else
	    {
	      PATH_PUSH (node);
	      ++level;
	      find_labels_gotos (THEN_CLAUSE (node));
	      --level;
	      PATH_POP ();
	    }
	  
	  if (ELSE_CLAUSE (node))
	    {
	      PATH_PUSH (node);
	      ++level;
	      dchain_build_scope (&ELSE_CLAUSE (node));
	      find_labels_gotos (ELSE_CLAUSE (node));
	      --level;
	      PATH_POP ();
	    }
	  break;
	  
	default:
	  break;
	}
      node = NEXT_STMT (node);
    }
}

/* Initialize logical variables.  */

static void 
init_logical_vars ()
{
  gotol it;
  
  GOTO_FOREACH (it, goto_list)
    {
      /* Reset the logical variable to FALSE once the corresponding LABEL_STMT 
	 is reached.  */
      {
	tree def, label_stmt;
	
	/* Build a statement "T=0".  */
	def = build_stmt (EXPR_STMT, build_modify_expr (GOTO_LVAR (it), 
							NOP_EXPR, 
							integer_zero_node));
	
	/* Retrieve a pointer to the corresponding label from the hash table.  */
	label_stmt = label_get_label (GOTO_LABEL (it));
	
	/* Reset the logical variable "T=0" once the label is reached.  */
	dchain_insert_stmt_after (label_stmt, def);
      }
      
      /* Set the logical variable to TRUE if the the GOTO_STMT is reached.  */
      {
	tree if_goto;
	tree def, goto_stmt;
	
	/* Build a statement "T=1".  */
	def = build_stmt (EXPR_STMT, build_modify_expr (GOTO_LVAR (it), 
							NOP_EXPR, 
							integer_one_node));
	if_goto = GOTO_IF_STMT (it);
	goto_stmt = NEXT_STMT (COMPOUND_BODY (THEN_CLAUSE (if_goto)));
	
	/* Replace "if (cond) {goto L;}" by "if (cond) {T=1;}".  */
	dchain_replace_stmt (goto_stmt, def);
      }
    }
}

/* Transform GOTO_STMTs in regular control flow.  */

static void 
transform_gotos ()
{
  gotol prev_g = NULL;
  
  /* Eliminate GOTO_STMTs one by one.  */
  GOTO_FOREACH (g, goto_list)
    /* G := select a goto from GOTO_LIST.  */
    {
      enum rel r;
      tree goto_in;

      GOTO_POP ();
      if (prev_g)
	/* No more need this element : free it.  */
	DELETE (prev_g);
      
      /* L := label matching G.  */
      l = label_hash_lookup (IDENTIFIER_POINTER (DECL_NAME (GOTO_LABEL (g))), false);
      
      goto_in = GOTO_IF_STMT (g);
      
      r = related ();
      switch (r)
	{
	case INDIRECTLY_RELATED:
	  {
	    PATH_TYPE oncp;
	    oncp = outer_non_common_parent ();
	    
	    /* Move the GOTO_STMT out of ONCP, and thus make it 
	       DIRECTLY_RELATED to the corresponding LABEL_STMT.  */
	    flow_out_of (oncp, GOTO_PATH (g), GOTO_IF_STMT (g), GOTO_LVAR (g), 
			 update_paths_if);
	    goto_in = PATH_STMT (oncp);
	  }
	  /* Note that GOTO_LEVEL is not updated, so don't use it any more if
	     'r==INDIRECTLY_RELATED'.  */
	  
	  /* ... fall through ... */
	  
	case DIRECTLY_RELATED:
	  /* Either the goto or the label is nested in some statements.  */
	  if (r == DIRECTLY_RELATED && (GOTO_LEVEL (g) > LABEL_LEVEL (l)))
	    /* The goto is nested.  Move it out to the same level as its LABEL_STMT.  */
	    {
	      PATH_TYPE oncp;
	      oncp = outer_non_common_parent ();
	      
	      /* Move the GOTO_STMT out of ONCP, and thus make it 
		 DIRECTLY_RELATED to the corresponding LABEL_STMT.  */
	      flow_out_of (oncp, GOTO_PATH (g), GOTO_IF_STMT (g), GOTO_LVAR (g), 
			   update_paths_if);
	      goto_in = PATH_STMT (oncp);
	      
	      /* ... fall through in SIBLINGS ...  */
	    }
	  else
	    /* Here the GOTO_STMT is guaranteed to be in GCP's scope.  We have 
	       to move the GOTO_STMT in LABEL_STMT's scope using inward motion.  */
	    {
	      PATH_TYPE label_in;
	      
	      /* Step 1: Identify in label's path the common parent from where
		 to start the inward motion : this pointer is called LABEL_IN.  
		 Note that here we get a pointer to LABEL_PATH that we will 
		 follow for the inward motion.  */
	      label_in = great_common_parent ();
	      
	      if (TREE_CODE (PATH_STMT (PATH_PARENT (label_in))) == IF_STMT 
		  && (goto_in == THEN_CLAUSE (PATH_STMT (PATH_PARENT (label_in))) 
		      || goto_in == ELSE_CLAUSE (PATH_STMT (PATH_PARENT (label_in)))))
		/* g and l were INDIRECTLY_RELATED in 2 different clauses of an IF_STMT.  */
		{
		  tree parent_stmt;
		  label_in = PATH_PARENT (label_in);
		  parent_stmt = PATH_STMT (label_in);
		  goto_in = eliminate_with_do (parent_stmt, parent_stmt, GOTO_LVAR (g));
		}
	      else if (GOTO_OFFSET (g) > LABEL_OFFSET (l))
		/* The GOTO_STMT is after the LABEL_STMT : lift the GOTO_STMT
		   before the LABEL_STMT.  */
		goto_in = eliminate_with_do (PATH_STMT (label_in), goto_in, GOTO_LVAR (g));
	      
	      /* Moves the GOTO_STMT in LABEL_STMT's scope, then eliminates it.  */
	      goto_in = flow_in (label_in, goto_in, GOTO_LVAR (g));
	      eliminate_with_if (goto_in, LABEL_LABEL_STMT (l), GOTO_LVAR (g), 
				 update_paths_if);
	      break;
	    }
	  
	case SIBLINGS:
	  /* G and L are guaranteed to be siblings.  */
	  {
	    if (GOTO_OFFSET (g) < LABEL_OFFSET (l))
	      eliminate_with_if (goto_in, LABEL_LABEL_STMT (l), GOTO_LVAR (g), 
				 update_paths_if);
	    else
	      eliminate_with_do (LABEL_LABEL_STMT (l), goto_in, GOTO_LVAR (g));
	    
	    break;
	  }
	}
      prev_g = g;
    }

  if (prev_g)
    /* No more need this element : free it.  */
    DELETE (prev_g);
}

/* Determine how a LABEL_STMT and a GOTO_STMT are related.  There are three
   cases : SIBLINGS, DIRECTLY_RELATED, and INDIRECTLY_RELATED.

   Def1 : A LABEL_STMT and a GOTO_STMT are SIBLINGS if there exist some statement
   sequence stmt_1; ... stmt_n; such that the LABEL_STMT corresponds to stmt_i 
   and the GOTO_STMT corresponds to stmt_j in the statement sequence.

   Def2 : A LABEL_STMT and a GOTO_STMT are DIRECTLY_RELATED if there exist some 
   statement sequence stmt_1; ... stmt_n; such that either the LABEL_STMT or 
   the GOTO_STMT corresponds to stmt_i and the matching GOTO_STMT or LABEL_STMT
   is nested inside some stmt_j in the statement sequence.

   Def3 : A LABEL_STMT and a GOTO_STMT are INDIRECTLY_RELATED if they appear in
   the same procedure body, but they are neither siblings, nor directly related.  */

static enum rel
related ()
{
  if (GOTO_LEVEL (g) == LABEL_LEVEL (l))
    /* Both at the same nesting level,  */
    {
      if (GOTO_OFFSET (g) < LABEL_OFFSET (l))
	{
	  tree it;
	  for (it = GOTO_IF_STMT (g); 
	       it != NULL_TREE && it != ERROR_MARK; 
	       it = NEXT_STMT (it))
	    {
	      if (it == LABEL_LABEL_STMT (l))
		/* and LABEL_STMT reachable from GOTO_STMT by CHAIN.  */
		return SIBLINGS;
	    }
	  return INDIRECTLY_RELATED;
	}
      else 
	{
	  tree it;
	  for (it = LABEL_LABEL_STMT (l); 
	       it != NULL_TREE && it != ERROR_MARK; 
	       it = NEXT_STMT (it))
	    {
	      if (it == GOTO_IF_STMT (g))
		/* && GOTO_STMT reachable from LABEL_STMT by CHAIN.  */
		return SIBLINGS;
	    }
	  return INDIRECTLY_RELATED;
	}
    }

  else if (GOTO_LEVEL (g) == 0 || LABEL_LEVEL (l) == 0)
    /* One of them is at the outermost level.  */
    return DIRECTLY_RELATED;

  else
    /* Not at the same nesting level.  */
    {
      PATH_TYPE gp, lp;
      
      gp = PATH_ANCESTOR (GOTO_PATH (g));
      lp = PATH_ANCESTOR (LABEL_PATH (l));
      
      while (gp != NULL && lp != NULL)
	{
	  if (PATH_STMT (gp) != PATH_STMT (lp))
	    /* At least one position in the path differs.  */
	    return INDIRECTLY_RELATED;

	  /* Next iteration.  */
	  gp = PATH_CHILD (gp);
	  lp = PATH_CHILD (lp);
	}

      /* All positions match up to a point, and one of the paths is shorter than
	 the other.  */
      return DIRECTLY_RELATED;
    }
}

/* Identify the parent of a goto from where its path differs from corresponding 
   label's path.  */

static PATH_TYPE
outer_non_common_parent ()
{
  PATH_TYPE gp, lp;
  
  /* Begin the search from the outer COMPOUND_STMT : function's level.  */
  gp = PATH_ANCESTOR (GOTO_PATH (g));
  lp = PATH_ANCESTOR (LABEL_PATH (l));
  
  while (lp != PATH_NULL && PATH_STMT (gp) == PATH_STMT (lp))
    {
      gp = PATH_CHILD (gp);
      lp = PATH_CHILD (lp);
    }
  return gp;
}

/* Identify the greatest common parent of a goto and label paths and returns a 
   pointer to the LABEL_PATH.  */

static PATH_TYPE
great_common_parent ()
{
  PATH_TYPE gp, lp;
  
  /* Begin the search from the outer COMPOUND_STMT : function's level.  */
  gp = PATH_ANCESTOR (GOTO_PATH (g));
  lp = PATH_ANCESTOR (LABEL_PATH (l));
  
  while (gp != PATH_NULL && PATH_STMT (gp) == PATH_STMT (lp))
    {
      gp = PATH_CHILD (gp);
      lp = PATH_CHILD (lp);
    }
  return lp;
}

/* This function moves the control flow (from LAST_EXECUTED) into the 
   COMPOUND_STMT pointed by the last element in PATH, under the control of the
   logical variable LVAR.  */

static tree
flow_in (label_in, goto_in, lvar)
     PATH_TYPE label_in;
     tree goto_in, lvar;
{
  /* For each statement in PATH, up to the corresponding LABEL_STMT,  */
  while (label_in)
    {
      tree label_in_stmt = PATH_STMT (label_in);
      
      eliminate_with_if (goto_in, label_in_stmt, lvar, update_paths_if);
      
      /* Step 2: Insert "lvar!=0 || ..." in loop's condition.  */
      /* Step 3: Update the LAST_EXECUTED statement to point to the first 
	 statement in the new scope.  */
      /* Step 4: Update PATH to point to the next statement to be entered.  */
      switch (TREE_CODE (label_in_stmt))
	{
	case WHILE_STMT:
	case FOR_STMT:
	  insert_orif_in_condition (lvar, label_in_stmt);
	  goto_in = tree_last_decl (COMPOUND_BODY (PATH_STMT (PATH_CHILD (label_in))));
	  label_in = PATH_CHILD (PATH_CHILD (label_in));
	  break;
	  
	case DO_STMT:
	  goto_in = tree_last_decl (COMPOUND_BODY (DO_BODY (label_in_stmt)));
	  label_in = PATH_CHILD (PATH_CHILD (label_in));
	  break;
	  
	case IF_STMT:
	  /* Determine in which clause we have to flow in.  */
	  if (PATH_STMT (PATH_CHILD (label_in)) == THEN_CLAUSE (label_in_stmt))
	    /* We enter the THEN_CLAUSE : "if (!lvar && condition)".  */
	    insert_orif_in_condition (lvar, label_in_stmt);
	  else
	    /* We enter the ELSE_CLAUSE : negate the logical variable, 
	       that is equivalent to say "lvar==0 && ..."  */
	    {
	      tree not_lvar;
	      not_lvar = build (EQ_EXPR, integer_type_node, lvar, integer_zero_node);
	      insert_andif_in_condition (not_lvar, label_in_stmt);
	    }
	  goto_in = tree_last_decl (COMPOUND_BODY (PATH_STMT (PATH_CHILD (label_in))));
	  label_in = PATH_CHILD (PATH_CHILD (label_in));
	  break;
	  
	case COMPOUND_STMT:
	  goto_in = tree_last_decl (COMPOUND_BODY (PATH_STMT (label_in)));
	  label_in = PATH_CHILD (label_in);
	  break;
	  	  
	default:
	  /* Nothing fall down here since all PATH elements are listed above.  */
	  abort ();
	  break;
	}
      /* Next iteration.  */
    }
  return goto_in;
}

/* Copied from c-simplify.  Maybe we have to declare it as extern and avoid to duplicate code...
   Returns the last DECL_STMT in the scope SCOPE.  */

static tree
tree_last_decl (scope)
     tree scope;
{
  tree last;

  /* Be sure that we get a scope.  */
  if (!SCOPE_BEGIN_P (scope))
    abort ();

  /* Find the last declaration statement in the scope.  */
  last = scope;
  while (TREE_CHAIN (last) && TREE_CODE (TREE_CHAIN (last)) == DECL_STMT)
    last = TREE_CHAIN (last);

  return last;
}

/* For a given statement BEFORE_STMT in the same scope as AFTER_STMT, transform
   the code between these two statements such that the code is not executed when
   LVAR is true.  */

void
eliminate_with_if (before_stmt, after_stmt, lvar, update_paths_fnct)
     tree before_stmt, after_stmt, lvar;
     void (*update_paths_fnct) PARAMS ((tree, tree, tree, tree));
{
  tree open_scope, close_scope;
  tree new_if;

  if (NEXT_STMT (before_stmt) == after_stmt)
    /* There are no statements to be removed from the flow since AFTER_STMT
       follows BEFORE_STMT.  */
    return;

  /* Include statements that we find between BEFORE_STMT and LABEL_STMT in the
     THEN_CLAUSE of an IF_STMT : that code is not executed when the logical 
     variable LVAR is true.  */
  new_if = build_stmt (IF_STMT, build (EQ_EXPR, integer_type_node, lvar, 
				       integer_zero_node), 
		       NULL_TREE, NULL_TREE);

  dchain_build_scope (&THEN_CLAUSE (new_if));
  dchain_build_scope (&ELSE_CLAUSE (new_if));

  open_scope = COMPOUND_BODY (THEN_CLAUSE (new_if));
  close_scope = NEXT_STMT (open_scope);
  
  dchain_two_chains (open_scope, close_scope, 
		     NEXT_STMT (before_stmt), PREV_STMT (after_stmt),
		     before_stmt, after_stmt, new_if, new_if);

  /* Update other paths.  */
  update_paths_fnct (new_if, THEN_CLAUSE (new_if), open_scope, close_scope);
}

/* Include all statements between BEFORE_STMT and AFTER_STMT into a DO_STMT
   controlled by the logical variable lvar.  Returns a pointer to the first 
   statement of the DO_BODY.  */

static tree
eliminate_with_do (before_stmt, after_stmt, lvar)
     tree before_stmt, after_stmt, lvar;
{
  tree do_loop;
  tree open_scope, close_scope;
  
  do_loop = build_stmt (DO_STMT, lvar, NULL_TREE);
  dchain_build_scope (&DO_BODY (do_loop));

  open_scope = COMPOUND_BODY (DO_BODY (do_loop));
  close_scope = NEXT_STMT (open_scope);
  
  dchain_two_chains (open_scope, close_scope, before_stmt, after_stmt,
		     PREV_STMT (before_stmt), NEXT_STMT (after_stmt), 
		     do_loop, do_loop);

  /* Update other paths.  */
  update_paths_do (do_loop, DO_BODY (do_loop), before_stmt, after_stmt);

  return tree_last_decl (COMPOUND_BODY (DO_BODY (do_loop)));
}

/* Update all paths of labels and gotos if their offset is between G and L. 
   ST1 and ST2 are two nodes that have to be added to the new path : they are 
   for example a DO_STMT and its DO_BODY if we are called from eliminate_with_do.
   All paths that contain a statement between BEGIN_STMT excluded and END_STMT
   excluded are updated.  */

static void
update_paths_if (st1, st2, begin_stmt, end_stmt)
     tree st1, st2, begin_stmt, end_stmt;
{
  tree it_stmt;

  for (it_stmt = NEXT_STMT (begin_stmt); it_stmt != end_stmt; it_stmt = NEXT_STMT (it_stmt))
    /* For each statement between BEFORE_STMT and AFTER_STMT,  */
    {
      switch (TREE_CODE (it_stmt))
	{
	case COMPOUND_STMT:
	case FOR_STMT:
	case WHILE_STMT:
	case DO_STMT:
	case IF_STMT:
	  /* a statement that can potentially contain a goto or a label.  */
	  {
	    gotol it_g;
	    label it_l;
	    PATH_TYPE path;
	    
	    GOTO_FOREACH (it_g, goto_list)
	      {
		path = path_contain_stmt (GOTO_PATH (it_g), it_stmt);
		if (path)
		  path_include (path, st1, st2, PATH_CHILD (path));
		
		/* IT_L is the label associated to the goto pointed by IT_G.  */
		it_l = label_hash_lookup (IDENTIFIER_POINTER (DECL_NAME (GOTO_LABEL (it_g))), false);
		path = path_contain_stmt (LABEL_PATH (it_l), it_stmt);
		if (path)
		{
		  tree tmp;
		  tmp = path_contain_stmt (LABEL_PATH (it_l), st1);
		  /* Don't include twice the path in the label's path (when 
		     several gotos refers to the same label).  */
		  if (tmp == PATH_NULL)
		    path_include (path, st1, st2, PATH_CHILD (path));
		}
	      }
	    break;
	  }

	case LABEL_STMT:
	  {
	    label lab;
	    lab = label_hash_lookup (IDENTIFIER_POINTER (DECL_NAME (TREE_OPERAND (it_stmt, 0))), false);

	    path_include (LABEL_PATH (lab), st1, st2, PATH_NULL);
	    
	    LABEL_PATH (lab) = PATH_CHILD (PATH_CHILD (LABEL_PATH (lab)));
	    break;
	  }

	default:
	  break;
	}
    }
}

/* Update all paths of labels and gotos if their offset is between G and L. 
   ST1 and ST2 are two nodes that have to be added to the new path : they are 
   for example a DO_STMT and its DO_BODY if we are called from eliminate_with_do.
   All paths that contain a statement between BEGIN_STMT excluded and END_STMT
   excluded are updated.  */

static void
update_paths_do (st1, st2, begin_stmt, end_stmt)
     tree st1, st2, begin_stmt, end_stmt;
{
  do {
    /* For each statement between BEFORE_STMT and AFTER_STMT,  */
    switch (TREE_CODE (begin_stmt))
      {
      case COMPOUND_STMT:
      case FOR_STMT:
      case WHILE_STMT:
      case DO_STMT:
      case IF_STMT:
	/* a statement that can potentially contain a goto or a label.  */
	{
	  gotol it_g;
	  label it_l;
	  PATH_TYPE path;
	  
	  GOTO_FOREACH (it_g, goto_list)
	    {
	      path = path_contain_stmt (GOTO_PATH (it_g), begin_stmt);
	      if (path)
		path_include (PATH_PARENT (path), st1, st2, path); 
			      
	      /* IT_L is the label associated to the goto pointed by IT_G.  */
	      it_l = label_hash_lookup (IDENTIFIER_POINTER (DECL_NAME (GOTO_LABEL (it_g))), false);
	      path = path_contain_stmt (LABEL_PATH (it_l), begin_stmt);
	      if (path)
		{
		  tree tmp;
		  tmp = path_contain_stmt (LABEL_PATH (it_l), st1);
		  if (tmp == PATH_NULL)
		    path_include (PATH_PARENT (path), st1, st2, path);
		}
	    }
	  break;
	}
	
      case LABEL_STMT:
	{
	  label lab;
	  lab = label_hash_lookup (IDENTIFIER_POINTER (DECL_NAME (TREE_OPERAND (begin_stmt, 0))), false);
	  path_include (LABEL_PATH (lab), st1, st2, PATH_NULL);
	  LABEL_PATH (lab) = PATH_CHILD (PATH_CHILD (LABEL_PATH (lab)));
	  break;
	}
	
      default:
	break;
      }
  } while (begin_stmt != end_stmt 
	   && (begin_stmt = NEXT_STMT (begin_stmt)));
}

/* Include S1 and S2 in the path between P1 and P2.  */

static void 
path_include (p1, s1, s2, p2)
     PATH_TYPE p1, p2;
     tree s1, s2;
{
  PATH_TYPE t1, t2;
  t1 = PATH_NEW (s1);
  t2 = PATH_NEW (s2);
  if (p1)
    PATH_CHILD (p1) = t1;
  PATH_CHILD (t1) = t2;
  PATH_CHILD (t2) = p2;
  if (p2)
    PATH_PARENT (p2) = t2;
  PATH_PARENT (t2) = t1;
  PATH_PARENT (t1) = p1;
}

 /* Returns a pointer on PATH when STMT belongs to this path.  */

static PATH_TYPE
path_contain_stmt (path, stmt)
     PATH_TYPE path;
     tree stmt;
{
  PATH_TYPE it_p;

  PATH_FOREACH (it_p, path)
    {
      if (PATH_STMT (it_p) == stmt)
	return it_p;
    }
  return PATH_NULL;
}

/* Create a new entry for the label hash table.
   Passed to hash_table_init.  */

static struct hash_entry *
label_hash_newfunc (entry, table, string)
     struct hash_entry *entry;
     struct hash_table *table;
     hash_table_key string ATTRIBUTE_UNUSED;
{
  label ret = (label) entry;
  if (ret == NULL)
    {
      ret = ((label)
	     hash_allocate (table, sizeof (struct label_hash_entry)));
      if (ret == NULL)
	return NULL;
    }
  ret->path = NULL;
  ret->id = NULL;
  return (struct hash_entry *) ret;
}

/* From a label declaration retrieve from the label hash table the LABEL_STMT.  */

static tree
label_get_label (lab_decl)
     tree lab_decl;
{
  label entry = NULL;
  entry = label_hash_lookup (IDENTIFIER_POINTER (DECL_NAME (lab_decl)), false);
  return LABEL_LABEL_STMT (entry);
}

/* Look up an entry in the label hash table.  If CREATE is not null, an entry 
   is created in the hash table.  */

static label 
label_hash_lookup (string, create)
     const char *string;
     int create;
{
  return ((label)
	  hash_lookup (&label_table, (const hash_table_key) string, create, 
		       string_copy));
}


#if 1
/* Debugging functions.  */
static void debug_path                         PARAMS ((PATH_TYPE));
static void debug_path_elt                     PARAMS ((PATH_TYPE));
static void debug_goto_list                    PARAMS ((gotol));
static void debug_label_hash_entry             PARAMS ((label));

/* A function for debuging the path.  */

static void
debug_path (list)
     PATH_TYPE list;
{
  PATH_TYPE it;
  it = PATH_ANCESTOR (list);
  while (it)
    {
      debug_path_elt (it);
      it = PATH_CHILD (it);
    }
}

/* A function for debuging a path element.  */

static void
debug_path_elt (elt)
     PATH_TYPE elt;
{
  fprintf (stderr, "<path_node> \n");
  debug_c_node (PATH_STMT (elt));
  fprintf (stderr, "</path_node> \n");
}

/* A function for debugging the goto list.  */

static void 
debug_goto_list (list)
     gotol list;
{
  gotol it;
  GOTO_FOREACH (it, list)
    {
      fprintf (stderr, "--- GOTO --------------------------\n");
      fprintf (stderr, "offset: %d\n", GOTO_OFFSET (it));
      fprintf (stderr, "level: %d\n", GOTO_LEVEL (it));
      fprintf (stderr, "if_stmt:\n");
      debug_c_node (GOTO_IF_STMT (it));
      fprintf (stderr, "path:\n");
      debug_path (GOTO_PATH (it));
      fprintf (stderr, "===================================\n");
    }
}

/* A function for debuging a label_hash_entry structure.  */

static void 
debug_label_hash_entry (entry)
     label entry;
{
  fprintf (stderr, "--- LABEL -------------------------\n");
  fprintf (stderr, "offset: %d\n", LABEL_OFFSET (entry));
  fprintf (stderr, "level: %d\n", LABEL_LEVEL (entry));
  fprintf (stderr, "label_id: %s\n", LABEL_ID (entry));
  fprintf (stderr, "label_stmt:\n");
  debug_c_node (LABEL_LABEL_STMT (entry));
  fprintf (stderr, "path:\n");
  debug_path (LABEL_PATH (entry));
  fprintf (stderr, "===================================\n");
}

#endif
