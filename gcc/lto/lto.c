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
#include "vec.h"
#include "bitmap.h"
#include "pointer-set.h"
#include "ipa-prop.h"
#include "common.h"

/* This needs to be included after config.h.  Otherwise, _GNU_SOURCE will not
   be defined in time to set __USE_GNU in the system headers, and strsignal
   will not be declared.  */
#include <sys/mman.h>

static bitmap_obstack lto_bitmap_obstack;

DEF_VEC_P(bitmap);
DEF_VEC_ALLOC_P(bitmap,heap);

/* Read the constructors and inits.  */

static void
lto_materialize_constructors_and_inits (struct lto_file_decl_data * file_data)
{
  size_t len;
  const char *data = lto_get_section_data (file_data, 
					   LTO_section_static_initializer,
					   NULL, &len);
  lto_input_constructors_and_inits (file_data, data);
  lto_free_section_data (file_data, LTO_section_static_initializer, NULL,
			 data, len);
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

      if (flag_wpa)
	lto_mark_function_body_in_file (decl);
      else
        lto_input_function_body (file_data, decl, data);

      fn = DECL_STRUCT_FUNCTION (decl);
      lto_free_section_data (file_data, LTO_section_function_body, name,
			     data, len);

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
  VEC(tree, heap) *common_nodes;
  tree node;

  /* The global tree for the main identifier is filled in by language-specific
     front-end initialization that is not run in the LTO back-end.  It appears
     that all languages that perform such initialization currently do so in the
     same way, so we do it here.  */
  if (!main_identifier_node)
    main_identifier_node = get_identifier ("main");

  ptrdiff_type_node = integer_type_node;

  common_nodes = lto_get_common_nodes ();
  /* FIXME lto.  In the C++ front-end, fileptr_type_node is defined as a
     variant copy of of ptr_type_node, rather than ptr_node itself.  The
     distinction should only be relevant to the front-end, so we always
     use the C definition here in lto1.  */
  gcc_assert (fileptr_type_node == ptr_type_node);

  index_table = htab_create (37, lto_hash_global_slot_node,
			     lto_eq_global_slot_node, free);

#ifdef GLOBAL_STREAMER_TRACE
  fprintf (stderr, "\n\nPreloading all common_nodes.\n");
#endif

  for (i = 0; VEC_iterate (tree, common_nodes, i, node); i++)
    preload_common_node (node, index_table, &data_in->globals_index, NULL);

#ifdef GLOBAL_STREAMER_TRACE
  fprintf (stderr, "\n\nPreloaded %u common nodes.\n", i - 1);
#endif

  VEC_free(tree, heap, common_nodes);
  htab_delete (index_table);
}

/* Decode the content of memory pointed to by DATA in the the
   in decl state object STATE. DATA_IN points to a data_in structure for
   decoding. Return the address after the decoded object in the input.  */

static const uint32_t*
lto_read_in_decl_state (struct data_in *data_in, const uint32_t *data,
			struct lto_in_decl_state *state)
{
  uint32_t fn_decl_index;
  tree decl;
  uint32_t i, j;
  
  fn_decl_index = *data++;
  decl = VEC_index (tree, data_in->globals_index, fn_decl_index);
  if (TREE_CODE (decl) != FUNCTION_DECL)
    {
      gcc_assert (decl == void_type_node);
      decl = NULL_TREE;
    }
  state->fn_decl = decl;

  for (i = 0; i < LTO_N_DECL_STREAMS; i++)
    {
      uint32_t size = *data++;
      tree *decls = (tree *) xcalloc (size, sizeof (tree));

      for (j = 0; j < size; j++)
        decls[j] = VEC_index (tree, data_in->globals_index, data[j]);
      state->streams[i].size = size;
      state->streams[i].trees = decls;
      data += size;
    }

  return data;
}

static void
lto_read_decls (struct lto_file_decl_data *decl_data, const void *data,
		VEC(ld_plugin_symbol_resolution_t,heap) *resolutions)
{
  const struct lto_decl_header *header = (const struct lto_decl_header *) data;
  const int32_t decl_offset = sizeof (struct lto_decl_header);
  const int32_t main_offset = decl_offset + header->decl_state_size;
  const int32_t string_offset = main_offset + header->main_size;
#ifdef LTO_STREAM_DEBUGGING
  int32_t debug_main_offset;
#endif
  struct lto_input_block ib_main;
  struct lto_input_block debug_main;
  struct data_in data_in;
  unsigned int i;
  const uint32_t *data_ptr, *data_end;
  uint32_t num_decl_states;

#ifdef LTO_STREAM_DEBUGGING
  debug_main_offset = string_offset + header->string_size;
#endif
  
  LTO_INIT_INPUT_BLOCK (ib_main, (const char *) data + main_offset, 0,
			header->main_size);
#ifdef LTO_STREAM_DEBUGGING
  LTO_INIT_INPUT_BLOCK (debug_main, (const char *) data + debug_main_offset, 0,
			header->debug_main_size);
#endif

  memset (&data_in, 0, sizeof (struct data_in));
  data_in.file_data          = decl_data;
  data_in.strings            = (const char *) data + string_offset;
  data_in.strings_len        = header->string_size;
  data_in.globals_index	     = NULL;
  data_in.globals_resolution = resolutions;

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
      input_tree (&ib_main, &data_in);
      gcc_assert (ib_main.p <= ib_main.len);
    }

  /* Read in lto_in_decl_state objects. */

  data_ptr = (const uint32_t *) ((const char*) data + decl_offset); 
  data_end =
     (const uint32_t *) ((const char*) data_ptr + header->decl_state_size);
  num_decl_states = *data_ptr++;
  
  gcc_assert (num_decl_states > 0);
  decl_data->global_decl_state = lto_new_in_decl_state ();
  data_ptr = lto_read_in_decl_state (&data_in, data_ptr,
				     decl_data->global_decl_state);

  /* Read in per-function decl states and enter them in hash table.  */
  decl_data->function_decl_states =
    htab_create (37, lto_hash_in_decl_state, lto_eq_in_decl_state, free);

  for (i = 1; i < num_decl_states; i++)
    {
      struct lto_in_decl_state *state = lto_new_in_decl_state ();
      void **slot;

      data_ptr = lto_read_in_decl_state (&data_in, data_ptr, state);
      slot = htab_find_slot (decl_data->function_decl_states, state, INSERT);
      gcc_assert (*slot == NULL);
      *slot = state;
    }
  gcc_assert (data_ptr == data_end);
  
  /* Set the current decl state to be the global state. */
  decl_data->current_decl_state = decl_data->global_decl_state;

  /* The globals index vector is needed only while reading.  */

  VEC_free (tree, heap, data_in.globals_index);
  VEC_free (ld_plugin_symbol_resolution_t, heap, data_in.globals_resolution);
}

/* Read resolution for file named FILE_NAME. The resolution is read from
   RESOLUTION. An array with the symbol resolution is returned. The array
   size is written to SIZE. */

static VEC(ld_plugin_symbol_resolution_t,heap) *
lto_resolution_read (FILE *resolution, const char *file_name)
{
  /* We require that objects in the resolution file are in the same
     order as the lto1 command line. */
  unsigned int name_len;
  char *obj_name;
  unsigned int num_symbols;
  unsigned int i;
  VEC(ld_plugin_symbol_resolution_t,heap) *ret = NULL;
  unsigned max_index = 0;

  if (!resolution)
    return NULL;

  name_len = strlen (file_name);
  obj_name = XNEWVEC (char, name_len + 1);
  fscanf (resolution, " ");   /* Read white space. */

  fread (obj_name, sizeof (char), name_len, resolution);
  obj_name[name_len] = '\0';
  gcc_assert (strcmp(obj_name, file_name) == 0);
  free (obj_name);

  fscanf (resolution, "%u", &num_symbols);

  for (i = 0; i < num_symbols; i++)
    {
      unsigned index;
      char r_str[27];
      enum ld_plugin_symbol_resolution r;
      unsigned int j;
      unsigned int lto_resolution_str_len =
	sizeof (lto_resolution_str) / sizeof (char *);

      fscanf (resolution, "%u %26s", &index, r_str);
      if (index > max_index)
	max_index = index;

      for (j = 0; j < lto_resolution_str_len; j++)
	{
	  if (strcmp (lto_resolution_str[j], r_str) == 0)
	    {
	      r = j;
	      break;
	    }
	}
      gcc_assert (j < lto_resolution_str_len);

      VEC_safe_grow_cleared (ld_plugin_symbol_resolution_t, heap, ret,
			     index + 1);
      VEC_replace (ld_plugin_symbol_resolution_t, ret, index, r);
    }

  return ret;
}

/* Generate a TREE representation for all types and external decls
   entities in FILE.  

   Read all of the globals out of the file.  Then read the cgraph
   and process the .o index into the cgraph nodes so that it can open
   the .o file to load the functions and ipa information.   */

static struct lto_file_decl_data *
lto_file_read (lto_file *file, FILE *resolution_file)
{
  struct lto_file_decl_data *file_data;
  const char *data;
  size_t len;
  VEC(ld_plugin_symbol_resolution_t,heap) *resolutions;
  
  resolutions = lto_resolution_read (resolution_file, file->filename);

  file_data = XCNEW (struct lto_file_decl_data);
  file_data->file_name = file->filename;
  file_data->fd = -1;
  file_data->section_hash_table = lto_elf_build_section_table (file);
  file_data->renaming_hash_table = lto_create_renaming_table ();

  data = lto_get_section_data (file_data, LTO_section_decls, NULL, &len);
  lto_read_decls (file_data, data, resolutions);
  lto_free_section_data (file_data, LTO_section_decls, NULL, data, len);

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
   NAME will be NULL unless the section type is for a function
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
  char *data = NULL;

  s_slot.name = section_name;
  f_slot = (struct lto_section_slot *) htab_find (section_hash_table, &s_slot);
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

/* Vector of all cgraph node sets. */
static GTY (()) VEC(cgraph_node_set ,gc) *lto_cgraph_node_sets;

/* Group cgrah nodes by input files.  This is used mainly for testing
   right now. */

static void
lto_1_to_1_map (void)
{
  struct cgraph_node *node;
  struct lto_file_decl_data *file_data;
  struct pointer_map_t *pmap;
  cgraph_node_set set;
  void **slot;

  lto_cgraph_node_sets = VEC_alloc (cgraph_node_set, gc, 1);
  pmap = pointer_map_create ();

  for (node = cgraph_nodes; node; node = node->next)
    {
      /* We assume file_data are unique. */
      file_data = node->local.lto_file_data;
      gcc_assert(file_data);

      slot = pointer_map_contains (pmap, file_data);
      if (slot)
	  set = (cgraph_node_set) *slot;
      else
	{
	  set = cgraph_node_set_new ();
	  slot = pointer_map_insert (pmap, file_data);
	  *slot = set;
	  VEC_safe_push (cgraph_node_set, gc, lto_cgraph_node_sets, set);
	}
      cgraph_node_set_add (set, node);
    }

  pointer_map_destroy (pmap);
}

/* Add inlined clone NODE and its master clone to SET, if NODE itself has
   inlined callee, recursively add the callees.  */

static void
lto_add_inline_clones (cgraph_node_set set, struct cgraph_node *node,
		       bitmap original_nodes, bitmap inlined_decls)
{
   struct cgraph_node *master_clone, *callee;
   struct cgraph_edge *edge;

   /* NODE must be an inlined clone.  Add both its master clone and node
      itself to SET and mark the decls as inlined.  */
   if (!bitmap_bit_p (original_nodes, node->uid))
     {
	master_clone = cgraph_master_clone (node, false);
	gcc_assert (master_clone != NULL && master_clone != node);
	cgraph_node_set_add (set, master_clone);
	cgraph_node_set_add (set, node);
	bitmap_set_bit (inlined_decls, DECL_UID (node->decl));
     }
   
   /* Check to see if NODE has any inlined callee.  */
   for (edge = node->callees; edge != NULL; edge = edge->next_callee)
     {
	callee = edge->callee;
	if (callee->global.inlined_to != NULL)
	    lto_add_inline_clones (set, callee, original_nodes,
				   inlined_decls);
     }
}

/* Compute the transitive closure of inlining of SET based on the
   information in the call-graph.  Returns a bitmap of decls indexed
   by UID.  */

static bitmap
lto_add_all_inlinees (cgraph_node_set set)
{
  cgraph_node_set_iterator csi;
  struct cgraph_node *node;
  bitmap original_nodes = BITMAP_ALLOC (&lto_bitmap_obstack);
  bitmap inlined_decls = BITMAP_ALLOC (&lto_bitmap_obstack);

  /* We are going to iterate SET will adding to it, mark all original
     nodes so that we only add node inlined to original nodes.  */
  for (csi = csi_start (set); !csi_end_p (csi); csi_next (&csi))
    bitmap_set_bit (original_nodes, csi_node (csi)->uid);

  for (csi = csi_start (set); !csi_end_p (csi); csi_next (&csi))
    {
      node = csi_node (csi);
      if (bitmap_bit_p (original_nodes, node->uid))
	lto_add_inline_clones (set, node, original_nodes, inlined_decls);
    }

  BITMAP_FREE (original_nodes);
  return inlined_decls;
}

static lto_file *current_lto_file;

/* Write all output files in WPA mode.  Returns a NULL-terminated array of
   output file names.  */

static char **
lto_wpa_write_files (void)
{
  char **output_files;
  unsigned i;
  lto_file *file;
  cgraph_node_set set;
  bitmap decls;
  VEC(bitmap,heap) *inlined_decls = NULL;

  /* Include all inlined function. */
  for (i = 0; VEC_iterate (cgraph_node_set, lto_cgraph_node_sets, i, set); i++)
    {
      decls = lto_add_all_inlinees (set);
      VEC_safe_push (bitmap, heap, inlined_decls, decls);
    }

  output_files = XNEWVEC (char *, VEC_length (cgraph_node_set,
					      lto_cgraph_node_sets) + 1);

  for (i = 0; VEC_iterate (cgraph_node_set, lto_cgraph_node_sets, i, set); i++)
    {
      size_t needed = 16;
      size_t len = needed;
      char * temp_filename = XNEWVEC (char, len);

      do 
	{
	  if (needed > len)
	    {
	      len = needed;
	      temp_filename = XRESIZEVEC (char, temp_filename, len);
	    }
	  needed = snprintf (temp_filename, len, "bogus%d.lto.o", i);
	}
      while (needed >= len);

      output_files[i] = temp_filename;

      file = lto_elf_file_open (temp_filename, /*writable=*/true);
      if (!file)
        fatal_error ("lto_elf_file_open() failed");

      lto_set_current_out_file (file);
      lto_new_static_inline_states ();

      decls = VEC_index (bitmap, inlined_decls, i);
      lto_force_functions_static_inline (decls);

      ipa_write_summaries_of_cgraph_node_set (set);
      lto_delete_static_inline_states ();
      
      lto_set_current_out_file (NULL);
      lto_elf_file_close (file);

    } 

  output_files[i] = NULL;

  for (i = 0; VEC_iterate (bitmap, inlined_decls, i, decls); i++)
    BITMAP_FREE (decls);
  VEC_free (bitmap, heap, inlined_decls);

  return output_files;
}


/* Perform local transformations (LTRANS) on the files in the NULL-terminated
   FILES array.  These should have been written previously by
   lto_wpa_write_files ().  Transformations are performed via the
   ltrans_driver executable, which is passed a list of filenames via the
   command line.  The CC and CFLAGS environment variables are set to
   appropriate values before it is executed.  */

static void
lto_execute_ltrans (char *const *files)
{
  struct pex_obj *pex;
  const char *env_val;
  const char *extra_cflags = " -fno-wpa -fltrans -xlto";
  struct obstack env_obstack;
  char **argv;
  const char **argv_ptr;
  const char *errmsg;
  size_t i;
  int err;
  int status;
  FILE *ltrans_output_list_stream = NULL;

  /* Set the CC environment variable.  */
  env_val = getenv ("COLLECT_GCC");
  if (!env_val)
    fatal_error ("environment variable COLLECT_GCC must be set");

  obstack_init (&env_obstack);
  obstack_grow (&env_obstack, "CC=", sizeof ("CC=") - 1);
  obstack_grow (&env_obstack, env_val, strlen (env_val) + 1);
  putenv (XOBFINISH (&env_obstack, char *));

  /* Set the CFLAGS environment variable.  */
  env_val = getenv ("COLLECT_GCC_OPTIONS");
  if (!env_val)
    fatal_error ("environment variable COLLECT_GCC_OPTIONS must be set");

  obstack_init (&env_obstack);
  obstack_grow (&env_obstack, "CFLAGS=", sizeof ("CFLAGS=") - 1);
  obstack_grow (&env_obstack, env_val, strlen (env_val));
  obstack_grow (&env_obstack, extra_cflags, strlen (extra_cflags) + 1);
  putenv (XOBFINISH (&env_obstack, char *));

  pex = pex_init (0, "lto1", NULL);
  if (pex == NULL)
    fatal_error ("pex_init failed: %s", xstrerror (errno));

  /* Initalize the arguments for the LTRANS driver.  */
  for (i = 0; files[i]; ++i);
  argv = XNEWVEC (char *, i + 2);

  /* Open the LTRANS output list.  */
  if (ltrans_output_list)
    {
      ltrans_output_list_stream = fopen (ltrans_output_list, "w");
      if (ltrans_output_list_stream == NULL)
	error ("opening LTRANS output list %s: %m", ltrans_output_list);
    }

  argv_ptr = (const char **)argv;
  *argv_ptr++ = ltrans_driver;
  for (i = 0; files[i]; ++i)
    {
      *argv_ptr++ = files[i];

      /* Replace the .o suffix with a .ltrans.o suffix and write the resulting
	 name to the LTRANS output list.  */
      if (ltrans_output_list_stream)
	{
	  size_t len = strlen (files[i]) - 2;

	  if (fwrite (files[i], 1, len, ltrans_output_list_stream) < len
	      || fwrite (".ltrans.o\n", 1, 10, ltrans_output_list_stream) < 10)
	    error ("writing to LTRANS output list %s: %m", ltrans_output_list);
	}
    }
  *argv_ptr++ = NULL;

  /* Close the LTRANS output list.  */
  if (ltrans_output_list_stream && fclose (ltrans_output_list_stream))
    error ("closing LTRANS output list %s: %m", ltrans_output_list);

  /* Execute the LTRANS driver.  */
  errmsg = pex_run (pex, PEX_LAST | PEX_SEARCH, argv[0], argv, NULL, NULL,
		    &err);
  if (errmsg)
    {
      fatal_error ("%s: %s", errmsg, xstrerror (err));
    }

  if (!pex_get_status (pex, 1, &status))
    fatal_error ("can't get program status: %s", xstrerror (errno));
  pex_free (pex);

  if (status)
    {
      if (WIFSIGNALED (status))
	{
	  int sig = WTERMSIG (status);
	  fatal_error ("%s terminated with signal %d [%s]%s",
		       argv[0], sig, strsignal (sig),
		       WCOREDUMP (status) ? ", core dumped" : "");
	}
      else
	fatal_error ("%s terminated with status %d", argv[0], status);
    }
}

typedef struct {
  struct pointer_set_t *free_list;
  struct pointer_set_t *seen;
} lto_fixup_data_t;

/* A walk_tree callback used by lto_fixup_state. TP is the pointer to the
   current tree. WALK_SUBTREES indicates if the subtrees will be walked.
   DATA is a pointer set to record visited nodes. */

static tree
lto_fixup_tree (tree *tp, int *walk_subtrees, void *data)
{
  tree t = *tp;
  lto_fixup_data_t *fixup_data = (lto_fixup_data_t *) data;
  tree prevailing;

  if (pointer_set_contains (fixup_data->seen, t))
    return NULL;

  *walk_subtrees = 1;

  if (TREE_CODE (t) == VAR_DECL || TREE_CODE (t) == FUNCTION_DECL)
    {
      prevailing = lto_symtab_prevailing_decl (t);

      if (t != prevailing)
	{
	  if (TREE_CODE (t) == FUNCTION_DECL
	      && flag_exceptions
	      && TREE_NOTHROW (prevailing) != TREE_NOTHROW (t))
	    {
	      if (TREE_NOTHROW (prevailing))
		lto_mark_nothrow_fndecl (prevailing);
	      else
		error ("%qD change to exception throwing", prevailing);
	    }

	  pointer_set_insert (fixup_data->free_list, t);

	   /* Also replace t with prevailing defintion.  We don't want to
	      insert the other defintion in the seen set as we want to
	      replace all instances of it.  */
	  *tp = prevailing;
	  t = prevailing;
	}
    }

  pointer_set_insert (fixup_data->seen, t);

  /* walk_tree does not visit all reachable nodes that need to be fixed up.
     Hence we do special processing here for those kind of nodes. */
  switch (TREE_CODE (t))
    {
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      walk_tree (&TREE_TYPE (t), lto_fixup_tree, data, NULL);
      walk_tree (&TYPE_FIELDS (t), lto_fixup_tree, data, NULL);
      walk_tree (&TYPE_BINFO (t), lto_fixup_tree, data, NULL);
      break;
    case TREE_BINFO:
      {
	tree base;
	unsigned i;

	walk_tree (&TREE_TYPE (t), lto_fixup_tree, data, NULL);
	walk_tree (&BINFO_OFFSET (t), lto_fixup_tree, data, NULL);
	walk_tree (&BINFO_VTABLE (t), lto_fixup_tree, data, NULL);
	walk_tree (&BINFO_VIRTUALS (t), lto_fixup_tree, data, NULL);
	walk_tree (&BINFO_VPTR_FIELD (t), lto_fixup_tree, data, NULL);
	walk_tree (&BINFO_SUBVTT_INDEX (t), lto_fixup_tree, data, NULL);
	walk_tree (&BINFO_VPTR_INDEX (t), lto_fixup_tree, data, NULL);
	walk_tree (&BINFO_INHERITANCE_CHAIN (t), lto_fixup_tree, data, NULL);
	for (i = 0; BINFO_BASE_ITERATE (t, i, base); i++)
	  {
	    tree old_base = base;
	    walk_tree (&base, lto_fixup_tree, data, NULL);
	    if (base != old_base)
	      VEC_replace (tree, BINFO_BASE_BINFOS (t), i, base);
	  }
      }
      break;
    case VAR_DECL:
      walk_tree (&DECL_INITIAL (t), lto_fixup_tree, data, NULL);
      break;	
    default:
      ;
    }
  return NULL;
}

/* Helper function of lto_fixup_decls. Walks the var and fn streams in STATE,
   replaces var and function decls with the corresponding prevailing def and
   records the old decl in the free-list in DATA. We also record visted nodes
   in the seen-set in DATA to avoid multiple visit for nodes that need not
   to be replaced.  */

static void
lto_fixup_state (struct lto_in_decl_state *state, lto_fixup_data_t *data)
{
  unsigned i;
  struct lto_tree_ref_table *vars = &state->streams[LTO_DECL_STREAM_VAR_DECL];
  struct lto_tree_ref_table *fns = &state->streams[LTO_DECL_STREAM_FN_DECL];
  struct lto_tree_ref_table *types = &state->streams[LTO_DECL_STREAM_TYPE];

  /* We have to fix up types as well. Vtables reference function decls.  */
  for (i = 0; i < types->size; i++)
    {
      walk_tree (types->trees + i, lto_fixup_tree, data, NULL);
    }

  for (i = 0; i < fns->size; i++)
    {
      tree decl = fns->trees[i];
      gcc_assert (decl);
      gcc_assert (TREE_CODE (decl) == FUNCTION_DECL);
      walk_tree (fns->trees + i, lto_fixup_tree, data, NULL);
    }

  for (i = 0; i < vars->size; i++)
    {
      tree decl = vars->trees[i];
      gcc_assert (decl);
      if (TREE_CODE (decl) == RESULT_DECL)
	continue;
      gcc_assert (TREE_CODE (decl) == VAR_DECL);
      walk_tree (vars->trees + i, lto_fixup_tree, data, NULL);
    }
}

/* A callback of htab_traverse. Just extract a state from SLOT and the
   lto_fixup_data_t object from AUX and calls lto_fixup_state. */

static int
lto_fixup_state_aux (void **slot, void *aux)
{
  struct lto_in_decl_state *state = (struct lto_in_decl_state *) *slot;
  lto_fixup_state (state, (lto_fixup_data_t *) aux);
  return 1;
}

/* A callback to pointer_set_traverse. Frees the tree pointed by p. Removes
   from it from the UID -> DECL mapping. */

static bool
free_decl (const void *p, void *data ATTRIBUTE_UNUSED)
{
  const_tree ct = (const_tree) p;
  tree t = CONST_CAST_TREE (ct);

  remove_decl_from_map (t);
  lto_symtab_clear_resolution (t);
  ggc_free (t);
  return true;
}

/* Fix the decls from all FILES. Replaces each decl with the corresponding
   prevailing one. */

static void
lto_fixup_decls (struct lto_file_decl_data **files)
{
  unsigned int i;
  tree decl;
  struct pointer_set_t *free_list = pointer_set_create ();
  struct pointer_set_t *seen = pointer_set_create ();
  lto_fixup_data_t data;

  data.free_list = free_list;
  data.seen = seen;
  for (i = 0; files[i]; i++)
    {
      struct lto_file_decl_data *file = files[i];
      struct lto_in_decl_state *state = file->global_decl_state;
      lto_fixup_state (state, &data);

      htab_traverse (file->function_decl_states, lto_fixup_state_aux, &data);
    }

  for (i = 0; VEC_iterate (tree, lto_global_var_decls, i, decl); i++)
    {
      tree saved_decl = decl;
      walk_tree (&decl, lto_fixup_tree, &data, NULL);
      if (decl != saved_decl)
	VEC_replace (tree, lto_global_var_decls, i, decl);
    }

  pointer_set_traverse (free_list, free_decl, NULL);
  pointer_set_destroy (free_list);
  pointer_set_destroy (seen);
}

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
  FILE *resolution = NULL;
  unsigned num_objects;
  int t;

  /* Set the hooks so that all of the ipa passes can read in their data.  */
  lto_set_in_hooks (all_file_decl_data, get_section_data,
		    free_section_data);

  bitmap_obstack_initialize (&lto_bitmap_obstack);

  /* Read the resolution file. */
  if (resolution_file_name)
    {
      resolution = fopen (resolution_file_name, "r");
      gcc_assert (resolution != NULL);
      t = fscanf (resolution, "%u", &num_objects);
      gcc_assert (t == 1);

      /* True, since the plugin splits the archives. */
      gcc_assert (num_objects == num_in_fnames);
    }

  /* Read all of the object files specified on the command line.  */
  for (i = 0; i < num_in_fnames; ++i)
    {
      current_lto_file = lto_elf_file_open (in_fnames[i], /*writable=*/false);
      if (!current_lto_file)
	break;
      file_data = lto_file_read (current_lto_file, resolution);
      if (!file_data)
	break;

      all_file_decl_data[j++] = file_data;

      lto_elf_file_close (current_lto_file);
      current_lto_file = NULL;
    }

  if (resolution_file_name)
    fclose (resolution);

  all_file_decl_data[j] = NULL;

  /* Set the hooks so that all of the ipa passes can read in their data.  */
  lto_set_in_hooks (all_file_decl_data, get_section_data,
		    free_section_data);

  ipa_read_summaries ();

  lto_fixup_decls (all_file_decl_data);

  /* FIXME!!! This loop needs to be changed to use the pass manager to
     call the ipa passes directly.  */
  for (i = 0; i < j; i++)
    {
      struct lto_file_decl_data* file_data = all_file_decl_data [i];

      lto_materialize_constructors_and_inits (file_data);
    }

  if (flag_wpa)
    lto_1_to_1_map ();

  /* Now that we have input the cgraph, we need to clear all of the aux
     nodes and read the functions if we are not running in WPA mode.  

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
  current_function_decl = NULL;
  set_cfun (NULL);

  /* Inform the middle end about the global variables we have seen.  */
  for (i = 0; VEC_iterate (tree, lto_global_var_decls, i, decl); i++)
    rest_of_decl_compilation (decl,
                              /*top_level=*/1,
                              /*at_end=*/0);

  /* Fix up any calls to DECLs that have become not exception throwing.  */
  lto_fixup_nothrow_decls ();

  /* Let the middle end know that we have read and merged all of the
     input files.  */ 
  /*cgraph_finalize_compilation_unit ();*/
  if (!flag_wpa)
    cgraph_optimize ();
  else
    {
      /* FIXME lto. Hack. We should use the IPA passes.  There are a number
         of issues with this now. 1. There is no convenient way to do this.
         2. Some passes may depend on properties that requires the function
	 bodies to compute.  */
      cgraph_function_flags_ready = true;
      bitmap_obstack_initialize (NULL);
      ipa_register_cgraph_hooks ();

      /* Reset inlining informationi before running IPA inliner.  */
      for (node = cgraph_nodes; node; node = node->next)
	reset_inline_failed (node);

      /* FIXME lto. We should not call this function directly. */
      pass_ipa_inline.pass.execute ();

      verify_cgraph ();
      bitmap_obstack_release (NULL);
    }

  if (flag_wpa)
    {
      char **output_files;
      size_t i;

      output_files = lto_wpa_write_files ();
      lto_execute_ltrans (output_files);

      for (i = 0; output_files[i]; ++i)
	XDELETEVEC (output_files[i]);
      XDELETEVEC (output_files);
    }

  bitmap_obstack_release (&lto_bitmap_obstack);
}

#include "gt-lto-lto.h"
