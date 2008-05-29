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

/* Included files.  */

#include "hashtab.h"
#include "tree.h"
#include "vec.h"
#include <inttypes.h>
#include "lto-header.h"
#include "lto-section-in.h"

/* Forward Declarations */
struct lto_file_struct;

/* The virtual function table for an lto_file.  */
typedef struct lto_file_vtable_struct GTY(())
{
  /* Return the address of the data in an lto section.
     LTO_SECTION_TYPE specifies the type of the section.  If this is a
     function or static initializer use FN, Returns the pointer to the
     data or NULL if the data is not available.  */
  const void *(*map_section)(struct lto_file_struct *file, enum lto_section_type, const char *fn);
  /* DATA is the non-NULL address returned by a previous call to
     MAP_SECTION, with the same value of FN.  Release any resources
     allocated by MAP_FN_BODY.  */
  void (*unmap_section)(struct lto_file_struct *file, const char *fn, const void *data);
}
lto_file_vtable;

/* An input file.  */
typedef struct lto_file_struct GTY(())
{
  /* Virtual functions implemented by the derived file class.  */
  const lto_file_vtable * GTY((skip)) vtable;
  /* The name of the file.  */
  const char *filename;

  /* ### */
  /* The debug_info and debug_abbrev fields are no longer used.
     As a result, the lto_file vs. lto_elf_file distinction is
     no longer useful, and should be eliminated.  */
}
lto_file;


/* lto.c */
 
/* Read all of the input object files, generate a TREE representation
   of the combined program, and provide that combined representation
   to the middle end.  */
extern void lto_main (int debug_p);


/* lto-elf.c */

/* Open the ELF input file indicated by FILENAME.  Return */
extern lto_file *lto_elf_file_open (const char *filename);

/* Close an ELF input file.  */
extern void lto_elf_file_close (lto_file *file);

/* Build and index of all lto sections in an elf file.  */
extern htab_t lto_elf_build_section_table (lto_file *file);

#endif /* LTO_H */
