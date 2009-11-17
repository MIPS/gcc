/* Function Instrumentation.
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

/* Info for function instrumentation.  */
instrinfo idi;

static bool
is_it_instrumentable (struct cgraph_node *cg_func)
{
  if (cgraph_function_body_availability (cg_func) == AVAIL_NOT_AVAILABLE)
    return false;
  else 
    return true;
}

static void
freeinstrinfo (instrinfo idi)
{
  /* free instrumentation info.  */
  int i;
  for(i=0; i<idi.numofinstrfun; i++)
    {
      free (*(idi.instrument_function_list + i));
      free (*(idi.function_filename_list + i));
      free (*(idi.timer1 + i));
      free (*(idi.timer2 + i));
    }
  free (idi.instrument_function_list);
  free (idi.function_filename_list);
  free (idi.timer1);
  free (idi.timer2);
}

static bool 
is_in_instrument_list (const char *func_name, const char *file_name,
		       int *nid)
{
  int i = 0;
  for(i = 0; i < idi.numofinstrfun; i++)
    {
      if (strcmp (func_name, *(idi.instrument_function_list + i)) == 0
	  && strcmp (file_name, *(idi.function_filename_list + i)) == 0) 
	{
	  *nid = i;
	  return true;
	}
    }
  return false;
}

static void
add_timer_begin (struct cgraph_node *cg_func, char *funname, int cloned)
{
  gimple_stmt_iterator gsi;
  struct function * func = NULL;
  gimple_seq gp_seq = NULL;
  gimple stmt = NULL;
  basic_block bb = NULL;
  location_t location;

  tree decl = cg_func->decl;
  tree ftype = build_function_type (void_type_node, void_list_node);
  tree func_decl = build_fn_decl (funname, ftype);
  tree call_expr
    = build_function_call_expr (UNKNOWN_LOCATION, func_decl, NULL_TREE);
  location = DECL_SOURCE_LOCATION(decl);
  DECL_SOURCE_LOCATION(func_decl) = location;
  
  /*convert the call_expr (tree) to gimple_seq(gp_seq)*/
  gimplify_stmt(&call_expr, &gp_seq);

  func = DECL_STRUCT_FUNCTION (decl);
  bb = ENTRY_BLOCK_PTR_FOR_FUNCTION (func)->next_bb;
  gsi = gsi_start_bb(bb);
  if(cloned == 1)
    gsi_insert_seq_after (&gsi, gp_seq, GSI_NEW_STMT);
  else
    gsi_insert_seq_before (&gsi, gp_seq, GSI_NEW_STMT);
  stmt = gsi_stmt (gsi);
  gimple_set_location (stmt, location);

  cgraph_create_edge (cg_func, cgraph_node (func_decl),
		      stmt,
		      bb->count, CGRAPH_FREQ_MAX, bb->loop_depth);

}

static void
add_timer_end (struct cgraph_node *cg_func, char *funname)
{
  edge e;
  edge_iterator ei;
  gimple_stmt_iterator gsi;
  struct function *func = NULL;
  gimple stmt = NULL;
  gimple_seq gp_seq = NULL;
  tree decl, ftype, func_decl, call_expr;
  location_t location;

  decl = cg_func->decl;
  ftype = build_function_type (void_type_node, void_list_node);
  func_decl = build_fn_decl (funname, ftype);

  func = DECL_STRUCT_FUNCTION (decl);
  location = func->function_end_locus;
  DECL_SOURCE_LOCATION(func_decl) = location;

  FOR_EACH_EDGE (e, ei, EXIT_BLOCK_PTR_FOR_FUNCTION (func)->preds)
    {
      gsi = gsi_last_bb (e->src);

      call_expr
	= build_function_call_expr (UNKNOWN_LOCATION, func_decl, NULL_TREE);
      gimplify_stmt(&call_expr, &gp_seq);

      if (gimple_code(gsi_stmt (gsi)) == GIMPLE_RETURN)
	gsi_insert_seq_before (&gsi, gp_seq, GSI_NEW_STMT);
      else
	gsi_insert_seq_after (&gsi, gp_seq, GSI_NEW_STMT);

      stmt = gsi_stmt (gsi);
      gimple_set_location (stmt, location);
      gp_seq = NULL;

      cgraph_create_edge (cg_func, cgraph_node (func_decl), 
			  stmt,
			  e->src->count, CGRAPH_FREQ_MAX,
			  e->src->loop_depth);
    }
}

static unsigned int
exec_instrument_functions (void)
{
  struct cgraph_node *node;
  struct function *saved_cfun, *func;
  int nid = -1;
  char *beginname = NULL, *endname = NULL;
  saved_cfun = cfun;
  idi.numofinstrfun = 0;

  invoke_named_callbacks ("load_instr_config",
			  "instr_info", EP_VOID, &idi, NULL);

  if (idi.numofinstrfun == 0)
    {
      fprintf (stderr, "No infomation for instrumentation pass\n");
      return 0;
    }

  /* Perform instrument to functions.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      const char *node_name = NULL;
      const char *file_name = NULL;
      func = DECL_STRUCT_FUNCTION (node->decl);
      set_cfun (func);
      node_name = IDENTIFIER_POINTER (DECL_NAME (node->decl));
      file_name = expand_location(DECL_SOURCE_LOCATION (node->decl)).file;

      if (is_in_instrument_list (node_name, file_name, &nid) 
	  && is_it_instrumentable(node))
        {
	  int flag_cloned = 0;
	  if (*(idi.cloned + nid) == '1')
	    flag_cloned = 1;
	  if (getenv ("ICI_VERBOSE") != NULL)
            printf("Now instrument function: %s \n", node_name);
	  beginname = (char *) alloca (strlen (node_name) + 
		 		       strlen (*(idi.timer1 + nid)));
	  if ((*(idi.timer1 + nid))[0] == '_')
	    sprintf (beginname, "%s%s", node_name, *(idi.timer1 + nid));
	  else
	    sprintf (beginname, "%s", *(idi.timer1 + nid));

          add_timer_begin (node, beginname, flag_cloned);

	  endname = (char *) alloca (strlen (node_name) + 
		 		     strlen (*(idi.timer2 + nid)));
          if ((*(idi.timer2 + nid))[0] == '_')
            sprintf (endname, "%s%s", node_name, *(idi.timer2 + nid));
          else
            sprintf (endname, "%s", *(idi.timer2 + nid));

          add_timer_end (node, endname);
        }
    }

  set_cfun(saved_cfun);
  freeinstrinfo (idi);
  return 0;
}

static bool
gate_instrument_functions (void)
{
  return flag_api_instrument_functions;
}

struct gimple_opt_pass pass_instrument_functions = {
 {
  SIMPLE_IPA_PASS  ,
  "instrumentation",	/* name */
  gate_instrument_functions,	/* gate */
  exec_instrument_functions,	/* execute */
  NULL,				/* sub */
  NULL,				/* next */
  0,				/* static_pass_number */
  TV_NONE,			/* tv_id */
  0,				/* properties_required */
  0,				/* properties_provided */
  0,				/* properties_destroyed */
  0,				/* todo_flags_start */
  TODO_dump_cgraph | TODO_verify_flow,	/* todo_flags_finish */
 }
};
