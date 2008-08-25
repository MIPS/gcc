/* Write the gimple representation of a function and it's local
   variables to a .o file.

   Copyright 2006, 2007, 2008 Free Software Foundation, Inc.
   Contributed by Kenneth Zadeck <zadeck@naturalbridge.com>

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
#include "tm.h"
#include "toplev.h"
#include "tree.h"
#include "expr.h"
#include "flags.h"
#include "params.h"
#include "input.h"
#include "varray.h"
#include "hashtab.h"
#include "basic-block.h"
#include "tree-iterator.h"
#include "tree-pass.h"
#include "tree-flow.h"
#include "cgraph.h"
#include "function.h"
#include "ggc.h"
#include "diagnostic.h"
#include "except.h"
#include "debug.h"
#include "vec.h"
#include "tree-vectorizer.h"
#include "timevar.h"
#include "lto-tags.h"
#include "lto-section-out.h"
#include "lto-tree-out.h"

sbitmap lto_flags_needed_for;
sbitmap lto_types_needed_for;

#ifdef LTO_STREAM_DEBUGGING
const char *LTO_tree_tag_names[LTO_tree_last_tag];
#endif


/* The index of the last eh_region seen for an instruction.  The
   eh_region for an instruction is only emitted if it different from
   the last instruction.  */
static int last_eh_region_seen;
static unsigned int expr_to_tag[NUM_TREE_CODES];

/* Returns nonzero if P1 and P2 are equal.  */

static int
eq_label_slot_node (const void *p1, const void *p2)
{
  const struct lto_decl_slot *ds1 =
    (const struct lto_decl_slot *) p1;
  const struct lto_decl_slot *ds2 =
    (const struct lto_decl_slot *) p2;

  return LABEL_DECL_UID (ds1->t) == LABEL_DECL_UID (ds2->t);
}

/* Returns a hash code for P.  */

static hashval_t
hash_label_slot_node (const void *p)
{
  const struct lto_decl_slot *ds = (const struct lto_decl_slot *) p;
  return (hashval_t) LABEL_DECL_UID (ds->t);
}


struct string_slot {
  const char *s;
  int len;
  unsigned int slot_num;
};


/* Returns a hash code for P.  */

static hashval_t
hash_string_slot_node (const void *p)
{
  const struct string_slot *ds = (const struct string_slot *) p;
  return (hashval_t) htab_hash_string (ds->s);
}


/* Returns nonzero if P1 and P2 are equal.  */

static int
eq_string_slot_node (const void *p1, const void *p2)
{
  const struct string_slot *ds1 =
    (const struct string_slot *) p1;
  const struct string_slot *ds2 =
    (const struct string_slot *) p2;

  if (ds1->len == ds2->len)
    {
      int i;
      for (i=0; i<ds1->len; i++)
	if (ds1->s[i] != ds2->s[i])
	  return 0;
      return 1;
    }
  else return 0;
}


/* Free the string slot.  */

static void 
string_slot_free (void *p)
{
  struct string_slot *slot = (struct string_slot *)p;
  free (CONST_CAST (void *, (const void *) slot->s));
  free (slot);
}


/* The output stream that contains the abbrev table for all of the
   functions in this compilation unit.  */
static void output_expr_operand (struct output_block *, tree);

/* Clear the line info stored in DATA_IN.  */

static void
clear_line_info (struct output_block *ob)
{
  ob->current_file = NULL;
  ob->current_line = 0;
  ob->current_col = 0;
}


/* Create the output block and return it.  SECTION_TYPE is LTO_section_function_body or
   lto_static_initializer.  */
/* FIXME: Now declared in lto-section-out.h.  Move definition to lto-section-out.c ? */

struct output_block *
create_output_block (enum lto_section_type section_type)
{
  struct output_block *ob = XCNEW (struct output_block);

  ob->section_type = section_type;
  ob->decl_state = lto_get_out_decl_state ();
  ob->main_stream = XCNEW (struct lto_output_stream);
  ob->string_stream = XCNEW (struct lto_output_stream);

  ob->named_label_stream = XCNEW (struct lto_output_stream);
  if (section_type == LTO_section_function_body)
    {
      ob->local_decl_index_stream = XCNEW (struct lto_output_stream);
      ob->local_decl_stream = XCNEW (struct lto_output_stream);
      ob->ssa_names_stream =XCNEW (struct lto_output_stream);
      ob->cfg_stream = XCNEW (struct lto_output_stream);
    }
#ifdef LTO_STREAM_DEBUGGING
  lto_debug_context.out = lto_debug_out_fun;
  lto_debug_context.indent = 0;
  lto_debug_context.tag_names = LTO_tree_tag_names;
#endif

  clear_line_info (ob);

  ob->label_hash_table
    = htab_create (37, hash_label_slot_node, eq_label_slot_node, free);

  ob->string_hash_table
    = htab_create (37, hash_string_slot_node, eq_string_slot_node,
		   string_slot_free);

  lto_init_tree_ref_encoder (&ob->local_decl_encoder, lto_hash_decl_slot_node,
			     lto_eq_decl_slot_node);

  /* The unnamed labels must all be negative.  */
  ob->next_unnamed_label_index = -1;
  return ob;
}


/* Destroy the output block OB.  */
/* FIXME: Now declared in lto-section-out.h.  Move definition to
   lto-section-out.c ? */

void
destroy_output_block (struct output_block * ob)
{
  enum lto_section_type section_type = ob->section_type;

  htab_delete (ob->label_hash_table);
  htab_delete (ob->string_hash_table);

  if (ob->main_hash_table)
    htab_delete (ob->main_hash_table);

  free (ob->main_stream);
  free (ob->string_stream);
  free (ob->named_label_stream);
  if (section_type == LTO_section_function_body)
    {
      free (ob->local_decl_index_stream);
      free (ob->local_decl_stream);
      free (ob->ssa_names_stream);
      free (ob->cfg_stream);
    }

  LTO_CLEAR_DEBUGGING_STREAM (debug_main_stream);
  LTO_CLEAR_DEBUGGING_STREAM (debug_label_stream);
  if (section_type == LTO_section_function_body)
    {
      LTO_CLEAR_DEBUGGING_STREAM (debug_ssa_names_stream);
      LTO_CLEAR_DEBUGGING_STREAM (debug_cfg_stream);
      LTO_CLEAR_DEBUGGING_STREAM (debug_decl_stream);
      LTO_CLEAR_DEBUGGING_STREAM (debug_decl_index_stream);
    }

  VEC_free (tree, heap, ob->named_labels);
  if (section_type == LTO_section_function_body)
    {
      VEC_free (int, heap, ob->local_decls_index);
      VEC_free (int, heap, ob->unexpanded_local_decls_index);
#ifdef LTO_STREAM_DEBUGGING
      VEC_free (int, heap, ob->local_decls_index_d);
#endif
    }

  lto_destroy_tree_ref_encoder (&ob->local_decl_encoder);

  free (ob);
}



/* Output STRING of LEN to the string table in OB.  Then put the index 
   onto the INDEX_STREAM.  */

static void
output_string (struct output_block *ob, 
	       struct lto_output_stream *index_stream,
	       const char *string,
	       unsigned int len)
{
  void **slot;
  struct string_slot s_slot;
  s_slot.s = string;
  s_slot.len = len;
  s_slot.slot_num = 0;

  slot = htab_find_slot (ob->string_hash_table, &s_slot, INSERT);
  if (*slot == NULL)
    {
      struct lto_output_stream *string_stream = ob->string_stream;
      unsigned int start = string_stream->total_size;
      struct string_slot *new_slot
	= (struct string_slot *) xmalloc (sizeof (struct string_slot));
      unsigned int i;
      char *new_string = (char *) xmalloc (len);

      memcpy (new_string, string, len);
      new_slot->s = new_string;
      new_slot->len = len;
      new_slot->slot_num = start;
      *slot = new_slot;
      lto_output_uleb128_stream (index_stream, start);
      lto_output_uleb128_stream (string_stream, len);
      for (i=0; i<len; i++)
	lto_output_1_stream (string_stream, string[i]);
    }
  else
    {
      struct string_slot *old_slot = (struct string_slot *)*slot;
      lto_output_uleb128_stream (index_stream, old_slot->slot_num);

      /* From the debugging protocol's point of view, the entry needs
	 to look the same reguardless of whether this is the first
	 occurence of this string or not.  Thus, we simulate the same
	 debugging info as would be output as if this was a new
	 string.  */
      LTO_DEBUG_WIDE ("U", len);
    }
  LTO_DEBUG_STRING (string, len);
}


/* Put out a real constant.  */

static void
output_real (struct output_block *ob, tree t)
{
  static char real_buffer[1000];
  const REAL_VALUE_TYPE *r = &TREE_REAL_CST (t);

  LTO_DEBUG_TOKEN ("real");
  real_to_hexadecimal (real_buffer, r, 1000, 0, 1);
  output_string (ob, ob->main_stream, real_buffer, strlen (real_buffer));
}


/* Write a zero to the output stream.  */

static void
output_zero (struct output_block *ob)
{
  LTO_DEBUG_WIDE ("U", 0);
  lto_output_1_stream (ob->main_stream, 0);
}


/* Output an unsigned LEB128 quantity to OB->main_stream.  */

static void
output_uleb128 (struct output_block *ob, unsigned HOST_WIDE_INT work)
{
  lto_output_uleb128_stream (ob->main_stream, work);
}


/* Output a signed LEB128 quantity to OB->main_stream.  */

static void
output_sleb128 (struct output_block *ob, HOST_WIDE_INT work)
{
  lto_output_sleb128_stream (ob->main_stream, work);
}

/* HOST_WIDEST_INT version of output_uleb128.  OB and WORK are as in
   output_uleb128. */

static void
output_widest_uint_uleb128 (struct output_block *ob,
			    unsigned HOST_WIDEST_INT work)
{
  lto_output_widest_uint_uleb128_stream (ob->main_stream, work);
}

/* Put out a integer constant.  These are stored as two HOST_WIDE_INTS
   so games may have to be played to shift the data from the high to
   the low value.  */

static void
output_integer (struct output_block *ob, tree t)
{
  lto_output_integer_stream (ob->main_stream, t);
}


/* Build a densely packed word that contains only the flags that are
   used for this type of tree EXPR and write the word in uleb128 to
   the OB.  IF CODE is 0 (ERROR_MARK), put the flags anyway.
   FORCE_LOC forces the line number to be serialized reguardless of
   the type of tree.  */


static void
output_tree_flags (struct output_block *ob, enum tree_code code, tree expr, 
		   bool force_loc)
{
  lto_flags_type flags = 0;
  const char *file_to_write = NULL;
  int line_to_write = -1;
  int col_to_write = -1;

  if (code == 0 || TEST_BIT (lto_flags_needed_for, code))
    {

#define START_CLASS_SWITCH()              \
  {                                       \
    enum tree_code code = TREE_CODE (expr); \
                                          \
    switch (TREE_CODE_CLASS (code))       \
    {

#define START_CLASS_CASE(class)    case class:
#define ADD_CLASS_DECL_FLAG(flag_name)    \
      { flags <<= 1; if (expr->decl_common. flag_name ) flags |= 1; }
#define ADD_CLASS_EXPR_FLAG(flag_name)    \
      { flags <<= 1; if (expr->base. flag_name ) flags |= 1; }
#define ADD_CLASS_TYPE_FLAG(flag_name)    \
      { flags <<= 1; if (expr->type. flag_name ) flags |= 1; }
#define END_CLASS_CASE(class)      break;
#define END_CLASS_SWITCH()                \
    default:                              \
      gcc_unreachable ();                 \
    }


#define START_EXPR_SWITCH()               \
    switch (code)			  \
    {
#define START_EXPR_CASE(code)    case code:
#define ADD_EXPR_FLAG(flag_name) \
      { flags <<= 1; if (expr->base. flag_name ) flags |= 1; }
#define ADD_TYPE_FLAG(flag_name) \
      { flags <<= 1; if (expr->type. flag_name ) flags |= 1; }
#define ADD_DECL_FLAG(flag_name) \
      { flags <<= 1; if (expr->decl_common. flag_name ) flags |= 1; }
#define ADD_VIS_FLAG(flag_name)  \
      { flags <<= 1; if (expr->decl_with_vis. flag_name ) flags |= 1; }
#define ADD_VIS_FLAG_SIZE(flag_name,size)	\
      { flags <<= size; if (expr->decl_with_vis. flag_name ) flags |= expr->decl_with_vis. flag_name; }
#define ADD_FUN_FLAG(flag_name)  \
      { flags <<= 1; if (expr->function_decl. flag_name ) flags |= 1; }
#define END_EXPR_CASE(class)      break;
#define END_EXPR_SWITCH()                 \
    default:                              \
      gcc_unreachable ();                 \
    }                                     \
  }

#include "lto-tree-flags.def"

#undef START_CLASS_SWITCH
#undef START_CLASS_CASE
#undef ADD_CLASS_DECL_FLAG
#undef ADD_CLASS_EXPR_FLAG
#undef ADD_CLASS_TYPE_FLAG
#undef END_CLASS_CASE
#undef END_CLASS_SWITCH
#undef START_EXPR_SWITCH
#undef START_EXPR_CASE
#undef ADD_EXPR_FLAG
#undef ADD_TYPE_FLAG
#undef ADD_DECL_FLAG
#undef ADD_VIS_FLAG
#undef ADD_VIS_FLAG_SIZE
#undef ADD_FUN_FLAG
#undef END_EXPR_CASE
#undef END_EXPR_SWITCH

      flags <<= LTO_SOURCE_LOC_BITS;
      if (expr)
	{
	  const char *current_file = NULL;
	  int current_line = 0;
	  int current_col = 0;
	  if (EXPR_P (expr) || GIMPLE_STMT_P (expr))
	    {
	      if (EXPR_HAS_LOCATION (expr))
		{
		  expanded_location xloc 
		    = expand_location (EXPR_LOCATION (expr));

		  current_file = xloc.file;
		  current_line = xloc.line;
		  current_col = xloc.column;
		  flags |= LTO_SOURCE_HAS_LOC;
		}
	    }

	  /* We use force_loc here because we only want to put out the
	     line number when we are writing the top level list of var
	     and parm decls, not when we access them inside a
	     function.  */
	  else if (force_loc && DECL_P (expr))
	    {
	      expanded_location xloc 
		= expand_location (DECL_SOURCE_LOCATION (expr));
	      if (xloc.file)
		{
		  current_file = xloc.file;
		  current_line = xloc.line;
		  current_col = xloc.column;
		  flags |= LTO_SOURCE_HAS_LOC;
		}
	    }

	  if (current_file
	      && (ob->current_file == NULL
		  || strcmp (ob->current_file, current_file) != 0))
	    {
	      file_to_write = current_file;
	      ob->current_file = current_file;
	      flags |= LTO_SOURCE_FILE;
	    }
	  if (current_line != 0
	      && ob->current_line != current_line)
	    {
	      line_to_write = current_line;
	      ob->current_line = current_line;
	      flags |= LTO_SOURCE_LINE;
	    }
	  if (current_col != 0
	      && ob->current_col != current_col)
	    {
	      col_to_write = current_col;
	      ob->current_col = current_col;
	      flags |= LTO_SOURCE_COL;
	    }
	}

      LTO_DEBUG_TOKEN ("flags");
      output_widest_uint_uleb128 (ob, flags);
      /* Note that when we force flags with code == 0,
         we cause the debugging info to be omitted.
         I tried to fix this like so:
           LTO_DEBUG_TREE_FLAGS (TREE_CODE (expr), flags);
         This breaks input_local_var, however, which
         expects the debug info to be missing.
         Do the fix anyway, and fix input_local_var.  */
      /* LTO_DEBUG_TREE_FLAGS (code, flags); */
      LTO_DEBUG_TREE_FLAGS (TREE_CODE (expr), flags);

      if (file_to_write)
	{
	  LTO_DEBUG_TOKEN ("file");
	  output_string (ob, ob->main_stream, 
			 file_to_write, strlen (file_to_write) + 1);
	}
      if (line_to_write != -1)
	{
	  LTO_DEBUG_TOKEN ("line");
	  output_uleb128 (ob, line_to_write);
	}
      if (col_to_write != -1)
	{
	  LTO_DEBUG_TOKEN ("col");
	  output_uleb128 (ob, col_to_write);
	}
    }
}

static void output_record_start (struct output_block *, tree, tree, unsigned int);
static unsigned int output_local_decl_ref (struct output_block *, tree, bool);

/* Return innermost enclosing FUNCTION_DECL for a type,
   or NULL_TREE if there is none.  */
/* FIXME: Move this to tree.c alongside DECL_FUNCTION_CONTEXT.  */
/* FIXME: Can type be ERROR_MARK?  See DECL_FUNCTION_CONTEXT.  */

static tree
type_function_context (const_tree type)
{
  tree context = TYPE_CONTEXT (type);

  while (context && TREE_CODE (context) != FUNCTION_DECL)
    {
      if (TREE_CODE (context) == BLOCK)
        context = BLOCK_SUPERCONTEXT (context);
      else
        context = get_containing_scope (context);
    }
  
  return context;
}

/* Return true if a FIELD_DECL depends on a function context,
   i.e., makes reference to a function body and should be serialized
   with the function body, not the file scope.  */

static bool
field_decl_is_local (tree decl ATTRIBUTE_UNUSED)
{
#ifdef STREAM_LOCAL_TYPES
  return (decl_function_context (decl)
          || variably_modified_type_p (TREE_TYPE(decl), NULL)
          || type_function_context (TREE_TYPE (decl)));
#else
  return false;
#endif
}

/* Return true if a TYPE__DECL depends on a function context,
   i.e., makes reference to a function body and should be serialized
   with the function body, not the file scope.  */

static bool
type_decl_is_local (tree decl ATTRIBUTE_UNUSED)
{
#ifdef STREAM_LOCAL_TYPES
  return (decl_function_context (decl)
          || variably_modified_type_p (TREE_TYPE(decl), NULL));
#else
  return false;
#endif
}

/* Like output_type_ref, but no debug information is written.  */

static void
output_type_ref_1 (struct output_block *ob, tree node)
{
#ifdef STREAM_LOCAL_TYPES
  if (variably_modified_type_p (node, NULL) || type_function_context (node))
    {
      output_record_start (ob, NULL, NULL, LTO_local_type_ref);
      output_local_decl_ref (ob, node, true);
    }
  else
#endif
    {
      output_record_start (ob, NULL, NULL, LTO_global_type_ref);
      lto_output_type_ref_index (ob->decl_state, ob->main_stream, node);
    }

  LTO_DEBUG_UNDENT();
}


/* Look up NODE in the type table and write the uleb128 index for it to OB.
   This is a hack and will be replaced with a real reference to the type.  */

static void
output_type_ref (struct output_block *ob, tree node)
{
  LTO_DEBUG_TOKEN ("type_ref");
  output_type_ref_1 (ob, node);
}


/* Look up NAME in the type table and if WRITE, write the uleb128
   index for it to OB.  */

static unsigned int
output_local_decl_ref (struct output_block *ob, tree name, bool write)
{
  bool new;
  unsigned int index;

  new = lto_output_decl_index (write ? ob->main_stream : NULL, 
                               &ob->local_decl_encoder,
                               name, &index);
  /* Push the new local decl onto a vector for later processing.  */
  if (new)
    {
      VEC_safe_push (int, heap, ob->local_decls_index, 0);
      VEC_safe_push (int, heap, ob->unexpanded_local_decls_index, -1);
#ifdef LTO_STREAM_DEBUGGING
      VEC_safe_push (int, heap, ob->local_decls_index_d, 0);
#endif
    }
  return index;
}

/* Look up LABEL in the label table and write the uleb128 index for it.  */

static void
output_label_ref (struct output_block *ob, tree label)
{
  void **slot;
  struct lto_decl_slot d_slot;
  d_slot.t = label;

  slot = htab_find_slot (ob->label_hash_table, &d_slot, INSERT);
  if (*slot == NULL)
    {
      struct lto_decl_slot *new_slot
	= (struct lto_decl_slot *) xmalloc (sizeof (struct lto_decl_slot));

      /* Named labels are given positive integers and unnamed labels are 
	 given negative indexes.  */
      bool named = (DECL_NAME (label) != NULL_TREE);
      int index = named
	? ob->next_named_label_index++ : ob->next_unnamed_label_index--;

      new_slot->t = label;
      new_slot->slot_num = index;
      *slot = new_slot;
      output_sleb128 (ob, index);
      if (named)
	VEC_safe_push (tree, heap, ob->named_labels, label);
    }
  else
    {
      struct lto_decl_slot *old_slot = (struct lto_decl_slot *)*slot;
      output_sleb128 (ob, old_slot->slot_num);
    }
}


/* Output the start of a record with TAG and possibly flags for EXPR,
   and the TYPE for VALUE to OB.  */

static void
output_record_start (struct output_block *ob, tree expr,
		     tree value, unsigned int tag)
{
  lto_output_1_stream (ob->main_stream, tag);
  LTO_DEBUG_INDENT (tag);
  if (expr)
    {
      enum tree_code code = TREE_CODE (expr);
      if (value
	  && TEST_BIT (lto_types_needed_for, code)
	  && TREE_TYPE (value))
	output_type_ref (ob, TREE_TYPE (value));
      output_tree_flags (ob, code, expr, false);
    }
}


/* Output a LIST of TYPE_DECLS.  */

static void
output_type_list (struct output_block *ob, tree list)
{
  tree tl;
  int count = 0;
  if (list)
    {
      gcc_assert (TREE_CODE (list) == TREE_LIST);
      for (tl = list; tl; tl = TREE_CHAIN (tl))
	if (TREE_VALUE (tl) != NULL_TREE)
	  count++;

      output_uleb128 (ob, count);
      for (tl = list; tl; tl = TREE_CHAIN (tl))
	if (TREE_VALUE (tl) != NULL_TREE)
	  output_type_ref (ob, TREE_VALUE (tl));
    }
  else
    output_zero (ob);
}


/* Output an eh_cleanup region with REGION_NUMBER.  HAS_INNER is true if
   there are children of this node and HAS_PEER is true if there are
   siblings of this node.  MAY_CONTAIN_THROW and PREV_TRY are the
   fields of the eh_region.  */

static void
output_eh_cleanup (void *obv,
		   int region_number,
		   bool has_inner, bool has_peer,
		   bool may_contain_throw, int prev_try)
{
  struct output_block *ob = (struct output_block*)obv;
  output_record_start (ob, NULL, NULL,
		       LTO_eh_table_cleanup0 +
		     (has_inner ? 1 : 0) + (may_contain_throw ? 2 : 0));
  output_sleb128 (ob, region_number);
  output_sleb128 (ob, prev_try);
  if (!has_peer)
    output_zero (ob);
  LTO_DEBUG_UNDENT ();
}


/* Output an eh_try region with REGION_NUMBER.  HAS_INNER is true if
   there are children of this node and HAS_PEER is true if there are
   siblings of this node.  MAY_CONTAIN_THROW, CATCH and LAST_CATCH are
   the fields of the eh_region.  */

static void
output_eh_try (void *obv,
	       int region_number,
	       bool has_inner, bool has_peer,
	       bool may_contain_throw, int catch,
	       int last_catch)
{
  struct output_block *ob = (struct output_block*)obv;
  output_record_start (ob, NULL, NULL,
		       LTO_eh_table_try0 +
		     (has_inner ? 1 : 0) + (may_contain_throw ? 2 : 0));
  output_sleb128 (ob, region_number);
  output_sleb128 (ob, catch);
  output_sleb128 (ob, last_catch);
  if (!has_peer)
    output_zero (ob);
  LTO_DEBUG_UNDENT ();
}


/* Output an eh_catch region with REGION_NUMBER.  HAS_INNER is true if
   there are children of this node and HAS_PEER is true if there are
   siblings of this node.  MAY_CONTAIN_THROW, NEXT_CATCH and
   PREV_CATCH, and TYPE_LIST are the fields of the eh_region.  */

static void
output_eh_catch (void *obv,
		 int region_number,
		 bool has_inner, bool has_peer,
		 bool may_contain_throw, int next_catch,
		 int prev_catch, tree type_list)
{
  struct output_block *ob = (struct output_block*)obv;
  output_record_start (ob, NULL, NULL,
		       LTO_eh_table_catch0 +
		     (has_inner ? 1 : 0) + (may_contain_throw ? 2 : 0));
  output_sleb128 (ob, region_number);
  output_sleb128 (ob, next_catch);
  output_sleb128 (ob, prev_catch);
  output_type_list (ob, type_list);
  if (!has_peer)
    output_zero (ob);
  LTO_DEBUG_UNDENT ();
}

/* Output an eh_allowed_exceptions region with REGION_NUMBER.
   HAS_INNER is true if there are children of this node and HAS_PEER
   is true if there are siblings of this node.  MAY_CONTAIN_THROW, and
   TYPE_LIST are the fields of the eh_region.  */

static void
output_eh_allowed (void *obv,
		   int region_number,
		   bool has_inner, bool has_peer,
		   bool may_contain_throw, tree type_list)
{
  struct output_block *ob = (struct output_block*)obv;
  output_record_start (ob, NULL, NULL,
		       LTO_eh_table_allowed0 +
		     (has_inner ? 1 : 0) + (may_contain_throw ? 2 : 0));
  output_sleb128 (ob, region_number);
  output_type_list (ob, type_list);
  if (!has_peer)
    output_zero (ob);
  LTO_DEBUG_UNDENT ();
}


/* Output an eh_must_not_throw region with REGION_NUMBER.  HAS_INNER
   is true if there are children of this node and HAS_PEER is true if
   there are siblings of this node.  MAY_CONTAIN_THROW is the field of
   the eh_region.  */

static void
output_eh_must_not_throw (void *obv,
			  int region_number,
			  bool has_inner, bool has_peer,
			  bool may_contain_throw)
{
  struct output_block *ob = (struct output_block*)obv;
  output_record_start (ob, NULL, NULL,
		       LTO_eh_table_must_not_throw0 +
		     (has_inner ? 1 : 0) + (may_contain_throw ? 2 : 0));
  output_sleb128 (ob, region_number);
  if (!has_peer)
    output_zero (ob);
  LTO_DEBUG_UNDENT ();
}


/* Output the existing eh_table to OB.  */

static void
output_eh_regions (struct output_block *ob, struct function *fn)
{
  if (0 && fn->eh)
    {
      output_record_start (ob, NULL, NULL, LTO_eh_table);
      output_eh_records (ob, fn,
			 output_eh_cleanup,
			 output_eh_try,
			 output_eh_catch,
			 output_eh_allowed,
			 output_eh_must_not_throw);
      LTO_DEBUG_UNDENT ();
    }
  /* The 0 either terminates the record or indicates that there are no
     eh_records at all.  */
  output_zero (ob);
}


/* Output constructor CTOR to OB.  */

static void
output_constructor (struct output_block *ob, tree ctor)
{
  tree value;
  tree purpose;
  unsigned HOST_WIDE_INT idx;

  output_record_start (ob, ctor, ctor, LTO_constructor);
  output_uleb128 (ob, VEC_length (constructor_elt, CONSTRUCTOR_ELTS (ctor)));

  FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (ctor), idx, purpose, value)
    {
      if (purpose)
	output_expr_operand (ob, purpose);
      else 
	output_zero (ob);

      if (TREE_CODE (value) == CONSTRUCTOR)
	{
	  output_constructor (ob, value);
	  LTO_DEBUG_UNDENT ();
	}
      else 
	output_expr_operand (ob, value);
    }
}

/* Output EXPR to the main stream in OB.  */

static void
output_expr_operand (struct output_block *ob, tree expr)
{
  enum tree_code code;
  enum tree_code_class class;
  unsigned int tag;

  gcc_assert (expr);

  code = TREE_CODE (expr);
  class = TREE_CODE_CLASS (code);
  tag = expr_to_tag [code];

  gcc_assert (class != tcc_type);

  switch (code)
    {
    case COMPLEX_CST:
      if (TREE_CODE (TREE_REALPART (expr)) == REAL_CST)
	{
	  output_record_start (ob, expr, expr, LTO_complex_cst1);
	  output_type_ref (ob, TREE_TYPE (TREE_REALPART (expr)));
	  output_real (ob, TREE_REALPART (expr));
	  output_real (ob, TREE_IMAGPART (expr));
	}
      else
	{
	  output_record_start (ob, expr, expr, LTO_complex_cst0);
	  output_type_ref (ob, TREE_TYPE (TREE_REALPART (expr)));
	  output_integer (ob, TREE_REALPART (expr));
	  output_integer (ob, TREE_IMAGPART (expr));
	}
      break;

    case INTEGER_CST:
      output_record_start (ob, expr, expr, tag);
      output_integer (ob, expr);
      break;

    case REAL_CST:
      output_record_start (ob, expr, expr, tag);
      output_real (ob, expr);
      break;

    case STRING_CST:
      {
	/* Most STRING_CSTs have a type when they get here.  The ones
	   in the string operands of asms do not.  Put something there
	   so that all STRING_CSTs can be handled uniformly.  */
	if (!TREE_TYPE (expr))
	  TREE_TYPE (expr) = void_type_node;

	output_record_start (ob, expr, expr, LTO_string_cst);
	output_string (ob, ob->main_stream, 
		       TREE_STRING_POINTER (expr),
		       TREE_STRING_LENGTH (expr));
      }
      break;

    case IDENTIFIER_NODE:
      {
	output_record_start (ob, expr, expr, LTO_identifier_node);
	output_string (ob, ob->main_stream, 
		       IDENTIFIER_POINTER (expr),
		       IDENTIFIER_LENGTH (expr));
      }
      break;
      

    case VECTOR_CST:
      {
	tree t = TREE_VECTOR_CST_ELTS (expr);
	int len = 1;

	while ((t = TREE_CHAIN (t)) != NULL)
	  len++;
	t = TREE_VECTOR_CST_ELTS (expr);
	if (TREE_CODE (TREE_VALUE(t)) == REAL_CST)
	  {
	    output_record_start (ob, expr, expr, LTO_vector_cst1);
	    output_uleb128 (ob, len);
	    output_type_ref (ob, TREE_TYPE (TREE_VALUE (t)));
	    output_real (ob, TREE_VALUE (t));
	    while ((t = TREE_CHAIN (t)) != NULL)
	      output_real (ob, TREE_VALUE (t));
	  }
	else
	  {
	    output_record_start (ob, expr, expr, LTO_vector_cst0);
	    output_uleb128 (ob, len);
	    output_type_ref (ob, TREE_TYPE (TREE_VALUE (t)));
	    output_integer (ob, TREE_VALUE (t));
	    while ((t = TREE_CHAIN (t)) != NULL)
	      output_integer (ob, TREE_VALUE (t));
	  }
      }
      break;

    case CASE_LABEL_EXPR:
      {
	int variant = 0;
	if (CASE_LOW (expr) != NULL_TREE)
	  variant |= 0x1;
	if (CASE_HIGH (expr) != NULL_TREE)
	  variant |= 0x2;
	output_record_start (ob, expr, NULL,
			     LTO_case_label_expr0 + variant);

	if (CASE_LOW (expr) != NULL_TREE)
	  output_expr_operand (ob, CASE_LOW (expr));
	if (CASE_HIGH (expr) != NULL_TREE)
	  output_expr_operand (ob, CASE_HIGH (expr));
	output_label_ref (ob, CASE_LABEL (expr));
      }
      break;

    case CONSTRUCTOR:
      output_constructor (ob, expr);
      break;

    case SSA_NAME:
      output_record_start (ob, expr, expr, LTO_ssa_name);
      output_uleb128 (ob, SSA_NAME_VERSION (expr));
      break;

    case CONST_DECL:
      /* We should not see these.  */
      gcc_unreachable ();

    case FIELD_DECL:
      if (!field_decl_is_local (expr))
        {
          output_record_start (ob, NULL, NULL, LTO_field_decl1);
          lto_output_field_decl_index (ob->decl_state, ob->main_stream, expr);
        }
      else
        {
	  /* Local FIELD_DECLs.  */
	  output_record_start (ob, NULL, NULL, LTO_field_decl0);
	  output_local_decl_ref (ob, expr, true);
        }
      break;

    case FUNCTION_DECL:
      /* FIXME: Local FUNCTION_DECLS are possible, i.e.,
         nested functions.  */
      output_record_start (ob, NULL, NULL, tag);
      lto_output_fn_decl_index (ob->decl_state, ob->main_stream, expr);
      break;

    case VAR_DECL:
      if (TREE_STATIC (expr) || DECL_EXTERNAL (expr))
	{
	  output_record_start (ob, NULL, NULL, LTO_var_decl1);
	  lto_output_var_decl_index (ob->decl_state, ob->main_stream, expr);
	}
      else
	{
	  /* Local VAR_DECLs.  */
	  output_record_start (ob, NULL, NULL, LTO_var_decl0);
	  output_local_decl_ref (ob, expr, true);
	}
      break;

    case TYPE_DECL:
      if (!type_decl_is_local (expr))
        {
          output_record_start (ob, NULL, NULL, LTO_type_decl1);
          lto_output_type_decl_index (ob->decl_state, ob->main_stream, expr);
        }
      else
        {
	  /* Local TYPE_DECLs.  */
	  output_record_start (ob, NULL, NULL, LTO_type_decl0);
	  output_local_decl_ref (ob, expr, true);
        }
      break;

    case NAMESPACE_DECL:
      output_record_start (ob, NULL, NULL, tag);
      lto_output_namespace_decl_index (ob->decl_state, ob->main_stream, expr);
      break;

    case PARM_DECL:
      gcc_assert (!DECL_RTL_SET_P (expr));
      output_record_start (ob, NULL, NULL, tag);
      output_local_decl_ref (ob, expr, true);
      break;

    case LABEL_DECL:
      output_record_start (ob, expr, NULL, tag);
      output_label_ref (ob, expr);
      break;

    case LABEL_EXPR:
      output_record_start (ob, expr, NULL, tag);
      output_label_ref (ob, TREE_OPERAND (expr, 0));
      break;

    case COND_EXPR:
      if (TREE_OPERAND (expr, 1))
	{
	  output_record_start (ob, expr, expr, LTO_cond_expr0);
	  output_expr_operand (ob, TREE_OPERAND (expr, 0));
	  output_expr_operand (ob, TREE_OPERAND (expr, 1));
	  output_expr_operand (ob, TREE_OPERAND (expr, 2));
	}
      else 
	{
	  output_record_start (ob, expr, expr, LTO_cond_expr1);
	  output_expr_operand (ob, TREE_OPERAND (expr, 0));
	}
      break;

    case RESULT_DECL:
      output_record_start (ob, expr, expr, tag);
      break;

    case COMPONENT_REF:
      output_record_start (ob, expr, expr, tag);
      output_expr_operand (ob, TREE_OPERAND (expr, 0));
      output_expr_operand (ob, TREE_OPERAND (expr, 1));
      /* Ignore 3 because it can be recomputed.  */
      break;

    case CALL_EXPR:
      {
	unsigned int count = TREE_INT_CST_LOW (TREE_OPERAND (expr, 0));
	unsigned int i;

	/* Operand 2 is the call chain.  */
	if (TREE_OPERAND (expr, 2))
	  {
	    output_record_start (ob, expr, expr, LTO_call_expr1);
	    output_uleb128 (ob, count);
	    output_expr_operand (ob, TREE_OPERAND (expr, 2));
	  }
	else
	  {
	    output_record_start (ob, expr, expr, LTO_call_expr0);
	    output_uleb128 (ob, count);
	  }
	output_expr_operand (ob, TREE_OPERAND (expr, 1));
	for (i = 3; i < count; i++)
	  output_expr_operand (ob, TREE_OPERAND (expr, i));
      }
      break;

    case BIT_FIELD_REF:
      {
	tree op1 = TREE_OPERAND (expr, 1);
	tree op2 = TREE_OPERAND (expr, 2);
	if ((TREE_CODE (op1) == INTEGER_CST)
	    && (TREE_CODE (op2) == INTEGER_CST))
	  {
	    output_record_start (ob, expr, expr,
				 LTO_bit_field_ref1);
	    output_uleb128 (ob, TREE_INT_CST_LOW (op1));
	    output_uleb128 (ob, TREE_INT_CST_LOW (op2));
	    output_expr_operand (ob, TREE_OPERAND (expr, 0));
	  }
	else
	  {
	    output_record_start (ob, expr, expr,
				 LTO_bit_field_ref0);
	    output_expr_operand (ob, TREE_OPERAND (expr, 0));
	    output_expr_operand (ob, op1);
	    output_expr_operand (ob, op2);
	  }
      }
      break;

    case ARRAY_REF:
    case ARRAY_RANGE_REF:
      /* Ignore operands 2 and 3 for ARRAY_REF and ARRAY_RANGE REF
	 because they can be recomputed.  */
      output_record_start (ob, expr, expr, tag);
      output_expr_operand (ob, TREE_OPERAND (expr, 0));
      output_expr_operand (ob, TREE_OPERAND (expr, 1));
      break;


    case ASM_EXPR:
      {
	tree string_cst = ASM_STRING (expr);
	output_record_start (ob, expr, NULL, LTO_asm_expr);
	output_string (ob, ob->main_stream, 
		       TREE_STRING_POINTER (string_cst),
		       TREE_STRING_LENGTH (string_cst));
	if (ASM_INPUTS (expr))
	  output_expr_operand (ob, ASM_INPUTS (expr));
	else 
	  output_zero (ob);

	if (ASM_OUTPUTS (expr))
	  output_expr_operand (ob, ASM_OUTPUTS (expr));
	else 
	  output_zero (ob);

	if (ASM_CLOBBERS (expr))
	  output_expr_operand (ob, ASM_CLOBBERS (expr));
	else 
	  output_zero (ob);
      }
      break;

    case RANGE_EXPR:
      {
	output_record_start (ob, NULL, NULL, LTO_range_expr);
	/* Need the types here to reconstruct the ranges.  */
	output_type_ref (ob, TREE_OPERAND (expr, 0));
	output_integer (ob, TREE_OPERAND (expr, 0));
	output_type_ref (ob, TREE_OPERAND (expr, 1));
	output_integer (ob, TREE_OPERAND (expr, 1));
      }
      break; 

    case RESX_EXPR:
      output_record_start (ob, expr, NULL, tag);
      output_uleb128 (ob, TREE_INT_CST_LOW (TREE_OPERAND (expr, 0)));
      break;

    case RETURN_EXPR:
      {
	tree t = TREE_OPERAND (expr, 0);
	if (t == NULL)
	  {
	    /* Form return.  */
	    output_record_start (ob, expr, expr,
				 LTO_return_expr0);
	  }
	else if (TREE_CODE (t) == MODIFY_EXPR)
	  {
	    /* Form return a = b;  */
	    output_record_start (ob, expr, expr,
				 LTO_return_expr2);
	    output_expr_operand (ob, TREE_OPERAND (t, 0));
	    output_expr_operand (ob, TREE_OPERAND (t, 1));
	  }
	else
	  {
	    /* Form return a; */
	    output_record_start (ob, expr, expr,
				 LTO_return_expr1);
            /* If the type of the argument is a type that gets returned
               in memory, then the gimplifier would have changed the
               argument of the RETURN_EXPR to point at DECL_RESULT of
               the current function.  Communicate this fact to the
               reader so we avoid reading in superfluous trees.  */
            if (t == DECL_RESULT (current_function_decl))
              output_zero (ob);
            else
              output_expr_operand (ob, t);
	  }
      }
      break;

    case GIMPLE_MODIFY_STMT:
      output_record_start (ob, expr, NULL, tag);
      output_expr_operand (ob, GIMPLE_STMT_OPERAND (expr, 0));
      output_expr_operand (ob, GIMPLE_STMT_OPERAND (expr, 1));
      break;

    case SWITCH_EXPR:
      {
	tree label_vec = TREE_OPERAND (expr, 2);
	size_t len = TREE_VEC_LENGTH (label_vec);
	size_t i;
	output_record_start (ob, expr, expr, tag);
	output_uleb128 (ob, len);
	output_expr_operand (ob, TREE_OPERAND (expr, 0));
	gcc_assert (TREE_OPERAND (expr, 1) == NULL);

	for (i = 0; i < len; ++i)
	  output_expr_operand (ob, TREE_VEC_ELT (label_vec, i));
      }
      break;

    case TREE_LIST:
      {
	tree tl;
	int count = 0;

	output_record_start (ob, expr, NULL, tag);
	for (tl = expr; tl; tl = TREE_CHAIN (tl))
	  count++;

	gcc_assert (count);
	output_uleb128 (ob, count);
	for (tl = expr; tl; tl = TREE_CHAIN (tl))
	  {
	    if (TREE_VALUE (tl) != NULL_TREE)
	      output_expr_operand (ob, TREE_VALUE (tl));
	    else
	      output_zero (ob);
	    
	    if (TREE_PURPOSE (tl))
	      output_expr_operand (ob, TREE_PURPOSE (tl));
	    else
	      output_zero (ob);
	  }
      }
      break;

      /* This is the default case. All of the cases that can be done
	 completely mechanically are done here.  */
      {
	int i;
#define SET_NAME(a,b)
#define MAP_EXPR_TAG(expr, tag) case expr:
#define TREE_SINGLE_MECHANICAL_TRUE

#include "lto-tree-tags.def"
#undef MAP_EXPR_TAG
#undef TREE_SINGLE_MECHANICAL_TRUE
#undef SET_NAME
	output_record_start (ob, expr, expr, tag);
	for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (expr)); i++)
	  output_expr_operand (ob, TREE_OPERAND (expr, i));
	break;
      }

      /* This is the error case, these are type codes that will either
	 never happen or that we have not gotten around to dealing
	 with are here.  */
    case BIND_EXPR:
    case BLOCK:
    case CATCH_EXPR:
    case EH_FILTER_EXPR:
    case NAME_MEMORY_TAG:
    case OMP_CONTINUE:
    case OMP_CRITICAL:
    case OMP_FOR:
    case OMP_MASTER:
    case OMP_ORDERED:
    case OMP_PARALLEL:
    case OMP_RETURN:
    case OMP_SECTIONS:
    case OMP_SINGLE:
    case SYMBOL_MEMORY_TAG:
    case TARGET_MEM_REF:
    case TRY_CATCH_EXPR:
    case TRY_FINALLY_EXPR:
    default:
      /* We cannot have forms that are not explicity handled.  So when
	 this is triggered, there is some form that is not being
	 output.  */
      gcc_unreachable ();
    }

  LTO_DEBUG_UNDENT ();
}


/* Output the local var at INDEX to OB.  */

static void
output_local_var_decl (struct output_block *ob, int index)
{
  tree decl = lto_tree_ref_encoder_get_tree (&ob->local_decl_encoder, index);
  unsigned int variant = 0;
  bool is_var = (TREE_CODE (decl) == VAR_DECL);
  bool needs_backing_var
    = (DECL_DEBUG_EXPR_IS_FROM (decl) && DECL_DEBUG_EXPR (decl));
  
  /* This will either be a local var decl or a parm decl. */
  unsigned int tag;
  
  /* FIXME lto: If we agree that is is correct for reset_lang_specific
     to null out the DECL_SIZE expression for variably-modified types,
     then we cannot assume that DECL_SIZE is non-null here.  This assertion
     fails on gcc.c-torture/compile/20020210-1.c for that reason.  */
  /*gcc_assert (DECL_SIZE (decl));*/
  variant |= DECL_ATTRIBUTES (decl)      != NULL_TREE ? 0x01 : 0;
  variant |= DECL_SIZE_UNIT (decl)       != NULL_TREE ? 0x02 : 0;
  variant |= needs_backing_var                        ? 0x04 : 0;
  variant |= DECL_ABSTRACT_ORIGIN (decl) != NULL_TREE ? 0x08 : 0;
  
  tag = (is_var
	 ? LTO_local_var_decl_body0
	 : LTO_parm_decl_body0)
    + variant;

  output_record_start (ob, NULL, NULL, tag);

  /* Put out the name if there is one.  */
  if (DECL_NAME (decl))
    {
      tree name = DECL_NAME (decl);
      output_string (ob, ob->main_stream, 
		     IDENTIFIER_POINTER (name), 
		     IDENTIFIER_LENGTH (name));
    }
  else
    output_zero (ob);
  
  output_type_ref (ob, TREE_TYPE (decl));
  
  if (is_var)
    {
      LTO_DEBUG_INDENT_TOKEN ("init");
      if (DECL_INITIAL (decl))
	output_expr_operand (ob, DECL_INITIAL (decl));
      else
	output_zero (ob);
      /* Index in unexpanded_vars_list.  */
      LTO_DEBUG_INDENT_TOKEN ("local decl index");
      output_sleb128 (ob, VEC_index (int, ob->unexpanded_local_decls_index, index));
    }
  else
    {
      output_type_ref (ob, DECL_ARG_TYPE (decl));
      /* The chain is only necessary for parm_decls.  */
      LTO_DEBUG_TOKEN ("chain");
      if (TREE_CHAIN (decl))
	output_expr_operand (ob, TREE_CHAIN (decl));
      else
	output_zero (ob);
    }

  clear_line_info (ob);
  output_tree_flags (ob, 0, decl, true);

  LTO_DEBUG_TOKEN ("context");
  if (DECL_CONTEXT (decl))
    output_expr_operand (ob, DECL_CONTEXT (decl));
  else 
    output_zero (ob);
  
  LTO_DEBUG_TOKEN ("align");
  output_uleb128 (ob, DECL_ALIGN (decl));
  
  /* Put out the subtrees.  */
  LTO_DEBUG_TOKEN ("size");
  /* Note that DECL_SIZE might be NULL_TREE for a
     variably-modified type.  See reset_lang_specific
     and the comment above.  */
  if (DECL_SIZE (decl))
    output_expr_operand (ob, DECL_SIZE (decl));
  else
    output_zero (ob);
  if (DECL_ATTRIBUTES (decl)!= NULL_TREE)
    {
      LTO_DEBUG_TOKEN ("attributes");
      output_expr_operand (ob, DECL_ATTRIBUTES (decl));
    }
  if (DECL_SIZE_UNIT (decl) != NULL_TREE)
    output_expr_operand (ob, DECL_SIZE_UNIT (decl));
  if (needs_backing_var)
    output_expr_operand (ob, DECL_DEBUG_EXPR (decl));
  if (DECL_ABSTRACT_ORIGIN (decl) != NULL_TREE)
    output_expr_operand (ob, DECL_ABSTRACT_ORIGIN (decl));
  
  LTO_DEBUG_UNDENT();
}


/* Emit tree node EXPR to output block OB.  */

static void
output_local_tree (struct output_block *ob, tree expr)
{
  if (expr == NULL_TREE)
    output_zero (ob);
  else if (TYPE_P (expr))
    output_type_ref_1 (ob, expr);
  else
    output_expr_operand (ob, expr);
}


/* Output the local field declaration DECL to OB.
   A "local" field declaration is one that that has a
   dependency on a function context, and should always
   have been declared within a function.  */

static void
output_local_field_decl (struct output_block *ob, tree decl)
{
  /* tag and flags */
  output_record_start (ob, NULL, NULL, LTO_field_decl0);
  output_tree_flags (ob, 0, decl, true);

  /* uid and locus are handled specially */
  output_local_tree (ob, decl->decl_minimal.name);
  output_local_tree (ob, decl->decl_minimal.context);

  output_local_tree (ob, decl->common.type);

  output_local_tree (ob, decl->decl_common.attributes);
  output_local_tree (ob, decl->decl_common.abstract_origin);

  output_uleb128 (ob, decl->decl_common.mode);
  output_uleb128 (ob, decl->decl_common.align);
  output_uleb128 (ob, decl->decl_common.off_align);

  output_local_tree (ob, decl->decl_common.size);
  output_local_tree (ob, decl->decl_common.size_unit);

  output_local_tree (ob, decl->field_decl.offset);
  output_local_tree (ob, decl->field_decl.bit_field_type);
  output_local_tree (ob, decl->field_decl.qualifier);
  output_local_tree (ob, decl->field_decl.bit_offset);
  output_local_tree (ob, decl->field_decl.fcontext);

  /* lang_specific */
  output_local_tree (ob, decl->decl_common.initial);

  /* Write out current field before its siblings,
     so follow the chain last.  */
  output_local_tree (ob, decl->common.chain);

  LTO_DEBUG_UNDENT();
}


/* Output the local type declaration DECL to OB.
   A "local" type declaration  is one that that has a
   dependency on a function context, and should always
   have been declared within a function.  */

static void
output_local_type_decl (struct output_block *ob, tree decl)
{
  /* tag and flags */
  output_record_start (ob, NULL, NULL, LTO_type_decl0);
  output_tree_flags (ob, 0, decl, true);

  /* uid and locus are handled specially */
  /* Must output name before type.  */
  output_local_tree (ob, decl->decl_minimal.name);
  output_local_tree (ob, decl->decl_minimal.context);

  output_local_tree (ob, decl->decl_with_vis.assembler_name);
  output_local_tree (ob, decl->decl_with_vis.section_name);

  output_local_tree (ob, decl->common.type);

  output_local_tree (ob, decl->decl_common.attributes);
  output_local_tree (ob, decl->decl_common.abstract_origin);

  output_uleb128 (ob, decl->decl_common.mode);
  output_uleb128 (ob, decl->decl_common.align);

  output_local_tree (ob, decl->decl_common.size);		/* ??? */
  output_local_tree (ob, decl->decl_common.size_unit);		/* ??? */

  /* lang_specific */

  gcc_assert (decl->decl_with_rtl.rtl == NULL);

  output_local_tree (ob, decl->decl_common.initial);		/* ??? */

  output_local_tree (ob, decl->decl_non_common.saved_tree);	/* ??? */
  output_local_tree (ob, decl->decl_non_common.arguments);	/* ??? */
  output_local_tree (ob, decl->decl_non_common.result);
  output_local_tree (ob, decl->decl_non_common.vindex);		/* ??? */

  LTO_DEBUG_UNDENT();
}


/* Output the local type TYPE to OB.
   A "local" type is one that that has a dependency on a
   function context, as the type of local var decl or
   type decl, or a type that is variably-modified with a
   dependency on a local variable or parameter.  */

static void
output_local_type (struct output_block *ob, tree type, enum LTO_tags tag)
{
  /* FIXME lto:  Support for local types is not used, and has not kept
     up with changes to the global type streamer.  Make sure we take
     another look if we start using it again.  */
  gcc_unreachable ();

  /* tag and flags */
  output_record_start (ob, NULL, NULL, tag);
  output_tree_flags (ob, 0, type, false);

  LTO_DEBUG_TOKEN ("type");
  output_local_tree (ob, type->common.type);
  LTO_DEBUG_TOKEN ("size");
  output_local_tree (ob, type->type.size);
  LTO_DEBUG_TOKEN ("size_unit");
  output_local_tree (ob, type->type.size_unit);
  LTO_DEBUG_TOKEN ("attributes");
  output_local_tree (ob, type->type.attributes);
  LTO_DEBUG_TOKEN ("uid");
  output_uleb128 (ob, type->type.uid);
  LTO_DEBUG_TOKEN ("precision");
  output_uleb128 (ob, type->type.precision);
  LTO_DEBUG_TOKEN ("mode");
  output_uleb128 (ob, type->type.mode);
  LTO_DEBUG_TOKEN ("align");
  output_uleb128 (ob, type->type.align);
  LTO_DEBUG_TOKEN ("pointer_to");
  output_local_tree (ob, type->type.pointer_to);
  LTO_DEBUG_TOKEN ("reference_to");
  output_local_tree (ob, type->type.reference_to);
  /* FIXME: Output symtab here.  Do we need it?  */
  LTO_DEBUG_TOKEN ("name");
  output_local_tree (ob, type->type.name);	/* may be a TYPE_DECL */
  LTO_DEBUG_TOKEN ("minval");
  output_local_tree (ob, type->type.minval);
  LTO_DEBUG_TOKEN ("maxval");
  output_local_tree (ob, type->type.maxval);
  LTO_DEBUG_TOKEN ("next_variant");
  output_local_tree (ob, type->type.next_variant);
  LTO_DEBUG_TOKEN ("main_variant");
  output_local_tree (ob, type->type.main_variant);
  /* FIXME: Handle BINFO.  */
  /*
    LTO_DEBUG_TOKEN ("binfo");
    output_local_tree (ob, type->type.binfo);
  */
  LTO_DEBUG_TOKEN ("context");
  /* FIXME: We don't emit BLOCKs, so context must be a
     function or toplevel.  We probably break debugging.  */
  /*output_tree (ob, type->type.context);*/
  output_local_tree (ob, type_function_context (type));
  LTO_DEBUG_TOKEN ("canonical");
  output_local_tree (ob, type->type.canonical);

  /* Slot 'values' may be the structures fields, so do them last,
     after other slots of the structure type have been filled in.  */
  LTO_DEBUG_TOKEN ("values");
  if (TYPE_CACHED_VALUES_P (type))
    {
      gcc_assert (tag != RECORD_TYPE
                  && tag != UNION_TYPE
                  && tag != ARRAY_TYPE);
      /* Don't stream the values cache.  We must clear flag
         TYPE_CACHED_VALUES_P on input.  We don't do it here
         because we don't want to clobber the tree as we write
         it, and there is no infrastructure for modifying
         flags as we serialize them.  */
      output_zero (ob);
    }
  else
    output_local_tree (ob, type->type.values);	/* should be a TREE_VEC */

  LTO_DEBUG_TOKEN ("chain");
  output_local_tree (ob, type->common.chain);	/* overloaded as TYPE_STUB_DECL */

  LTO_DEBUG_UNDENT();
}


/* Output the local declaration or type at INDEX to OB.  */

static void
output_local_decl (struct output_block *ob, int index)
{
  tree decl = lto_tree_ref_encoder_get_tree (&ob->local_decl_encoder, index);

  /* DEBUG */
  /*
    fprintf (stderr, "LOCAL: ");
    print_generic_expr (stderr, decl, 0);
    fprintf (stderr, "\n");
  */

  VEC_replace (int, ob->local_decls_index, index, ob->main_stream->total_size);
#ifdef LTO_STREAM_DEBUGGING
  VEC_replace (int, ob->local_decls_index_d, index, ob->debug_decl_stream->total_size);
#endif

  if (TREE_CODE (decl) == FIELD_DECL)
    output_local_field_decl (ob, decl);
  else if (TREE_CODE (decl) == VAR_DECL
           || TREE_CODE (decl) == PARM_DECL)
    output_local_var_decl (ob, index);
  else if (TREE_CODE (decl) == TYPE_DECL)
    output_local_type_decl (ob, decl);
  else
    {
      gcc_assert (TYPE_P (decl));

      output_local_type (ob, decl, expr_to_tag [TREE_CODE (decl)]);
    }
}


/* Output the local declarations and types to OB.  */

static void
output_local_vars (struct output_block *ob, struct function *fn)
{
  unsigned int index = 0;
  tree t;
  int i = 0;
  struct lto_output_stream *tmp_stream = ob->main_stream;
  bitmap local_statics = BITMAP_ALLOC (NULL);

  ob->main_stream = ob->local_decl_stream;

  /* We have found MOST of the local vars by scanning the function.
     However, many local vars have other local vars inside them.
     Other local vars can be found by walking the unexpanded vars
     list.  */

  /* Need to put out the local statics first, to avoid the pointer
     games used for the regular locals.  */
  LTO_DEBUG_TOKEN ("local statics");
  for (t = fn->local_decls; t; t = TREE_CHAIN (t))
    {
      tree lv = TREE_VALUE (t);

      if (TREE_STATIC (lv))
	{
	  /* Do not put the static in the chain more than once, even
	     if it was in the chain more than once to start.  */
	  if (!bitmap_bit_p (local_statics, DECL_UID (lv)))
	    {
	      bitmap_set_bit (local_statics, DECL_UID (lv));
	      output_expr_operand (ob, lv);
	      if (DECL_CONTEXT (lv) == fn->decl)
		output_uleb128 (ob, 1); /* Restore context.  */
	      else
		output_zero (ob); /* Restore context. */
	      if (DECL_INITIAL (lv))
		output_expr_operand (ob, DECL_INITIAL (lv));
	      else 
		output_zero (ob); /* DECL_INITIAL.  */
	    }
	}
      else
	{
	  int j = output_local_decl_ref (ob, lv, false);
	  /* Just for the fun of it, some of the locals are in the
	     local_decls_list more than once.  */
	  if (VEC_index (int, ob->unexpanded_local_decls_index, j) == -1)
	    VEC_replace (int, ob->unexpanded_local_decls_index, j, i++);
	}
    }


  /* End of statics.  */
  output_zero (ob);
  BITMAP_FREE (local_statics);

  /* The easiest way to get all of this stuff generated is to play
     pointer games with the streams and reuse the code for putting out
     the function bodies for putting out the local decls.  It needs to
     go into a separate stream because the LTO reader will want to
     process the local variables first, rather than have to back patch
     them.  */
  LTO_DEBUG_TOKEN ("local vars");
  while (index < lto_tree_ref_encoder_size (&ob->local_decl_encoder))
    output_local_decl (ob, index++);

  ob->main_stream = tmp_stream;
}


/* Output the local var_decls index and parm_decls index to OB.  */

static void
output_local_vars_index (struct output_block *ob)
{
  unsigned int index = 0;
  unsigned int stop;

  struct lto_output_stream *tmp_stream = ob->main_stream;
  ob->main_stream = ob->local_decl_index_stream;

  stop = VEC_length (int, ob->local_decls_index);
  for (index = 0; index < stop; index++)
    {
      output_uleb128 (ob, VEC_index (int, ob->local_decls_index, index));
#ifdef LTO_STREAM_DEBUGGING
      output_uleb128 (ob, VEC_index (int, ob->local_decls_index_d, index));
#endif
    }
  ob->main_stream = tmp_stream;
}


/* Output the names in the named labels to the named_label stream.  */

static void
output_named_labels (struct output_block *ob)
{
  unsigned int index = 0;
  clear_line_info (ob);

  while (index < VEC_length (tree, ob->named_labels))
    {
      tree decl = VEC_index (tree, ob->named_labels, index++);
      tree name = DECL_NAME (decl);
      output_string (ob, ob->named_label_stream, 
		     IDENTIFIER_POINTER (name), 
		     IDENTIFIER_LENGTH (name));
   }
}


/* Output all of the active ssa names to the ssa_names stream.  */

static void
output_ssa_names (struct output_block *ob, struct function *fn)
{
  /* Switch streams so we can use output_expr_operand to write the
     SSA_NAME_VAR.  */
  struct lto_output_stream *tmp_stream = ob->main_stream;
  unsigned int i;
  unsigned int len = VEC_length (tree, SSANAMES (fn));

  ob->main_stream = ob->ssa_names_stream;
  output_uleb128 (ob, len);

  for (i = 1; i < len; i++)
    {
      tree ptr = VEC_index (tree, SSANAMES (fn), i);

      if (ptr == NULL_TREE || SSA_NAME_IN_FREE_LIST (ptr))
	continue;

      output_uleb128 (ob, i);
      output_expr_operand (ob, SSA_NAME_VAR (ptr));
      /* Use code 0 to force flags to be output.  */
      output_tree_flags (ob, 0, ptr, false);
    }

  output_zero (ob);
  ob->main_stream = tmp_stream;
}


/* Output the cfg.  */

static void
output_cfg (struct output_block *ob, struct function *fn)
{
  struct lto_output_stream *tmp_stream = ob->main_stream;
  basic_block bb;

  ob->main_stream = ob->cfg_stream;

  /* Output the number of the highest basic block.  */
  LTO_DEBUG_TOKEN ("lastbb");
  output_uleb128 (ob, last_basic_block_for_function(fn));

  FOR_ALL_BB_FN (bb, fn)
    {
      edge_iterator ei;
      edge e;

      LTO_DEBUG_TOKEN ("bbindex");
      output_sleb128 (ob, bb->index);

      /* Output the successors and the edge flags.  */
      LTO_DEBUG_TOKEN ("edgecount");
      output_uleb128 (ob, EDGE_COUNT (bb->succs));
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  LTO_DEBUG_TOKEN ("dest");
	  output_uleb128 (ob, e->dest->index);
	  LTO_DEBUG_TOKEN ("eflags");
	  output_uleb128 (ob, e->flags);
	}
    }

  LTO_DEBUG_TOKEN ("bbindex");
  output_sleb128 (ob, -1);

  bb = ENTRY_BLOCK_PTR;
  while (bb->next_bb)
    {
      LTO_DEBUG_TOKEN ("bbchain");
      output_sleb128 (ob, bb->next_bb->index);
      bb = bb->next_bb;
    }
  LTO_DEBUG_TOKEN ("bbchain");
  output_sleb128 (ob, -1);

  ob->main_stream = tmp_stream;
}


/* Output a phi function to the main stream in OB.  */

static void
output_phi (struct output_block *ob, tree expr)
{
  int len = PHI_NUM_ARGS (expr);
  int i;
  
  output_record_start (ob, expr, expr, LTO_phi_node);
  output_uleb128 (ob, SSA_NAME_VERSION (PHI_RESULT (expr)));

  for (i = 0; i < len; i++)
    {
      output_expr_operand (ob, PHI_ARG_DEF (expr, i));
      output_uleb128 (ob, PHI_ARG_EDGE (expr, i)->src->index);
    }
  LTO_DEBUG_UNDENT ();
}


/* Output a basic block BB to the main stream in OB for this FN.  */

static void
output_bb (struct output_block *ob, basic_block bb, struct function *fn)
{
  block_stmt_iterator bsi = bsi_start (bb);

  output_record_start (ob, NULL, NULL,
		       (!bsi_end_p (bsi)) || phi_nodes (bb) ? LTO_bb1 : LTO_bb0);

  /* The index of the basic block.  */
  LTO_DEBUG_TOKEN ("bbindex");
  output_uleb128 (ob, bb->index);

  if ((!bsi_end_p (bsi)) || phi_nodes (bb))
    {
      /* Output the statements.  The list of statements is terminated
	 with a zero.  */
      tree phi;

      for (bsi = bsi_start (bb);
	   !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  tree stmt = bsi_stmt (bsi);

	  LTO_DEBUG_INDENT_TOKEN ("stmt");
	  output_expr_operand (ob, stmt);
	
	  /* We only need to set the region number of the tree that
	     could throw if the region number is different from the
	     last region number we set.  */
	  if (0 && tree_could_throw_p (stmt))
	    {
	      int region = lookup_stmt_eh_region_fn (fn, stmt);
	      if (region != last_eh_region_seen)
		{
		  output_record_start (ob, NULL, NULL,
				       LTO_set_eh0 + region ? 1 : 0);
		  if (region)
		    output_sleb128 (ob, region);
		
		  last_eh_region_seen = region;
		}
	    }
	}

      LTO_DEBUG_INDENT_TOKEN ("stmt");
      output_zero (ob);

      for (phi = phi_nodes (bb); phi; phi = PHI_CHAIN (phi))
	{
	  LTO_DEBUG_INDENT_TOKEN ("phi");
	  output_phi (ob, phi);
	}

      LTO_DEBUG_INDENT_TOKEN ("phi");
      output_zero (ob);
    }

  LTO_DEBUG_UNDENT();

#ifdef LTO_STREAM_DEBUGGING
  gcc_assert (lto_debug_context.indent == 1);
#endif
}

/* Create the header in the file using OB.  If the section type is for
   a function, set FN to the decl for that function.  */

static void
produce_asm (struct output_block *ob, tree fn)
{
  enum lto_section_type section_type = ob->section_type;
  struct lto_function_header header;
  char *section_name;
  struct lto_output_stream *header_stream;

  if (section_type == LTO_section_function_body)
    {
      const char *name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (fn));
      section_name = lto_get_section_name (section_type, name);
    }
  else
    section_name = lto_get_section_name (section_type, NULL);

  lto_begin_section (section_name);
  free (section_name);

  /* The entire header is stream computed here.  */
  memset (&header, 0, sizeof (struct lto_function_header));
  
  /* Write the header.  */
  header.lto_header.major_version = LTO_major_version;
  header.lto_header.minor_version = LTO_minor_version;
  header.lto_header.section_type = section_type;
  
  header.num_local_decls =
    lto_tree_ref_encoder_size (&ob->local_decl_encoder);
  header.num_named_labels = ob->next_named_label_index;
  header.num_unnamed_labels = -ob->next_unnamed_label_index;
  header.compressed_size = 0;
  
  header.named_label_size = ob->named_label_stream->total_size;
  if (section_type == LTO_section_function_body)
    {
      header.ssa_names_size = ob->ssa_names_stream->total_size;
      header.cfg_size = ob->cfg_stream->total_size;
      header.local_decls_index_size = ob->local_decl_index_stream->total_size;
      header.local_decls_size = ob->local_decl_stream->total_size;
    }
  header.main_size = ob->main_stream->total_size;
  header.string_size = ob->string_stream->total_size;
#ifdef LTO_STREAM_DEBUGGING
  header.debug_label_size = ob->debug_label_stream->total_size;
  if (section_type == LTO_section_function_body)
    {
      header.debug_decl_index_size = ob->debug_decl_index_stream->total_size;
      header.debug_decl_size = ob->debug_decl_stream->total_size;
      header.debug_ssa_names_size = ob->debug_ssa_names_stream->total_size;
      header.debug_cfg_size = ob->debug_cfg_stream->total_size;
    }
  header.debug_main_size = ob->debug_main_stream->total_size;
#else
  header.debug_decl_index_size = -1;
  header.debug_decl_size = -1;
  header.debug_label_size = -1;
  header.debug_ssa_names_size = -1;
  header.debug_cfg_size = -1;
  header.debug_main_size = -1;
#endif

  header_stream = ((struct lto_output_stream *)
		   xcalloc (1, sizeof (struct lto_output_stream)));
  lto_output_data_stream (header_stream, &header, sizeof header);
  lto_write_stream (header_stream);
  free (header_stream);

  /* Put all of the gimple and the string table out the asm file as a
     block of text.  */
  lto_write_stream (ob->named_label_stream);
  if (section_type == LTO_section_function_body)
    {
      lto_write_stream (ob->ssa_names_stream);
      lto_write_stream (ob->cfg_stream);
      lto_write_stream (ob->local_decl_index_stream);
      lto_write_stream (ob->local_decl_stream);
    }
  lto_write_stream (ob->main_stream);
  lto_write_stream (ob->string_stream);
#ifdef LTO_STREAM_DEBUGGING
  if (section_type == LTO_section_function_body)
    {
      lto_write_stream (ob->debug_decl_index_stream);
      lto_write_stream (ob->debug_decl_stream);
    }
  lto_write_stream (ob->debug_label_stream);
  if (section_type == LTO_section_function_body)
    {
      lto_write_stream (ob->debug_ssa_names_stream);
      lto_write_stream (ob->debug_cfg_stream);
    }
  lto_write_stream (ob->debug_main_stream);
#endif

  lto_end_section ();
}


static bool initialized = false;
static bool initialized_local = false;


/* Static initialization for both the lto reader and the lto
   writer.  */

void
lto_static_init (void)
{
  if (initialized)
    return;

  initialized = true;

  lto_flags_needed_for = sbitmap_alloc (NUM_TREE_CODES);
  sbitmap_ones (lto_flags_needed_for);
  RESET_BIT (lto_flags_needed_for, FIELD_DECL);
  RESET_BIT (lto_flags_needed_for, FUNCTION_DECL);
  RESET_BIT (lto_flags_needed_for, IDENTIFIER_NODE);
  RESET_BIT (lto_flags_needed_for, PARM_DECL);
  RESET_BIT (lto_flags_needed_for, SSA_NAME);
  RESET_BIT (lto_flags_needed_for, VAR_DECL);
  RESET_BIT (lto_flags_needed_for, TREE_LIST);
  RESET_BIT (lto_flags_needed_for, TREE_VEC);
  RESET_BIT (lto_flags_needed_for, TYPE_DECL);
  RESET_BIT (lto_flags_needed_for, TRANSLATION_UNIT_DECL);
  RESET_BIT (lto_flags_needed_for, NAMESPACE_DECL);

  lto_types_needed_for = sbitmap_alloc (NUM_TREE_CODES);

  /* Global declarations and types will handle the
     type field by other means, so lto_types_needed_for
     should not be set for them.  */
#if REDUNDANT_TYPE_SYSTEM
  /* These forms never need types.  */
  sbitmap_ones (lto_types_needed_for);
  RESET_BIT (lto_types_needed_for, ASM_EXPR);
  RESET_BIT (lto_types_needed_for, CASE_LABEL_EXPR);
  RESET_BIT (lto_types_needed_for, FIELD_DECL);
  RESET_BIT (lto_types_needed_for, FUNCTION_DECL);
  RESET_BIT (lto_types_needed_for, GIMPLE_MODIFY_STMT);
  RESET_BIT (lto_types_needed_for, IDENTIFIER_NODE);
  RESET_BIT (lto_types_needed_for, LABEL_DECL);
  RESET_BIT (lto_types_needed_for, LABEL_EXPR);
  RESET_BIT (lto_types_needed_for, MODIFY_EXPR);
  RESET_BIT (lto_types_needed_for, PARM_DECL);
  RESET_BIT (lto_types_needed_for, PHI_NODE);
  RESET_BIT (lto_types_needed_for, RESX_EXPR);
  RESET_BIT (lto_types_needed_for, SSA_NAME);
  RESET_BIT (lto_types_needed_for, VAR_DECL);
  RESET_BIT (lto_types_needed_for, TREE_LIST);
  RESET_BIT (lto_types_needed_for, TREE_VEC);
  RESET_BIT (lto_types_needed_for, TYPE_DECL);
  RESET_BIT (lto_types_needed_for, NAMESPACE_DECL);
  RESET_BIT (lto_types_needed_for, TRANSLATION_UNIT_DECL);
  /* These forms *are* the types.  */
  RESET_BIT (lto_types_needed_for, VOID_TYPE);
  RESET_BIT (lto_types_needed_for, INTEGER_TYPE);
  RESET_BIT (lto_types_needed_for, REAL_TYPE);
  RESET_BIT (lto_types_needed_for, FIXED_POINT_TYPE);
  RESET_BIT (lto_types_needed_for, COMPLEX_TYPE);
  RESET_BIT (lto_types_needed_for, BOOLEAN_TYPE);
  RESET_BIT (lto_types_needed_for, OFFSET_TYPE);
  RESET_BIT (lto_types_needed_for, ENUMERAL_TYPE);
  RESET_BIT (lto_types_needed_for, POINTER_TYPE);
  RESET_BIT (lto_types_needed_for, REFERENCE_TYPE);
  RESET_BIT (lto_types_needed_for, VECTOR_TYPE);
  RESET_BIT (lto_types_needed_for, ARRAY_TYPE);
  RESET_BIT (lto_types_needed_for, RECORD_TYPE);
  RESET_BIT (lto_types_needed_for, UNION_TYPE);
  RESET_BIT (lto_types_needed_for, QUAL_UNION_TYPE);
  RESET_BIT (lto_types_needed_for, FUNCTION_TYPE);
  RESET_BIT (lto_types_needed_for, METHOD_TYPE);
#else
  /* These forms will need types, even when the type system is fixed.  */
  SET_BIT (lto_types_needed_for, COMPLEX_CST);
  SET_BIT (lto_types_needed_for, CONSTRUCTOR);
  SET_BIT (lto_types_needed_for, CONVERT_EXPR);
  SET_BIT (lto_types_needed_for, FIXED_CONVERT_EXPR);
  SET_BIT (lto_types_needed_for, FIXED_CST);
  SET_BIT (lto_types_needed_for, INTEGER_CST);
  SET_BIT (lto_types_needed_for, NOP_EXPR);
  SET_BIT (lto_types_needed_for, REAL_CST);
  SET_BIT (lto_types_needed_for, STRING_CST);
  SET_BIT (lto_types_needed_for, VECTOR_CST );
  SET_BIT (lto_types_needed_for, VIEW_CONVERT_EXPR);
#endif

#ifdef LTO_STREAM_DEBUGGING
#define LTO_STREAM_DEBUGGING_INIT_NAMES
#define SET_NAME(index,value) LTO_tree_tag_names[index] = value;
#include "lto-tree-tags.def"
#undef SET_NAME
#undef LTO_STREAM_DEBUGGING_INIT_NAMES
#endif
}


/* Static initialization for the lto writer.  */

static void
lto_static_init_local (void)
{
  if (initialized_local)
    return;

  initialized_local = true;

  /* Initialize the expression to tag mapping.  */
#define MAP_EXPR_TAG(expr,tag)   expr_to_tag [expr] = tag;
#define TREE_SINGLE_MECHANICAL_TRUE
#define TREE_SINGLE_MECHANICAL_FALSE
#include "lto-tree-tags.def"

#undef MAP_EXPR_TAG
#undef TREE_SINGLE_MECHANICAL_TRUE
#undef TREE_SINGLE_MECHANICAL_FALSE

  lto_static_init ();
}


#ifdef FILE_PER_FUNCTION
/* The once per compilation unit initialization flag.  */
static int function_num;
#endif

/* Output FN.  */

static void
output_function (struct cgraph_node* node)
{
  tree function = node->decl;
  struct function *fn = DECL_STRUCT_FUNCTION (function);
  basic_block bb;
  struct output_block *ob = create_output_block (LTO_section_function_body);
  tree context;

  LTO_SET_DEBUGGING_STREAM (debug_main_stream, main_data);
  clear_line_info (ob);
  ob->cgraph_node = node;

  gcc_assert (!current_function_decl && !cfun);

  /* Set current_function_decl and cfun.  */
  current_function_decl = function;
  push_cfun (fn);

  /* Make string 0 be a NULL string.  */
  lto_output_1_stream (ob->string_stream, 0);

  last_eh_region_seen = 0;

  output_record_start (ob, NULL, NULL, LTO_function);

  /* Output any exception-handling regions.  */
  output_eh_regions (ob, fn);

  /* Output the head of the arguments list.  */
  LTO_DEBUG_INDENT_TOKEN ("decl_arguments");
  if (DECL_ARGUMENTS (function))
    output_expr_operand (ob, DECL_ARGUMENTS (function));
  else
    output_zero (ob);
    
  LTO_DEBUG_INDENT_TOKEN ("decl_context");
  context = DECL_CONTEXT (function);
  if (!context)
    output_zero (ob);
  else if (TYPE_P (context))
    output_type_ref_1 (ob, context);
  else
    output_expr_operand (ob, DECL_CONTEXT (function));

  /* We will renumber the statements.  The code that does this uses
     the same ordering that we use for serializing them so we can use
     the same code on the other end and not have to write out the
     statement numbers.  */
  renumber_gimple_stmt_uids ();

#ifdef LOCAL_TRACE
  fprintf (stderr, "%s\n", IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (function)));
  FOR_ALL_BB_FN (bb, fn)
    {
      block_stmt_iterator bsi;
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  tree stmt = bsi_stmt (bsi);
	  fprintf (stderr, "%d = ", gimple_stmt_uid (stmt));
	  print_generic_stmt (stderr, stmt, 0);
	}
    }
#endif

  /* Output the code for the function.  */
  FOR_ALL_BB_FN (bb, fn)
    output_bb (ob, bb, fn);

  /* The terminator for this function.  */
  output_zero (ob);
  LTO_DEBUG_UNDENT();

  LTO_SET_DEBUGGING_STREAM (debug_ssa_names_stream, ssa_names_data);
  output_ssa_names (ob, fn);

  LTO_SET_DEBUGGING_STREAM (debug_cfg_stream, cfg_data);
  output_cfg (ob, fn);

  LTO_SET_DEBUGGING_STREAM (debug_decl_stream, decl_data);
  output_local_vars (ob, fn);

  LTO_SET_DEBUGGING_STREAM (debug_decl_index_stream, decl_index_data);
  output_local_vars_index (ob);

  LTO_SET_DEBUGGING_STREAM (debug_label_stream, label_data);
  output_named_labels (ob);

  /* Create a section to hold the pickled output of this function.   */
  produce_asm (ob, function);

  destroy_output_block (ob);

  current_function_decl = NULL;

  pop_cfun ();
}


/* Output constructor for static or external vars.  */

static void
output_constructors_and_inits (void)
{
  struct output_block *ob = create_output_block (LTO_section_static_initializer);
  struct varpool_node *vnode;
  unsigned i;
  alias_pair *p;

  ob->cgraph_node = NULL;

  LTO_SET_DEBUGGING_STREAM (debug_main_stream, main_data);
  clear_line_info (ob);

  /* Make string 0 be a NULL string.  */
  lto_output_1_stream (ob->string_stream, 0);

  /* Process the global static vars that have initializers or
     constructors.  */
  FOR_EACH_STATIC_VARIABLE (vnode)
    {
      tree var = vnode->decl;
      tree context = DECL_CONTEXT (var);
      if (!context || TREE_CODE (context) != FUNCTION_DECL)
	{
	  output_expr_operand (ob, var);
	  
	  LTO_DEBUG_TOKEN ("init");
	  if (DECL_INITIAL (var))
	    output_expr_operand (ob, DECL_INITIAL (var));
	  else
	    output_zero (ob);
	}
    }
  /* The terminator for the constructor.  */
  output_zero (ob);

  for (i = 0; VEC_iterate (alias_pair, alias_pairs, i, p); i++)
    {
      output_expr_operand (ob, p->decl);
      LTO_DEBUG_TOKEN ("alias_target");
      output_expr_operand (ob, p->target);
    }

  output_zero (ob);
  LTO_SET_DEBUGGING_STREAM (debug_label_stream, label_data);
  output_named_labels (ob);

  produce_asm (ob, NULL);
  destroy_output_block (ob);
}


/* Main entry point from the pass manager.  */

static void
lto_output (void)
{
  struct cgraph_node *node;
  struct lto_out_decl_state *decl_state;

  lto_static_init_local ();

  /* Remove some front-end specific garbage from the tree.
     We don't yet have a middle-end type system, so front-end
     specific types leak into the middle-end.  */
  free_lang_specifics ();

  /* Process only the functions with bodies and only process the master
     ones of them.  */
  for (node = cgraph_nodes; node; node = node->next)
    if (node->analyzed && cgraph_is_master_clone (node, false))
      {
	decl_state = lto_new_out_decl_state ();
	lto_push_out_decl_state (decl_state);
        output_function (node);
	gcc_assert (lto_get_out_decl_state () == decl_state);
	lto_pop_out_decl_state ();
	lto_record_function_out_decl_state (node->decl, decl_state);
      }

  output_constructors_and_inits ();
}

struct ipa_opt_pass pass_ipa_lto_gimple_out =
{
 {
  IPA_PASS,
  "lto_gimple_out",	                /* name */
  gate_lto_out,			        /* gate */
  NULL,		                	/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_IPA_LTO_OUT,		        /* tv_id */
  0,	                                /* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,            			/* todo_flags_start */
  TODO_dump_func                        /* todo_flags_finish */
 },
 NULL,		                        /* generate_summary */
 lto_output,           			/* write_summary */
 NULL,		         		/* read_summary */
 NULL,					/* function_read_summary */
 0,					/* TODOs */
 NULL,			                /* function_transform */
 NULL					/* variable_transform */
};


#ifdef LTO_STREAM_DEBUGGING
/* Print the tree flags to the debugging stream.  */
   
void 
lto_debug_tree_flags (struct lto_debug_context *context, 
		      enum tree_code code, 
		      lto_flags_type flags)
{
#define CLEAROUT (BITS_PER_LTO_FLAGS_TYPE - 1)

#define START_CLASS_SWITCH()              \
  {                                       \
                                          \
    switch (TREE_CODE_CLASS (code))       \
    {

#define START_CLASS_CASE(class)    case class:
#define ADD_CLASS_DECL_FLAG(flag_name) \
  { if (flags >> CLEAROUT) lto_debug_token (context, " " # flag_name ); flags <<= 1; }
#define ADD_CLASS_EXPR_FLAG(flag_name) \
  { if (flags >> CLEAROUT) lto_debug_token (context, " " # flag_name ); flags <<= 1; }
#define ADD_CLASS_TYPE_FLAG(flag_name) \
  { if (flags >> CLEAROUT) lto_debug_token (context, " " # flag_name ); flags <<= 1; }
#define END_CLASS_CASE(class)      break;
#define END_CLASS_SWITCH()                \
    default:                              \
      gcc_unreachable ();                 \
    }


#define START_EXPR_SWITCH()               \
    switch (code)			  \
    {
#define START_EXPR_CASE(code)    case code:
#define ADD_EXPR_FLAG(flag_name) \
  { if (flags >> CLEAROUT) lto_debug_token (context, " " # flag_name ); flags <<= 1; }
#define ADD_TYPE_FLAG(flag_name) \
  { if (flags >> CLEAROUT) lto_debug_token (context, " " # flag_name ); flags <<= 1; }
#define ADD_DECL_FLAG(flag_name) \
  { if (flags >> CLEAROUT) lto_debug_token (context, " " # flag_name ); flags <<= 1; }
#define ADD_VIS_FLAG(flag_name)  \
  { if (flags >> CLEAROUT) lto_debug_token (context, " " # flag_name ); flags <<= 1; }
#define ADD_VIS_FLAG_SIZE(flag_name,size)					\
  { if (flags >> (BITS_PER_LTO_FLAGS_TYPE - size)) lto_debug_token (context, " " # flag_name ); flags <<= size; }
#define ADD_FUN_FLAG(flag_name)  \
  { if (flags >> CLEAROUT) lto_debug_token (context, " " # flag_name ); flags <<= 1; }
#define END_EXPR_CASE(class)      break;
#define END_EXPR_SWITCH()                 \
    default:                              \
      gcc_unreachable ();                 \
    }                                     \
  }

#include "lto-tree-flags.def"

#undef START_CLASS_SWITCH
#undef START_CLASS_CASE
#undef ADD_CLASS_DECL_FLAG
#undef ADD_CLASS_EXPR_FLAG
#undef ADD_CLASS_TYPE_FLAG
#undef END_CLASS_CASE
#undef END_CLASS_SWITCH
#undef START_EXPR_SWITCH
#undef START_EXPR_CASE
#undef ADD_EXPR_FLAG
#undef ADD_TYPE_FLAG
#undef ADD_DECL_FLAG
#undef ADD_VIS_FLAG
#undef ADD_FUN_FLAG
#undef END_EXPR_CASE
#undef END_EXPR_SWITCH
}
#endif


/* Serialization of global types and declarations.  */

void output_tree (struct output_block *, tree);
void output_type_tree (struct output_block *, tree);

/* Output the start of a record with TAG and possibly flags for EXPR,
   and the TYPE for VALUE to OB.   Unlike output_record_start, use
   output_type_tree instead of output_type_ref.  */

static void
output_global_record_start (struct output_block *ob, tree expr,
                            tree value, unsigned int tag)
{
  lto_output_1_stream (ob->main_stream, tag);
  LTO_DEBUG_INDENT (tag);
  if (expr)
    {
      enum tree_code code = TREE_CODE (expr);
      if (value
	  && TEST_BIT (lto_types_needed_for, code)
	  && TREE_TYPE (value))
	output_type_tree (ob, TREE_TYPE (value));
      output_tree_flags (ob, code, expr, false);
    }
}

/* Write the current global vector length to the debugging stream.  */

#ifdef LTO_STREAM_DEBUGGING
static void
global_vector_debug (struct output_block *ob)
{
  LTO_DEBUG_TOKEN ("[");
  LTO_DEBUG_WIDE ("U", htab_elements(ob->main_hash_table));
  LTO_DEBUG_TOKEN ("]");
}
#else
#define global_vector_debug(ob)
#endif

static void
output_field_decl (struct output_block *ob, tree decl)
{
  /* tag and flags */
  output_global_record_start (ob, NULL, NULL, LTO_field_decl1);
  output_tree_flags (ob, 0, decl, true);

  global_vector_debug (ob);

  /* uid and locus are handled specially */
  LTO_DEBUG_TOKEN ("name");
  output_tree (ob, decl->decl_minimal.name);
  LTO_DEBUG_TOKEN ("context");
  output_tree (ob, decl->decl_minimal.context);

  LTO_DEBUG_TOKEN ("type");
  output_tree (ob, decl->common.type);

  LTO_DEBUG_TOKEN ("attributes");
  output_tree (ob, decl->decl_common.attributes);
  LTO_DEBUG_TOKEN ("abstract_origin");
  output_tree (ob, decl->decl_common.abstract_origin);

  output_uleb128 (ob, decl->decl_common.mode);
  output_uleb128 (ob, decl->decl_common.align);
  output_uleb128 (ob, decl->decl_common.off_align);

  LTO_DEBUG_TOKEN ("size");
  output_tree (ob, decl->decl_common.size);
  LTO_DEBUG_TOKEN ("size_unit");
  output_tree (ob, decl->decl_common.size_unit);

  LTO_DEBUG_TOKEN ("offset");
  output_tree (ob, decl->field_decl.offset);
  LTO_DEBUG_TOKEN ("bit_field_type");
  output_tree (ob, decl->field_decl.bit_field_type);
  LTO_DEBUG_TOKEN ("qualifier");
  output_tree (ob, decl->field_decl.qualifier);
  LTO_DEBUG_TOKEN ("bit_offset");
  output_tree (ob, decl->field_decl.bit_offset);
  LTO_DEBUG_TOKEN ("fcontext");
  output_tree (ob, decl->field_decl.fcontext);

  /* lang_specific */
  LTO_DEBUG_TOKEN ("initial");
  output_tree (ob, decl->decl_common.initial);

  /* Write out current field before its siblings,
     so follow the chain last.  */
  LTO_DEBUG_TOKEN ("chain");
  output_tree (ob, decl->common.chain);

  LTO_DEBUG_TOKEN ("end_field_decl");
}


/* Write FUNCTION_DECL DECL to the output block OB.  */

static void
output_function_decl (struct output_block *ob, tree decl)
{
  /* tag and flags */
  output_global_record_start (ob, NULL, NULL, LTO_function_decl);
  output_tree_flags (ob, 0, decl, true);

  global_vector_debug (ob);

  /* uid and locus are handled specially */
  output_tree (ob, decl->decl_minimal.name);
  output_tree (ob, decl->decl_minimal.context);

  output_tree (ob, decl->decl_with_vis.assembler_name);
  output_tree (ob, decl->decl_with_vis.section_name);

  /* omit chain, which would result in writing all functions  */
  output_tree (ob, decl->common.type);

  output_tree (ob, decl->decl_common.attributes);
  output_tree (ob, decl->decl_common.abstract_origin);

  output_uleb128 (ob, decl->decl_common.mode);
  output_uleb128 (ob, decl->decl_common.align);
  gcc_assert (decl->decl_common.off_align == 0);

  output_tree (ob, decl->decl_common.size);
  output_tree (ob, decl->decl_common.size_unit);

  /* lang_specific */

  /* omit rtl */

  /* saved_tree -- this is a function body, so omit it here */
  output_tree (ob, decl->decl_non_common.arguments);
  output_tree (ob, decl->decl_non_common.result);
  output_tree (ob, decl->decl_non_common.vindex);

  /* omit initial -- should be written with body */

  output_uleb128 (ob, decl->function_decl.function_code);
  output_uleb128 (ob, decl->function_decl.built_in_class);
  LTO_DEBUG_TOKEN ("end_function_decl");
}

static void
output_var_decl (struct output_block *ob, tree decl)
{
  /* tag and flags */
  /* Assume static or external variable.  */
  output_global_record_start (ob, NULL, NULL, LTO_var_decl1);
  output_tree_flags (ob, 0, decl, true);

  global_vector_debug (ob);

  /* uid and locus are handled specially */
  output_tree (ob, decl->decl_minimal.name);
  output_tree (ob, decl->decl_minimal.context);

  LTO_DEBUG_TOKEN ("var_decl_assembler_name");
  output_tree (ob, decl->decl_with_vis.assembler_name);
  output_tree (ob, decl->decl_with_vis.section_name);

  /* omit chain */
  output_tree (ob, decl->common.type);

  output_tree (ob, decl->decl_common.attributes);
  output_tree (ob, decl->decl_common.abstract_origin);

  output_uleb128 (ob, decl->decl_common.mode);
  output_uleb128 (ob, decl->decl_common.align);
  gcc_assert (decl->decl_common.off_align == 0);

  LTO_DEBUG_TOKEN ("var_decl_size");
  output_tree (ob, decl->decl_common.size);
  output_tree (ob, decl->decl_common.size_unit);

  /* lang_specific */

  /* omit rtl */

  /* DECL_DEBUG_EXPR is stored in a table on the side,
     not in the VAR_DECL node itself.  */
  LTO_DEBUG_TOKEN ("var_decl_debug_expr");
  output_tree (ob, DECL_DEBUG_EXPR (decl));
  
  /* Write initial expression last.  */
  output_tree (ob, decl->decl_common.initial);

  LTO_DEBUG_TOKEN ("end_var_decl");
}

static void
output_parm_decl (struct output_block *ob, tree decl)
{
  /* tag and flags */
  output_global_record_start (ob, NULL, NULL, LTO_parm_decl);
  output_tree_flags (ob, 0, decl, true);

  global_vector_debug (ob);

  /* uid and locus are handled specially */
  output_tree (ob, decl->decl_minimal.name);
  output_tree (ob, decl->decl_minimal.context);
        
  output_tree (ob, decl->common.type);

  output_tree (ob, decl->decl_common.attributes);
  output_tree (ob, decl->decl_common.abstract_origin);

  output_uleb128 (ob, decl->decl_common.mode);
  output_uleb128 (ob, decl->decl_common.align);
  gcc_assert (decl->decl_common.off_align == 0);

  output_tree (ob, decl->decl_common.size);
  output_tree (ob, decl->decl_common.size_unit);

  output_tree (ob, decl->decl_common.initial);

  /* lang_specific */
  /* omit rtl, incoming_rtl */

  output_tree (ob, decl->common.chain);

  LTO_DEBUG_TOKEN ("end_parm_decl");
}

static void
output_result_decl (struct output_block *ob, tree decl)
{
  /* tag and flags */
  output_global_record_start (ob, NULL, NULL, LTO_result_decl);
  output_tree_flags (ob, 0, decl, true);

  global_vector_debug (ob);

  /* uid and locus are handled specially */
  output_tree (ob, decl->decl_minimal.name);
  output_tree (ob, decl->decl_minimal.context);
        
  output_tree (ob, decl->common.type);

  output_tree (ob, decl->decl_common.attributes);
  output_tree (ob, decl->decl_common.abstract_origin);

  output_uleb128 (ob, decl->decl_common.mode);
  output_uleb128 (ob, decl->decl_common.align);
  gcc_assert (decl->decl_common.off_align == 0);

  output_tree (ob, decl->decl_common.size);
  output_tree (ob, decl->decl_common.size_unit);

  /* lang_specific */
  /* omit rtl */

  /* FIXME: Does this make sense for result?  */
  output_tree (ob, decl->decl_common.initial);

  gcc_assert (!decl->common.chain);

  LTO_DEBUG_TOKEN ("end_result_decl");
}

static void
output_type_decl (struct output_block *ob, tree decl)
{
  /* tag and flags */
  output_global_record_start (ob, NULL, NULL, LTO_type_decl1);
  output_tree_flags (ob, 0, decl, true);

  global_vector_debug (ob);

  /* uid and locus are handled specially */
  /* Must output name before type.  */
  output_tree (ob, decl->decl_minimal.name);

  /* Should be cleared by free_lang_specifics.  */
  gcc_assert (decl->decl_minimal.context == NULL_TREE);

  output_tree (ob, decl->decl_with_vis.assembler_name);
  output_tree (ob, decl->decl_with_vis.section_name);

  output_tree (ob, decl->common.type);

  output_tree (ob, decl->decl_common.attributes);
  output_tree (ob, decl->decl_common.abstract_origin);

  output_uleb128 (ob, decl->decl_common.mode);
  output_uleb128 (ob, decl->decl_common.align);

  output_tree (ob, decl->decl_common.size);
  output_tree (ob, decl->decl_common.size_unit);

  /* We expect free_lang_specifics to clear the INITIAL field.  */
  gcc_assert (decl->decl_common.initial == NULL_TREE);

  /* lang_specific */

  gcc_assert (decl->decl_with_rtl.rtl == NULL);

  output_tree (ob, decl->decl_non_common.saved_tree);		/* ??? */
  output_tree (ob, decl->decl_non_common.arguments);
  output_tree (ob, decl->decl_non_common.result);		/* ??? */
  output_tree (ob, decl->decl_non_common.vindex);		/* ??? */

  LTO_DEBUG_TOKEN ("end_type_decl");
}

static void
output_label_decl (struct output_block *ob, tree decl)
{
  /* tag and flags */
  output_global_record_start (ob, NULL, NULL, LTO_label_decl);
  output_tree_flags (ob, 0, decl, true);

  global_vector_debug (ob);

  /* uid and locus are handled specially */
  output_tree (ob, decl->decl_minimal.name);
  output_tree (ob, decl->decl_minimal.context);
        
  output_tree (ob, decl->common.type);

  output_tree (ob, decl->decl_common.attributes);		/* ??? */
  output_tree (ob, decl->decl_common.abstract_origin);		/* ??? */

  output_uleb128 (ob, decl->decl_common.mode);			/* ??? */
  output_uleb128 (ob, decl->decl_common.align);			/* ??? */
  gcc_assert (decl->decl_common.off_align == 0);

  gcc_assert (decl->decl_common.size == NULL_TREE);
  gcc_assert (decl->decl_common.size_unit == NULL_TREE);

  output_tree (ob, decl->decl_common.initial);

  /* lang_specific */
  /* omit rtl, incoming_rtl */
  /* omit chain */

  LTO_DEBUG_TOKEN ("end_label_decl");
}

static void
output_namespace_decl (struct output_block *ob, tree decl)
{
  /* tag and flags */
  output_global_record_start (ob, NULL, NULL, LTO_namespace_decl);
  output_tree_flags (ob, 0, decl, true);

  global_vector_debug (ob);

  /* uid and locus are handled specially */
  output_tree (ob, decl->decl_minimal.name);
  output_tree (ob, decl->decl_minimal.context);

  output_tree (ob, decl->decl_with_vis.assembler_name);
  output_tree (ob, decl->decl_with_vis.section_name);

  gcc_assert (decl->common.type == void_type_node
	      || decl->common.type == NULL_TREE);

  output_tree (ob, decl->decl_common.attributes);
  output_tree (ob, decl->decl_common.abstract_origin);

  gcc_assert (decl->decl_common.mode == 0);
  /* FIXME lto:  I'm seeing both 0 and 1 here, but I don't
     think alignment should be meaningful for NAMESPACE_DECL.  */
  output_uleb128 (ob, decl->decl_common.align);

  gcc_assert (decl->decl_common.size == NULL_TREE);
  gcc_assert (decl->decl_common.size_unit == NULL_TREE);

  /* lang_specific */

  gcc_assert (decl->decl_with_rtl.rtl == NULL);

  output_tree (ob, decl->decl_non_common.saved_tree);  		/* ??? */
  gcc_assert (decl->decl_non_common.arguments == NULL_TREE);
  gcc_assert (decl->decl_non_common.result == NULL_TREE);
  output_tree (ob, decl->decl_non_common.vindex);

  /* omit chain */
  LTO_DEBUG_TOKEN ("end_namespace_decl");
}

static void
output_translation_unit_decl (struct output_block *ob, tree decl)
{
  /* tag and flags */
  output_global_record_start (ob, NULL, NULL, LTO_translation_unit_decl);
  output_tree_flags (ob, 0, decl, true);

  global_vector_debug (ob);

  /* uid and locus are handled specially */
  output_tree (ob, decl->decl_minimal.name);
  output_tree (ob, decl->decl_minimal.context);

  output_tree (ob, decl->decl_with_vis.assembler_name);
  output_tree (ob, decl->decl_with_vis.section_name);

  output_tree (ob, decl->common.type);

  gcc_assert (decl->decl_common.attributes == NULL_TREE);
  output_tree (ob, decl->decl_common.abstract_origin);

  gcc_assert (decl->decl_common.mode == 0);
  output_uleb128 (ob, decl->decl_common.align);

  gcc_assert (decl->decl_common.size == NULL_TREE);
  gcc_assert (decl->decl_common.size_unit == NULL_TREE);
  /* FIXME lto: Verify this.  */
  /* Omit initial value.  I believe this is covered when
     we read constructors and inits.  */

  gcc_assert (decl->decl_with_rtl.rtl == NULL);

  /* omit chain */
  LTO_DEBUG_TOKEN ("end_translation_unit_decl");
}

static void
output_binfo (struct output_block *ob, tree binfo)
{
  size_t i;
  size_t num_base_accesses = VEC_length (tree, binfo->binfo.base_accesses);
  size_t num_base_binfos = VEC_length (tree, &binfo->binfo.base_binfos);

  output_global_record_start (ob, NULL, NULL, LTO_tree_binfo);
  output_tree_flags (ob, 0, binfo, false);

  output_uleb128 (ob, num_base_accesses);
  output_uleb128 (ob, num_base_binfos);

  global_vector_debug (ob);  /* note different location from others */

  output_tree (ob, binfo->common.type);

  output_tree (ob, binfo->binfo.offset);
  output_tree (ob, binfo->binfo.vtable);
  output_tree (ob, binfo->binfo.virtuals);
  output_tree (ob, binfo->binfo.vptr_field);
  output_tree (ob, binfo->binfo.inheritance);
  output_tree (ob, binfo->binfo.vtt_subvtt);
  output_tree (ob, binfo->binfo.vtt_vptr);

  LTO_DEBUG_TOKEN ("base_accesses");
  for (i = 0; i < num_base_accesses; ++i)
    output_tree (ob, VEC_index (tree, binfo->binfo.base_accesses, i));

  LTO_DEBUG_TOKEN ("base_binfos");
  for (i = 0; i < num_base_binfos; ++i)
    output_tree (ob, VEC_index (tree, &binfo->binfo.base_binfos, i));

  output_tree (ob, binfo->common.chain);
  LTO_DEBUG_TOKEN ("end_binfo");
}

static void
output_type (struct output_block *ob, tree type, enum LTO_tags tag)
{
  /* tag and flags */
  output_global_record_start (ob, NULL, NULL, tag);
  output_tree_flags (ob, 0, type, false);

  global_vector_debug (ob);

  LTO_DEBUG_TOKEN ("type");
  output_tree (ob, type->common.type);
  LTO_DEBUG_TOKEN ("size");
  output_tree (ob, type->type.size);
  LTO_DEBUG_TOKEN ("size_unit");
  output_tree (ob, type->type.size_unit);
  LTO_DEBUG_TOKEN ("attributes");
  output_tree (ob, type->type.attributes);
  /* Do not write UID.  Assign a new one on input.  */
  LTO_DEBUG_TOKEN ("precision");
  output_uleb128 (ob, type->type.precision);
  LTO_DEBUG_TOKEN ("mode");
  output_uleb128 (ob, type->type.mode);
  LTO_DEBUG_TOKEN ("align");
  output_uleb128 (ob, type->type.align);
  LTO_DEBUG_TOKEN ("pointer_to");
  output_tree (ob, type->type.pointer_to);
  LTO_DEBUG_TOKEN ("reference_to");
  output_tree (ob, type->type.reference_to);
  /* FIXME: Output symtab here.  Do we need it?  */
  LTO_DEBUG_TOKEN ("name");
  output_tree (ob, type->type.name);	/* may be a TYPE_DECL */
  LTO_DEBUG_TOKEN ("minval");
  output_tree (ob, type->type.minval);
  LTO_DEBUG_TOKEN ("maxval");
  output_tree (ob, type->type.maxval);
  LTO_DEBUG_TOKEN ("next_variant");
  output_tree (ob, type->type.next_variant);
  LTO_DEBUG_TOKEN ("main_variant");
  output_tree (ob, type->type.main_variant);
  gcc_assert (type->type.binfo == NULL_TREE
	      || TREE_CODE (type) == RECORD_TYPE
	      || TREE_CODE (type) == UNION_TYPE);
  LTO_DEBUG_TOKEN ("binfo");
  output_tree (ob, type->type.binfo);

  /* Should be cleared by free_lang_specifics.  */
  gcc_assert (type->type.context == NULL_TREE);

  LTO_DEBUG_TOKEN ("canonical");
  output_tree (ob, type->type.canonical);

  /* Slot 'values' may be the structures fields, so do them last,
     after other slots of the structure type have been filled in.  */
  if (tag == LTO_record_type || tag == LTO_union_type)
    {
      LTO_DEBUG_TOKEN ("fields");
      output_tree (ob, TYPE_FIELDS (type));
    }
  else
    {
      LTO_DEBUG_TOKEN ("values");
      if (TYPE_CACHED_VALUES_P (type))
	{
	  gcc_assert (tag != RECORD_TYPE
		      && tag != UNION_TYPE
		      && tag != ARRAY_TYPE);
	  /* Don't stream the values cache.  We must clear flag
	     TYPE_CACHED_VALUES_P on input.  We don't do it here
	     because we don't want to clobber the tree as we write
	     it, and there is no infrastructure for modifying
	     flags as we serialize them.  */
	  output_zero (ob);
	}
      else
	output_tree (ob, type->type.values);
    }

  LTO_DEBUG_TOKEN ("chain");
  output_tree (ob, type->common.chain);	   /* overloaded as TYPE_STUB_DECL */
  LTO_DEBUG_TOKEN ("end_type");
}

/* Output the start of a record with TAG and possibly flags for EXPR,
   and the TYPE for VALUE to OB.   Unlike output_record_start, use
   output_type_tree instead of output_type_ref.  */

static void
output_global_record_start_1 (struct output_block *ob, tree expr,
			      tree value, unsigned int tag)
{
  lto_output_1_stream (ob->main_stream, tag);
  LTO_DEBUG_INDENT (tag);
  if (expr)
    {
      enum tree_code code = TREE_CODE (expr);
      if (value && TEST_BIT (lto_types_needed_for, code))
	{
	  if (TREE_TYPE (value))
	    output_type_tree (ob, TREE_TYPE (value));
	  else
	    {
	      /* Allow for null tree type */
	      LTO_DEBUG_TOKEN ("type");
	      output_zero (ob);
	    }
	}
      output_tree_flags (ob, code, expr, false);
    }
}


/* Output constructor CTOR to OB.  */

static void
output_global_constructor (struct output_block *ob, tree ctor)
{
  tree value;
  tree purpose;
  unsigned HOST_WIDE_INT idx;

  output_global_record_start_1 (ob, ctor, ctor, LTO_constructor);
  output_uleb128 (ob, VEC_length (constructor_elt, CONSTRUCTOR_ELTS (ctor)));

  FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (ctor), idx, purpose, value)
    {
      if (purpose)
	output_tree (ob, purpose);
      else 
	output_zero (ob);

      if (TREE_CODE (value) == CONSTRUCTOR)
	{
	  output_global_constructor (ob, value);
	  LTO_DEBUG_UNDENT ();
	}
      else 
	output_tree (ob, value);
    }
}


/* Emit tree node EXPR to output block OB.  */

void
output_tree (struct output_block *ob, tree expr)
{
  enum tree_code code;
  enum tree_code_class class;
  unsigned int tag;
  void **slot;
  struct lto_decl_slot d_slot;

  if (expr == NULL_TREE)
    {
      output_zero (ob);
      return;
    }

#ifdef GLOBAL_STREAMER_TRACE
  fprintf (stderr, "Emitting tree %p : [%s] ", (void *) expr,
	   tree_code_name[TREE_CODE (expr)]);
  print_generic_expr (stderr, expr, 0);
  fprintf (stderr, "\n");
#endif

  if (TYPE_P (expr) || DECL_P (expr) || TREE_CODE (expr) == TREE_BINFO)
    {
      unsigned int global_index;
      /* FIXME lto:  There are decls that pass the predicate above, but which
	 we do not handle.  We must avoid assigning a global index to such a node,
 	 as we will not emit it, and the indices will get out of sync with the
 	 global vector on the reading side.  We shouldn't be seeing these nodes,
 	 and, ideally, we should abort on them.  This is an interim measure
 	 for the sake of making forward progress.  */
      {
 	enum tree_code code = TREE_CODE (expr);
 
 	switch (code)
 	  {
 	  case CONST_DECL:
 	  case FIELD_DECL:
 	  case FUNCTION_DECL:
 	  case VAR_DECL:
 	  case PARM_DECL:
 	  case RESULT_DECL:
 	  case TYPE_DECL:
 	  case NAMESPACE_DECL:
 	  case TRANSLATION_UNIT_DECL:
	  case LABEL_DECL:
 	    break;
 	  case VOID_TYPE:
 	  case INTEGER_TYPE:
 	  case REAL_TYPE:
 	  case FIXED_POINT_TYPE:
 	  case COMPLEX_TYPE:
 	  case BOOLEAN_TYPE:
 	  case OFFSET_TYPE:
 	  case ENUMERAL_TYPE:
 	  case POINTER_TYPE:
 	  case REFERENCE_TYPE:
 	  case VECTOR_TYPE:
 	  case ARRAY_TYPE:
 	  case RECORD_TYPE:
 	  case UNION_TYPE:
 	  case QUAL_UNION_TYPE:
 	  case FUNCTION_TYPE:
 	  case METHOD_TYPE:
 	    break;
 	  case TREE_BINFO:
 	    break;
 
 	  default:
	    error ("Unhandled type or decl: %s", tree_code_name[code]);
	    gcc_unreachable ();
 	  }
      }
 
      /* If we've already pickled this node, emit a reference.
         Otherwise, assign an index for the node we are about to emit.  */
      if (get_ref_idx_for (expr, ob->main_hash_table, NULL, &global_index))
        {
#ifdef GLOBAL_STREAMER_TRACE
          fprintf (stderr, "%p -> OLD %d\n", (void *) expr, global_index);
#endif
          output_global_record_start (ob, NULL, NULL,
				      LTO_tree_pickle_reference);
          output_uleb128 (ob, global_index);
          LTO_DEBUG_UNDENT ();
          return;
        }
#ifdef GLOBAL_STREAMER_TRACE
    fprintf (stderr, "%p -> NEW %d\n", (void *) expr, global_index);
#endif
    }
  else
    {
      /* We don't share new instances of other classes of tree nodes,
         but we always want to share the preloaded "well-known" nodes.  */
      d_slot.t = expr;
      slot = htab_find_slot (ob->main_hash_table, &d_slot, NO_INSERT);
      if (slot != NULL)
        {
          struct lto_decl_slot *old_slot = (struct lto_decl_slot *)*slot;
#ifdef GLOBAL_STREAMER_TRACE
          fprintf (stderr, "%p -> OLD %d\n", (void *) expr, old_slot->slot_num);
#endif
          output_global_record_start (ob, NULL, NULL, LTO_tree_pickle_reference);
          output_uleb128 (ob, old_slot->slot_num);
          LTO_DEBUG_UNDENT ();
          return;
        }
    }

  code = TREE_CODE (expr);
  class = TREE_CODE_CLASS (code);
  tag = expr_to_tag [code];

  switch (code)
    {
    case COMPLEX_CST:
      if (TREE_CODE (TREE_REALPART (expr)) == REAL_CST)
	{
	  output_global_record_start (ob, expr, expr, LTO_complex_cst1);
	  output_type_tree (ob, TREE_TYPE (TREE_REALPART (expr)));
	  output_real (ob, TREE_REALPART (expr));
	  output_real (ob, TREE_IMAGPART (expr));
	}
      else
	{
	  output_global_record_start (ob, expr, expr, LTO_complex_cst0);
	  output_type_tree (ob, TREE_TYPE (TREE_REALPART (expr)));
	  output_integer (ob, TREE_REALPART (expr));
	  output_integer (ob, TREE_IMAGPART (expr));
	}
      break;

    case INTEGER_CST:
      output_global_record_start (ob, expr, expr, tag);
      output_integer (ob, expr);
      break;

    case REAL_CST:
      output_global_record_start (ob, expr, expr, tag);
      output_real (ob, expr);
      break;

    case STRING_CST:
      {
	/* Most STRING_CSTs have a type when they get here.  The ones
	   in the string operands of asms do not.  Put something there
	   so that all STRING_CSTs can be handled uniformly.  */
	if (!TREE_TYPE (expr))
	  TREE_TYPE (expr) = void_type_node;

	output_global_record_start (ob, expr, expr, LTO_string_cst);
	output_string (ob, ob->main_stream, 
		       TREE_STRING_POINTER (expr),
		       TREE_STRING_LENGTH (expr));
      }
      break;

    case IDENTIFIER_NODE:
      {
	output_global_record_start (ob, expr, expr, LTO_identifier_node);
	output_string (ob, ob->main_stream, 
		       IDENTIFIER_POINTER (expr),
		       IDENTIFIER_LENGTH (expr));
      }
      break;
      

    case VECTOR_CST:
      {
	tree t = TREE_VECTOR_CST_ELTS (expr);
	int len = 1;

	while ((t = TREE_CHAIN (t)) != NULL)
	  len++;
	t = TREE_VECTOR_CST_ELTS (expr);
	if (TREE_CODE (TREE_VALUE(t)) == REAL_CST)
	  {
	    output_global_record_start (ob, expr, expr, LTO_vector_cst1);
	    output_uleb128 (ob, len);
	    output_type_tree (ob, TREE_TYPE (TREE_VALUE (t)));
	    output_real (ob, TREE_VALUE (t));
	    while ((t = TREE_CHAIN (t)) != NULL)
	      output_real (ob, TREE_VALUE (t));
	  }
	else
	  {
	    output_global_record_start (ob, expr, expr, LTO_vector_cst0);
	    output_uleb128 (ob, len);
	    output_type_tree (ob, TREE_TYPE (TREE_VALUE (t)));
	    output_integer (ob, TREE_VALUE (t));
	    while ((t = TREE_CHAIN (t)) != NULL)
	      output_integer (ob, TREE_VALUE (t));
	  }
      }
      break;

    case CASE_LABEL_EXPR:
      /* FIXME: We should see case labels here.  Assert?  */
      {
	int variant = 0;
	if (CASE_LOW (expr) != NULL_TREE)
	  variant |= 0x1;
	if (CASE_HIGH (expr) != NULL_TREE)
	  variant |= 0x2;
	output_global_record_start (ob, expr, NULL,
                                    LTO_case_label_expr0 + variant);

	if (CASE_LOW (expr) != NULL_TREE)
	  output_tree (ob, CASE_LOW (expr));
	if (CASE_HIGH (expr) != NULL_TREE)
	  output_tree (ob, CASE_HIGH (expr));
	output_tree (ob, CASE_LABEL (expr));
      }
      break;

    case CONSTRUCTOR:
      output_global_constructor (ob, expr);
      break;

    case SSA_NAME:
      /* FIXME: I don't think SSA_NAME nodes make sense here.  */
      gcc_unreachable ();
      /*
      output_global_record_start (ob, expr, expr, LTO_ssa_name);
      output_uleb128 (ob, SSA_NAME_VERSION (expr));
      */
      break;

    case CONST_DECL:
      /* We should not see these.  */
      gcc_unreachable ();

    case FIELD_DECL:
      output_field_decl (ob, expr);
      break;

    case FUNCTION_DECL:
      output_function_decl (ob, expr);
      break;

    case VAR_DECL:
      if (TREE_STATIC (expr) || DECL_EXTERNAL (expr))
        output_var_decl (ob, expr);
      else
        /* We should not be seeing local variables here.  */
        gcc_unreachable ();
      break;

    case PARM_DECL:
      output_parm_decl (ob, expr);
      break;

    case RESULT_DECL:
      output_result_decl (ob, expr);
      break;

    case TYPE_DECL:
      output_type_decl (ob, expr);
      break;

    case NAMESPACE_DECL:
      output_namespace_decl (ob, expr);
      break;

    case TRANSLATION_UNIT_DECL:
      output_translation_unit_decl (ob, expr);
      break;


    case LABEL_DECL:
      output_label_decl (ob, expr);
      break;

    case LABEL_EXPR:
      output_global_record_start (ob, expr, NULL, tag);
      output_tree (ob, TREE_OPERAND (expr, 0));
      break;

    case COND_EXPR:
      if (TREE_OPERAND (expr, 1))
	{
	  output_global_record_start (ob, expr, expr, LTO_cond_expr0);
	  output_tree (ob, TREE_OPERAND (expr, 0));
	  output_tree (ob, TREE_OPERAND (expr, 1));
	  output_tree (ob, TREE_OPERAND (expr, 2));
	}
      else 
	{
	  output_global_record_start (ob, expr, expr, LTO_cond_expr1);
	  output_tree (ob, TREE_OPERAND (expr, 0));
	}
      break;

    case COMPONENT_REF:
      output_global_record_start (ob, expr, expr, tag);
      output_tree (ob, TREE_OPERAND (expr, 0));
      output_tree (ob, TREE_OPERAND (expr, 1));
      /* Ignore 3 because it can be recomputed.  */
      break;

    case CALL_EXPR:
      {
	unsigned int count = TREE_INT_CST_LOW (TREE_OPERAND (expr, 0));
	unsigned int i;

	/* Operand 2 is the call chain.  */
	if (TREE_OPERAND (expr, 2))
	  {
	    output_global_record_start (ob, expr, expr, LTO_call_expr1);
	    output_uleb128 (ob, count);
	    output_tree (ob, TREE_OPERAND (expr, 2));
	  }
	else
	  {
	    output_global_record_start (ob, expr, expr, LTO_call_expr0);
	    output_uleb128 (ob, count);
	  }
	output_tree (ob, TREE_OPERAND (expr, 1));
	for (i = 3; i < count; i++)
	  output_tree (ob, TREE_OPERAND (expr, i));
      }
      break;

    case BIT_FIELD_REF:
      {
	tree op1 = TREE_OPERAND (expr, 1);
	tree op2 = TREE_OPERAND (expr, 2);
	if ((TREE_CODE (op1) == INTEGER_CST)
	    && (TREE_CODE (op2) == INTEGER_CST))
	  {
	    output_global_record_start (ob, expr, expr,
				 LTO_bit_field_ref1);
	    output_uleb128 (ob, TREE_INT_CST_LOW (op1));
	    output_uleb128 (ob, TREE_INT_CST_LOW (op2));
	    output_tree (ob, TREE_OPERAND (expr, 0));
	  }
	else
	  {
	    output_global_record_start (ob, expr, expr,
				 LTO_bit_field_ref0);
	    output_tree (ob, TREE_OPERAND (expr, 0));
	    output_tree (ob, op1);
	    output_tree (ob, op2);
	  }
      }
      break;

    case ARRAY_REF:
    case ARRAY_RANGE_REF:
      /* Ignore operands 2 and 3 for ARRAY_REF and ARRAY_RANGE REF
	 because they can be recomputed.  */
      output_global_record_start (ob, expr, expr, tag);
      output_tree (ob, TREE_OPERAND (expr, 0));
      output_tree (ob, TREE_OPERAND (expr, 1));
      break;


    case ASM_EXPR:
      {
	tree string_cst = ASM_STRING (expr);
	output_global_record_start (ob, expr, NULL, LTO_asm_expr);
	output_string (ob, ob->main_stream, 
		       TREE_STRING_POINTER (string_cst),
		       TREE_STRING_LENGTH (string_cst));
	if (ASM_INPUTS (expr))
	  output_tree (ob, ASM_INPUTS (expr));
	else 
	  output_zero (ob);

	if (ASM_OUTPUTS (expr))
	  output_tree (ob, ASM_OUTPUTS (expr));
	else 
	  output_zero (ob);

	if (ASM_CLOBBERS (expr))
	  output_tree (ob, ASM_CLOBBERS (expr));
	else 
	  output_zero (ob);
      }
      break;

    case RANGE_EXPR:
      {
	output_global_record_start (ob, NULL, NULL, LTO_range_expr);
	/* Need the types here to reconstruct the ranges.  */
	output_type_tree (ob, TREE_OPERAND (expr, 0));
	output_integer (ob, TREE_OPERAND (expr, 0));
	output_type_tree (ob, TREE_OPERAND (expr, 1));
	output_integer (ob, TREE_OPERAND (expr, 1));
      }
      break; 

    case RESX_EXPR:
      output_global_record_start (ob, expr, NULL, tag);
      output_uleb128 (ob, TREE_INT_CST_LOW (TREE_OPERAND (expr, 0)));
      break;

    case RETURN_EXPR:
      {
	tree t = TREE_OPERAND (expr, 0);
	if (t == NULL)
	  {
	    /* Form return.  */
	    output_global_record_start (ob, expr, expr,
				 LTO_return_expr0);
	  }
	else if (TREE_CODE (t) == MODIFY_EXPR)
	  {
	    /* Form return a = b;  */
	    output_global_record_start (ob, expr, expr,
				 LTO_return_expr2);
	    output_tree (ob, TREE_OPERAND (t, 0));
	    output_tree (ob, TREE_OPERAND (t, 1));
	  }
	else
	  {
	    /* Form return a; */
	    output_global_record_start (ob, expr, expr,
				 LTO_return_expr1);
            /* If the type of the argument is a type that gets returned
               in memory, then the gimplifier would have changed the
               argument of the RETURN_EXPR to point at DECL_RESULT of
               the current function.  Communicate this fact to the
               reader so we avoid reading in superfluous trees.  */
            if (t == DECL_RESULT (current_function_decl))
              output_zero (ob);
            else
              output_tree (ob, t);
	  }
      }
      break;

    case GIMPLE_MODIFY_STMT:
      output_global_record_start (ob, expr, NULL, tag);
      output_tree (ob, GIMPLE_STMT_OPERAND (expr, 0));
      output_tree (ob, GIMPLE_STMT_OPERAND (expr, 1));
      break;

    case SWITCH_EXPR:
      /* FIXME: We should not see these  here.  Assert?  */
      {
	tree label_vec = TREE_OPERAND (expr, 2);
	size_t len = TREE_VEC_LENGTH (label_vec);
	size_t i;
	output_global_record_start (ob, expr, expr, tag);
	output_uleb128 (ob, len);
	output_tree (ob, TREE_OPERAND (expr, 0));
	gcc_assert (TREE_OPERAND (expr, 1) == NULL);

	for (i = 0; i < len; ++i)
	  output_tree (ob, TREE_VEC_ELT (label_vec, i));
      }
      break;

    case TREE_LIST:
      {
	tree tl;
	int count = 0;

	output_global_record_start (ob, expr, NULL, tag);
	for (tl = expr; tl; tl = TREE_CHAIN (tl))
	  count++;

	gcc_assert (count);
	output_uleb128 (ob, count);
	for (tl = expr; tl; tl = TREE_CHAIN (tl))
	  {
	    if (TREE_VALUE (tl) != NULL_TREE)
	      output_tree (ob, TREE_VALUE (tl));
	    else
	      output_zero (ob);
	    
	    if (TREE_PURPOSE (tl))
	      output_tree (ob, TREE_PURPOSE (tl));
	    else
	      output_zero (ob);
	  }
      }
      break;

    case TREE_VEC:
      {
	size_t i;
	size_t len = TREE_VEC_LENGTH (expr);

	output_global_record_start (ob, NULL, NULL, tag);
	output_uleb128 (ob, len);
	for (i = 0; i < len; ++i)
	  output_tree (ob, TREE_VEC_ELT (expr, i));
      }
      break;

    case ERROR_MARK:
      /* The canonical error node is preloaded,
         so we should never see another one here.  */
      gcc_unreachable ();

    case VOID_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case FIXED_POINT_TYPE:
    case COMPLEX_TYPE:
    case BOOLEAN_TYPE:
    case OFFSET_TYPE:
    case ENUMERAL_TYPE:
    case POINTER_TYPE:
    case REFERENCE_TYPE:
    case VECTOR_TYPE:
    case ARRAY_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
    case FUNCTION_TYPE:
    case METHOD_TYPE:
      output_type (ob, expr, tag);
      break;

    case LANG_TYPE:
      /* FIXME */
      gcc_unreachable ();

    case TREE_BINFO:
      output_binfo (ob, expr);
      break;

      /* This is the default case. All of the cases that can be done
	 completely mechanically are done here.  */
      {
	int i;
#define SET_NAME(a,b)
#define MAP_EXPR_TAG(expr, tag) case expr:
#define TREE_SINGLE_MECHANICAL_TRUE

#include "lto-tree-tags.def"
#undef MAP_EXPR_TAG
#undef TREE_SINGLE_MECHANICAL_TRUE
#undef SET_NAME
	output_global_record_start (ob, expr, expr, tag);
	for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (expr)); i++)
	  output_tree (ob, TREE_OPERAND (expr, i));
	break;
      }

      /* This is the error case, these are type codes that will either
	 never happen or that we have not gotten around to dealing
	 with are here.  */
    case BIND_EXPR:
    case BLOCK:
    case CATCH_EXPR:
    case EH_FILTER_EXPR:
    case NAME_MEMORY_TAG:
    case OMP_CONTINUE:
    case OMP_CRITICAL:
    case OMP_FOR:
    case OMP_MASTER:
    case OMP_ORDERED:
    case OMP_PARALLEL:
    case OMP_RETURN:
    case OMP_SECTIONS:
    case OMP_SINGLE:
    case SYMBOL_MEMORY_TAG:
    case TARGET_MEM_REF:
    case TRY_CATCH_EXPR:
    case TRY_FINALLY_EXPR:
    default:
      if (TREE_CODE (expr) >= NUM_TREE_CODES)
	{
	  /* EXPR is a language-specific tree node, which has no meaning
	     outside of the front-end.  Something along the lines of
	     http://gcc.gnu.org/ml/gcc-patches/2008-03/msg00349.html
	     should be implemented.  */
	  error ("Unimplemented code (FE-specific): %d", (int) code);
	  gcc_unreachable ();
	}
      else
	{
	  /* All forms must be explicitly handled.  */
	  error ("Unimplemented code: %s", tree_code_name[code]);
	  gcc_unreachable ();
	}
    }

  LTO_DEBUG_UNDENT ();
}

/* Replacement for output_type_ref when serializing globals.  */

void
output_type_tree (struct output_block *ob, tree type)
{
  LTO_DEBUG_TOKEN ("type");
  gcc_assert (type && TYPE_P (type));
  output_tree (ob, type);
}
