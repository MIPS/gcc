/* Top-level LTO routines.
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

#include <sys/mman.h>
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "opts.h"
#include "toplev.h"
#include "tree.h"
#include "tm.h"
#include "cgraph.h"
#include "ggc.h"
#include "tree-ssa-operands.h"
#include "tree-pass.h"
#include "langhooks.h"
#include "lto.h"
#include "lto-tree.h"
#include "lto-section.h"
#include "lto-section-in.h"
#include "lto-section-out.h"
#include "lto-tree-in.h"
#include "lto-tags.h"

/* Read the constructors and inits.  */

static void
lto_materialize_constructors_and_inits (struct lto_file_decl_data * file_data)
{
  size_t len;
  const char *data = lto_get_section_data (file_data, 
					   LTO_section_static_initializer, NULL, &len);
  lto_input_constructors_and_inits (file_data, data);
  lto_free_section_data (file_data, LTO_section_static_initializer, NULL, data, len);
}

/* Read the function body for the function associated with NODE if possible.  */

static void
lto_materialize_function (struct cgraph_node *node)
{
  tree decl = node->decl;
  struct lto_file_decl_data *file_data = node->local.lto_file_data;
  const char *data;
  size_t len;
  tree step;
  const char *name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (decl)); 

  /* We may have renamed the declaration, e.g., a static function.  */
  name = lto_original_decl_name (file_data, name);

  data = lto_get_section_data (file_data, LTO_section_function_body,
			       name, &len);
  if (data)
    {
      struct function *fn;

      /* This function has a definition.  */
      TREE_STATIC (decl) = 1;
      DECL_EXTERNAL (decl) = 0;

      allocate_struct_function (decl, false);
      lto_input_function_body (file_data, decl, data);
      fn = DECL_STRUCT_FUNCTION (decl);
      lto_free_section_data (file_data, LTO_section_function_body, name, data, len);

      /* Look for initializers of constant variables and private
	 statics.  */
      for (step = fn->local_decls;
	   step;
	   step = TREE_CHAIN (step))
	{
	  tree decl = TREE_VALUE (step);
	  if (TREE_CODE (decl) == VAR_DECL
	      && (TREE_STATIC (decl) && !DECL_EXTERNAL (decl))
	      && flag_unit_at_a_time)
	    varpool_finalize_decl (decl);
	}
    }
  else
    DECL_EXTERNAL (decl) = 1;

  /* Let the middle end know about the function.  */
  rest_of_decl_compilation (decl,
                            /*top_level=*/1,
                            /*at_end=*/0);
  if (cgraph_node (decl)->needed)
    cgraph_mark_reachable_node (cgraph_node (decl));
}


/* Initialize the globals vector with pointers to well-known trees.  */

static void
preload_common_nodes (struct data_in *data_in)
{
  unsigned i;
  htab_t index_table;

  /* The global tree for the main identifier is filled in by language-specific
     front-end initialization that is not run in the LTO back-end.  It appears
     that all languages that perform such initialization currently do so in the
     same way, so we do it here.  */
  if (!main_identifier_node)
    main_identifier_node = get_identifier ("main");

  ptrdiff_type_node = integer_type_node;

  /* FIXME lto.  In the C++ front-end, fileptr_type_node is defined as a
     variant copy of of ptr_type_node, rather than ptr_node itself.  The
     distinction should only be relevant to the front-end, so we always
     use the C definition here in lto1.  */
  gcc_assert (fileptr_type_node == ptr_type_node);

  index_table = htab_create (37, lto_hash_global_slot_node,
			     lto_eq_global_slot_node, free);

#ifdef GLOBAL_STREAMER_TRACE
  fprintf (stderr, "\n\nPreloading all global_trees[]\n");
#endif

  for (i = 0; i < TI_MAX; i++)
    preload_common_node (global_trees[i], index_table, &data_in->globals_index,
			 NULL);

#ifdef GLOBAL_STREAMER_TRACE
  fprintf (stderr, "\n\nPreloaded %u entries in global_trees[]\n", i - 1);
#endif

#ifdef GLOBAL_STREAMER_TRACE
  fprintf (stderr, "\n\nPreloading all integer_types[]\n");
#endif

  for (i = 0; i < itk_none; i++)
    preload_common_node (integer_types[i], index_table, &data_in->globals_index,
			 NULL);

#ifdef GLOBAL_STREAMER_TRACE
  fprintf (stderr, "\n\nPreloaded %u entries in integer_types[]\n", i - 1);
#endif

  htab_delete (index_table);
}

/* Load in the global vars and all of the types from the main symbol
   table.  */

static void
lto_read_decls (struct lto_file_decl_data *decl_data, const void *data)
{
  const struct lto_decl_header * header 
      = (const struct lto_decl_header *) data;

  int32_t types_offset = sizeof (struct lto_decl_header); 
  int32_t fields_offset
      = types_offset + (header->num_types * sizeof (uint32_t));
  int32_t fns_offset 
      = fields_offset + (header->num_field_decls * sizeof (uint32_t));
  int32_t vars_offset 
      = fns_offset + (header->num_fn_decls * sizeof (uint32_t));
  int32_t type_decls_offset 
      = vars_offset + (header->num_var_decls * sizeof (uint32_t));
  int32_t namespace_decls_offset
      = type_decls_offset + (header->num_type_decls * sizeof (uint32_t));

  const uint32_t *in_types
    = (const uint32_t *) ((const char *) data + types_offset);
  const uint32_t *in_field_decls
    = (const uint32_t *) ((const char *) data + fields_offset);
  const uint32_t *in_fn_decls
    = (const uint32_t *) ((const char *) data + fns_offset);
  const uint32_t *in_var_decls
    = (const uint32_t *) ((const char *) data + vars_offset);
  const uint32_t *in_type_decls
    = (const uint32_t *) ((const char *) data + type_decls_offset);
  const uint32_t *in_namespace_decls
    = (const uint32_t *) ((const char *) data + namespace_decls_offset);

  int32_t main_offset
      = namespace_decls_offset + (header->num_namespace_decls * sizeof (uint32_t));
  const int32_t string_offset = main_offset + header->main_size;
#ifdef LTO_STREAM_DEBUGGING
  const int32_t debug_main_offset = string_offset + header->string_size;
#endif

  struct lto_input_block ib_main;
  struct lto_input_block debug_main;
  struct data_in data_in;
  int i;

  LTO_INIT_INPUT_BLOCK (ib_main, (const char*) data + main_offset, 0, header->main_size);
#ifdef LTO_STREAM_DEBUGGING
  LTO_INIT_INPUT_BLOCK (debug_main, (const char*) data + debug_main_offset, 0, header->debug_main_size);
#endif

  decl_data->field_decls = (tree *) xcalloc (header->num_field_decls, sizeof (tree));
  decl_data->fn_decls    = (tree *) xcalloc (header->num_fn_decls, sizeof (tree));
  decl_data->var_decls   = (tree *) xcalloc (header->num_var_decls, sizeof (tree));
  decl_data->type_decls  = (tree *) xcalloc (header->num_type_decls, sizeof (tree));
  decl_data->namespace_decls = (tree *) xcalloc (header->num_namespace_decls, sizeof (tree));
  decl_data->types       = (tree *) xcalloc (header->num_types, sizeof (tree));

  memset (&data_in, 0, sizeof (struct data_in));
  data_in.file_data          = decl_data;
  data_in.strings            = (const char *) data + string_offset;
  data_in.strings_len        = header->string_size;
  data_in.globals_index	     = NULL;

  /* FIXME: This doesn't belong here.
     Need initialization not done in lto_static_init ().  */
  lto_static_init_local ();

#ifdef LTO_STREAM_DEBUGGING
  lto_debug_context.out = lto_debug_in_fun;
  lto_debug_context.indent = 0;
  lto_debug_context.tag_names = LTO_tree_tag_names;
  lto_debug_context.current_data = &debug_main;
#endif

  /* Preload references to well-known trees.  */
  preload_common_nodes (&data_in);

  /* Read the global declarations and types.  */
  /* FIXME: We should be a bit more graceful regarding truncated files. */
  while (ib_main.p < ib_main.len)
    {
      tree dummy;

      input_tree (&dummy, &ib_main, &data_in);
      gcc_assert (ib_main.p <= ib_main.len);
    }

  /* Construct index vectors for use when reading function bodies.  */

  for (i=0; i<header->num_types; i++)
    decl_data->types[i]
        = VEC_index (tree, data_in.globals_index, in_types[i]);
  decl_data->num_types = header->num_types;

  for (i=0; i<header->num_field_decls; i++)
    decl_data->field_decls[i]
        = VEC_index (tree, data_in.globals_index, in_field_decls[i]);
  decl_data->num_field_decls = header->num_field_decls;

  for (i=0; i<header->num_fn_decls; i++)
    decl_data->fn_decls[i]
        = VEC_index (tree, data_in.globals_index, in_fn_decls[i]);
  decl_data->num_fn_decls = header->num_fn_decls;

  for (i=0; i<header->num_var_decls; i++)
    decl_data->var_decls[i]
        = VEC_index (tree, data_in.globals_index, in_var_decls[i]);
  decl_data->num_var_decls = header->num_var_decls;

  for (i=0; i<header->num_type_decls; i++)
    decl_data->type_decls[i]
        = VEC_index (tree, data_in.globals_index, in_type_decls[i]);
  decl_data->num_type_decls = header->num_type_decls;

  for (i=0; i<header->num_namespace_decls; i++)
    decl_data->namespace_decls[i]
        = VEC_index (tree, data_in.globals_index, in_namespace_decls[i]);
  decl_data->num_namespace_decls = header->num_namespace_decls;

  /* The globals index vector is needed only while reading.  */

  VEC_free (tree, heap, data_in.globals_index);
}

/* Generate a TREE representation for all types and external decls
   entities in FILE.  If an entity in FILE has already been read (from
   another object file), merge the two entities.  Returns the
   file_data from the last context.

   FIXME, this is a bug that will go away with Maddox's streaming
   merge since there will no longer be contexts.

   Read all of the globals out of the file.  Then read the cgraph
   and process the .o index into the cgraph nodes so that it can open
   the .o file to load the functions and ipa information.   */

static struct lto_file_decl_data*
lto_file_read (lto_file *file)
{
  struct lto_file_decl_data* file_data;
  const char *data;
  size_t len;
  htab_t section_hash_table;
  htab_t renaming_hash_table;

  file_data = XNEW (struct lto_file_decl_data);

  file_data->file_name = file->filename;
  file_data->fd = -1;

  section_hash_table = lto_elf_build_section_table (file);
  file_data->section_hash_table = section_hash_table;

  renaming_hash_table = lto_create_renaming_table ();
  file_data->renaming_hash_table = renaming_hash_table;
  
  data = lto_get_section_data (file_data, LTO_section_decls, NULL, &len);
  lto_read_decls (file_data, data);
  lto_free_section_data (file_data, LTO_section_decls, NULL, data, len);

  /* FIXME: We never free file_data.  */

  return file_data;
}

/****************************************************************************
  Input routines for reading sections from .o files.

  FIXME: These routines may need to be generalized.  They assume that
  the .o file can be read into memory and the secions just mapped.
  This may not be true if the .o file is in some form of archive.
****************************************************************************/

/* Page size of machine is used for mmap and munmap calls.  */
static size_t page_mask;

/* Get the section data of length LEN from FILENAME starting at
   OFFSET.  The data segment must be freed by the caller when the
   caller is finished.  Returns NULL if all was not well.  */

static char *
lto_read_section_data (struct lto_file_decl_data *file_data,
		       intptr_t offset, size_t len)
{
  char *result;
  intptr_t computed_len;
  intptr_t computed_offset;
  intptr_t diff;

  if (!page_mask)
    {
      size_t page_size = sysconf (_SC_PAGE_SIZE);
      page_mask = ~(page_size - 1);
    }

  if (file_data->fd == -1)
    file_data->fd = open (file_data->file_name, O_RDONLY);

  if (file_data->fd == -1)
    return NULL;

  computed_offset = offset & page_mask;
  diff = offset - computed_offset;
  computed_len = len + diff;

  result = (char *) mmap (NULL, computed_len, PROT_READ, MAP_PRIVATE,
			  file_data->fd, computed_offset);
  if (result == MAP_FAILED)
    {
      close (file_data->fd);
      return NULL;
    }

  return result + diff;
}    


/* Get the section data from FILE_DATA of SECTION_TYPE with NAME.
   NAME will be null unless the section type is for a function
   body.  */

static const char *
get_section_data (struct lto_file_decl_data *file_data,
		      enum lto_section_type section_type,
		      const char *name,
		      size_t *len)
{
  htab_t section_hash_table = file_data->section_hash_table;
  struct lto_section_slot *f_slot;
  struct lto_section_slot s_slot;
  const char *section_name = lto_get_section_name (section_type, name);
  char * data = NULL;

  s_slot.name = section_name;
  f_slot = (struct lto_section_slot *)htab_find (section_hash_table, &s_slot);
  if (f_slot)
    {
      data = lto_read_section_data (file_data, f_slot->start, f_slot->len);
      *len = f_slot->len;
    }

  free (CONST_CAST (char *, section_name));
  return data;
}


/* Free the section data from FILE_DATA of SECTION_TYPE with NAME that
   starts at OFFSET and has LEN bytes.  */

static void
free_section_data (struct lto_file_decl_data *file_data,
		       enum lto_section_type section_type ATTRIBUTE_UNUSED,
		       const char *name ATTRIBUTE_UNUSED,
		       const char *offset, size_t len)
{
  intptr_t computed_len;
  intptr_t computed_offset;
  intptr_t diff;

  if (file_data->fd == -1)
    return;

  computed_offset = ((intptr_t)offset) & page_mask;
  diff = (intptr_t)offset - computed_offset;
  computed_len = len + diff;

  munmap ((void *)computed_offset, computed_len);
}


/* Needed so the garbage collector knows to root around in functions we
   have not yet materialized and the huge DIE -> tree table we keep
   around.  */
static GTY(()) lto_file *current_lto_file;

void
lto_main (int debug_p ATTRIBUTE_UNUSED)
{
  unsigned int i;
  unsigned int j = 0;
  tree decl;
  struct cgraph_node *node; 
  struct lto_file_decl_data** all_file_decl_data 
    = XNEWVEC (struct lto_file_decl_data*, num_in_fnames + 1);
  struct lto_file_decl_data* file_data = NULL;

  /* Set the hooks so that all of the ipa passes can read in their data.  */
  lto_set_in_hooks (all_file_decl_data, get_section_data,
		    free_section_data);

  /* Read all of the object files specified on the command line.  */
  for (i = 0; i < num_in_fnames; ++i)
    {
      current_lto_file = lto_elf_file_open (in_fnames[i], /*writable=*/false);
      if (!current_lto_file)
	break;
      file_data = lto_file_read (current_lto_file);
      if (!file_data)
	break;

      all_file_decl_data [j++] = file_data;

      lto_elf_file_close (current_lto_file);
      current_lto_file = NULL;
    }

  all_file_decl_data [j] = NULL;

  /* Set the hooks so that all of the ipa passes can read in their data.  */
  lto_set_in_hooks (all_file_decl_data, get_section_data,
		    free_section_data);

  /* FIXME!!! This loop needs to be changed to use the pass manager to
     call the ipa passes directly.  */
  for (i = 0; i < j; i++)
    {
      struct lto_file_decl_data* file_data = all_file_decl_data [i];

      lto_materialize_constructors_and_inits (file_data);
    }

  ipa_read_summaries ();

  /* Now that we have input the cgraph, we need to clear all of the aux
     nodes and read the functions.  

     FIXME!!!!! This loop obviously leaves a lot to be desired:
     1) it loads all of the functions at once.  
     2) it closes and reopens the files over and over again. 

     It would obviously be better for the cgraph code to look to load
     a batch of functions and sort those functions by the file they
     come from and then load all of the functions from a give .o file
     at one time.  This of course will require that the open and close
     code be pulled out of lto_materialize_function, but that is a
     small part of what will be a complex set of management
     issues.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      /* FIXME!!!  There really needs to be some check to see if the
	 function is really not external here.  Currently the only
	 check is to see if the section was defined in the file_data
	 index.  There is of course the value in the node->aux field
	 that is nulled out in the previous line, but we should really
	 be able to look at the cgraph info at the is point and make
	 the proper determination.   Honza will fix this.  */
      lto_materialize_function (node);
    }

  /* Inform the middle end about the global variables we have seen.  */
  for (i = 0; VEC_iterate (tree, lto_global_var_decls, i, decl); i++)
    rest_of_decl_compilation (decl,
                              /*top_level=*/1,
                              /*at_end=*/0);

  /* This is some bogus wrapper code for development testing.  It will be
     replaced once some basic WPA partitioning logic is implemented.  To use
     this pass "-flto -fsyntax-only" to the lto1 invocation.  */
  if (flag_generate_lto)
    {
      lto_file *file;

      file = lto_elf_file_open ("bogus.lto.o", /*writable=*/true);
      if (!file)
	fatal_error ("lto_elf_file_open() failed");
      lto_set_current_out_file (file);
    }

  /* Let the middle end know that we have read and merged all of the
     input files.  */ 
  /*cgraph_finalize_compilation_unit ();*/
  cgraph_optimize ();

  /* This is the continuation of the previous bogus wrapper code.  It will be
     replaced once some basic WPA partitioning logic is implemented.  */
  if (flag_generate_lto)
    {
      lto_file *file;

      file = lto_set_current_out_file (NULL);
      lto_elf_file_close (file);
    }
}

#include "gt-lto-lto.h"
