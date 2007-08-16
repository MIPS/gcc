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
#include <inttypes.h>

/* Forward Declarations */

typedef struct lto_file lto_file;
typedef struct DWARF2_abbrev DWARF2_abbrev;
typedef struct DWARF2_CompUnit DWARF2_CompUnit;
typedef struct lto_context lto_context;

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
  /* A map from DIEs to trees.  The keys are offsets into the DWARF
     information section; the values are trees.  */
  htab_t die_cache;
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

/* The virtual function table for an lto_file.  */
typedef struct lto_file_vtable
{
  /* Return the address of the function-body data for the function
     named FN, or NULL if the data is not available.  */
  const void *(*map_fn_body)(lto_file *file, const char *fn);
  /* DATA is the non-NULL address returned by a previous call to
     MAP_FN_BODY, with the same value of FN.  Release any resources
     allocated by MAP_FN_BODY.  */
  void (*unmap_fn_body)(lto_file *file, const char *fn, const void *data);
  /* Return the address of the variable-initializer data for the function
     named VAR, or NULL if the data is not available.  */
  const void *(*map_var_init)(lto_file *file, const char *var);
  /* DATA is the non-NULL address returned by a previous call to
     MAP_VAR_INIT, with the same value of VAR.  Release any resources
     allocated by MAP_VAR_INIT.  */
  void (*unmap_var_init)(lto_file *file, const char *var, const void *data);
} lto_file_vtable;

/* An input file.  */
struct lto_file
{
  /* Virtual functions implemented by the derived file class.  */
  const lto_file_vtable *vtable;
  /* The name of the file.  */
  const char *filename;
  /* The contents of the .debug_info section.  */
  lto_info_fd debug_info;
  /* The contents of the .debug_abbrev section.  */
  lto_abbrev_fd debug_abbrev;
};

/* A reference to a global entity (type, variable, or function).  */
typedef struct lto_ref
{
  /* The DWARF compilation unit containing the entity.  */
  uint64_t section;
  /* The offset of the DIE corresponding to the entity.  */
  uint64_t offset;
} lto_ref;

/* lto.c */
 
/* Read all of the input object files, generate a TREE representation
   of the combined program, and provide that combined representation
   to the middle end.  */
extern void lto_main (int debug_p);

/* Initialize the newly allocated FILE, which corresponds to
   FILENAME.  VTABLE is the virtual table for FILE.  */
extern void lto_file_init (lto_file *file, 
			   const lto_file_vtable *vtable,
			   const char *filename);

/* Free resources associated with FILE.  FILE itself will be
   deallocated by this function.  */
extern void lto_file_close (lto_file *file);

/* Generate a TREE representation for all entities in FILE.  If an
   entity in FILE has already been read (from another object file),
   merge the two entities.  Returns TRUE iff FILE was successfully
   processed.  */
extern bool lto_file_read (lto_file *file);

/* Return the TYPE referred to by REF.  */
extern tree lto_resolve_type_ref (lto_info_fd *info_fd,
				  lto_context *context,
				  const lto_ref *ref);

/* Return the VAR_DECL referred to by REF.  */
extern tree lto_resolve_var_ref (lto_info_fd *info_fd,
				 lto_context *context,
				 const lto_ref *ref);

/* Return the FUNCTION_DECL referred to by REF.  */
extern tree lto_resolve_fn_ref (lto_info_fd *info_fd,
				lto_context *context,
				const lto_ref *ref);

/* Return the FIELD_DECL referred to by REF.  */
extern tree lto_resolve_field_ref (lto_info_fd *info_fd,
				lto_context *context,
				const lto_ref *ref);

/* lto-elf.c */

/* Open the ELF input file indicated by FILENAME.  Return */
extern lto_file *lto_elf_file_open (const char *filename);

/* Close an ELF input file.  */
extern void lto_elf_file_close (lto_file *file);

/* lto-read.c */

/* FN is a FUNCTION_DECL.  DATA is the LTO data written out during
   ordinary compilation, encoding the body of FN.  FD and CONTEXT may
   be passed back to the lto_resolve__ref functions to retrieve
   information about glogal entities.  Upon return, DECL_SAVED_TREE
   for FN contains the reconstituted body of FN and DECL_INITIAL
   contains the BLOCK tree for the function.  However, it is not this
   function's responsibility to provide FN to the optimizers or
   code-generators; that will be done by the caller.  */
extern void
lto_read_function_body (lto_info_fd *fd,
			lto_context *context,
			tree fn,
			const void *data);

/* VAR is a VAR_DECL.  DATA is the LTO data written out during
   ordinary compilation, encoding the initializer for VAR.  FD and
   CONTEXT are as for lto_read_function_body.  Upon return,
   DECL_INITIAL for VAR contains the reconsitituted initializer for
   VAR.  However, it is not this function's responsibility to provide
   VAR to the optimizers or code-generators; that will be done by the
   caller.  */
extern void
lto_read_var_init (lto_info_fd *fd,
		   lto_context *context,
		   tree var,
		   const void *data);

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

#endif /* LTO_H */
