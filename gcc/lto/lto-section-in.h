/* LTO input code.
   Copyright (C) 2007, 2008 Free Software Foundation, Inc.
   Contributed by Kenneth Zadeck <zadeck@naturalbridge.com>

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

#ifndef GCC_LTO_SECTION_IN_H
#define GCC_LTO_SECTION_IN_H

struct lto_input_block 
{
  const char *data;
  unsigned int p;
  unsigned int len;
};

unsigned char lto_input_1_unsigned (struct lto_input_block *);
unsigned HOST_WIDE_INT lto_input_uleb128 (struct lto_input_block *);
unsigned HOST_WIDEST_INT lto_input_widest_uint_uleb128 (struct lto_input_block *);
HOST_WIDE_INT lto_input_sleb128 (struct lto_input_block *);
tree lto_input_integer (struct lto_input_block *, tree);

#ifdef LTO_STREAM_DEBUGGING
void lto_debug_in_fun (struct lto_debug_context *, char);
void dump_debug_stream (struct lto_input_block *, char, char);
#endif

#endif  /* GCC_LTO_SECTION_IN_H  */
