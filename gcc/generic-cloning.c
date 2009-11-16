/* Generic function cloning.
     Copyright (C) 2009 by Cupertino Miranda, Shuangde Huang, Liang Peng 
     INRIA Futurs, France; ICT, China;
   
This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.
   
You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "hard-reg-set.h"
#include "basic-block.h"
#include "output.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-dump.h"
#include "tree-pass.h"
#include "timevar.h"
#include "cfgloop.h"
#include "flags.h"
#include "tree-inline.h"
#include "cgraph.h"
#include "ipa-prop.h"
#include "opts.h"
#include "tree-iterator.h"
#include "gimple.h"
#include "toplev.h"
#include "highlev-plugin-internal.h"

/* Info for generic cloning.  */
cloneinfo cdi;

static inline bool
is_it_main (struct cgraph_node *cg_func)
{
  if (!strcmp (IDENTIFIER_POINTER (DECL_NAME (cg_func->decl)), "MAIN__"))
    return true;
  if (!strcmp (IDENTIFIER_POINTER (DECL_NAME (cg_func->decl)), "main"))
    return true;

  return false;
}

static bool
has_variable_arguments(tree decl)
{
   return DECL_STRUCT_FUNCTION(decl)->stdarg == 1;
}

static bool
is_it_clonable (struct cgraph_node *cg_func)
{
  tree decl = cg_func->decl;

  /* TODO  Are this enough */
  if (cg_func->global.inlined_to)
    return false;

  if (is_it_main (cg_func))
    return false;

  if (cgraph_function_body_availability (cg_func) == AVAIL_NOT_AVAILABLE)
    return false;

  if (flags_from_decl_or_type (decl) & ECF_NORETURN)
    return false;

  /* ??? Mismatch between the returned type and the result declaration.  */
  if (TREE_TYPE (TREE_TYPE (decl)) != TREE_TYPE (DECL_RESULT (decl)))
    return false;

  /* it has variable number of arguments */
  if (has_variable_arguments(decl))
    return false;
    
  return true;
}

static void
freecloneinfo (cloneinfo cdi)
{
  /* free clone info.  */ 
  int i,j;
  int num = 3;
  free (cdi.external_libraries);
  for(i = 0, num = 0; i<cdi.numofclonefun; num += *(cdi.clones + i), i++)
    {               
      free (*(cdi.clone_function_list + i));
      free (*(cdi.function_filename_list + i));                          
      free (*(cdi.clone_extension + i));
      free (*(cdi.adaptation_function + i));
      for(j=0; j<*(cdi.clones + i); j++)
	free (*(cdi.clone_option + num + j));
    }
  free (cdi.clone_function_list);
  free (cdi.function_filename_list);
  free (cdi.clone_extension);
  free (cdi.adaptation_function);
  free (cdi.clones);
  free (cdi.clone_option);
}

static bool 
is_in_clone_list (const char *func_name, const char *file_name, 
		  int *nid)
{
  int i = 0;
  for(i = 0; i < cdi.numofclonefun; i++)
    {
      if (strcmp (func_name, *(cdi.clone_function_list + i)) == 0
	  && strcmp (file_name, *(cdi.function_filename_list + i)) == 0) 
	{
	  *nid = i;
	  return true;
	}
    }
  return false;
}

static void 
set_function_invalid (const char *func_name)
{
  int i = 0;
  for(i = 0; i < cdi.numofclonefun; i++)
    {
      if (strcmp (func_name, *(cdi.clone_function_list + i)) == 0) 
	{
          strcpy (*(cdi.clone_function_list + i), "");
          return;
        }
    }
}

static bool
find_clone_options (char *funcname, int *nid)
{
  int i, j, cid;
  char *clone_name = NULL;
  cid = -1;
  for(i = 0; i < cdi.numofclonefun; i++)
    {
      for(j = 0; j < cdi.clones[i]; j++)
	{
	  cid ++;
	  clone_name = (char *) alloca 
	    (strlen (*(cdi.clone_function_list + i)) 
	     + strlen (*(cdi.clone_extension + i))
	     + 64);
	  sprintf (clone_name, "%s%s_%d",
		   *(cdi.clone_function_list + i),
		   *(cdi.clone_extension + i), j);
          if (strcmp (funcname, clone_name) == 0) 
	    {
	      *nid = cid;
	      return true;
	    }
	}
    }
  return false;
}

/* Function for parsing adaptation arguments and desirable functions. */
static char **
parse_clone_option (char *text, unsigned int *argc)
{
  unsigned int i, size;
  char **argv;
  char *start;

  *argc = 0;
  if (text[0] != 0)
    *argc = 2;

  size = strlen (text);
  for (i = 0; i < size; i++)
    {
      if (text[i] == ' ')
	{
	  text[i] = '\0';
	  (*argc)++;
	}
    }

  argv = (char **) xmalloc (sizeof (char *) * (*argc));

  start = text;
  for (i = 1; i < (*argc); i++)
    {
      argv[i] = start;
      start += (strlen (start) + 1);
    }

  return argv;
}


extern tree mf_mark (tree);
static tree
get_arguments (tree tree_list)
{
  tree args = NULL_TREE;

  while (tree_list)
    {
      args = tree_cons (NULL_TREE, tree_list, args);
      tree_list = TREE_CHAIN (tree_list);
    }

  return nreverse (args);
}

static void
add_call_to_clones (struct cgraph_node *orig, int nid)
{
  tree decl, ftype, args, select_func, select_call, clone_call, index;
  tree body_label, call_label, return_var;
  char *select_name, *clone_fname;
  VEC (tree,heap) *labels;
  basic_block first_bb, entry, call_block, body_block;
  gimple gp_switch_expr, gp_clone_expr, gp_return_expr, 
    gp_select_expr, gp_body_label_expr, gp_call_label_expr;
  struct function *func;
  gimple_stmt_iterator gsi;
  struct cgraph_node *clone = NULL;
  struct cgraph_node *curr_node = NULL;
  struct cgraph_node *select_node;
  struct cgraph_edge *cge;
  edge body_edge, e;
  edge_iterator ei;
  int i;
  bool removed;
  location_t location;

  /* decl of orig function node.  */
  decl = orig->decl;
  location = DECL_SOURCE_LOCATION(decl);

  /* Get arguments of original function.  */
  func = DECL_STRUCT_FUNCTION (decl);
  args = get_arguments (DECL_ARGUMENTS (decl));

  first_bb = ENTRY_BLOCK_PTR_FOR_FUNCTION (func)->next_bb;
  gsi = gsi_start_bb (first_bb);
  entry = first_bb;
	
  /* build call to adaptation_function.  */
  ftype = build_function_type (unsigned_type_node, void_list_node);
  select_name = (char *) alloca 
    (strlen((IDENTIFIER_POINTER (DECL_NAME (decl))))
     + 64);
  if ((*(cdi.adaptation_function + nid))[0] == '_')
    sprintf (select_name, "%s%s",IDENTIFIER_POINTER (DECL_NAME (decl)),
             *(cdi.adaptation_function + nid));
  else
    sprintf (select_name, "%s", *(cdi.adaptation_function + nid));

  select_func = build_fn_decl (select_name, ftype);
  DECL_SOURCE_LOCATION(select_func) = location;
  select_call = build_function_call_expr (select_func, NULL_TREE);
  gp_select_expr = gimple_build_call_from_tree (select_call);
  gimple_set_location (gp_select_expr, location);

  /* switch index.  */
  index = create_tmp_var (unsigned_type_node, "index");
  gimple_call_set_lhs (gp_select_expr, index);

  DECL_EXTERNAL (select_func) = 1;
  TREE_PUBLIC (select_func) = 1;
  select_node = cgraph_node (select_func);
  cgraph_mark_needed_node (select_node);
  select_node->local.externally_visible = 1;
  select_node->local.local = 0;
  cgraph_create_edge (orig, select_node, gp_select_expr,
                      first_bb->count,
                      CGRAPH_FREQ_MAX, first_bb->loop_depth);
  cgraph_mark_needed_node (select_node);

  /* default label of switch.  */
  body_label = build3 (CASE_LABEL_EXPR, void_type_node, NULL_TREE,
		       NULL_TREE, create_artificial_label ());
  gp_body_label_expr = gimple_build_label(CASE_LABEL(body_label));
  gimple_set_location (gp_body_label_expr, location);

  /* switch label vector : labels.  */
  labels = VEC_alloc (tree, heap, cdi.clones[nid]);

  gsi_insert_before (&gsi, gp_select_expr, GSI_SAME_STMT);
  body_edge = split_block (entry, gp_select_expr);
  body_block = body_edge->dest;
  first_bb = body_edge->src;
  entry->flags |= BB_REACHABLE;
  body_block->flags |= BB_REACHABLE;
  body_edge->flags |= EDGE_EXECUTABLE;
  body_edge->flags &= ~EDGE_FALLTHRU;

  /* For each clone.  */
  for(i = 0; i < cdi.clones[nid]; i++)
    {
      clone = NULL;
      /*get the clone node from function name.  */
      clone_fname = (char *) alloca 
	(strlen (IDENTIFIER_POINTER (DECL_NAME (decl)))
	 + strlen (*(cdi.clone_extension + nid))
	 + 64);
      sprintf (clone_fname, "%s%s_%d",
	       IDENTIFIER_POINTER (DECL_NAME (decl)), 
	       *(cdi.clone_extension + nid), i);

      for(curr_node = cgraph_nodes; curr_node; 
	  curr_node = curr_node->next)
	{
	  if (strcmp (IDENTIFIER_POINTER (DECL_NAME (curr_node->decl)),
		      clone_fname) == 0)
	    {
	      clone = curr_node;
	      break;
	    }
	}

      if (clone == NULL)
	break;

      call_block = create_basic_block (NULL, NULL, first_bb);

      /* Set gsi to current block.  */
      gsi = gsi_start_bb (call_block);

      /* build clone call expr, and return expr.  */
      clone_call = build_function_call_expr (clone->decl, args);
      gp_clone_expr = gimple_build_call_from_tree (clone_call);
      gimple_set_location (gp_clone_expr, location);

      if(TREE_TYPE (TREE_TYPE (decl)) != void_type_node)
	{
	  if (aggregate_value_p (DECL_RESULT(decl), decl)
	      && !is_gimple_reg_type (TREE_TYPE (DECL_RESULT (decl)))
	      && !DECL_NAME (DECL_RESULT(decl)))
	    return_var = DECL_RESULT (decl);
	  else
	    return_var = 
	      create_tmp_var (TREE_TYPE (DECL_RESULT (decl)), "ret");
	  DECL_CONTEXT (return_var) = decl;
	  gimple_call_set_lhs (gp_clone_expr, return_var);
	  gp_return_expr = gimple_build_return(return_var);
	}
      else
	{
	  gp_return_expr = gimple_build_return(NULL);
	}
	  
	  gimple_set_location (gp_return_expr, location);

      /* add switch label, 
	 clone expr and return expr to current block.  */
      call_label = build3 (CASE_LABEL_EXPR, void_type_node, 
			   build_int_cst (integer_type_node, i+1), 
			   NULL_TREE, create_artificial_label ());
      gp_call_label_expr = gimple_build_label(CASE_LABEL(call_label));
      gimple_set_location (gp_call_label_expr, location);

      gsi_insert_before (&gsi, gp_call_label_expr, GSI_SAME_STMT);
      gsi_insert_before (&gsi, gp_clone_expr, GSI_SAME_STMT);
      /*
      SET_EXPR_LOCUS (clone_call, gimple_location_ptr (first_stmt (first_bb)));
      TREE_BLOCK (clone_call) = TREE_BLOCK (select_call);  */

      gsi_insert_before (&gsi, gp_return_expr, GSI_SAME_STMT);

      VEC_safe_insert (tree, heap, labels, i, call_label);

      /* CGRAPH corrections.  */

      cge = cgraph_create_edge (orig, cgraph_node (clone->decl), 
				gp_clone_expr,
                                first_bb->count, CGRAPH_FREQ_MAX, 
				first_bb->loop_depth);
      cge->inline_failed = "Clonned function";
      cgraph_mark_needed_node (cgraph_node (clone->decl));

      /* CFG corrections.  */
      call_block->flags |= BB_REACHABLE;
      call_block->flags |= BB_DISABLE_SCHEDULE;
      make_edge (entry, call_block, EDGE_EXECUTABLE);
      make_edge (call_block, EXIT_BLOCK_PTR, 0);
      update_stmt (gp_call_label_expr);
      update_stmt (gp_clone_expr);
      update_stmt (gp_return_expr);
    }

  entry->flags |= BB_DISABLE_SCHEDULE;
  body_block->flags |= BB_DISABLE_SCHEDULE;

  /* insert default label.  */
  gsi = gsi_start_bb (body_block);
  gsi_insert_before (&gsi, gp_body_label_expr, GSI_SAME_STMT);

  CASE_LOW (body_label) = NULL;
  if (!VEC_empty (tree, labels))
    sort_case_labels (labels);

  gp_switch_expr = gimple_build_switch_vec (index, body_label, labels);
  gimple_set_location (gp_switch_expr, location);
  gsi = gsi_start_bb (first_bb);
  gsi_insert_after (&gsi, gp_switch_expr, GSI_SAME_STMT);

  /* redirect incomming edges from entry to body_block.  */
  removed = true;
  while (removed)
    {
      removed = false;
      FOR_EACH_EDGE (e, ei, entry->preds)
        {
          if (e->src != ENTRY_BLOCK_PTR_FOR_FUNCTION (func))
            {
              make_edge (e->src, body_block, e->flags);
              remove_edge (e);
              removed = true;
              break;
            }
        }
    }
  update_stmt (gp_select_expr);
  update_stmt (gp_body_label_expr);
  update_stmt (gp_switch_expr);
}

static unsigned int
exec_clone_functions (void)
{
  struct cgraph_node *node;
  struct function *saved_cfun, *func;
  tree fundecl;
  unsigned int argc;
  char **argv;
  int nid = -1, optid = -1;
  int i;
  int saved_flag_strict_aliasing, saved_flag_pcc_struct_return, 
    saved_flag_omit_frame_pointer, saved_flag_asynchronous_unwind_tables;
  char *output_name = NULL;

  saved_cfun = cfun;
  cdi.numofclonefun = 0;

  register_event_parameter("clone_info", &cdi, EP_VOID);
  call_plugin_event("load_clone_config");
  unregister_event_parameter("clone_info");

  if (cdi.numofclonefun == 0)
    {
      fprintf (stderr, "No infomation for function cloning pass\n");
      return 0;
    }

  /* Perform cloning to functions */
  for (node = cgraph_nodes; node; node = node->next)
    {
      const char *node_name = NULL;
      const char *file_name = NULL;
      func = DECL_STRUCT_FUNCTION (node->decl);
      set_cfun(func);
      node_name = IDENTIFIER_POINTER (DECL_NAME (node->decl));
      file_name = expand_location(DECL_SOURCE_LOCATION (node->decl)).file;
      /*is cfun in the clone_list*/
      if(is_in_clone_list(node_name, file_name, &nid)) 
	{
      	  if (is_it_clonable (node))
	    {
	      struct cgraph_node *clone_node = NULL;
	      if (getenv ("ICI_VERBOSE") != NULL)
                printf("Now clone function: %s \n", node_name);
	      /* Do not clone virtual clone function.  */
	      for(i = 0; i < cdi.numofclonefun; i++)
		{
	          if (!strcmp (node_name + (strlen (node_name) - 
			       strlen (*(cdi.clone_extension + i))),
			       *(cdi.clone_extension + i)))
	            {
	              printf ("%s is a virtual clone function\n", 
			      node_name);
		      break;
	            }
                }
	      if( i < cdi.numofclonefun)
		continue;
		  
              DECL_UNINLINABLE(node->decl) = 1;
	      for(i = 0; i < cdi.clones[nid]; i++)
		{
	          set_cfun(DECL_STRUCT_FUNCTION (node->decl));

	          clone_node = generic_cloning_cgraph_function_versioning (node, 
							   NULL, 
							   NULL, 0);
			  
	          DECL_POSSIBLY_INLINED(clone_node->decl) = 0;

	          /* The cloned function should not be inlined.  */
	          clone_node->local.inlinable = false;
		  DECL_UNINLINABLE(clone_node->decl) = 1;

		  /* Generate a new name for the new version. */
		  output_name =
		    (char *) alloca (strlen 
				     (IDENTIFIER_POINTER 
				      (DECL_NAME (node->decl)))
				     + 64);
		  sprintf (output_name, "%s%s_%d",
			   IDENTIFIER_POINTER 
			   (DECL_NAME (node->decl)), 
			   *(cdi.clone_extension + nid), i);
		  
		  DECL_NAME (clone_node->decl) = 
		    get_identifier (output_name);
		  
		  /* Create a new SYMBOL_REF rtx for the new name. */
		  
		  if (DECL_RTL (node->decl) != NULL)
		    {
		      SET_DECL_RTL (clone_node->decl, 
				    copy_rtx (DECL_RTL 
					      (node->decl)));
		      XEXP (DECL_RTL (clone_node->decl), 0) = 
			gen_rtx_SYMBOL_REF( GET_MODE 
					    (XEXP (DECL_RTL 
						   (node->decl), 0)),
					    IDENTIFIER_POINTER 
					    (DECL_NAME
					     (clone_node->decl)));
		    }
		  		  
		  DECL_STRUCT_FUNCTION (clone_node->decl)
		    ->function_start_locus = DECL_STRUCT_FUNCTION 
		    (node->decl)->function_start_locus;
		  
		  fundecl = clone_node->decl;
		  if (TREE_CODE (fundecl) == FUNCTION_DECL 
		      && find_clone_options (output_name, &optid))
		    {
		      struct cl_optimization cur_opts;
		      tree old_opts = 
			DECL_FUNCTION_SPECIFIC_OPTIMIZATION (fundecl);
		      
		      /* Save current options.  */
		      cl_optimization_save (&cur_opts);
		      
		      /* If we previously had some optimization options, 
			 use them as the default.  */
		      if (old_opts)
			cl_optimization_restore (TREE_OPTIMIZATION (old_opts));
		      
		      /* Parse options, and update the vector.  */
		      argv = 
			parse_clone_option (*(cdi.clone_option + optid), &argc);

		      /* may need to preserve more in the future, 
			 reference OPTIMIZATION_OPTIONS and OVERRIDE_OPTIONS */	  
		      saved_flag_strict_aliasing = flag_strict_aliasing;
		      saved_flag_omit_frame_pointer = flag_omit_frame_pointer;
		      saved_flag_pcc_struct_return = flag_pcc_struct_return;
		      saved_flag_asynchronous_unwind_tables = 
			flag_asynchronous_unwind_tables;
		      
		      decode_options (argc, (const char **) argv);
		      
		      /* Don't allow changing following flags.  */
		      flag_omit_frame_pointer = saved_flag_omit_frame_pointer;
		      flag_asynchronous_unwind_tables = 
			saved_flag_asynchronous_unwind_tables;
		      flag_pcc_struct_return = saved_flag_pcc_struct_return;
		      flag_strict_aliasing = saved_flag_strict_aliasing;
		      
		      DECL_FUNCTION_SPECIFIC_OPTIMIZATION (fundecl)
			= build_optimization_node ();
		      
		      /* Restore current options.  */
		      cl_optimization_restore (&cur_opts);
		    }
		  
		}
	      add_call_to_clones (node, nid);
	    }
	  else 
	    {
	      printf("%s (in the clone list) can not be cloned!\n", 
		     node_name);
	      set_function_invalid(node_name);
	    }
	}
    }
  
  set_cfun(saved_cfun);
  freecloneinfo (cdi);
  return 0;
}

static bool
gate_clone_functions(void)
{
  return flag_api_clone_functions;
}

struct gimple_opt_pass pass_clone_functions = {
 {
  SIMPLE_IPA_PASS  ,
  "generic_cloning",		/* name */
  gate_clone_functions,		/* gate */
  exec_clone_functions,		/* execute */
  NULL,				/* sub */
  NULL,				/* next */
  0,				/* static_pass_number */
  0,				/* tv_id */
  0,				/* properties_required */
  0,				/* properties_provided */
  0,				/* properties_destroyed */
  0,				/* todo_flags_start */
  TODO_dump_cgraph | TODO_verify_flow,	/* todo_flags_finish */
 }
};

