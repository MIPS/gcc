/* Dump of the GENERIC trees in CIL.

   Copyright (C) 2006-2007 Free Software Foundation, Inc.

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
   Andrea Bona
   Andrea Ornstein
   Erven Rohou
   Roberto Costa

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Erven Rohou             <erven.rohou@st.com>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "diagnostic.h"
#include "real.h"
#include "hashtab.h"
#include "tree-flow.h"
#include "langhooks.h"
#include "tree-iterator.h"
#include "tree-chrec.h"
#include "tree-pass.h"
#include "timevar.h"
#include "assert.h"
#include "toplev.h"
#include "pointer-set.h"
#include "output.h"
#include "varray.h"
#include "ggc.h"
#include "tree-simp-cil.h"
#include "gen-cil.h"
#include "emit-hints.h"

/* Nonzero for a type which is at file scope.  */
#define TYPE_FILE_SCOPE_P(EXP)                                  \
  (! TYPE_CONTEXT (EXP)                                         \
   || TREE_CODE (TYPE_CONTEXT (EXP)) == TRANSLATION_UNIT_DECL)

/* Nonzero for a zero-length array type */
#define ARRAY_TYPE_ZEROLENGTH(EXP)                              \
  (TYPE_SIZE (EXP) == NULL_TREE)

/* Nonzero for a variable-length array type */
#define ARRAY_TYPE_VARLENGTH(EXP)                               \
  (TYPE_SIZE (EXP) != NULL_TREE && TREE_CODE (TYPE_SIZE (EXP)) != INTEGER_CST)

/* Length of compacted identifiers (in characters) */
#define COMPACT_ID_LENGTH 16

struct fnct_attr
{
  const char *assembly_name;
  const char *cil_name;
  const char *cusattr_string;
  const char *pinvoke_assembly;
  const char *pinvoke_fname;
};

/* Element of the pointer id hash table */
struct pointer_id_data
{
  const void *ptr;
  unsigned int id;
};

/* Local functions, macros and variables.  */
static hashval_t pointer_id_data_hash (const void *);
static int pointer_id_data_eq (const void *, const void *);
static void decode_function_attrs (tree, struct fnct_attr *);
static void mark_referenced_type (tree);
static void mark_referenced_string (tree);
static unsigned int get_string_cst_id (tree);
static void dump_decl_name (FILE *, tree);
static void dump_string_name (FILE *, tree);
static void dump_label_name (FILE *, tree);
static void dump_entry_label_name (FILE *, basic_block);
static void dump_fun_type (FILE *, tree, tree, const char *, bool);
static void dump_valuetype_name (FILE *, tree);
static void gen_addr_expr (FILE *, tree);
static void print_type_suffix (FILE *, tree, bool);
static void gen_cil_modify_expr (FILE *, tree, tree);
static char * append_string (char *, const char *,
                             unsigned int *, unsigned int *);
static char * append_coded_type (char *, tree, unsigned int *, unsigned int *);
static char * get_compact_identifier (const char *, size_t, size_t *);
static tree make_valuetype_identifier (tree);
static void print_valuetype_decl (FILE *, tree);
static void dump_type (FILE *, tree, bool, bool);
static void dump_complex_type (FILE *, tree, bool);
static void dump_vector_type (FILE *, tree, bool);
static void dump_type_promotion (FILE *, tree, bool);
static bool dump_type_promoted_type_def (FILE *, tree);
static void dump_type_for_builtin (FILE *, tree, bool);
static void dump_type_eval_mode (FILE *, tree, bool, bool);

static void stack_set (unsigned int) ATTRIBUTE_UNUSED;
static void stack_reset (void);
static void stack_push (unsigned int);
static void stack_pop (unsigned int);
static void gen_conv (FILE *, bool, tree, tree);
static void gen_integral_conv (FILE *, tree, tree);
static void gen_binary_cond_branch (FILE *, tree, tree);
static void gen_call_expr (FILE *, tree);
static void gen_start_function (FILE *);
static void gen_string_custom_attr (FILE *, const char*);
static unsigned int gen_cil (void);
static void gen_cil_1 (FILE *);
static void gen_cil_node (FILE *, tree);
static bool gen_cil_gate (void);
static void create_init_method(void);

static struct pointer_set_t *referenced_types;
static GTY(()) varray_type referenced_strings;
static struct pointer_set_t *referenced_string_ptrs;
static struct pointer_set_t *referenced_pinvoke;
static GTY(()) varray_type pending_ctors;
static htab_t pointer_id_htable;
static unsigned int pointer_next_id = 0;
static unsigned int stack;
static unsigned int max_stack;
static basic_block bb;
static tree addr_taken_labels = NULL;


/* Hashing and equality routines for pointer id hash table.  */
static hashval_t
pointer_id_data_hash (const void *p)
{
  return ((unsigned long)((struct pointer_id_data *)p)->ptr >> 4) & 255;
}

static int
pointer_id_data_eq (const void *p1, const void *p2)
{
  const void* ptr1 = ((struct pointer_id_data *)p1)->ptr;
  const void* ptr2 = ((struct pointer_id_data *)p2)->ptr;

  return ptr1 == ptr2;
}

/* stack_* functions are used to keep track of the number of elements
   in the evaluation stack, in order to record the maximum number.
   This is emitted in .maxstack directive for each function.   */

/* Set that the evaluation stack contains n elements.   */

static void
stack_set (unsigned int n)
{
  stack = n;
  if (stack > max_stack)
    max_stack = stack;
}

/* Set that the evaluation stack is empty.   */

static inline void
stack_reset (void)
{
  stack = 0;
}

/* Add n elements to the evaluation stack.   */

static void
stack_push (unsigned int n)
{
  stack += n;
  if (stack > max_stack)
    max_stack = stack;
}

/* Remove n elements from the evaluation stack.   */

static inline void
stack_pop (unsigned int n)
{
  gcc_assert (stack >= n);
  stack -= n;
}

/* Given the FUNCTION_DECL tree T, decode its CIL-specific function
   attributes and record them in ATTRS.   */

static void
decode_function_attrs (tree t, struct fnct_attr *attrs)
{
  tree tmp;

  gcc_assert (TREE_CODE (t) == FUNCTION_DECL);

  attrs->assembly_name    = 0;
  attrs->cil_name         = 0;
  attrs->cusattr_string   = 0;
  attrs->pinvoke_assembly = 0;
  attrs->pinvoke_fname    = 0;

  tmp = DECL_ATTRIBUTES (t);
  while (tmp)
    {
      const char *attr_name = IDENTIFIER_POINTER (TREE_PURPOSE (tmp));
      tree params = TREE_VALUE (tmp);

      if (strcmp (attr_name, "assembly_name") == 0)
        attrs->assembly_name = TREE_STRING_POINTER (TREE_VALUE (params));
      else if (strcmp (attr_name, "cil_name") == 0)
        attrs->cil_name = TREE_STRING_POINTER (TREE_VALUE (params));
      else if (strcmp (attr_name, "cil_strattr") == 0)
        attrs->cusattr_string = TREE_STRING_POINTER (TREE_VALUE (params));
      else if (strcmp (attr_name, "pinvoke") == 0)
        {
          attrs->pinvoke_assembly = TREE_STRING_POINTER (TREE_VALUE (params));
          if (TREE_CHAIN (params))
            attrs->pinvoke_fname = TREE_STRING_POINTER (TREE_VALUE (TREE_CHAIN (params)));
        }
      tmp = TREE_CHAIN (tmp);
    }
}

/* Mark the type represented by tree T as referenced.
   This function works recursively, since types referenced by type T
   itself are also marked as referenced.
   Referenced types are emitted at the end of the compilation unit,
   non-referenced types are not.
   T must be a type node.   */

static void
mark_referenced_type (tree t)
{
  if (t == NULL_TREE || t == error_mark_node)
    return;

  t = TYPE_MAIN_VARIANT (t);

  if (t == cil32_arg_iterator_type)
    return;

  /* If the type was already referenced, nothing else to do */
  if (pointer_set_contains (referenced_types, t))
    return;

  /* Give the aggregate a name unless it has it already */
  switch (TREE_CODE (t))
    {
    /* Incomplete and variable-length arrays are pointers and
       they must be dealt with as such.   */
    case ARRAY_TYPE:
      if (! TYPE_DOMAIN (t) || ARRAY_TYPE_VARLENGTH (t))
        break;
    case ENUMERAL_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      if (TYPE_NAME (t) == 0)
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

      tmp_name = (char *)xmalloc (tmp_name_max_len);
      tmp_name = append_string (tmp_name, orig_name,
                                &tmp_name_len, &tmp_name_max_len);
      tmp_name = append_string (tmp_name, suffix,
                                &tmp_name_len, &tmp_name_max_len);

      TYPE_NAME (t) = get_identifier_with_length (tmp_name, tmp_name_len);
      TYPE_CONTEXT (t) = 0;
      free (tmp_name);
    }

  switch (TREE_CODE (t))
    {
    case ENUMERAL_TYPE:
      pointer_set_insert (referenced_types, t);
      break;

    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      {
        tree tmp;

        pointer_set_insert (referenced_types, t);
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
      if (TYPE_DOMAIN (t) && ! ARRAY_TYPE_VARLENGTH (t))
        pointer_set_insert (referenced_types, t);
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

/* Mark the string represented by tree T as referenced.
   Referenced strings are emitted at the end of the compilation unit,
   non-referenced strings are not.
   T must be a STRING_CST node.   */

static void
mark_referenced_string (tree t)
{
  gcc_assert (TREE_CODE (t) == STRING_CST);
  if (!pointer_set_contains (referenced_string_ptrs,
                             (void *)(TREE_STRING_POINTER (t))))
    {
      VARRAY_PUSH_TREE (referenced_strings, t);
      pointer_set_insert (referenced_string_ptrs,
                          (void *)(TREE_STRING_POINTER (t)));
      mark_referenced_type (TREE_TYPE (t));
    }
}

/* Get an unique id for string constant NODE.   */

static
unsigned int get_string_cst_id (tree node)
{
  struct pointer_id_data tmp_pid;
  void **slot;

  gcc_assert (TREE_CODE (node) == STRING_CST);

  tmp_pid.ptr = TREE_STRING_POINTER (node);
  slot = htab_find_slot (pointer_id_htable, &tmp_pid, INSERT);

  if (*slot != NULL)
    return ((struct pointer_id_data *)(*slot))->id;
  else
    {
      struct pointer_id_data *pid = XNEW (struct pointer_id_data);

      *slot = pid;
      pid->ptr = TREE_STRING_POINTER (node);
      pid->id = pointer_next_id++;

      return pid->id;
    }
}

/* Mark the function represented by tree T as a pinvoke.
   T must be a FUNCTION_DECL node.   */

void
cil_add_pinvoke (tree t)
{
  gcc_assert (TREE_CODE (t) == FUNCTION_DECL);
  pointer_set_insert (referenced_pinvoke, t);
  mark_referenced_type (TREE_TYPE (t));
}

/* Dump the name of a _DECL node.  */

static void
dump_decl_name (FILE* file, tree node)
{
  gcc_assert (DECL_P (node));
  fputs ("'", file);

  mark_decl_referenced (node);

  if (DECL_ASSEMBLER_NAME_SET_P (node))
    fprintf (file, IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (node)));
  else if (DECL_NAME (node))
    fprintf (file, IDENTIFIER_POINTER (DECL_NAME (node)));
  else
    fprintf (file, "?UNNAMED%d", DECL_UID (node));

  fputs ("'", file);
}

/* Dump the name of a STRING_CST node.  */

static void
dump_string_name (FILE* file, tree node)
{
  gcc_assert (TREE_CODE (node) == STRING_CST);

  fprintf (file, "'?string%u'", get_string_cst_id (node));
}

/* Dump the name of a label.  */

static void
dump_label_name (FILE* file, tree node)
{
  /* Always print the label id. */
  fprintf (file, "?L" HOST_WIDE_INT_PRINT_DEC, LABEL_DECL_UID (node));

  /* For convenience, also print the identifier when available.  Note that the
     identifier alone is incorrect: in case of inlining, several labels can
     end up with the same id. */
  if (DECL_NAME (node))
    {
      fputs ("_", file);
      fprintf (file, IDENTIFIER_POINTER (DECL_NAME (node)));
    }
}

static void
dump_entry_label_name (FILE *stream, basic_block bb)
{
  block_stmt_iterator bsi = bsi_start (bb);
  if (!bsi_end_p (bsi))
    {
      tree stmt = bsi_stmt (bsi);
      if (TREE_CODE (stmt) == LABEL_EXPR)
        {
          tree op0 = TREE_OPERAND (stmt, 0);
          dump_label_name (stream, op0);
        }
      else
        {
          fprintf (stream, "<no label>");
        }
    }
  else
    {
      fprintf (stream, "<no basic block>");
    }
}

/* Dump the name of a valuetype.
   T must be an aggregate type or an enumeral type, since these are
   the types emitted as CIL valuetypes.   */

static void
dump_valuetype_name (FILE *file, tree t)
{
  tree type_name;

  gcc_assert (AGGREGATE_TYPE_P (t) || TREE_CODE (t) == ENUMERAL_TYPE);
  gcc_assert (TYPE_MAIN_VARIANT (t) == t);
  gcc_assert (TYPE_FILE_SCOPE_P (t));
  gcc_assert (TYPE_NAME (t));

  type_name = TYPE_NAME (t);
  gcc_assert (DECL_P (type_name) || TREE_CODE (type_name) == IDENTIFIER_NODE);

  fputs ("'", file);
  if (TREE_CODE (type_name) == IDENTIFIER_NODE)
    fprintf (file, IDENTIFIER_POINTER (type_name));
  else if (DECL_NAME (type_name))
    fprintf (file, IDENTIFIER_POINTER (DECL_NAME (type_name)));
  else
    fprintf (file, "?UNNAMED%d", DECL_UID (type_name));
  fputs ("'", file);
}

/* Dump the signature of function type FUN_TYPE.
   The function name that is dumped is taken from function_decl FUN
   or from NAME. Only and exactly one of the two must be non-null.
   REF tells whether the function type (and the types of the return value
   and of the incoming parameters) have to be marked as referenced.
   FUN_TYPE must be a FUNCTION_TYPE.
   FUN, if not null, must be a FUNCTION_DECL.   */

static void
dump_fun_type (FILE *stream, tree fun_type, tree fun, const char *name, bool ref)
{
  tree args_type;
  tree last_arg_type = NULL;
  bool varargs = FALSE;

  gcc_assert (! (fun && name));

  if (ref)
    mark_referenced_type (fun_type);

  args_type = TYPE_ARG_TYPES (fun_type);

  if (args_type == NULL)
    warning (OPT_Wcil_missing_prototypes,
             "Missing function %s prototype, guessing it, you should fix the code",
             fun?IDENTIFIER_POINTER (DECL_NAME (fun)):"");
  else
    {
      last_arg_type = args_type;
      while (TREE_CHAIN (last_arg_type))
        last_arg_type = TREE_CHAIN (last_arg_type);

      if (TREE_VALUE (last_arg_type) != void_type_node)
        {
          last_arg_type = NULL;
          varargs = TRUE;
        }
    }

  if (varargs)
    fputs ("vararg ", stream);

  dump_type (stream, TREE_TYPE (fun_type), ref, false);

  fputs (" ", stream);
  if (fun)
    {
      struct fnct_attr attrs;

      decode_function_attrs (fun, &attrs);

      if (attrs.assembly_name)
        fprintf (stream, "[%s]", attrs.assembly_name);
      else if (TARGET_EMIT_EXTERNAL_ASSEMBLY && DECL_EXTERNAL (fun))
        fputs ("[ExternalAssembly]ExternalAssembly::", stream);

      if (attrs.cil_name)
        fprintf (stream, "%s", attrs.cil_name);
      else
        dump_decl_name (stream, fun);
    }

  if (name)
    fputs (name, stream);

  fputs ("(", stream);

  while (args_type != last_arg_type)
    {
      dump_type (stream, TREE_VALUE (args_type), ref, true);
      args_type = TREE_CHAIN (args_type);

      if (args_type != last_arg_type)
        fputs (", ", stream);
    }

  if (varargs)
    fputs (", ...", stream);

  fputs (")", stream);
}

/* Dump type NODE.
   REF tells whether the function type (and the types of the return value
   and of the incoming parameters) have to be marked as referenced.
   QUALIF tells whether to emit C qualifiers (const, restrict, volatile)
   NODE must be a type node.   */

static void
dump_type (FILE *file, tree node, bool ref, bool qualif)
{
  if (node == NULL_TREE || node == error_mark_node)
    return;

/*   node = TYPE_MAIN_VARIANT (node); */

  if (TYPE_MAIN_VARIANT (node) == cil32_arg_iterator_type)
    {
      fputs ("valuetype [mscorlib]System.ArgIterator", file);
      return;
    }

  switch (TREE_CODE (node))
    {
    /* Incomplete and variable-length arrays are pointers and
       they must be dealt with as such.   */
    case ARRAY_TYPE:
      if (! TYPE_DOMAIN (node) || ARRAY_TYPE_VARLENGTH (node))
        goto pointer;
    case ENUMERAL_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      node = TYPE_MAIN_VARIANT (node);

      /* Reference the type if told to do so */
      if (ref)
        mark_referenced_type (node);

      /* Print the name of the structure.  */
      fputs ("valuetype ", file);
      dump_valuetype_name (file, node);
      break;

    case VOID_TYPE:
      fputs ("void", file);
      break;

    case INTEGER_TYPE:
      {
        int type_size = GET_MODE_BITSIZE (TYPE_MODE (node));

        if (TYPE_UNSIGNED (node))
          fputs ("unsigned ", file);

        switch (type_size)
          {
          case 8:  fputs ("int8",  file); break;
          case 16: fputs ("int16", file); break;
          case 32: fputs ("int32", file); break;
          case 64: fputs ("int64", file); break;
          default:
            fprintf (stderr, "Unsupported integer size %d\n", type_size);
            gcc_assert (0);
          }
      }
      break;

    case REAL_TYPE:
      {
        int type_size = TYPE_PRECISION (node);

        switch (type_size)
          {
          case 32: fputs ("float32", file); break;
          case 64: fputs ("float64", file); break;
          default:
            fprintf (stderr, "Unsupported floating point size %d\n", type_size);
            gcc_assert (0);
          }
      }
      break;

    case BOOLEAN_TYPE:
      fputs ("int8", file);
      break;

    pointer:
    case POINTER_TYPE:
      if (TREE_CODE (TREE_TYPE (node)) == FUNCTION_TYPE)
        {
          fputs ("method ", file);
          dump_fun_type (file, TREE_TYPE (node), NULL, " * ", ref);
        }
      else
        {
          dump_type (file, TREE_TYPE (node), ref, qualif);
          fputs (" *", file);
        }
      break;

    case FUNCTION_TYPE:
/*       dump_fun_type (file, node, NULL, NULL, ref); */
      gcc_assert (0);
      break;

    case VECTOR_TYPE:
      dump_vector_type (file, node, true);
      break;

    case COMPLEX_TYPE:
      dump_complex_type (file, node, true);
      break;

    case REFERENCE_TYPE:

    default:
      fprintf (stderr, "%s: %s\n", __func__, tree_code_name[TREE_CODE (node)]);
      gcc_assert (0);
      break;
    }

  if (qualif)
    {
      unsigned int quals = TYPE_QUALS (node);

      if (quals & TYPE_QUAL_CONST)
        fputs (" modopt([gcc4net]gcc4net.CQualifiers.IsConst)", file);
      if (quals & TYPE_QUAL_RESTRICT)
        fputs (" modopt([gcc4net]gcc4net.CQualifiers.IsRestrict)", file);
#if 0
      if (quals & TYPE_QUAL_VOLATILE)
        fputs (" modopt([gcc4net]gcc4net.CQualifiers.IsVolatile)", file);
#endif
    }

}


static void
dump_complex_type (FILE *file, tree node, bool full)
{
  int type_size = TYPE_PRECISION (TREE_TYPE (node));

  gcc_assert (TREE_CODE (node) == COMPLEX_TYPE);

  if (full)
    fputs ("valuetype [gcc4net]gcc4net.", file);

  fputs ("complex_", file);
  if (GET_MODE_CLASS (TYPE_MODE (TREE_TYPE (node))) == MODE_INT)
    {
      if (TYPE_UNSIGNED (node))
        fputs ("u", file);

      switch (type_size)
        {
        case 8: fputs ("char", file); break;
        case 16: fputs ("short", file); break;
        case 32: fputs ("int", file); break;
        case 64: fputs ("long", file); break;
        default:
          fprintf (stderr, "Unsupported complex size %d\n", type_size);
          gcc_assert (0);
        }
    }
  else
    {
      switch (type_size)
        {
        case 32: fputs ("float", file); break;
        case 64: fputs ("double", file); break;
        default:
          fprintf (stderr, "Unsupported complex size %d\n", type_size);
          gcc_assert (0);
        }
    }
}


/* Dump vector type NODE.

   FULL tells whether the type must be qualified with the 'valuetype' keyword,
   assembly and namespace.
   NODE must be a type node of type VECTOR_TYPE.   */

static void
dump_vector_type (FILE *file, tree node, bool full)
{
  int  vec_bitsize = TREE_INT_CST_LOW (TYPE_SIZE (node));
  tree innertype = TREE_TYPE (node);
  int  inner_bitsize = TYPE_PRECISION (innertype);
  enum machine_mode innermode = TYPE_MODE (innertype);

  gcc_assert (TREE_CODE (node) == VECTOR_TYPE);

  if (full)
    fputs ("valuetype [gcc4net]gcc4net.", file);

  fprintf (file, "V%d", vec_bitsize / inner_bitsize);

  if (GET_MODE_CLASS (innermode) == MODE_INT)
    {
      switch (inner_bitsize)
        {
        case 8:  fputs ("QI",  file); break;
        case 16: fputs ("HI", file); break;
        case 32: fputs ("SI", file); break;
        case 64: fputs ("DI", file); break;
        default:
          fprintf (stderr, "Unsupported integer size %d\n", inner_bitsize);
          gcc_assert (0);
        }
    }
  else if (GET_MODE_CLASS (innermode) == MODE_FLOAT)
    {
      switch (inner_bitsize)
        {
        case 32: fputs ("SF", file); break;
        case 64: fputs ("DF", file); break;
        default:
          fprintf (stderr, "Unsupported float size %d\n", inner_bitsize);
          gcc_assert (0);
        }
    }
  else
    {
      fprintf (stderr, "Unsupported mode class\n");
      gcc_assert (0);
    }

}


/* Dump type NODE, promoted following C conventions for var args.
   REF tells whether the function type (and the types of the return value
   and of the incoming parameters) have to be marked as referenced.
   NODE must be a type node.   */

static void
dump_type_promotion (FILE *stream, tree node, bool ref)
{
  if (node == NULL_TREE || node == error_mark_node)
    return;

  switch (TREE_CODE (node))
    {
    /* Incomplete and variable-length arrays are pointers and
       they must be dealt with as such.   */
    case ARRAY_TYPE:
      if (! TYPE_DOMAIN (node) || ARRAY_TYPE_VARLENGTH (node))
        goto pointer;
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      node = TYPE_MAIN_VARIANT (node);

      /* Reference the type if told to do so */
      if (ref)
        mark_referenced_type (node);

      /* Print the name of the structure.  */
      fputs ("valuetype ", stream);
      dump_valuetype_name (stream, node);
      break;

    case COMPLEX_TYPE:
      dump_complex_type (stream, node, true);
      break;

    case ENUMERAL_TYPE:
    case INTEGER_TYPE:
    case BOOLEAN_TYPE:
      {
        int type_size = TYPE_PRECISION (node);

        gcc_assert (type_size <= 64);

        if (type_size <= 32)
          fputs ("unsigned int32", stream);
        else
          fputs ("unsigned int64", stream);
      }
      break;

    case REAL_TYPE:
      fputs ("float64", stream);
      break;

    pointer:
    case POINTER_TYPE:
      /* cil32 is a 32bit machine, in case we support 64bit model
       * changes are needed
       */
      fputs ("unsigned int32", stream);
      break;

    default:
      fprintf (stderr, "%s: %s\n", __func__, tree_code_name[TREE_CODE (node)]);
      gcc_assert (0);
      break;
    }
}

/* Dump the type def of type NODE, promoted following C conventions
   for var args.
   NODE must be a type node.
   returns true iff the dumped type is a pointer */

static bool
dump_type_promoted_type_def (FILE *stream, tree node)
{
  bool result = false;

  if (node == NULL_TREE || node == error_mark_node)
    return false;

  switch (TREE_CODE (node))
    {
    /* Incomplete and variable-length arrays are pointers and
       they must be dealt with as such.   */
    case ARRAY_TYPE:
      if (! TYPE_DOMAIN (node) || ARRAY_TYPE_VARLENGTH (node))
        goto pointer;
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      node = TYPE_MAIN_VARIANT (node);

      /* Reference the type if told to do so */
      mark_referenced_type (node);

      /* Print the name of the structure.  */
      fputs ("valuetype ", stream);
      dump_valuetype_name (stream, node);
      break;

    case COMPLEX_TYPE:
      dump_complex_type (stream, node, true);
      break;

    case ENUMERAL_TYPE:
    case INTEGER_TYPE:
    case BOOLEAN_TYPE:
      {
        int type_size = TYPE_PRECISION (node);

        gcc_assert (type_size <= 64);

        if (type_size <= 32)
          fputs ("class [mscorlib]System.UInt32", stream);
        else
          fputs ("class [mscorlib]System.UInt64", stream);
      }
      break;

    case REAL_TYPE:
      fputs ("class [mscorlib]System.Double", stream);
      break;

    pointer:
    case POINTER_TYPE:
      /* cil32 is a 32bit machine, in case we support 64bit model
       * changes are needed
       */
      fputs ("class [mscorlib]System.UInt32", stream);
      result = true;
      break;

    default:
      fprintf (stderr, "%s: %s\n", __func__, tree_code_name[TREE_CODE (node)]);
      gcc_assert (0);
      break;
    }
  return result;
}

static void
dump_type_for_builtin (FILE *file, tree node, bool all_types)
{
  if (node == NULL_TREE || node == error_mark_node)
    return;

/*   node = TYPE_MAIN_VARIANT (node); */

  switch (TREE_CODE (node))
    {
    case ENUMERAL_TYPE:
    case INTEGER_TYPE:
    case BOOLEAN_TYPE:
      {
        int type_size = GET_MODE_BITSIZE (TYPE_MODE (node));

        if (TYPE_UNSIGNED (node))
          fputs ("unsigned ", file);

        switch (type_size)
          {
          case 8:  if (all_types) { fputs ("int8",  file); return; }
          case 16: if (all_types) { fputs ("int16", file); return; }
          case 32: fputs ("int32", file); break;
          case 64: fputs ("int64", file); break;
          default:
            fprintf (stderr, "Unsupported integer size %d\n", type_size);
            gcc_assert (0);
          }
      }
      break;

    case REAL_TYPE:
      {
        int type_size = TYPE_PRECISION (node);

        switch (type_size)
          {
          case 32: fputs ("float32", file); break;
          case 64: fputs ("float64", file); break;
          default:
            fprintf (stderr, "Unsupported floating point size %d\n", type_size);
            gcc_assert (0);
          }
      }
      break;

    default:
      fprintf (stderr, "%s: %s\n", __func__, tree_code_name[TREE_CODE (node)]);
      gcc_assert (0);
      break;
  }
}

static void
dump_type_eval_mode (FILE *stream, tree node, bool all_types, bool sign)
{
  if (node == NULL_TREE || node == error_mark_node)
    return;

  switch (TREE_CODE (node))
    {
    case ENUMERAL_TYPE:
    case INTEGER_TYPE:
    case BOOLEAN_TYPE:
      {
        int type_size = GET_MODE_BITSIZE (TYPE_MODE (node));

        if (sign && TYPE_UNSIGNED (node))
          fputs ("u", stream);

        switch (type_size)
          {
          case 8:  if (all_types) { fputs ("qi", stream); break; }
          case 16: if (all_types) { fputs ("hi", stream); break; }
          case 32: fputs ("si", stream); break;
          case 64: fputs ("di", stream); break;
          default:
            fprintf (stderr, "Unsupported integer size %d\n", type_size);
            gcc_assert (0);
          }
      }
      break;

    case REAL_TYPE:
      {
        int type_size = GET_MODE_BITSIZE (TYPE_MODE (node));

        switch (type_size)
          {
          case 32: fputs ("sf", stream); break;
          case 64: fputs ("df", stream); break;
          default:
            fprintf (stderr, "Unsupported floating point size %d\n",
                     type_size);
            gcc_assert (0);
          }
      }
      break;

    case VECTOR_TYPE:
      dump_vector_type (stream, node, false);
      break;

    default:
      fprintf (stderr, "%s: %s\n", __func__, tree_code_name[TREE_CODE (node)]);
      gcc_assert (0);
      break;
    }
}

static void
gen_addr_expr (FILE *file, tree t)
{
  if (t == NULL_TREE || t == error_mark_node)
    return;

  switch (TREE_CODE (t))
    {
    case STRING_CST:
      mark_referenced_string (t);
      fputs ("\n\tldsflda\t", file);
      dump_type (file, TREE_TYPE (t), true, false);
      fputs (" ", file);
      dump_string_name (file, t);
      stack_push (1);
      break;

    case VAR_DECL:
      if (!DECL_FILE_SCOPE_P (t))
        fputs ("\n\tldloca\t", file);
      else
        {
          fputs ("\n\tldsflda\t", file);
          if (COMPLETE_TYPE_P (TREE_TYPE (t)))
            dump_type (file, TREE_TYPE (t), true, false);
          else
            fputs ("native int", file);
          fputs (" ", file);
          if (TARGET_EMIT_EXTERNAL_ASSEMBLY && DECL_EXTERNAL (t))
            fputs ("[ExternalAssembly]ExternalAssembly::", file);
        }

      dump_decl_name (file, t);
      stack_push (1);
      break;

    case PARM_DECL:
      fputs ("\n\tldarga\t", file);
      dump_decl_name (file, t);
      stack_push (1);
      break;

    case FUNCTION_DECL:
      fputs ("\n\tldftn\t", file);
      dump_fun_type (file, TREE_TYPE (t), t, NULL, true);
      stack_push (1);
      break;

    case LABEL_DECL:
      {
        /* We cannot emit the address of the label in CIL, so we map each
          label to an ID and emit the ID.  The GOTO will then be implemented
          with a switch based on that ID.  The ID is simply the position in
          the list of all address taken labels. */

        int pos = 0;
        tree label_iter = addr_taken_labels;
        while (label_iter != t)
          {
            gcc_assert (label_iter);
            ++pos;
            label_iter = TREE_CHAIN (label_iter);
          }
        fprintf (file, "\n\tldc.i4\t%d", pos);
        stack_push (1);
      }
      break;

    case INDIRECT_REF:
      gen_cil_node (file, TREE_OPERAND (t, 0));
      break;

    case ARRAY_REF:
      {
        tree node = t;

        do {
          gcc_assert (integer_zerop (TREE_OPERAND (node, 1)));
          node = TREE_OPERAND (node, 0);
        } while (TREE_CODE (node) == ARRAY_REF);

        gen_addr_expr (file, node);
      }
      break;

    case COMPONENT_REF:
      {
        tree obj = TREE_OPERAND (t, 0);
        tree fld = TREE_OPERAND (t, 1);
        tree obj_type = TYPE_MAIN_VARIANT (TREE_TYPE (obj));

        gcc_assert (! DECL_BIT_FIELD (fld));

        gen_addr_expr (file, obj);
        fputs ("\n\tldflda\t", file);
        dump_type (file, TREE_TYPE (fld), true, false);
        fputs (" ", file);
        mark_referenced_type (obj_type);
        dump_valuetype_name (file, obj_type);
        fputs ("::", file);
        dump_decl_name (file, fld);
      }
      break;

    default:
      fprintf (stderr, "%s: [7m%s[m\n",
               __func__, tree_code_name[TREE_CODE (t)]);
      gcc_assert (0);
    }

  if (TREE_CODE (TREE_TYPE (t)) == ARRAY_TYPE)
    fputs ("\n\tconv.i", file);
}

static void
print_type_suffix (FILE *file, tree type_node, bool unsign)
{
  switch (TREE_CODE (type_node))
    {
    case ENUMERAL_TYPE:
    case INTEGER_TYPE:
      {
        int type_size = TYPE_PRECISION (type_node);
        bool uns = unsign && TYPE_UNSIGNED (type_node);

        switch (type_size)
          {
          case 8:  fputs ((uns)?"u1":"i1", file); break;
          case 16: fputs ((uns)?"u2":"i2", file); break;
          case 32: fputs ((uns)?"u4":"i4", file); break;
          case 64: fputs ((uns)?"u8":"i8", file); break;
          default:
            fprintf (stderr, "Unsupported integer size %d\n", type_size);
            gcc_assert (0);
            break;
          }
      }
      break;

    case BOOLEAN_TYPE:
      fputs ((unsign && TYPE_UNSIGNED (type_node))?"u1":"i1", file);
      break;

    case REAL_TYPE:
      {
        int type_size = TYPE_PRECISION (type_node);

        switch (type_size)
          {
          case 32: fputs ("r4", file); break;
          case 64: fputs ("r8", file); break;
          default:
            fprintf (stderr, "Unsupported floating point size %d\n", type_size);
            gcc_assert (0);
            break;
          }
      }
      break;

    case POINTER_TYPE:
      fputs ("i", file);
      break;

    case VECTOR_TYPE:
      {
        int type_size = TREE_INT_CST_LOW (TYPE_SIZE (type_node));
        bool uns = TYPE_UNSIGNED (type_node) && unsign;
        tree innertype = TREE_TYPE (type_node);
        enum machine_mode innermode = TYPE_MODE (innertype);

        /* Only expect integer vectors */
        gcc_assert (GET_MODE_CLASS (innermode) == MODE_INT);

        switch (type_size)
          {
          case 8:  fputs ((uns)?"u1":"i1", file); break;
          case 16: fputs ((uns)?"u2":"i2", file); break;
          case 32: fputs ((uns)?"u4":"i4", file); break;
          case 64: fputs ((uns)?"u8":"i8", file); break;
          default:
            fprintf (stderr, "Unsupported integer size %d\n", type_size);
            gcc_assert (0);
            break;
          }
        break;
      }

    default:
      internal_error ("print_type_suffix %s\n",
                      tree_code_name[TREE_CODE (type_node)]);
      gcc_assert (0);
      break;
    }
}

/* Emit a conversion from type IN_TYPE to type OUT_TYPE to file FILE.
   IS_NOP says whether the conversion comes from a NOP_EXPR.   */

static void
gen_conv (FILE *file, bool is_nop, tree out_type, tree in_type)
{
  if (is_nop
      && INTEGRAL_TYPE_P (out_type)
      && INTEGRAL_TYPE_P (in_type))
    {
      if (TYPE_PRECISION (out_type) > TYPE_PRECISION (in_type))
        {
          tree tmp_type = TYPE_UNSIGNED (in_type)
                          ? unsigned_type_for (out_type)
                          : signed_type_for (out_type);

          gen_integral_conv (file, tmp_type, in_type);
          gen_integral_conv (file, out_type, tmp_type);
        }
      else
        gen_integral_conv (file, out_type, in_type);
    }

  /* Special case for conversion to float type are not orthogonal
     in CIL opcode set.   */
  else if (SCALAR_FLOAT_TYPE_P (out_type)
           && INTEGRAL_TYPE_P (in_type)
           && TYPE_UNSIGNED (in_type))
    {
      fputs ("\n\tconv.r.un", file);

      if (TYPE_PRECISION (out_type) <= 32)
        fputs ("\n\tconv.r4", file);
    }

  /* Do nothing for a conversion from two REAL_TYPEs with the
     same precision.   */
  else if (! SCALAR_FLOAT_TYPE_P (out_type)
           || ! SCALAR_FLOAT_TYPE_P (in_type)
           || TYPE_PRECISION (out_type) != TYPE_PRECISION (in_type))
    {
      fputs ("\n\tconv.", file);
      print_type_suffix (file, out_type, true);
    }
}

/* Emit a conversion from integral or pointer type IN_TYPE
   to integral type OUT_TYPE to file FILE.
   If the precision of OUT_TYPE is bigger than that of IN_TYPE,
   then IN_TYPE and OUT_TYPE have to have the same signedness.   */

static void
gen_integral_conv (FILE *file, tree out_type, tree in_type)
{
  unsigned int out_bits, cont_size, in_bits;

  gcc_assert (INTEGRAL_TYPE_P (out_type));
  gcc_assert (INTEGRAL_TYPE_P (in_type) || POINTER_TYPE_P (in_type));
  gcc_assert (TYPE_PRECISION (out_type) <= 64);
  gcc_assert (TYPE_PRECISION (out_type) <= TYPE_PRECISION (in_type)
              || TYPE_UNSIGNED (out_type) == TYPE_UNSIGNED (in_type));

  /* Get the precision of the output and input types and the size
     of the output type container */
  in_bits = TYPE_PRECISION (in_type);
  out_bits = TYPE_PRECISION (out_type);
  cont_size = GET_MODE_BITSIZE (TYPE_MODE (out_type));
  gcc_assert (cont_size >= out_bits);

  /* Dump a conv with for the container size, if not superfluous */
  if ((cont_size == out_bits && (out_bits != in_bits || out_bits < 32))
      || ((out_bits > 32) ^ (in_bits > 32)))
    {
      fputs ("\n\tconv.", file);
      print_type_suffix (file, get_integer_type (cont_size,
                                                 TYPE_UNSIGNED (out_type)),
                                                 true);
    }

  /* If the container is bigger than the output type precision,
     force the output to be of the desired precision.   */
  if (cont_size > out_bits)
    {
      if (TYPE_UNSIGNED (out_type))
        {
          unsigned HOST_WIDEST_INT mask;
          double_int mask_di;

          /* Compute the mask to be applied to the existing value */
          gcc_assert (HOST_BITS_PER_WIDEST_INT >= 64);
          mask = (1LL << out_bits) - 1LL;
          mask_di.low = mask;
          mask_di.high = (HOST_WIDE_INT)(mask >> HOST_BITS_PER_WIDE_INT);

          /* Apply the mask */
          if (out_bits <= 32)
            fputs ("\n\tldc.i4\t", file);
          else
            fputs ("\n\tldc.i8\t", file);
          dump_double_int (file, mask_di, false);
          fputs ("\n\tand", file);
        }
      else
        {
          unsigned int cont_size = (out_bits > 32) ? 64 : 32;
          unsigned int shift = cont_size - out_bits;

          /* Do a pair of shift to perform the sign extension */
          fprintf (file, "\n\tldc.i4\t%d", shift);
          fputs ("\n\tshl\t", file);
          fprintf (file, "\n\tldc.i4\t%d", shift);
          fputs ("\n\tshr\t", file);
        }

      stack_push (1);
      stack_pop (1);
    }
}

static void
gen_binary_cond_branch (FILE *file, tree node, tree label)
{
  tree op0 = TREE_OPERAND (node, 0);
  tree op1 = TREE_OPERAND (node, 1);
  bool is_unsigned = TYPE_UNSIGNED (TREE_TYPE (op0));

  gcc_assert (TREE_CODE (label) == LABEL_DECL);

  if (TREE_CODE (node) == UNORDERED_EXPR
      || TREE_CODE (node) == ORDERED_EXPR)
    {
      gen_cil_node (file, op0);
      fputs ("\n\tdup"
             "\n\tceq", file);

      stack_push (1);
      stack_pop (1);

      gen_cil_node (file, op1);
      fputs ("\n\tdup"
             "\n\tceq"
             "\n\tand"
             "\n\tbr", file);

      if (TREE_CODE (node) == UNORDERED_EXPR)
        fputs ("false\t", file);
      else
        fputs ("true\t", file);

      dump_label_name (file, label);

      stack_push (1);
      stack_pop (3);

      return;
    }

  gen_cil_node (file, op0);
  gen_cil_node (file, op1);

  switch (TREE_CODE (node))
    {
    case LE_EXPR:   fputs ("\n\tble", file); break;
    case LT_EXPR:   fputs ("\n\tblt", file); break;
    case GE_EXPR:   fputs ("\n\tbge", file); break;
    case GT_EXPR:   fputs ("\n\tbgt", file); break;
    case EQ_EXPR:   fputs ("\n\tbeq", file); is_unsigned = FALSE; break;
    case NE_EXPR:   fputs ("\n\tbne", file); is_unsigned = TRUE;  break;
    case UNLT_EXPR: fputs ("\n\tblt", file); is_unsigned = TRUE;  break;
    case UNLE_EXPR: fputs ("\n\tble", file); is_unsigned = TRUE;  break;
    case UNGT_EXPR: fputs ("\n\tbgt", file); is_unsigned = TRUE;  break;
    case UNGE_EXPR: fputs ("\n\tbge", file); is_unsigned = TRUE;  break;
    default:
      gcc_unreachable ();
    }

  if (is_unsigned)
    fputs (".un\t", file);
  else
    fputs ("\t", file);
  dump_label_name (file, label);

  stack_pop (2);
}

/* Dump a CALL_EXPR gimple node NODE in CIL on the file FILE. */

static void
gen_call_expr (FILE *file, tree node)
{
  bool done = false;
  tree fdecl;
  tree fun_expr;
  tree fun_type;
  bool direct_call;
  tree static_chain;
  tree args;
  tree args_type;
  tree last_arg_type;
  bool varargs;
  int promote_types = 0;
  unsigned int aidx;
  unsigned int nargs;

  gcc_assert(TREE_CODE (node) == CALL_EXPR);

  fun_expr = get_callee_fndecl (node);
  if (fun_expr)
    {
      fdecl = fun_expr;
      fun_type = TREE_TYPE (fun_expr);
      direct_call = true;
    }
  else
    {
      fun_expr = CALL_EXPR_FN (node);
      fdecl    = NULL_TREE;
      fun_type = TREE_TYPE (TREE_TYPE (fun_expr));
      direct_call = false;
    }
  nargs = call_expr_nargs(node);

  /* Built-in functions must be handled in a special way */
  if (direct_call && DECL_BUILT_IN (fdecl))
    {

      if (DECL_BUILT_IN_CLASS (fdecl) == BUILT_IN_MD)
        {
          switch (DECL_FUNCTION_CODE (fdecl))
            {
            case CIL32_BUILT_IN_VA_START:
              {
                tree va = CALL_EXPR_ARG (node, 0);

                gcc_assert (POINTER_TYPE_P (TREE_TYPE (va))
                            && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (va)))
                               == cil32_arg_iterator_type);

                gen_cil_node (file, va);
                fputs ("\n\tdup"
                       "\n\tinitobj\tvaluetype [mscorlib]System.ArgIterator"
                       "\n\targlist"
                       "\n\tcall\tinstance void "
                       "[mscorlib]System.ArgIterator::.ctor(valuetype "
                       "[mscorlib]System.RuntimeArgumentHandle)",
                       file);

                stack_push (1);
                stack_pop (2);
                done = true;
              }
              break;

            case CIL32_BUILT_IN_VA_ARG:
              {
                tree va    = CALL_EXPR_ARG (node, 0);
                tree dummy = CALL_EXPR_ARG (node, 1);
                tree type  = TREE_TYPE (TREE_TYPE (dummy));

                gcc_assert (POINTER_TYPE_P (TREE_TYPE (va))
                            && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (va)))
                               == cil32_arg_iterator_type);

                gen_cil_node (file, va);
                fputs ("\n\tcall\tinstance typedref [mscorlib]System.ArgIterator::GetNextArg()"
                       "\n\trefanyval ",
                       file);

                if (dump_type_promoted_type_def (file, type))
                  fputs ("\n\tconv.i", file);

                done = true;
              }
              break;

            case CIL32_BUILT_IN_VA_END:
              {
                tree va = CALL_EXPR_ARG (node, 0);

                gcc_assert (POINTER_TYPE_P (TREE_TYPE (va))
                            && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (va)))
                               == cil32_arg_iterator_type);

                gen_cil_node (file, va);
                fputs ("\n\tcall\tinstance void [mscorlib]System.ArgIterator::End()",
                       file);

                stack_pop (1);
                done = true;
              }
              break;

            case CIL32_BUILT_IN_VA_COPY:
              {
                tree va_dest = CALL_EXPR_ARG (node, 0);
                tree va_src  = CALL_EXPR_ARG (node, 1);

                gcc_assert (POINTER_TYPE_P (TREE_TYPE (va_dest))
                            && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (va_dest)))
                               == cil32_arg_iterator_type);
                gcc_assert (POINTER_TYPE_P (TREE_TYPE (va_src))
                            && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (va_src)))
                               == cil32_arg_iterator_type);

                gen_cil_node (file, va_dest);
                fputs ("\n\tdup"
                       "\n\tinitobj\tvaluetype [mscorlib]System.ArgIterator",
                       file);
                stack_push (1);
                stack_pop (1);

                gen_cil_node (file, va_src);
                fputs ("\n\tcpobj\t[mscorlib]System.ArgIterator", file);

                stack_pop (2);
                done = true;
              }
              break;

            case CIL32_BUILT_IN_IS_LITTLE_ENDIAN:
              {
                fputs ("\n\tcall\tbool [gcc4net]gcc4net.Crt::__isLittleEndian()",
                       file);

                stack_push (1);
                done = true;
              }
              break;

            case CIL32_V2SF_CTOR:
            case CIL32_V4SF_CTOR:
            case CIL32_V4QI_CTOR:
            case CIL32_V2HI_CTOR:
            case CIL32_V8QI_CTOR:
            case CIL32_V4HI_CTOR:
            case CIL32_V2SI_CTOR:
            case CIL32_V4SI_CTOR:
            case CIL32_V8HI_CTOR:
            case CIL32_V16QI_CTOR:
              {
                unsigned int i;
                tree arg;
                call_expr_arg_iterator iter;
                tree arg_type;
                tree result = TREE_TYPE (fun_type);

                /* the type should be the same for all args. */
                arg_type = TREE_TYPE (CALL_EXPR_ARG (node, 0));
                FOR_EACH_CALL_EXPR_ARG (arg, iter, node)
                  gen_cil_node (file, arg);

                fputs ("\n\tcall ", file);
                dump_vector_type (file, result, true);
                fputs (" [gcc4net]gcc4net.", file);
                dump_vector_type (file, result, false);
                fputs ("::", file);
                dump_decl_name (file, fdecl);
                fputs ("(", file);

                /* emit the same type 'nargs' times */
                for(i = 0; i < nargs; ++i)
                  {
                    if (i)
                      fputs (", ", file);

                    if (TREE_CODE (arg_type) == INTEGER_TYPE
                        && !TYPE_UNSIGNED (arg_type))
                      fputs ("unsigned ", file);

                    dump_type (file, arg_type, false, false);
                  }
                fputs (")", file);

                stack_pop (nargs-1);
                done = true;
                break;
              }

            default:
              gcc_assert (0);
            }
        }
      else
        {
          switch (DECL_FUNCTION_CODE (fdecl))
            {
            case BUILT_IN_VA_START:
            case BUILT_IN_VA_END:
            case BUILT_IN_VA_COPY:
              gcc_assert (0);
              break;

            case BUILT_IN_INIT_TRAMPOLINE:
            case BUILT_IN_ADJUST_TRAMPOLINE:
            case BUILT_IN_NONLOCAL_GOTO:
              internal_error ("Builtins to support Trampolines not implemented\n");
              break;

            case BUILT_IN_PROFILE_FUNC_ENTER:
            case BUILT_IN_PROFILE_FUNC_EXIT:
              internal_error ("Builtins to support Profiling not implemented\n");
              break;

            case BUILT_IN_SETJMP_SETUP:
            case BUILT_IN_SETJMP_DISPATCHER:
            case BUILT_IN_SETJMP_RECEIVER:
              internal_error ("Builtins to support Setjump not implemented\n");
              break;

            case BUILT_IN_MEMSET:
              {
                tree ptr   = CALL_EXPR_ARG (node, 0);
                tree value = CALL_EXPR_ARG (node, 1);
                tree size  = CALL_EXPR_ARG (node, 2);

                gen_cil_node (file, ptr);
                fputs ("\n\tdup", file);
                stack_push (1);

                gen_cil_node (file, value);
                gen_cil_node (file, size);

                fputs ("\n\tunaligned. 1"
                       "\n\tinitblk", file);
                stack_pop (3);
                done = true;
              }
              break;

            case BUILT_IN_MEMCPY:
              {
                tree ptr_dst = CALL_EXPR_ARG (node, 0);
                tree ptr_src = CALL_EXPR_ARG (node, 1);
                tree size    = CALL_EXPR_ARG (node, 2);

                gen_cil_node (file, ptr_dst);
                fputs ("\n\tdup", file);
                stack_push (1);

                gen_cil_node (file, ptr_src);
                gen_cil_node (file, size);

                fputs ("\n\tunaligned. 1"
                       "\n\tcpblk", file);
                stack_pop (3);
                done = true;
              }
              break;

            case BUILT_IN_ALLOCA:
              {
                tree size = CALL_EXPR_ARG (node, 0);

                gen_cil_node (file, size);
                fputs ("\n\tlocalloc", file);

                done = true;
              }
              break;

            case BUILT_IN_STACK_SAVE:
              /* This built-in is only used for the implementation
                 of variable-length arrays.
                 It's not needed in CIL.   */
              fputs ("\n\tldc.i4.0"
                     "\n\tconv.i", file);
              stack_push (1);
              done = true;
              break;

            case BUILT_IN_STACK_RESTORE:
              /* This built-in is only used for the implementation
                 of variable-length arrays.
                 It's not needed in CIL.   */
              done = true;
              break;

            case BUILT_IN_EXPECT:
              /* __builtin_expect(exp,val) evalutes exp and tells the
                 compiler that it most likely gives val.  We just
                 evaluate exp.  */
              gen_cil_node (file, CALL_EXPR_ARG (node, 0));
              done = true;
              break;

            case BUILT_IN_TRAP:
              fputs ("\n\tcall\tvoid 'abort' ()", file);
              done = true;
              break;

            case BUILT_IN_CLZ:
            case BUILT_IN_CLZL:
            case BUILT_IN_CLZLL:
            case BUILT_IN_CTZ:
            case BUILT_IN_CTZL:
            case BUILT_IN_CTZLL:
            case BUILT_IN_FFS:
            case BUILT_IN_FFSL:
            case BUILT_IN_FFSLL:
            case BUILT_IN_PARITY:
            case BUILT_IN_PARITYL:
            case BUILT_IN_PARITYLL:
            case BUILT_IN_POPCOUNT:
            case BUILT_IN_POPCOUNTL:
            case BUILT_IN_POPCOUNTLL:
              {
                tree fun_arg_types = TYPE_ARG_TYPES (fun_type);
                tree arg = CALL_EXPR_ARG (node, 0);
                tree arg_type = TREE_VALUE (fun_arg_types);
                gen_cil_node (file, arg);
                fputs ("\n\tcall\tint32 [gcc4net]gcc4net.Crt::__", file);
                switch (DECL_FUNCTION_CODE (fdecl))
                  {
                  case BUILT_IN_CLZ:
                  case BUILT_IN_CLZL:
                  case BUILT_IN_CLZLL:
                    fputs ("clz", file);
                    break;
                  case BUILT_IN_CTZ:
                  case BUILT_IN_CTZL:
                  case BUILT_IN_CTZLL:
                    fputs ("ctz", file);
                    break;
                  case BUILT_IN_FFS:
                  case BUILT_IN_FFSL:
                  case BUILT_IN_FFSLL:
                    fputs ("ffs", file);
                    break;
                  case BUILT_IN_PARITY:
                  case BUILT_IN_PARITYL:
                  case BUILT_IN_PARITYLL:
                    fputs ("parity", file);
                    break;
                  case BUILT_IN_POPCOUNT:
                  case BUILT_IN_POPCOUNTL:
                  case BUILT_IN_POPCOUNTLL:
                    fputs ("popcount", file);
                    break;
                  default:
                    gcc_assert (0);
                    break;
                  }
                dump_type_eval_mode (file, arg_type, false, false);
                fputs ("2(", file);
                dump_type_for_builtin (file, arg_type, false);
                fputs (")", file);
              }
              done = true;
              break;

            default:
              if (DECL_ASSEMBLER_NAME_SET_P (node))
                {
                  /* Go Ahead as a normal function call */
                }
/*               else */
/*                 { */
/*                   fprintf (stderr, */
/*                            "unsupported builtin: %s\n", */
/*                            IDENTIFIER_POINTER (DECL_NAME (fdecl))); */
/*                   gcc_assert (0); */
/*                 } */
            }
        }
    }

  if (!done)
    {
      /* Print parameters.  */

      /* If static Chain present, it will be the first argument */
      static_chain = CALL_EXPR_STATIC_CHAIN(node);
      if (static_chain)
        gen_cil_node (file, static_chain);

      for (aidx=0;aidx<nargs;++aidx)
        gen_cil_node (file, CALL_EXPR_ARG (node, aidx));

      /* Print function pointer, in case of indirect call */
      if (!direct_call)
        gen_cil_node (file, fun_expr);

#if 0
      if (0 && CALL_EXPR_RETURN_SLOT_OPT (node))
        fprintf (file, " [return slot optimization]");
#endif

      fprintf (file, "\n\t");
#if 0
      if (CALL_EXPR_TAILCALL (node))
        fprintf (file, "tail.");
#endif
      if (direct_call)
        fputs ("call\t", file);
      else
        fputs ("calli\t", file);

      args_type = TYPE_ARG_TYPES (fun_type);
      last_arg_type = 0;
      varargs = FALSE;

      if (args_type == NULL)
        {
          if (direct_call)
            warning (OPT_Wcil_missing_prototypes,
                     "Missing function %s prototype, guessing it, "
                     "you should fix the code",
                     IDENTIFIER_POINTER (DECL_NAME (fdecl)));
          else
            warning (OPT_Wcil_missing_prototypes,
                     "Missing indirect function prototype, guessing it, "
                     "you should fix the code");
          /* Guess types using the type of the arguments */
          promote_types = 1;
        }
      else
        {
          last_arg_type = tree_last (args_type);

          if (TREE_VALUE (last_arg_type) != void_type_node)
            {
              last_arg_type = 0;
              varargs = TRUE;
            }
        }

      if (varargs)
        fputs ("vararg ", file);

      dump_type (file, TREE_TYPE (fun_type), true, false);

      if (direct_call)
        {
          struct fnct_attr attrs;
          decode_function_attrs (fdecl, &attrs);

          fputs (" ", file);

          if (attrs.assembly_name)
            fprintf (file, "[%s]", attrs.assembly_name);
          else if (TARGET_EMIT_EXTERNAL_ASSEMBLY && DECL_EXTERNAL (fdecl))
            fputs ("[ExternalAssembly]ExternalAssembly::", file);

          if (attrs.cil_name)
            fprintf (file, "%s", attrs.cil_name);
          else
            dump_decl_name (file, fdecl);
        }

      fputs (" (", file);
      args = TREE_OPERAND (node, 1);

      if (static_chain)
        {
          dump_type (file, TREE_TYPE (static_chain), true, true);
          if (args_type != last_arg_type)
            fputs (", ", file);
        }

      for (aidx=0;aidx<nargs;++aidx)
        {
          if (aidx!=0)
            {
              if (!promote_types && varargs && args_type == last_arg_type)
                {
                  fputs (", ..., ", file);
                  promote_types = 1;
                }
              else
                fputs (", ", file);
            }

          if (promote_types)
            {
              dump_type_promotion (file, TREE_TYPE (CALL_EXPR_ARG (node, aidx)), true);
            }
          else
            {
              dump_type (file, TREE_VALUE (args_type), true, true);
              args_type = TREE_CHAIN (args_type);
            }
        }

      fputs (")", file);

      if (!direct_call)
        ++nargs;
      if (static_chain)
        ++nargs;
      stack_pop (nargs);

      if (TREE_CODE (TREE_TYPE (fun_type)) != VOID_TYPE)
        stack_push (1);
    }
}

/* Dump the node NODE in CIL on the file FILE. */

static void
gen_cil_node (FILE *file, tree node)
{
  tree op0, op1;

  if (node == NULL_TREE || node == error_mark_node)
    return;

  if (TARGET_EMIT_GIMPLE_COMMENTS && EXPR_HAS_LOCATION (node))
    {
      expanded_location xloc = expand_location (EXPR_LOCATION (node));
      if (xloc.file)
        fprintf (file, "\n\t/* [file:%s,line:%d] */", xloc.file, xloc.line);
      else
        fprintf (file, "\n\t/* [line:%d] */", xloc.line);
    }

  switch (TREE_CODE (node))
    {
    case INTEGER_CST:
      {
        int type_size = TYPE_PRECISION (TREE_TYPE (node));

        gcc_assert (type_size <= 64);
        if (type_size <= 32)
          fputs ("\n\tldc.i4\t", file);
        else
          fputs ("\n\tldc.i8\t", file);
        dump_double_int (file, TREE_INT_CST (node), false);

        if (POINTER_TYPE_P (TREE_TYPE (node)))
          fputs ("\n\tconv.i", file);

        stack_push (1);
      }
      break;

    case COMPLEX_CST:
      {
        tree cplx_type = TREE_TYPE (node);
        tree re = TREE_REALPART (node);
        tree im = TREE_IMAGPART (node);

        gen_cil_node (file, re);
        gen_cil_node (file, im);
        fputs ("\n\tcall ", file);
        dump_complex_type (file, cplx_type, true);
        fputs (" [gcc4net]gcc4net.", file);
        dump_complex_type(file, cplx_type, false);
        fputs ("::", file);
        dump_complex_type(file, cplx_type, false);
        fputs ("_ctor (", file);
        dump_type (file, TREE_TYPE (re), false, false);
        fputs (", ", file);
        dump_type (file, TREE_TYPE (re), false, false);
        fputs (")", file);
        stack_pop (2);
        stack_push (1);
      }
      break;

    case REAL_CST:
      {
        REAL_VALUE_TYPE d;
        tree type_tree;
        int type_size;
        enum machine_mode mode;
        char string[100];
        /* we have 32 and 64 bit reals, real_to_format fills 32 bits per long */
        long  buf[2];

        d = TREE_REAL_CST (node);
        type_tree = TREE_TYPE (node);
        mode = TYPE_MODE (type_tree);
        real_to_target (buf, &d, mode);
        real_to_decimal (string, &d, sizeof (string), 5, 1);
        type_size = TYPE_PRECISION (type_tree);

        switch (type_size)
          {
          case 32:
            fprintf (file, "\n\tldc.r4\tfloat32(%#08lx)", buf[0]);
            break;

          case 64:
            fprintf (file, "\n\tldc.r8\tfloat64(%#08lx%08lx)", buf[1], buf[0]);
            break;

          default:
            internal_error ("\nldc.r: unsupported float size %d\n", type_size);
            break;
          }
        fprintf (file, "  /* %s */", string);
        stack_push (1);
        break;
      }

    case VECTOR_CST:
      {
        int  num_elt = 0;
        int  i;
        tree elt;
        tree vector_type = TREE_TYPE (node);
        int  vector_bitsize = TREE_INT_CST_LOW (TYPE_SIZE (vector_type));
        tree unit_type = TREE_TYPE (vector_type);
        int  unit_bitsize = TYPE_PRECISION (unit_type);

        gcc_assert (HOST_BITS_PER_WIDEST_INT >= 64);

        for (elt = TREE_VECTOR_CST_ELTS (node); elt; elt = TREE_CHAIN (elt))
          {
            tree elt_val = TREE_VALUE (elt);
            gen_cil_node (file, elt_val);
            ++num_elt;
          }
        /* Fill in the missing initializers, if any */
        for ( ; num_elt < vector_bitsize/unit_bitsize; ++num_elt)
          {
            if (GET_MODE_CLASS (TYPE_MODE (unit_type)) == MODE_INT)
            {
              fputs ("\n\tldc.i4.0", file);
            }
            else if (GET_MODE_CLASS (TYPE_MODE (unit_type)) == MODE_FLOAT)
            {
              fputs ("\n\tldc.r4 0.0", file);
            }
            else
              internal_error ("\n\nDo not know how to initialize vector\n");

            stack_push (1);
          }
        fputs ("\n\tcall ", file);
        dump_type(file, vector_type, false, false);
        fputs (" [gcc4net]gcc4net.", file);
        dump_vector_type(file, vector_type, false);
        fputs ("::", file);
        dump_vector_type(file, vector_type, false);
        fputs ("_ctor1(", file);
        for (i=0; i < num_elt; ++i)
          {
            if (i)
              fputs (", ", file);
            if ((GET_MODE_CLASS (TYPE_MODE (unit_type)) == MODE_INT) &&
                (!TYPE_UNSIGNED (unit_type)))
            {
              fputs ("unsigned ", file);
            }

            dump_type (file, unit_type, false, false);
          }
        fputs (")", file);
        stack_pop (num_elt);
        stack_push (1);
        break;
      }

    case LABEL_DECL:
      internal_error ("use dump_label_name instead of gen_cil_node\n");
      break;

    case INIT_EXPR:
    case MODIFY_EXPR:
    case GIMPLE_MODIFY_STMT:
      gen_cil_modify_expr (file,
                           GENERIC_TREE_OPERAND (node, 0),
                           GENERIC_TREE_OPERAND (node, 1));
      break;

    case GOTO_EXPR:
      {
        tree label_decl = GOTO_DESTINATION (node);
        tree label_iter = addr_taken_labels;

        if (TREE_CODE (label_decl) != LABEL_DECL)
          {
            /* This is a goto to the address of a label. Labels have been
               numbered, and we emit a switch based on that ID. */
            gen_cil_node (file, label_decl);
            fputs ("\n\tswitch (", file);
            while (label_iter)
              {
                dump_label_name (file, label_iter);
                label_iter = TREE_CHAIN (label_iter);
                if (label_iter)
                  fputs (", ", file);
              }
            fputs (")", file);
            stack_pop (1);
          }
        else
          {
            basic_block dest_bb = label_to_block (label_decl);

            if (bb->next_bb != dest_bb)
              {
                fputs ("\n\tbr\t", file);
                dump_label_name (file, label_decl);
              }
          }
        gcc_assert (stack == 0);
      }
      break;

    case COND_EXPR:
      {
        tree label_then, label_else;

        if (COND_EXPR_THEN (node)
            && simple_goto_p (COND_EXPR_THEN (node)))
          {
            /* At this point, both clauses of COND_EXPR must contain simple gotos */
            gcc_assert (COND_EXPR_ELSE (node)
                        && simple_goto_p (COND_EXPR_ELSE (node)));
            label_then = GOTO_DESTINATION (COND_EXPR_THEN (node));
            label_else = GOTO_DESTINATION (COND_EXPR_ELSE (node));
          }
        else
          {
            edge true_edge;
            edge false_edge;
            extract_true_false_edges_from_block (bb, &true_edge, &false_edge);
            label_then = tree_block_label (true_edge->dest);
            label_else = tree_block_label (false_edge->dest);
          }

        gcc_assert (label_then != NULL_TREE && label_else != NULL_TREE);

        op0 = COND_EXPR_COND (node);
        if (TREE_CODE (op0) == EQ_EXPR
            || TREE_CODE (op0) == NE_EXPR)
          {
            tree op00 = TREE_OPERAND (op0, 0);
            tree op01 = TREE_OPERAND (op0, 1);
            tree cond_type = TREE_TYPE (op00);

            if ((INTEGRAL_TYPE_P (cond_type) || POINTER_TYPE_P (cond_type))
                && ((TREE_CODE (op00) == INTEGER_CST
                     && double_int_zero_p (TREE_INT_CST (op00)))
                    || (TREE_CODE (op01) == INTEGER_CST
                        && double_int_zero_p (TREE_INT_CST (op01)))))
              {
                if ((TREE_CODE (op00) == INTEGER_CST
                     && double_int_zero_p (TREE_INT_CST (op00))))
                  gen_cil_node (file, op01);
                else
                  gen_cil_node (file, op00);

                if (TREE_CODE (op0) == EQ_EXPR)
                  fputs ("\n\tbrfalse\t", file);
                else
                  fputs ("\n\tbrtrue\t", file);
                dump_label_name (file, label_then);

                stack_pop (1);
              }
            else
              gen_binary_cond_branch (file, op0, label_then);
          }
        else if (TREE_CODE (op0) == LE_EXPR
                 || TREE_CODE (op0) == LT_EXPR
                 || TREE_CODE (op0) == GE_EXPR
                 || TREE_CODE (op0) == GT_EXPR
                 || TREE_CODE (op0) == UNLT_EXPR
                 || TREE_CODE (op0) == UNLE_EXPR
                 || TREE_CODE (op0) == UNGT_EXPR
                 || TREE_CODE (op0) == UNGE_EXPR)
          {
            gen_binary_cond_branch (file, op0, label_then);
          }
        else
          {
            gen_cil_node (file, op0);
            fputs ("\n\tldc.i4.0"
                   "\n\tbne.un\t", file);
            dump_label_name (file, label_then);
            stack_push (1);
            stack_pop (2);
          }

        if (TARGET_EMIT_JIT_COMPILATION_HINTS)
          branch_probability_add (file, node);

        {
          basic_block dest_bb = label_to_block (label_else);

          /* Emit else block only if it is not a fallthrough */
          if (bb->next_bb != dest_bb)
            {
              fputs ("\n\tbr\t", file);
              dump_label_name (file, label_else);
            }
          gcc_assert (stack == 0);
        }
      }
      break;

    case SWITCH_EXPR:
      {
        tree vec = SWITCH_LABELS (node);
        unsigned int vec_len, i;
        bool first_case = true;
        tree min_val = 0, max_val = 0;
        double_int max_min_diff;
        unsigned int switch_n;
        tree default_label;
        tree *labels;

        /* The switch body is lowered in gimplify.c, we should never have
           switches with a non-NULL SWITCH_BODY here.  */
        gcc_assert (vec && !SWITCH_BODY (node));
        vec_len = TREE_VEC_LENGTH (vec);

        /* Compute range of cases */
        for (i = 0; i < vec_len - 1 ; ++i)
          {
            tree elt  = TREE_VEC_ELT (vec, i);
            tree low  = CASE_LOW (elt);
            tree high = CASE_HIGH (elt);

            if (!high)
              high = low;

            gcc_assert (low && high);

            /* Discard empty ranges.  */
            if (INT_CST_LT (high, low))
              continue;

            if (first_case)
              {
                min_val = low;
                max_val = high;
                first_case = false;
              }
            else
              {
                if (INT_CST_LT (low, min_val))
                  min_val = low;

                if (INT_CST_LT (max_val, high))
                  max_val = high;
              }
          }
        gcc_assert (!INT_CST_LT (max_val, min_val));

        /* Get the default label */
        gcc_assert (!CASE_HIGH (TREE_VEC_ELT (vec, TREE_VEC_LENGTH (vec)- 1)));
        gcc_assert (!CASE_LOW (TREE_VEC_ELT (vec, TREE_VEC_LENGTH (vec) - 1)));
        default_label = CASE_LABEL (TREE_VEC_ELT (vec,
                                                  TREE_VEC_LENGTH (vec) - 1));

        /* Prepare a table with the label for each value of the condition */
        max_min_diff = double_int_add (double_int_neg (TREE_INT_CST (min_val)),
                                       TREE_INT_CST (max_val));
        gcc_assert (max_min_diff.high == 0 && max_min_diff.low < 8192);
        switch_n = max_min_diff.low + 1;
        gcc_assert (switch_n > 0);
        labels = (tree*)xmalloc (switch_n * sizeof (tree));

        for (i=0 ; i < switch_n; ++i)
          labels[i] = default_label;

        for (i = 0; i < vec_len - 1 ; ++i)
          {
            tree elt  = TREE_VEC_ELT (vec, i);
            tree low  = CASE_LOW (elt);
            tree high = CASE_HIGH (elt);
            double_int low_val, high_val, end, j;
            double_int minus_min_val = double_int_neg (TREE_INT_CST (min_val));

            if (!high)
              high = low;

            gcc_assert (low && high);

            /* Discard empty ranges.  */
            if (INT_CST_LT (high, low))
              continue;

            low_val  = TREE_INT_CST (low);
            high_val = TREE_INT_CST (high);
            end = double_int_add (high_val, double_int_one);

            for (j = low_val;
                 !double_int_equal_p (j, end);
                 j = double_int_add (j, double_int_one)
                )
              {
                double_int norm_j = double_int_add (j, minus_min_val);

                gcc_assert (norm_j.high == 0 && norm_j.low < 8192);
                labels[norm_j.low] = CASE_LABEL (elt);
              }
          }

        /* Emit switch condition */
        gen_cil_node (file, SWITCH_COND (node));

        /* Emit subtraction to normalize the condition */
        if (!double_int_equal_p (TREE_INT_CST (min_val), double_int_zero))
          {
            double_int min = TREE_INT_CST (min_val);

            fputs ("\n\tldc.i4\t", file);

            if (double_int_negative_p (min))
              {
                dump_double_int (file, double_int_neg (min), false);
                fputs ("\n\tadd", file);
              }
            else
              {
                dump_double_int (file, min, false);
                fputs ("\n\tsub", file);
              }

            stack_push (1);
            stack_pop (1);
          }

        /* Emit switch instruction */
        fputs ("\n\tswitch\t(", file);
        for (i=0 ; i < switch_n; ++i)
          {
            dump_label_name (file, labels[i]);
            if (i < switch_n - 1)
              fputs (", ", file);
          }
        fputs (")", file);
        stack_pop (1);

        /* Emit branch for default label */
        fputs ("\n\tbr\t", file);
        dump_label_name (file, default_label);
        gcc_assert (stack == 0);
      }
      break;

    case CALL_EXPR:
      gen_call_expr (file, node);
      break;

    case MULT_EXPR:
    case PLUS_EXPR:
    case POINTER_PLUS_EXPR:
    case MINUS_EXPR:
    case RDIV_EXPR:
    case LSHIFT_EXPR:
      op0 = TREE_OPERAND (node, 0);
      op1 = TREE_OPERAND (node, 1);

      gen_cil_node (file, op0);
      gen_cil_node (file, op1);

      switch (TREE_CODE (node))
        {
        case MULT_EXPR:    fputs ("\n\tmul", file); break;
        case POINTER_PLUS_EXPR:
        case PLUS_EXPR:    fputs ("\n\tadd", file); break;
        case MINUS_EXPR:   fputs ("\n\tsub", file); break;
        case RDIV_EXPR:    fputs ("\n\tdiv", file); break;
        case LSHIFT_EXPR:  fputs ("\n\tshl", file); break;
        default:
          gcc_unreachable ();
        }

      stack_pop (1);

      /* Values with precision smaller than the one used
         on the evaluation stack require an explicit conversion.   */
      if (INTEGRAL_TYPE_P (TREE_TYPE (node)))
        gen_integral_conv (file, TREE_TYPE (node), TREE_TYPE (node));
      break;

    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
      op0 = TREE_OPERAND (node, 0);
      op1 = TREE_OPERAND (node, 1);

      gen_cil_node (file, op0);
      gen_cil_node (file, op1);

      switch (TREE_CODE (node))
        {
        case BIT_IOR_EXPR: fputs ("\n\tor",  file); break;
        case BIT_XOR_EXPR: fputs ("\n\txor", file); break;
        default:
          gcc_unreachable ();
        }

      /* No need for conversions even in case of values with precision
         smaller than the one used on the evaluation stack,
         since for these operations the output is
         always less or equal than both operands.   */

      stack_pop (1);
      break;

    case BIT_AND_EXPR:
      op0 = TREE_OPERAND (node, 0);
      op1 = TREE_OPERAND (node, 1);

      if (TREE_CODE (op0) == INTEGER_CST
          && (TREE_INT_CST_LOW (op0) == 255U
              || TREE_INT_CST_LOW (op0) == 65535U
              || TREE_INT_CST_LOW (op0) == 4294967295U)
          && TREE_INT_CST_HIGH (op0) == 0)
        {
          gen_cil_node (file, op1);

          fputs ("\n\tconv.u", file);

          switch (TREE_INT_CST_LOW (op0))
            {
            case 255U:        fputs ("1", file); break;
            case 65535U:      fputs ("2", file); break;
            case 4294967295U: fputs ("4", file); break;
            default:
              gcc_unreachable ();
            }

          if (TYPE_PRECISION (TREE_TYPE (node)) > 32)
            fputs ("\n\tconv.u8", file);
        }
      else if (TREE_CODE (op1) == INTEGER_CST
               && (TREE_INT_CST_LOW (op1) == 255U
                   || TREE_INT_CST_LOW (op1) == 65535U
                   || TREE_INT_CST_LOW (op1) == 4294967295U)
               && TREE_INT_CST_HIGH (op1) == 0)
        {
          gen_cil_node (file, op0);

          fputs ("\n\tconv.u", file);

          switch (TREE_INT_CST_LOW (op1))
            {
            case 255U:        fputs ("1", file); break;
            case 65535U:      fputs ("2", file); break;
            case 4294967295U: fputs ("4", file); break;
            default:
              gcc_unreachable ();
            }

          if (TYPE_PRECISION (TREE_TYPE (node)) > 32)
            fputs ("\n\tconv.u8", file);
        }
      else
        {
          gen_cil_node (file, op0);
          gen_cil_node (file, op1);

          fputs ("\n\tand", file);

          /* No need for conversions even in case of values with precision
             smaller than the one used on the evaluation stack,
             since for these operations the output is
             always less or equal than both operands.   */

          stack_pop (1);
        }
      break;

    case LT_EXPR:
    case GT_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
    case UNLT_EXPR:
    case UNGT_EXPR:
      op0 = TREE_OPERAND (node, 0);
      op1 = TREE_OPERAND (node, 1);

      gen_cil_node (file, op0);
      gen_cil_node (file, op1);

      switch (TREE_CODE (node))
        {
        case LT_EXPR:
          fputs (TYPE_UNSIGNED (TREE_TYPE (op0))?"\n\tclt.un":"\n\tclt", file);
          break;

        case GT_EXPR:
          fputs (TYPE_UNSIGNED (TREE_TYPE (op0))?"\n\tcgt.un":"\n\tcgt", file);
          break;

        case EQ_EXPR:   fputs ("\n\tceq", file); break;
        case NE_EXPR:   fputs ("\n\tceq"
                               "\n\tldc.i4.1"
                               "\n\txor", file); break;
        case UNLT_EXPR: fputs ("\n\tclt.un", file); break;
        case UNGT_EXPR: fputs ("\n\tcgt.un", file); break;

        default:
          gcc_unreachable ();
        }

      if (TYPE_PRECISION (TREE_TYPE (node)) > 32)
        fputs ("\n\tconv.i8", file);

      stack_pop (1);
      break;

    case LE_EXPR:
    case GE_EXPR:
      op0 = TREE_OPERAND (node, 0);
      op1 = TREE_OPERAND (node, 1);

      gen_cil_node (file, op0);
      gen_cil_node (file, op1);

      switch (TREE_CODE (node))
        {
        case LE_EXPR:
          fputs (TYPE_UNSIGNED (TREE_TYPE (op0))?"\n\tcgt.un":"\n\tcgt", file);
          break;

        case GE_EXPR:
          fputs (TYPE_UNSIGNED (TREE_TYPE (op0))?"\n\tclt.un":"\n\tclt", file);
          break;

        default:
          gcc_unreachable ();
        }

      fputs ("\n\tldc.i4.1"
             "\n\txor", file);

      if (TYPE_PRECISION (TREE_TYPE (node)) > 32)
        fputs ("\n\tconv.i8", file);

      stack_pop (1);
      break;

    case UNORDERED_EXPR:
      op0 = TREE_OPERAND (node, 0);
      op1 = TREE_OPERAND (node, 1);

      gen_cil_node (file, op0);
      fputs ("\n\tdup"
             "\n\tceq", file);

      stack_push (1);
      stack_pop (1);

      gen_cil_node (file, op1);
      fputs ("\n\tdup"
             "\n\tceq"
             "\n\tand"
             "\n\tldc.i4.1"
             "\n\txor", file);

      stack_push (1);
      stack_pop (2);
      break;

    case ORDERED_EXPR:
      op0 = TREE_OPERAND (node, 0);
      op1 = TREE_OPERAND (node, 1);

      gen_cil_node (file, op0);
      fputs ("\n\tdup"
             "\n\tceq", file);

      stack_push (1);
      stack_pop (1);

      gen_cil_node (file, op1);
      fputs ("\n\tdup"
             "\n\tceq"
             "\n\tand", file);

      stack_push (1);
      stack_pop (2);
      break;

    case EXACT_DIV_EXPR:
    case TRUNC_DIV_EXPR:
    case TRUNC_MOD_EXPR:
    case RSHIFT_EXPR:
      op0 = TREE_OPERAND (node, 0);
      op1 = TREE_OPERAND (node, 1);

      gen_cil_node (file, op0);
      gen_cil_node (file, op1);

      switch (TREE_CODE (node))
        {
        case EXACT_DIV_EXPR:
        case TRUNC_DIV_EXPR: fputs ("\n\tdiv", file); break;
        case TRUNC_MOD_EXPR: fputs ("\n\trem", file); break;
        case RSHIFT_EXPR:    fputs ("\n\tshr", file); break;
        default:
          gcc_unreachable ();
        }

      if (TYPE_UNSIGNED (TREE_TYPE (node)))
        fputs (".un", file);

      /* No need for conversions even in case of values with precision
         smaller than the one used on the evaluation stack,
         since for these operations the output is
         always less or equal than both operands.   */

      stack_pop (1);
      break;

    case FLOOR_DIV_EXPR:
      {
        bool is_signed0, is_signed1;

        op0 = TREE_OPERAND (node, 0);
        op1 = TREE_OPERAND (node, 1);

        gen_cil_node (file, op0);
        gen_cil_node (file, op1);

        is_signed0 = TYPE_UNSIGNED (TREE_TYPE (op0));
        is_signed1 = TYPE_UNSIGNED (TREE_TYPE (op1));
        /* If both operands are unsigned, the result is positive and thus
           rounding towards zero is identical to towards -infinity. */
        if (is_signed0 && is_signed1)
          {
            fputs ("\n\tdiv.un", file);
          }
        else
          {
            fputs ("\n\tcall\t int32 [gcc4net]gcc4net.Crt::floordiv(", file);
            dump_type_for_builtin (file, TREE_TYPE (op0), true);
            fputs (", ", file);
            dump_type_for_builtin (file, TREE_TYPE (op1), true);
            fputs (")", file);
          }

      /* No need for conversions even in case of values with precision
         smaller than the one used on the evaluation stack,
         since for these operations the output is
         always less or equal than both operands.   */

        stack_pop (1);
        break;
      }

    case NEGATE_EXPR:
    case BIT_NOT_EXPR:
      gen_cil_node (file, TREE_OPERAND (node, 0));

      switch (TREE_CODE (node))
        {
        case NEGATE_EXPR:    fputs ("\n\tneg", file); break;
        case BIT_NOT_EXPR:   fputs ("\n\tnot", file); break;
        default:
          gcc_unreachable ();
        }

      /* Values with precision smaller than the one used
         on the evaluation stack require an explicit conversion.
         Unfortunately this is true for the negation as well just
         for the case in which the operand is the smallest negative value.
         Example: 8-bit negation of -128 gives 0 and not 128.   */
      if (INTEGRAL_TYPE_P (TREE_TYPE (node)))
        gen_integral_conv (file, TREE_TYPE (node), TREE_TYPE (node));
      break;

    case INDIRECT_REF:
    case ARRAY_REF:
      gen_addr_expr (file, node);

      if (TREE_THIS_VOLATILE (node))
        fputs ("\n\tvolatile.", file);

      if (AGGREGATE_TYPE_P (TREE_TYPE (node)))
        {
          fputs ("\n\tldobj\t", file);
          dump_type (file, TREE_TYPE (node), true, false);
        }
      else if (TREE_CODE (TREE_TYPE (node)) == COMPLEX_TYPE)
        {
          fputs ("\n\tldobj ", file);
          dump_complex_type (file, TREE_TYPE (node), true);
        }
      else if (TREE_CODE (TREE_TYPE (node)) == VECTOR_TYPE)
        {
          fputs ("\n\tldobj ", file);
          dump_vector_type (file, TREE_TYPE (node), true);
        }
      else
        {
          fputs ("\n\tldind.", file);
          print_type_suffix (file, TREE_TYPE (node), true);
        }
      break;

    case CONVERT_EXPR:
    case FLOAT_EXPR:
      /* ER: if flag_trapv is set, we could generate the .ovf version? */
      /* TODO: */
    case FIX_TRUNC_EXPR:
    case NOP_EXPR:
      gen_cil_node (file, TREE_OPERAND (node, 0));
      gen_conv (file,
                (TREE_CODE (node) == NOP_EXPR),
                TREE_TYPE (node),
                TREE_TYPE (TREE_OPERAND (node, 0)));
      break;

    case LABEL_EXPR:
      op0 = TREE_OPERAND (node, 0);
      /* If this is for break or continue, don't bother printing it.  */
      if (DECL_NAME (op0))
        {
          const char *name = IDENTIFIER_POINTER (DECL_NAME (op0));
          if (strcmp (name, "break") == 0
              || strcmp (name, "continue") == 0)
            break;
        }
      fprintf (file, "\n");
      dump_label_name (file, op0);
      fprintf (file, ":");

      if (DECL_NONLOCAL (op0))
        fprintf (file, " [non-local]");

      break;

    case RETURN_EXPR:
      op0 = TREE_OPERAND (node, 0);
      if (op0)
        {
          /* dump_generic_node (file, op0); */
          if (TREE_CODE (op0) == MODIFY_EXPR || TREE_CODE (op0) == GIMPLE_MODIFY_STMT)
            op0 = GENERIC_TREE_OPERAND (op0, 1);

          gen_cil_node (file, op0);
        }

      /* Pre-C99 code may contain void-returns for non-void functions,
         but the simplification pass should already have avoided this.   */
      gcc_assert (op0
                  || TREE_CODE (TREE_TYPE (DECL_RESULT (current_function_decl)))
                  == VOID_TYPE);

      fputs ("\n\tret", file);
      stack_reset ();
      break;

    case ASM_EXPR:
      {
        /* support just a simple string, no input/output/clober */
        tree asm_string = ASM_STRING (node);
        const char *str = TREE_STRING_POINTER (asm_string);

        fputs ("\n\t", file);
        fputs (str, file);
        break;
      }

    case MIN_EXPR:
    case MAX_EXPR:
      {
        tree node_type = TREE_TYPE (node);

        gcc_assert (!TARGET_EXPAND_MINMAX);

        op0 = TREE_OPERAND (node, 0);
        op1 = TREE_OPERAND (node, 1);

        gen_cil_node (file, op0);
        gen_cil_node (file, op1);

        /* emit a call */
        fputs ("\n\tcall\t", file);
        dump_type_for_builtin (file, node_type, false);
        fputs (" [gcc4net]gcc4net.Crt::__",  file);

        if (TYPE_UNSIGNED (node_type))
          fputs ("u",  file);

        if (TREE_CODE (node) == MIN_EXPR)
          fputs ("min",  file);
        else
          fputs ("max",  file);

        dump_type_eval_mode (file, node_type, false, false);
        fputs ("3(",  file);
        dump_type_for_builtin (file, TREE_TYPE (op0), false);
        fputs (", ", file);
        dump_type_for_builtin (file, TREE_TYPE (op1), false);
        fputs (")", file);
        stack_pop (1);
      }
      break;

    case ABS_EXPR:
      {
        tree node_type = TREE_TYPE (node);

        gcc_assert (!TARGET_EXPAND_ABS);

        op0 = TREE_OPERAND (node, 0);
        gen_cil_node (file, op0);

        /* emit a call */
        fputs ("\n\tcall\t", file);
        dump_type_for_builtin (file, node_type, false);
        fputs (" [gcc4net]gcc4net.Crt::__abs", file);
        dump_type_eval_mode (file, node_type, false, false);
        fputs ("2(",  file);
        dump_type_for_builtin (file, TREE_TYPE (op0), false);
        fputs (")", file);
      }
      break;

    case SSA_NAME:
      gcc_assert (0);
      break;

    case VAR_DECL:
      mark_referenced_type (TREE_TYPE (node));

      if (!DECL_FILE_SCOPE_P (node))
        {
          if (TREE_THIS_VOLATILE (node))
            {
              /* put the address of the loc on the stack */
              fputs ("\n\tldloca\t", file);
              dump_decl_name (file, node);
              /* and emit a volatile ldind or ldobj */
              fputs ("\n\tvolatile.", file);
              if (AGGREGATE_TYPE_P (TREE_TYPE (node)) ||
                  TREE_CODE (TREE_TYPE (node)) == COMPLEX_TYPE ||
                  TREE_CODE (TREE_TYPE (node)) == VECTOR_TYPE)
                {
                  fputs ("\n\tldobj\t", file);
                  dump_type (file, TREE_TYPE (node), true, false);
                }
              else
                {
                  fputs ("\n\tldind.", file);
                  print_type_suffix (file, TREE_TYPE (node), true);
                }
            }
          else
            {
              fputs ("\n\tldloc\t", file);
              dump_decl_name (file, node);
            }
        }
      else
        {
          if (TREE_THIS_VOLATILE (node))
            fputs ("\n\tvolatile.", file);
          fputs ("\n\tldsfld\t", file);
          dump_type (file, TREE_TYPE (node), true, false);
          fputs (" ", file);
          if (TARGET_EMIT_EXTERNAL_ASSEMBLY && DECL_EXTERNAL (node))
            fputs ("[ExternalAssembly]ExternalAssembly::", file);
          dump_decl_name (file, node);
        }

      stack_push (1);
      break;

    case PARM_DECL:
      mark_referenced_type (TREE_TYPE (node));
      fputs ("\n\tldarg\t", file);
      dump_decl_name (file, node);
      stack_push (1);

      /* K&R C allows declaration type to be wider than the actual type */
      if (TREE_TYPE (node) != DECL_ARG_TYPE (node))
        {
          mark_referenced_type (DECL_ARG_TYPE (node));
          gen_conv (file, true, TREE_TYPE (node), DECL_ARG_TYPE (node));
        }
      break;

    case FIELD_DECL:
    case NAMESPACE_DECL:
      fprintf (stderr, "CIL: Cannot handle FIELD_DECL or NAMESPACE_DECL: ");
      dump_decl_name (stderr, node);
      gcc_assert (0);
      break;

    case TREE_LIST:
      gcc_assert (0);
      break;

    case FUNCTION_DECL:
    case CONST_DECL:
      gcc_assert (0);
      break;

    case ADDR_EXPR:
      gen_addr_expr (file, TREE_OPERAND (node, 0));
      break;

    case COMPONENT_REF:
      {
        tree obj = TREE_OPERAND (node, 0);
        tree fld = TREE_OPERAND (node, 1);
        tree obj_type = TYPE_MAIN_VARIANT (TREE_TYPE (obj));
        tree fld_type = TREE_TYPE (fld);

        gcc_assert (TREE_CODE (fld) == FIELD_DECL);
        gcc_assert (! DECL_BIT_FIELD (fld));

        gen_addr_expr (file, obj);
        if (TREE_THIS_VOLATILE (node))
          fputs ("\n\tvolatile.", file);
        fputs ("\n\tldfld\t", file);
        dump_type (file, fld_type, true, false);
        fputs (" ", file);
        mark_referenced_type (obj_type);
        dump_valuetype_name (file, obj_type);
        fputs ("::", file);
        dump_decl_name (file, fld);
      }
      break;

    case TRUTH_NOT_EXPR:
      gen_cil_node (file, TREE_OPERAND (node, 0));
      fputs ("\n\tldc.i4.0"
             "\n\tceq", file);
      stack_push (1);
      stack_pop (1);
      break;

    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_XOR_EXPR:
      op0 = TREE_OPERAND (node, 0);
      op1 = TREE_OPERAND (node, 1);

      gen_cil_node (file, op0);
      gcc_assert (TREE_CODE (TREE_TYPE (op0)) == INTEGER_TYPE
                  || TREE_CODE (TREE_TYPE (op0)) == BOOLEAN_TYPE);
      if (TREE_CODE (TREE_TYPE (op0)) == INTEGER_TYPE)
        {
          fputs ("\n\tldc.i4.0"
                 "\n\tceq"
                 "\n\tldc.i4.1"
                 "\n\txor", file);
          stack_push (1);
          stack_pop (1);
        }

      gen_cil_node (file, op1);
      gcc_assert (TREE_CODE (TREE_TYPE (op1)) == INTEGER_TYPE
                  || TREE_CODE (TREE_TYPE (op1)) == BOOLEAN_TYPE);
      if (TREE_CODE (TREE_TYPE (op1)) == INTEGER_TYPE)
        {
          fputs ("\n\tldc.i4.0"
                 "\n\tceq"
                 "\n\tldc.i4.1"
                 "\n\txor", file);
          stack_push (1);
          stack_pop (1);
        }

      if (TREE_CODE (node) == TRUTH_AND_EXPR)
        fputs ("\n\tand", file);
      else if (TREE_CODE (node) == TRUTH_OR_EXPR)
        fputs ("\n\tor", file);
      else
        fputs ("\n\txor"
               "\n\tldc.i4.1"
               "\n\tand", file);
      stack_pop (1);
      break;

    case VIEW_CONVERT_EXPR:
      {
        op0 = TREE_OPERAND (node, 0);
        gen_cil_node (file, op0);

        if (TREE_CODE (TREE_TYPE (op0)) == VECTOR_TYPE)
          {
            /* Convert a vector type to something. */
            tree vec_type = TREE_TYPE (op0);
            fputs ("\n\tcall ", file);
            dump_type (file, TREE_TYPE (node), false, false);
            fputs (" [gcc4net]gcc4net.", file);
            dump_vector_type (file, vec_type, false);
            fputs ("::", file);
            dump_vector_type (file, vec_type, false);
            fputs ("_to_", file);
            dump_type_eval_mode (file, TREE_TYPE (node), true, true);
            fputs (" (", file);
            dump_type (file, vec_type, false, false);
            fputs (")", file);
          }
        else if (TREE_CODE (TREE_TYPE (node)) == VECTOR_TYPE)
          {
            /* Convert something to a vector type. */
            tree vec_type = TREE_TYPE (node);
            fputs ("\n\tcall ", file);
            dump_type (file, vec_type, false, false);
            fputs (" [gcc4net]gcc4net.", file);
            dump_vector_type (file, vec_type, false);
            fputs ("::ctor (", file);
            dump_type (file, TREE_TYPE (op0), false, false);
            fputs (")", file);
          }
        else
          {
            internal_error ("\n\nVIEW_CONVERT_EXPR not supported on '%s'\n",
                            tree_code_name[TREE_CODE (TREE_TYPE (node))]);
          }
      }
      break;

    case REALPART_EXPR:
    case IMAGPART_EXPR:
      {
        tree cplx_type = TREE_TYPE (TREE_OPERAND (node, 0));
        op0 = TREE_OPERAND (node, 0);

        gcc_assert (TREE_CODE (cplx_type) == COMPLEX_TYPE);

        gen_cil_node (file, op0);
        fputs ("\n\tcall ", file);
        dump_type (file, TREE_TYPE (node), false, false);
        fputs (" [gcc4net]gcc4net.", file);
        dump_complex_type (file, cplx_type, false);
        fputs ("::", file);
        dump_complex_type(file, cplx_type, false);
        fputs ("_", file);
        if (TREE_CODE (node) == REALPART_EXPR)
          fputs ("Re(", file);
        else
          fputs ("Im(", file);
        dump_complex_type(file, cplx_type, true);
        fputs (") ", file);
        stack_pop (1);
        stack_push (1);
      }
      break;

    case COMPLEX_EXPR:
      {
        tree cplx_type = TREE_TYPE (node);
        op0 = TREE_OPERAND (node, 0);
        op1 = TREE_OPERAND (node, 1);
        gen_cil_node (file, op0);
        gen_cil_node (file, op1);
        fputs ("\n\tcall ", file);
        dump_complex_type (file, cplx_type, true);
        fputs (" [gcc4net]gcc4net.", file);
        dump_complex_type(file, cplx_type, false);
        fputs ("::", file);
        dump_complex_type(file, cplx_type, false);
        fputs ("_ctor (", file);
        dump_type (file, TREE_TYPE (op0), false, false);
        fputs (", ", file);
        dump_type (file, TREE_TYPE (op0), false, false);
        fputs (")", file);
        stack_pop (2);
        stack_push (1);
      }
      break;

    case ENUMERAL_TYPE:
    case ARRAY_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
    case VOID_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case COMPLEX_TYPE:
    case VECTOR_TYPE:
    case BOOLEAN_TYPE:
    case POINTER_TYPE:
    case REFERENCE_TYPE:
      internal_error ("gen_cil_node does not support TYPE nodes,"
                      " to dump Type name use dump_type.\n");
      break;

    default:
      internal_error ("\n\nUnsupported tree in CIL generation: '[7m%s[m'\n",
                      tree_code_name[TREE_CODE (node)]);
      break;
    }
}

static void
gen_cil_modify_expr (FILE *file, tree lhs, tree rhs)
{
  switch (TREE_CODE (lhs))
    {
    case SSA_NAME:
      gcc_assert (0);
      break;

    case REALPART_EXPR:
    case IMAGPART_EXPR:
      {
        tree cplx_op = TREE_OPERAND (lhs, 0);
        tree cplx_type = TREE_TYPE (cplx_op);

        gen_addr_expr (file, cplx_op);
        gen_cil_node (file, rhs);

        fputs ("\n\tcall instance void [gcc4net]gcc4net.", file);
        dump_complex_type(file, cplx_type, false);
        fputs ("::", file);
        dump_complex_type(file, cplx_type, false);
        fputs ("_set_", file);
        if (TREE_CODE (lhs) == REALPART_EXPR)
          fputs ("Re(", file);
        else
          fputs ("Im(", file);
        dump_type (file, TREE_TYPE (lhs), false, false);
        fputs (")", file);
        stack_pop (2);
      }
      break;

    case INDIRECT_REF:
    case ARRAY_REF:
    case COMPLEX_EXPR:
      gen_addr_expr (file, lhs);
      gen_cil_node (file, rhs);

      if (TREE_THIS_VOLATILE (lhs))
        fputs ("\n\tvolatile.", file);

      if (AGGREGATE_TYPE_P (TREE_TYPE (lhs)) ||
          TREE_CODE (TREE_TYPE (lhs)) == COMPLEX_TYPE ||
          TREE_CODE (TREE_TYPE (lhs)) == VECTOR_TYPE)
        {
          fputs ("\n\tstobj\t", file);
          dump_type (file, TREE_TYPE (lhs), true, false);
        }
      else
        {
          fputs ("\n\tstind.", file);
          print_type_suffix (file, TREE_TYPE (lhs), false);
        }

      stack_pop (2);
      break;

    case VAR_DECL:
      mark_referenced_type (TREE_TYPE (lhs));

      if (!DECL_FILE_SCOPE_P (lhs))
        {
          if (TREE_THIS_VOLATILE (lhs))
            {
              /* put the address of the loc on the stack */
              fputs ("\n\tldloca\t", file);
              dump_decl_name (file, lhs);
              stack_push (1);
              /* put the value */
              gen_cil_node (file, rhs);
              /* and emit a volatile stind or stobj */
              fputs ("\n\tvolatile.", file);
              if (AGGREGATE_TYPE_P (TREE_TYPE (lhs)) ||
                  TREE_CODE (TREE_TYPE (lhs)) == COMPLEX_TYPE ||
                  TREE_CODE (TREE_TYPE (lhs)) == VECTOR_TYPE)
                {
                  fputs ("\n\tstobj\t", file);
                  dump_type (file, TREE_TYPE (lhs), true, false);
                }
              else
                {
                  fputs ("\n\tstind.", file);
                  print_type_suffix (file, TREE_TYPE (lhs), true);
                }
              stack_pop (2);
            }
          else
            {
              gen_cil_node (file, rhs);
              fputs ("\n\tstloc\t", file);
              dump_decl_name (file, lhs);
              stack_pop (1);
            }
        }
      else
        {
          gen_cil_node (file, rhs);
          if (TREE_THIS_VOLATILE (lhs))
            fputs ("\n\tvolatile.", file);
          fputs ("\n\tstsfld\t", file);
          dump_type (file, TREE_TYPE (lhs), true, false);
          fputs (" ", file);
          if (TARGET_EMIT_EXTERNAL_ASSEMBLY && DECL_EXTERNAL (lhs))
            fputs ("[ExternalAssembly]ExternalAssembly::", file);
          dump_decl_name (file, lhs);
          stack_pop (1);
        }
      break;

    case PARM_DECL:
      gen_cil_node (file, rhs);
      fputs ("\n\tstarg\t", file);
      dump_decl_name (file, lhs);
      stack_pop (1);
      break;

    case COMPONENT_REF:
      {
        tree obj = TREE_OPERAND (lhs, 0);
        tree fld = TREE_OPERAND (lhs, 1);
        tree obj_type = TYPE_MAIN_VARIANT (TREE_TYPE (obj));
        tree fld_type = TREE_TYPE (fld);

        gcc_assert (TREE_CODE (fld) == FIELD_DECL);
        gcc_assert (! DECL_BIT_FIELD (fld));

        gen_addr_expr (file, obj);
        gen_cil_node (file, rhs);
        mark_referenced_type (obj_type);
        if (TREE_THIS_VOLATILE (lhs))
          fputs ("\n\tvolatile.", file);
        fputs ("\n\tstfld\t", file);
        dump_type (file, fld_type, true, false);
        fputs (" ", file);
        dump_valuetype_name (file, obj_type);
        fputs ("::", file);
        dump_decl_name (file, fld);
        stack_pop (2);
      }
      break;

    default:
      fprintf (stderr, "CIL: Cannot handle lhs: ");
      debug_generic_expr (lhs);
      gcc_assert (0);
    }
}

/* Warning: these strings are not null-terminated */
static char *
append_string (char *str, const char *to_append,
               unsigned int *len, unsigned int *max_len)
{
  size_t i, orig_len = *len;
  size_t append_len = strlen (to_append);

  *len += append_len;

  if (*len > *max_len)
    {
      while (*len > *max_len)
        *max_len *= 2;
      str = (char *)xrealloc (str, *max_len);
    }

  for (i=0; i < append_len; ++i)
    str[orig_len + i] = to_append[i];

  return str;
}

/* Warning: these strings are not null-terminated */
static char *
append_coded_type (char *str, tree type,
                   unsigned int *len, unsigned int *max_len)
{
  if (type == NULL_TREE || type == error_mark_node)
    return str;

 encode_type:
  type = TYPE_MAIN_VARIANT (type);

  switch (TREE_CODE (type))
    {
    case INTEGER_TYPE:
      {
        int type_size = TYPE_PRECISION (type);
        char tmp_str[8] = "UI";
        char *tmp_str_ptr = tmp_str;

        snprintf (tmp_str_ptr + 2, 6, "%d", type_size);

        if (!TYPE_UNSIGNED (type))
          ++tmp_str_ptr;

        str = append_string (str, tmp_str_ptr, len, max_len);
      }
      break;

    case BOOLEAN_TYPE:
      str = append_string (str, "B", len, max_len);
      break;

    case REAL_TYPE:
      {
        int type_size = TYPE_PRECISION (type);
        char tmp_str[4] = "F";

        snprintf (tmp_str + 1, 3, "%d", type_size);

        str = append_string (str, tmp_str, len, max_len);
      }
      break;

    pointer:
    case POINTER_TYPE:
      str = append_string (str, "*", len, max_len);
      type = TREE_TYPE (type);
      goto encode_type;

    case ARRAY_TYPE:
      if (TYPE_DOMAIN (type) && ! ARRAY_TYPE_VARLENGTH (type))
        {
          tree domain = TYPE_DOMAIN (type);
          tree min = TYPE_MIN_VALUE (domain);
          tree max = TYPE_MAX_VALUE (domain);

          str = append_string (str, "[", len, max_len);

          if (ARRAY_TYPE_ZEROLENGTH (type))
            str = append_string (str, "0", len, max_len);
          else if (min && max
              && integer_zerop (min)
              && host_integerp (max, 0))
            {
              unsigned int size = TREE_INT_CST_LOW (max) + 1;
              char tmp_str[32];

              snprintf (tmp_str, 32, "%d", size);
              str = append_string (str, tmp_str, len, max_len);
            }
          else
            str = append_string (str, "unk", len, max_len);

          str = append_string (str, "]", len, max_len);
        }
      else
        goto pointer;
      type = TREE_TYPE (type);
      goto encode_type;

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

        switch (TREE_CODE (type))
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
            gcc_assert (0);
            prefix = "error";
          }

        str = append_string (str, prefix, len, max_len);
        str = append_string (str, type_str, len, max_len);
      }
      break;

    default:
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
      compact_str = (char *)xmalloc (len);
      memcpy (compact_str, str, len);

      *compact_len = len;
      return compact_str;
    }

  /* Fill the buffer */
  memset (buffer, 0, COMPACT_ID_LENGTH / 2);
  for (i=0; i < len; ++i)
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

          ++j;
        }
    }

  /* Build the compact string */
  compact_str = (char *)xmalloc (COMPACT_ID_LENGTH);
  for (i=0; i < COMPACT_ID_LENGTH / 2; ++i)
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

  tmp_name = (char *)xmalloc (tmp_name_max_len);

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
      gcc_assert (TYPE_DOMAIN (t) && ! ARRAY_TYPE_VARLENGTH (t));
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
          if (DECL_NAME (tmp) != 0)
            tmp_name = append_string (tmp_name,
                                      IDENTIFIER_POINTER (DECL_NAME (tmp)),
                                      &tmp_name_len, &tmp_name_max_len);
          else
            /* Unnamed bitfields or unions */
            tmp_name = append_string (tmp_name, "?unnamed",
                                      &tmp_name_len, &tmp_name_max_len);
          tmp_name = append_string (tmp_name, "?",
                                    &tmp_name_len, &tmp_name_max_len);
          tmp = TREE_CHAIN (tmp);
        }
    }

  vt_name = get_compact_identifier (tmp_name, tmp_name_len, &vt_name_len);
  free (tmp_name);

  ident = get_identifier_with_length (vt_name, vt_name_len);
  free (vt_name);

  return ident;
}

static void
print_valuetype_decl (FILE *file, tree t)
{
  bool is_enum;

  if (t == NULL_TREE || t == error_mark_node)
    return;

  if (!AGGREGATE_TYPE_P (t) && TREE_CODE (t) != ENUMERAL_TYPE)
    return;

  if (file == 0)
    return;

  gcc_assert (TYPE_MAIN_VARIANT (t) == t);
  gcc_assert (TYPE_NAME (t));

  is_enum = (TREE_CODE (t) == ENUMERAL_TYPE);

  /* Print the name of the valuetype.  */
  fputs ("\n.class ", file);

  if (TYPE_FILE_SCOPE_P (t))
    fputs ("public ", file);
  else
    fputs ("private ", file);

  if (!is_enum)
    fputs ("explicit ", file);

  fputs ("sealed serializable ansi ", file);
  dump_valuetype_name (file, t);
  fputs (" extends ['mscorlib']System.", file);

  if (is_enum)
    fputs ("Enum\n", file);
  else
    fputs ("ValueType\n", file);

  /* Print the contents of the valuetype.  */
  fputs ("{\n", file);

  if (is_enum)
    {
      int type_size = TYPE_PRECISION (t);
      char tmp_str[8] = "int";
      char *base_type_str = tmp_str;
      tree tmp;

      snprintf (base_type_str + 3, 5, "%d", type_size);

      fputs ("\t.field public specialname rtspecialname ", file);

      if (!TYPE_UNSIGNED (t))
        fputs ("unsigned ", file);

      fprintf (file, "%s 'value__'\n", base_type_str);

      tmp = TYPE_VALUES (t);
      while (tmp)
        {
          fputs ("\t.field public static literal ", file);
          dump_type (file, t, false, false);
          fputs (" '", file);
          gcc_assert (TREE_CODE (TREE_PURPOSE (tmp)) == IDENTIFIER_NODE);
          fprintf (file, IDENTIFIER_POINTER (TREE_PURPOSE (tmp)));
          fprintf (file, "' = %s(%ld)\n",
                   base_type_str, TREE_INT_CST_LOW (TREE_VALUE (tmp)));
          tmp = TREE_CHAIN (tmp);
        }
    }
  else if (TREE_CODE (t) == ARRAY_TYPE)
    {
      /* array */
      fprintf (file, "\t.size %ld\n", TREE_INT_CST_LOW (TYPE_SIZE_UNIT (t)));
      fputs ("\t.field [0] public specialname ", file);
      dump_type (file, TREE_TYPE (t), false, false);
      fputs (" 'elem__'\n", file);
    }
  else
    {
      /* struct and union */
      tree tmp;

      fprintf (file, "\t.size %ld\n", TREE_INT_CST_LOW (TYPE_SIZE_UNIT (t)));

      tmp = TYPE_FIELDS (t);
      while (tmp)
        {
          tree type;
          unsigned int bit_offset =
            TREE_INT_CST_LOW (DECL_FIELD_BIT_OFFSET (tmp));
          unsigned int byte_offset =
            TREE_INT_CST_LOW (DECL_FIELD_OFFSET (tmp));
          unsigned int offset;

          /* Skip unnamed bitfields */
          if (DECL_NAME (tmp) == 0 && DECL_BIT_FIELD (tmp))
            goto next;

          if (DECL_BIT_FIELD (tmp))
            {
              unsigned int type_size;

              type = DECL_BIT_FIELD_TYPE (tmp);

              gcc_assert (TREE_CODE (TREE_TYPE (tmp)) == INTEGER_TYPE
                          || TREE_CODE (TREE_TYPE (tmp)) == BOOLEAN_TYPE);
              gcc_assert (TREE_CODE (type) == INTEGER_TYPE
                          || TREE_CODE (type) == BOOLEAN_TYPE
                          || TREE_CODE (type) == ENUMERAL_TYPE);

              type_size = TREE_INT_CST_LOW (TYPE_SIZE (type));
              offset = byte_offset + (bit_offset & ~(type_size - 1)) / 8;
            }
          else
            {
              type = TREE_TYPE (tmp);
              gcc_assert (bit_offset % 8 == 0);
              offset = byte_offset + bit_offset / 8;
            }

          fprintf (file, "\t.field [%d] public ", offset);
          dump_type (file, type, false, false);
          fputs (" ", file );
          dump_decl_name (file, tmp);
          fputs ("\n", file);
        next:
          tmp = TREE_CHAIN (tmp);
        }
    }

  fputs ("}\n", file);
}

  /* The attribute string must be less than 127 characters and
     contain only 7-bit ASCII chars.  No checks though. */
static void
gen_string_custom_attr (FILE *stream, const char* parameter)
{
  int i;
  int len = strlen (parameter);

  if (TARGET_EMIT_GIMPLE_COMMENTS)
    fprintf (stream, "\n\t// Custom attribute String \"%s\"", parameter);

  gcc_assert (len <= 127);

  fprintf (stream, "\n\t.custom instance void [mscorlib]System.String::.ctor(int8 *) = (01 00 %02x ", len);
  for (i=0; i < len; ++i)
    fprintf (stream, "%02x ", parameter[i]);
  fputs ("00 00)\n", stream);
}


static void
print_string_decl (FILE *file, tree t)
{
  const char *str;
  int len, i;

  gcc_assert (TREE_CODE (t) == STRING_CST);
  str = TREE_STRING_POINTER (t);
  len = TREE_STRING_LENGTH (t);

  mark_referenced_type (TREE_TYPE (t));

  /* Emit the string in readable form as a comment. */
  fputs ("// string: \"", file);
  for (i=0; i < len-1; ++i)
    {
      switch (str[i]) {
      case '\n': fputs ("\\n",  file); break;
      case '"':  fputs ("\"",   file); break;
      default:   fputc (str[i], file); break;
      }
    }
  fputs ("\"\n", file);

  fprintf (file, ".data 'DataStr%u' = bytearray(", get_string_cst_id (t));
  for (i=0; i < len; ++i)
    fprintf (file, "%02x ", (unsigned char)str[i]);
  fputs (")\n", file);

  fputs (".field private static ", file);
  dump_type (file, TREE_TYPE (t), true, false);
  fputs (" ", file);
  dump_string_name (file, t);
  fprintf (file, " at 'DataStr%u'\n", get_string_cst_id (t));
}

static void
gen_start_function (FILE *stream)
{
  int nargs;
  tree args;
  for (nargs=0,args = DECL_ARGUMENTS (current_function_decl);
       args;
       args = TREE_CHAIN (args),++nargs)
    {
    }

  fputs ("\n.method public static void '.start'(class [mscorlib]System.String[] 'args') cil managed",
         stream);
  fputs ("\n{"
         "\n\t.entrypoint"
         "\n\t.maxstack 3"
         "\n\t.locals (int32 'argc', int8** 'argv', int8** 'env')", stream);
  /* TODO: add startup code*/
  switch (nargs)
    {
    case 0:
      fputs ("\n\tcall\tvoid [gcc4net]gcc4net.StartupHelper::Startup()"
             "\n\tcall\tint32 main()", stream);
      break;

    case 1:
      fputs ("\n\tldloca\t'argc'"
             "\n\tcall\tnative int [gcc4net]gcc4net.StartupHelper::GetArgv(int32&)"
             "\n\tpop"
             "\n\tcall\tvoid [gcc4net]gcc4net.StartupHelper::Startup()"
             "\n\tldloc\t'argc'"
             "\n\tcall\tint32 main(int32)", stream);
      break;

    case 2:
      fputs ("\n\tldloca\t'argc'"
             "\n\tcall\tnative int [gcc4net]gcc4net.StartupHelper::GetArgv(int32&)"
             "\n\tstloc\t'argv'"
             "\n\tcall\tvoid [gcc4net]gcc4net.StartupHelper::Startup()"
             "\n\tldloc\t'argc'"
             "\n\tldloc\t'argv'"
             "\n\tcall\tint32 main(int32, int8**)", stream);
      break;

    case 3:
      fputs ("\n\tldloca\t'argc'"
             "\n\tcall\tnative int [gcc4net]gcc4net.StartupHelper::GetArgv(int32&)"
             "\n\tstloc\t'argv'"
             "\n\tcall\tnative int [gcc4net]gcc4net.StartupHelper::GetEnvVars()"
             "\n\tstloc\t'env'"
             "\n\tcall\tvoid [gcc4net]gcc4net.StartupHelper::Startup()"
             "\n\tldloc\t'argc'"
             "\n\tldloc\t'argv'"
             "\n\tldloc\t'env'"
             "\n\tcall\tint32 main(int32, int8**, int8**)", stream);
      break;

    default:
      gcc_assert (0);
    }

  /* TODO: add exit code*/
  fputs ("\n\tcall\tvoid [gcc4net]gcc4net.StartupHelper::Shutdown(int32)"
         "\n\tret"
         "\n} // .start"
         "\n\n", stream);
}


/* This function is mostly a copy of the last part of 'gen_cil'. */
static void
gen_cil_vcg (FILE *vcg_stream)
{
  block_stmt_iterator bsi;
  edge_iterator ei;
  const char *fun_name = lang_hooks.decl_printable_name (current_function_decl, 1);
  edge e;
  int  i=0;

  fprintf (vcg_stream, "graph: {\n");
  fprintf (vcg_stream, "title: \"%s\"\n",
           lang_hooks.decl_printable_name (current_function_decl, 1));
  fprintf (vcg_stream, "node: { title: \"%sBB%d\" label: \"ENTRY %s\" }\n",
           fun_name, ENTRY_BLOCK, fun_name);
  fprintf (vcg_stream, "node: { title: \"%sBB%d\" label: \"EXIT\" }\n",
           fun_name, EXIT_BLOCK);
  fprintf (vcg_stream, "edge:{sourcename: \"%sBB%d\" targetname: \"%sBB%d\"}\n",
           fun_name, ENTRY_BLOCK,
           fun_name, single_succ (ENTRY_BLOCK_PTR)->index);

  FOR_EACH_BB (bb)
    {
      tree stmt = NULL_TREE;

      fprintf (vcg_stream, "node: { title: \"%sBB%d\" label: \"(BB%d, pos: %d)",
               fun_name, bb->index, bb->index, i++);

      if (bb->loop_depth)
        fprintf (vcg_stream, " LOOP DEPTH %d ", bb->loop_depth);

      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
          stmt = bsi_stmt (bsi);
          if (TARGET_EMIT_GIMPLE_COMMENTS)
            {
              fprintf (vcg_stream, "\n\t [ ");
              print_generic_expr (vcg_stream, stmt, 0);
              fprintf (vcg_stream, " ]");
            }
          gcc_assert (stack == 0);

          if (TREE_CODE (stmt) != NOP_EXPR
              || TREE_CODE (TREE_OPERAND (stmt, 0)) != INTEGER_CST)
            gen_cil_node (vcg_stream, stmt);

          if (TREE_CODE (stmt) == CALL_EXPR)
            {
              tree fun_expr = CALL_EXPR_FN (stmt);
              tree fun_type = TREE_TYPE (TREE_TYPE (fun_expr));

              if (TREE_CODE (TREE_TYPE (fun_type)) != VOID_TYPE)
                {
                  fputs ("\n\tpop", vcg_stream);
                  stack_pop (1);
                }
            }
        }

      if ((!stmt || (TREE_CODE (stmt) != COND_EXPR)) && single_succ_p (bb))
        {
          basic_block succ = single_succ (bb);

          /* The last part of the test (succ != bb->next_bb) is a HACK.  It
             avoids generating a branch to the successor in case of a
             fallthrough. To be fixed when we have a proper layout of basic
             blocks.  Note that branches from COND_EXPR are still generated,
             even to a fallthrough. */
          if ((succ->index != EXIT_BLOCK) && (succ != bb->next_bb))
            {
              tree label = tree_block_label (succ);
              fputs ("\n\tbr\t", vcg_stream);
              dump_label_name (vcg_stream, label);
              gcc_assert (stack == 0);
            }
        }
      fprintf (vcg_stream, "\" }\n");  /* close 'label' clause */

      for (ei = ei_start (bb->succs); ei_cond (ei, &e); ei_next (&ei))
        {
          if (e->flags & EDGE_DFS_BACK)
            fprintf (vcg_stream, "backedge: { color: red");
          else if (e->flags & EDGE_LOOP_EXIT)
            fprintf (vcg_stream, "edge: { color: blue");
          else
            fprintf (vcg_stream, "edge: {");

          fprintf (vcg_stream, " label:\"%d", e->probability);

          if (e->flags & EDGE_LOOP_EXIT)
            fprintf (vcg_stream, " loop_exit");

          fprintf (vcg_stream, "\"");

          fprintf (vcg_stream,
                   " sourcename: \"%sBB%d\" targetname: \"%sBB%d\" }\n",
                   fun_name, bb->index, fun_name, e->dest->index);
        }
    }
  fprintf (vcg_stream, "}\n");
}


static void
gen_cil_1 (FILE *stream)
{
  block_stmt_iterator bsi;
  bool varargs = FALSE;
  tree args;

  remove_stloc_ldloc ();

  addr_taken_labels = NULL;
  FOR_EACH_BB (bb)
    {
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
          tree stmt = bsi_stmt (bsi);

          /* Record the address taken labels. */
          if (TREE_CODE (stmt) == LABEL_EXPR)
            {
              tree label = LABEL_EXPR_LABEL (stmt);
              /* Check if the label has its address taken. */
              if (FORCED_LABEL (label))
                {
                  TREE_CHAIN (label) = addr_taken_labels;  /* prepend */
                  addr_taken_labels = label; /* new list head */
                }
            }
        }
    }

  /* Make sure that every bb has a label */
  FOR_EACH_BB (bb)
    {
      tree_block_label (bb);
    }

  stack_reset ();
  max_stack = 0;

  if (MAIN_NAME_P (DECL_NAME (current_function_decl)))
    gen_start_function (stream);

  {
    tree var, cell;

    for (cell = cfun->unexpanded_var_list;
         cell;
         cell = TREE_CHAIN (cell))
      {
        var = TREE_VALUE (cell);

        if (TREE_STATIC (var) && TREE_ASM_WRITTEN (var) == 0)
          make_decl_cil (stream, var);
      }
  }

  {
    tree args_type = TYPE_ARG_TYPES (TREE_TYPE (current_function_decl));

    if (args_type != NULL)
      {
        while (TREE_CHAIN (args_type))
          args_type = TREE_CHAIN (args_type);

        if (TREE_VALUE (args_type) != void_type_node)
          varargs = TRUE;
      }
  }

  fputs ("\n.method ", stream);

  if (TREE_PUBLIC (current_function_decl))
    fputs ("public ", stream);
  else
    fputs ("private ", stream);

  fputs ("static ", stream);
  if (varargs)
    fputs ("vararg ", stream);
  dump_type (stream, TREE_TYPE (TREE_TYPE (current_function_decl)), true, false);
  fprintf (stream, " '%s' (",
           lang_hooks.decl_printable_name (current_function_decl, 1));

  args = DECL_ARGUMENTS (current_function_decl);

  if (cfun->static_chain_decl)
    {
      dump_type (stream, DECL_ARG_TYPE (cfun->static_chain_decl), true, true);
      fputs (" ", stream);
      dump_decl_name (stream, cfun->static_chain_decl);
      if (args)
        fputs (", ", stream);
    }


  while (args)
    {
      dump_type (stream, DECL_ARG_TYPE (args), true, true);
      fputs (" ", stream);
      dump_decl_name (stream, args);
      args = TREE_CHAIN (args);

      if (args)
        fputs (", ", stream);
      else  if (varargs)
        fputs (", ...", stream);
    }

  fputs (") cil managed"
         "\n{", stream);

  fputs ("\n\t.locals (", stream);
  {
    tree var, cell;
    bool first = true;

    for (cell = cfun->unexpanded_var_list;
         cell;
         cell = TREE_CHAIN (cell))
      {
        var = TREE_VALUE (cell);
        if (!TREE_STATIC (var))
          {
            if (!first)
              fputs (", ", stream);
            first = false;
            dump_type (stream, TREE_TYPE (var), true, false);
            fputs (" ", stream);
            dump_decl_name (stream, var);
          }
      }
  }
  fputs (")\n", stream);

  if (DECL_STATIC_CONSTRUCTOR (current_function_decl))
    {
      /* For the time being this attribute is a String. */
      gen_string_custom_attr (stream, "initfun");

      if (TARGET_OPENSYSTEMC)
        fputs ("\n\t.custom instance "
               "void ['OpenSystem.C']'OpenSystem.C'.InitializerAttribute::.ctor() "
               "= (01 00 00 00)", stream);
    }

  {
    struct fnct_attr attributes;
    decode_function_attrs (current_function_decl, &attributes);
    if (attributes.cusattr_string)
      gen_string_custom_attr (stream, attributes.cusattr_string);
  }

  FOR_EACH_BB (bb)
    {
      tree stmt = NULL_TREE;

      if (TARGET_EMIT_GIMPLE_COMMENTS)
        {
          fprintf (stream, "\n\n\t/* Basic block frequency: %d */",
                   bb->frequency * 100 / BB_FREQ_MAX);
        }

      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
          stmt = bsi_stmt (bsi);
          if (TARGET_EMIT_GIMPLE_COMMENTS)
            {
              fprintf (stream, "\n\t/* ");
              print_generic_expr (stream, stmt, 0);
              fprintf (stream, " */");
            }
          gcc_assert (stack == 0);

          if (TREE_CODE (stmt) != NOP_EXPR
              || TREE_CODE (TREE_OPERAND (stmt, 0)) != INTEGER_CST)
            gen_cil_node (stream, stmt);

          if (TREE_CODE (stmt) == CALL_EXPR)
            {
              tree fun_expr = CALL_EXPR_FN (stmt);
              tree fun_type = TREE_TYPE (TREE_TYPE (fun_expr));

              if (TREE_CODE (TREE_TYPE (fun_type)) != VOID_TYPE)
                {
                  fputs ("\n\tpop", stream);
                  stack_pop (1);
                }
            }
        }

      if ((!stmt || (TREE_CODE (stmt) != COND_EXPR)) && single_succ_p (bb))
        {
          basic_block succ = single_succ (bb);

          /* The last part of the test (succ != bb->next_bb) is a HACK.  It
             avoids generating a branch to the successor in case of a
             fallthrough. To be fixed when we have a proper layout of basic
             blocks.   */
          if ((succ->index != EXIT_BLOCK) && (succ != bb->next_bb))
            {
              tree label = tree_block_label (succ);
              fputs ("\n\tbr\t", stream);
              dump_label_name (stream, label);
              gcc_assert (stack == 0);
            }
        }

      if (TARGET_EMIT_GIMPLE_COMMENTS)
        {
          edge e;
          edge_iterator ei;

          fputs ("\n\t/* Edge probabilities: ", stream);
          FOR_EACH_EDGE (e, ei, bb->succs)
            {
              dump_entry_label_name (stream, e->dest);
              fprintf (stream, ": %d ", e->probability * 100 / REG_BR_PROB_BASE);
            }
          fputs ("*/", stream);
        }
    }

  if (TARGET_EMIT_JIT_COMPILATION_HINTS)
    {
      basic_block_frequency_emit (stream);
      branch_probability_emit_and_reset (stream);
    }

  fprintf (stream, "\n\t.maxstack %d\n", max_stack);
  fprintf (stream, "\n} // %s\n",
           lang_hooks.decl_printable_name (current_function_decl, 1));
  TREE_ASM_WRITTEN (current_function_decl) = 1;
}

static void
create_init_method (void)
{
  struct function *current_cfun = cfun;
  tree fun_type;
  tree fun_decl;
  tree init_expr = NULL;
  tree result;

  fun_type = build_function_type (void_type_node, void_list_node);
  fun_decl = build_decl (FUNCTION_DECL, get_identifier ("COBJ?init"), fun_type);

  result = build_decl (RESULT_DECL, NULL_TREE, void_type_node);
  DECL_ARTIFICIAL (result) = 1;
  DECL_IGNORED_P (result) = 1;
  DECL_RESULT (fun_decl) = result;

  /* Allocate memory for the function structure.  The call to
     allocate_struct_function clobbers CFUN, so we need to restore
     it afterward.  */
  allocate_struct_function (fun_decl);
/*   DECL_SOURCE_LOCATION (fun_decl) = DECL_SOURCE_LOCATION (decl); */
/*   cfun->function_end_locus = DECL_SOURCE_LOCATION (decl); */


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

  {
    int i, n;

    i = 0;
    n = VARRAY_ACTIVE_SIZE (pending_ctors);
    while (i<n)
      {
        for (; i < n; ++i)
          {
            tree decl = VARRAY_TREE (pending_ctors, i);
            tree init = DECL_INITIAL (decl);

            DECL_INITIAL (decl) = NULL_TREE;

            expand_init_to_stmt_list (decl, init, &init_expr);

          }
        n = VARRAY_ACTIVE_SIZE (pending_ctors);
      }
  }
  DECL_SAVED_TREE (fun_decl) = init_expr;

  gimplify_function_tree (fun_decl);
  tree_lowering_passes (fun_decl);
  tree_rest_of_compilation (fun_decl);

  /* Restore the current function */
  cfun = current_cfun;
}

void
make_decl_cil (FILE *stream, tree decl)
{
  if (TREE_CODE (decl) == VAR_DECL && (TREE_STATIC (decl) || TREE_PUBLIC (decl)))
    {
      tree init = DECL_INITIAL (decl);

      fputs ("\n.field ", stream);

      if (TREE_PUBLIC (decl))
        fputs ("public  ", stream);
      else
        fputs ("private ", stream);

      fputs ("static ", stream);
      dump_type (stream, TREE_TYPE (decl), true, false);
      fputs (" ", stream);
      dump_decl_name (stream, decl);
      fputs ("\n", stream);

      if (init && init != error_mark_node)
        VARRAY_PUSH_TREE (pending_ctors, decl);

      TREE_ASM_WRITTEN (decl) = 1;
    }
}

void
gen_cil_init (void)
{
  FILE *stream = asm_out_file;

  fputs (".assembly extern gcc4net {}", stream);
  if (TARGET_EMIT_EXTERNAL_ASSEMBLY)
    fputs ("\n.assembly '_C_MONO_ASSEMBLY' {}", stream);
  fputs ("\n.module '<Module>'", stream);

  if (TARGET_OPENSYSTEMC)
    fputs ("\n.custom instance "
           "void ['OpenSystem.C']'OpenSystem.C'.ModuleAttribute::.ctor() "
           "= (01 00 00 00)"
           "\n", stream);

  referenced_types = pointer_set_create ();
  VARRAY_TREE_INIT (referenced_strings, 32, "strings used in current unit");
  referenced_string_ptrs = pointer_set_create ();
  referenced_pinvoke = pointer_set_create ();
  VARRAY_TREE_INIT (pending_ctors, 32, "pending ctors");

  /* Allocate hash table for pointer ids */
  pointer_id_htable = htab_create (256,
                                   pointer_id_data_hash,
                                   pointer_id_data_eq,
                                   free);

  if (TARGET_EMIT_VCG)
    fprintf (stdout, "graph: {\ndisplay_edge_labels: yes\n");
}

void
gen_cil_fini (void)
{
  FILE *stream = asm_out_file;
  struct pointer_set_iter_t it;
  int i, n;


  if (VARRAY_ACTIVE_SIZE (pending_ctors) > 0)
    {
      create_init_method ();
    }
  VARRAY_CLEAR (pending_ctors);

  i = 0;
  n = VARRAY_ACTIVE_SIZE (referenced_strings);
  for (; i < n; ++i)
    {
      tree type =  VARRAY_TREE (referenced_strings, i);
      print_string_decl (stream, type);
    }
  VARRAY_CLEAR (referenced_strings);
  pointer_set_destroy (referenced_string_ptrs);

  /* There may be distinct tree types that correspond to identical types.
     In order not to slow down mark_referenced_type(...) function (which
     may typically be called several times for the same type), insertion
     of types in the mark set makes only sure that the same tree type
     pointer is not inserted twice. As a consequence, there may still be
     distinct tree types that correspond to identical types in the
     reference type set.
     Hence, before emitting a type, make sure no type with the same name
     has already been emitted.  */

  it = pointer_set_begin (referenced_types);
  while (!POINTER_SET_ITER_IS_END (it))
    {
      tree type = (tree)POINTER_SET_ITER_ELEM (it);
      bool found = false;
      struct pointer_set_iter_t it2;
      tree type_name = TYPE_NAME (type);
      const char* type_str;

      if (COMPLETE_TYPE_P (type))
        {
          gcc_assert (DECL_P (type_name)
                      || TREE_CODE (type_name) == IDENTIFIER_NODE);

          if (TREE_CODE (type_name) == IDENTIFIER_NODE)
            type_str = IDENTIFIER_POINTER (type_name);
          else
            type_str = IDENTIFIER_POINTER (DECL_NAME (type_name));

          it2 = pointer_set_begin (referenced_types);
          while (!found && POINTER_SET_ITER_ELEM (it2) != type)
            {
              tree type2 = (tree)POINTER_SET_ITER_ELEM (it2);
              tree type2_name = TYPE_NAME (type2);
              const char* type2_str;

              gcc_assert (DECL_P (type2_name)
                          || TREE_CODE (type2_name) == IDENTIFIER_NODE);

              if (TREE_CODE (type2_name) == IDENTIFIER_NODE)
                type2_str = IDENTIFIER_POINTER (type2_name);
              else
                type2_str = IDENTIFIER_POINTER (DECL_NAME (type2_name));

              found = strcmp (type2_str, type_str) == 0;
              it2 = pointer_set_next (referenced_types, it2);
            }

          if (!found)
            print_valuetype_decl (stream, type);
        }

      it = pointer_set_next (referenced_types, it);
    }
  pointer_set_destroy (referenced_types);

  it = pointer_set_begin (referenced_pinvoke);
  while (!POINTER_SET_ITER_IS_END (it))
    {
      tree fun = (tree)POINTER_SET_ITER_ELEM (it);
      tree fun_type = TREE_TYPE (fun);
      struct fnct_attr attributes;

      decode_function_attrs (fun, &attributes);

      fputs (".method ", stream);

      if (TREE_PUBLIC (fun))
        fputs ("public  ", stream);
      else
        fputs ("private ", stream);

      fprintf (stream, "static pinvokeimpl(\"%s\"", attributes.pinvoke_assembly);

      if (attributes.pinvoke_fname)
        fprintf (stream, " as \"%s\"", attributes.pinvoke_fname);

      fputs (") ", stream);

      dump_fun_type (stream, fun_type, fun, NULL, false);

      fputs (" cil managed {}\n", stream);
      it = pointer_set_next (referenced_pinvoke, it);
    }
  pointer_set_destroy (referenced_pinvoke);

  /* Delete hash table for pointer ids */
  htab_delete (pointer_id_htable);

  if (TARGET_EMIT_VCG)
    fprintf (stdout, "}\n");
}

static bool
gen_cil_gate (void)
{
  return current_function_decl != NULL;
}

static unsigned int
gen_cil (void)
{
  gen_cil_1 (asm_out_file);

  if (TARGET_EMIT_VCG)
    gen_cil_vcg(stdout);

  return 0;
}

/* Define the parameters of the gen-CIL pass.  */

struct tree_opt_pass pass_gen_cil =
{
  "cil",                                /* name */
  gen_cil_gate,                         /* gate */
  gen_cil,                              /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_REST_OF_COMPILATION,               /* tv_id */
  PROP_cfg,                             /* properties_required */
  0,                                    /* properties_provided */
  /* ??? If TER is enabled, we also kill gimple.  */
  0,                                    /* properties_destroyed */
  0,
  0,
  0                                     /* letter */
};

#include "gt-gen-cil.h"

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
