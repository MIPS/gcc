/* Call site versioning
   Copyright (C) 2005 Free Software Foundation, Inc.

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "langhooks.h"
#include "ggc.h"
#include "target.h"
#include "cgraph.h"
#include "function.h"
#include "ipa-prop.h"
#include "cfghooks.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "splay-tree.h"
#include "tree-inline-aux.h"
#include "tree-pass.h"
#include "flags.h"
#include "timevar.h"

typedef struct loop_info
{
  basic_block preheader;
  basic_block postheader;
  basic_block loop_begin;
  basic_block loop_end;
} loop_info;

#define SWITCH_BLOCKS_NUM 6
#define DUPLICATE_BLOCKS_NUM 6
typedef struct duplicate_info
{
  int n_switch_bb;
  basic_block switch_blocks[SWITCH_BLOCKS_NUM];
  int n_duplicate_bb;
  basic_block duplicate_blocks[DUPLICATE_BLOCKS_NUM]; 
  basic_block merge_block;
} duplicate_info;

bool
type_to_consider (tree type);
bool arg_defined_in_statement_p (tree stmt, tree argument);
bool
has_defs (tree call, basic_block * switch_blocks, int switch_blocks_num);
tree
loop_find_call (struct loop_info *loopinfo);



bool
has_defs (tree call, basic_block * switch_blocks, int switch_blocks_num)
{
  int i = 0, j = 0;
  tree arg,stmt;
  basic_block current;
  block_stmt_iterator bsi;
  gcc_assert (TREE_CODE (call) == CALL_EXPR);
  arg = TREE_OPERAND (call, 1);
                                                                                           
/*  fprintf (dump_file, "In has_defs\n");                                                                                           */
 
  for (i = 0; arg != NULL_TREE && i < switch_blocks_num; arg = TREE_CHAIN (arg), i++)
    {
/*fprintf (dump_file, "In for. i = %d\n",i);*/
                                                                                           
                                                                                           
      if (type_to_consider (arg))
        {
          /* fprintf (dump_file, "Exited type to consider!\n");*/
          j = 0;
          while (j < switch_blocks_num && switch_blocks[j] != NULL)
            {
                                                                                           
              /* fprintf (dump_file, "Reading basic block %d",j); */
               current = switch_blocks[j];
   /* fprintf (dump_file, "Read %d",j); */
              for (bsi = bsi_start (/*switch_blocks[j])*/current); !bsi_end_p (bsi);
                   bsi_next (&bsi))
                {
/*                   fprintf (dump_file, "In while's for !\n");                                                                                            */
                  stmt = bsi_stmt (bsi);
                  if (arg_defined_in_statement_p (stmt, arg))
                  {
/*                    fprintf (dump_file, "Found !!!\n"); */
                    return true;
                  }
                }
             j++;
            }
        }
    }
    /*fprintf (dump_file, "Exiting !!!\n");                                                                                            */
  return false;
}


bool
type_to_consider (tree type)
{
/*   fprintf (dump_file, "In type to consider!\n"); */
                                                                                           
  /* Strip the *'s off.  */
  type = TREE_TYPE(TREE_VALUE(type));
 /* while (POINTER_TYPE_P (type) || TREE_CODE (type) == ARRAY_TYPE)
    type = TYPE_MAIN_VARIANT (TREE_TYPE (type));
  */                                                                                            
  switch (TREE_CODE (type))
    {
    case BOOLEAN_TYPE:
    case CHAR_TYPE:
    case COMPLEX_TYPE:
    case ENUMERAL_TYPE:
    case INTEGER_TYPE:
    case QUAL_UNION_TYPE:
    case REAL_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case VECTOR_TYPE:
    case VOID_TYPE:
      return true;
                                                                                           
                                                                                           
    default:
      return false;
    }
}

bool arg_defined_in_statement_p (tree stmt, tree argument)
{
        if (TREE_CODE(stmt) == MODIFY_EXPR)
                if (TREE_OPERAND (stmt, 0) == TREE_VALUE(argument))
                        return true;
        return false;
}

tree
loop_find_call (struct loop_info *loopinfo)
{
  basic_block currloop[2], bb = NULL;
  int loopnumbb = 2;
  int count;
  block_stmt_iterator bsi;
  tree call, stmt, decl;
  currloop[0] = loopinfo->loop_begin;
  currloop[1] = loopinfo->loop_end;
  /* fprintf (dump_file, "check loop bb %d \n", 4444); */
  /*basic_block bb; */
  for (count = 0; count < loopnumbb; count++)
    {
      if ((bb = currloop[count]) != NULL)
        for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
          {
           /* fprintf (dump_file, "hi check loop bb %d \n", bb->index); */
            stmt = bsi_stmt (bsi);
            call = get_call_expr_in (stmt);
            if (call && (decl = get_callee_fndecl (call)))
              {
/*                fprintf (dump_file,"call bb %d \n", bb->index); */
                return call;
              }
          }
    }
                                                                                           
  return NULL_TREE;
}
                                                                                           


static int
check_single_loop (basic_block bb, loop_info *li)
{
  edge e;
  edge_iterator ei;
  basic_block bb1;
  int found;

  if (dump_file)
    fprintf (dump_file, "check loop bb %d \n", bb->index);

  if (EDGE_COUNT (bb->succs) != 2)
    return 0;
  found = 0;
  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      bb1 = e->dest;
      if (!bb1)
	continue;
      if (dump_file)
        fprintf (dump_file, "check succs bb1 %d \n", bb1->index);

      if ((EDGE_COUNT (bb1->preds) != 1) ||
	  (EDGE_COUNT (bb1->succs) != 1) ||
	  (EDGE_SUCC (bb1, 0)->dest) != bb )
	{
	  li->postheader = bb1; 
	  continue;
	}


      if (found)
	return 0;

      li->loop_begin = bb;
      li->loop_end = bb1;
      found = 1;
    }

  if (!found)
    return 0;

  if (!li->postheader)
    return 0;

  if (dump_file)
    {
      fprintf (dump_file, "postheader %d\n", li->postheader->index);
      fprintf (dump_file, "header %d\n", li->loop_begin->index);
      fprintf (dump_file, "latch %d\n", li->loop_end->index);
    }

  if (EDGE_COUNT (bb->preds) != 2)
      return 0;
  FOR_EACH_EDGE (e, ei, bb->preds)
    {
      bb1 = e->src;
      if (!bb1 || 
	  (bb1 == li->loop_end))
	continue;

      if (li->preheader)
	return 0;
      li->preheader = bb1;
    }

  if (!li->preheader ||
      EDGE_COUNT (li->preheader->preds) < 2 ||
      EDGE_COUNT (li->preheader->succs) != 1)
    return 0;

  if (dump_file)
    fprintf (dump_file, "prehader %d\n", li->preheader->index);

  if (li->preheader->next_bb != li->loop_end ||
      li->loop_end->next_bb != li->loop_begin ||
      li->loop_begin->next_bb != li->postheader)
    return 0;
      
  return found;
}

static int
find_switch_blocks (loop_info *li, duplicate_info *di)
{
  edge e;
  edge_iterator ei;
  basic_block bb1;
  int i;

  bb1 = li->preheader;
  i = 0;
  FOR_EACH_EDGE (e, ei, bb1->preds) 
    {
      if (i >= SWITCH_BLOCKS_NUM)
	return 0;
      if (EDGE_COUNT (e->src->succs) != 1)
	continue;
      di->switch_blocks[i++] = e->src;
    }
  if (!i)
    return 0;
  di->n_switch_bb = i;


  i = 0;
  di->duplicate_blocks[i++] = bb1;
  di->duplicate_blocks[i++] = li->loop_end;
  di->duplicate_blocks[i++] = li->loop_begin;
  di->n_duplicate_bb = i;

  if (dump_file)
    {
      for (i = 0; i < di->n_switch_bb; i++)
	fprintf (dump_file, "switch %d\n",di->switch_blocks[i]->index);
      for (i = 0; i < di->n_duplicate_bb; i++)
	fprintf (dump_file, "duplicate %d\n",di->duplicate_blocks[i]->index);
    }

  /*  di->merge_block = di->duplicate_blocks [di->n_duplicate_bb - 1]->next_bb; remove after CHECK */
  di->merge_block = li->postheader;

#if 0
  for (i = 0; i < di->n_switch_bb; i++)
    { 
      edge e;
      block_stmt_iterator bsi;

      bsi = bsi_last (di->switch_blocks[i]);

      /* enable after CHECK.
      tree goto_stmt;
      goto_stmt = bsi_stmt (bsi);
      if (TREE_CODE (goto_stmt) == GOTO_EXPR)
	 return 0;;
	 bsi_prev (&bsi); */
      e = split_block (di->switch_blocks[i], bsi_stmt (bsi));
    }
#endif

  return 1;
}

static void
duplicate_code_path (duplicate_info *di, int index, inline_data *id)
{
  gcov_type count_scale; 
  int frequency_scale;
  int j;
  splay_tree st;
  basic_block after_switch;

  if (dump_file)
    fprintf (dump_file, "path %d\n", di->switch_blocks[index]->index);
  count_scale = 1;
  if (di->switch_blocks[index]->count)
    count_scale = (REG_BR_PROB_BASE * count_scale
		   / di->switch_blocks[index]->count);

  frequency_scale = 1;
  if (di->switch_blocks[index]->frequency)
    frequency_scale = (REG_BR_PROB_BASE * frequency_scale
		   / di->switch_blocks[index]->frequency);

  di->duplicate_blocks [0]->prev_bb->aux = di->switch_blocks [index];
  di->duplicate_blocks [di->n_duplicate_bb - 1]->next_bb->aux = 
    EDGE_SUCC (di->switch_blocks [index], 0)->dest;

  st = id->decl_map;
  id->decl_map = splay_tree_new (splay_tree_compare_pointers,
				 NULL, NULL);

  /* CHECK lang_hooks.tree_inlining.auto_var_in_fn_p */
  for (j = 0; j < di->n_duplicate_bb; j++)
    {
      if (dump_file)
	fprintf (dump_file, "copy bb %d\n", di->duplicate_blocks[j]->index);
      di->duplicate_blocks [j]->aux = 
	copy_bb (id, di->duplicate_blocks [j], frequency_scale, count_scale);
      if (dump_file)
	fprintf (dump_file, "duplicate bb %d\n", ((basic_block) (di->duplicate_blocks[j]->aux))->index);
    }

  for (j = 0; j < di->n_duplicate_bb; j++)
    {
      if (dump_file)
 	fprintf (dump_file, "edges bb %d\n", di->duplicate_blocks[j]->index);

      copy_edges_for_bb (di->duplicate_blocks [j], count_scale);
    }

  after_switch = EDGE_SUCC (di->switch_blocks [index], 0)->dest;
  redirect_edge_succ (EDGE_SUCC (di->switch_blocks [index], 0),
		      di->duplicate_blocks [0]->aux);
  /* EDGE_SUCC (after_switch, 0)->flags &= ~EDGE_FALLTHRU; CHECK */
  redirect_edge_succ (EDGE_SUCC (after_switch, 0), 
		      di->duplicate_blocks [di->n_duplicate_bb - 1]->next_bb);
  for (j = 0; j < di->n_duplicate_bb; j++)
    di->duplicate_blocks [j]->aux = NULL;
  di->duplicate_blocks [0]->prev_bb->aux = NULL;
  di->duplicate_blocks [di->n_duplicate_bb - 1]->next_bb->aux = NULL;

  splay_tree_delete (id->decl_map);
  id->decl_map = st;
}

static void
duplicate_code (duplicate_info *di, inline_data *id, struct function *cfun)
{
  int i;

#if 1
  for (i = 0; i < di->n_switch_bb; i++)
    { 
      edge e;
      block_stmt_iterator bsi;

      bsi = bsi_last (di->switch_blocks[i]);

      /* enable after CHECK.
      tree goto_stmt;
      goto_stmt = bsi_stmt (bsi);
      if (TREE_CODE (goto_stmt) == GOTO_EXPR)
	 return 0;;
	 bsi_prev (&bsi); */
      e = split_block (di->switch_blocks[i], bsi_stmt (bsi));
    }
#endif

  id->current_node = id->node = cgraph_node (current_function_decl);
  id->caller = id->callee = current_function_decl;
  id->callee_cfun = cfun;
  id->eh_region_offset = 0;
  id->eh_region = 0;
  id->versioning_p = 1;
  id->saving_p = 0;
  id->cloning_p = 0;
  id->ipa_info = 0;
  id->block = NULL;
  id->copybb_only_p = 1;
  for (i = 0; i < di->n_switch_bb; i++)
    duplicate_code_path (di, i, id);
}

static void
callsite_versioning (struct function *cfun)
{
  basic_block bb;
  loop_info li;
  duplicate_info di;
  int found, check;
  inline_data id;
  tree call_insn;  


  /*  printf ("enter callsite versioning \n"); */
  FOR_EACH_BB (bb)   
    {
      li.preheader = NULL;
      li.loop_begin = NULL;
      li.loop_end = NULL;
      li.postheader = NULL;
      found = check_single_loop (bb, &li) ;
      if (!found)
	continue;
      if (dump_file)
	fprintf (dump_file, "***loop info*** \n");

      di.n_switch_bb = 0;
      di.n_duplicate_bb = 0;
      di.merge_block = NULL;
      check = find_switch_blocks (&li, &di);
      if (!check)
	continue;
      if (dump_file)
	fprintf (dump_file, "***duplicate info*** \n");

      call_insn =  loop_find_call (&li) ; 
      if (call_insn == NULL_TREE ) continue;
      else
          if(has_defs (call_insn, di.switch_blocks,  di.n_switch_bb))
          {
	    /* fprintf (dump_file, "Ura! Check passed \n");  */
	    memset (&id, 0, sizeof (id));
	    duplicate_code (&di, &id, cfun);
	    if (dump_file)
	      fprintf (dump_file, "***success*** \n");
          }
          /*else fprintf (dump_file, "Noooo! \n"); */

/*      int_args = call_find_args (call_insn); */
      /* if (has_defs (di.switch_blocks, int_args)) */
    }
/*  printf ("exit callsite versioning \n"); */
}

void
cv_driver (void)
{
  struct cgraph_node *node;
  
  /*  printf("enter cv \n"); */
  for (node = cgraph_nodes; node; node = node->next)
    {
      tree temp_fn;

      if (DECL_SAVED_TREE (node->decl) == NULL)
	continue;

      if (!node->lowered)
	continue;

      temp_fn = current_function_decl;
      current_function_decl = node->decl;
      push_cfun (DECL_STRUCT_FUNCTION (node->decl));
      /*      bitmap_obstack_initialize (NULL); */
      tree_register_cfg_hooks ();
      callsite_versioning (DECL_STRUCT_FUNCTION (node->decl));
      free_dominance_info (CDI_DOMINATORS);
      free_dominance_info (CDI_POST_DOMINATORS);
      pop_cfun ();
      current_function_decl = temp_fn;
    }
  /*  printf("exit cv \n"); */
}

/* Gate for the call versionin ogptimization.  */
static bool
cgraph_gate_cv (void)
{
  return flag_cv;
}

struct tree_opt_pass pass_cv = {
  "cv",				/* name */
  cgraph_gate_cv,		/* gate */
  cv_driver,			/* execute */
  NULL,				/* sub */
  NULL,				/* next */
  0,				/* static_pass_number */
  TV_CV,        		/* tv_id */
  0,				/* properties_required */
  PROP_trees,			/* properties_provided */
  0,				/* properties_destroyed */
  0,				/* todo_flags_start */
  TODO_dump_func,	        /* todo_flags_finish */
  0				/* letter */
};
