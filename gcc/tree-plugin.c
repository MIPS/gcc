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

unsigned int run_all_plugins(void);

struct tree_opt_pass pass_plugin =
{
  /* const char*		name				*/
  "plugin",
  /* bool			(*gate)			(void)	*/
  NULL,
  /* int			(*execute)		(void)	*/
  run_all_plugins,
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

#define MAX_PLUGINS 32

struct plugin_handle {
	void (*pre_translation_unit)(void);
	void (*run)(void);
	void (*post_translation_unit)(void);

	char* spec;
	char* path;

	int argc;
	struct plugin_argument*	argv;
};

static struct plugin_handle plugins[MAX_PLUGINS];
static int num_plugins = 0;
static int plugin_index;

#define RUN_ALL_FUNCS(function_name)					\
  do									\
    {									\
      for(plugin_index = 0; plugin_index < num_plugins; plugin_index++)	\
        {								\
          if(plugins[plugin_index].function_name)			\
            {								\
              plugins[plugin_index].function_name();			\
            }								\
        }								\
    }									\
  while(0)

void pre_tu_plugins(void)
{
  RUN_ALL_FUNCS(pre_translation_unit);
}

unsigned int run_all_plugins(void)
{
  RUN_ALL_FUNCS(run);
  return 0;
}

void post_tu_plugins(void)
{
  RUN_ALL_FUNCS(post_translation_unit);
}

#undef RUN_ALL_FUNCS

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

void register_tree_plugin(const char* spec)
{
  lt_dlhandle handle;
  void* pre_translation_unit_object;
  void* run_object;
  void* post_translation_unit_object;

  if(num_plugins == MAX_PLUGINS)
    {
      /* An error I'd love to get. */

      warning(0, "Too many plugins. Increase MAX_PLUGINS");
      return;
    }

  plugins[num_plugins].spec = xstrdup(spec);

  extract_path_and_args(&plugins[num_plugins]);
  load_config_files(&plugins[num_plugins]);

  lt_dlinit();

  handle = lt_dlopen(plugins[num_plugins].path);

  if(!handle) 
    {
      warning(0, "Loading plugin %s failed: %s\n", plugins[num_plugins].path, lt_dlerror());
      return;
    }
  
  pre_translation_unit_object = lt_dlsym(handle, "pre_translation_unit");
  run_object = lt_dlsym(handle, "run");
  post_translation_unit_object = lt_dlsym(handle, "post_translation_unit");

  memcpy(&(plugins[num_plugins].pre_translation_unit), &pre_translation_unit_object, sizeof(void*));
  memcpy(&(plugins[num_plugins].run), &run_object, sizeof(void*));
  memcpy(&(plugins[num_plugins].post_translation_unit), &post_translation_unit_object, sizeof(void*));

  num_plugins++;
}

int get_plugin_arguments(int* argc_ptr, struct plugin_argument** argv_ptr)
{
  if(plugin_index < num_plugins)
    {
      *argv_ptr = plugins[plugin_index].argv;
      *argc_ptr = plugins[plugin_index].argc;
      return 0;
    }
  else 
    {
      return -1;
    }
}
