/* Declarations of the section header.

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

#ifndef GCC_LTO_HEADER_H
#define GCC_LTO_HEADER_H

/* The string that is the prefix on the section names we make for lto.
   For decls the DECL_ASSEMBLER_NAME is appended to make the section
   name for the functions and static_initializers.  For other types of
   sections a '.' and the section type are appended.  */
#define LTO_SECTION_NAME_PREFIX         ".gnu.lto_"

#define LTO_major_version 1
#define LTO_minor_version 0

enum lto_section_type
{
  lto_function_body,
  lto_static_initializer,
  lto_cgraph
};

struct lto_header
{
  int16_t major_version;          /* LTO_major_version. */
  int16_t minor_version;          /* LTO_minor_version. */
  enum lto_section_type section_type;
};

#endif /* lto-header.h */
