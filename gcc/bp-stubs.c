/* Subroutines shared by all languages that are variants of C.
   Copyright (C) 2000
   Free Software Foundation, Inc.

   This file is part of GNU CC.

   GNU CC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GNU CC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU CC; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "tree.h"
#include "errors.h"

static tree bp_abort PARAMS ((void));


static tree
bp_abort ()
{
  error ("bounded pointers are unsupported");
  abort ();
  return NULL_TREE;
}

tree
build_bounded_ptr_field_ref (bp, field_number)
     tree bp ATTRIBUTE_UNUSED;
     int field_number ATTRIBUTE_UNUSED;
{
  if (field_number == 0)
    return bp;
  return bp_abort ();
}

tree
build_bounded_ptr_check (bp, length)
     tree bp ATTRIBUTE_UNUSED;
     tree length ATTRIBUTE_UNUSED;
{
  return bp_abort ();
}

tree
build_bounded_ptr_constructor (addr)
     tree addr ATTRIBUTE_UNUSED;
{
  return bp_abort ();
}

tree
build_bounded_ptr_constructor_2 (addr, bounds)
     tree addr ATTRIBUTE_UNUSED;
     tree bounds ATTRIBUTE_UNUSED;
{
  return bp_abort ();
}

tree
build_bounded_ptr_constructor_3 (addr, low_bound, high_bound)
     tree addr ATTRIBUTE_UNUSED;
     tree low_bound ATTRIBUTE_UNUSED;
     tree high_bound ATTRIBUTE_UNUSED;
{
  return bp_abort ();
}

tree
get_high_bound_decl (decl)
     tree decl ATTRIBUTE_UNUSED;
{
  return NULL_TREE;
}
