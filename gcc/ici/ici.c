/* Callgraph based interprocedural optimizations.
   Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009
   Free Software Foundation, Inc.
   Contributed by Jan Hubicka

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "cgraph.h"
#include "function.h"
#include "tree-pass.h"
#include "opts.h"
#include "plugin.h" /* Need internal version for invoke_plugin_callbacks.  */
#include "highlev-plugin-internal.h"

void plugin_is_GPL_compatible (void);
static void ici_load_function_specific_optimizations (void);

void
plugin_is_GPL_compatible (void)
{
}

static int ici_all_ipa_passes = 1;
static int ici_bypass_gimple_in_ipa = 0;

/* Perform ICI internal tasks at the start of IPA passes.
   This must run before any ICI user registered callbacks, which is why
   register_plugin_event must unregister / re-register this callback when
   a ICI user callback for PLUGIN_ALL_IPA_PASSES_START is added.  */
static void
ici_all_ipa_passes_start (void *gcc_data ATTRIBUTE_UNUSED,
			  void *user_data ATTRIBUTE_UNUSED)
{
  ici_load_function_specific_optimizations ();
  register_event_parameter ("all_ipa_passes", &ici_all_ipa_passes, EP_SILENT);
  register_event_parameter ("bypass_gimple_in_ipa", &ici_bypass_gimple_in_ipa, 
			    EP_SILENT);
}

/* Execute ICI internal tasks at the end of IPA passes.
   This must run after all ICI user registered PLUGIN_ALL_IPA_PASSES_END
   callbacks, which we get automatically be registering this callback
   first.  */
static void
ici_all_ipa_passes_end (void *gcc_data ATTRIBUTE_UNUSED,
			void *user_data ATTRIBUTE_UNUSED)
{
  unregister_event_parameter ("bypass_gimple_in_ipa");
  unregister_event_parameter ("all_ipa_passes");
}

/* This one again should run before ICI user callbacks.  */
static void
ici_early_gimple_passes_start (void *gcc_data ATTRIBUTE_UNUSED,
			       void *user_data ATTRIBUTE_UNUSED)
{
  /* Switch to GIMPLE passes.  */
  ici_all_ipa_passes = 2;

  /* GIMPLE passes are recorded only once.  ici_bypass_gimple_in_ipa
     will be set again in ici_set_bypass_gimple_in_ipa.  */
  ici_bypass_gimple_in_ipa = 0;
}

/* Set bypass_gimple_in_ipa back to 1 to stop recording GIMPLE passes.  */
static void
ici_set_bypass_gimple_in_ipa (void *gcc_data ATTRIBUTE_UNUSED,
			      void *user_data ATTRIBUTE_UNUSED)
{
  ici_bypass_gimple_in_ipa = 1;
}

/* The low-level callback implementation calls the last registered
   callback first.  Make sure that, if appropriate, the ICI internal
   callback for EVENT is registered last, i.e. called first.
   If EVENT is -1, we are called from the ICI plugin_init, and are to
   initialize all permanent ICI internall callbacks.  */
void
ici_refresh_internal_callbacks (int event)
{
  if (event == PLUGIN_ALL_IPA_PASSES_START)
    unregister_callback ("ICI_INTERNAL", PLUGIN_ALL_IPA_PASSES_START);
  if (event == PLUGIN_ALL_IPA_PASSES_START || event < 0)
    register_callback ("ICI_INTERNAL", PLUGIN_ALL_IPA_PASSES_START,
		       ici_all_ipa_passes_start, NULL);
  if (event == PLUGIN_EARLY_GIMPLE_PASSES_START)
    unregister_callback ("ICI_INTERNAL", PLUGIN_EARLY_GIMPLE_PASSES_START);
  if (event == PLUGIN_EARLY_GIMPLE_PASSES_START || event < 0)
    register_callback ("ICI_INTERNAL", PLUGIN_EARLY_GIMPLE_PASSES_START,
		       ici_early_gimple_passes_start, NULL);
  if (event < 0)
    {
      register_callback ("ICI_INTERNAL", PLUGIN_ALL_IPA_PASSES_END,
			 ici_all_ipa_passes_end, NULL);
      register_callback ("ICI_INTERNAL", PLUGIN_PASS_EXECUTION,
			 ici_set_bypass_gimple_in_ipa, NULL);
    }
}

int
plugin_init (struct plugin_name_args *plugin_info ATTRIBUTE_UNUSED,
	     struct plugin_gcc_version *version ATTRIBUTE_UNUSED)
{
  static struct register_pass_info instrument_functions_info
    = { &pass_instrument_functions.pass, "early_local_cleanups",
	1, PASS_POS_INSERT_BEFORE
      };

  ici_refresh_internal_callbacks (-1);
  register_callback ("ICI_INTERNAL", PLUGIN_PASS_MANAGER_SETUP,
		     NULL, &instrument_functions_info);
  load_ici_plugin ();
  return 0; /* Success.  */
}

/* Load function specific optimizations with parameters.  */

/* separate string into arguments.  */
static void
ici_separate_arguments (const char *string, unsigned int *argc, char ***argv)
{
  const char *p;
  char c;
  char **args;
  unsigned int len; 
  int i;

  /* Count number of args  */
  p = string;
  c = *p;
  *argc = 1; 
  while (c)
    {    
      if ((c == ' ' || c == '\t') && len) 
        {    
          len = 0; 
          ++*argc;
        }    
      else 
        len = 1; 
      c = *++p;
    }    
  if (len)
    ++*argc;

  args = (char **) xmalloc (sizeof(char *) * (*argc));
  *argv = args;
  args[0] = (char*)xmalloc(sizeof(char)); /* argv[0] unavailable  */
  args[0][0]='\0';
  i = 1; 
  p = string;
  c = *p;
  len = 0; 
  while (c)
    {
      if (c == ' ' || c == '\t')
        {
        if (len)
          {           
           *(args + i) = (char *) xmalloc (sizeof(char) * (len + 1));
            strncpy (*(args + i), (p - len), len);
	     args[i][len] = '\0';
            ++i;
            len = 0;
          }
        }
      else
        ++len;
      c = *++p;
    }
  if (len)
     {
      *(args + i) = (char *) xmalloc (sizeof(char) * (len + 1));
      strncpy (*(args + i), (p - len), len);
	args[i][len] = '\0';
     }
}

/* free arguments string.  */
static void 
ici_free_arguments (unsigned int argc, char **argv)
{
  unsigned int i;
  for (i = 0; i < argc; ++i) 
    {    
      free (argv[i]); 
    }    
  free (argv);
 
}


/* load function specific option strings and save to function structures.  */
static void 
ici_load_function_specific_optimizations (void)
{
  static char *ici_function_spec_string;
  struct cgraph_node *node;
  struct function *old_cfun = cfun; /* Backup cfun.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      struct function *fun;
      tree fun_decl;

      fun = DECL_STRUCT_FUNCTION (node->decl);
      if (!fun)
        continue;
      set_cfun (fun);

      ici_function_spec_string = NULL;
      invoke_named_callbacks ("function_spec_loader", "function_spec_string",
			      EP_VOID, (void *) &ici_function_spec_string,
			      NULL);
      if (!ici_function_spec_string)
        continue;

      fun_decl = fun->decl;
      if (TREE_CODE (fun_decl) == FUNCTION_DECL)
        {
          unsigned int argc;
          char **argv;
          struct cl_optimization old_global_opts;
          tree old_function_opts;
	   int saved_flag_strict_aliasing;
	   int saved_flag_pcc_struct_return, 
	        saved_flag_omit_frame_pointer,
		 saved_flag_asynchronous_unwind_tables;

          /* Save old global and function-specific optimizations.  */
          cl_optimization_save (&old_global_opts);

          /* Store function-specific optimizations to global.  */
          old_function_opts
            =  DECL_FUNCTION_SPECIFIC_OPTIMIZATION (fun_decl);
          if (old_function_opts)
            cl_optimization_restore (TREE_OPTIMIZATION (old_function_opts));

          /* Parse options string.  */
          ici_separate_arguments (ici_function_spec_string, &argc, &argv);

          /* Save flags which should not be changed.  */

          /* Change global optimizations with loaded 
             function specific string  */
           saved_flag_strict_aliasing = flag_strict_aliasing;
	    saved_flag_omit_frame_pointer = flag_omit_frame_pointer;
	    saved_flag_pcc_struct_return = flag_pcc_struct_return;
	    saved_flag_asynchronous_unwind_tables = flag_asynchronous_unwind_tables;
           decode_options (argc, (const char **) argv);
		   
	    flag_strict_aliasing = saved_flag_strict_aliasing;
           flag_omit_frame_pointer = saved_flag_omit_frame_pointer;
   	    flag_asynchronous_unwind_tables = saved_flag_asynchronous_unwind_tables;
     	    flag_pcc_struct_return = saved_flag_pcc_struct_return;
			
           ici_free_arguments (argc, argv);
	    argv = NULL;

          /* Restore saved flags.  */

          /* Store global optimizations to function.  */
          DECL_FUNCTION_SPECIFIC_OPTIMIZATION (fun_decl)
            = build_optimization_node ();

          /* Restore old global optmizations.  */
          cl_optimization_restore (&old_global_opts);
        }

      free (ici_function_spec_string);
    }
  /* Restore cfun  */
  set_cfun (old_cfun);
}

/* ICI: Execute one IPA summary pass  */
void
execute_one_ipa_summary_pass (struct opt_pass *pass)
{
  gcc_assert (!current_function_decl);
  gcc_assert (!cfun);
  gcc_assert (pass->type == SIMPLE_IPA_PASS || pass->type == IPA_PASS);
  if (!quiet_flag && !cfun)
    fprintf (stderr, " <summary generate>");

  /* Generate summary as execute_ipa_summary_passes,
     but only for current pass.  */
  if (pass->type == IPA_PASS
      && (!pass->gate || pass->gate ()))
    {
      pass_init_dump_file (pass);
      ((struct ipa_opt_pass_d *)pass)->generate_summary ();
      pass_fini_dump_file (pass);
    }
}

int
invoke_named_callbacks (const char *name, ...)
{
  va_list va;
  int retval;
  int event = get_named_event_id (name, NO_INSERT);
  va_start (va, name);
  retval = invoke_plugin_callbacks (event, &va);
  va_end (va);
  return retval;
}
