/* Declarations and definitions of codes relating to the
   encoding of global decls and types into the object files.

   Copyright (C) 2008 Free Software Foundation, Inc.
   Contributed by Kenneth Zadeck <zadeck@naturalbridge.com>

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2, or (at your option) any later
   version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.  */

#ifndef GCC_LTO_SECTION_H
#define GCC_LTO_SECTION_H

#include "lto-header.h"

struct lto_decl_header
{
  struct lto_header lto_header;   /* The header for all types of sections. */
  int32_t decl_state_size;	  /* Size of region for decl state. */
  int32_t num_nodes;              /* Number of nodes in globals stream.  */
  int32_t main_size;		  /* Size of region for expressions, decls, types, etc. */
  int32_t string_size;		  /* Size of the string table.  */
  int32_t debug_main_size;	  /* Size of region for main stream debugging information.  */
};

#endif /* GCC_LTO_SECTION_H */
