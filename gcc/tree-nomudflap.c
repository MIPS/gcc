/* Mudflap: narrow-pointer bounds-checking by tree rewriting.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
   Contributed by Frank Ch. Eigler <fche@redhat.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */


#include "config.h"
#include "errors.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-inline.h"
#include "c-tree.h"
#include "c-common.h"
#include "tree-simple.h"
#include "diagnostic.h"
#include "hashtab.h"
#include "output.h"
#include "varray.h"
#include "langhooks.h"
#include "tree-mudflap.h"
#include "ggc.h"



/* This file contains placeholder functions, to be used only for
   language processors that cannot handle tree-mudflap.c directly.
   (e.g. Fortran).  */

 
void 
mudflap_c_function (t)
     tree t ATTRIBUTE_UNUSED;
{
  abort ();
}


void 
mudflap_enqueue_decl (obj, label)
     tree obj ATTRIBUTE_UNUSED;
     const char *label ATTRIBUTE_UNUSED;
{
  internal_error ("mudflap: this language is not supported");
}


void 
mudflap_enqueue_constant (obj, label)
     tree obj ATTRIBUTE_UNUSED;
     const char *label ATTRIBUTE_UNUSED;
{
  internal_error ("mudflap: this language is not supported");
}



/* Emit file-wide instrumentation.  */
 
void 
mudflap_finish_file ()
{
  internal_error ("mudflap: this language is not supported");
}


/* Instead of:
#include "gt-tree-mudflap.h"
We prepare a little dummy struct here.
*/

const struct ggc_root_tab gt_ggc_r_gt_tree_mudflap_h[] = {
  LAST_GGC_ROOT_TAB
};
