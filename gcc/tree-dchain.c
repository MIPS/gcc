/* Interface to the double linked statement chain. 
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
#include "tree-dchain.h"


/* Next functions define the transformation from the simple linked statement
   chain to the double linked statement chain and an interface that allows 
   operations on it.  
   This implementation is based on the fact that 'type' field is not used for 
   representing statement nodes.  The PREV_STMT pointer is stored in the 'type' 
   field, the NEXT_STMT is just an alias for TREE_CHAIN.  That makes the 
   interface used to walk trees closer to RTL interface.  */



/* Store information about previous statements in the tree structure.  */

void 
double_chain_stmts (node)
     tree node;
{
  tree prev;

  if (node == NULL_TREE)
    return;

  /* The PREV_STMT points to NULL_TREE for the first statement in the chain.  */
  prev = NULL_TREE;
  
  while (node)
    {
      switch (TREE_CODE (node))
	{
	case COMPOUND_STMT:
	  double_chain_stmts (COMPOUND_BODY (node));
	  PREV_STMT (node) = prev;
	  break;

	case FOR_STMT:
	  double_chain_stmts (FOR_INIT_STMT (node));
	  double_chain_stmts (FOR_BODY (node));
	  PREV_STMT (node) = prev;
	  break;

	case WHILE_STMT:
	  double_chain_stmts (WHILE_BODY (node));
	  PREV_STMT (node) = prev;
	  break;

	case DO_STMT:
	  double_chain_stmts (DO_BODY (node));
	  PREV_STMT (node) = prev;
	  break;

	case IF_STMT:
	  double_chain_stmts (THEN_CLAUSE (node));
	  double_chain_stmts (ELSE_CLAUSE (node));
	  PREV_STMT (node) = prev;
	  break;

	case SWITCH_STMT:
	  double_chain_stmts (SWITCH_BODY (node));
	  PREV_STMT (node) = prev;
	  break;

	case EXPR_STMT:
	case DECL_STMT:
	case RETURN_STMT:
	case SCOPE_STMT:
	case FILE_STMT:
	case LABEL_STMT:
	case GOTO_STMT:
	case ASM_STMT:
	case CASE_LABEL:
	case CONTINUE_STMT:
	case BREAK_STMT:
	  PREV_STMT (node) = prev;
	  break;

	default:
	  break;
	}
      prev = node;
      node = NEXT_STMT (node);
    }
}

/* Free all information about previous statements from the tree structure.  */

void 
double_chain_free (node)
     tree node;
{
  while (node)
    {
      switch (TREE_CODE (node))
	{
	case COMPOUND_STMT:
	  double_chain_stmts (COMPOUND_BODY (node));
	  PREV_STMT (node) = NULL_TREE;
	  break;
	  
	case FOR_STMT:
	  double_chain_stmts (FOR_INIT_STMT (node));
	  double_chain_stmts (FOR_BODY (node));
	  PREV_STMT (node) = NULL_TREE;
	  break;

	case WHILE_STMT:
	  double_chain_stmts (WHILE_BODY (node));
	  PREV_STMT (node) = NULL_TREE;
	  break;

	case DO_STMT:
	  double_chain_stmts (DO_BODY (node));
	  PREV_STMT (node) = NULL_TREE;
	  break;

	case IF_STMT:
	  double_chain_stmts (THEN_CLAUSE (node));
	  double_chain_stmts (ELSE_CLAUSE (node));
	  PREV_STMT (node) = NULL_TREE;
	  break;

	case SWITCH_STMT:
	  double_chain_stmts (SWITCH_BODY (node));
	  PREV_STMT (node) = NULL_TREE;
	  break;

	case EXPR_STMT:
	case DECL_STMT:
	case RETURN_STMT:
	case SCOPE_STMT:
	case FILE_STMT:
	case LABEL_STMT:
	case GOTO_STMT:
	case ASM_STMT:
	case CASE_LABEL:
	case CONTINUE_STMT:
	case BREAK_STMT:
	  PREV_STMT (node) = NULL_TREE;
	  break;

	default:
	  break;
	}
      node = NEXT_STMT (node);
    }
}

/* Inserts STMT after LOC.
   Example :
   Given a chain <s1; s2;> calling "dchain_insert_stmt_after (s1, insert)"
   will produce the chain <s1; insert; s2;>.  */

void 
dchain_insert_stmt_after (loc, stmt)
     tree loc, stmt;
{
  PREV_STMT (NEXT_STMT (loc)) = stmt;
  PREV_STMT (stmt) = loc;
  NEXT_STMT (stmt) = NEXT_STMT (loc);
  NEXT_STMT (loc) = stmt;
}

/* Inserts STMT before LOC.
   Example :
   Given a chain <s1; s2;> calling "dchain_insert_stmt_after (s2, insert)" 
   will produce the chain <s1; insert; s2;>.  */

void 
dchain_insert_stmt_before (loc, stmt)
     tree loc, stmt;
{
  NEXT_STMT (PREV_STMT (loc)) = stmt;
  NEXT_STMT (stmt) = loc;
  PREV_STMT (stmt) = PREV_STMT (loc);
  PREV_STMT (loc) = stmt;
}

/* Delete all statements between BEGIN excluded and END excluded from the double
   linked chain of statements.  
   Example : 
   Given a chain <s1; stmts_to_delete; s2;> calling dchain_delete_stmts (s1, s2)
   will produce the chain <s1; s2;>.
   If END is not reachable from BEGIN through NEXT_STMT, it is an error.  */

void 
dchain_delete_stmts (begin, end)
     tree begin, end;
{
#ifdef ENABLE_TREE_CHECKING
  {
    tree it;
    for (it = begin; it != end && it != NULL_TREE; it = NEXT_STMT (it)) ;
    if (it == NULL_TREE)
      abort ();  /* END is not reachable by tree chain from BEGIN.  */
  }
#endif
  NEXT_STMT (begin) = end;
  PREV_STMT (end) = begin;
}

/* Insert a statement chain <CH_BEGIN included, CH_END included> between BEGIN 
   and END in the double linked chain of statements.  
   Example :
   Given two chains <s1; s2;> and <t1; t2; t3;> applying 
   dchain_insert_chain (s1, s2, t1, t3)
   produces the following chain : <s1; t1; t2; t3; s2;>.
   If END is not reachable from BEGIN through NEXT_STMT, or if CH_END is not 
   reachable from CH_BEGIN, it is an error.  */

void 
dchain_insert_chain (begin, end, ch_begin, ch_end)
     tree begin, end, ch_begin, ch_end;
{
#ifdef ENABLE_TREE_CHECKING
  {
    tree it;

    for (it = ch_begin; it != ch_end && it != NULL_TREE; it = NEXT_STMT (it));
    if (it == NULL_TREE)
      abort ();  /* CH_END is not reachable by tree chain from CH_BEGIN.  */
  }
#endif
  NEXT_STMT (begin) = ch_begin;
  PREV_STMT (ch_begin) = begin;

  PREV_STMT (end) = ch_end;
  NEXT_STMT (ch_end) = end;
}

/* This has the effect of 2 applications of dchain_insert_chain, but is simpler 
   to use when dealing with it since parameters are evaluated before the 
   execution of the function.
   
   This function acts as follows :
   <b1, e1> and <b2, e2> are two chains in which we will insert respectively 
   <cb1, ..., ce1> and <cb2, ..., ce2> : at the end of this process we obtain 
   two chains : <b1, cb1, ..., ce1, e1> and <b2, cb2, ..., ce2, e2>.
   
   Note that statements between <b1, ..., e1> and between <b2, ..., e2> are 
   removed from the chain (if there was one...).  */

void 
dchain_two_chains (b1, e1, cb1, ce1, b2, e2, cb2, ce2)
     tree b1, e1, cb1, ce1, b2, e2, cb2, ce2;
{
#ifdef ENABLE_TREE_CHECKING
  {
    tree it;

    for (it = cb1; it != ce1 && it != NULL_TREE; it = NEXT_STMT (it));
    if (it == NULL_TREE)
      abort ();  /* CE1 is not reachable by tree chain from CB1.  */

    for (it = cb2; it != ce2 && it != NULL_TREE; it = NEXT_STMT (it));
    if (it == NULL_TREE)
      abort ();  /* CE2 is not reachable by tree chain from CB2.  */
  }
#endif
  NEXT_STMT (b1) = cb1;
  PREV_STMT (cb1) = b1;
  PREV_STMT (e1) = ce1;
  NEXT_STMT (ce1) = e1;

  NEXT_STMT (b2) = cb2;
  PREV_STMT (cb2) = b2;
  PREV_STMT (e2) = ce2;
  NEXT_STMT (ce2) = e2;
}


/* Copy a statement by using copy_node when needed, but ensuring that we copy 
   enough information in order to have distinct statements.  
   This is the double linked stmt chain version for deep_copy_node.  */

tree
dchain_deep_copy_node (stmt)
     tree stmt;
{
  tree res;
  res = deep_copy_node (stmt);
  double_chain_stmts (res);
  return res;
}

/* Replaces STMT by REPLACEMENT in the double linked chain of statements.
   Example :
   Given a chain <s1; s2; s3;> calling dchain_replace_stmt (s2, r) 
   will produce the chain <s1; r; s3;>.  */

void 
dchain_replace_stmt (stmt, replacement)
     tree stmt, replacement;
{
  dchain_insert_stmt_before (stmt, replacement);
  dchain_delete_stmts (PREV_STMT (stmt), NEXT_STMT (stmt));
}

/* Construct a scope around STMT_P and verify that the begin and end scopes are 
   correctly double chained.  */

void 
dchain_build_scope (stmt_p)
     tree *stmt_p;
{
  tree_build_scope (stmt_p);

  /* Double chain.  */
  {
    tree open_scope;
    tree close_scope, prev_close_scope;
    open_scope = COMPOUND_BODY (*stmt_p);
    PREV_STMT (NEXT_STMT (open_scope)) = open_scope;
    
    close_scope = open_scope;
    prev_close_scope = open_scope;

    while (NEXT_STMT (close_scope))
      {
	prev_close_scope = close_scope;
	close_scope = NEXT_STMT (close_scope);
      }
    PREV_STMT (close_scope) = prev_close_scope;
  }
}

#if 0
/* Declares all the variables in VARS in SCOPE for the double linked chain. 
   Returns the last DECL_STMT emitted.  */

tree 
dchain_declare_tmp_vars (vars, scope)
     tree vars, scope;
{
  tree last, it;
  last = declare_tmp_vars (vars, scope);
  
  /* Back chain declarations.  */
  for (it = scope; it != last; it = NEXT_STMT (it))
    {
      PREV_STMT (NEXT_STMT (it)) = it;
    }
  /* Back chain last declaration with the first statement.  */
  PREV_STMT (NEXT_STMT (it)) = it;
  
  return last;
}
#endif

/* Delete all LABEL_STMTs from the statement STMT.  */

void 
dchain_delete_labels (stmt)
     tree stmt;
{
  while (stmt)
    {
      switch (TREE_CODE (stmt))
	{
	case COMPOUND_STMT:
	  dchain_delete_labels (COMPOUND_BODY (stmt));
	  break;
	  
	case FOR_STMT:
	  dchain_delete_labels (FOR_INIT_STMT (stmt));
	  dchain_delete_labels (FOR_BODY (stmt));
	  break;
	  
	case WHILE_STMT:
	  dchain_delete_labels (WHILE_BODY (stmt));
	  break;
	  
	case DO_STMT:
	  dchain_delete_labels (DO_BODY (stmt));
	  break;
	  
	case IF_STMT:
	  dchain_delete_labels (THEN_CLAUSE (stmt));
	  dchain_delete_labels (ELSE_CLAUSE (stmt));
	  break;
	  
	case SWITCH_STMT:
	  dchain_delete_labels (SWITCH_BODY (stmt));
	  break;
	  
	case LABEL_STMT:
	  dchain_delete_stmts (PREV_STMT (stmt), NEXT_STMT (stmt));
	  break;
	  
	case EXPR_STMT:
	case DECL_STMT:
	case RETURN_STMT:
	case SCOPE_STMT:
	case FILE_STMT:
	case GOTO_STMT:
	case ASM_STMT:
	case CASE_LABEL:
	case CONTINUE_STMT:
	case BREAK_STMT:
	default:
	  break;
	}
      stmt = NEXT_STMT (stmt);
    }
}


