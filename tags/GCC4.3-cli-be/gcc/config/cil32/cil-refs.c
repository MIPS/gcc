/* Database of entities referenced in a compilation unit implementation.

   Copyright (C) 2006-2009 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Authors:
   Andrea Ornstein
   Erven Rohou
   Gabriele Svelto

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "ggc.h"
#include "debug.h"
#include "errors.h"
#include "toplev.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-gimple.h"
#include "tree-pass.h"
#include "function.h"
#include "hashtab.h"
#include "vec.h"
#include "cil-builtins.h"
#include "cil-types.h"
#include "cil-refs.h"

/******************************************************************************
 * Local function prototypes                                                  *
 ******************************************************************************/

static hashval_t assembly_hash (const void *);
static int assembly_eq (const void *, const void *);

static char *append_string (char *, const char *, size_t *, size_t *);
static char *append_coded_type (char *, tree, size_t *, size_t *);
static char *get_compact_identifier (const char *, size_t, size_t *);
static tree make_valuetype_identifier (tree);
static hashval_t ref_type_hash (const void *);
static int ref_type_eq (const void *, const void *);

static hashval_t str_ref_hash (const void *);
static int str_ref_eq (const void *, const void *);

static hashval_t pinvoke_hash (const void *);
static int pinvoke_eq (const void *, const void *);

static hashval_t label_addr_hash (const void *);
static int label_addr_eq (const void *, const void *);
static int fill_label_addrs (void **, void *);

static bool mostly_zeros_p (tree);
static bool all_zeros_p (tree);
static void expand_init_to_stmt_list1 (tree, tree, tree *, bool, tree *,
                                       void *, void *, unsigned HOST_WIDE_INT);
static int statement_list_num_instr (tree);

/******************************************************************************
 * Globals                                                                    *
 ******************************************************************************/

static GTY((param_is (union tree_node))) htab_t ref_assemblies = NULL;
static GTY((param_is (union tree_node))) htab_t pending_assemblies = NULL;
static GTY((param_is (union tree_node))) htab_t ref_types = NULL;
static GTY((param_is (struct str_ref_d))) htab_t ref_strings = NULL;
static unsigned int string_id = 0;
static GTY((param_is (union tree_node))) htab_t ref_pinvokes = NULL;
static GTY((param_is (struct label_addr_d))) htab_t labels_map = NULL;
static GTY(()) VEC(tree, gc) *pending_ctors = NULL;

/******************************************************************************
 * Initialization and teardown                                                *
 ******************************************************************************/

/* Initialize the database of referenced entities.  */

void
refs_init (void)
{
  if (ref_types == NULL)
    {
      ref_assemblies = htab_create_ggc (4, assembly_hash, assembly_eq, NULL);
      pending_assemblies = htab_create_ggc (4, assembly_hash, assembly_eq,
					    NULL);
      ref_types = htab_create_ggc (32, ref_type_hash, ref_type_eq, NULL);
      ref_strings = htab_create_ggc (32, str_ref_hash, str_ref_eq, NULL);
      labels_map = htab_create_ggc (32, label_addr_hash, label_addr_eq, NULL);
      ref_pinvokes = htab_create_ggc (32, pinvoke_hash, pinvoke_eq, NULL);
    }
}

/* Tears down the database of referenced entities. */

void refs_fini (void)
{
  ref_types = NULL;
  ref_pinvokes = NULL;
  ref_strings = NULL;
  labels_map = NULL;
}

/******************************************************************************
 * Referenced assemblies                                                      *
 ******************************************************************************/

/* Hash function for referenced assemblies.  */

static hashval_t assembly_hash (const void *ptr)
{
  const_tree assembly = (const_tree) ptr;
  const char *str = TREE_STRING_POINTER (assembly);
  hashval_t hash = 0;
  size_t len = TREE_STRING_LENGTH (assembly);
  size_t i;

  for (i = 0; i < len; i++)
    {
      hash += str[i];
      hash += (hash << 10);
      hash ^= (hash >> 6);
    }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

/* Equality function for referenced assemblies.  */

static int assembly_eq (const void *ptr1, const void *ptr2)
{
  const char *str1 = TREE_STRING_POINTER ((const_tree) ptr1);
  const char *str2 = TREE_STRING_POINTER ((const_tree) ptr2);

  return str1 == str2;
}

/* Add a referenced assembly to the list of pending assemblies if it is not
   already present among the referenced assemblies.  NAME is the name of the
   referenced assembly.  */

void add_referenced_assembly (const char *name)
{
  void **slot;
  tree str = build_string (strlen (name), name);

  slot = htab_find_slot (ref_assemblies, str, NO_INSERT);

  if (slot != NULL)
    return; /* We have already emitted this assembly's name.  */

  /* Add the assembly to the pending list if it has not been added yet.  */
  slot = htab_find_slot (pending_assemblies, str, INSERT);

  if (*slot == NULL)
    *slot = str;
}

/* Return a pointer to the hash-table holding the referenced assemblies which
   have not been emitted yet.  */

htab_t pending_assemblies_htab ( void )
{
  return pending_assemblies;
}

/* Move the pending assemblies to the referenced assemblies hash table.  */

void mark_pending_assemblies ( void )
{
  htab_iterator hti;
  void **slot;
  tree name;

  FOR_EACH_HTAB_ELEMENT (pending_assemblies, name, tree, hti)
    {
      slot = htab_find_slot (ref_assemblies, name, INSERT);
      *slot = name;
    }

  htab_empty (pending_assemblies);
}

/******************************************************************************
 * Referenced types                                                           *
 ******************************************************************************/

/* Warning: these strings are not null-terminated */
static char *
append_string (char *str, const char *to_append, size_t *len,
               size_t *max_len)
{
  size_t i, orig_len = *len;
  size_t append_len = strlen (to_append);

  *len += append_len;

  if (*len > *max_len)
    {
      while (*len > *max_len)
        *max_len *= 2;

      str = XRESIZEVEC (char, str, *max_len);
    }

  for (i = 0; i < append_len; i++)
    str[orig_len + i] = to_append[i];

  return str;
}

/* Warning: these strings are not null-terminated */
static char *
append_coded_type (char *str, tree type,
                   size_t *len, size_t *max_len)
{
  unsigned HOST_WIDE_INT size;
  enum tree_code type_code;

  if (type == NULL_TREE || type == error_mark_node)
    return str;

  type = TYPE_MAIN_VARIANT (type);
  type_code = TREE_CODE (type);

restartswitch:
  switch (type_code)
    {
    case VOID_TYPE:
      str = append_string (str, "VOID", len, max_len);
      break;

    case INTEGER_TYPE:
      {
	char tmp_str[8] = "UI";
	char *tmp_str_ptr = tmp_str;

	size = tree_low_cst (TYPE_SIZE (type), 1);
	snprintf (tmp_str_ptr + 2, 6, HOST_WIDE_INT_PRINT_UNSIGNED, size);

	if (!TYPE_UNSIGNED (type))
	  tmp_str_ptr++;

	str = append_string (str, tmp_str_ptr, len, max_len);
      }
      break;

    case BOOLEAN_TYPE:
      str = append_string (str, "B", len, max_len);
      break;

    case REAL_TYPE:
      {
	char tmp_str[4] = "F";

	size = tree_low_cst (TYPE_SIZE (type), 1);
	snprintf (tmp_str + 1, 3, HOST_WIDE_INT_PRINT_UNSIGNED, size);
	str = append_string (str, tmp_str, len, max_len);
      }
      break;

    case POINTER_TYPE:
      if (TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
	{
	  tree fun_type = TREE_TYPE (type);
	  tree args_type = TYPE_ARG_TYPES (fun_type);
/*           bool varargs = TREE_VALUE (tree_last (args_type)) != void_type_node; */
	  unsigned int nargs_base = list_length (args_type);
	  unsigned int i;
	  char tmp_str[5] = "FP";

	  snprintf (tmp_str + 2, 3, "%d", nargs_base);
	  str = append_string (str, tmp_str, len, max_len);
	  str = append_string (str, "_", len, max_len);
	  str = append_coded_type (str, TREE_TYPE (fun_type), len, max_len);
	  str = append_string (str, "_", len, max_len);

	  for (i = 0; i < nargs_base; i++)
	    {
	      str = append_string (str, "_", len, max_len);
	      str = append_coded_type (str, TREE_VALUE (args_type), len, max_len);
	      args_type = TREE_CHAIN (args_type);
	    }
	}
      else
	{
	  str = append_string (str, "*", len, max_len);
	  type = TYPE_MAIN_VARIANT (TREE_TYPE (type));
	  type_code = TREE_CODE (type);
	  goto restartswitch;
	}
      break;

    case ARRAY_TYPE:
      if (TYPE_DOMAIN (type) && !ARRAY_TYPE_VARLENGTH (type))
	{
	  tree domain = TYPE_DOMAIN (type);
	  tree min = TYPE_MIN_VALUE (domain);
	  tree max = TYPE_MAX_VALUE (domain);

	  str = append_string (str, "[", len, max_len);

	  if (ARRAY_TYPE_ZEROLENGTH (type))
	    str = append_string (str, "0", len, max_len);
	  else if (min && max && integer_zerop (min) && host_integerp (max, 0))
	    {
	      char tmp_str[32];

	      size = tree_low_cst (max, 1) + 1;
	      snprintf (tmp_str, 32, HOST_WIDE_INT_PRINT_UNSIGNED, size);
	      str = append_string (str, tmp_str, len, max_len);
	    }
	  else
	    str = append_string (str, "unk", len, max_len);

	  str = append_string (str, "]", len, max_len);
	}
      else
	{
	  type_code = POINTER_TYPE;
	  goto restartswitch;
	}

      type = TYPE_MAIN_VARIANT (TREE_TYPE (type));
      type_code = TREE_CODE (type);
      goto restartswitch;

    case ENUMERAL_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      {
	const char *prefix;
	const char *type_str;
	tree type_name;

	/* Give the aggregate a name unless it has it already */
	if (TYPE_NAME (type) == 0)
	  {
	    tree type_decl = build0 (TYPE_DECL, type);
	    DECL_NAME (type_decl) = make_valuetype_identifier (type);
	    TYPE_NAME (type) = type_decl;
	  }

	type_name = TYPE_NAME (type);

	if (TREE_CODE (type_name) == IDENTIFIER_NODE)
	  type_str = IDENTIFIER_POINTER (type_name);
	else
	  type_str = IDENTIFIER_POINTER (DECL_NAME (type_name));

	switch (type_code)
	  {
	  case ENUMERAL_TYPE:
	    prefix = "E";
	    break;

	  case RECORD_TYPE:
	    prefix = "S";
	    break;

	  case UNION_TYPE:
	  case QUAL_UNION_TYPE:
	    prefix = "UN";
	    break;

	  default:
	    gcc_unreachable ();
	  }

	str = append_string (str, prefix, len, max_len);
	str = append_string (str, type_str, len, max_len);
      }
      break;

    default:
/*       debug_tree(type); */
/*       gcc_assert (0); */
      str = append_string (str, "unknown", len, max_len);
    }

  return str;
}

/* Compute and return a compact identifier from identifier STR of size LEN.
   Memory is allocated for the compact identifier.
   Store the length of the compact identifier in COMPACT_LEN.   */

static char *
get_compact_identifier (const char *str, size_t len, size_t *compact_len)
{
  char *compact_str;
  size_t i;
  unsigned char buffer[COMPACT_ID_LENGTH / 2];

  gcc_assert (COMPACT_ID_LENGTH % 2 == 0);

  /* If the string is shorter than the length of compact strings,
     then return it unchanged.   */
  if (len <= COMPACT_ID_LENGTH)
    {
      compact_str = XNEWVEC (char, len);
      memcpy (compact_str, str, len);

      *compact_len = len;
      return compact_str;
    }

  /* Fill the buffer */
  memset (buffer, 0, COMPACT_ID_LENGTH / 2);

  for (i = 0; i < len; i++)
    {
      int j = 0;
      unsigned char c = str[i];

      while (true)
	{
	  unsigned char tmp_c;

	  /* Modify a position in buffer */
	  buffer[(i + j) % (COMPACT_ID_LENGTH / 2)] ^= c;

	  if (j == COMPACT_ID_LENGTH / 2)
	    break;

	  /* Rotate c 1-bit right */
	  tmp_c = c >> 1;
	  tmp_c |= (c & 1) << 7;
	  c = tmp_c;

	  j++;
	}
    }

  /* Build the compact string */
  compact_str = XNEWVEC (char, COMPACT_ID_LENGTH);

  for (i = 0; i < COMPACT_ID_LENGTH / 2; i++)
    {
      unsigned char c1, c2;

      c1 = buffer[i] & 0xf;
      c2 = buffer[i] >> 4;

      compact_str[i * 2]     = c1 + ((c1 < 10) ? '0' : 'a' - 10);
      compact_str[i * 2 + 1] = c2 + ((c2 < 10) ? '0' : 'a' - 10);
    }

  /* Return the compact string and its length */
  *compact_len = COMPACT_ID_LENGTH;
  return compact_str;
}

static tree
make_valuetype_identifier (tree t)
{
  size_t tmp_name_max_len = 256;
  size_t tmp_name_len = 0;
  char *tmp_name;
  size_t vt_name_len = 0;
  char *vt_name;
  tree ident;

  tmp_name = XNEWVEC (char, tmp_name_max_len);

  if (TREE_CODE (t) == ENUMERAL_TYPE)
    {
      tree tmp;

      tmp_name = append_string (tmp_name, "enum?",
				&tmp_name_len, &tmp_name_max_len);
      tmp = TYPE_VALUES (t);

      while (tmp)
	{
	  tmp_name = append_string (tmp_name,
				    IDENTIFIER_POINTER (TREE_PURPOSE (tmp)),
				    &tmp_name_len, &tmp_name_max_len);
	  tmp_name = append_string (tmp_name, "?",
				    &tmp_name_len, &tmp_name_max_len);
	  tmp = TREE_CHAIN (tmp);
	}
    }
  else if (TREE_CODE (t) == ARRAY_TYPE)
    {
      gcc_assert (TYPE_DOMAIN (t) && !ARRAY_TYPE_VARLENGTH (t));
      tmp_name = append_string (tmp_name, "array?",
				&tmp_name_len, &tmp_name_max_len);
      tmp_name = append_coded_type (tmp_name, t,
				    &tmp_name_len, &tmp_name_max_len);
    }
  else
    {
      tree tmp;

      if (TREE_CODE (t) == RECORD_TYPE)
	tmp_name = append_string (tmp_name, "struct?",
				  &tmp_name_len, &tmp_name_max_len);
      else
	tmp_name = append_string (tmp_name, "union?",
				  &tmp_name_len, &tmp_name_max_len);

      tmp = TYPE_FIELDS (t);
      while (tmp)
	{
	  tree ttype = TREE_TYPE (tmp);

	  tmp_name = append_coded_type (tmp_name, ttype,
					&tmp_name_len, &tmp_name_max_len);
	  tmp_name = append_string (tmp_name, "?",
				    &tmp_name_len, &tmp_name_max_len);
	  if (DECL_NAME (tmp) != NULL_TREE)
	    {
	      tmp_name = append_string (tmp_name,
					IDENTIFIER_POINTER (DECL_NAME (tmp)),
					&tmp_name_len, &tmp_name_max_len);
	    }
          else
	    {
	      /* Unnamed bit-fields or unions */
	      tmp_name = append_string (tmp_name, "?unnamed",
					&tmp_name_len, &tmp_name_max_len);
	    }

	  tmp_name = append_string (tmp_name, "?",
				    &tmp_name_len, &tmp_name_max_len);
	  tmp = TREE_CHAIN (tmp);
	}
    }

  vt_name = get_compact_identifier (tmp_name, tmp_name_len, &vt_name_len);
  XDELETEVEC (tmp_name);

  ident = get_identifier_with_length (vt_name, vt_name_len);
  XDELETEVEC (vt_name);

  return ident;
}

/* Hash function for the referenced types */

static hashval_t
ref_type_hash (const void *ptr)
{
  const_tree type = (const_tree) ptr;
  return (hashval_t) TYPE_UID (type);
}

/* Equality function for the referenced types */

static int
ref_type_eq (const void *ptr1, const void *ptr2)
{
  const_tree type1 = (const_tree) ptr1;
  const_tree type2 = (const_tree) ptr2;

  return TYPE_UID (type1) == TYPE_UID (type2);
}

/* Mark the type represented by tree T as referenced.
   This function works recursively, since types referenced by type T
   itself are also marked as referenced.
   Referenced types are emitted at the end of the compilation unit,
   non-referenced types are not.
   T must be a type node.   */

void
mark_referenced_type (tree t)
{
  void **slot;

  if (t == NULL_TREE || t == error_mark_node)
    return;

  t = TYPE_MAIN_VARIANT (t);

  if (cil_builtin_type_p (t))
    return;

  /* If the type was already referenced, nothing else to do */
  slot = htab_find_slot (ref_types, t, INSERT);

  if (*slot != NULL)
    return;

  /* Give the aggregate a name unless it has it already */
  switch (TREE_CODE (t))
    {
    /* Incomplete and variable-length arrays are pointers and
       they must be dealt with as such.   */
    case ARRAY_TYPE:
      if (!TYPE_DOMAIN (t) || ARRAY_TYPE_VARLENGTH (t))
	break;

    case ENUMERAL_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      if (TYPE_NAME (t) == NULL_TREE)
	{
	  tree type_decl = build0 (TYPE_DECL, t);
	  DECL_NAME (type_decl) = make_valuetype_identifier (t);
	  TYPE_NAME (t) = type_decl;
	}
      break;

    default:
      /* Nothing to do for the other types */
      ;
    }

  /* Transform local-scope types into global-scope types */
  if (!TYPE_FILE_SCOPE_P (t))
    {
      tree type_name = TYPE_NAME (t);
      const char *orig_name;
      size_t tmp_name_max_len = 256;
      size_t tmp_name_len = 0;
      char *tmp_name;
      char suffix[32];

      gcc_assert (type_name != 0);
      gcc_assert (DECL_P (type_name)
                  || TREE_CODE (type_name) == IDENTIFIER_NODE);

      if (TREE_CODE (type_name) == IDENTIFIER_NODE)
	orig_name = IDENTIFIER_POINTER (type_name);
      else
	orig_name = IDENTIFIER_POINTER (DECL_NAME (type_name));

      snprintf (suffix, 15, "?vt%u", TYPE_UID (t));

      tmp_name = XNEWVEC (char, tmp_name_max_len);
      tmp_name = append_string (tmp_name, orig_name,
				&tmp_name_len, &tmp_name_max_len);
      tmp_name = append_string (tmp_name, suffix,
				&tmp_name_len, &tmp_name_max_len);

      TYPE_NAME (t) = get_identifier_with_length (tmp_name, tmp_name_len);
      TYPE_CONTEXT (t) = 0;
      XDELETEVEC (tmp_name);
    }

  switch (TREE_CODE (t))
    {
    case ENUMERAL_TYPE:
      *slot = t;
      break;

    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      {
	tree tmp;

	*slot = t;
	tmp = TYPE_FIELDS (t);

	while (tmp)
	  {
	    if (DECL_BIT_FIELD (tmp))
	      mark_referenced_type (DECL_BIT_FIELD_TYPE (tmp));
	    else
	      mark_referenced_type (TREE_TYPE (tmp));

	    tmp = TREE_CHAIN (tmp);
          }
      }
      break;

    case POINTER_TYPE:
    case REFERENCE_TYPE:
      mark_referenced_type (TREE_TYPE (t));
      break;

    case ARRAY_TYPE:
      if (TYPE_DOMAIN (t) && !ARRAY_TYPE_VARLENGTH (t))
	*slot = t;

      mark_referenced_type (TREE_TYPE (t));
      break;

    case FUNCTION_TYPE:
      {
	tree args_type;

	mark_referenced_type (TREE_TYPE (t));
	args_type = TYPE_ARG_TYPES (t);

	while (args_type)
	  {
	    mark_referenced_type (TREE_VALUE (args_type));
	    args_type = TREE_CHAIN (args_type);
	  }
      }
      break;

    default:
      /* Nothing to do for the other types */
      ;
    }
}

/* Return the hash table holding the referenced types.  */

htab_t
referenced_types_htab ( void )
{
  return ref_types;
}

/* Promote the type TYPE following the C conventions for variable argument
   calls.  */

tree
promote_type_for_vararg (tree type)
{
  unsigned HOST_WIDE_INT size;

  if (type == NULL_TREE || type == error_mark_node)
    return type;

  switch (TREE_CODE (type))
    {
    /* Incomplete and variable-length arrays are pointers and
       they must be dealt with as such.   */
    case ARRAY_TYPE:
      if (!TYPE_DOMAIN (type) || ARRAY_TYPE_VARLENGTH (type))
	goto pointer;

    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      return TYPE_MAIN_VARIANT (type);

    case COMPLEX_TYPE:
      return type;

    case ENUMERAL_TYPE:
    case INTEGER_TYPE:
    case BOOLEAN_TYPE:
      size = tree_low_cst (TYPE_SIZE (type), 1);
      gcc_assert (size <= 64);
      return (size <= 32) ? unsigned_intSI_type_node : unsigned_intDI_type_node;

    case REAL_TYPE:
      return double_type_node;

pointer:
    case POINTER_TYPE:
      /* FIXME: cil32 is a 32-bit machine, in case we support 64-bit model
         changes are needed.  */
      return unsigned_intSI_type_node;

    default:
      internal_error ("%s: %s\n", __func__, tree_code_name[TREE_CODE (type)]);
    }
}

/* Return the promoted type of the local variable represented by VAR.  In most
   cases this will be simply the variable type, however some temporaries may
   have been declared with integral types which do not exist in CIL
   (e.g. 3-bit integers). The types of those variables are automatically
   promoted to a larger stack type since those variables will end up
   only on the stack.  */

tree
promote_local_var_type (tree var)
{
  unsigned int precision;
  unsigned int unsignedp;
  tree type = TREE_TYPE (var);

  if (INTEGRAL_TYPE_P (type) && DECL_ARTIFICIAL (var))
    {
      gcc_assert (!DECL_FILE_SCOPE_P (var) && !TREE_STATIC (var));

      precision = TYPE_PRECISION (type);
      unsignedp = TYPE_UNSIGNED (type);

      switch (precision)
	{
	case 8:  /* FALLTHROUGH */
	case 16: /* FALLTHROUGH */
	case 32: /* FALLTHROUGH */
	case 64: return type;
	default:
	  if (precision < 32)
	    return unsignedp ? unsigned_intSI_type_node : intSI_type_node;
	  else
	    return unsignedp ? unsigned_intDI_type_node : intDI_type_node;
	}
    }
  else
    return type;
}

/* Return true if we treat the type TYPE as a pointer, false otherwise.  */

bool
cil_pointer_type_p (tree type)
{
  if (TREE_CODE (type) == ARRAY_TYPE)
    {
      if (!TYPE_DOMAIN (type) || ARRAY_TYPE_VARLENGTH (type))
	return true;
    }
  else if (POINTER_TYPE_P (type))
    return true;

  return false;
}

/* Return an integer type of SIZE bits, unsigned if UNSIGNEDP is set, signed
   otherwise.  */

tree
get_integer_type (unsigned int size, bool unsignedp)
{
  switch (size)
    {
      case 8:  return unsignedp ? unsigned_intQI_type_node : intQI_type_node;
      case 16: return unsignedp ? unsigned_intHI_type_node : intHI_type_node;
      case 32: return unsignedp ? unsigned_intSI_type_node : intSI_type_node;
      case 64: return unsignedp ? unsigned_intDI_type_node : intDI_type_node;
      default:
	gcc_unreachable ();
    }
}

/******************************************************************************
 * Strings                                                                    *
 ******************************************************************************/

/* Hash value calculation function for referenced strings.  */

static hashval_t
str_ref_hash (const void *ptr)
{
  const_str_ref ref = (const_str_ref) ptr;
  const char *str = TREE_STRING_POINTER (ref->cst);
  hashval_t hash = 0;
  size_t len = TREE_STRING_LENGTH (ref->cst);
  size_t i;

  for (i = 0; i < len; i++)
    {
      hash += str[i];
      hash += (hash << 10);
      hash ^= (hash >> 6);
    }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

/* Equality function for referenced strings.  */

static int
str_ref_eq (const void *ptr1, const void *ptr2)
{
  const char *str1 = TREE_STRING_POINTER (((const_str_ref) ptr1)->cst);
  const char *str2 = TREE_STRING_POINTER (((const_str_ref) ptr2)->cst);

  return str1 == str2;
}

/* Mark the string represented by tree STR as referenced.  If an identical
   string has been already marked it will be returned, otherwise STR will
   will be returnd.  Referenced strings are emitted at the end of the
   compilation unit, non-referenced strings are not. STR must be a STRING_CST
   node.  */

tree
mark_referenced_string (tree str)
{
  struct str_ref_d tmp;
  void **slot;

  tmp.cst = str;
  slot = htab_find_slot (ref_strings, &tmp, INSERT);

  if (*slot == NULL)
    {
      str_ref ref = GGC_NEW (struct str_ref_d);

      ref->cst = str;
      ref->id = string_id++;
      *slot = ref;

      return str;
    }
  else
    return ((str_ref) *slot)->cst;
}

/* Get an unique id for string constant NODE.   */

unsigned int
get_string_cst_id (tree str)
{
  struct str_ref_d tmp;
  void **slot;

  tmp.cst = str;
  slot = htab_find_slot (ref_strings, &tmp, INSERT);

  gcc_assert (*slot != NULL);
  return ((str_ref) *slot)->id;
}

/* Return the hash table holding the referenced strings.  */

htab_t
referenced_strings_htab ( void )
{
  return ref_strings;
}

/******************************************************************************
 * Functions                                                                  *
 ******************************************************************************/

/* Hash function for pinvokes */

static hashval_t
pinvoke_hash (const void *ptr)
{
  const_tree func = (const_tree) ptr;
  return (hashval_t) DECL_UID (func);
}

/* Equality function for pinvokes */

static int
pinvoke_eq (const void *ptr1, const void *ptr2)
{
  const_tree func1 = (const_tree) ptr1;
  const_tree func2 = (const_tree) ptr2;

  return DECL_UID (func1) == DECL_UID (func2);
}

/* Mark the function represented by tree T as a pinvoke.
   T must be a FUNCTION_DECL node.  */

void
add_pinvoke (tree t)
{
  void **slot;

  gcc_assert (TREE_CODE (t) == FUNCTION_DECL);

  refs_init ();

  slot = htab_find_slot (ref_pinvokes, t, INSERT);

  if (*slot == NULL)
    *slot = t;

  mark_referenced_type (TREE_TYPE (t));
}

/* Return the hash table holding the referenced strings.  */

htab_t
pinvokes_htab ( void )
{
  return ref_pinvokes;
}

/******************************************************************************
 * Labels                                                                     *
 ******************************************************************************/

/* Hash function for label addresses.  */

static hashval_t
label_addr_hash (const void *ptr)
{
  const_label_addr addr = (const_label_addr) ptr;
  return (hashval_t) LABEL_DECL_UID (addr->label);
}

/* Equality function for label addresses.  */

static int
label_addr_eq (const void *ptr1, const void *ptr2)
{
  const_label_addr addr1 = (const_label_addr) ptr1;
  const_label_addr addr2 = (const_label_addr) ptr2;

  return LABEL_DECL_UID (addr1->label) == LABEL_DECL_UID (addr2->label);
}

/* Add a LABEL_DECL to the map of labels whose addresses were taken.  */

void
record_addr_taken_label (tree label)
{
  struct machine_function *machine = cfun->machine;
  void **slot;
  label_addr addr = GGC_NEW (struct label_addr_d);

  addr->label = label;
  addr->id = build_int_cst (intSI_type_node, machine->label_id++);

  gcc_assert (htab_find (labels_map, addr) == NULL);
  slot = htab_find_slot (labels_map, addr, INSERT);
  *slot = addr;
}

/* Returns the integer ID corresponding to the label LABEL.  */

tree
get_addr_taken_label_id (tree label)
{
  struct label_addr_d addr;

  addr.label = label;
  addr.id = NULL_TREE;

  return ((label_addr) htab_find (labels_map, &addr))->id;
}

/* Helper function used by get_label_addr_ids().  */

static int
fill_label_addrs (void **slot, void *data)
{
  tree addrs = data;
  tree case_label;
  label_addr addr = *slot;

  if (DECL_CONTEXT (addr->label) == current_function_decl)
    {
      case_label = build3 (CASE_LABEL_EXPR, void_type_node,
			   addr->id, NULL_TREE, addr->label);
      TREE_VEC_ELT (addrs, tree_low_cst (addr->id, 1)) = case_label;
    }

  return 1;
}

/* Return a tree array holding the INTEGER_CST used as IDs for mimicking the
   label addresses in computed GOTO statements.  The underlying array is
   allocated lazily.  */

tree
get_label_addrs ( void )
{
  struct machine_function *machine = cfun->machine;
  tree default_label;

  if (machine->label_addrs == NULL_TREE)
    {
      /* TODO: CIL switches cannot be larger than 8192 entries. Handling larger
	 ranges could make the code for replacing computed GOTOs significnaly
	 more complex.  */
      gcc_assert (machine->label_id < 8192);

      machine->label_addrs = make_tree_vec (machine->label_id + 1);
      htab_traverse (labels_map, fill_label_addrs, machine->label_addrs);

      /* Add a fake default label.  */
      default_label = build3 (CASE_LABEL_EXPR, void_type_node,
			      build_int_cst (NULL_TREE, machine->label_id),
			      NULL_TREE, create_artificial_label ());
      TREE_VEC_ELT (machine->label_addrs, machine->label_id) = default_label;
    }

  return machine->label_addrs;
}

/******************************************************************************
 * Constructors                                                               *
 ******************************************************************************/

void
record_ctor (tree decl)
{
  VEC_safe_push (tree, gc, pending_ctors, decl);
}

void
create_init_method (void)
{
  struct function *current_cfun = cfun;
  tree fun_type;
  tree fun_decl;
  tree init_expr = NULL;
  tree result;
  size_t i;

  if (VEC_length (tree, pending_ctors) != 0)
    {
      fun_type = build_function_type (void_type_node, void_list_node);
      fun_decl = build_decl (FUNCTION_DECL, get_identifier ("COBJ?init"),
			     fun_type);

      result = build_decl (RESULT_DECL, NULL_TREE, void_type_node);
      DECL_ARTIFICIAL (result) = 1;
      DECL_IGNORED_P (result) = 1;
      DECL_RESULT (fun_decl) = result;

      /* Allocate memory for the function structure.  The call to
	 allocate_struct_function clobbers CFUN, so we need to restore
	 it afterward.  */
      allocate_struct_function (fun_decl, false);

      TREE_STATIC (fun_decl) = 1;
      TREE_USED (fun_decl) = 1;
      DECL_ARTIFICIAL (fun_decl) = 1;
      DECL_IGNORED_P (fun_decl) = 0;
      TREE_PUBLIC (fun_decl) = 0;
      DECL_NO_INSTRUMENT_FUNCTION_ENTRY_EXIT (fun_decl) = 1;
      DECL_UNINLINABLE (fun_decl) = 1;
      DECL_EXTERNAL (fun_decl) = 0;
      DECL_STATIC_CONSTRUCTOR (fun_decl) = 1;
      DECL_CONTEXT (fun_decl) = NULL_TREE;
      DECL_INITIAL (fun_decl) = make_node (BLOCK);

      for (i = 0; i < VEC_length (tree, pending_ctors); i++)
	{
	  tree decl = VEC_index (tree, pending_ctors, i);
	  tree init = DECL_INITIAL (decl);

	  DECL_INITIAL (decl) = NULL_TREE;
	  expand_init_to_stmt_list (decl, init, &init_expr);
	}

      DECL_SAVED_TREE (fun_decl) = init_expr;

      gimplify_function_tree (fun_decl);
      tree_lowering_passes (fun_decl);
      tree_rest_of_compilation (fun_decl);

      /* Restore the current function */
      set_cfun (current_cfun);
  }
}

/* Return TRUE if EXP contains mostly (3/4)  zeros.  */

static bool
mostly_zeros_p (tree exp)
{
  HOST_WIDE_INT nz_elts, count, elts;
  bool must_clear;

  gcc_assert (TREE_CODE (exp) == CONSTRUCTOR);

  categorize_ctor_elements (exp, &nz_elts, &count, &must_clear);

  if (must_clear)
    return TRUE;

  elts = count_type_elements (TREE_TYPE (exp), false);

  return (nz_elts < elts / 4);
}

/* Return TRUE if EXP contains all zeros. */

static bool
all_zeros_p (tree exp)
{
  HOST_WIDE_INT nz_elts, count;
  bool must_clear;

  gcc_assert (TREE_CODE (exp) == CONSTRUCTOR);

  categorize_ctor_elements (exp, &nz_elts, &count, &must_clear);

  return (nz_elts == 0);
}

/* Expand the initialization of tree DECL to tree INIT
   into the statement list pointed by STMT_LIST.
   Beware that statements inserted into the list cannot be assumed
   to be in GIMPLE form and/or simplified for CIL.
   If GIMPLE CIL-simplified statements are required, explicit
   gimplification and CIL simplification have to be performed on them.
   CLEARED tells whether unmentioned fields in the initializer
   statement may be considered already initialized to zero or not.
   The expansion is especially meant to expand a CONSTRUCTOR into
   an equivalent statement sequence; anyway, any initialization
   is properly handled: in case of no expansion, a simple MODIFY_EXPR
   is appended to STMT_LIST.
   STMT_LIST may be NULL; in this case a statement list is allocated.
*/

static void
expand_init_to_stmt_list1 (tree decl, tree init,
			   tree *stmt_list1, bool cleared,
			   tree *stmt_list2, void *le_image, void *be_image,
			   unsigned HOST_WIDE_INT image_offset)
{
  tree decl_size = TYPE_SIZE_UNIT (TREE_TYPE (decl));
  unsigned HOST_WIDE_INT size = tree_low_cst (decl_size, 1);
  bool need_to_clear = FALSE;

  gcc_assert (TREE_CODE (*stmt_list1) == STATEMENT_LIST);
  gcc_assert (TREE_CODE (*stmt_list2) == STATEMENT_LIST);

  if (TREE_CODE (init) == CONST_DECL)
    {
      init = DECL_INITIAL (init);
      gcc_assert (init && init != error_mark_node);
    }

  if (!cleared && TREE_CODE (init) == CONSTRUCTOR && all_zeros_p (init))
    {
      tree args, t, decl_ptr;

      args = tree_cons (NULL, decl_size, NULL);
      args = tree_cons (NULL, integer_zero_node, args);
      decl_ptr = build_fold_addr_expr (decl);
      args = tree_cons (NULL, decl_ptr, args);
      t = cil32_builtins[CIL32_BUILT_IN_INITBLK];
      t = build_function_call_expr (t, args);

      append_to_statement_list (t, stmt_list1);

      return;
    }

  switch (TREE_CODE (init))
    {
    case STRING_CST:
      {
	tree args, t, to_ptr, from_ptr;

	gcc_assert (TREE_CODE (TREE_TYPE (init)) == ARRAY_TYPE);

	args = tree_cons (NULL, decl_size, NULL);

	from_ptr = build_fold_addr_expr (init);
	args = tree_cons (NULL, from_ptr, args);

	to_ptr = build_fold_addr_expr (decl);
	args = tree_cons (NULL, to_ptr, args);

	/* We know they do not overlap */
	t = cil32_builtins[CIL32_BUILT_IN_CPBLK];
	t = build_function_call_expr (t, args);

	append_to_statement_list (t, stmt_list1);

	memcpy((unsigned char *) le_image + image_offset,
	       TREE_STRING_POINTER (init), tree_low_cst (decl_size, 1));
	memcpy((unsigned char *) be_image + image_offset,
	       TREE_STRING_POINTER (init), tree_low_cst (decl_size, 1));
      }
    break;

    case CONSTRUCTOR:
      switch (TREE_CODE (TREE_TYPE (init)))
	{
	case RECORD_TYPE:
	case UNION_TYPE:
	case QUAL_UNION_TYPE:
	  {
	    unsigned HOST_WIDE_INT idx;
	    tree init_type = TREE_TYPE (init);
	    tree field, value;

	    /* If size is zero or the target is already cleared, do nothing */
	    if (size == 0 || cleared)
	      {
		need_to_clear = FALSE;
		cleared = TRUE;
	      }

	    /* We either clear the aggregate or indicate the value is dead.  */
	    else if ((TREE_CODE (init_type) == UNION_TYPE
		      || TREE_CODE (init_type) == QUAL_UNION_TYPE)
		     && !CONSTRUCTOR_ELTS (init))
	      /* If the constructor is empty, clear the union.  */
	      need_to_clear = TRUE;

	    /* If the constructor has fewer fields than the structure or
	       if we are initializing the structure to mostly zeros, clear
	       the whole structure first. */
	    else if (size > 0
		     && (((int)VEC_length (constructor_elt,
					   CONSTRUCTOR_ELTS (init))
			  != fields_length (init_type))
			 || mostly_zeros_p (init)))
		need_to_clear = TRUE;

	    if (need_to_clear && size > 0)
	      {
		tree args, t, decl_ptr;

		args = tree_cons (NULL, decl_size, NULL);
		args = tree_cons (NULL, integer_zero_node, args);
		decl_ptr = build_fold_addr_expr (decl);
		args = tree_cons (NULL, decl_ptr, args);
		t = cil32_builtins[CIL32_BUILT_IN_INITBLK];
		t = build_function_call_expr (t, args);

		append_to_statement_list (t, stmt_list1);

		cleared = TRUE;
	      }

	    /* Store each element of the constructor into the
	       corresponding field of TARGET.  */
	    FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (init), idx,
				      field, value)
	      {
		tree ltarget;

		/* Just ignore missing fields.  We cleared the whole
		   structure, above, if any fields are missing.  */
		if (field == 0)
		  continue;

		if (cleared && initializer_zerop (value))
		  continue;

		ltarget = build3 (COMPONENT_REF, TREE_TYPE (field), decl, field, NULL);

		if (le_image != NULL && !DECL_BIT_FIELD (field))
		  {
		    unsigned HOST_WIDE_INT offset = tree_low_cst (DECL_FIELD_OFFSET (field), 1);
		    unsigned HOST_WIDE_INT bit_offset = tree_low_cst (DECL_FIELD_BIT_OFFSET (field), 1);
		    gcc_assert (bit_offset % BITS_PER_UNIT == 0);
		    offset += (bit_offset / BITS_PER_UNIT) + image_offset;

		    expand_init_to_stmt_list1 (ltarget, value,
					       stmt_list1, cleared,
					       stmt_list2,
					       le_image, be_image, offset);
		  }
		else if (le_image != NULL && DECL_BIT_FIELD (field)
			 && (TARGET_LITTLE_ENDIAN || TARGET_BIG_ENDIAN))
		  {
		    unsigned char b0, b1, b2, b3;
		    unsigned HOST_WIDE_INT offset = 0;
		    HOST_WIDE_INT bit_size = 0;
		    HOST_WIDE_INT bit_pos = 0;
		    HOST_WIDE_INT cont_off;
		    HOST_WIDE_INT cont_size = 8;
		    enum machine_mode mode;
		    int unsignedp = 0;
		    int volatilep = 0;
		    tree cont_type;
		    tree shift_cst;
		    tree tmp;

		    get_inner_reference (ltarget, &bit_size, &bit_pos,
					 &tmp, &mode, &unsignedp,
					 &volatilep, false);

		    /* Calculate the container size.  */
		    while ((bit_pos % cont_size + bit_size) > cont_size)
		      cont_size *= 2;

		    if (cont_size > 32)
		      {
			expand_init_to_stmt_list1 (ltarget, value,
						   stmt_list1, cleared,
						   stmt_list2, NULL, NULL, 0);
		      }
		    else
		      {
			cont_type = get_integer_type (cont_size, true);
			cont_off = bit_pos % cont_size;

			/* Calculate the container offset.  */
			if ((bit_pos - cont_off) / BITS_PER_UNIT != 0)
			  {
			    tmp = build_int_cst (intSI_type_node,
						 (bit_pos - cont_off)
						 / BITS_PER_UNIT);
			    offset = tree_low_cst (tmp, 1);
			  }

			shift_cst = build_int_cst (intSI_type_node, cont_off);
			tmp = fold_binary_to_constant (LSHIFT_EXPR, cont_type,
						       fold_convert (cont_type,
								     value),
						       shift_cst);

			b0 = tree_low_cst (tmp, 1);
			b1 = tree_low_cst (tmp, 1) >> 8;
			b2 = tree_low_cst (tmp, 1) >> 16;
			b3 = tree_low_cst (tmp, 1) >> 24;

			switch (cont_size)
			  {
			  case 8:
			    *((unsigned char *) le_image + offset) |= b0;
			    *((unsigned char *) be_image + offset) |= b0;
			    break;

			  case 16:
			    *((unsigned char *) le_image + offset + 0) |= b0;
			    *((unsigned char *) le_image + offset + 1) |= b1;
			    *((unsigned char *) be_image + offset + 0) |= b1;
			    *((unsigned char *) be_image + offset + 1) |= b0;
			    break;

			  case 32:
			    *((unsigned char *) le_image + offset + 0) |= b0;
			    *((unsigned char *) le_image + offset + 1) |= b1;
			    *((unsigned char *) le_image + offset + 2) |= b2;
			    *((unsigned char *) le_image + offset + 3) |= b3;
			    *((unsigned char *) be_image + offset + 0) |= b3;
			    *((unsigned char *) be_image + offset + 1) |= b2;
			    *((unsigned char *) be_image + offset + 2) |= b1;
			    *((unsigned char *) be_image + offset + 3) |= b0;
			    break;

		          default:
			    gcc_unreachable ();
		          }
		      }
		  }
		else
		  {
		    expand_init_to_stmt_list1 (ltarget, value,
					       stmt_list1, cleared,
					       stmt_list2, NULL, NULL, 0);
		  }
	      }
	  }
	  break;

	case ARRAY_TYPE:
	  {
	    tree value, index;
	    unsigned HOST_WIDE_INT i;
	    tree domain;
	    tree elttype = TREE_TYPE (TREE_TYPE (init));
	    int const_bounds_p;
	    HOST_WIDE_INT minelt = 0;
	    HOST_WIDE_INT maxelt = 0;

	    domain = TYPE_DOMAIN (TREE_TYPE (init));
	    const_bounds_p = (TYPE_MIN_VALUE (domain)
			      && TYPE_MAX_VALUE (domain)
			      && host_integerp (TYPE_MIN_VALUE (domain), 0)
			      && host_integerp (TYPE_MAX_VALUE (domain), 0));

	    /* If we have constant bounds for the range
	       of the type, get them.  */
	    if (const_bounds_p)
	      {
		minelt = tree_low_cst (TYPE_MIN_VALUE (domain), 0);
		maxelt = tree_low_cst (TYPE_MAX_VALUE (domain), 0);
	      }

	    /* If the constructor has fewer elements than the array, clear
	       the whole array first. */
	    if (cleared)
	      need_to_clear = FALSE;
	    else
	      {
		unsigned HOST_WIDE_INT idx;
		tree index, value;
		HOST_WIDE_INT count = 0;
		HOST_WIDE_INT zero_count = 0;
		need_to_clear = !const_bounds_p;

		/* This loop is a more accurate version of the loop in
		   mostly_zeros_p (it handles RANGE_EXPR in an index).  It
		   is also needed to check for missing elements.  */
		FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (init), idx,
					  index, value)
		  {
		    HOST_WIDE_INT this_node_count;

		    if (need_to_clear)
		      break;

		    if (index != NULL_TREE && TREE_CODE (index) == RANGE_EXPR)
		      {
			tree lo_index = TREE_OPERAND (index, 0);
			tree hi_index = TREE_OPERAND (index, 1);

			if (!host_integerp (lo_index, 1)
			    || !host_integerp (hi_index, 1))
			  {
			    need_to_clear = TRUE;
			    break;
			  }

			this_node_count = tree_low_cst (hi_index, 1)
					  - tree_low_cst (lo_index, 1) + 1;
		      }
		    else
		      this_node_count = 1;

		    count += this_node_count;
		    if (TREE_CODE (value) == CONSTRUCTOR
		        && mostly_zeros_p (value))
		      {
			zero_count += this_node_count;
		      }
		  }

		/* Clear the entire array first if there are any missing
		   elements, or if the incidence of zero elements is >= 75%.  */
		if (!need_to_clear
		    && (count < maxelt - minelt + 1
			|| 4 * zero_count >= 3 * count))
		  {
		    need_to_clear = TRUE;
		  }
	      }

	    if (need_to_clear && size > 0)
	      {
		tree args, t, decl_ptr;

		args = tree_cons (NULL, decl_size, NULL);
		args = tree_cons (NULL, integer_zero_node, args);
		decl_ptr = build_fold_addr_expr (decl);
		args = tree_cons (NULL, decl_ptr, args);
		t = cil32_builtins[CIL32_BUILT_IN_INITBLK];
		t = build_function_call_expr (t, args);

		append_to_statement_list (t, stmt_list1);

		cleared = TRUE;
	      }

	    /* Store each element of the constructor into the
	       corresponding element of TARGET, determined by counting the
	       elements.  */
	    FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (init), i, index, value)
	      {
		tree t;
		tree elsize;

		if (initializer_zerop (value))
		  continue;

		gcc_assert (index == NULL_TREE
			    || TREE_CODE (index) != RANGE_EXPR);

		if (minelt)
		  index = fold_convert (ssizetype,
					fold_build2 (MINUS_EXPR,
						     TREE_TYPE (index),
						     index,
						     TYPE_MIN_VALUE (domain)));

		t = build4 (ARRAY_REF, elttype, decl, index, NULL, NULL);

		elsize = array_ref_element_size (t);

		if (le_image != NULL
		    && TREE_CODE (index)  == INTEGER_CST
		    && TREE_CODE (elsize) == INTEGER_CST)
		  {
		    unsigned HOST_WIDE_INT offset;

		    offset = tree_low_cst (index, 1) * tree_low_cst (elsize, 1)
		             + image_offset;
		    expand_init_to_stmt_list1 (t, value,
					       stmt_list1, cleared,
					       stmt_list2,
					       le_image, be_image, offset);
		  }
		else
		  {
		    expand_init_to_stmt_list1 (t, value,
					       stmt_list1, cleared,
					       stmt_list2, NULL, NULL, 0);
		  }
	      }
	  }
	  break;

	case VECTOR_TYPE:
	  {
	    tree fun, stmt;
	    tree args = NULL;
	    tree value;
	    unsigned HOST_WIDE_INT idx;
	    tree vector_type = TREE_TYPE (init);
	    tree vector_elt_type = TREE_TYPE (vector_type);
	    unsigned HOST_WIDE_INT elt_size;
	    unsigned HOST_WIDE_INT num_elt;
	    unsigned HOST_WIDE_INT i, num_args = 0;
	    enum cil32_builtin builtin;

	    elt_size = tree_low_cst (TYPE_SIZE (vector_elt_type), 1);
	    num_elt = TYPE_VECTOR_SUBPARTS (vector_type);

	    /* Build the list of args. */
	    FOR_EACH_CONSTRUCTOR_VALUE (CONSTRUCTOR_ELTS (init), idx, value)
	      {
		args = tree_cons (NULL, value, args);
		++num_args;
	      }

	    /* The constructor might not initialize all args.  */
	    for (i = num_args; i < num_elt; i++)
	      args = tree_cons (NULL, integer_zero_node, args);

	    /* find the right constructor */
	    if (TREE_CODE (vector_elt_type) == INTEGER_TYPE)
	      {
		switch (num_elt)
		  {
		  case 2:
		    switch (elt_size)
		      {
		      case 16: builtin = CIL32_V2HI_CTOR; break;
		      case 32: builtin = CIL32_V2SI_CTOR; break;
		      default:
			internal_error ("V2QI/DI vectors are not supported");
		      }

		    break;

		  case 4:
		    switch (elt_size)
		      {
		      case 8:  builtin = CIL32_V4QI_CTOR; break;
		      case 16: builtin = CIL32_V4HI_CTOR; break;
		      case 32: builtin = CIL32_V4SI_CTOR; break;
		      default:
			internal_error ("V4DI vectors are not supported");
		      }

		    break;

		  case 8:
		    switch (elt_size)
		      {
		      case 8:  builtin = CIL32_V8QI_CTOR; break;
		      case 16: builtin = CIL32_V8HI_CTOR; break;
		      default:
			internal_error ("V8SI/DI vectors are not supported");
		      }

		    break;

		  case 16:
		    switch (elt_size)
		      {
		      case 8: builtin = CIL32_V16QI_CTOR; break;
		      default:
			internal_error ("V16HI/SI/DI vectors are not supported");
		      }

		    break;

		  default:
		    internal_error (HOST_WIDE_INT_PRINT_UNSIGNED" bit wide "
				    "vectors are not supported",
				    num_elt * elt_size);
		  }
	      }
	    else if (TREE_CODE (vector_elt_type) == REAL_TYPE)
	      {
		switch (num_elt)
		  {
		  case 2: builtin = CIL32_V2SF_CTOR; break;
		  case 4: builtin = CIL32_V4SF_CTOR; break;
		  default:
		    internal_error ("V"HOST_WIDE_INT_PRINT_UNSIGNED
				    "SF vectors not supported\n", num_elt);
		  }
	      }
	    else
	      gcc_unreachable ();

	    /* Note that the args list must be reversed. Can do better? */
	    fun = build_function_call_expr (cil32_builtins[builtin],
					    nreverse (args));
	    stmt = build_gimple_modify_stmt(decl, fun);
	    append_to_statement_list (stmt, stmt_list1);
	    append_to_statement_list (stmt, stmt_list2);
	  }
	  break;

	default:
	  gcc_unreachable ();
	  break;
	}
      break;

    case INTEGER_CST:
      {
	unsigned HOST_WIDE_INT type_size = tree_low_cst (decl_size, 1);
	tree t = build_gimple_modify_stmt(decl, init);
	append_to_statement_list (t, stmt_list1);

	if (le_image != NULL)
	  {
	    unsigned char b0 = TREE_INT_CST_LOW (init);
	    unsigned char b1 = TREE_INT_CST_LOW (init) >> 8;
	    unsigned char b2 = TREE_INT_CST_LOW (init) >> 16;
	    unsigned char b3 = TREE_INT_CST_LOW (init) >> 24;

            gcc_assert (be_image != NULL);
	    switch (type_size)
	      {
	      case 1:
		*((unsigned char *) le_image + image_offset) = b0;
		*((unsigned char *) be_image + image_offset) = b0;
		break;

	      case 2:
		*((unsigned char *) le_image + image_offset + 0) = b0;
		*((unsigned char *) le_image + image_offset + 1) = b1;
		*((unsigned char *) be_image + image_offset + 0) = b1;
		*((unsigned char *) be_image + image_offset + 1) = b0;
		break;

	      case 4:
		*((unsigned char *) le_image + image_offset + 0) = b0;
		*((unsigned char *) le_image + image_offset + 1) = b1;
		*((unsigned char *) le_image + image_offset + 2) = b2;
		*((unsigned char *) le_image + image_offset + 3) = b3;
		*((unsigned char *) be_image + image_offset + 0) = b3;
		*((unsigned char *) be_image + image_offset + 1) = b2;
		*((unsigned char *) be_image + image_offset + 2) = b1;
		*((unsigned char *) be_image + image_offset + 3) = b0;
		break;

	      default:
		append_to_statement_list (t, stmt_list2);
		break;
	      }
	  }
	else
	  {
	    append_to_statement_list (t, stmt_list2);
	  }
      }
      break;

    case REAL_CST:
      /* Missing optimization, fall through for now */
    default:
      {
	tree t = build_gimple_modify_stmt(decl, init);
	append_to_statement_list (t, stmt_list1);
	append_to_statement_list (t, stmt_list2);
      }
      break;
    }
}

static int
statement_list_num_instr (tree stmt_list)
{
  int i = 0;
  tree_stmt_iterator it = tsi_start (stmt_list);
  while (!tsi_end_p (it))
    {
      ++i;
      tsi_next (&it);
    }
  return i;
}

void
expand_init_to_stmt_list (tree decl, tree init, tree *stmt_list)
{
  unsigned HOST_WIDE_INT size = tree_low_cst (TYPE_SIZE_UNIT (TREE_TYPE (decl)), 1);
  void *le_image = alloca (size);
  void *be_image = alloca (size);
  tree stmt_list1 = alloc_stmt_list ();
  int num_list1;
  tree stmt_list2 = alloc_stmt_list ();
  int num_list2;

  memset (le_image, 0, size);
  memset (be_image, 0, size);

  expand_init_to_stmt_list1 (decl, init,
			     &stmt_list1, FALSE,
			     &stmt_list2, le_image, be_image, 0);

  num_list1 = statement_list_num_instr (stmt_list1);
  num_list2 = statement_list_num_instr (stmt_list2);


  /* Decide what to do */
  if (TARGET_BIG_ENDIAN || TARGET_LITTLE_ENDIAN
      || ( (num_list2 + 2) < num_list1))
    {
      tree mem_cpy;
      tree args;
      tree from_ptr;
      tree to_ptr = build_fold_addr_expr (decl);

      if (TARGET_LITTLE_ENDIAN || (memcmp (le_image, be_image, size) == 0))
	{
	  from_ptr = build_string_literal (size, le_image);
	}
      else if (TARGET_BIG_ENDIAN)
	{
	  from_ptr = build_string_literal (size, be_image);
	}
      else
	{
	  tree sconst = build_string_literal (size, le_image);
	  tree sconst2 = build_string_literal (size, be_image);

	  gcc_assert (TREE_TYPE (sconst) == TREE_TYPE (sconst2));

	  args = tree_cons (NULL, sconst2, NULL);
	  args = tree_cons (NULL, sconst, args);
	  from_ptr = build_function_call_expr (cil32_builtins[CIL32_BUILT_IN_ENDIAN_SELECT], args);
	}

      args = tree_cons (NULL, size_int (size), NULL);
      args = tree_cons (NULL, from_ptr, args);
      args = tree_cons (NULL, to_ptr,   args);

      mem_cpy = build_function_call_expr (cil32_builtins[CIL32_BUILT_IN_CPBLK],
					  args);

      append_to_statement_list (mem_cpy,    stmt_list);
      append_to_statement_list (stmt_list2, stmt_list);
    }
  else
    append_to_statement_list (stmt_list1, stmt_list);
}

#include "gt-cil-refs.h"
