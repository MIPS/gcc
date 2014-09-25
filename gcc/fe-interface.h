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

/* Currently there are references to 'enum machine_mode' in these prototypes
   so include machmode.h here until we get to how to export that.  */
#include "machmode.h"


/* From function.c. */
/* ------------------------------------------------------------------------ */

extern GTY(()) struct function *cfun;
extern void push_function_context (void);
extern void pop_function_context (void);
extern void used_types_insert (tree);

/* Related function prototypes for fe-interface.c.  */
extern struct language_function * function_language (struct function *);
extern void function_set_language (struct function *,
				   struct language_function *);
extern void function_set_calls_cilk_spawn (struct function *, unsigned int);
extern location_t function_get_end_locus (struct function *);
extern bool function_returns_struct (struct function *);
extern tree function_decl (struct function *);
extern void function_add_local_decl (struct function *, tree);

/* Used in ada/gcc-interface/misc.c with a NULL first param, so provide a
   function without that parameter. */
extern bool pass_by_reference (enum machine_mode, tree, bool);


/* From optabs.c. */
/* ------------------------------------------------------------------------ */

/* Return true if there is an inline compare and swap pattern.  */
extern bool can_compare_and_swap_p (enum machine_mode, bool);



/* From cgraph.c.  */
/* ------------------------------------------------------------------------ */

/* Related function prototypes for fe-interface.c.  */
extern void cgraph_add_to_same_comdat_group (tree, tree);


#endif /* GCC_FE_INTERFACE_H */
