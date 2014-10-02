/* Front end interface to the back end.
   Copyright (C) 2014 Free Software Foundation, Inc.

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
<http://www.gnu.org/licenses/>.  

This file contains the wrapper functions for any back end specific functions
which a front end may need.  */


#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "function.h"
#include "hash-map.h"
#include "is-a.h"
#include "plugin-api.h"
#include "vec.h"
#include "basic-block.h"
#include "function.h"
#include "ipa-ref.h"
#include "dumpfile.h"
#include "cgraph.h"

/* From funciton.[ch].  */
/* ----------------------------------------------------------------------- */

/* Return the language dependent pointer from satruct function.  */
struct language_function *
function_language (struct function * f)
{
  return f->language;
}

/* Set the language dependent pointer from satruct function.  */
void
function_set_language (struct function *f, struct language_function *lang)
{
  f->language = lang;
}


/* Set the calls_cilk_spawn field.  */
void
function_set_calls_cilk_spawn (struct function *f, unsigned int val)
{
  f->calls_cilk_spawn = val;
}

/* Get the end locus field.  */
extern location_t 
function_get_end_locus (struct function *f)
{
  return f->function_end_locus;
}

/* Get the returns_struct field.  */
bool
function_returns_struct (struct function *f)
{
  return (f->returns_struct);
}

/* Get the function decl.  */
tree
function_decl (struct function *f)
{
  return f->decl;
}

/* add_Local_decl() is a static inline function in function.h. Change the name
    of the interface routine, and call it. */
void
function_add_local_decl (struct function *fun, tree d)
{
  add_local_decl (fun, d);
}

/* Call pass_by_reference with a NULL first parameter.  */
bool 
pass_by_reference (enum machine_mode m, tree t, bool b)
{
  /* ada uses thisroutine with a NULL CUMULTIVE_ARG argument.  */
  return pass_by_reference (NULL, m, t , b);
}


/* From cgraph.[ch].  */
/* ----------------------------------------------------------------------- */

/*  Call add_to_same_comdat_group. */
void
cgraph_add_to_same_comdat_group (tree statfn, tree callop)
{
  /* Put the thunk in the same comdat group as the call op.  */
  cgraph_node::get_create (statfn)->add_to_same_comdat_group
    (cgraph_node::get_create (callop));
}
