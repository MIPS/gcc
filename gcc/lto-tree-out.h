/* LTO output code.
   Copyright (C) 2007, 2008 Free Software Foundation, Inc.
   Contributed by Kenneth Zadeck <zadeck@naturalbridge.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_LTO_TREE_OUT_H
#define GCC_LTO_TREE_OUT_H

#include "lto-section-out.h"


/* lto-function-out.c */

/* The fields written to a function or extern variable header.  */

struct output_block
{
  enum lto_section_type section_type;
  struct lto_out_decl_state *decl_state;

  /* The stream that the main tree codes are written to.  */
  struct lto_output_stream *main_stream;
  /* The stream that contains the indexes for the local name table.  */
  struct lto_output_stream *local_decl_index_stream;
  /* The stream that contains the local name table.  */
  struct lto_output_stream *local_decl_stream;
  /* The stream that contains the names for the named_labels.  */
  struct lto_output_stream *named_label_stream;
  /* The stream that contains the string table.  */
  struct lto_output_stream *string_stream;
  /* The stream that contains the ssa_names table.  */
  struct lto_output_stream *ssa_names_stream;
  /* The stream that contains the cfg.  */
  struct lto_output_stream *cfg_stream;

#ifdef LTO_STREAM_DEBUGGING
  /* The stream that contains the local decls index debugging information.  */
  struct lto_output_stream *debug_decl_index_stream;
  /* The stream that contains the local decls debugging information.  */
  struct lto_output_stream *debug_decl_stream;
  /* The stream that contains the labels debugging information.  */
  struct lto_output_stream *debug_label_stream;
  /* The stream that contains the ssa_names debugging information.  */
  struct lto_output_stream *debug_ssa_names_stream;
  /* The stream that contains the cfg debugging information.  */
  struct lto_output_stream *debug_cfg_stream;
  /* The stream that contains the gimple debugging information.  */
  struct lto_output_stream *debug_main_stream;
#endif

  /* The hash table that contains the set of labels we have seen so
     far and the indexes assigned to them.  */
  htab_t label_hash_table;
  int next_named_label_index;
  int next_unnamed_label_index;
  VEC(tree,heap) *named_labels;

  /* The out decl buffer that contains the set of local parm and var decls
     we have seen so far and the indexes assigned to them.  */
  struct lto_tree_ref_encoder local_decl_encoder;

  /* The local_decls_index and the local_decls_index_d are the indexes
     in the local var stream and the local var debugging stream where
     a particular local var is located.  This allows the local vars to
     be read in random order.  */ 
  VEC(int,heap) *local_decls_index;
  /* Index in local_decls so that list can be reconstructed
     properly.  */
  VEC(int,heap) *unexpanded_local_decls_index;
#ifdef LTO_STREAM_DEBUGGING
  VEC(int,heap) *local_decls_index_d;
#endif

  /* The hash table that contains the set of strings we have seen so
     far and the indexes assigned to them.  */
  htab_t string_hash_table;

  /* The current cgraph_node that we are currently serializing.  Null
     if we are serializing something else.  */
  struct cgraph_node *cgraph_node;

  /* These are the last file and line that were seen in the stream.
     If the current node differs from these, it needs to insert
     something into the stream and fix these up.  */
  const char *current_file;
  int current_line;
  int current_col;

  /* True if writing globals and types.  */
  bool global;

  /* Map global decls and types to indices in the main stream.  */
  htab_t main_hash_table;
};

struct output_block *create_output_block (enum lto_section_type);
void destroy_output_block (struct output_block *);
extern void output_constructors_and_inits (struct cgraph_node_set_def *);
extern void output_tree (struct output_block *, tree);

#endif  /* GCC_LTO_TREE_OUT_H  */
