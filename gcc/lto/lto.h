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

/* An input file.  */
typedef struct lto_file_struct GTY(())
{
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

/* lto-symtab.c */

/* The NEW_VAR (a VAR_DECL) has just been read.  If there is an
   existing variable with the same name, merge the declaration for
   NEW_VAR with the previous declaration and return the previous
   declaration.  In this case, NEW_VAR must no longer be used by the
   caller.  All other entities referenced from NEW_VAR (including, in
   particular, its type) must already have been merged before this
   function is called.  If the merge fails (due to inconsistencies
   between the declarations), an error message is issued, and
   error_mark_node is returned.  If there is no previous declaration,
   NEW_VAR is returned.  */
extern tree lto_symtab_merge_var (tree new_var);

/* Like lto_symtab_merge_var, but for functions.  */
extern tree lto_symtab_merge_fn (tree new_fn);

/* Hash table entry to hold the start offset and length of an lto
   section in a .o file.  */

struct lto_section_slot {
  const char * name;
  intptr_t start;
  size_t len;
};


#endif /* LTO_H */
