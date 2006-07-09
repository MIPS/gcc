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

/* Forward Declarations */

typedef struct lto_file lto_file;
typedef struct DWARF2_abbrev DWARF2_abbrev;
typedef struct DWARF2_CompUnit DWARF2_CompUnit;

/* Types */

/* A file descriptor for reading from a particular DWARF section.  */
typedef struct lto_fd
{
  /* The name of this section.  */
  const char *name;
  /* The first byte of the section.  */ 
  const char *start;
  /* The byte just past the end of the section.  */
  const char *end;
  /* The next available byte.  */
  const char *cur;
  /* The lto_file with which this section is associated.  */
  lto_file *file;
  /* True if using 64-bit DWARF.  */
  bool dwarf64;
} lto_fd;

/* A file descriptor for reading from a DWARF information section. */
typedef struct lto_info_fd
{
  /* The base object.  */
  lto_fd base;
  /* The number of compilation units in this section.  */
  size_t num_units;
  /* The compilation units themselves.  */
  DWARF2_CompUnit **units;
} lto_info_fd;

/* A file descriptor for reading from a DWARF abbreviation section.  */
typedef struct lto_abbrev_fd
{
  /* The base object.  */
  lto_fd base;
  /* The number of abbreviations in this section.  */
  size_t num_abbrevs;
  /* The abbreviations themselves.  */
  DWARF2_abbrev **abbrevs;
} lto_abbrev_fd;

/* An input file.  */
struct lto_file
{
  /* The name of the file.  */
  const char *filename;
  /* The contents of the .debug_info section.  */
  lto_info_fd debug_info;
  /* The contents of the .debug_abbrev section.  */
  lto_abbrev_fd debug_abbrev;
};

/* lto.c */
 
/* Read all of the input object files, generate a TREE representation
   of the combined program, and provide that combined representation
   to the middle end.  */
extern void lto_main (int debug_p);

/* Initialize the newly allocated FILE, which corresponds to
   FILENAME.  */
extern void lto_file_init (lto_file *file, const char *filename);

/* Free resources associated with FILE.  FILE itself will be
   deallocated by this function.  */
extern void lto_file_close (lto_file *file);

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
