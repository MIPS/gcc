/* [Legacy] environment-based interface for loading high-level plugins.
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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "hashtab.h"
#include "toplev.h"
#include "gcc.h"
#include "options.h"		/* flag_ici */

#include "highlev-plugin-internal.h"
#include "feature-internal.h"	/* init_features() */

#include <dlfcn.h>

int ici_loaded = 0;

/* plugin initialization/cleanup function type */
typedef void (*start_stop_func) (void);

typedef union {
  void *data_ptr;
  start_stop_func function_ptr;
} dl_symbol_t ; 


/* pointers to initialization/cleanup functions */
start_stop_func ici_plugin_start = NULL;
start_stop_func ici_plugin_stop = NULL;


/* report any errors encountered during a dl* operation */
static inline bool check_for_dlerror (void)
{
  const char *dlError;
  dlError = dlerror ();
  if (dlError)
  {
    fprintf (stderr, "Plugin manager error: DLerror: %s ...\n", dlError);
    return true;
  }
  return false;
}


/* load the ICI plugin shared library file named 'dynlib_file'.
   Return 0 if successful, and 1 otherwise. */
static int 
load_ici_plugin_file (char *dynlib_file)
{
  void  *dlhandle;
  bool error = 0;
  dl_symbol_t dl_symbol;

  if (getenv ("ICI_VERBOSE") != NULL)
    fprintf (stderr, "ICI: loading plugin ...\n");
  
  dlhandle = dlopen (dynlib_file, RTLD_LAZY);
  error |= check_for_dlerror ();

  /* assignment of data pointers to function pointers is forbidden in strict
     ANSI mode - use a union to work around this dlsym() issue. */

  /* plugin initialization function */
  dl_symbol.data_ptr = dlsym (dlhandle, "start");
  ici_plugin_start = dl_symbol.function_ptr;
  error |= check_for_dlerror ();

  /* plugin cleanup function */
  dl_symbol.data_ptr = dlsym (dlhandle, "stop");
  ici_plugin_stop = dl_symbol.function_ptr;
  error |= check_for_dlerror ();

  /* if init and cleanup functions are correctly resolverd, initialize the feature list */
  if (!error)
    init_features ();

  return error;
}


/* load a plugin speficied by envariable ICI_PLUGIN if either -fici was
   given on the command line or envariable ICI_USE is set to '1'. */
void load_ici_plugin (void)
{
  char *ici_use = getenv ("ICI_USE");

  if ((ici_use != NULL) && (ici_use[0] == '1'))
    flag_ici = true;

  if (flag_ici)
    {
      char *dynlib_file = getenv ("ICI_PLUGIN");
      if (dynlib_file != NULL)
	{
	  if ((load_ici_plugin_file (dynlib_file) == 0) && ici_plugin_start)
	    {
	      ici_plugin_start ();
	      ici_loaded = 1;
	    }
	}
      else
	{
	  fprintf (stderr, "ICI error: Environment variable ICI_PLUGIN is not defined ...\n");
	  exit (-1);
	}
    }
}

void unload_ici_plugin (void)
{
  if ((ici_loaded == 1) && ici_plugin_stop)
    ici_plugin_stop ();
 
  if (getenv ("ICI_VERBOSE") != NULL)
    fprintf (stderr, "ICI: unloading plugin...\n"); 
}

