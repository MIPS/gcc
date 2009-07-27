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
#include "errors.h"
#include "toplev.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "vec.h"

#include "cil-builtins.h"
#include "cil-refs.h"
#include "cil-types.h"
#include "cil-stack.h"

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
  const_tree str1 = ((const_tree) ptr1);
  const_tree str2 = ((const_tree) ptr2);

  return (TREE_STRING_LENGTH (str1) == TREE_STRING_LENGTH (str2))
	 && ! memcmp (TREE_STRING_POINTER (str1), TREE_STRING_POINTER (str2),
		      TREE_STRING_LENGTH (str1));
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
  if (type == NULL_TREE || type == error_mark_node)
    return type;

  if (cil_value_type_p (type) || cil_vector_type_p (type))
    return TYPE_MAIN_VARIANT (type);
  else
    {
      cil_type_t cil_type = type_to_cil (type);

      if (cil_int_or_smaller_p(cil_type))
        return unsigned_intSI_type_node;
      else if (cil_long_p(cil_type))
        return unsigned_intDI_type_node;
      else if (cil_float_p(cil_type))
        return double_type_node;
      else if (cil_pointer_p(cil_type))
        {
          /* cil32 is a 32bit machine, in case we support 64bit model
           * changes are needed
           */
          return unsigned_intSI_type_node;
        }
      else
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
  const_tree str1 = ((const_str_ref) ptr1)->cst;
  const_tree str2 = ((const_str_ref) ptr2)->cst;

  return (TREE_STRING_LENGTH (str1) == TREE_STRING_LENGTH (str2))
	 && ! memcmp (TREE_STRING_POINTER (str1), TREE_STRING_POINTER (str2),
		      TREE_STRING_LENGTH (str1));
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
  tree addrs = (tree) data;
  tree case_label;
  label_addr addr = (label_addr) *slot;

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

VEC(tree, gc) *
pending_ctors_vec ( void )
{
  return pending_ctors;
}

#include "gt-cil-refs.h"

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
