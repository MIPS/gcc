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
#include "hashtab.h"
#include "hash-set.h"
#include "vec.h"
#include "machmode.h"
#include "tm.h"
#include "hard-reg-set.h"
#include "input.h"
#include "function.h"
#include "hash-map.h"
#include "is-a.h"
#include "plugin-api.h"
#include "predict.h"
#include "basic-block.h"
#include "ipa-ref.h"
#include "dumpfile.h"
#include "cgraph.h"

#define FE_INTERFACE_C
#include "fe-interface.h"

/* From function.[ch].  */
/* ----------------------------------------------------------------------- */

/* Call pass_by_reference with a NULL first parameter.  */
bool 
pass_by_reference (enum machine_mode m, tree t, bool b)
{
  /* ada uses thisroutine with a NULL CUMULTIVE_ARG argument.  */
  return pass_by_reference (NULL, m, t , b);
}


struct function *
fe_function::func ()
{
  return reinterpret_cast<struct function *>(this);
}

/* Return the language dependent pointer from struct function.  */
struct language_function *
fe_function::language ()
{
  return func()->language;
}

/* Set the language dependent pointer from satruct function.  */
void
fe_function::set_language (struct language_function *lang)
{
  func ()->language = lang;
}


/* Set the calls_cilk_spawn field.  */
void
fe_function::set_calls_cilk_spawn (unsigned int val)
{
  func ()->calls_cilk_spawn = val;
}

/* Get the end locus field.  */
extern location_t 
fe_function::function_end_locus ()
{
  return func ()->function_end_locus;
}

/* Get the returns_struct field.  */
unsigned int
fe_function::returns_struct ()
{
  return (func ()->returns_struct);
}

void
fe_function::set_returns_struct (unsigned int b)
{
  func ()->returns_struct = b;
}

unsigned int
fe_function::returns_pcc_struct ()
{
  return (func ()->returns_pcc_struct);
}

void
fe_function::set_returns_pcc_struct (unsigned int b)
{
  func ()->returns_pcc_struct = b;
}

/* Get the function decl.  */
tree
fe_function::decl ()
{
  return func ()->decl;
}

/* add_Local_decl() is a static inline function in function.h. Change the name
    of the interface routine, and call it. */
void
fe_function::add_local_decl (tree d)
{
  ::add_local_decl (func (), d);
}

tree
fe_function::cilk_frame_decl ()
{
  return func ()->cilk_frame_decl;
}

void 
fe_function::set_cilk_frame_decl (tree d)
{
  func ()->cilk_frame_decl = d;
}

void
fe_function::set_function_end_locus (location_t loc)
{
  func ()->function_end_locus = loc;
}

void
fe_function::set_function_start_locus (location_t loc)
{
  func ()->function_start_locus = loc;
}

void
fe_function::set_is_cilk_function (unsigned int b)
{
  func ()->is_cilk_function = b;
}

unsigned
fe_function::curr_properties ()
{
  return func ()->curr_properties;
}

void
fe_function::set_curr_properties (unsigned i)
{
  func ()->curr_properties = i;
}

vec<tree, va_gc> *
fe_function::local_decls ()
{
  return func ()->local_decls;
}

unsigned int 
fe_function::cannot_be_copied_set ()
{
  return func ()->cannot_be_copied_set;
}

const char *
fe_function::cannot_be_copied_reason ()
{
  return func ()->cannot_be_copied_reason;
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




/* ----------------------------------------------------------------------- */

/* cfun is defined as (cfun + 0) to prevent assignment.  circumnavigate this
   so we can take its address...  DO it last so we dont accidentally 
   trip over it in this file. */

#undef cfun
fe_function_ptr *current_function = reinterpret_cast<fe_function_ptr *>(&cfun);


