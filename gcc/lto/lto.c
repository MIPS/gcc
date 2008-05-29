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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "opts.h"
#include "toplev.h"
#include "dwarf2.h"
#include "dwarf2out.h"          /* For dwarf_attr_name.  */
#include "tree.h"
#include "tm.h"
#include "cgraph.h"
#include "ggc.h"
#include "lto.h"
#include "lto-tree.h"
#include "tree-ssa-operands.h"  /* For init_ssa_operands.  */
#include "langhooks.h"
#include "lto-section.h"
#include "lto-section-in.h"
#include "lto-tree-in.h"
#include "lto-tags.h"  		/* For LTO_tree_tag_names.  */


/* Read the constructors and inits.  */

static void
lto_materialize_constructors_and_inits (struct lto_file_decl_data * file_data)
{
  const char *data = lto_get_section_data (file_data, LTO_section_static_initializer, NULL);
  lto_input_constructors_and_inits (file_data, data);
  free ((char *)data);
}

/* Read the function body for the function associated with NODE if possible.  */

static void
lto_materialize_function (struct cgraph_node *node)
{
  tree decl = node->decl;
  struct lto_file_decl_data *file_data = node->local.lto_file_data;
  const char *data;
  tree step;

  data = lto_get_section_data (file_data, LTO_section_function_body,
			       IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (decl)));
  if (data)
    {
      struct function *fn;

      /* This function has a definition.  */
      TREE_STATIC (decl) = 1;
      DECL_EXTERNAL (decl) = 0;

      allocate_struct_function (decl, false);
      lto_input_function_body (file_data, decl, data);
      fn = DECL_STRUCT_FUNCTION (decl);
      free ((char *)data);

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
    /* ### Shouldn't we just check this and assert if not?  */
    DECL_EXTERNAL (decl) = 1;

  /* Let the middle end know about the function.  */
  rest_of_decl_compilation (decl,
                            /*top_level=*/1,
                            /*at_end=*/0);
  if (cgraph_node (decl)->needed)
    cgraph_mark_reachable_node (cgraph_node (decl));
}

/* Read the cgraph for this file.  */

static void
lto_materialize_cgraph (struct lto_file_decl_data * file_data)
{
  const char *data = lto_get_section_data (file_data, LTO_section_cgraph, NULL);
  lto_input_cgraph (file_data, data);
  free ((char *)data);
}

/* ### */
/* Initialize the globals vector with pointers to well-known trees.  */

static void
preload_common_nodes (struct data_in *data_in)
{
  unsigned i;

  for (i = 0; i < TI_MAX; i++)
    VEC_safe_push (tree, heap, data_in->globals_index, global_trees[i]);
  for (i = 0; i < itk_none; i++)
    VEC_safe_push (tree, heap, data_in->globals_index, integer_types[i]);
}

/* ### */
/* Load in the global vars and all of the types from the main symbol
   table.  */

static void
lto_read_decls (struct lto_file_decl_data *decl_data, const void *data)
{
  struct lto_decl_header * header 
      = (struct lto_decl_header *) data;

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

  uint32_t *in_types       = (uint32_t*)(data + types_offset);
  uint32_t *in_field_decls = (uint32_t*)(data + fields_offset);
  uint32_t *in_fn_decls    = (uint32_t*)(data + fns_offset);
  uint32_t *in_var_decls   = (uint32_t*)(data + vars_offset);
  uint32_t *in_type_decls  = (uint32_t*)(data + type_decls_offset);
  uint32_t *in_namespace_decls  = (uint32_t*)(data + namespace_decls_offset);

  int32_t main_offset
      = namespace_decls_offset + (header->num_namespace_decls * sizeof (uint32_t));
  int32_t string_offset = main_offset + header->main_size;
#ifdef LTO_STREAM_DEBUGGING
  int32_t debug_main_offset = string_offset + header->string_size;
#endif

  struct lto_input_block ib_main;
  struct lto_input_block debug_main;
  struct data_in data_in;
  int i;

  LTO_INIT_INPUT_BLOCK (ib_main, data + main_offset, 0, header->main_size);
  LTO_INIT_INPUT_BLOCK (debug_main, data + debug_main_offset, 0, header->debug_main_size);

  decl_data->field_decls = xcalloc (header->num_field_decls, sizeof (tree));
  decl_data->fn_decls    = xcalloc (header->num_fn_decls, sizeof (tree));
  decl_data->var_decls   = xcalloc (header->num_var_decls, sizeof (tree));
  decl_data->type_decls  = xcalloc (header->num_type_decls, sizeof (tree));
  decl_data->namespace_decls = xcalloc (header->num_namespace_decls, sizeof (tree));
  decl_data->types       = xcalloc (header->num_types, sizeof (tree));

  memset (&data_in, 0, sizeof (struct data_in));
  data_in.file_data          = decl_data;
  data_in.strings            = data + string_offset;
  data_in.strings_len        = header->string_size;
  data_in.globals_index	     = NULL;
  data_in.global	     = true;  /* ### unused */

  /* ### This doesn't belong here */
  /* ### Need initialization not done in lto_static_init () */
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
  /* ### We should be a bit more graceful regarding truncated files. */
  while (ib_main.p < ib_main.len)
    {
      input_tree (&ib_main, &data_in);
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
   another object file), merge the two entities.  Returns TRUE iff
   FILE was successfully processed.

   Read all of the globals out of the file.  Then read the cgraph
   and process the .o index into the cgraph nodes so that it can open
   the .o file to load the functions and ipa information.   */

static bool
lto_file_read (lto_file *file)
{
  struct lto_file_decl_data* file_data;
  char *data;

  file_data = xmalloc (sizeof (struct lto_file_decl_data));

  file_data->file_name = file->filename;
  file_data->section_hash_table = lto_elf_build_section_table (file);
  
  data = lto_get_section_data (file_data, LTO_section_decls, NULL);

  lto_read_decls (file_data, data);

  free ((char *)data);

  lto_materialize_constructors_and_inits (file_data);

  lto_materialize_cgraph (file_data);

  /* ### We never free file_data.  */

  return true;
}

/* Needed so the garbage collector knows to root around in functions we
   have not yet materialized and the huge DIE -> tree table we keep
   around.  */
static GTY(()) lto_file *current_lto_file;

void
lto_main (int debug_p ATTRIBUTE_UNUSED)
{
  unsigned i;
  tree decl;
  struct cgraph_node *node; 

  /* Read all of the object files specified on the command line.  */
  for (i = 0; i < num_in_fnames; ++i)
    {
      current_lto_file = lto_elf_file_open (in_fnames[i]);
      if (!current_lto_file)
	break;
      if (!lto_file_read (current_lto_file))
	break;
      lto_elf_file_close (current_lto_file);
      current_lto_file = NULL;
    }

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
      node->aux = NULL;
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

  /* Let the middle end know that we have read and merged all of the
     input files.  */ 
  /*cgraph_finalize_compilation_unit ();*/
  cgraph_optimize ();
}

#include "gt-lto-lto.h"
