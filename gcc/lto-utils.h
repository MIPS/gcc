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

#define LTO_DECL_FLAG_DEFINED		(1<<0)
#define LTO_DECL_FLAG_SUPPRESS_OUTPUT	(1<<1)

typedef unsigned char	lto_decl_flags_t;

/* In lto-utils.c.  */
extern bitmap lto_bitmap_alloc (void);
extern void lto_bitmap_free (bitmap);
extern lto_decl_flags_t lto_get_decl_flags (tree);
extern void lto_set_decl_flags (tree, lto_decl_flags_t);
extern char *lto_get_section_name (int, const char *);
extern void print_lto_report (void);

/* Return true if LABEL should be emitted in the global context.  */
static inline bool
emit_label_in_global_context_p (tree label)
{
  return DECL_NONLOCAL (label) || FORCED_LABEL (label);
}

#endif /* !GCC_LTO_UTILS_H */
