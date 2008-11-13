/* Miscellaneous utilies for LTO.

   Copyright (C) 2008 Free Software Foundation, Inc.
   Contributed by Doug Kwan <dougkwan@google.com>

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

#ifndef GCC_LTO_UTILS_H
#define GCC_LTO_UITLS_H

bitmap lto_bitmap_alloc (void);

#define LTO_VAR_FLAG_DEFINED		(1<<0)
#define LTO_VAR_FLAG_SUPPRESS_OUTPUT	(1<<1)
#define LTO_VAR_FLAG_FORCE_GLOBAL	(1<<2)

typedef unsigned char	lto_var_flags_t;

void lto_bitmap_free (bitmap);
lto_var_flags_t lto_get_var_flags (tree);
void lto_set_var_flags (tree, lto_var_flags_t);
char* lto_get_section_name (int, const char *);

#endif /* !GCC_LTO_UTILS_H */
