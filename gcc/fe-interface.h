/* Header file for the front end interface to the back end.
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

This file contains the prototypes for any back end specific functions which
a front end may need.  These functions are declared in fe-interface.c and
act as wrappers to the actual backend call.   

This allows front ends to access individual backend routines without requiring
all the other stuff in back end header files for compilation.  */

#ifndef GCC_FE_INTERFACE_H
#define GCC_FE_INTERFACE_H

/* Defined if this is being used in a backend interface  (ie, fe-interface.c
  or a gimpliffication file.  */
#ifndef FE_INTERFACE_C

#include "tree-fe.h"
#define FOR_EACH_LOCAL_DECL(FUN, I, D)			\
  FOR_EACH_VEC_SAFE_ELT_REVERSE ((FUN)->local_decls(), I, D)

#else
#include "tree.h"
#endif

/* Currently there are references to 'enum machine_mode' in these prototypes
   so include machmode.h here until we get to how to export that.  */
#include "machmode.h"


/* From function.c. */
/* ------------------------------------------------------------------------ */

// extern GTY(()) struct function *cfun;
extern void push_function_context (void);
extern void pop_function_context (void);
extern void used_types_insert (tree);
extern void allocate_struct_function (tree, bool);
extern void set_cfun (struct function *);
extern void pop_cfun (void);
extern void push_cfun (struct function *new_cfun);
extern void push_struct_function (tree fndecl);
extern tree block_chainon (tree, tree);
extern int aggregate_value_p (const_tree, const_tree);

extern GTY(()) vec<tree, va_gc> *types_used_by_cur_var_decl;
void types_used_by_var_decl_insert (tree type, tree var_decl);



/* Used in ada/gcc-interface/misc.c with a NULL first param, so provide a
   function without that parameter. */
bool pass_by_reference (enum machine_mode, tree, bool);


class fe_function {
public:
  struct language_function * language ();
  void set_language (struct language_function *);
  void set_calls_cilk_spawn (unsigned int);
  location_t function_end_locus ();
  unsigned int returns_struct ();
  void set_returns_struct (unsigned int);
  unsigned int returns_pcc_struct ();
  void set_returns_pcc_struct (unsigned int);
  tree decl ();
  void add_local_decl (tree);
  tree cilk_frame_decl ();
  void set_cilk_frame_decl (tree);
  void set_function_end_locus(location_t);
  void set_function_start_locus(location_t);
  void set_is_cilk_function (unsigned int);
  unsigned curr_properties ();
  void set_curr_properties (unsigned i);
  vec<tree, va_gc> *local_decls();
  unsigned int cannot_be_copied_set();
  const char *cannot_be_copied_reason();

protected:
  struct function *func();

};


class fe_function_ptr {
  public:
    fe_function_ptr () { ptr = NULL; };
    fe_function_ptr (struct function *p)
				  { ptr = reinterpret_cast<fe_function *>(p); }
    operator bool() const { return ptr != NULL; }
    operator struct function *()
			   { return reinterpret_cast<struct function *>(ptr); }
    fe_function *operator->() { return ptr; }
    fe_function &operator*() { return *ptr; }
    friend bool operator== (const fe_function_ptr&, const fe_function_ptr&);
    friend bool operator!= (const fe_function_ptr&, const fe_function_ptr&);
    friend bool operator== (const fe_function_ptr&, long int);
    friend bool operator!= (const fe_function_ptr&, long int);
  protected:
    fe_function *ptr;
  private:
    fe_function_ptr& operator= (fe_function_ptr& p);
};

inline bool 
operator== (const fe_function_ptr& p1, const fe_function_ptr& p2)
{
  return p1.ptr == p2.ptr;
}

inline bool 
operator!= (const fe_function_ptr& p1, const fe_function_ptr& p2)
{
  return p1.ptr != p2.ptr;
}

inline bool 
operator== (const fe_function_ptr& p, long int x)
{
  return p.ptr == (void *)x;
}

inline bool 
operator!= (const fe_function_ptr& p, long int x)
{
  return p.ptr != (void *)x;
}



#ifndef FE_INTERFACE_C

extern fe_function_ptr *current_function;
#define cfun  (*current_function)

#endif


/* From optabs.c. */ /* ------------------------------------------------------------------------ */

/* Return true if there is an inline compare and swap pattern.  */
extern bool can_compare_and_swap_p (enum machine_mode, bool);



/* From cgraph.c.  */
/* ------------------------------------------------------------------------ */

/* Related function prototypes for fe-interface.c.  */
extern void cgraph_add_to_same_comdat_group (tree, tree);


#endif /* GCC_FE_INTERFACE_H */
