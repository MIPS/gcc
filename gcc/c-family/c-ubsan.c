/* UndefinedBehaviorSanitizer, undefined behavior detector.
   Copyright (C) 2013 Free Software Foundation, Inc.
   Contributed by Marek Polacek <polacek@redhat.com>

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "alloc-pool.h"
#include "cgraph.h"
#include "gimple.h"
#include "hash-table.h"
#include "output.h"
#include "toplev.h"
#include "c-family/c-common.h"
#include "c-family/c-ubsan.h"

/* This type represents an entry in the hash table.  */
struct ubsan_typedesc
{
  tree type;
  tree decl;
};

static alloc_pool ubsan_typedesc_alloc_pool;

/* Hash table for type descriptors.  */
struct ubsan_typedesc_hasher
  : typed_noop_remove <ubsan_typedesc>
{
  typedef ubsan_typedesc value_type;
  typedef ubsan_typedesc compare_type;

  static inline hashval_t hash (const value_type *);
  static inline bool equal (const value_type *, const compare_type *);
};

/* Hash a memory reference.  */

inline hashval_t
ubsan_typedesc_hasher::hash (const ubsan_typedesc *data)
{
  hashval_t h = iterative_hash_object (data->type, 0);
  h = iterative_hash_object (data->decl, h);
  return h;
}

/* Compare two data types.  */

inline bool
ubsan_typedesc_hasher::equal (const ubsan_typedesc *d1,
			      const ubsan_typedesc *d2)
{
  /* ??? Here, the types should have identical __typekind,
     _typeinfo and __typename.  Is this enough?  */
  return d1->type == d2->type;
}

static hash_table <ubsan_typedesc_hasher> ubsan_typedesc_ht;

/* Initializes an instance of ubsan_typedesc.  */

static void
ubsan_typedesc_init (ubsan_typedesc *data, tree type, tree decl)
{
  data->type = type;
  data->decl = decl;
}

/* This creates the alloc pool used to store the instances of
   ubsan_typedesc that are stored in the hash table ubsan_typedesc_ht.  */

static alloc_pool
ubsan_typedesc_get_alloc_pool ()
{
  if (ubsan_typedesc_alloc_pool == NULL)
    ubsan_typedesc_alloc_pool = create_alloc_pool ("ubsan_typedesc",
						   sizeof (ubsan_typedesc),
						   10);
  // XXX But where do we free this?  We'll need GTY machinery.
  return ubsan_typedesc_alloc_pool;
}

/* Returns a reference to the hash table containing data type.
   This function ensures that the hash table is created.  */

static hash_table <ubsan_typedesc_hasher> &
get_typedesc_hash_table ()
{
  if (!ubsan_typedesc_ht.is_created ())
    ubsan_typedesc_ht.create (10);

  return ubsan_typedesc_ht;
}

/* Allocates memory for an instance of ubsan_typedesc into the memory
   pool returned by ubsan_typedesc_get_alloc_pool and initialize it.
   TYPE describes a particular type, DECL is its VAR_DECL.  */

static ubsan_typedesc *
ubsan_typedesc_new (tree type, tree decl)
{
  ubsan_typedesc *desc =
    (ubsan_typedesc *) pool_alloc (ubsan_typedesc_get_alloc_pool ());

  ubsan_typedesc_init (desc, type, decl);
  return desc;
}

/* Clear all entries from the type descriptor hash table.  */

#if 0
static void
empty_ubsan_typedesc_hash_table ()
{
  // XXX But when do we call this?
  if (ubsan_typedesc_ht.is_created ())
    ubsan_typedesc_ht.empty ();
}
#endif

/* Build the ubsan uptr type.  */

static tree
uptr_type (void)
{
  return build_nonstandard_integer_type (POINTER_SIZE, 1);
}

/* Helper routine, which encodes a value in the uptr type.
   Arguments with precision <= POINTER_SIZE are passed directly,
   the rest is passed by reference.  T is a value we are to encode.  */

static tree
ubsan_encode_value (tree t)
{
  tree type = TREE_TYPE (t);
  switch (TREE_CODE (type))
    {
    case INTEGER_TYPE:
      if (TYPE_PRECISION (type) <= POINTER_SIZE)
	return fold_build1 (NOP_EXPR, uptr_type (), t);
      else
	return build_fold_addr_expr (t);
    case REAL_TYPE:
      {
	unsigned int bitsize = GET_MODE_BITSIZE (TYPE_MODE (type));
	if (bitsize <= POINTER_SIZE)
	  {
	    tree itype = build_nonstandard_integer_type (bitsize, true);
	    t = fold_build1 (VIEW_CONVERT_EXPR, itype, t);
	    return fold_convert (uptr_type (), t);
	  }
	else
	  {
	    if (!TREE_ADDRESSABLE (t))
	      {
		/* The reason for this is that we don't want to pessimize
		   code by making vars unnecessarily addressable.  */
		tree var = create_tmp_var (TREE_TYPE (t), NULL);
		tree tem = build2 (MODIFY_EXPR, void_type_node, var, t);
		t = build_fold_addr_expr (var);
		return build2 (COMPOUND_EXPR, TREE_TYPE (t), tem, t);
	      }
	    else
	      return build_fold_addr_expr (t);
	  }
      }
    default:
      gcc_unreachable ();
    }
}

/* Build
   struct __ubsan_type_descriptor
   {
     unsigned short __typekind;
     unsigned short __typeinfo;
     char __typename[];
   }
   type.  */

static tree
ubsan_type_descriptor_type (void)
{
  static const char *field_names[3]
    = { "__typekind", "__typeinfo", "__typename" };
  tree fields[3], ret;
  tree itype = build_range_type (sizetype, size_zero_node, NULL_TREE);
  tree flex_arr_type = build_array_type (char_type_node, itype);

  ret = make_node (RECORD_TYPE);
  for (int i = 0; i < 3; i++)
    {
      fields[i] = build_decl (UNKNOWN_LOCATION, FIELD_DECL,
			      get_identifier (field_names[i]),
			      (i == 2) ? flex_arr_type
			      : short_unsigned_type_node);
      DECL_CONTEXT (fields[i]) = ret;
      if (i)
	DECL_CHAIN (fields[i - 1]) = fields[i];
    }
  TYPE_FIELDS (ret) = fields[0];
  TYPE_NAME (ret) = get_identifier ("__ubsan_type_descriptor");
  layout_type (ret);
  return ret;
}

/* Build
   struct __ubsan_source_location
   {
     const char *__filename;
     unsigned int __line;
     unsigned int __column;
   }
   type.  */

static tree
ubsan_source_location_type (void)
{
  static const char *field_names[3]
    = { "__filename", "__line", "__column" };
  tree fields[3], ret;

  ret = make_node (RECORD_TYPE);
  for (int i = 0; i < 3; i++)
    {
      fields[i] = build_decl (UNKNOWN_LOCATION, FIELD_DECL,
			      get_identifier (field_names[i]),
			      (i == 0) ? const_string_type_node
			      : unsigned_type_node);
      DECL_CONTEXT (fields[i]) = ret;
      if (i)
	DECL_CHAIN (fields[i - 1]) = fields[i];
    }
  TYPE_FIELDS (ret) = fields[0];
  TYPE_NAME (ret) = get_identifier ("__ubsan_source_location");
  layout_type (ret);
  return ret;
}

/* Helper routine that returns a CONSTRUCTOR of __ubsan_source_location
   type with its fields filled from a location_t LOC.  */

static tree
ubsan_source_location (location_t loc)
{
  expanded_location xloc;
  tree type = ubsan_source_location_type ();
  vec<constructor_elt, va_gc> *v;

  xloc = expand_location (loc);

  /* Fill in the values from LOC.  */
  vec_alloc (v, 3);
  tree ctor = build_constructor (type, v);
  size_t len = strlen (xloc.file);
  tree str = build_string (len + 1, xloc.file);
  TREE_TYPE (str) = build_array_type (char_type_node,
				      build_index_type (size_int (len)));
  TREE_READONLY (str) = 1;
  TREE_STATIC (str) = 1;
  str = build_fold_addr_expr_loc (loc, str);
  CONSTRUCTOR_APPEND_ELT (v, NULL_TREE, str);
  CONSTRUCTOR_APPEND_ELT (v, NULL_TREE, build_int_cst (unsigned_type_node,
						       xloc.line));
  CONSTRUCTOR_APPEND_ELT (v, NULL_TREE, build_int_cst (unsigned_type_node,
						       xloc.column));
  TREE_CONSTANT (ctor) = 1;
  TREE_STATIC (ctor) = 1;

  return ctor;
}

/* This routine returns a magic number for TYPE.
   ??? This is probably too ugly.  Tweak it.  */

static unsigned short
get_tinfo_for_type (tree type)
{
  unsigned short tinfo;

  switch (GET_MODE_SIZE (TYPE_MODE (type)))
    {
    case 4:
      tinfo = 5;
      break;
    case 8:
      tinfo = 6;
      break;
    case 16:
      tinfo = 7;
      break;
    default:
      error ("unexpected size of type %qT", type);
    }

  tinfo <<= 1;

  /* The MSB here says whether the value is signed or not.  */
  tinfo |= !TYPE_UNSIGNED (type);
  return tinfo;
}

/* Helper routine that returns ADDR_EXPR of a VAR_DECL of a type
   descriptor.  It first looks into the hash table; if not found,
   create the VAR_DECL, put it into the hash table and return the
   ADDR_EXPR of it.  TYPE describes a particular type.  */

static tree
ubsan_type_descriptor (tree type)
{
  hash_table <ubsan_typedesc_hasher> ht = get_typedesc_hash_table ();
  ubsan_typedesc d;
  ubsan_typedesc_init (&d, type, NULL);

  ubsan_typedesc **slot = ht.find_slot (&d, INSERT);
  if (*slot != NULL)
    /* We have the VAR_DECL in the table.  Return it.  */
    return (*slot)->decl;

  tree dtype = ubsan_type_descriptor_type ();
  vec<constructor_elt, va_gc> *v;
  const char *tname;
  unsigned short tkind, tinfo;

  /* At least for INTEGER_TYPE/REAL_TYPE/COMPLEX_TYPE, this should work.
     ??? For e.g. type_unsigned_for (type), the TYPE_NAME would be NULL.  */
  if (TYPE_NAME (type) != NULL)
    tname = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type)));
  else
    tname = "<unknown>";
  if (TREE_CODE (type) == INTEGER_TYPE)
    {
      /* For INTEGER_TYPE, this is 0x0000.  */
      tkind = 0x000;
      tinfo = get_tinfo_for_type (type);
    }
  else if (TREE_CODE (type) == REAL_TYPE)
    /* We don't have float support yet.  */
    gcc_unreachable ();
  else
    gcc_unreachable ();

  /* Create a new VAR_DECL of type descriptor.  */
  char *tmp_name;
  static unsigned int type_var_id_num;
  ASM_FORMAT_PRIVATE_NAME (tmp_name, ".Lubsan_type", type_var_id_num++);
  tree decl = build_decl (UNKNOWN_LOCATION, VAR_DECL, get_identifier (tmp_name),
			  dtype);
  TREE_STATIC (decl) = 1;
  TREE_PUBLIC (decl) = 0;
  DECL_ARTIFICIAL (decl) = 1;
  DECL_IGNORED_P (decl) = 1;
  DECL_EXTERNAL (decl) = 0;

  vec_alloc (v, 3);
  tree ctor = build_constructor (dtype, v);
  size_t len = strlen (tname);
  tree str = build_string (len + 1, tname);
  TREE_TYPE (str) = build_array_type (char_type_node,
				      build_index_type (size_int (len)));
  TREE_READONLY (str) = 1;
  TREE_STATIC (str) = 1;
  CONSTRUCTOR_APPEND_ELT (v, NULL_TREE, build_int_cst (short_unsigned_type_node,
						       tkind));
  CONSTRUCTOR_APPEND_ELT (v, NULL_TREE, build_int_cst (short_unsigned_type_node,
						       tinfo));
  CONSTRUCTOR_APPEND_ELT (v, NULL_TREE, str);

  TREE_CONSTANT (ctor) = 1;
  TREE_STATIC (ctor) = 1;
  DECL_INITIAL (decl) = ctor;
  rest_of_decl_compilation (decl, 1, 0);

  /* Save the address of the VAR_DECL into the hash table.  */
  decl = build_fold_addr_expr (decl);
  *slot = ubsan_typedesc_new (type, decl);

  return decl;
}

/* Create a structure for the ubsan library.  NAME is a name of the new
   structure.  The arguments in ... are of __ubsan_type_descriptor type
   and there are at most two of them.  */

static tree
ubsan_create_data (const char *name, location_t loc, ...)
{
  va_list args;
  tree ret, t;
  tree fields[3];
  vec<tree, va_gc> *saved_args = NULL;
  size_t i = 0;

  /* Firstly, create a pointer to type descriptor type.  */
  tree td_type = ubsan_type_descriptor_type ();
  TYPE_READONLY (td_type) = 1;
  td_type = build_pointer_type (td_type);

  /* Create the structure type.  */
  ret = make_node (RECORD_TYPE);
  if (loc != UNKNOWN_LOCATION)
    {
      fields[i] = build_decl (UNKNOWN_LOCATION, FIELD_DECL, NULL_TREE,
			      ubsan_source_location_type ());
      DECL_CONTEXT (fields[i]) = ret;
      i++;
    }

  va_start (args, loc);
  for (t = va_arg (args, tree); t != NULL_TREE;
       i++, t = va_arg (args, tree))
    {
      gcc_checking_assert (i < 3);
      /* Save the tree argument for later use.  */
      vec_safe_push (saved_args, t);
      fields[i] = build_decl (UNKNOWN_LOCATION, FIELD_DECL, NULL_TREE,
			      td_type);
      DECL_CONTEXT (fields[i]) = ret;
      if (i)
	DECL_CHAIN (fields[i - 1]) = fields[i];
    }
  TYPE_FIELDS (ret) = fields[0];
  TYPE_NAME (ret) = get_identifier (name);
  layout_type (ret);
  va_end (args);

  /* Now, fill in the type.  */
  char *tmp_name;
  static unsigned int ubsan_var_id_num;
  ASM_FORMAT_PRIVATE_NAME (tmp_name, ".Lubsan_data", ubsan_var_id_num++);
  tree var = build_decl (UNKNOWN_LOCATION, VAR_DECL, get_identifier (tmp_name),
			 ret);
  TREE_STATIC (var) = 1;
  TREE_PUBLIC (var) = 0;
  DECL_ARTIFICIAL (var) = 1;
  DECL_IGNORED_P (var) = 1;
  DECL_EXTERNAL (var) = 0;

  vec<constructor_elt, va_gc> *v;
  vec_alloc (v, i);
  tree ctor = build_constructor (ret, v);

  /* If desirable, set the __ubsan_source_location element.  */
  if (loc != UNKNOWN_LOCATION)
    CONSTRUCTOR_APPEND_ELT (v, NULL_TREE, ubsan_source_location (loc));

  size_t nelts = vec_safe_length (saved_args);
  for (i = 0; i < nelts; i++)
    {
      t = (*saved_args)[i];
      CONSTRUCTOR_APPEND_ELT (v, NULL_TREE, t);
    }

  TREE_CONSTANT (ctor) = 1;
  TREE_STATIC (ctor) = 1;
  DECL_INITIAL (var) = ctor;
  rest_of_decl_compilation (var, 1, 0);

  return var;
}

/* Instrument division by zero and INT_MIN / -1.  If not instrumenting,
   return NULL_TREE.  */

tree
ubsan_instrument_division (location_t loc, tree op0, tree op1)
{
  tree t, tt;
  tree type = TREE_TYPE (op0);

  /* At this point both operands should have the same type,
     because they are already converted to RESULT_TYPE.  */
  gcc_assert (type == TREE_TYPE (op1));

  /* TODO: REAL_TYPE is not supported yet.  */
  if (TREE_CODE (type) != INTEGER_TYPE)
    return NULL_TREE;

  /* If we *know* that the divisor is not -1 or 0, we don't have to
     instrument this expression.
     ??? We could use decl_constant_value to cover up more cases.  */
  if (TREE_CODE (op1) == INTEGER_CST
      && integer_nonzerop (op1)
      && !integer_minus_onep (op1))
    return NULL_TREE;

  t = fold_build2 (EQ_EXPR, boolean_type_node,
		    op1, build_int_cst (type, 0));

  /* We check INT_MIN / -1 only for signed types.  */
  if (!TYPE_UNSIGNED (type))
    {
      tree x;
      tt = fold_build2 (EQ_EXPR, boolean_type_node, op1,
			build_int_cst (type, -1));
      x = fold_build2 (EQ_EXPR, boolean_type_node, op0,
		       TYPE_MIN_VALUE (type));
      x = fold_build2 (TRUTH_AND_EXPR, boolean_type_node, x, tt);
      t = fold_build2 (TRUTH_OR_EXPR, boolean_type_node, t, x);
    }
  tree data = ubsan_create_data ("__ubsan_overflow_data",
				 loc, ubsan_type_descriptor (type),
				 NULL_TREE);
  data = build_fold_addr_expr_loc (loc, data);
  tt = builtin_decl_explicit (BUILT_IN_UBSAN_HANDLE_DIVREM_OVERFLOW);
  tt = build_call_expr_loc (loc, tt, 3, data, ubsan_encode_value (op0),
			    ubsan_encode_value (op1));
  t = fold_build3 (COND_EXPR, void_type_node, t, tt, void_zero_node);

  return t;
}

/* Instrument left and right shifts.  If not instrumenting, return
   NULL_TREE.  */

tree
ubsan_instrument_shift (location_t loc, enum tree_code code,
			tree op0, tree op1)
{
  tree t, tt = NULL_TREE;
  tree type0 = TREE_TYPE (op0);
  tree type1 = TREE_TYPE (op1);
  tree op1_utype = unsigned_type_for (type1);
  HOST_WIDE_INT op0_prec = TYPE_PRECISION (type0);
  tree uprecm1 = build_int_cst (op1_utype, op0_prec - 1);
  tree precm1 = build_int_cst (type1, op0_prec - 1);

  t = fold_convert_loc (loc, op1_utype, op1);
  t = fold_build2 (GT_EXPR, boolean_type_node, t, uprecm1);

  /* For signed x << y, in C99/C11, the following:
     (unsigned) x >> (precm1 - y)
     if non-zero, is undefined.  */
  if (code == LSHIFT_EXPR
      && !TYPE_UNSIGNED (type0)
      && flag_isoc99)
    {
      tree x = fold_build2 (MINUS_EXPR, integer_type_node, precm1, op1);
      tt = fold_convert_loc (loc, unsigned_type_for (type0), op0);
      tt = fold_build2 (RSHIFT_EXPR, TREE_TYPE (tt), tt, x);
      tt = fold_build2 (NE_EXPR, boolean_type_node, tt,
			build_int_cst (TREE_TYPE (tt), 0));
    }

  /* For signed x << y, in C++11/C++14, the following:
     x < 0 || ((unsigned) x >> (precm1 - y))
     if > 1, is undefined.  */
  if (code == LSHIFT_EXPR
      && !TYPE_UNSIGNED (TREE_TYPE (op0))
      && (cxx_dialect == cxx11 || cxx_dialect == cxx1y))
    {
      tree x = fold_build2 (MINUS_EXPR, integer_type_node, precm1, op1);
      tt = fold_convert_loc (loc, unsigned_type_for (type0), op0);
      tt = fold_build2 (RSHIFT_EXPR, TREE_TYPE (tt), tt, x);
      tt = fold_build2 (GT_EXPR, boolean_type_node, tt,
			build_int_cst (TREE_TYPE (tt), 1));
      x = fold_build2 (LT_EXPR, boolean_type_node, op0,
		       build_int_cst (type0, 0));
      tt = fold_build2 (TRUTH_OR_EXPR, boolean_type_node, x, tt);
    }
  tree data = ubsan_create_data ("__ubsan_shift_data",
				 loc, ubsan_type_descriptor (type0),
				 ubsan_type_descriptor (type1), NULL_TREE);

  data = build_fold_addr_expr_loc (loc, data);

  t = fold_build2 (TRUTH_OR_EXPR, boolean_type_node, t,
		   tt ? tt : integer_zero_node);
  tt = builtin_decl_explicit (BUILT_IN_UBSAN_HANDLE_SHIFT_OUT_OF_BOUNDS);
  tt = build_call_expr_loc (loc, tt, 3, data, ubsan_encode_value (op0),
			    ubsan_encode_value (op1));
  t = fold_build3 (COND_EXPR, void_type_node, t, tt, void_zero_node);

  return t;
}
