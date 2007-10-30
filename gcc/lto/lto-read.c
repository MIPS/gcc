/* Read the gimple representation of a function and it's local
   variables from the memory mapped representation of a a .o file.

   Copyright 2006 Free Software Foundation, Inc.
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
#include "langhooks.h"
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
#include "timevar.h"
#include "dwarf2asm.h"
#include "dwarf2out.h"
#include "output.h"
#include "lto-tags.h"
#include "lto.h"
#include <ctype.h>
#include "cpplib.h"

static enum tree_code tag_to_expr[LTO_last_tag];

/* The number of flags that are defined for each tree code.  */
static int num_flags_for_code[NUM_TREE_CODES];

struct data_in
{
  tree *field_decls;        /* The field decls.  */
  tree *fn_decls;           /* The function decls.  */
  tree *var_decls;          /* The global or static var_decls.  */
  tree *type_decls;         /* The type_decls.  */
  tree *types;              /* All of the types.  */
  int *local_decls_index;   /* The offsets to decode the local_decls.  */
#ifdef LTO_STREAM_DEBUGGING
  int *local_decls_index_d; /* The offsets to decode the local_decls debug info.  */
#endif
  tree *local_decls;        /* The local var_decls and the parm_decls.  */
  tree *labels;             /* All of the labels.  */
  const char * strings;     /* The string table.  */
  unsigned int strings_len; /* The length of the string table.  */
  /* Number of named labels.  Used to find the index of unnamed labels
     since they share space with the named labels.  */
  unsigned int num_named_labels;  
  const char *current_file;
  int current_line;
#ifdef USE_MAPPED_LOCATION  
  int current_col;
#endif
};



/* This hash table is used to hash the file names in the
   source_location field.  Unlike other structures here, this is a
   persistent structure whose data lives for the entire
   compilation.  */

struct string_slot {
  const char *s;
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

  return strcmp (ds1->s, ds2->s) == 0;
}

/* The table to hold the file_names.  */
static htab_t file_name_hash_table;

struct input_block 
{
  const char *data;
  unsigned int p;
  unsigned int len;
};


#ifdef LTO_STREAM_DEBUGGING
static struct lto_debug_context lto_debug_context;
static void debug_out_fun (struct lto_debug_context *, char);
static void dump_debug_stream (struct input_block *, char, char);
#endif

static tree
input_expr_operand (struct input_block *, struct data_in *, struct function *, 
                    enum LTO_tags);
static tree
input_local_var (struct input_block *, struct data_in *, struct function *, unsigned int i);


/* Return the next character of input from IB.  Abort if you
   overrun.  */

static unsigned char 
input_1_unsigned (struct input_block *ib)
{
  gcc_assert (ib->p < ib->len);
  return (ib->data[ib->p++]);
}


/* Read an ULEB128 Number of IB.  */

static unsigned HOST_WIDE_INT 
input_uleb128 (struct input_block *ib)
{
  unsigned HOST_WIDE_INT result = 0;
  int shift = 0;
  unsigned HOST_WIDE_INT byte;

  while (true)
    {
      byte = input_1_unsigned (ib);
      result |= (byte & 0x7f) << shift;
      shift += 7;
      if ((byte & 0x80) == 0)
	{
	  LTO_DEBUG_WIDE ("U", result);
	  return result;
	}
    }
}


/* Read an SLEB128 Number of IB.  */

static HOST_WIDE_INT 
input_sleb128 (struct input_block *ib)
{
  HOST_WIDE_INT result = 0;
  int shift = 0;
  unsigned HOST_WIDE_INT byte;

  while (true)
    {
      byte = input_1_unsigned (ib);
      result |= (byte & 0x7f) << shift;
      shift += 7;
      if ((byte & 0x80) == 0)
	{
	  if ((shift < HOST_BITS_PER_WIDE_INT) && (byte & 0x40))
	    result |= - ((HOST_WIDE_INT)1 << shift);

	  LTO_DEBUG_WIDE ("S", result);
	  return result;
	}
    }
}


/* Read the string at LOC from the string table in DATA_IN.  */

static const char * 
input_string_internal (struct data_in *data_in, unsigned int loc, 
		       unsigned int *rlen)
{
  struct input_block str_tab 
    = {data_in->strings, loc, data_in->strings_len};
  unsigned int len = input_uleb128 (&str_tab);
  const char * result;

  *rlen = len;
  gcc_assert (str_tab.p + len <= data_in->strings_len);

  result = (const char *)(data_in->strings + str_tab.p);
  LTO_DEBUG_STRING (result, len);
  return result;
}


/* Read a STRING_CST at LOC from the string table in DATA_IN.  */

static tree
input_string (struct data_in *data_in, unsigned int loc)
{
  unsigned int len;
  const char * ptr = input_string_internal (data_in, loc, &len);
  return build_string (len, ptr);
}


/* Input a real constant of TYPE at LOC.  */

static tree
input_real (struct input_block *ib, struct data_in *data_in, tree type)
{
  unsigned int loc;
  unsigned int len;
  const char * str;
  REAL_VALUE_TYPE value;

  LTO_DEBUG_TOKEN ("real");
  loc = input_uleb128 (ib);
  str = input_string_internal (data_in, loc, &len);
  real_from_string (&value, str);
  return build_real (type, value);
}


/* Input the next integer constant of TYPE in IB.  */

static tree
input_integer (struct input_block *ib, tree type)
{
  HOST_WIDE_INT low = 0;
  HOST_WIDE_INT high = 0;
  int shift = 0;
  unsigned HOST_WIDE_INT byte;

  while (true)
    {
      byte = input_1_unsigned (ib);
      if (shift < HOST_BITS_PER_WIDE_INT - 7)
	/* Working on the low part.  */
	low |= (byte & 0x7f) << shift;
      else if (shift >= HOST_BITS_PER_WIDE_INT)
	/* Working on the high part.  */
	high |= (byte & 0x7f) << (shift - HOST_BITS_PER_WIDE_INT);
      else
	{
	  /* Working on the transition between the low and high parts.  */
	  low |= (byte & 0x7f) << shift;
	  high |= (byte & 0x7f) >> (HOST_BITS_PER_WIDE_INT - shift);
	}

      shift += 7;
      if ((byte & 0x80) == 0)
	{
	  if (byte & 0x40)
	    {
	      /* The number is negative.  */
	      if (shift < HOST_BITS_PER_WIDE_INT)
		{
		  low |= - ((HOST_WIDE_INT)1 << shift);
		  high = -1;
		}
	      else if (shift < (2 * HOST_BITS_PER_WIDE_INT))
		high |= - ((HOST_WIDE_INT)1 << shift);
	    }

#ifdef LTO_STREAM_DEBUGGING
	  /* Have to match the quick out in the lto writer.  */
	  if (((high == -1) && (low < 0))
	      || ((high == 0) && (low >= 0)))
	    LTO_DEBUG_WIDE ("S", low);
	  else 
	    LTO_DEBUG_INTEGER ("SS", high, low);
#endif	  
	  return build_int_cst_wide (type, low, high);
	}
    }
}


/* Return the next tag in the input block IB.  */

static enum LTO_tags
input_record_start (struct input_block *ib)
{
  enum LTO_tags tag = input_1_unsigned (ib);

#ifdef LTO_STREAM_DEBUGGING
  if (tag)
    LTO_DEBUG_INDENT (tag);
  else
    LTO_DEBUG_WIDE ("U", 0);
#endif    
  return tag;
} 


/* Build a tree list stopping when the tag is 0.  */

static tree 
input_tree_list (struct input_block *ib, struct data_in *data_in, struct function *fn)
{
  unsigned int count = input_uleb128 (ib);

  tree first = NULL_TREE;

  if (count)
    {
      tree next;
      enum LTO_tags tag = input_record_start (ib);

      /* Peel out the first iteration so we return the correct thing.  */
      first = build_tree_list (NULL_TREE, input_expr_operand (ib, data_in, fn, tag));
      next = first;
      count--;

      while (count--)
	{
	  tag = input_record_start (ib);
	  TREE_CHAIN (next) 
	    = build_tree_list (NULL_TREE, input_expr_operand (ib, data_in, fn, tag));
	  next = TREE_CHAIN (next);
	}
    }

  return first;
}


/* Get the label referenced by the next token in IB.  */

static tree 
get_label_decl (struct data_in *data_in, struct input_block *ib)
{
  int index = input_sleb128 (ib);
  if (index >= 0)
    return data_in->labels[index];
  else
    return data_in->labels[data_in->num_named_labels - index];
}


/* Get the type referenced by the next token in IB.  */

static tree
get_type_ref (struct data_in *data_in, struct input_block *ib)
{
  int index;

  LTO_DEBUG_TOKEN ("type");
  index = input_uleb128 (ib);
  return data_in->types[index];
}

/* Set all of the FLAGS for NODE.  */
#define CLEAROUT (HOST_BITS_PER_WIDE_INT - 1)


/* Read the tree flags for CODE from IB.  */

static unsigned HOST_WIDE_INT 
input_tree_flags (struct input_block *ib, enum tree_code code)
{
  unsigned HOST_WIDE_INT flags;

  if (TEST_BIT (lto_flags_needed_for, code))
    {
      LTO_DEBUG_TOKEN ("flags");
      flags = input_uleb128 (ib);
      LTO_DEBUG_TREE_FLAGS (code, flags);
    }
  else
    flags = 0;
  return flags;
}


/* Set all of the flag bits inside EXPR by unpacking FLAGS.  */

static void
process_tree_flags (tree expr, unsigned HOST_WIDE_INT flags)
{
  enum tree_code code = TREE_CODE (expr);
  /* Shift the flags up so that the first flag is at the top of the
     flag word.  */
  flags <<= HOST_BITS_PER_WIDE_INT - num_flags_for_code[code];

#define START_CLASS_SWITCH()              \
  {                                       \
                                          \
    switch (TREE_CODE_CLASS (code))       \
    {

#define START_CLASS_CASE(class)    case class:
#define ADD_CLASS_FLAG(flag_name) \
  { expr->base. flag_name = flags >> CLEAROUT; flags <<= 1; }
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
  { expr->base. flag_name = (flags >> CLEAROUT); flags <<= 1; }
#define ADD_DECL_FLAG(flag_name) \
  { expr->decl_common. flag_name = flags >> CLEAROUT; flags <<= 1; }
#define ADD_VIS_FLAG(flag_name)  \
  { expr->decl_with_vis. flag_name = (flags >> CLEAROUT); flags <<= 1; }
#define ADD_FUNC_FLAG(flag_name) \
  { expr->function_decl. flag_name = (flags >> CLEAROUT); flags <<= 1; }
#define END_EXPR_CASE(class)      break;
#define END_EXPR_SWITCH()                 \
    default:                              \
      gcc_unreachable ();                 \
    }                                     \
  }

#include "lto-tree-flags.def"

#undef START_CLASS_SWITCH
#undef START_CLASS_CASE
#undef ADD_CLASS_FLAG
#undef END_CLASS_CASE
#undef END_CLASS_SWITCH
#undef START_EXPR_SWITCH
#undef START_EXPR_CASE
#undef ADD_EXPR_FLAG
#undef ADD_DECL_FLAG
#undef ADD_VIS_FLAG
#undef ADD_FUNC_FLAG
#undef END_EXPR_CASE
#undef END_EXPR_SWITCH
}


/* Return the one true copy of STRING.  */

static const char *
canon_file_name (const char *string)
{
  void **slot;
  struct string_slot s_slot;
  s_slot.s = string;

  slot = htab_find_slot (file_name_hash_table, &s_slot, INSERT);
  if (*slot == NULL)
    {
      size_t len = strlen (string);
      char * saved_string = xmalloc (len + 1);
      strcpy (saved_string, string);

      struct string_slot *new_slot
	= xmalloc (sizeof (struct string_slot));

      new_slot->s = saved_string;
      *slot = new_slot;
      return saved_string;
    }
  else
    {
      struct string_slot *old_slot = (struct string_slot *)*slot;
      return old_slot->s;
    }
}


/* Based on the FLAGS, read in a file, a line and a col into the
   fields in DATA_IN.  */

static void 
input_line_info (struct input_block *ib, struct data_in *data_in, 
		 unsigned HOST_WIDE_INT flags)
{
#ifdef USE_MAPPED_LOCATION  
  if (flags & LTO_SOURCE_FILE)
    {
      unsigned int len;
      if (data_in->current_file)
	linemap_add (line_table, LC_LEAVE, false, data_in->current_file, 0);

      LTO_DEBUG_TOKEN ("file");
      data_in->current_file 
	= canon_file_name (input_string_internal (data_in, input_uleb128 (ib), &len));
    }
  if (flags & LTO_SOURCE_LINE)
    {
      LTO_DEBUG_TOKEN ("line");
      data_in->current_line = input_uleb128 (ib);

      if (!(flags & LTO_SOURCE_FILE))
	linemap_line_start (line_table, data_in->current_line, 80);
    }
  if (flags & LTO_SOURCE_FILE)
    linemap_add (line_table, LC_ENTER, false, data_in->current_file, data_in->current_line);
    
  if (flags & LTO_SOURCE_COL)
    {
      LTO_DEBUG_TOKEN ("col");
      data_in->current_col = input_uleb128 (ib);
    }
#else
  if (flags & LTO_SOURCE_FILE)
    {
      unsigned int len;
      LTO_DEBUG_TOKEN ("file");
      data_in->current_file 
	= input_string_internal (data_in, input_uleb128 (ib), &len);
    }
  if (flags & LTO_SOURCE_LINE)
    {
      LTO_DEBUG_TOKEN ("line");
      data_in->current_line = input_uleb128 (ib);
    }
#endif
}


/* Set the line info stored in DATA_IN for NODE.  */

static void
set_line_info (struct data_in *data_in, tree node)
{
  if (data_in->current_file)
    {
#ifdef USE_MAPPED_LOCATION
  


#else
      if (EXPR_P (node))
	  annotate_with_file_line (node, 
				   canon_file_name (data_in->current_file), 
				   data_in->current_line);
      else if (DECL_P (node))
	{
	  DECL_SOURCE_LOCATION (node).file = canon_file_name (data_in->current_file);
	  DECL_SOURCE_LOCATION (node).line = data_in->current_line;
	}
#endif
    }
}


/* Clear the line info stored in DATA_IN.  */

static void
clear_line_info (struct data_in *data_in)
{
#ifdef USE_MAPPED_LOCATION  
  if (data_in->current_file)
    linemap_add (line_table, LC_LEAVE, false, data_in->current_file, 0);
  data_in->current_col = 0;
#endif
  data_in->current_file = NULL;
  data_in->current_line = 0;
}


/* Read a node in the gimple tree from IB.  The TAG has already been
   read.  */

static tree
input_expr_operand (struct input_block *ib, struct data_in *data_in, 
		    struct function *fn, enum LTO_tags tag)
{
  enum tree_code code = tag_to_expr[tag];
  tree type = NULL_TREE;
  unsigned HOST_WIDE_INT flags;
  gcc_assert (code);
  tree result = NULL_TREE;
  
  if (TEST_BIT (lto_types_needed_for, code))
    type = get_type_ref (data_in, ib);

  flags = input_tree_flags (ib, code);

  /* FIXME! need to figure out how to set the file and line number.  */
  if (IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (code)))
    input_line_info (ib, data_in, flags);

  switch (code)
    {
    case COMPLEX_CST:
      result = build0 (code, type);
      if (tag == LTO_complex_cst1)
	{
	  TREE_REALPART (result) 
	    = input_real (ib, data_in, type);
	  TREE_IMAGPART (result) 
	    = input_real (ib, data_in, type);
	}
      else
	{
	  TREE_REALPART (result) = input_integer (ib, type);
	  TREE_IMAGPART (result) = input_integer (ib, type);
	}
      break;

    case INTEGER_CST:
      result = input_integer (ib, type);
      break;

    case REAL_CST:
      result = input_real (ib, data_in, type);
      break;

    case STRING_CST:
      result = input_string (data_in, input_uleb128 (ib));
      TREE_TYPE (result) = type;
      break;

    case VECTOR_CST:
      {
	tree chain = NULL_TREE;
	int len = input_uleb128 (ib);

	if (len && tag == LTO_vector_cst1)
	  {
	    int i;
	    tree last 
	      = build_tree_list (NULL_TREE, input_real (ib, data_in, type));
	    chain = last; 
	    for (i = 1; i < len; i++)
	      {
		tree t 
		  = build_tree_list (NULL_TREE, input_real (ib, data_in, type));
		TREE_CHAIN (last) = t;
		last = t;
	      }
	  }
	else
	  {
	    int i;
	    tree last = build_tree_list (NULL_TREE, input_integer (ib, type));
	    chain = last; 
	    for (i = 1; i < len; i++)
	      {
		tree t 
		  = build_tree_list (NULL_TREE, input_integer (ib, type));
		TREE_CHAIN (last) = t;
		last = t;
	      }
	  }
	result = build_vector (type, chain);
      }
      break;

    case CASE_LABEL_EXPR:
      {
	int variant = tag - LTO_case_label_expr0;
	tree op0 = NULL_TREE;
	tree op1 = NULL_TREE;
	
	if (variant & 0x1)
	  op0 = input_expr_operand (ib, data_in, fn, 
				    input_record_start (ib));

	if (variant & 0x2)
	  op1 = input_expr_operand (ib, data_in, fn, 
				    input_record_start (ib));

	result = build3 (code, void_type_node, 
			 op0, op1, get_label_decl (data_in, ib));
      }
      break;

    case CONSTRUCTOR:
      {
	VEC(constructor_elt,gc) *vec = NULL;
	unsigned int len = input_uleb128 (ib);
	
	if (len)
	  {
	    tree purpose = NULL_TREE;
	    unsigned int i = 0;
	    vec = VEC_alloc (constructor_elt, gc, len);
	    while (i < len)
	      {
		enum LTO_tags ctag = input_record_start (ib);
		if (ctag == LTO_constructor_range)
		  {
		    tree op0 = input_integer (ib, get_type_ref (data_in, ib));
		    tree op1 = input_integer (ib, get_type_ref (data_in, ib));
		    purpose = build2 (RANGE_EXPR, sizetype, op0, op1);
		    LTO_DEBUG_UNDENT();
		  }
		else
		  {
		    tree value = 
		      input_expr_operand (ib, data_in, fn, ctag);
		    constructor_elt *elt 
		      = VEC_quick_push (constructor_elt, vec, NULL);
		    elt->index = purpose;
		    elt->value = value;
		    
		    /* Only use the range once.  */
		    purpose = NULL_TREE;
		    i++;
		  }
	      }
	  }
	result = build_constructor (type, vec);
      }
      break;

    case SSA_NAME:
      result = VEC_index (tree, SSANAMES (fn), input_uleb128 (ib));
      break;

    case CONST_DECL:
      /* Just ignore these, Mark will make them disappear.  */
      break;

    case FIELD_DECL:
      result = data_in->field_decls [input_uleb128 (ib)];
      break;

    case FUNCTION_DECL:
      result = data_in->fn_decls [input_uleb128 (ib)];
      gcc_assert (result);
      break;

    case TYPE_DECL:
      result = data_in->type_decls [input_uleb128 (ib)];
      gcc_assert (result);
      break;

    case VAR_DECL:
    case PARM_DECL:
      if (tag == LTO_var_decl1)
	/* Static or externs are here.  */
	result = data_in->var_decls [input_uleb128 (ib)];
      else 
	{
	  /* Locals are here.  */
	  int lv_index = input_uleb128 (ib);
	  result = data_in->local_decls [lv_index];
	  if (result == NULL)
	    {
	      struct input_block lib;

#ifdef LTO_STREAM_DEBUGGING
	      struct input_block *current = lto_debug_context.current_data;
	      struct input_block debug;
	      int current_indent = lto_debug_context.indent;

	      debug.data = current->data;
	      debug.len = current->len;
	      debug.p = data_in->local_decls_index_d[lv_index];

	      lto_debug_context.indent = 0;
	      lto_debug_context.current_data = &debug;
#endif

	      lib.data = ib->data;
	      lib.len = ib->len;
	      lib.p = data_in->local_decls_index[lv_index];

	      result = input_local_var (&lib, data_in, fn, lv_index); 
	      data_in->local_decls [lv_index] = result;

#ifdef LTO_STREAM_DEBUGGING
	      lto_debug_context.indent = current_indent;
	      lto_debug_context.current_data = current;
#endif

	    }
	}
      break;

    case LABEL_DECL:
      result = get_label_decl (data_in, ib);
      break;

    case LABEL_EXPR:
      result = build1 (code, void_type_node, get_label_decl (data_in, ib));
      if (!DECL_CONTEXT (LABEL_EXPR_LABEL (result)))
	DECL_CONTEXT (LABEL_EXPR_LABEL (result)) = fn->decl;
      break;

    case COND_EXPR:
      if (tag == LTO_cond_expr0)
	{
	  tree op0;
	  tree op1;
	  tree op2;
	  op0 = input_expr_operand (ib, data_in, fn, 
				    input_record_start (ib));
	  op1 = input_expr_operand (ib, data_in, fn, 
				    input_record_start (ib));
	  op2 = input_expr_operand (ib, data_in, fn, 
				    input_record_start (ib));
	  result = build3 (code, type, op0, op1, op2);
	}
      else
	{
	  tree op0;
	  op0 = input_expr_operand (ib, data_in, fn, 
				    input_record_start (ib));
	  result = build3 (code, type, op0, NULL, NULL);
	}
      break;
      

    case RESULT_DECL:
      result = build0 (code, type);
      break;

    case COMPONENT_REF:
      {
	tree op0;
	tree op1;
	op0 = input_expr_operand (ib, data_in, fn, 
				  input_record_start (ib));
	op1 = input_expr_operand (ib, data_in, fn,
				  input_record_start (ib));
  
	/* Ignore 3 because it can be recomputed.  */
	result = build3 (code, type, op0, op1, NULL_TREE);
      }
      break;

    case CALL_EXPR:
      {
	unsigned int i;
	unsigned int count = input_uleb128 (ib);
	tree op1;
	tree op2 = NULL_TREE;

	/* The call chain.  */
	if (tag == LTO_call_expr1)
	  op2 = input_expr_operand (ib, data_in, fn, 
				    input_record_start (ib));

	/* The callee.  */
	op1 = input_expr_operand (ib, data_in, fn, 
				  input_record_start (ib));

	result = build_vl_exp (code, count);
	CALL_EXPR_FN (result) = op1;
	CALL_EXPR_STATIC_CHAIN (result) = op2;
	for (i = 3; i < count; i++)
	  TREE_OPERAND (result, i) 
	    = input_expr_operand (ib, data_in, fn, 
				  input_record_start (ib));
        TREE_TYPE (result) = type;
      }
      break;

    case BIT_FIELD_REF:
      {
	tree op0;
	tree op1;
	tree op2;
	if (tag == LTO_bit_field_ref1)
	  {
	    op1 = build_int_cst_wide (sizetype, input_uleb128 (ib), 0);
	    op2 = build_int_cst_wide (sizetype, input_uleb128 (ib), 0);
	    op0 = input_expr_operand (ib, data_in, fn,
				      input_record_start (ib));
	  }
	else
	  {
	    op0 = input_expr_operand (ib, data_in, fn,
				      input_record_start (ib));
	    op1 = input_expr_operand (ib, data_in, fn,
				      input_record_start (ib));
	    op2 = input_expr_operand (ib, data_in, fn,
				      input_record_start (ib));
	  }
	result = build3 (code, type, op0, op1, op2);
      }
      break;

    case ARRAY_REF:
    case ARRAY_RANGE_REF:
      /* Ignore operands 2 and 3 for ARRAY_REF and ARRAY_RANGE REF
	 because they can be recomputed.  */
      {
	tree op0 = input_expr_operand (ib, data_in, fn, 
				       input_record_start (ib));
	tree op1 = input_expr_operand (ib, data_in, fn,
				       input_record_start (ib));
	result = build4 (code, type, op0, op1, NULL_TREE, NULL_TREE);
      }
      break;

    case ASM_EXPR:
      {
	tree str = input_string (data_in, input_uleb128 (ib));
	tree ins = input_tree_list (ib, data_in, fn); 
	tree outs = input_tree_list (ib, data_in, fn); 
	tree clobbers = input_tree_list (ib, data_in, fn);
	tree tl;
	result = build4 (code, void_type_node, str, outs, ins, clobbers);

	for (tl = ASM_OUTPUTS (result); tl; tl = TREE_CHAIN (tl))
	  if (TREE_CODE (TREE_VALUE (tl)) == SSA_NAME)
	    SSA_NAME_DEF_STMT (TREE_VALUE (tl)) = result;
      }
      break;

    case RESX_EXPR:
      result = build1 (code, void_type_node, input_integer (ib, NULL_TREE));
      break;

    case RETURN_EXPR:
      switch (tag) 
	{
	case LTO_return_expr0:
	  result = build1 (code, type, NULL_TREE);
	  break;
	  
	case LTO_return_expr1:
	  result = build1 (code, type, 
			   input_expr_operand (ib, data_in, fn, 
					       input_record_start (ib)));
	  break;
	  
	case LTO_return_expr2:
	  {
	    tree op0 = input_expr_operand (ib, data_in, fn,
					   input_record_start (ib));
	    tree op1 = input_expr_operand (ib, data_in, fn,
					   input_record_start (ib));
	    result = build1 (code, type, 
			     build2 (MODIFY_EXPR, NULL_TREE, op0, op1));
	  }
	  break;

        default:
          gcc_unreachable ();
	}
      break;

    case GIMPLE_MODIFY_STMT:
      {
	tree op0 = input_expr_operand (ib, data_in, fn, 
				       input_record_start (ib));
	tree op1 = input_expr_operand (ib, data_in, fn,
				       input_record_start (ib));

	result = build_gimple_modify_stmt (op0, op1);
	if (TREE_CODE (op0) == SSA_NAME)
	  SSA_NAME_DEF_STMT (op0) = result;
      }
      break;

    case SWITCH_EXPR:
      {
	unsigned int len = input_uleb128 (ib);
	unsigned int i;
	tree op0 = input_expr_operand (ib, data_in, fn, 
				       input_record_start (ib));
	tree op2 = make_tree_vec (len);
	
	for (i = 0; i < len; ++i)
	  TREE_VEC_ELT (op2, i) 
	    = input_expr_operand (ib, data_in, fn,
				  input_record_start (ib));
	result = build3 (code, void_type_node, op0, NULL_TREE, op2);
      }
      break;

      /* This is the default case. All of the cases that can be done
	 completely mechanically are done here.  */
#define SET_NAME(a,b)
#define TREE_SINGLE_MECHANICAL_TRUE
#define MAP_EXPR_TAG(expr,tag) case expr:
#include "lto-tree-tags.def"
#undef MAP_EXPR_TAG
#undef TREE_SINGLE_MECHANICAL_TRUE
#undef SET_NAME
      {
	tree ops[7];
	int len = TREE_CODE_LENGTH (code);
	int i;
	for (i = 0; i<len; i++)
	  ops[i] = input_expr_operand (ib, data_in, fn, 
				       input_record_start (ib));
	switch (len)
	  {
	  case 0:
	    result = build0 (code, type);
	    break;
	  case 1:
	    result = build1 (code, type, ops[0]);
	    break;
	  case 2:
	    result = build2 (code, type, ops[0], ops[1]);
	    break;
	  case 3:
	    result = build3 (code, type, ops[0], ops[1], ops[2]);
	    break;
	  case 4:
	    result = build4 (code, type, ops[0], ops[1], ops[2], ops[3]);
	    break;
	  case 5:
	    result = build5 (code, type, ops[0], ops[1], ops[2], ops[3], 
			     ops[4]);
	    break;
            /* No 'case 6'.  */
	  case 7:
	    result = build7 (code, type, ops[0], ops[1], ops[2], ops[3], 
			     ops[4], ops[5], ops[6]);
	    break;
	  default:
	    gcc_unreachable ();
	  }
      }
      break;
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
    case STRUCT_FIELD_TAG:
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

  LTO_DEBUG_UNDENT();
  if (flags)
    process_tree_flags (result, flags);

  if (IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (code)))
    set_line_info (data_in, result);

  /* It is not enought to just put the flags back as we serialized
     them.  There are side effects to the buildN functions which play
     with the flags to the point that we just have to call this here
     to get it right.  */
  if (code == ADDR_EXPR)
    recompute_tree_invariant_for_addr_expr (result);
  return result;
}


/* Load in the global vars and all of the types from the main symbol
   table.  */

static void
input_globals (struct lto_header * header,
	      lto_info_fd *fd,
	      lto_context *context,	
	      struct data_in *data_in, 
	      lto_ref in_field_decls[],
	      lto_ref in_fn_decls[],
	      lto_ref in_var_decls[],
	      lto_ref in_type_decls[],
	      lto_ref in_types[])
{
  int i;
  data_in->field_decls = xcalloc (header->num_field_decls, sizeof (tree*));
  data_in->fn_decls    = xcalloc (header->num_fn_decls, sizeof (tree*));
  data_in->var_decls   = xcalloc (header->num_var_decls, sizeof (tree*));
  data_in->type_decls  = xcalloc (header->num_type_decls, sizeof (tree*));
  data_in->types       = xcalloc (header->num_types, sizeof (tree*));

  for (i=0; i<header->num_field_decls; i++)
    data_in->field_decls[i] 
      = lto_resolve_field_ref (fd, context, &in_field_decls[i]);

  for (i=0; i<header->num_fn_decls; i++)
    data_in->fn_decls[i] 
      = lto_resolve_fn_ref (fd, context, &in_fn_decls[i]);

  for (i=0; i<header->num_var_decls; i++)
    data_in->var_decls[i] 
      = lto_resolve_var_ref (fd, context, &in_var_decls[i]);

  for (i=0; i<header->num_type_decls; i++)
    data_in->type_decls[i] 
      = lto_resolve_typedecl_ref (fd, context, &in_type_decls[i]);

  for (i=0; i<header->num_types; i++)
    data_in->types[i] = lto_resolve_type_ref (fd, context, &in_types[i]);
}


/* Load NAMED_COUNT named labels and constuct UNNAMED_COUNT unnamed
   labels from DATA segment SIZE bytes long using DATA_IN.  */

static void 
input_labels (struct input_block *ib, struct data_in *data_in, 
	      unsigned int named_count, unsigned int unnamed_count)
{
  unsigned int i;

  clear_line_info (data_in);
  /* The named and unnamed labels share the same array.  In the lto
     code, the unnamed labels have a negative index.  Their position
     in the array can be found by subtracting that index from the
     number of named labels.  */
  data_in->labels = xcalloc (named_count + unnamed_count, sizeof (tree*));
  for (i = 0; i < named_count; i++)
    {
      unsigned int name_index = input_uleb128 (ib);
      unsigned int len;
      const char *s = input_string_internal (data_in, name_index, &len);
      tree name = get_identifier_with_length (s, len);
      data_in->labels[i] = build_decl (LABEL_DECL, name, void_type_node);
    }

  for (i = 0; i < unnamed_count; i++)
    data_in->labels[i + named_count] 
      = build_decl (LABEL_DECL, NULL_TREE, void_type_node);
 }


/* Input the local var index table.  */


static void
input_local_vars_index (struct input_block *ib, struct data_in *data_in, 
			unsigned int count)
{
  unsigned int i;
  data_in->local_decls_index = xcalloc (count, sizeof (unsigned int));
#ifdef LTO_STREAM_DEBUGGING
  data_in->local_decls_index_d = xcalloc (count, sizeof (unsigned int));
#endif

  for (i = 0; i < count; i++)
    {
      data_in->local_decls_index[i] = input_uleb128 (ib); 
#ifdef LTO_STREAM_DEBUGGING
      data_in->local_decls_index_d[i] = input_uleb128 (ib); 
#endif
    }
}


/* Input local var I from IB.  */

static tree
input_local_var (struct input_block *ib, struct data_in *data_in, 
		 struct function *fn, unsigned int i)
{
  enum LTO_tags tag;
  unsigned int variant;
  bool is_var;
  unsigned int name_index;
  tree name;
  tree type;
  unsigned HOST_WIDE_INT flags;
  tree result;
  tree context;

  /* The line number info needs to be reset for each local var since
     they are read in random order.  */
  clear_line_info (data_in);

  tag = input_record_start (ib);
  variant = tag & 0xF;
  is_var = ((tag & 0xFFF0) == LTO_local_var_decl_body0);
  
  name_index = input_uleb128 (ib);
  if (name_index)
    {
      unsigned int len;
      const char *s = input_string_internal (data_in, name_index, &len);
      name = get_identifier_with_length (s, len);
    }
  else 
    name = NULL_TREE;
  
  type = get_type_ref (data_in, ib);
  gcc_assert (type);
  
  if (is_var)
    result = build_decl (VAR_DECL, name, type);
  else
    result = build_decl (PARM_DECL, name, type);

  data_in->local_decls[i] = result;
  
  if (!is_var)
    DECL_ARG_TYPE (result) = get_type_ref (data_in, ib);

  flags = input_tree_flags (ib, LTO_flags_needed);
  input_line_info (ib, data_in, flags);
  set_line_info (data_in, result);

  LTO_DEBUG_TOKEN ("chain");
  tag = input_record_start (ib);
  if (tag)
    TREE_CHAIN (result) = input_expr_operand (ib, data_in, fn, tag);
  else 
    TREE_CHAIN (result) = NULL_TREE;

  LTO_DEBUG_TOKEN ("context");
  context = input_expr_operand (ib, data_in, fn, input_record_start (ib));
  if (TYPE_P (context))
    DECL_CONTEXT (result) = TYPE_NAME (context);
  else
    DECL_CONTEXT (result) = context;
  
  LTO_DEBUG_TOKEN ("align");
  DECL_ALIGN (result) = input_uleb128 (ib);
  LTO_DEBUG_TOKEN ("size");
  DECL_SIZE (result) = input_expr_operand (ib, data_in, fn, input_record_start (ib));

  if (variant & 0x1)
    {
      LTO_DEBUG_TOKEN ("attributes");
      DECL_ATTRIBUTES (result) = input_tree_list (ib, data_in, fn);
    }
  if (variant & 0x2)
    DECL_SIZE_UNIT (result) 
      = input_expr_operand (ib, data_in, fn, input_record_start (ib));
  if (variant & 0x4)
    SET_DECL_DEBUG_EXPR (result, 
			 input_expr_operand (ib, data_in, fn, 
					     input_record_start (ib)));
  if (variant & 0x8)
    DECL_ABSTRACT_ORIGIN (result) 
      = input_expr_operand (ib, data_in, fn, input_record_start (ib));
  
  process_tree_flags (result, flags);
  LTO_DEBUG_UNDENT();

  /* Record the variable.  */
  if (is_var)
    fn->unexpanded_var_list = tree_cons (NULL_TREE, result,
					 fn->unexpanded_var_list);

  return result;
}


/* Load COUNT local var_decls and parm_decls from a DATA segment SIZE
   bytes long using DATA_IN.  */

static void 
input_local_vars (struct input_block *ib, struct data_in *data_in, 
		  struct function *fn, unsigned int count)
{
  unsigned int i;

  data_in->local_decls = xcalloc (count, sizeof (tree*));
  for (i = 0; i < count; i++)
    /* Some local decls may have already been read in if they are used
       as part of a previous local_decl.  */
    if (!data_in->local_decls[i])
      {
#ifdef LTO_STREAM_DEBUGGING
	((struct input_block *)lto_debug_context.current_data)->p 
	  = data_in->local_decls_index_d[i]; 
#endif
	ib->p = data_in->local_decls_index[i];
	input_local_var (ib, data_in, fn, i);
      }
}


/* Read the exception table.  */

static void
input_eh_regions (struct input_block *ib, 
		  struct function *fn ATTRIBUTE_UNUSED, 
		  struct data_in *data_in ATTRIBUTE_UNUSED)
{
  /* Not ready to read exception records yet.  */
  input_uleb128 (ib);
}


/* Make a new basic block at INDEX in FN.  */

static basic_block
make_new_block (struct function *fn, unsigned int index)
{
  basic_block bb = alloc_block ();
  bb->index = index;
  SET_BASIC_BLOCK_FOR_FUNCTION (fn, index, bb);
  bb->il.tree = GGC_CNEW (struct tree_bb_info);
  bb->flags = 0;
  set_bb_stmt_list (bb, alloc_stmt_list ());
  return bb;
}


/* Set up the cfg for THIS_FUN.  */

static void 
input_cfg (struct input_block *ib, struct function *fn)
{
  unsigned int bb_count;
  basic_block p_bb;
  unsigned int i;
  int index;

  init_flow (fn);

  LTO_DEBUG_TOKEN ("lastbb");
  bb_count = input_uleb128 (ib);

  profile_status_for_function (fn) = PROFILE_ABSENT;
  n_basic_blocks_for_function (fn) = bb_count;
  last_basic_block_for_function (fn) = bb_count;
  basic_block_info_for_function (fn)
    = VEC_alloc (basic_block, gc, bb_count);
  VEC_safe_grow (basic_block, gc,
                 basic_block_info_for_function (fn), bb_count);
  memset (VEC_address (basic_block, 
		       basic_block_info_for_function (fn)), 
	  0, sizeof (basic_block) * bb_count);

  /* Build a mapping of labels to their associated blocks.  */
  label_to_block_map_for_function (fn)
    = VEC_alloc (basic_block, gc, bb_count);
  VEC_safe_grow (basic_block, gc, 
		 label_to_block_map_for_function (fn), bb_count);
  memset (VEC_address (basic_block, 
		       label_to_block_map_for_function (fn)),
	  0, sizeof (basic_block) * bb_count);

  SET_BASIC_BLOCK_FOR_FUNCTION (fn, ENTRY_BLOCK, 
				ENTRY_BLOCK_PTR_FOR_FUNCTION (fn));
  SET_BASIC_BLOCK_FOR_FUNCTION (fn, EXIT_BLOCK, 
		   EXIT_BLOCK_PTR_FOR_FUNCTION (fn));

  LTO_DEBUG_TOKEN ("bbindex");
  index = input_sleb128 (ib);
  while (index != -1)
    {
      basic_block bb = BASIC_BLOCK_FOR_FUNCTION (fn, index);
      unsigned int edge_count;

      if (bb == NULL)
	bb = make_new_block (fn, index);

      LTO_DEBUG_TOKEN ("edgecount");
      edge_count = input_uleb128 (ib);

      /* Connect up the cfg.  */
      for (i = 0; i < edge_count; i++)
	{
	  unsigned int dest_index;
	  unsigned int edge_flags;
	  basic_block dest;

	  LTO_DEBUG_TOKEN ("dest");
	  dest_index = input_uleb128 (ib);
	  LTO_DEBUG_TOKEN ("eflags");
	  edge_flags = input_uleb128 (ib);
	  dest = BASIC_BLOCK_FOR_FUNCTION (fn, dest_index);

	  if (dest == NULL) 
	    dest = make_new_block (fn, dest_index);
	  make_edge (bb, dest, edge_flags);
	}

      LTO_DEBUG_TOKEN ("bbindex");
      index = input_sleb128 (ib);
    }

  p_bb = ENTRY_BLOCK_PTR_FOR_FUNCTION(fn);
  for (i = NUM_FIXED_BLOCKS; i < bb_count; i++)
    {
      basic_block bb = BASIC_BLOCK_FOR_FUNCTION (fn, i);
      if (bb)
	{
	  bb->prev_bb = p_bb;
	  p_bb->next_bb = bb;
	  p_bb = bb;
	}
    }

  p_bb->next_bb = EXIT_BLOCK_PTR_FOR_FUNCTION(fn);
  EXIT_BLOCK_PTR_FOR_FUNCTION(fn)->prev_bb = p_bb;
}


/* Input the next phi function for BB.  */

static tree
input_phi (struct input_block *ib, basic_block bb, 
	   struct data_in *data_in, struct function *fn)
{
  unsigned HOST_WIDE_INT flags = input_tree_flags (ib, PHI_NODE);

  tree phi_result = VEC_index (tree, SSANAMES (fn), input_uleb128 (ib));
  int len = EDGE_COUNT (bb->preds);
  int i;
  tree result = create_phi_node (phi_result, bb);

  SSA_NAME_DEF_STMT (phi_result) = result;

  for (i = 0; i < len; i++)
    {
      tree def = input_expr_operand (ib, data_in, fn, input_record_start (ib));
      int src_index = input_uleb128 (ib);
      basic_block sbb = BASIC_BLOCK_FOR_FUNCTION (fn, src_index);
      
      edge e = NULL;
      int j;
      
      for (j = 0; j < len; j++)
	if (EDGE_PRED (bb, j)->src == sbb)
	  {
	    e = EDGE_PRED (bb, j);
	    break;
	  }

      add_phi_arg (result, def, e); 
    }

  if (flags)
    process_tree_flags (result, flags);

  LTO_DEBUG_UNDENT();

  return result;
}


/* Read in the ssa_names array from IB.  */

static void
input_ssa_names (struct input_block *ib, struct data_in *data_in, struct function *fn)
{
  unsigned int i;
  int size = input_uleb128 (ib);

  init_tree_ssa (fn);
  init_ssanames (fn, size);
  i = input_uleb128 (ib);
  
  while (i)
    {
      tree ssa_name;
      tree name;
      unsigned HOST_WIDE_INT flags;

      /* Skip over the elements that had been freed.  */
      while (VEC_length (tree, SSANAMES (fn)) < i)
	VEC_quick_push (tree, SSANAMES (fn), NULL_TREE);

      name = input_expr_operand (ib, data_in, fn, input_record_start (ib));
      ssa_name = make_ssa_name (fn, name, build_empty_stmt ());

      flags = input_tree_flags (ib, LTO_flags_needed);
      process_tree_flags (ssa_name, flags);

      i = input_uleb128 (ib);
    } 
}

 
/* Read in the next basic block.  */

static void
input_bb (struct input_block *ib, enum LTO_tags tag, 
	  struct data_in *data_in, struct function *fn)
{
  unsigned int index;
  basic_block bb;
  block_stmt_iterator bsi;

  LTO_DEBUG_TOKEN ("bbindex");
  index = input_uleb128 (ib);
  bb = BASIC_BLOCK_FOR_FUNCTION (fn, index);

  /* LTO_bb1 has stmts, LTO_bb0 does not.  */
  if (tag == LTO_bb0)
    {
      LTO_DEBUG_UNDENT();
      return;
    }

  bsi = bsi_start (bb);
  LTO_DEBUG_INDENT_TOKEN ("stmt");
  tag = input_record_start (ib);
  while (tag)
    {
      tree stmt = input_expr_operand (ib, data_in, fn, tag);
      bsi_insert_after (&bsi, stmt, BSI_NEW_STMT);
      LTO_DEBUG_INDENT_TOKEN ("stmt");
      tag = input_record_start (ib);
      /* FIXME, add code to handle the exception.  */
    }

  LTO_DEBUG_INDENT_TOKEN ("phi");
  tag = input_record_start (ib);
  while (tag)
    {
      input_phi (ib, bb, data_in, fn);
      LTO_DEBUG_INDENT_TOKEN ("phi");
      tag = input_record_start (ib);
    }

  LTO_DEBUG_UNDENT();
}


/* Fill in the body of FN_DECL.  */

static void
input_function (tree fn_decl, struct data_in *data_in, 
		struct input_block *ib)
{
  struct function *fn = DECL_STRUCT_FUNCTION (fn_decl);
  enum LTO_tags tag = input_record_start (ib);

  DECL_INITIAL (fn_decl) = DECL_SAVED_TREE (fn_decl) = make_node (BLOCK);
  BLOCK_ABSTRACT_ORIGIN (DECL_SAVED_TREE (fn_decl)) = fn_decl;
  clear_line_info (data_in);

  tree_register_cfg_hooks ();
  gcc_assert (tag == LTO_function);

  input_eh_regions (ib, fn, data_in);

  LTO_DEBUG_INDENT_TOKEN ("decl_arguments");
  tag = input_record_start (ib);
  if (tag)
    DECL_ARGUMENTS (fn_decl) = input_expr_operand (ib, data_in, fn, tag); 

  tag = input_record_start (ib);
  while (tag)
    {
      input_bb (ib, tag, data_in, fn);
      tag = input_record_start (ib);
    }

  LTO_DEBUG_UNDENT();
}


/* Fill in the body of VAR.  */

static void
input_constructor (tree var, struct data_in *data_in, 
		   struct input_block *ib)
{
  enum LTO_tags tag;

  clear_line_info (data_in);
  LTO_DEBUG_INDENT_TOKEN ("init");
  tag = input_record_start (ib);
  DECL_INITIAL (var) = input_expr_operand (ib, data_in, NULL, tag);
}


static bool initialized_local = false;


/* Static initialization for the lto reader.  */

static void
lto_static_init_local (void)
{
  if (initialized_local)
    return;

  initialized_local = true;

  /* Initialize the expression to tag mapping.  */
#define MAP_EXPR_TAG(expr,tag)   tag_to_expr [tag] = expr;
#define MAP_EXPR_TAGS(expr,tag,count) \
  {                                   \
    int i;                            \
    for (i=0; i<count; i++)           \
      tag_to_expr [tag + i] = expr;   \
  }
#define TREE_MULTIPLE
#define TREE_SINGLE_MECHANICAL_TRUE
#define TREE_SINGLE_MECHANICAL_FALSE
#define SET_NAME(a,b)
#include "lto-tree-tags.def"

#undef MAP_EXPR_TAG
#undef MAP_EXPR_TAGS
#undef TREE_MULTIPLE
#undef TREE_SINGLE_MECHANICAL_TRUE
#undef TREE_SINGLE_MECHANICAL_FALSE
#undef SET_NAME
  /* Initialize num_flags_for_code.  */


#define START_CLASS_SWITCH()                  \
  {                                           \
    int code;				      \
    for (code=0; code<NUM_TREE_CODES; code++) \
      {                                       \
	/* The LTO_SOURCE_LOC_BITS leaves room for file and line number for exprs.  */ \
        num_flags_for_code[code] = LTO_SOURCE_LOC_BITS; \
                                              \
        switch (TREE_CODE_CLASS (code))       \
          {

#define START_CLASS_CASE(class)    case class:
#define ADD_CLASS_FLAG(flag_name)    num_flags_for_code[code]++;
#define END_CLASS_CASE(class)      break;
#define END_CLASS_SWITCH()                    \
          default:                            \
	    fprintf (stderr, "no declaration for TREE CODE CLASS for = %s(%d)\n", \
                     tree_code_name[code], code);                                 \
            gcc_unreachable ();               \
          }


#define START_EXPR_SWITCH()                   \
        switch (code)			      \
          {
#define START_EXPR_CASE(code)    case code:
#define ADD_EXPR_FLAG(flag_name)    num_flags_for_code[code]++;
#define ADD_DECL_FLAG(flag_name)    num_flags_for_code[code]++;
#define ADD_VIS_FLAG(flag_name)     num_flags_for_code[code]++;
#define ADD_FUNC_FLAG(flag_name)    num_flags_for_code[code]++;
#define END_EXPR_CASE(class)      break;
#define END_EXPR_SWITCH()                     \
          default:                            \
	    fprintf (stderr, "no declaration for TREE CODE = %s(%d)\n", \
                     tree_code_name[code], code);		        \
            gcc_unreachable ();               \
          }                                   \
      }					      \
  }

#include "lto-tree-flags.def"

#undef START_CLASS_SWITCH
#undef START_CLASS_CASE
#undef ADD_CLASS_FLAG
#undef END_CLASS_CASE
#undef END_CLASS_SWITCH
#undef START_EXPR_SWITCH
#undef START_EXPR_CASE
#undef ADD_EXPR_FLAG
#undef ADD_DECL_FLAG
#undef ADD_VIS_FLAG
#undef ADD_FUNC_FLAG
#undef END_EXPR_CASE
#undef END_EXPR_SWITCH

  lto_static_init ();
  tree_register_cfg_hooks ();

  file_name_hash_table
    = htab_create (37, hash_string_slot_node, eq_string_slot_node, free);
}


/* Read the body form DATA for tree T and fill it in.
   FD and CONTEXT are magic cookies used to resolve global decls and
   types.  IN_FUNCTION should be true if DATA describes the
   body of a function, or false otherwise.  */
static void 
lto_read_body (lto_info_fd *fd,
	       lto_context *context,
	       tree t,
	       const void *data,
	       bool in_function)
{
  struct lto_header * header 
    = (struct lto_header *) data;
  struct data_in data_in;

  int32_t fields_offset = sizeof (struct lto_header); 
  int32_t fns_offset 
    = fields_offset + (header->num_field_decls * sizeof (lto_ref));
  int32_t vars_offset 
    = fns_offset + (header->num_fn_decls * sizeof (lto_ref));
  int32_t type_decls_offset 
    = vars_offset + (header->num_type_decls * sizeof (lto_ref));
  int32_t types_offset 
    = type_decls_offset + (header->num_var_decls * sizeof (lto_ref));
  int32_t named_label_offset 
    = types_offset + (header->num_types * sizeof (lto_ref));
  int32_t ssa_names_offset 
    = named_label_offset + header->named_label_size;
  int32_t cfg_offset 
    = ssa_names_offset + header->ssa_names_size;
  int32_t local_decls_index_offset = cfg_offset + header->cfg_size;
  int32_t local_decls_offset = local_decls_index_offset + header->local_decls_index_size;
  int32_t main_offset = local_decls_offset + header->local_decls_size;
  int32_t string_offset = main_offset + header->main_size;

#ifdef LTO_STREAM_DEBUGGING
  int32_t debug_decl_index_offset = string_offset + header->string_size;
  int32_t debug_decl_offset = debug_decl_index_offset + header->debug_decl_index_size;
  int32_t debug_label_offset = debug_decl_offset + header->debug_decl_size;
  int32_t debug_ssa_names_offset = debug_label_offset + header->debug_label_size;
  int32_t debug_cfg_offset = debug_ssa_names_offset + header->debug_ssa_names_size;
  int32_t debug_main_offset = debug_cfg_offset + header->debug_cfg_size;

  struct input_block debug_decl_index 
    = {data + debug_decl_index_offset, 0, header->debug_decl_index_size};
  struct input_block debug_decl 
    = {data + debug_decl_offset, 0, header->debug_decl_size};
  struct input_block debug_label 
    = {data + debug_label_offset, 0, header->debug_label_size};
  struct input_block debug_ssa_names 
    = {data + debug_ssa_names_offset, 0, header->debug_ssa_names_size};
  struct input_block debug_cfg 
    = {data + debug_cfg_offset, 0, header->debug_cfg_size};
  struct input_block debug_main 
    = {data + debug_main_offset, 0, header->debug_main_size};

  lto_debug_context.out = debug_out_fun;
  lto_debug_context.indent = 0;
#endif

  lto_ref *in_field_decls = (lto_ref*)(data + fields_offset);
  lto_ref *in_fn_decls    = (lto_ref*)(data + fns_offset);
  lto_ref *in_var_decls   = (lto_ref*)(data + vars_offset);
  lto_ref *in_type_decls  = (lto_ref*)(data + type_decls_offset);
  lto_ref *in_types       = (lto_ref*)(data + types_offset);

  struct input_block ib_named_labels 
    = {data + named_label_offset, 0, header->named_label_size};
  struct input_block ib_ssa_names 
    = {data + ssa_names_offset, 0, header->ssa_names_size};
  struct input_block ib_cfg 
    = {data + cfg_offset, 0, header->cfg_size};
  struct input_block ib_local_decls_index 
    = {data + local_decls_index_offset, 0, header->local_decls_index_size};
  struct input_block ib_local_decls 
    = {data + local_decls_offset, 0, header->local_decls_size};
  struct input_block ib_main 
    = {data + main_offset, 0, header->main_size};


  data_in.strings            = data + string_offset;
  data_in.strings_len        = header->string_size;

  lto_static_init_local ();

  /* No upward compatibility here.  */
  gcc_assert (header->major_version == LTO_major_version);
  gcc_assert (header->minor_version == LTO_minor_version);

  input_globals (header, fd, context, &data_in, 
		 in_field_decls, in_fn_decls, 
		 in_var_decls, in_type_decls, in_types);

  if (in_function)
    {
      struct function *fn = DECL_STRUCT_FUNCTION (t);
      cfun = fn;
      data_in.num_named_labels = header->num_named_labels;

#ifdef LTO_STREAM_DEBUGGING
      lto_debug_context.current_data = &debug_label;
#endif
      input_labels (&ib_named_labels, &data_in, 
		    header->num_named_labels, header->num_unnamed_labels);
      
#ifdef LTO_STREAM_DEBUGGING
      lto_debug_context.current_data = &debug_decl_index;
#endif
      input_local_vars_index (&ib_local_decls_index, &data_in, header->num_local_decls);
      
#ifdef LTO_STREAM_DEBUGGING
      lto_debug_context.current_data = &debug_decl;
#endif
      input_local_vars (&ib_local_decls, &data_in, fn, header->num_local_decls);
      
#ifdef LTO_STREAM_DEBUGGING
      lto_debug_context.current_data = &debug_ssa_names;
#endif
      input_ssa_names (&ib_ssa_names, &data_in, fn);
      
#ifdef LTO_STREAM_DEBUGGING
      lto_debug_context.current_data = &debug_cfg;
#endif
      input_cfg (&ib_cfg, fn);

#ifdef LTO_STREAM_DEBUGGING
      lto_debug_context.current_data = &debug_main;
#endif
      /* Set up the struct function.  */
      input_function (t, &data_in, &ib_main);
    }
  else 
    {
#ifdef LTO_STREAM_DEBUGGING
      lto_debug_context.current_data = &debug_main;
#endif
      input_constructor (t, &data_in, &ib_main);
    }

  free (data_in.field_decls);
  free (data_in.fn_decls);
  free (data_in.var_decls);
  free (data_in.type_decls);
  free (data_in.types);
  if (in_function)
    {
      free (data_in.labels);
      free (data_in.local_decls_index);
      free (data_in.local_decls_index_d);
    }
}


/* Read in FN_DECL using DATA.  FD and CONTEXT are magic cookies.  */

void 
lto_read_function_body (lto_info_fd *fd,
			lto_context *context,
			tree fn_decl,
			const void *data)
{
  current_function_decl = fn_decl;
  lto_read_body (fd, context, fn_decl, data, true);
}


/* Read in VAR_DECL using DATA.  FD and CONTEXT are magic cookies.  */

void 
lto_read_var_init (lto_info_fd *fd,
		   lto_context *context,
		   tree var_decl,
		   const void *data)
{
  lto_read_body (fd, context, var_decl, data, false);
}

/* Dump the debug STREAM, and two characters B and C.  */

static void 
dump_debug_stream (struct input_block *stream, char b, char c)
{
  unsigned int i = 0;
  bool new_line = true;
  int chars = 0;
  int hit_pos = -1;
  fprintf (stderr, 
	   "stream failure: looking for a '%c'[0x%x] in the debug stream.\nHowever the data translated into a '%c'[0x%x]at position%d\n\n",
	   c, c, b, b, stream->p);
  
  while (i < stream->len)
    {
      char x;
      
      if (new_line)
	{
	  if (hit_pos >= 0)
	    {
	      int j;
	      fprintf (stderr, "             ");
	      for (j=0; j<hit_pos; j++)
		fputc (' ', stderr);
	      fprintf (stderr, "^\n             ");
	      for (j=0; j<hit_pos; j++)
		fputc (' ', stderr);
	      fprintf (stderr, "|\n");
	      hit_pos = -1;
	    }
	  
	  fprintf (stderr, "%6d   -->>", i);
	  new_line = false;
	  chars = 0;
	}
      
      x = stream->data[i++];
      if (x == '\n')
	{
	  fprintf (stderr, "<<--\n");
	  new_line = true;
	}
      else 
	fputc (x, stderr);
      
      if (i == stream->p)
	hit_pos = chars;
      chars++;
    }
}


#ifdef LTO_STREAM_DEBUGGING

/* The low level output routine to for a single character.  Unlike the
   version on the writing side, this does interesting processing.

   This call checks that the debugging information generated by
   lto-function-out matches the debugging information generated by the
   reader. Each character is checked and a call to abort is generated
   when the first mismatch is found.  
  */

static void
debug_out_fun (struct lto_debug_context *context, char c)
{
  struct input_block *stream = (struct input_block *)context->current_data;
  char b = input_1_unsigned (stream);

  if (b != c)
    {
      dump_debug_stream (stream, b, c);
      gcc_unreachable ();
    }
}
 
#endif
