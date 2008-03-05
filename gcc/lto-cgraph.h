/* Declarations and definitions relating to the encoding of the cgraph
   into the object files.

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

#ifndef GCC_LTO_CGRAPH_H
#define GCC_LTO_CGRAPH_H

#include "tree.h"
#include "lto-header.h"

enum LTO_cgraph_tags
{
  /* Must leave 0 for the stopper.  */
  LTO_cgraph_avail_node = 1,
  LTO_cgraph_overwritable_node,
  LTO_cgraph_unavail_node,
  LTO_cgraph_edge,
  LTO_cgraph_last_tag
};

/* The is the first part of the cgraph record in the .o file.  */
struct lto_cgraph_header
{
  struct lto_header lto_header;   /* The header for all types of sections. */
  int32_t main_size;              /* Size of main gimple body of function.  */
  int32_t debug_main_size;        /* Size of main stream debugging information.  */
  int32_t compressed_size;
};

#ifdef LTO_STREAM_DEBUGGING
extern const char * LTO_cgraph_tag_names[LTO_cgraph_last_tag];
#endif

#endif /* GCC_LTO_CGRAPH_H */
