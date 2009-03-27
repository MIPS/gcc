/* Dump of CIL code into assembly.

   Copyright (C) 2006-2008 Free Software Foundation, Inc.

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
Erven Rohou             <erven.rohou@st.com>
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "timevar.h"
#include "toplev.h"
#include "errors.h"
#include "ggc.h"
#include "real.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "ebitmap.h"
#include "langhooks.h"
#include "output.h"
#include "pointer-set.h"
#include "varray.h"
#include "cil-builtins.h"
#include "cil-refs.h"
#include "cil-stmt.h"
#include "cil-types.h"
#include "emit-hints.h"
#include "emit-cil.h"

/******************************************************************************
 * Type declarations                                                          *
 ******************************************************************************/

struct fnct_attr
{
  const char *assembly_name;
  const char *cil_name;
  const char *cusattr_string;
  const char *pinvoke_assembly;
  const char *pinvoke_fname;
};

/* Structure used to store the number of uses of a local variable.  */

struct var_uses
{
  tree var;
  unsigned int uses;
};

typedef struct var_uses var_uses_s;

DEF_VEC_O(var_uses_s);
DEF_VEC_ALLOC_O(var_uses_s, heap);

/******************************************************************************
 * Globals                                                                    *
 ******************************************************************************/

static bool init = false;

/******************************************************************************
 * Local function prototypes                                                  *
 ******************************************************************************/

static unsigned int compute_max_stack ( void );
static void decode_function_attrs (tree, struct fnct_attr *);
static void emit_enum_decl (FILE *, tree);
static void emit_array_decl (FILE *, tree);
static void emit_incomplete_decl (FILE *, tree);
static void emit_struct_union_decl (FILE *, tree);
static void emit_valuetype_decl (FILE *, tree);
static void dump_decl_name (FILE *, tree);
static void dump_string_name (FILE *, tree);
static void dump_label_name (FILE *, tree);
static void dump_valuetype_name (FILE *, tree);
static void dump_fun_type (FILE *, tree, tree, const char *, bool);
static void dump_vector_type (FILE *, tree, bool);
static void dump_complex_type (FILE *, tree, bool);
static void dump_type (FILE *, tree, bool, bool);
static void dump_string_type (FILE *, tree);
static void dump_string_decl (FILE *, tree);
static bool dump_type_promoted_type_def (FILE *, tree);

static void emit_referenced_strings (void);
static void emit_referenced_types (void);
static void emit_pinvoke_function (FILE *, tree);
static void emit_referenced_pinvokes (void);
static void emit_prefixes (FILE *, const_cil_stmt);
static void emit_ldsfld (FILE *, const_cil_stmt);
static void emit_ldsflda (FILE *, const_cil_stmt);
static void emit_ldfld (FILE *, const_cil_stmt);
static void emit_ldflda (FILE *, const_cil_stmt);
static void emit_stfld (FILE *, const_cil_stmt);
static void emit_stsfld (FILE *, const_cil_stmt);
static void emit_switch (FILE *, const_cil_stmt);
static void emit_string_custom_attr (FILE *, const char *);
static bool emit_builtin_call (FILE *, const_cil_stmt);
static void emit_call (FILE *, const_cil_stmt);
static void emit_cil_stmt (FILE *, const_cil_stmt);
static void emit_start_function (FILE *);
static void rename_var (tree, const char *, unsigned long);
static void emit_static_vars (FILE *);
static int var_uses_compare (const void *, const void *);
static void emit_local_vars (FILE *);
static void emit_function_header (FILE *);
static void emit_cil_bb (FILE *, basic_block);
static void emit_cil_1 (FILE *);
static unsigned int emit_cil (void);


static void emit_cil_vcg_1 (FILE *);
static bool emit_cil_vcg_gate (void);
static unsigned int emit_cil_vcg (void);

/******************************************************************************
 * CIL assembly emission functions                                            *
 ******************************************************************************/

void
emit_cil_init (void)
{
  FILE *file = asm_out_file;

  gcc_assert (!init);

  if (TARGET_GCC4NET_LINKER)
    {
      fputs (".assembly extern mscorlib {}\n"
	     ".assembly extern gcc4net {}\n"
	     ".assembly extern ExternalAssembly {}\n", file);
      fprintf (file, ".assembly '%s' {\n", aux_base_name);
      fputs ("\t.custom instance "
	     "void [gcc4net]gcc4net.C_Attributes.CObjectFile::.ctor() "
	     "= (01 00 00 00)\n"
	     "}\n", file);
      fprintf (file, ".module '%s'\n", aux_base_name);
    }
  else if (TARGET_OPENSYSTEMC)
    {
      fputs (".assembly extern gcc4net {}\n"
	     ".module '<Module>'\n"
	     ".custom instance "
	     "void ['OpenSystem.C']'OpenSystem.C'.ModuleAttribute::.ctor() "
	     "= (01 00 00 00)\n", file);
    }

  init = true;
}

void
emit_cil_fini (void)
{
  gcc_assert (init);

  create_init_method ();
  emit_referenced_strings ();
  emit_referenced_types ();
  emit_referenced_pinvokes ();
}

void
emit_cil_decl (FILE *file, tree decl)
{
  if (TREE_CODE (decl) == VAR_DECL
      && (TREE_STATIC (decl) || TREE_PUBLIC (decl)))
    {
      tree init = DECL_INITIAL (decl);

      fprintf (file, "\n.field %s static ",
	       TREE_PUBLIC (decl) ? "public" : "private");
      dump_type (file, TREE_TYPE (decl), true, false);
      fprintf (file, " ");
      dump_decl_name (file, decl);
      fprintf (file, "\n");

      if (init && init != error_mark_node)
	record_ctor (decl);

      TREE_ASM_WRITTEN (decl) = 1;
    }
}

/* Compute and return the maximum stack depth of the current function.  */

static unsigned int
compute_max_stack (void)
{
  unsigned int *depths;
  unsigned int max_depth = 0;
  unsigned int depth;
  basic_block bb;
  edge e;
  edge_iterator ei;

  depths = XCNEWVEC (unsigned int, last_basic_block);

  FOR_EACH_BB (bb)
    {
      depth =  cil_seq_stack_depth (cil_bb_seq (bb), depths[bb->index], true);
      max_depth = (depth > max_depth) ? depth : max_depth;
      depth = cil_seq_stack_depth (cil_bb_seq (bb), depths[bb->index], false);

      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  depths[e->dest->index] = depth;
	}
    }

  XDELETEVEC (depths);

  return max_depth;
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

/* Dump the name of a _DECL node pointed by NODE into the file FILE.  */

static void
dump_decl_name (FILE *file, tree node)
{
  gcc_assert (DECL_P (node));
  fprintf (file, "'");

  mark_decl_referenced (node);

  if (DECL_ASSEMBLER_NAME_SET_P (node))
    {
      const char *tname = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (node));

      if (tname[0] == '*')
        fprintf (file, tname + 1);
      else
        fprintf (file, tname);
    }
  else if (DECL_NAME (node))
    fprintf (file, IDENTIFIER_POINTER (DECL_NAME (node)));
  else
    fprintf (file, "?UNNAMED%d", DECL_UID (node));

  fprintf (file, "'");
}

/* Dump the name of a STRING_CST node.  */

static void
dump_string_name (FILE* file, tree node)
{
  gcc_assert (TREE_CODE (node) == STRING_CST);

  fprintf (file, "'?string%u'", get_string_cst_id (node));
}

/* Dump the name of a label pointed by NODE into the file FILE.  */

static void
dump_label_name (FILE *file, tree node)
{
  /* Always print the label id. */
  fprintf (file, "?L" HOST_WIDE_INT_PRINT_DEC, LABEL_DECL_UID (node));

  /* For convenience, also print the identifier when available.  Note that the
     identifier alone is incorrect: in case of inlining, several labels can
     end up with the same id. */
  if (DECL_NAME (node))
    fprintf (file, "_%s", IDENTIFIER_POINTER (DECL_NAME (node)));
}

/* Dump the name of a valuetype.
   T must be an aggregate type or an enumeral type, since these are
   the types emitted as CIL valuetypes.  */

static void
dump_valuetype_name (FILE *file, tree t)
{
  tree name = TYPE_NAME (t);
  const char *str;
  size_t i;
  bool builtin_p = cil_builtin_type_p (t);

  gcc_assert (AGGREGATE_TYPE_P (t) || TREE_CODE (t) == ENUMERAL_TYPE);
  gcc_assert (TYPE_MAIN_VARIANT (t) == t);
  gcc_assert (TYPE_FILE_SCOPE_P (t));
  gcc_assert (DECL_P (name) || TREE_CODE (name) == IDENTIFIER_NODE);

  if (builtin_p)
    fprintf (file, "[gcc4net]gcc4net.");

  fputs ("'", file);

  if (TREE_CODE (name) == IDENTIFIER_NODE)
    str = IDENTIFIER_POINTER (name);
  else if (DECL_NAME (name))
    str = IDENTIFIER_POINTER (DECL_NAME (name));
  else
    str = NULL;

  if (str)
    {
      /* Dots inside the names of non builtin types are turned into '?'
	 characters.  */
      for (i = 0; i < strlen (str); i++)
	{
	  if (str[i] == '.' && !builtin_p)
            fprintf (file, "?");
	  else
            fprintf (file, "%c", str[i]);
	}
    }
  else
    fprintf (file, "?UNNAMED%d", DECL_UID (name));

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
dump_fun_type (FILE *file, tree fun_type, tree fun, const char *name, bool ref)
{
  tree args_type;
  tree last_arg_type = NULL;
  bool varargs = false;

  gcc_assert (! (fun && name));

  if (ref)
    mark_referenced_type (fun_type);

  args_type = TYPE_ARG_TYPES (fun_type);

  if (args_type == NULL)
    {
      warning (OPT_Wcil_missing_prototypes,
	       "Missing function %s prototype, guessing it, you should fix the "
	       "code", fun ? IDENTIFIER_POINTER (DECL_NAME (fun)) : "");
    }
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

  fprintf (file, "%s", varargs ? "vararg " : "");
  dump_type (file, TREE_TYPE (fun_type), ref, false);
  fprintf (file, " ");

  if (fun)
    {
      struct fnct_attr attrs;

      decode_function_attrs (fun, &attrs);

      if (attrs.assembly_name)
	fprintf (file, "[%s]", attrs.assembly_name);
      else if (TARGET_GCC4NET_LINKER && DECL_EXTERNAL (fun) && TREE_PUBLIC (fun))
	{
	  fputs ("[ExternalAssembly]", file);
	  if (!attrs.cil_name)
	    fputs ("ExternalAssembly::", file);
	}

      if (attrs.cil_name)
	fprintf (file, "%s", attrs.cil_name);
      else
	dump_decl_name (file, fun);
    }

  fprintf (file, "%s(", name ? name : "");

  while (args_type != last_arg_type)
    {
      dump_type (file, TREE_VALUE (args_type), ref, true);
      args_type = TREE_CHAIN (args_type);

      if (args_type != last_arg_type)
	fprintf (file, ", ");
    }

  fprintf (file, ")");
}

/* Dump vector type TYPE.

   FULL tells whether the type must be qualified with the 'valuetype' keyword,
   assembly and namespace.
   TYPE must be a type node of type VECTOR_TYPE.   */

static void
dump_vector_type (FILE *file, tree node, bool full)
{
  tree innertype = TYPE_MAIN_VARIANT (TREE_TYPE (node));
  unsigned HOST_WIDE_INT size = tree_low_cst (TYPE_SIZE (innertype), 1);

  gcc_assert (TREE_CODE (node) == VECTOR_TYPE);

  if (full)
    fprintf (file, "valuetype [gcc4net]gcc4net.");

  fprintf (file, "V"HOST_WIDE_INT_PRINT_UNSIGNED, TYPE_VECTOR_SUBPARTS (node));

  if (TREE_CODE (innertype) == INTEGER_TYPE)
    {
      switch (size)
	{
	case 8:  fprintf (file, "QI"); break;
	case 16: fprintf (file, "HI"); break;
	case 32: fprintf (file, "SI"); break;
	case 64: fprintf (file, "DI"); break;
	default:
	  internal_error ("Unsupported integer size"
			  HOST_WIDE_INT_PRINT_UNSIGNED"\n", size);
	}
    }
  else if (TREE_CODE (innertype) == REAL_TYPE)
    {
      if (size == 32)
	fprintf (file, "SF");
      else if (size == 64)
	fprintf (file, "DF");
      else
	{
	  internal_error ("Unsupported floating point size"
			  HOST_WIDE_INT_PRINT_UNSIGNED"\n", size);
	}
    }
  else
    gcc_unreachable ();
}

static void
dump_complex_type (FILE *file, tree node, bool full)
{
  tree elem_type = TYPE_MAIN_VARIANT (TREE_TYPE (node));
  const char *name = NULL;
  const char *prefix = full ? "valuetype [gcc4net]gcc4net." : "";
  unsigned HOST_WIDE_INT size = tree_low_cst (TYPE_SIZE (elem_type), 1);
  bool unsignedp;

  gcc_assert (TREE_CODE (node) == COMPLEX_TYPE);

  if (INTEGRAL_TYPE_P (elem_type))
    {
      unsignedp = TYPE_UNSIGNED (elem_type);

      switch (size)
	{
	case 8:  name = unsignedp ? "uchar" : "char";   break;
	case 16: name = unsignedp ? "ushort" : "short"; break;
	case 32: name = unsignedp ? "uint" : "int";     break;
	case 64: name = unsignedp ? "ulong" : "long";   break;
	default:
	  internal_error ("Unsupported integer size"
			  HOST_WIDE_INT_PRINT_UNSIGNED"\n", size);
	}
    }
  else if (SCALAR_FLOAT_TYPE_P (elem_type))
    {
      if (size == 32)
	name = "float";
      else if (size == 64)
	name = "double";
      else
	{
	  internal_error ("Unsupported floating point size"
			  HOST_WIDE_INT_PRINT_UNSIGNED"\n", size);
	}
    }

  fprintf (file, "%scomplex_%s", prefix, name);
}

static void
emit_enum_decl (FILE *file, tree t)
{
  unsigned HOST_WIDE_INT size = tree_low_cst (TYPE_SIZE (t), 1);
  char tmp_str[8] = "int";
  char *base_type_str = tmp_str;
  tree tmp;

  gcc_assert (t != NULL_TREE && TREE_CODE (t) == ENUMERAL_TYPE);

  fprintf (file, "\n.class ");

  if (TYPE_FILE_SCOPE_P (t))
    fprintf (file, "public ");
  else
    fprintf (file, "private ");

  fprintf (file, "sealed serializable ansi ");
  dump_valuetype_name (file, t);
  fprintf (file,
           " extends ['mscorlib']System.Enum\n"
           "{\n");
  snprintf (base_type_str + 3, 5, HOST_WIDE_INT_PRINT_UNSIGNED, size);
  fprintf (file, "\t.field public specialname rtspecialname %s%s 'value__'\n",
	   !TYPE_UNSIGNED (t) ? "unsigned " : "",
	   base_type_str);

  for (tmp = TYPE_VALUES (t); tmp; tmp = TREE_CHAIN (tmp))
    {
      fprintf (file, "\t.field public static literal ");
      dump_type (file, t, false, false);
      gcc_assert (TREE_CODE (TREE_PURPOSE (tmp)) == IDENTIFIER_NODE);
      fprintf (file, " '%s' = %s(", IDENTIFIER_POINTER (TREE_PURPOSE (tmp)),
	       base_type_str);
      dump_double_int (file, TREE_INT_CST (TREE_VALUE (tmp)), false);
      fprintf (file, ")\n");
    }

  fprintf (file, "}\n");
}

static void
emit_array_decl (FILE *file, tree t)
{
  fprintf (file, "\n.class %s explicit sealed serializable ansi ",
	   TYPE_FILE_SCOPE_P (t) ? "public" : "private");
  dump_valuetype_name (file, t);
  fprintf (file,
	   " extends ['mscorlib']System.ValueType\n"
	   "{\n"
	   "\t.custom instance "
	   "void [gcc4net]gcc4net.C_Attributes.ArrayType::.ctor() "
	   "= (01 00 00 00)\n"
	   "\t.size " HOST_WIDE_INT_PRINT_DEC "\n"
	   "\t.field [0] public specialname ",
	   tree_low_cst (TYPE_SIZE_UNIT (t), 1));
  dump_type (file, TREE_TYPE (t), false, false);
  fprintf (file,
	   " 'elem__'\n"
	   "}\n");
}

static void
emit_incomplete_decl (FILE *file, tree t)
{
  if (t == NULL_TREE || t == error_mark_node)
    return;

  if (!AGGREGATE_TYPE_P (t) && TREE_CODE (t) != ENUMERAL_TYPE)
    return;

  gcc_assert (file != 0);

  gcc_assert (TYPE_MAIN_VARIANT (t) == t);
  gcc_assert (TYPE_NAME (t));
  gcc_assert (!COMPLETE_TYPE_P (t));

  fputs ("\n.class public sealed ", file);
  dump_valuetype_name (file, t);
  fputs (" extends ['mscorlib']System.ValueType\n"
	 "{\n"
	 "\t.custom instance "
	 "void [gcc4net]gcc4net.C_Attributes.IncompleteType::.ctor() "
	 "= (01 00 00 00)\n"
	 "}\n", file);
}

static void
emit_struct_union_decl (FILE *file, tree t)
{
  tree tmp;

  fprintf (file, "\n.class %s explicit sealed serializable ansi ",
	   TYPE_FILE_SCOPE_P (t) ? "public" : "private");
  dump_valuetype_name (file, t);
  fprintf (file,
	   " extends ['mscorlib']System.ValueType\n"
	   "{\n"
	   "\t.size " HOST_WIDE_INT_PRINT_DEC "\n",
	   tree_low_cst (TYPE_SIZE_UNIT (t), 1));

  for (tmp = TYPE_FIELDS (t); tmp; tmp = TREE_CHAIN (tmp))
    {
      if (DECL_NAME (tmp) == NULL_TREE && DECL_BIT_FIELD (tmp))
	{
	  /* Skip unnamed bitfields */
	}
      else
	{
	  tree type;
	  unsigned HOST_WIDE_INT bit_offset;
	  unsigned HOST_WIDE_INT byte_offset;
	  unsigned HOST_WIDE_INT offset;

	  bit_offset = tree_low_cst (DECL_FIELD_BIT_OFFSET (tmp), 1);
	  byte_offset = tree_low_cst (DECL_FIELD_OFFSET (tmp), 1);

	  if (DECL_BIT_FIELD (tmp))
	    {
	      unsigned HOST_WIDE_INT size;

	      type = DECL_BIT_FIELD_TYPE (tmp);

	      gcc_assert (TREE_CODE (TREE_TYPE (tmp)) == INTEGER_TYPE
			  || TREE_CODE (TREE_TYPE (tmp)) == BOOLEAN_TYPE);
	      gcc_assert (TREE_CODE (type) == INTEGER_TYPE
			  || TREE_CODE (type) == BOOLEAN_TYPE
			  || TREE_CODE (type) == ENUMERAL_TYPE);

	      size = tree_low_cst (TYPE_SIZE (type), 1);
	      offset = byte_offset + (bit_offset & ~(size - 1)) / 8;
	    }
	  else
	    {
	      type = TREE_TYPE (tmp);
	      gcc_assert (bit_offset % 8 == 0);
	      offset = byte_offset + bit_offset / 8;
	    }

	  fprintf (file, "\t.field ["HOST_WIDE_INT_PRINT_UNSIGNED"] public ",
		   offset);
	  dump_type (file, type, false, false);
	  fprintf (file, " ");
	  dump_decl_name (file, tmp);
	  fprintf (file, "\n");
	}
    }

  fprintf (file, "}\n");
}

static void
emit_valuetype_decl (FILE *file, tree t)
{
  if (t == NULL_TREE || t == error_mark_node)
    return;

  if (!AGGREGATE_TYPE_P (t) && TREE_CODE (t) != ENUMERAL_TYPE)
    return;

  gcc_assert (file != 0);
  gcc_assert (TYPE_MAIN_VARIANT (t) == t);
  gcc_assert (TYPE_NAME (t));

  if (TREE_CODE (t) == ENUMERAL_TYPE)
    emit_enum_decl (file, t);
  else if (TREE_CODE (t) == ARRAY_TYPE)
    emit_array_decl (file, t);
  else /* struct or union */
    emit_struct_union_decl (file, t);
}

/* Dump type TYPE.
   REF tells whether the function type (and the types of the return value
   and of the incoming parameters) have to be marked as referenced.
   QUALIF tells whether to emit C qualifiers (const, restrict, volatile)
   NODE must be a type node.   */

static void
dump_type (FILE *file, tree type, bool ref, bool qualif)
{
  unsigned HOST_WIDE_INT size;

  if (type == NULL_TREE || type == error_mark_node)
    return;

  if (TYPE_MAIN_VARIANT (type) == cil32_arg_iterator_type)
    {
      fprintf (file, "valuetype [mscorlib]System.ArgIterator");
      return;
    }

  switch (TREE_CODE (type))
    {
    /* Incomplete and variable-length arrays are pointers and
       they must be dealt with as such.   */
    case ARRAY_TYPE:
      if (!TYPE_DOMAIN (type) || ARRAY_TYPE_VARLENGTH (type))
        goto pointer;

    case ENUMERAL_TYPE:
    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      /* Reference the type if told to do so */
      if (ref)
        mark_referenced_type (TYPE_MAIN_VARIANT (type));

      /* Print the name of the structure.  */
      fprintf (file, "valuetype ");
      dump_valuetype_name (file, TYPE_MAIN_VARIANT (type));
      break;

    case VOID_TYPE:
      fprintf (file, "void");
      break;

    case INTEGER_TYPE:
      fprintf (file, "%s", TYPE_UNSIGNED (type) ? "unsigned " : "");
      size = tree_low_cst (TYPE_SIZE (type), 1);

      switch (size)
	{
	  case 8:  fprintf (file, "int8");  break;
	  case 16: fprintf (file, "int16"); break;
	  case 32: fprintf (file, "int32"); break;
	  case 64: fprintf (file, "int64"); break;
	  default:
	    internal_error ("Unsupported integer size "
                            HOST_WIDE_INT_PRINT_UNSIGNED"\n", size);
	}
      break;

    case REAL_TYPE:
      size = tree_low_cst (TYPE_SIZE (type), 1);

      gcc_assert (size == 32 || size == 64);
      fprintf (file, "float" HOST_WIDE_INT_PRINT_UNSIGNED, size);
      break;

    case BOOLEAN_TYPE:
      fprintf (file, "int8");
      break;

pointer:
    case POINTER_TYPE:
      if (TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
	{
	  fprintf (file, "method ");
	  dump_fun_type (file, TREE_TYPE (type), NULL, " * ", ref);
	}
      else
	{
	  dump_type (file, TREE_TYPE (type), ref, qualif);
	  fprintf (file, " *");
	}
      break;

    case FUNCTION_TYPE:
/*    FIXME?
      dump_fun_type (file, node, NULL, NULL, ref); */
      gcc_unreachable ();
      break;

    case VECTOR_TYPE:
      dump_vector_type (file, type, true);
      break;

    case COMPLEX_TYPE:
      dump_complex_type (file, type, true);
      break;

    case REFERENCE_TYPE:

    default:
      gcc_unreachable ();
    }

  if (qualif)
    {
      unsigned int quals = TYPE_QUALS (type);

      if (quals & TYPE_QUAL_CONST)
	fprintf (file, " modopt([gcc4net]gcc4net.CQualifiers.IsConst)");
      if (quals & TYPE_QUAL_RESTRICT)
	fprintf (file, " modopt([gcc4net]gcc4net.CQualifiers.IsRestrict)");
#if 0
      /* FIXME? */
      if (quals & TYPE_QUAL_VOLATILE)
	fprintf (file, " modopt([gcc4net]gcc4net.CQualifiers.IsVolatile)");
#endif
    }
}

static void
dump_string_type (FILE *file, tree node)
{
  tree domain;
  tree min;
  tree max;

  gcc_assert(TREE_CODE (node) == ARRAY_TYPE
             && TYPE_DOMAIN (node)
             && ! ARRAY_TYPE_VARLENGTH (node));

  domain = TYPE_DOMAIN (node);
  min = TYPE_MIN_VALUE (domain);
  max = TYPE_MAX_VALUE (domain);
  gcc_assert(integer_zerop (min));
  fprintf (file, "valuetype '?string_type?"HOST_WIDE_INT_PRINT_UNSIGNED"'",
	   tree_low_cst (max, 1) + 1);
}

/* Print a string declaration pointed by T into the file pointed by FILE.  */

static void
dump_string_decl (FILE *file, tree t)
{
  const char *str;
  unsigned int len, len_type, i;

  str = TREE_STRING_POINTER (t);
  len = TREE_STRING_LENGTH (t);
  len_type = tree_low_cst (TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (t))), 1) + 1;

  /* Emit the string in readable form as a comment. */
  fprintf (file, "\n// string: \"");

  for (i = 0; i < len - 1; i++)
    {
      switch (str[i])
	{
	case '\n': fprintf (file, "\\n"); break;
	case '"':  fprintf (file, "\"");  break;
	default:   fputc (str[i], file);  break;
	}
    }

  fprintf (file,
	   "\"\n"
	   ".data 'DataStr%u' = bytearray(", get_string_cst_id (t));

  for (i = 0; i < len; i++)
    fprintf (file, "%02x ", (unsigned char) str[i]);

  for (; i < len_type; i++)
    fprintf (file, "00 ");

  fprintf (file,
	   ")\n"
	   ".field private static ");
  dump_string_type (file, TREE_TYPE (t));
  fprintf (file, " ");
  dump_string_name (file, t);
  fprintf (file, " at 'DataStr%u'\n", get_string_cst_id (t));
}

/* Dump the type def of type NODE, promoted following C conventions
   for var args.
   NODE must be a type node.
   returns true if the dumped type is a pointer */

static bool
dump_type_promoted_type_def (FILE *file, tree node)
{
  bool result = false;
  unsigned HOST_WIDE_INT size;

  if (node == NULL_TREE || node == error_mark_node)
    return false;

  switch (TREE_CODE (node))
    {
    /* Incomplete and variable-length arrays are pointers and
       they must be dealt with as such.   */
    case ARRAY_TYPE:
      if (!TYPE_DOMAIN (node) || ARRAY_TYPE_VARLENGTH (node))
	goto pointer;

    case RECORD_TYPE:
    case UNION_TYPE:
    case QUAL_UNION_TYPE:
      node = TYPE_MAIN_VARIANT (node);

      /* Reference the type if told to do so */
      mark_referenced_type (node);

      /* Print the name of the structure.  */
      fprintf (file, "valuetype ");
      dump_valuetype_name (file, node);
      break;

    case COMPLEX_TYPE:
      dump_complex_type (file, node, true);
      break;

    case ENUMERAL_TYPE:
    case INTEGER_TYPE:
    case BOOLEAN_TYPE:
      size = tree_low_cst (TYPE_SIZE (node), 1);

      if (size <= 32)
	fprintf (file, "class [mscorlib]System.UInt32");
      else if (size <= 64)
	fprintf (file, "class [mscorlib]System.UInt64");
      else
	{
	  internal_error ("Unsupported integer size"
			  HOST_WIDE_INT_PRINT_UNSIGNED"\n", size);
	}
      break;

    case REAL_TYPE:
      fprintf (file, "class [mscorlib]System.Double");
      break;

    pointer:
    case POINTER_TYPE:
      /* cil32 is a 32bit machine, in case we support 64bit model
       * changes are needed
       */
      fprintf (file, "class [mscorlib]System.UInt32");
      result = true;
      break;

    default:
      gcc_unreachable ();
    }

  return result;
}

/* Emit all the strings referenced in this compilation unit.  */

static void
emit_referenced_strings (void)
{
  FILE *file = asm_out_file;
  htab_iterator hti;
  str_ref ref;
  tree str;
  unsigned int str_size;
  ebitmap used_stringtypes;
  ebitmap_iterator ebi;

  used_stringtypes = ebitmap_alloc (1);

  FOR_EACH_HTAB_ELEMENT (referenced_strings_htab (), ref, str_ref, hti)
    {
      str = ref->cst;
      str_size = tree_low_cst (TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (str))),
			       1) + 1;
      dump_string_decl (file, str);
      ebitmap_set_bit (used_stringtypes, str_size);
    }

  EXECUTE_IF_SET_IN_EBITMAP(used_stringtypes, 0, str_size, ebi)
    {
      fprintf (file,
	       "\n.class public explicit sealed serializable ansi "
	       "'?string_type?%u'", str_size);
      fprintf (file,
	       " extends ['mscorlib']System.ValueType\n"
	       "{\n"
	       "\t.custom instance "
	       "void [gcc4net]gcc4net.C_Attributes.ConstStringType::.ctor() "
	       "= (01 00 00 00)\n"
	       "\t.size %u\n", str_size);
      fprintf (file,
	       "\t.field [0] public specialname int8 'elem__'\n"
	       "}\n");
    }

  ebitmap_free (used_stringtypes);
}

static bool
emit_incomplete_type (void *elem, void *data)
{
  tree type = (tree) elem;
  tree type_name = TYPE_NAME (type);
  struct pointer_set_t *emitted_types = data;

  gcc_assert (DECL_P (type_name)
	      || TREE_CODE (type_name) == IDENTIFIER_NODE);

  if (TREE_CODE (type_name) != IDENTIFIER_NODE)
      type_name = DECL_NAME (type_name);

  if (!pointer_set_contains (emitted_types, type_name))
    {
      emit_incomplete_decl (asm_out_file, type);
      pointer_set_insert (emitted_types, type_name);
    }

  return true;
}

/* Emit the valuetypes referenced by the current function.  */

static void
emit_referenced_types (void)
{
  /* There may be distinct tree types that correspond to identical types.
     In order not to slow down mark_referenced_type(...) function (which
     may typically be called several times for the same type), insertion
     of types in the mark set makes only sure that the same tree type
     pointer is not inserted twice. As a consequence, there may still be
     distinct tree types that correspond to identical types in the
     reference type set.
     Hence, before emitting a type, make sure no type with the same name
     has already been emitted.  */

  FILE *file = asm_out_file;
  htab_iterator hti;
  tree type;
  struct pointer_set_t *emitted_types = pointer_set_create ();
  struct pointer_set_t *incomplete_types = pointer_set_create ();

  FOR_EACH_HTAB_ELEMENT (referenced_types_htab (), type, tree, hti)
    {
      if (COMPLETE_TYPE_P (type))
	{
	  tree type_name = TYPE_NAME (type);

	  gcc_assert (DECL_P (type_name)
		      || TREE_CODE (type_name) == IDENTIFIER_NODE);

	  if (TREE_CODE (type_name) != IDENTIFIER_NODE)
	    type_name = DECL_NAME (type_name);

	  if (!pointer_set_contains (emitted_types, type_name))
	    {
	      emit_valuetype_decl (file, type);
	      pointer_set_insert (emitted_types, type_name);
	    }
	}
      else if (TARGET_GCC4NET_LINKER)
	pointer_set_insert (incomplete_types, type);
    }

  /* emit incomplete types */
  pointer_set_traverse (incomplete_types, emit_incomplete_type, emitted_types);
  pointer_set_destroy (incomplete_types);
  pointer_set_destroy (emitted_types);
}

static void
emit_pinvoke_function (FILE *file, tree fun)
{
  tree fun_type = TREE_TYPE (fun);
  struct fnct_attr attributes;

  decode_function_attrs (fun, &attributes);

  fprintf (file, ".method %s static pinvokeimpl(\"%s\"",
	   TREE_PUBLIC (fun) ? "public" : "private",
	   attributes.pinvoke_assembly);

  if (attributes.pinvoke_fname)
    fprintf (file, " as \"%s\"", attributes.pinvoke_fname);

  fprintf (file, ") ");
  DECL_EXTERNAL (fun) = 0;
  dump_fun_type (file, fun_type, fun, NULL, false);
  fprintf (file, " cil managed {}\n");
}

/* Emit the PINVOKES referenced in this compilation unit.  */

static void
emit_referenced_pinvokes (void)
{
  FILE *file = asm_out_file;
  htab_iterator hti;
  tree pinvoke;

  FOR_EACH_HTAB_ELEMENT (pinvokes_htab (), pinvoke, tree, hti)
    {
      emit_pinvoke_function (file, pinvoke);
    }
}

/* Emit an indirect access.  */

static void
emit_prefixes (FILE *file, const_cil_stmt stmt)
{
    enum cil_opcode opcode = cil_opcode (stmt);

    if ((opcode == CIL_CALL) || (opcode == CIL_CALLI))
    {
        if (cil_prefix_tail (stmt))
            fprintf (file, "\n\ttail.");
    }
    else if ((opcode == CIL_CPBLK) || (opcode == CIL_INITBLK)
            || ((opcode >= CIL_LDIND_I1) && (opcode <= CIL_LDIND_I))
            || ((opcode >= CIL_STIND_I1) && (opcode <= CIL_STIND_I))
            || (opcode == CIL_LDFLD) || (opcode == CIL_STFLD))
    {
        if (cil_prefix_unaligned (stmt) != 0)
            fprintf (file, "\n\tunaligned.%d", cil_prefix_unaligned (stmt));
    }
    else if ((opcode == CIL_CPBLK) || (opcode == CIL_INITBLK)
            || ((opcode >= CIL_LDIND_I1) && (opcode <= CIL_LDIND_I))
            || ((opcode >= CIL_STIND_I1) && (opcode <= CIL_STIND_I))
            || (opcode == CIL_LDFLD) || (opcode == CIL_STFLD)
            || (opcode == CIL_LDSFLD) || (opcode == CIL_STSFLD))
    {
        if (cil_prefix_volatile (stmt))
            fprintf (file, "\n\tvolatile.");
    }
}

/* Emit a CIL LDSFLD instruction.  */

static void
emit_ldsfld (FILE *file, const_cil_stmt stmt)
{
  tree arg = cil_field (stmt);

  fprintf (file, "\n\tldsfld\t");
  dump_type (file, TREE_TYPE (arg), true, false);
  fprintf (file, " ");

  if (TARGET_GCC4NET_LINKER && DECL_EXTERNAL (arg) && TREE_PUBLIC (arg))
    fprintf (file, "[ExternalAssembly]ExternalAssembly::");

  dump_decl_name (file, arg);
}

/* Emit a CIL LDSFLDA instruction.  This instruction is used for obtaining the
   addresses of global variables but also string constants.  */

static void
emit_ldsflda (FILE *file, const_cil_stmt stmt)
{
  tree field = cil_field (stmt);

  fprintf (file, "\n\tldsflda\t");

  if (TREE_CODE (field) == STRING_CST)
    {
      dump_string_type (file, TREE_TYPE (field));
      fprintf (file, " ");
      dump_string_name (file, field);
    }
  else
    {
      gcc_assert (TREE_CODE (field) == VAR_DECL);

      if (COMPLETE_TYPE_P (TREE_TYPE (field)))
	dump_type (file, TREE_TYPE (field), true, false);
      else
	fprintf (file, "native int");

      fprintf (file, " ");

      if (TARGET_GCC4NET_LINKER && DECL_EXTERNAL (field) && TREE_PUBLIC (field))
	fprintf (file, "[ExternalAssembly]ExternalAssembly::");

      dump_decl_name (file, field);
    }
}

/* Emit a CIL LDFLD instruction.  */

static void
emit_ldfld (FILE *file, const_cil_stmt stmt)
{
  tree field = cil_field (stmt);
  tree obj_type = DECL_FIELD_CONTEXT (field);

  mark_referenced_type (obj_type);
  fprintf (file, "\n\tldfld\t");
  dump_type (file, TREE_TYPE (field), true, false);
  fprintf (file, " ");
  mark_referenced_type (obj_type);
  dump_valuetype_name (file, obj_type);
  fprintf (file, "::");
  dump_decl_name (file, field);
}

/* Emit a CIL LDFLDA instruction.  */

static void
emit_ldflda (FILE *file, const_cil_stmt stmt)
{
  tree field = cil_field (stmt);
  tree obj_type = DECL_FIELD_CONTEXT (field);

  mark_referenced_type (obj_type);
  fprintf (file, "\n\tldflda\t");
  dump_type (file, TREE_TYPE (field), true, false);
  fprintf (file, " ");
  dump_valuetype_name (file, obj_type);
  fprintf (file, "::");
  dump_decl_name (file, field);
}

/* Emit a CIL STFLD instruction.  */

static void
emit_stfld (FILE *file, const_cil_stmt stmt)
{
  tree field = cil_field (stmt);
  tree obj_type = DECL_FIELD_CONTEXT (field);

  mark_referenced_type (obj_type);
  fprintf (file, "\n\tstfld\t");
  dump_type (file, TREE_TYPE (field), true, false);
  fprintf (file, " ");
  dump_valuetype_name (file, obj_type);
  fprintf (file, "::");
  dump_decl_name (file, field);
}

/* Emit a CIL STSFLD instruction.  */

static void
emit_stsfld (FILE *file, const_cil_stmt stmt)
{
  tree arg = cil_field (stmt);

  fprintf (file, "\n\tstsfld\t");
  dump_type (file, TREE_TYPE (arg), true, false);
  fprintf (file, " ");

  if (TARGET_GCC4NET_LINKER && DECL_EXTERNAL (arg) && TREE_PUBLIC (arg))
    fprintf (file, "[ExternalAssembly]ExternalAssembly::");

  dump_decl_name (file, arg);
}

/* Emit a SWITCH instruction.  */

static void
emit_switch (FILE *file, const_cil_stmt stmt)
{
  unsigned HOST_WIDE_INT i, range;
  unsigned int n_cases = cil_switch_ncases (stmt);
  HOST_WIDE_INT lo, hi, offset, j;
  tree *labels;

  if (n_cases > 1)
    {
      lo = cil_switch_case_low (stmt, 0);
      hi = cil_switch_case_high (stmt, n_cases - 2);
      range = hi - lo + 1;
      labels = XCNEWVEC (tree, range);
      offset = lo;
    }
  else
    {
      /* If the switch contains only one case, it's the default label. If the
         default label is artificial then this is an empty switch we created
	 for a computed GOTO expression. */
      if (DECL_ARTIFICIAL (CASE_LABEL (cil_switch_default (stmt))))
	range = 0;
      else
	range = 1;

      labels = XCNEWVEC (tree, 1);
      offset = 0;
    }

  /* Flatten the cases filling an array of adjacent cases with their
     associated labels taken from the switch. */

  for (i = 0; i < n_cases - 1; i++)
    {
      lo = cil_switch_case_low (stmt, i);
      hi = cil_switch_case_high (stmt, i);

      for (j = lo; j <= hi; j++)
        labels[j - offset] = cil_switch_case_label (stmt, i);
    }

  /* Fill the empty slots in the array with the default case label. */

  for (i = 0; i < range; i++)
    {
      if (labels[i] == NULL_TREE)
	labels[i] = CASE_LABEL (cil_switch_default (stmt));
    }

  fprintf (file, "\n\tswitch\t(");

  for (i = 0 ; i < range; i++)
    {
      dump_label_name (file, labels[i]);

      if (i < range - 1)
	fprintf (file, ", ");
    }

  fprintf (file, ")");

  XDELETEVEC (labels);
}

/* The attribute string must be less than 127 characters and contain only
   7-bit ASCII chars.  No checks though.  */

static void
emit_string_custom_attr (FILE *file, const char *parameter)
{
  size_t i;
  size_t len = strlen (parameter);

  gcc_assert (len <= 127);

  fprintf (file, "\t.custom instance void [mscorlib]System.String::.ctor(int8 *) = (01 00 %02x ", len);

  for (i = 0; i < len; i++)
    fprintf (file, "%02x ", parameter[i]);

  fprintf (file, "00 00)\n");
}

/* Emit the assembler representing a built in call. Return true if the built in
   was handled, false if it must be emitted like a regular function call.  */

static bool
emit_builtin_call (FILE *file, const_cil_stmt call)
{
  tree fdecl = cil_call_fdecl (call);

  if (DECL_BUILT_IN_CLASS (fdecl) == BUILT_IN_MD)
    {
      switch (DECL_FUNCTION_CODE (fdecl))
	{
	case CIL32_BUILT_IN_VA_START:
	  fprintf (file,
		   "\n\targlist"
		   "\n\tcall\tinstance void "
		   "[mscorlib]System.ArgIterator::.ctor(valuetype "
		   "[mscorlib]System.RuntimeArgumentHandle)");
	  return true;

	case CIL32_BUILT_IN_VA_END:
	  fprintf (file, "\n\tcall\tinstance void [mscorlib]System.ArgIterator::End()");
	  return true;

	case CIL32_BUILT_IN_VA_COPY:
	  fprintf (file, "\n\tcpobj\t[mscorlib]System.ArgIterator");
	  return true;

	case CIL32_BUILT_IN_VA_INIT:
	  fprintf (file, "\n\tinitobj\tvaluetype [mscorlib]System.ArgIterator");
	  return true;

	case CIL32_BUILT_IN_VA_ARG:
	  {
	    tree type = TREE_TYPE (TREE_TYPE (cil_call_dummy_arg (call)));

	    fprintf (file,
		     "\n\tcall\tinstance typedref [mscorlib]System.ArgIterator::GetNextArg()"
		     "\n\trefanyval ");

	    if (dump_type_promoted_type_def (file, type))
		fprintf (file, "\n\tconv.i");
	  }
	  return true;

	default:
	  return false;
	}
    }
  else
    {
      switch (DECL_FUNCTION_CODE (fdecl))
	{
	case BUILT_IN_VA_START:
	case BUILT_IN_VA_END:
	case BUILT_IN_VA_COPY:
	  internal_error ("va_* builtins shouldn't be present\n");
	  break;

	case BUILT_IN_TRAP:
	  fprintf (file, "\n\tcall\tvoid 'abort' ()");
	  return true;

	default:
	  /* Go Ahead as a normal function call */
          return false;
	}
    }

  return false;
}

/* Emit the assembler representing the CALL instruction into the file pointed
   by STREAM.  */

static void
emit_call (FILE *file, const_cil_stmt call)
{
  bool direct = cil_opcode (call) == CIL_CALLI ? false : true;
  tree ftype = cil_call_ftype (call);
  tree fdecl;
  size_t nargs_base;
  size_t nargs;
  size_t i;

  switch (cil_opcode (call))
    {
    case CIL_CALL:
      if (DECL_BUILT_IN (cil_call_fdecl (call)))
	if (emit_builtin_call (file, call))
	  return;

      fprintf (file, "\n\tcall\t");
      break;

    case CIL_CALLI:
      fprintf (file, "\n\tcalli\t");
      break;

    case CIL_JMP:
      fprintf (file, "\n\tjmp\t");
      break;

    default:
      gcc_unreachable ();
    }

  fprintf (file, "%s", cil_call_vararg_p (call) ? "vararg " : "");
  /* Dump the return type.  */
  dump_type (file, TREE_TYPE (ftype), true, false);

  if (direct)
    {
      struct fnct_attr attrs;

      fdecl = cil_call_fdecl (call);
      decode_function_attrs (fdecl, &attrs);

      fprintf (file, " ");

      if (DECL_BUILT_IN (fdecl) && DECL_BUILT_IN_CLASS (fdecl) == BUILT_IN_MD)
	fprintf (file, "%s", IDENTIFIER_POINTER (DECL_NAME (fdecl)));
      else
      	{
	  if (attrs.assembly_name)
	    fprintf (file, "[%s]", attrs.assembly_name);
	  else if (TARGET_GCC4NET_LINKER
		   && DECL_EXTERNAL (fdecl)
		   && TREE_PUBLIC (fdecl))
	    {
	      fputs ("[ExternalAssembly]", file);
	      if (!attrs.cil_name)
		fputs ("ExternalAssembly::", file);
	    }

	  if (attrs.cil_name)
	    fprintf (file, "%s", attrs.cil_name);
	  else
	    dump_decl_name (file, fdecl);
	}
    }

  fprintf (file, " (");
  nargs_base = cil_call_nargs_base (call);
  nargs = cil_call_nargs (call);

  if (cil_call_static_chain (call))
    {
      dump_type (file, cil_call_static_chain (call), true, true);

      if (nargs_base > 0)
	fprintf (file, ", ");
    }

  for (i = 0; i < nargs_base; i++)
    {
      if (i != 0)
	fprintf (file, ", ");

      dump_type (file, cil_call_arg_type (call, i), true, true);
    }

  if (cil_call_vararg_p (call) && i < nargs)
    fprintf (file, ", ..., ");

  for (; i < nargs; i++)
    {
      tree type = cil_call_arg_type (call, i);

      if (i != nargs_base)
	fprintf (file, ", ");

      if (cil_call_missing_proto_p (call) && cil_pointer_type_p (type))
	fprintf (file, "native int");
      else
	{
	  type = promote_type_for_vararg (type);
	  dump_type (file, type, true, false);
	}
    }

  fprintf (file, ")");
}

/* Emits a single CIL statement STMT the file specified by FILE.  */

static void
emit_cil_stmt (FILE *file, const_cil_stmt stmt)
{
  emit_prefixes (file, stmt);

  switch (cil_opcode (stmt))
    {
    case CIL_ADD:
      fprintf (file, "\n\tadd");
      break;

    case CIL_AND:
      fprintf (file, "\n\tand");
      break;

    case CIL_ARGLIST:
      fprintf (file, "\n\targlist");
      break;

    case CIL_BEQ:
      fprintf (file, "\n\tbeq\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_BGE:
      fprintf (file, "\n\tbge\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_BGE_UN:
      fprintf (file, "\n\tbge.un\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_BGT:
      fprintf (file, "\n\tbgt\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_BGT_UN:
      fprintf (file, "\n\tbgt.un\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_BLE:
      fprintf (file, "\n\tble\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_BLE_UN:
      fprintf (file, "\n\tble.un\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_BLT:
      fprintf (file, "\n\tblt\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_BLT_UN:
      fprintf (file, "\n\tblt.un\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_BNE_UN:
      fprintf (file, "\n\tbne.un\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_BR:
      fprintf (file, "\n\tbr\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_BREAK:
      fprintf (file, "\n\tbreak");
      break;

    case CIL_BRFALSE:
      fprintf (file, "\n\tbrfalse\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_BRTRUE:
      fprintf (file, "\n\tbrtrue\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_CALL:
    case CIL_CALLI:
      emit_call (file, stmt);
      break;

    case CIL_CEQ:
      fprintf (file, "\n\tceq");
      break;

    case CIL_CGT:
      fprintf (file, "\n\tcgt");
      break;

    case CIL_CGT_UN:
      fprintf (file, "\n\tcgt.un");
      break;

    case CIL_CKFINITE:
      fprintf (file, "\n\tckfinite");
      break;

    case CIL_CLT:
      fprintf (file, "\n\tclt");
      break;

    case CIL_CLT_UN:
      fprintf (file, "\n\tclt.un");
      break;

    case CIL_CONV_I1:
      fprintf (file, "\n\tconv.i1");
      break;

    case CIL_CONV_I2:
      fprintf (file, "\n\tconv.i2");
      break;

    case CIL_CONV_I4:
      fprintf (file, "\n\tconv.i4");
      break;

    case CIL_CONV_I8:
      fprintf (file, "\n\tconv.i8");
      break;

    case CIL_CONV_R4:
      fprintf (file, "\n\tconv.r4");
      break;

    case CIL_CONV_R8:
      fprintf (file, "\n\tconv.r8");
      break;

    case CIL_CONV_U1:
      fprintf (file, "\n\tconv.u1");
      break;

    case CIL_CONV_U2:
      fprintf (file, "\n\tconv.u2");
      break;

    case CIL_CONV_U4:
      fprintf (file, "\n\tconv.u4");
      break;

    case CIL_CONV_U8:
      fprintf (file, "\n\tconv.u8");
      break;

    case CIL_CONV_I:
      fprintf (file, "\n\tconv.i");
      break;

    case CIL_CONV_U:
      fprintf (file, "\n\tconv.u");
      break;

    case CIL_CONV_R_UN:
      fprintf (file, "\n\tconv.r.un");
      break;

    case CIL_CPBLK:
      fprintf (file, "\n\tcpblk");
      break;

    case CIL_DIV:
      fprintf (file, "\n\tdiv");
      break;

    case CIL_DIV_UN:
      fprintf (file, "\n\tdiv.un");
      break;

    case CIL_DUP:
      fprintf (file, "\n\tdup");
      break;

    case CIL_INITBLK:
      fprintf (file, "\n\tinitblk");
      break;

    case CIL_INITOBJ:
      fprintf (file, "\n\tinitobj\t");
      dump_type (file, cil_type (stmt), true, false);
      break;

    case CIL_JMP:
      emit_call (file, stmt);
      break;

    case CIL_LDARG:
      fprintf (file, "\n\tldarg\t");
      dump_decl_name (file, cil_var (stmt));
      break;

    case CIL_LDARGA:
      fprintf (file, "\n\tldarga\t");
      dump_decl_name (file, cil_var (stmt));
      break;

    case CIL_LDC_I4:
      fprintf (file, "\n\tldc.i4\t");
      dump_double_int (file, TREE_INT_CST (cil_cst (stmt)), false);
      break;

    case CIL_LDC_I8:
      fprintf (file, "\n\tldc.i8\t");
      dump_double_int (file, TREE_INT_CST (cil_cst (stmt)), false);
      break;

    case CIL_LDC_R4:
      {
	REAL_VALUE_TYPE d = TREE_REAL_CST (cil_cst (stmt));
	tree type = TYPE_MAIN_VARIANT (TREE_TYPE (cil_cst (stmt)));
	char string[100];
	long buf;

	real_to_target (&buf, &d, TYPE_MODE (type));
	real_to_decimal (string, &d, sizeof (string), 0, 1);
	fprintf (file, "\n\tldc.r4\tfloat32(%#08lx)\t/* %s */",
		 buf, string);
      }
      break;

    case CIL_LDC_R8:
      {
	REAL_VALUE_TYPE d = TREE_REAL_CST (cil_cst (stmt));
	tree type = TYPE_MAIN_VARIANT (TREE_TYPE (cil_cst (stmt)));
	char string[100];
	long buf[2];

	real_to_target (buf, &d, TYPE_MODE (type));
	real_to_decimal (string, &d, sizeof (string), 0, 1);
	fprintf (file, "\n\tldc.r8\tfloat64(%#08lx%08lx)\t/* %s */",
		 buf[1], buf[0], string);
      }
      break;

    case CIL_LDFLD:
      emit_ldfld (file, stmt);
      break;

    case CIL_LDFLDA:
      emit_ldflda (file, stmt);
      break;

    case CIL_LDFTN:
      fprintf (file, "\n\tldftn\t");
      /* FIXME: We shouldn't print the vararg ellipsis */
      dump_fun_type (file, TREE_TYPE (cil_func (stmt)),
		     cil_func (stmt), NULL, true);
      break;

    case CIL_LDIND_I1:
      fprintf (file, "\n\tldind.i1");
      break;

    case CIL_LDIND_I2:
      fprintf (file, "\n\tldind.i2");
      break;

    case CIL_LDIND_I4:
      fprintf (file, "\n\tldind.i4");
      break;

    case CIL_LDIND_I8:
      fprintf (file, "\n\tldind.i8");
      break;

    case CIL_LDIND_U1:
      fprintf (file, "\n\tldind.u1");
      break;

    case CIL_LDIND_U2:
      fprintf (file, "\n\tldind.u2");
      break;

    case CIL_LDIND_U4:
      fprintf (file, "\n\tldind.u4");
      break;

    case CIL_LDIND_U8:
      fprintf (file, "\n\tldind.u8");
      break;

    case CIL_LDIND_R4:
      fprintf (file, "\n\tldind.r4");
      break;

    case CIL_LDIND_R8:
      fprintf (file, "\n\tldind.r8");
      break;

    case CIL_LDIND_I:
      fprintf (file, "\n\tldind.i");
      break;

    case CIL_LDLOC:
      fprintf (file, "\n\tldloc\t");
      dump_decl_name (file, cil_var (stmt));
      break;

    case CIL_LDLOCA:
      fprintf (file, "\n\tldloca\t");
      dump_decl_name (file, cil_var (stmt));
      break;

    case CIL_LDOBJ:
      fprintf (file, "\n\tldobj\t");
      dump_type (file, cil_type (stmt), true, false);
      break;

    case CIL_LDSFLD:
      emit_ldsfld (file, stmt);
      break;

    case CIL_LDSFLDA:
      emit_ldsflda (file, stmt);
      break;

    case CIL_LOCALLOC:
      fprintf (file, "\n\tlocalloc");
      break;

    case CIL_MUL:
      fprintf (file, "\n\tmul");
      break;

    case CIL_NEG:
      fprintf (file, "\n\tneg");
      break;

    case CIL_NOT:
      fprintf (file, "\n\tnot");
      break;

    case CIL_OR:
      fprintf (file, "\n\tor");
      break;

    case CIL_POP:
      fprintf (file, "\n\tpop");
      break;

    case CIL_REM:
      fprintf (file, "\n\trem");
      break;

    case CIL_REM_UN:
      fprintf (file, "\n\trem.un");
      break;

    case CIL_RET:
      fprintf (file, "\n\tret");
      break;

    case CIL_SHL:
      fprintf (file, "\n\tshl");
      break;

    case CIL_SHR:
      fprintf (file, "\n\tshr");
      break;

    case CIL_SHR_UN:
      fprintf (file, "\n\tshr.un");
      break;

    case CIL_STARG:
      fprintf (file, "\n\tstarg\t");
      dump_decl_name (file, cil_var (stmt));
      break;

    case CIL_STFLD:
      emit_stfld (file, stmt);
      break;

    case CIL_STIND_I1:
      fprintf (file, "\n\tstind.i1");
      break;

    case CIL_STIND_I2:
      fprintf (file, "\n\tstind.i2");
      break;

    case CIL_STIND_I4:
      fprintf (file, "\n\tstind.i4");
      break;

    case CIL_STIND_I8:
      fprintf (file, "\n\tstind.i8");
      break;

    case CIL_STIND_R4:
      fprintf (file, "\n\tstind.r4");
      break;

    case CIL_STIND_R8:
      fprintf (file, "\n\tstind.r8");
      break;

    case CIL_STIND_I:
      fprintf (file, "\n\tstind.i");
      break;

    case CIL_STLOC:
      fprintf (file, "\n\tstloc\t");
      dump_decl_name (file, cil_var (stmt));
      break;

    case CIL_STOBJ:
      fprintf (file, "\n\tstobj\t");
      dump_type (file, cil_type (stmt), true, false);
      break;

    case CIL_STSFLD:
      emit_stsfld (file, stmt);
      break;

    case CIL_SUB:
      fprintf (file, "\n\tsub");
      break;

    case CIL_SWITCH:
      emit_switch (file, stmt);
      break;

    case CIL_XOR:
      fprintf (file, "\n\txor");
      break;

    case CIL_ASM:
      fprintf (file,
	       "\n\t// BEGIN ASM"
	       "\n\t%s"
	       "\n\t// END ASM",
	       TREE_STRING_POINTER (cil_string (stmt)));
      break;

    default:
      gcc_unreachable ();
    }

}


/* Emits the start function used to call C's main() function.  */

static void
emit_start_function (FILE *file)
{
  size_t nargs = 0;
  tree args = DECL_ARGUMENTS (current_function_decl);

  while (args)
    {
      args = TREE_CHAIN (args);
      nargs++;
    }

  fprintf (file,
	   "\n.method public static void '.start'(class [mscorlib]System.String[] 'args') cil managed"
	   "\n{"
	   "\n\t.entrypoint"
	   "\n\t.maxstack 3"
	   "\n\t.locals (int32 'argc', int8** 'argv', int8** 'env')");

  /* TODO?: add startup code*/

  switch (nargs)
    {
    case 0:
      fprintf (file,
	       "\n\tcall\tvoid [gcc4net]gcc4net.StartupHelper::Startup()"
	       "\n\tcall\tint32 main()");
      break;

    case 1:
      fprintf (file,
	       "\n\tldloca\t'argc'"
	       "\n\tcall\tnative int [gcc4net]gcc4net.StartupHelper::GetArgv(int32&)"
	       "\n\tpop"
	       "\n\tcall\tvoid [gcc4net]gcc4net.StartupHelper::Startup()"
	       "\n\tldloc\t'argc'"
	       "\n\tcall\tint32 main(int32)");
      break;

    case 2:
      fprintf (file,
	       "\n\tldloca\t'argc'"
	       "\n\tcall\tnative int [gcc4net]gcc4net.StartupHelper::GetArgv(int32&)"
	       "\n\tstloc\t'argv'"
	       "\n\tcall\tvoid [gcc4net]gcc4net.StartupHelper::Startup()"
	       "\n\tldloc\t'argc'"
	       "\n\tldloc\t'argv'"
	       "\n\tcall\tint32 main(int32, int8**)");
      break;

    case 3:
      fprintf (file,
	       "\n\tldloca\t'argc'"
	       "\n\tcall\tnative int [gcc4net]gcc4net.StartupHelper::GetArgv(int32&)"
	       "\n\tstloc\t'argv'"
	       "\n\tcall\tnative int [gcc4net]gcc4net.StartupHelper::GetEnvVars()"
	       "\n\tstloc\t'env'"
	       "\n\tcall\tvoid [gcc4net]gcc4net.StartupHelper::Startup()"
	       "\n\tldloc\t'argc'"
	       "\n\tldloc\t'argv'"
	       "\n\tldloc\t'env'"
	       "\n\tcall\tint32 main(int32, int8**, int8**)");
      break;

    default:
      gcc_unreachable ();
    }

  /* TODO?: add exit code*/

  fprintf (file,
	   "\n\tcall\tvoid [gcc4net]gcc4net.StartupHelper::Shutdown(int32)"
	   "\n\tret"
	   "\n} // .start"
	   "\n\n");
}

/* Rename a single variable using the specified suffix */

static void
rename_var (tree var, const char *suffix, unsigned long index)
{
  const char *orig_name = IDENTIFIER_POINTER (DECL_NAME (var));
  size_t size = strlen (orig_name) + strlen (suffix) + 10 + 1;
  char *newsym = XNEWVEC (char, size);

  size = snprintf (newsym, size, "%s%s%lu", orig_name, suffix, index);
  DECL_NAME (var) = get_identifier_with_length (newsym, size);
  XDELETEVEC (newsym);
}

/* Emit the static variables of a function, rename them as appropriate.  */

static void
emit_static_vars (FILE *file)
{
  tree cell = cfun->unexpanded_var_list;

  while (cell != NULL_TREE)
    {
      tree var = TREE_VALUE (cell);

      if (TREE_STATIC (var) && TREE_ASM_WRITTEN (var) == 0)
	{
	  /* TODO: Verify if unexpanded_var_list contains variables with
	     DECL_CONTEXT () set to NULL_TREE.  */
	  if (DECL_CONTEXT (var))
	    rename_var (var, "?fs", DECL_UID (var));

	  emit_cil_decl (file, var);
	}

      cell = TREE_CHAIN (cell);
    }
}

/* Helper function used by as a sorting function for qsort () by
   emit_local_vars ().  */

static int
var_uses_compare (const void *t1, const void *t2)
{
  const var_uses_s *vu1 = t1;
  const var_uses_s *vu2 = t2;

  if (vu1->uses == vu2->uses)
    return DECL_UID (vu1->var) - DECL_UID (vu2->var);
  else
    return vu1->uses - vu2->uses;
}

/* Emit the local variables of a function, rename them as appropriate.  The
   most used local variables are emitted first, this is done in order to
   reduce the size of the CIL code as the LDLOC/STLOC instruction have a
   shorter form for the first 4 locals.  */

static void
emit_local_vars (FILE *file)
{
  VEC (var_uses_s, heap) *locals;
  var_uses_s entry;
  struct pointer_map_t *locals_map = pointer_map_create ();
  unsigned int i, locals_n = 0;
  cil_stmt_iterator csi;
  cil_stmt stmt;
  basic_block bb;
  void **slot;
  tree var;

  /* Count and collect all the local variables used in the function, store them
     in a pointer-map, the corresponding slots are left empty.  */

  FOR_EACH_BB (bb)
    {
      for (csi = csi_start_bb (bb); !csi_end_p (csi); csi_next (&csi))
	{
	  stmt = csi_stmt (csi);

	  switch (cil_opcode (stmt))
	    {
	    case CIL_LDLOC:
	    case CIL_LDLOCA:
	    case CIL_STLOC:
	      var = cil_var (stmt);

	      if (!pointer_map_contains (locals_map, var))
		{
		  *pointer_map_insert (locals_map, var) = NULL;
		  locals_n++;
		}

	      break;

	    default:
	      ;
	    }
	}
    }

  /* Push into the 'locals' array all the local variables and store the number
     of times each one is used. LDLOCA instruction do not increase the use
     count as there is no short version of LDLOCA.  */

  locals = VEC_alloc (var_uses_s, heap, locals_n);

  FOR_EACH_BB (bb)
    {
      for (csi = csi_start_bb (bb); !csi_end_p (csi); csi_next (&csi))
	{
	  stmt = csi_stmt (csi);

	  switch (cil_opcode (stmt))
	    {
	    case CIL_LDLOC:
	    case CIL_LDLOCA:
	    case CIL_STLOC:
	      var = cil_var (stmt);
	      slot = pointer_map_contains (locals_map, var);

	      if (*slot == NULL)
		{
		  entry.var = var;
		  entry.uses = 1;
		  *slot = VEC_quick_push (var_uses_s, locals, &entry);
		}
	      else if (cil_opcode (stmt) != CIL_LDLOCA)
		((var_uses_s *) *slot)->uses++;

	      break;

	    default:
	      ;
	    }
	}
    }

  /* Sort the local variables by uses.  DECL_UIDs are used to break ties so
     that the sorting of the array remains the same between different runs.  */

  pointer_map_destroy (locals_map);
  qsort (VEC_address (var_uses_s, locals), locals_n, sizeof (var_uses_s),
	 var_uses_compare);

  /* Emit the local variables starting from the most used ones.  */

  fprintf (file, "\n\t.locals (");

  for (i = 0; i < locals_n; i++)
    {
      var = (VEC_last (var_uses_s, locals))->var;
      VEC_pop (var_uses_s, locals);

      if (DECL_NAME (var) != NULL_TREE)
        rename_var (var, DECL_FROM_INLINE (var) ? "?in" : "?", i);

      if (i != 0)
        fprintf (file, ", ");

      dump_type (file, promote_local_var_type (var), true, false);
      fprintf (file, " ");
      dump_decl_name (file, var);
    }

  fprintf (file, ")\n");
}

/* Emit the prototype of the current function, it's attributes, etc...  */

static void
emit_function_header (FILE *file)
{
  tree args;
  bool varargs = false;
  bool missing = false;

  if (TARGET_OPENSYSTEMC && MAIN_NAME_P (DECL_NAME (current_function_decl)))
    emit_start_function (file);

  emit_static_vars (file);

  {
    tree args_type = TYPE_ARG_TYPES (TREE_TYPE (current_function_decl));

    if (args_type != NULL)
      {
	while (TREE_CHAIN (args_type))
	  args_type = TREE_CHAIN (args_type);

	if (TREE_VALUE (args_type) != void_type_node)
	  varargs = true;
      }
    else
      missing = true;
  }

  fprintf (file, "\n.method %s static %s",
	   TREE_PUBLIC (current_function_decl) ? "public" : "private",
	   varargs ? "vararg " : "");
  dump_type (file, TREE_TYPE (TREE_TYPE (current_function_decl)), true, false);
  fprintf (file, " '%s' (",
           lang_hooks.decl_printable_name (current_function_decl, 1));

  args = DECL_ARGUMENTS (current_function_decl);

  if (cfun->static_chain_decl)
    {
      dump_type (file, DECL_ARG_TYPE (cfun->static_chain_decl), true, true);
      fprintf (file, " ");
      dump_decl_name (file, cfun->static_chain_decl);
      fprintf (file, "%s", args ? ", " : "");
    }

  while (args)
    {
      if (missing)
	{
	  if (cil_pointer_type_p (DECL_ARG_TYPE (args)))
	    fprintf (file, "native int");
	  else
	    {
	      dump_type (file, promote_type_for_vararg (DECL_ARG_TYPE (args)),
			 true, true);
	    }
        }
      else
	dump_type (file, DECL_ARG_TYPE (args), true, true);

      fprintf (file, " ");
      dump_decl_name (file, args);
      args = TREE_CHAIN (args);

      if (args)
	fprintf (file, ", ");
      else if (varargs)
	fprintf (file, ", ...");
    }

  fprintf (file,
	   ") cil managed"
	   "\n{");

  emit_local_vars (file);

  if (DECL_STATIC_CONSTRUCTOR (current_function_decl))
    {
      /* For the time being this attribute is a String. */
      emit_string_custom_attr (file, "initfun");

      if (TARGET_GCC4NET_LINKER)
	{
	  fputs ("\t.custom instance "
		 "void [gcc4net]gcc4net.C_Attributes.Initializer::.ctor() "
		 "= (01 00 00 00)\n", file);
	}
      else if (TARGET_OPENSYSTEMC)
	{
	  fprintf (file,
		   "\t.custom instance "
		   "void ['OpenSystem.C']'OpenSystem.C'.InitializerAttribute::.ctor() "
		   "= (01 00 00 00)\n");
	}
    }

  {
    struct fnct_attr attributes;

    decode_function_attrs (current_function_decl, &attributes);

    if (attributes.cusattr_string)
      emit_string_custom_attr (file, attributes.cusattr_string);
  }
}

static void
emit_cil_bb (FILE *file, basic_block bb)
{
  cil_stmt_iterator csi;
  block_stmt_iterator bsi = bsi_start (bb);
  tree label;
  cil_stmt stmt = NULL;

  /* Dump this block label */
  label = LABEL_EXPR_LABEL (bsi_stmt (bsi));
  fprintf (file, "\n");
  dump_label_name (file, label);
  fprintf (file, ":");

  for (csi = csi_start_bb (bb); !csi_end_p (csi); csi_next (&csi))
    {
      stmt = csi_stmt (csi);
      emit_cil_stmt (file, stmt);
    }
}

static void
emit_cil_1 (FILE *file)
{
  basic_block bb;

  /* Make sure that every bb has a label */
  FOR_EACH_BB (bb)
    {
      tree_block_label (bb);
    }

  emit_function_header (file);

  FOR_EACH_BB (bb)
    {
      emit_cil_bb (file, bb);
    }

  if (TARGET_EMIT_JIT_COMPILATION_HINTS)
    {
      basic_block_frequency_emit (file);
      branch_probability_emit_and_reset (file);
    }

  fprintf (file,
	   "\n\t.maxstack %u\n"
	   "\n} // %s\n",
	   compute_max_stack (),
	   lang_hooks.decl_printable_name (current_function_decl, 1));
  TREE_ASM_WRITTEN (current_function_decl) = 1;
}

/* Gate function of the CIL assembly emission pass.  */

static bool
emit_cil_gate (void)
{
  return current_function_decl != NULL;
}

/* Entry point of the CIL assembly emission pass.  */

static unsigned int
emit_cil (void)
{
  emit_cil_1 (asm_out_file);

  return 0;
}

/* Define the parameters of the CIL assembly emission pass.  */

struct tree_opt_pass pass_emit_cil =
{
  "emitcil",                            /* name */
  emit_cil_gate,                        /* gate */
  emit_cil,                             /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_EMIT_CIL,                          /* tv_id */
  PROP_cfg,                             /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,
  0,                                    /* todo_flags_finish */
  0                                     /* letter */
};

/* This function is mostly a copy of the last part of 'gen_cil'. */
static void
emit_cil_vcg_1 (FILE *vcg_file)
{
  basic_block bb;
  edge_iterator ei;
  const char *fun_name = lang_hooks.decl_printable_name (current_function_decl, 1);
  edge e;
  int  i=0;

  fprintf (vcg_file, "graph: {\n");
  fprintf (vcg_file, "title: \"%s\"\n", fun_name);
  fprintf (vcg_file, "node: { title: \"%sBB%d\" label: \"ENTRY %s\" }\n",
	   fun_name, ENTRY_BLOCK, fun_name);
  fprintf (vcg_file, "node: { title: \"%sBB%d\" label: \"EXIT\" }\n",
	   fun_name, EXIT_BLOCK);
  fprintf (vcg_file, "edge:{sourcename: \"%sBB%d\" targetname: \"%sBB%d\"}\n",
	   fun_name, ENTRY_BLOCK,
	   fun_name, single_succ (ENTRY_BLOCK_PTR)->index);

  FOR_EACH_BB (bb)
    {
      fprintf (vcg_file, "node: { title: \"%sBB%d\" label: \"(BB%d, pos: %d)",
	       fun_name, bb->index, bb->index, i++);

      if (bb->loop_depth)
	fprintf (vcg_file, " LOOP DEPTH %d ", bb->loop_depth);

      emit_cil_bb (vcg_file, bb);

      fprintf (vcg_file, "\" }\n");  /* close 'label' clause */

      for (ei = ei_start (bb->succs); ei_cond (ei, &e); ei_next (&ei))
	{
	  if (e->flags & EDGE_DFS_BACK)
	    fprintf (vcg_file, "backedge: { color: red");
	  else if (e->flags & EDGE_LOOP_EXIT)
	    fprintf (vcg_file, "edge: { color: blue");
	  else
	    fprintf (vcg_file, "edge: {");

	  fprintf (vcg_file, " label:\"%d", e->probability);

	  if (e->flags & EDGE_LOOP_EXIT)
	    fprintf (vcg_file, " loop_exit");

	  fprintf (vcg_file, "\"");

	  fprintf (vcg_file,
		   " sourcename: \"%sBB%d\" targetname: \"%sBB%d\" }\n",
		   fun_name, bb->index, fun_name, e->dest->index);
	}
    }
  fprintf (vcg_file, "}\n");
}

void
emit_vcg_init (void)
{
  if (TARGET_EMIT_VCG)
    fputs ("graph: {\n"
           "display_edge_labels: yes\n", stdout);
}

void
emit_vcg_fini (void)
{
  if (TARGET_EMIT_VCG)
    fputs ("}\n", stdout);
}

static bool
emit_cil_vcg_gate (void)
{
  return TARGET_EMIT_VCG && current_function_decl != NULL;
}

static unsigned int
emit_cil_vcg (void)
{
  emit_cil_vcg_1(stdout);

  return 0;
}

/* Define the parameters of the CIL_VCG pass.  */

struct tree_opt_pass pass_emit_cil_vcg =
{
  "emit_cil_vcg",                       /* name */
  emit_cil_vcg_gate,                    /* gate */
  emit_cil_vcg,                         /* execute */
  NULL,                                 /* sub */
  NULL,                                 /* next */
  0,                                    /* static_pass_number */
  TV_EMIT_CIL_VCG,                      /* tv_id */
  PROP_cfg,                             /* properties_required */
  0,                                    /* properties_provided */
  0,                                    /* properties_destroyed */
  0,
  0,                                    /* todo_flags_finish */
  0                                     /* letter */
};


/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
