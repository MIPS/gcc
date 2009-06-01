/* High-level access points to global compiler state in GCC.
   Copyright (C) 2009 Free Software Foundation, Inc.
   
   Contributed by Inria.

   Authors: Grigori Fursin <grigori.fursin@inria.fr>, Cupertino Miranda
   <cupertinomiranda@gmail.com>, Zbigniew Chamski <zbigniew.chamski@gmail.com>.

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

#include "feature-internal.h"

#include "config.h"
#include "system.h"

#include "coretypes.h"
#include "tm.h"
#include "line-map.h"
#include "input.h"
#include "tree.h"
#include "basic-block.h"
#include "tree-inline.h"
#include "tree-flow.h"
#include "tree-pass.h"

#include "version.h"
#include "opts.h"
#include "params.h"
#include "function.h"
#include "input.h"

/* FORNOW: ZC: make declarations from "passes.c" visible here */
/* A map from static pass dump id to optimization pass.  */
extern struct opt_pass **passes_by_id;
extern int passes_by_id_size;


/* FICI0: "function_name": get name of current function if the
   current function is defined, and return NULL otherwise.  */
static const void *function_name (void)
{
  if (cfun)
    return (const void *) current_function_name();
  else
    return NULL;
}

const struct feature feature_function_name = {
  "function_name",                /* name */
  NULL,                           /* data */
  0,				  /* no data */
  &function_name,                 /* callback */
  NULL,				  /* no get subfeature callback */
  NULL				  /* no set subfeature callback */
};

/* FICI0: "first_pass": get name of the first pass of the compiler */
struct opt_pass *_pass_parents[] = { NULL, NULL, NULL, NULL, NULL, 
					 NULL, NULL, NULL, NULL, NULL}; 
int _pass_parents_count = 0; 
struct opt_pass *_current_pass = NULL;

static const void *first_pass (void)
{
  _current_pass = all_passes;
  return (const void *) _current_pass->name;
}

const struct feature feature_first_pass = {
  "first_pass",                   /* name */
  NULL,                           /* data */
  0,				  /* no data */
  &first_pass,                    /* callback */
  NULL,				  /* no get subfeature callback */
  NULL				  /* no set subfeature callback */
};

/* FICI0: "next_pass": get the name of the next pass to be executed */
static const void *next_pass (void)
{
  if(_current_pass == NULL)
    {
      return (const void *) NULL;
    }

  if(_current_pass->sub != NULL)
    {
      /* Insert _current_pass->next in parents list */
      _pass_parents[_pass_parents_count++] = _current_pass->next;
      _current_pass = _current_pass->sub;

      return (const void *) _current_pass->name;
    }
  
  if(_current_pass->next == NULL)
    {
      while(_pass_parents[_pass_parents_count] == NULL
	    && _pass_parents_count > 0)
	_pass_parents_count--;

      _current_pass = _pass_parents[_pass_parents_count];
      /* Delete _current_pass in parents list */
      _pass_parents[_pass_parents_count] = NULL;

      return (const void *) (_current_pass != NULL ? 
			     _current_pass->name :
			     NULL);
    }

  _current_pass = _current_pass->next;
  return (const void *) _current_pass->name;
}

const struct feature feature_next_pass = {
  "next_pass",                    /* name */
  NULL,                           /* data */
  0,				  /* data size */
  &next_pass,                     /* callback */
  NULL,				  /* no get subfeature callback */
  NULL				  /* no set subfeature callback */
};


/* FICI0: compiler_all_passes */
/* list of all passes requires names or address/sourceloc of each instance */
static const void *
all_gcc_pass_names (void); /* forward declaration - code references feature */

struct feature feature_named_passes = {
  "named_passes",                 /* name */
  NULL,                           /* initial data */
  0,				  /* initially no data */
  &all_gcc_pass_names,            /* callback */
  NULL,				  /* no get subfeature callback */
  NULL				  /* no set subfeature callback */  
};


static const void *
all_gcc_pass_names (void)
{
  int i;
  char *new_name;
  const char **pass_name =
    (const char **) xmalloc (passes_by_id_size * sizeof (const char *));

  for (i = 0;  i < passes_by_id_size; i++)
    {
      if (passes_by_id[i] == NULL)
	pass_name[i] = "<no pass defined>";
      else
	{
	  if (passes_by_id[i]->name != NULL)
	    pass_name[i] = passes_by_id[i]->name;
	  else
	    {
	      /* be defensive - do not allow string overflow */
	      gcc_assert (passes_by_id_size < 1000);
	      new_name = (char *) xmalloc (strlen ("pass_XXX") + 1);
	      sprintf (new_name, "pass_%03d", i);
	      /* store the new name in the option as well */
	      /* to keep memory clean when freeing a pass, any non-NULL
		 pass name of the format "pass_%03d" should be freed as
		 well, because it was dynamically allocated. */
	      pass_name[i] = passes_by_id[i]->name = (const char *) new_name;
	    }
	}
    }

  /* memoize the result */
  feature_named_passes.data = (void *) pass_name ;
  feature_named_passes.data_size =
    passes_by_id_size * sizeof (char *);
  return (void *) pass_name;
}


/* FICI0: get name of the current pass */
static const void *
get_gcc_pass_name (void)
{
  return (const void *) get_current_pass_name ();
}


const struct feature feature_crnt_pass_name = {
  "pass_name",                    /* name */
  NULL,                           /* data */
  0,				  /* no data */
  &get_gcc_pass_name,             /* callback */
  NULL,				  /* no get subfeature callback */
  NULL				  /* no set subfeature callback */
};


/* FICI0: "compiler_version": get version string of the compiler */
static const void *
get_compiler_version_string (void);


/* FICI0: query compiler version */
const struct feature feature_compiler_version = {
  "compiler_version",		  /* GCC version */
  NULL,				  /* data */
  0,				  /* no data */
  get_compiler_version_string,    /* data: version_string of GCC */
  NULL,				  /* no get subfeature callback */
  NULL				  /* no set subfeature callback */
};


static const void *
get_compiler_version_string (void)
{
  /* this is the time to memoize the value if necessary */
  return (const void *) version_string;
}


/* FICI0: "flags": get the command-line flags of the compiler
 * return values:
 *  - string subfeatures: pointer-to-char corresponding to the string;
 *  - integer/boolean/bit subfeatures: integer converted to pointer-to-void.
 */
static const void *get_gcc_option_names(void);

static const void *
get_gcc_option_value (const char *opt)
{
  size_t opt_index;
  const struct cl_option *option;
  struct cl_option_state state;
  bool was_set;
  
  opt_index = find_opt (opt, -1);
  if (opt_index == cl_options_count)
    return (const char *) NULL;
  
  /* at this point, the option is known to exist */
  option = &cl_options[opt_index];
  was_set = get_option_state (opt_index, &state);
	   
  /* check if option is set, and return the value (int or ptr); IMPORANT:
     the user has to know how to interpret the value that's returned. */
  if (!was_set)
    return (const void *) NULL;
  else switch (cl_options[opt_index].var_type) {
    case CLVC_STRING:
      /* for string options, return actual string */
      return (state.data ? state.data : (const void *) "");

    case CLVC_BOOLEAN:
    case CLVC_EQUAL:
      /* booleans and integers are stored by converting them to a 'const
	 void *'. */
      return state.data;
    case CLVC_BIT_CLEAR:
    case CLVC_BIT_SET:
      /* bits set/cleared: return the char (0 or 1) converted to 'const void *' */
      return (const void *) ((long int) state.ch);
    default:
      gcc_unreachable ();
    }
}


/* value is a pointer to the actual value.  NULL means no value.
   Implementation is based on static unsigned int handle_option in gcc/opts.c
*/
static void *
set_gcc_option_value (const char *opt __attribute__ ((unused)),
		      void *value __attribute__ ((unused)))
{
  /* not implemented yet */
  return NULL;
}


/* FICI0: "compiler_flags": get list/values of compiler flags.  Example:
   call 'ici_get_feature ("compiler_params")' to get the list of parameters.
   The value returned by this call will be memoized in the 'data' field.
   Reset the 'data' field to NULL (and 'data_size' to zero) to force
   a recomputation of the list.
*/
struct feature feature_compiler_flags = {
  "compiler_flags",		/* name */
  NULL,				/* initially NULL to trigger callback, later
				   stores the memoized result */
  0,				/* no data */
  get_gcc_option_names,		/* return list of option names */
  get_gcc_option_value,		/* get value of named option */
  set_gcc_option_value		/* set value of named option */
};


static const void *get_gcc_option_names (void)
{
  size_t i;

  const char **option_names =
    (const char **) xmalloc (cl_options_count * sizeof (const char *));

  for (i = 0; i < cl_options_count; i++)
    option_names[i] = cl_options[i].opt_text;

  feature_compiler_flags.data = option_names;
  feature_compiler_flags.data_size = cl_options_count * sizeof (const char *);

  return (const void *) option_names;
}


/* FICI0: "compiler_params": get list/values of compiler parameters. */

/* forward declaration - definition follows feature initializer */
static const void *get_gcc_param_names (void);


static const void *
get_gcc_param_value (const char *name)
{
  size_t i;

  /* Scan the parameter table to find a matching entry - from 'set_param_value'  */
  for (i = 0; i < num_compiler_params; ++i)
    if (strcmp (compiler_params[i].option, name) == 0)
      return (void *) ((long int) compiler_params[i].value);

  /* If we didn't find the parameter, return the invalid parameter value */
  return (void *)(-1);
}

static void *set_gcc_param_value (const char *name, void *value)
{
  size_t i;

  /* Scan the parameter table to find a matching entry - from 'set_param_value'  */
  for (i = 0; i < num_compiler_params; ++i)
    if (strcmp (compiler_params[i].option, name) == 0)
      compiler_params[i].value = (int) ((long int) value);

  /* If we didn't find the parameter, return the invalid parameter value */
  return (void *)(-1); 
}

/* FICI0: "compiler_params": get list/values of compiler parameters.
   Call 'ici_get_feature ("compiler_params")' to get the list of parameters.
   The value returned by this call will be memoized in the 'data' field.
   Reset the 'data' field to NULL (and 'data_size' to zero) to force
   a recomputation of the list.
*/
struct feature feature_compiler_params = {
  "compiler_params",		/* name */
  NULL,				/* initially NULL to trigger callback, later
				   stores memoized callback result */
  0,				/* no data */
  get_gcc_param_names,		/* return list of parameter names */
  get_gcc_param_value,		/* get value of named parameter - always an integer */
  set_gcc_param_value		/* set value of named parameter */
};


static const void *
get_gcc_param_names (void)
{
  size_t i;
  const char **param_list =
    (const char **) xmalloc (num_compiler_params * sizeof (const char *));

  /* collect all parameter names */
  for (i = 0; i < num_compiler_params; i++)
    param_list[i] = compiler_params[i].option;

  /* memoize list in feature descriptor */
  feature_compiler_params.data = param_list;
  feature_compiler_params.data_size =
    num_compiler_params * sizeof (const char *);
  
  return (void *) param_list;
}


/* FICI3: Get cfun source code information:
   - declaration line
   - declaration filename
   - definition start/end line
   - definition filename
*/

static const void *
get_cfun_decl_line (void)
{
  if (!cfun)
    return (const void *) -1;

  return (const void *) ((long int) LOCATION_LINE (DECL_SOURCE_LOCATION (cfun->decl)));
}

static const void *
get_cfun_decl_filename (void)
{
  if (!cfun)
    return NULL;

  return (const void *) LOCATION_FILE (DECL_SOURCE_LOCATION (cfun->decl));
}

static const void *
get_cfun_def_start_line (void)
{
  if (!cfun)
    return (const void *) -1;

  return (const void *) ((long int) LOCATION_LINE (cfun->function_start_locus));
}

static const void *
get_cfun_def_end_line (void)
{
  if (!cfun)
    return (const void *) -1;

  return (const void *) ((long int) LOCATION_LINE (cfun->function_end_locus)); 
}

static const void *
get_cfun_def_filename (void)
{
  if (!cfun)
    return NULL;

  return (const void *) LOCATION_FILE (cfun->function_start_locus);
}

const struct feature feature_function_start = {
  "function_start_line",          /* name */
  NULL,                           /* data */
  0,				  /* no data */
  &get_cfun_def_start_line,       /* callback */
  NULL,				  /* no get subfeature callback */
  NULL				  /* no set subfeature callback */
};

const struct feature feature_function_end = {
  "function_end_line",            /* name */
  NULL,                           /* data */
  0,				  /* no data */
  &get_cfun_def_end_line,         /* callback */
  NULL,				  /* no get subfeature callback */
  NULL				  /* no set subfeature callback */
};

const struct feature feature_function_file = {
  "function_filename",            /* name */
  NULL,                           /* data */
  0,				  /* no data */
  &get_cfun_def_filename,         /* callback */
  NULL,				  /* no get subfeature callback */
  NULL				  /* no set subfeature callback */
};

const struct feature feature_function_decl_line = {
  "function_decl_line",           /* name */
  NULL,                           /* data */
  0,				  /* no data */
  &get_cfun_decl_line,            /* callback */
  NULL,				  /* no get subfeature callback */
  NULL				  /* no set subfeature callback */
};

const struct feature feature_function_decl_file = {
  "function_decl_filename",       /* name */
  NULL,                           /* data */
  0,				  /* no data */
  &get_cfun_decl_filename,        /* callback */
  NULL,				  /* no get subfeature callback */
  NULL				  /* no set subfeature callback */
};


/* FICI0: initialize all known features */
void init_features (void)
{
  register_feature (&feature_function_name); 
  register_feature (&feature_crnt_pass_name);

  register_feature (&feature_first_pass); 
  register_feature (&feature_next_pass);
  register_feature (&feature_compiler_version);

  register_feature (&feature_compiler_flags);
  register_feature (&feature_compiler_params);
  register_feature (&feature_named_passes);

  register_feature (&feature_function_start);
  register_feature (&feature_function_end);
  register_feature (&feature_function_file);
  register_feature (&feature_function_decl_line);
  register_feature (&feature_function_decl_file);
}

