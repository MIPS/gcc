/* LTO declarations.
   Copyright 2006 Free Software Foundation, Inc.
   Contributed by CodeSourcery, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#ifndef LTO_H
#define LTO_H

/* An input file.  */
typedef struct lto_file
{
  /* The name of the file.  */
  const char *filename;
  /* The contents of the .debug_info section.  */
  const void *debug_info;
  /* The number of bytes pointed to by DEBUG_INFO.  */ 
  size_t debug_info_length;
  /* The contents of the .debug_abbrev section.  */
  const void *debug_abbrev;
  /* The number of bytes pointed to by DEBUG_ABBREV.  */
  size_t debug_abbrev_length;
} lto_file;

/* lto.c */
 
/* Read all of the input object files, generate a TREE representation
   of the combined program, and provide that combined representation
   to the middle end.  */
extern void lto_main (int debug_p);

/* Initialize the newly allocated FILE, which corresponds to
   FILENAME.  */
extern void lto_file_init (lto_file *file, const char *filename);

/* Generate a TREE representation for all entities in FILE.  If an
   entity in FILE has already been read (from another object file),
   merge the two entities.  Returns TRUE iff FILE was successfully
   processed.  */
extern bool lto_file_read (lto_file *file);

/* lto-elf.c */

/* Open the ELF input file indicated by FILENAME.  Return */
extern lto_file *lto_elf_file_open (const char *filename);

/* Close an ELF input file.  */
extern void lto_elf_file_close (lto_file *file);

#endif /* LTO_H */
