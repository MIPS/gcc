/* Pluggable tree transforms
   Copyright 2005 Free Software Foundation, Inc.
   Contributed by Sean Callanan <sean@fsl.cs.sunysb.edu>

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
#include "toplev.h"
#include "tree.h"
#include "rtl.h"
#include "expr.h"
#include "flags.h"
#include "params.h"
#include "input.h"
#include "insn-config.h"
#include "integrate.h"
#include "varray.h"
#include "hashtab.h"
#include "pointer-set.h"
#include "splay-tree.h"
#include "langhooks.h"
#include "cgraph.h"
#include "intl.h"
#include "function.h"
#include "diagnostic.h"
#include "debug.h"

#include "timevar.h"

#include "tree-inline.h"
#include "tree-mudflap.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "tree-plugin.h"

#include <assert.h>
#include <inttypes.h>
#include <ltdl.h>

struct tree_opt_pass pass_plugin_gimple =
{
  /* const char*		name				*/
  "plugin",
  /* bool			(*gate)			(void)	*/
  NULL,
  /* unsigned int		(*execute)		(void)	*/
  plugins_transform_gimple,
  /* struct tree_opt_pass*	sub				*/
  NULL,
  /* struct tree_opt_pass*	next				*/
  NULL,
  /* int			static_pass_number		*/
  0,
  /* unsigned int		tv_id				*/
  TV_TREE_PLUGIN,
  /* unsigned int		properties_required		*/
  PROP_gimple_any,
  /* unsigned int		properties_provided		*/
  0,
  /* unsigned int		properties_destroyed		*/
  0,
  /* unsigned int		todo_flags_start		*/
  0,
  /* unsigned int		todo_flags_finish		*/
  TODO_cleanup_cfg | TODO_update_ssa,
  /* char			letter				*/
  '\0'
};

struct tree_opt_pass pass_plugin_ipa =
{
  /* const char*		name				*/
  "plugin",
  /* bool			(*gate)			(void)	*/
  NULL,
  /* unsigned int		(*execute)		(void)	*/
  plugins_transform_cgraph,
  /* struct tree_opt_pass*	sub				*/
  NULL,
  /* struct tree_opt_pass*	next				*/
  NULL,
  /* int			static_pass_number		*/
  0,
  /* unsigned int		tv_id				*/
  TV_TREE_PLUGIN,
  /* unsigned int		properties_required		*/
  PROP_gimple_any,
  /* unsigned int		properties_provided		*/
  0,
  /* unsigned int		properties_destroyed		*/
  0,
  /* unsigned int		todo_flags_start		*/
  0,
  /* unsigned int		todo_flags_finish		*/
  TODO_cleanup_cfg | TODO_update_ssa,
  /* char			letter				*/
  '\0'
};

struct tree_opt_pass pass_plugin_rtl =
{
  /* const char*		name				*/
  "plugin",
  /* bool			(*gate)			(void)	*/
  NULL,
  /* unsigned int		(*execute)		(void)	*/
  plugins_transform_rtl,
  /* struct tree_opt_pass*	sub				*/
  NULL,
  /* struct tree_opt_pass*	next				*/
  NULL,
  /* int			static_pass_number		*/
  0,
  /* unsigned int		tv_id				*/
  TV_TREE_PLUGIN,
  /* unsigned int		properties_required		*/
  PROP_rtl,
  /* unsigned int		properties_provided		*/
  0,
  /* unsigned int		properties_destroyed		*/
  0,
  /* unsigned int		todo_flags_start		*/
  0,
  /* unsigned int		todo_flags_finish		*/
  0,
  /* char			letter				*/
  '\0'

};
#define MAX_PLUGINS 32

typedef struct plugin_handle GTY(())
  {
    void (*pre_translation_unit)	(int argc, struct plugin_argument* argv);
    void (*transform_ctrees)		(int argc, struct plugin_argument* argv, tree fndecl);
    void (*transform_gimple)		(int argc, struct plugin_argument* argv);
    void (*transform_cgraph)		(int argc, struct plugin_argument* argv);
    void (*transform_rtl)		(int argc, struct plugin_argument* argv);
    void (*post_translation_unit)	(int argc, struct plugin_argument* argv);

    char* spec;
    char* path;

    int argc;
    struct plugin_argument* argv;
  } handle;

DEF_VEC_O(handle);
DEF_VEC_ALLOC_O(handle, heap);

VEC(handle, heap)* handles = NULL;

#define RUN_ALL_FUNCS(function_name)						\
  do										\
    {										\
      if(handles)								\
        {									\
          unsigned int handle_index;						\
          handle* handle_pointer;						\
										\
          for(handle_index = 0;							\
              VEC_iterate(handle, handles, handle_index, handle_pointer);	\
              handle_index++)							\
            {									\
              if(handle_pointer->function_name)					\
                {								\
                  handle_pointer->function_name(handle_pointer->argc,		\
                                                handle_pointer->argv);		\
                }								\
            }									\
        }									\
    }										\
  while(0)

#define RUN_ALL_FUNCS_PARAMS(function_name, ...)				\
  do										\
    {										\
      if(handles)								\
        {									\
          unsigned int handle_index;						\
          handle* handle_pointer;						\
										\
          for(handle_index = 0;							\
              VEC_iterate(handle, handles, handle_index, handle_pointer);	\
              handle_index++)							\
            {									\
              if(handle_pointer->function_name)					\
                {								\
                  handle_pointer->function_name(handle_pointer->argc,		\
                                                handle_pointer->argv,		\
                                                __VA_ARGS__);			\
                }								\
            }									\
        }									\
    }										\
  while(0)

void plugins_pre_translation_unit(void)
{
  RUN_ALL_FUNCS(pre_translation_unit);
}

void plugins_transform_ctrees(tree fndecl)
{
  RUN_ALL_FUNCS_PARAMS(transform_ctrees, fndecl);
}

unsigned int plugins_transform_gimple(void)
{
  RUN_ALL_FUNCS(transform_gimple);
  return 0;
}

unsigned int plugins_transform_cgraph(void)
{
  RUN_ALL_FUNCS(transform_cgraph);
  return 0;
}

unsigned int plugins_transform_rtl(void)
{
  RUN_ALL_FUNCS(transform_rtl);
  return 0;
}

void plugins_post_translation_unit(void)
{
  RUN_ALL_FUNCS(post_translation_unit);
}

#undef RUN_ALL_FUNCS
#undef RUN_ALL_FUNCS_PARAMS

unsigned int plugins_require_ipa(void)
{
  if(handles)
    {
      unsigned int handle_index;
      handle* handle_pointer;

      for(handle_index = 0;
          VEC_iterate(handle, handles, handle_index, handle_pointer);
          handle_index++)
        {
          if(handle_pointer->transform_cgraph)
            {
              return 1;
            }
        }
    }

  return 0;
}

static void extract_path_and_args(struct plugin_handle* const handle)
{
  char* argptr;
  int arg_index;
  size_t path_size;

  if(!strchr(handle->spec, ':'))
    {
      handle->path = xstrdup(handle->spec);
      handle->argc = 0;
      handle->argv = NULL;
      return;
    }

  argptr = handle->spec;
  handle->argc = 0;

  while((argptr = strchr(argptr, ':'))) {
    handle->argc++;
    argptr++;
  }

  path_size = (uintptr_t)strchr(handle->spec, ':') - (uintptr_t)handle->spec;
  handle->path = xmalloc(path_size + 1);
  memcpy(handle->path, handle->spec, path_size);
  handle->path[path_size] = '\0';

  handle->argv = xmalloc(sizeof(struct plugin_argument) * handle->argc);

  argptr = handle->spec;

  for(arg_index = 0; arg_index < handle->argc; arg_index++)
    {
      char* next_arg;
  
      char* key_pos;
      char* value_pos;
      size_t key_size, value_size;
  
      argptr = strchr(argptr, ':');
      argptr++;
      key_pos = argptr;
  
      next_arg = strchr(argptr, ':');

      if(next_arg)
        {
          assert(arg_index < handle->argc - 1);
          value_pos = strchr(argptr, '=');
          if(value_pos && value_pos < next_arg)
            {
              value_pos++;
      
              key_size = ((uintptr_t)value_pos - 1) - (uintptr_t)key_pos;
              value_size = (uintptr_t)next_arg - (uintptr_t)value_pos;
      
              handle->argv[arg_index].key = xmalloc(key_size + 1);
              memcpy(handle->argv[arg_index].key, key_pos, key_size);
              handle->argv[arg_index].key[key_size] = '\0';
      
              handle->argv[arg_index].value = xmalloc(value_size + 1);
              memcpy(handle->argv[arg_index].value, value_pos, value_size);
              handle->argv[arg_index].value[value_size] = '\0';
            }
          else
            {
              key_size = (uintptr_t)next_arg - (uintptr_t)key_pos;
      
              handle->argv[arg_index].key = xmalloc(key_size + 1);
              memcpy(handle->argv[arg_index].key, key_pos, key_size);
              handle->argv[arg_index].key[key_size] = '\0';
            }
        }
      else
        {
          assert(arg_index == handle->argc - 1);
          value_pos = strchr(argptr, '=');
          if(value_pos)
            {
              value_pos++;
      
              key_size = ((uintptr_t)value_pos - 1) - (uintptr_t)key_pos;
      
              handle->argv[arg_index].key = xmalloc(key_size + 1);
              memcpy(handle->argv[arg_index].key, key_pos, key_size);
              handle->argv[arg_index].key[key_size] = '\0';
      
              handle->argv[arg_index].value = xstrdup(value_pos);
            }
          else
            {
              handle->argv[arg_index].key = xstrdup(key_pos);
              handle->argv[arg_index].value = NULL;
            }
        }
    }
}

static void load_config_files(struct plugin_handle* const handle)
{
  struct plugin_argument* new_argv;
  int new_argc;
  int arg_index;

  for(arg_index = 0; arg_index < handle->argc; arg_index++)
    {
      if(!strcmp(handle->argv[arg_index].key, "_CONF"))
        {
          break;
        }
    }

  if(arg_index == handle->argc)
    {
      return;
    }

  new_argc = 0;
  new_argv = NULL;

  for(arg_index = 0; arg_index < handle->argc; arg_index++)
    {
      if(strcmp(handle->argv[arg_index].key, "_CONF"))
        {
          new_argc++;
          new_argv = xrealloc(new_argv, new_argc * sizeof(struct plugin_argument));
          memcpy(&new_argv[new_argc - 1], &handle->argv[arg_index], sizeof(struct plugin_argument));
        }
      else
        {
          char* path;
          FILE* fp;
          char* current_line;

          current_line = xmalloc(4096);

          path = handle->argv[arg_index].value;
          fp = fopen(path, "r");

          if(!fp)
            {
              warning(0, "Loading config file %s failed: %s", path, strerror(errno));
              continue;
            }

          while(fgets(current_line, 4096, fp))
            {
              char* newline_pos;
              char* key_pos;
              char* value_pos;

              /* chomp the string, as in Perl */

              newline_pos = strchr(current_line, '\r');
              if(!newline_pos)
                {
                  newline_pos = strchr(current_line, '\n');
                }
              if(newline_pos)
                {
                  *newline_pos = '\0';
                }

              key_pos = current_line;

              value_pos = strchr(current_line, '=');

              if(value_pos)
                {
                  *value_pos = '\0';
                  value_pos++;
                }

              new_argc++;
              new_argv = xrealloc(new_argv, new_argc * sizeof(struct plugin_argument));

              new_argv[new_argc - 1].key = xstrdup(key_pos);
              new_argv[new_argc - 1].value = value_pos ? xstrdup(value_pos) : NULL;
            }

          fclose(fp);
        }
    }

  handle->argc = new_argc;
  handle->argv = new_argv;
}

#define EXTRACT_SYMBOL(sym)				\
  do							\
    {							\
      void* code;					\
      code = lt_dlsym(object_handle, #sym);		\
      memcpy(&new_handle.sym, &code, sizeof(void*));	\
    }							\
  while(0);

void register_tree_plugin(const char* spec)
{
  lt_dlhandle object_handle;
  handle new_handle;

  if(!handles)
    {
      handles = VEC_alloc(handle, heap, MAX_PLUGINS);
    }

  new_handle.spec = xstrdup(spec);

  extract_path_and_args(&new_handle);
  load_config_files(&new_handle);

  lt_dlinit();

  object_handle = lt_dlopen(new_handle.path);

  if(!object_handle) 
    {
      warning(0, "Loading plugin %s failed: %s\n", new_handle.path, lt_dlerror());
      return;
    }

  EXTRACT_SYMBOL(pre_translation_unit);
  EXTRACT_SYMBOL(transform_ctrees);
  EXTRACT_SYMBOL(transform_gimple);
  EXTRACT_SYMBOL(transform_cgraph);
  EXTRACT_SYMBOL(transform_rtl);
  EXTRACT_SYMBOL(post_translation_unit);

  VEC_safe_push(handle, heap, handles, &new_handle);
}

#undef EXTRACT_SYMBOL
