/* Dump of CIL code into assembly.

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
#include "cil-stack.h"
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

static const char* const cil_names[] = {
#define CIL_INSTRDEF(A,B,C,D) C,
#include "cil-instr.def"
#undef CIL_INSTRDEF
};

static const char* const cil_type_names[] = {
#define CIL_TYPEDEF(A,B) B,
#include "cil-types.def"
#undef CIL_TYPEDEF
};

/******************************************************************************
 * Local function prototypes                                                  *
 ******************************************************************************/

static unsigned int compute_max_stack (struct function *);
static struct fnct_attr decode_function_attrs (tree);
static void emit_enum_decl (FILE *, tree);
static void emit_array_decl (FILE *, tree);
static void emit_incomplete_decl (FILE *, const_tree);
static void emit_struct_union_decl (FILE *, tree);
static void emit_valuetype_decl (FILE *, tree);
static void dump_type (FILE *, tree, bool, bool);
static void dump_valuetype_name (FILE *, const_tree);
static void dump_fun_type (FILE *, tree, tree, const char *, bool);
static void dump_decl_name (FILE *, tree);
static void dump_method_name(FILE *, tree);
static void dump_string_name (FILE *, tree);
static void dump_string_type (FILE *, tree);
static void dump_label_name (FILE *, tree);
static void dump_vector_type (FILE *, cil_type_t);
static void dump_string_decl (FILE *, tree);
static bool dump_type_promoted_type_def (FILE *, tree);

static void emit_init_method (FILE *);
static void emit_referenced_strings (void);
static void emit_referenced_types (void);
static void emit_pinvoke_function (FILE *, tree);
static void emit_referenced_pinvokes (void);
static void emit_prefixes (FILE *, const_cil_stmt);
static void emit_sfld (FILE *, tree);
static void emit_ldsflda (FILE *, tree);
static void emit_fld (FILE *, tree);
static void emit_switch_labels (FILE *, const_cil_stmt);
static void emit_string_custom_attr (FILE *, const char *);
static bool emit_builtin_call (FILE *, const_cil_stmt);
static void emit_call_arg (FILE *, const_cil_stmt);
static void emit_cil_stmt (FILE *, const_cil_stmt);
static void emit_referenced_assemblies (FILE *);
static void emit_start_function (FILE *, struct function *);
static void rename_var (tree, const char *, unsigned long);
static void emit_static_vars (FILE *, struct function *);
static int var_uses_compare (const void *, const void *);
static void emit_local_vars (FILE *, struct function *);
static void emit_function_attributes (FILE *, bool, struct fnct_attr);
static void emit_function_header (FILE *, struct function *);
static void emit_cil_bb (FILE *, basic_block);
static void emit_cil_1 (FILE *, struct function *);
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

  if (TARGET_GCC4NET_LINKER)
    {
      add_referenced_assembly ("gcc4net");
      add_referenced_assembly ("mscorlib");
      add_referenced_assembly ("ExternalAssembly");
    }

  if (simd_type == MONO_SIMD)
    add_referenced_assembly ("Mono.Simd");

  emit_referenced_assemblies (file);

  if (TARGET_GCC4NET_LINKER)
    {
      fprintf (file, ".assembly '%s' {\n", aux_base_name);
      fputs ("\t.custom instance "
	     "void [gcc4net]gcc4net.C_Attributes.CObjectFile::.ctor() "
	     "= (01 00 00 00)\n"
	     "}\n", file);
      fprintf (file, ".module '%s'\n", aux_base_name);
    }
  else if (TARGET_OPENSYSTEMC)
    {
      fputs (".module '<Module>'\n"
	     ".custom instance "
	     "void ['OpenSystem.C']'OpenSystem.C'.ModuleAttribute::.ctor() "
	     "= (01 00 00 00)\n", file);
    }
}

void
emit_cil_fini (void)
{
  emit_init_method (asm_out_file);
  emit_referenced_strings ();
  emit_referenced_types ();
  emit_referenced_pinvokes ();
}

void
emit_cil_decl (FILE *file, tree decl)
{
  if (flag_syntax_only)
    return;

  refs_init ();

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
compute_max_stack (struct function *fun)
{
  unsigned int *depths;
  unsigned int max_depth = 0;
  unsigned int depth;
  basic_block bb;
  edge e;
  edge_iterator ei;

  depths = XCNEWVEC (unsigned int, last_basic_block);

  FOR_EACH_BB_FN (bb, fun)
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

static struct fnct_attr
decode_function_attrs (tree t)
{
  tree tmp;
  struct fnct_attr attrs;

  gcc_assert (TREE_CODE (t) == FUNCTION_DECL);

  attrs.assembly_name    = 0;
  attrs.cil_name         = 0;
  attrs.cusattr_string   = 0;
  attrs.pinvoke_assembly = 0;
  attrs.pinvoke_fname    = 0;

  tmp = DECL_ATTRIBUTES (t);

  while (tmp)
    {
      const char *attr_name = IDENTIFIER_POINTER (TREE_PURPOSE (tmp));
      tree params = TREE_VALUE (tmp);

      if (strcmp (attr_name, "assembly_name") == 0)
	attrs.assembly_name = TREE_STRING_POINTER (TREE_VALUE (params));
      else if (strcmp (attr_name, "cil_name") == 0)
	attrs.cil_name = TREE_STRING_POINTER (TREE_VALUE (params));
      else if (strcmp (attr_name, "cil_strattr") == 0)
	attrs.cusattr_string = TREE_STRING_POINTER (TREE_VALUE (params));
      else if (strcmp (attr_name, "pinvoke") == 0)
	{
	  attrs.pinvoke_assembly = TREE_STRING_POINTER (TREE_VALUE (params));

	  if (TREE_CHAIN (params))
	    attrs.pinvoke_fname = TREE_STRING_POINTER (TREE_VALUE (TREE_CHAIN (params)));
	}

      tmp = TREE_CHAIN (tmp);
    }
  return attrs;
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

/* Dump the name of a method node the file FILE.  */

static void
dump_method_name(FILE *file, tree method)
{
  if (DECL_BUILT_IN (method) && DECL_BUILT_IN_CLASS (method) == BUILT_IN_MD)
    fprintf (file, "%s", IDENTIFIER_POINTER (DECL_NAME (method)));
  else
    {
      struct fnct_attr attrs = decode_function_attrs (method);

      if (attrs.assembly_name)
	fprintf (file, "[%s]", attrs.assembly_name);
      else if (TARGET_GCC4NET_LINKER && DECL_EXTERNAL (method) && TREE_PUBLIC (method))
	{
	  fputs ("[ExternalAssembly]", file);
	  if (!attrs.cil_name)
	    fputs ("ExternalAssembly::", file);
	}

      if (attrs.cil_name)
	fprintf (file, "%s", attrs.cil_name);
      else
	dump_decl_name (file, method);
    }
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
  fprintf (file, "?L" HOST_WIDE_INT_PRINT_DEC,
	   (HOST_WIDE_INT) LABEL_DECL_UID (node));

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
dump_valuetype_name (FILE *file, const_tree t)
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
    fprintf (file, "[gcc4net]'gcc4net.");
  else
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

  gcc_assert (!(fun && name));

  if (ref)
    mark_referenced_type (fun_type);

  args_type = TYPE_ARG_TYPES (fun_type);

  if (args_type == NULL)
    {
      warning (OPT_Wcil_missing_prototypes,
	       "Missing function %s prototype, guessing it, you should fix the "
	       "code", fun ? IDENTIFIER_POINTER (DECL_NAME (fun)) : "");
    }

  last_arg_type = tree_last (args_type);

  if (last_arg_type && TREE_VALUE (last_arg_type) != void_type_node)
    {
      last_arg_type = NULL;
      varargs = true;
    }

  fprintf (file, "%s", varargs ? "vararg " : "");
  dump_type (file, TREE_TYPE (fun_type), ref, false);
  fprintf (file, " ");

  if (fun)
    dump_method_name(file, fun);

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

   TYPE must be a type node of type VECTOR_TYPE.   */
static void
dump_vector_type (FILE *file, cil_type_t cil_type)
{
  if (simd_type == MONO_SIMD)
    {
      /* Mono.Simd types */
      const char* suffix;
      switch (cil_type)
	{
	case CIL_V2HI: suffix = "2s";  break;
	case CIL_V4QI: suffix = "4sb"; break;
	case CIL_V2SI: suffix = "2i";  break;
	case CIL_V4HI: suffix = "4s";  break;
	case CIL_V8QI: suffix = "8sb"; break;
	case CIL_V2SF: suffix = "2f";  break;
	case CIL_V2DI: suffix = "2l";  break;
	case CIL_V4SI: suffix = "4i";  break;
	case CIL_V8HI: suffix = "8s";  break;
	case CIL_V16QI:suffix = "16sb";break;
	case CIL_V2DF: suffix = "2d";  break;
	case CIL_V4SF: suffix = "4f";  break;
	default:
	  gcc_unreachable ();
	  break;
	}
      fprintf (file, "valuetype [Mono.Simd]Mono.Simd.Vector%s", suffix);
    }
  else /* Gcc.Simd types */
    fprintf (file, "valuetype [gcc4net]gcc4net.%s", cil_type_names [cil_type]);
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
emit_incomplete_decl (FILE *file, const_tree t)
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
  cil_type_t cil_type;

  if (type == NULL_TREE || type == error_mark_node)
    return;

  cil_type = type_to_cil (type);

  if (cil_vector_p (cil_type))
    dump_vector_type (file, cil_type);
  else
    switch (cil_type)
      {
      case CIL_VALUE_TYPE:
	/* Reference the type if told to do so */
	if (ref)
	  mark_referenced_type (TYPE_MAIN_VARIANT (type));

	/* Print the name of the structure.  */
	fprintf (file, "valuetype ");
	dump_valuetype_name (file, TYPE_MAIN_VARIANT (type));
	break;

      case CIL_POINTER:
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

      default:
	fprintf (file, cil_type_names [cil_type]);
	break;
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
	     && !ARRAY_TYPE_VARLENGTH (node));

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
	default:
	  if (ISPRINT (str[i]))
	    fputc (str[i], file);
	  else
	    fprintf (file, "\\x%x", str[i]);
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

  if (node == NULL_TREE || node == error_mark_node)
    return false;

  if (cil_value_type_p (node) || cil_vector_type_p (node))
    dump_type (file, node, true, false);
  else
    {
      cil_type_t cil_type = type_to_cil (node);

      if (cil_int_or_smaller_p(cil_type))
        fprintf (file, "class [mscorlib]System.UInt32");
      else if (cil_long_p(cil_type))
        fprintf (file, "class [mscorlib]System.UInt64");
      else if (cil_float_p(cil_type))
        fprintf (file, "class [mscorlib]System.Double");
      else if (cil_pointer_p(cil_type))
        {
          /* cil32 is a 32bit machine, in case we support 64bit model
           * changes are needed
           */
          fprintf (file, "class [mscorlib]System.UInt32");
          result = true;
        }
      else
        gcc_unreachable ();
    }

  return result;
}

/* Emit a per-compilation unit initialization method used for initalizing
   global and static variables.  The function is emitted in the file
   specified by FILE.  */

static void
emit_init_method (FILE *file)
{
  VEC (tree, gc) *pending_ctors = pending_ctors_vec ();
  cil_stmt_iterator csi;
  cil_stmt stmt;
  cil_seq seq;
  size_t i;

  if (VEC_length (tree, pending_ctors) != 0)
    {
      struct fnct_attr attributes = { NULL, NULL, NULL, NULL, NULL };
      seq = cil_seq_alloc ();
      csi = csi_start (seq);

      /* Emit the initializer function header.  We assume that the function
	 doesn't use any local variable.  */
      fprintf (file,
	       "\n.method private static void 'COBJ?init' () cil managed"
	       "\n{"
	       "\n\t.locals ()\n");


      emit_function_attributes (file, true, attributes);

      fprintf(file,
	      "\n"
	      "\n?L0:");

      for (i = 0; i < VEC_length (tree, pending_ctors); i++)
	{
	  tree decl = VEC_index (tree, pending_ctors, i);
	  tree init = DECL_INITIAL (decl);

	  DECL_INITIAL (decl) = NULL_TREE;
	  csi_insert_seq_after (&csi, expand_init_to_cil_seq (decl, init),
				CSI_CONTINUE_LINKING);
	}

      stmt = cil_build_stmt (CIL_RET);
      csi_insert_after (&csi, stmt, CSI_CONTINUE_LINKING);

      /* Output the actual code.  */
      for (csi = csi_start (seq); !csi_end_p (csi); csi_next (&csi))
	{
	  stmt = csi_stmt (csi);
	  emit_cil_stmt (file, stmt);
	}

      /* Output the maximum stack size.  */
      fprintf (file,
	       "\n\t.maxstack %u"
	       "\n} // COBJ?init",
	       cil_seq_stack_depth (seq, 0, true));
    }
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
emit_incomplete_type (const void *elem, void *data)
{
  const_tree type = (const_tree) elem;
  tree type_name = TYPE_NAME (type);
  struct pointer_set_t *emitted_types = (struct pointer_set_t *) data;

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
  struct fnct_attr attributes = decode_function_attrs (fun);

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

  if (cil_prefix_tail (stmt))
    {
      gcc_assert ((opcode == CIL_CALL) || (opcode == CIL_CALLI));
      fprintf (file, "\n\ttail.");
    }

  if (cil_prefix_unaligned (stmt) != 0)
    {
      gcc_assert ((opcode == CIL_CPBLK) || (opcode == CIL_INITBLK)
		  || ((opcode >= CIL_LDIND_I1) && (opcode <= CIL_LDIND_I))
		  || ((opcode >= CIL_STIND_I1) && (opcode <= CIL_STIND_I))
		  || (opcode == CIL_LDFLD) || (opcode == CIL_STFLD));
      fprintf (file, "\n\tunaligned. %d", cil_prefix_unaligned (stmt));
    }

  if (cil_prefix_volatile (stmt))
    {
      gcc_assert ((opcode == CIL_CPBLK) || (opcode == CIL_INITBLK)
		  || ((opcode >= CIL_LDIND_I1) && (opcode <= CIL_LDIND_I))
		  || ((opcode >= CIL_STIND_I1) && (opcode <= CIL_STIND_I))
		  || (opcode == CIL_LDFLD) || (opcode == CIL_STFLD)
		  || (opcode == CIL_LDSFLD) || (opcode == CIL_STSFLD)
		  || (opcode == CIL_LDOBJ) || (opcode == CIL_STOBJ));
      fprintf (file, "\n\tvolatile.");
    }
}

/* Emit a CIL LDSFLD/STSFLD instruction argument.  */

static void
emit_sfld (FILE *file, tree arg)
{
  dump_type (file, TREE_TYPE (arg), true, false);
  fprintf (file, " ");

  if (TARGET_GCC4NET_LINKER && DECL_EXTERNAL (arg) && TREE_PUBLIC (arg))
    fprintf (file, "[ExternalAssembly]ExternalAssembly::");

  dump_decl_name (file, arg);
}

/* Emit a CIL LDSFLDA instruction argument. This instruction is used for
   obtaining the addresses of global variables but also string constants. */

static void
emit_ldsflda (FILE *file, tree field)
{
  gcc_assert (TREE_CODE (field) == STRING_CST || TREE_CODE (field) == VAR_DECL);

  if (TREE_CODE (field) == STRING_CST)
    {
      dump_string_type (file, TREE_TYPE (field));
      fprintf (file, " ");
      dump_string_name (file, field);
    }
  else if (COMPLETE_TYPE_P (TREE_TYPE (field)))
    {
      emit_sfld (file, field);
    }
  else
    {
      fprintf (file, "native int ");
      /* Global Variable with incomplete type, consider it an External definition */
      if (TARGET_GCC4NET_LINKER)
	fprintf (file, "[ExternalAssembly]ExternalAssembly::");
      dump_decl_name (file, field);
    }
}

/* Emit a CIL LDFLD/LDFLDA/STFLD instruction argument.  */

static void
emit_fld (FILE *file, tree field)
{
  tree obj_type = DECL_FIELD_CONTEXT (field);

  mark_referenced_type (obj_type);
  dump_type (file, TREE_TYPE (field), true, false);
  fprintf (file, " ");
  mark_referenced_type (obj_type);
  dump_valuetype_name (file, obj_type);
  fprintf (file, "::");
  dump_decl_name (file, field);
}

/* Emit a SWITCH instruction.  */

static void
emit_switch_labels (FILE *file, const_cil_stmt stmt)
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

  fprintf (file, "(");

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
	  if (TARGET_GCC4NET_LINKER)
	    fprintf (file, "\n\tcall\tvoid [ExternalAssembly]ExternalAssembly::abort ()");
	  else
	    fprintf (file, "\n\tcall\tvoid abort ()");
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
emit_call_arg (FILE *file, const_cil_stmt call)
{
  enum cil_opcode opcode = cil_opcode (call);
  size_t nargs_base;
  size_t nargs;

  gcc_assert (opcode == CIL_CALL
	      || opcode == CIL_CALLI
	      || opcode == CIL_JMP
	      || opcode == CIL_NEWOBJ);

  nargs_base = cil_call_nargs_base (call);
  nargs = cil_call_nargs (call);

  /* Dump the return type.  */
  if (opcode == CIL_NEWOBJ)
    fprintf (file, "instance void class");
  else
    {
      tree ftype = cil_call_ftype (call);
      if (cil_call_vararg_p (call))
	fprintf (file, "vararg ");
      dump_type (file, TREE_TYPE (ftype), true, false);
    }

  if (opcode == CIL_CALLI)
      --nargs;
  else
    {
      /* If Direct Call Dump the called method name.  */
      fprintf (file, " ");
      dump_method_name(file, cil_call_fdecl (call));
    }

  /* Dump the call arguments.  */
  fprintf (file, " (");
  {
    size_t i;

    if (cil_call_static_chain (call))
      {
	dump_type (file, cil_call_static_chain (call), true, true);
	--nargs;

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
  }
  fprintf (file, ")");
}

/* Emits a single CIL statement STMT the file specified by FILE.  */

static void
emit_cil_stmt (FILE *file, const_cil_stmt stmt)
{
  enum cil_opcode opcode = cil_opcode (stmt);

  gcc_assert (opcode != CIL_LDVEC
	      && opcode != CIL_STVEC
	      && opcode != CIL_VEC_CTOR);

  if (opcode == CIL_CALL
      && DECL_BUILT_IN (cil_call_fdecl (stmt))
      && emit_builtin_call (file, stmt))
    return;

  emit_prefixes (file, stmt);

  fprintf (file, "\n\t");
  fprintf (file, "%s", cil_names[opcode]);

  switch (opcode_arg_type (opcode))
    {
    case CIL_NONE:
      break;

    case CIL_VAR:
      fprintf (file, "\t");
      dump_decl_name (file, cil_var (stmt));
      break;

    case CIL_TYPE:
      fprintf (file, "\t");
      dump_type (file, cil_type (stmt), true, false);
      break;

    case CIL_FIELD:
      fprintf (file, "\t");
      switch (opcode)
	{
	case CIL_LDFLD:
	case CIL_LDFLDA:
	case CIL_STFLD:
	  emit_fld (file, cil_field (stmt));
	  break;

	case CIL_LDSFLD:
	case CIL_STSFLD:
	  emit_sfld (file, cil_field (stmt));
	  break;

	case CIL_LDSFLDA:
	  emit_ldsflda (file, cil_field (stmt));
	  break;

	default:
	  gcc_unreachable ();
	}
      break;

    case CIL_LABEL:
      fprintf (file, "\t");
      dump_label_name (file, cil_label (stmt));
      break;

    case CIL_LABELS:
      fprintf (file, "\t");
      emit_switch_labels (file, stmt);
      break;

    case CIL_FUNC:
      fprintf (file, "\t");
      dump_fun_type (file, TREE_TYPE (cil_func (stmt)),
		     cil_func (stmt), NULL, true);
      break;

    case CIL_FCALL:
      fprintf (file, "\t");
      emit_call_arg (file, stmt);
      break;

    case CIL_CST:
      fprintf (file, "\t");
      switch (opcode)
	{
	case CIL_LDC_I4:
	case CIL_LDC_I8:
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
	    fprintf (file, "float32(%#08lx)\t/* %s */",
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
	    fprintf (file, "float64(%#08lx%08lx)\t/* %s */",
		     buf[1], buf[0], string);
	  }
	  break;

	default:
	  gcc_unreachable ();
	}
      break;
    case CIL_STRING:
      {
	const char *str = TREE_STRING_POINTER (cil_string (stmt));
	const char *str2 = str;

	fprintf (file, "\n\t// BEGIN ASM");
	for (;str2[0] && ISSPACE(str2[0]); ++str2)
	  ;
	fprintf (file, "\n\t%s%s", (str2[0] == '#') ? "//" : "", str);
	fprintf (file,"\n\t// END ASM");
      }
      break;

    default:
      gcc_unreachable ();
    }
}


/* Emits the start function used to call C's main() function.  */

static void
emit_start_function (FILE *file, struct function *fun)
{
  size_t nargs = list_length (DECL_ARGUMENTS (fun->decl));

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
emit_static_vars (FILE *file, struct function *fun)
{
  tree cell = fun->local_decls;

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
  const var_uses_s *vu1 = (const var_uses_s *) t1;
  const var_uses_s *vu2 = (const var_uses_s *) t2;

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
emit_local_vars (FILE *file, struct function *fun)
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

  FOR_EACH_BB_FN (bb, fun)
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

  FOR_EACH_BB_FN (bb, fun)
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

  /* FIXME: We shouldn't always set init */
  if (fun->machine->locals_init || 1)
    fprintf (file, "\n\t.locals init (");
  else
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

/* Emit the external assembly references which might have been added during
   the previous phases.  */

static void
emit_referenced_assemblies (FILE *file)
{
  htab_iterator hti;
  tree name;

  FOR_EACH_HTAB_ELEMENT (pending_assemblies_htab (), name, tree, hti)
    {
      fprintf(file, ".assembly extern %s {}\n", TREE_STRING_POINTER (name));
    }

  mark_pending_assemblies ();
}

/* Emit the attributes of a function  */

static void
emit_function_attributes (FILE *file, bool is_init, struct fnct_attr attributes)
{
  if (is_init)
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

  if (attributes.cusattr_string)
    emit_string_custom_attr (file, attributes.cusattr_string);
}

/* Emit the prototype of the current function */

static void
emit_function_header (FILE *file, struct function *fun)
{
  tree args;
  bool varargs = false;
  bool missing = false;

  {
    tree args_type = TYPE_ARG_TYPES (TREE_TYPE (fun->decl));

    if (args_type != NULL)
      {
        tree last_args_type = tree_last (args_type);

	if (TREE_VALUE (last_args_type) != void_type_node)
	  varargs = true;
      }
    else
      missing = true;
  }

  fprintf (file, "\n.method %s static %s",
	   TREE_PUBLIC (fun->decl) ? "public" : "private",
	   varargs ? "vararg " : "");
  dump_type (file, TREE_TYPE (TREE_TYPE (fun->decl)), true, false);
  fprintf (file, " '%s' (",
	   lang_hooks.decl_printable_name (fun->decl, 1));

  args = DECL_ARGUMENTS (fun->decl);

  if (fun->static_chain_decl)
    {
      dump_type (file, DECL_ARG_TYPE (fun->static_chain_decl), true, true);
      fprintf (file, " ");
      dump_decl_name (file, fun->static_chain_decl);
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
}

static void
emit_cil_bb (FILE *file, basic_block bb)
{
  cil_stmt_iterator csi;
  gimple_stmt_iterator gsi = gsi_start_bb (bb);
  tree label;
  cil_stmt stmt = NULL;

  /* Dump this block label */
  label = gimple_label_label (gsi_stmt (gsi));
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
emit_cil_1 (FILE *file, struct function *fun)
{
  basic_block bb;

  emit_referenced_assemblies (file);

  if (TARGET_OPENSYSTEMC && MAIN_NAME_P (DECL_NAME (fun->decl)))
    emit_start_function (file, fun);

  emit_static_vars (file, fun);

  emit_function_header (file, fun);

  emit_local_vars (file, fun);

  emit_function_attributes (file, DECL_STATIC_CONSTRUCTOR (fun->decl),
                            decode_function_attrs (fun->decl));

  /* Make sure that every bb has a label */
  FOR_EACH_BB_FN (bb, fun)
    {
      gimple_block_label (bb);
    }

  FOR_EACH_BB_FN (bb, fun)
    {
      emit_cil_bb (file, bb);
    }

  if (TARGET_EMIT_JIT_COMPILATION_HINTS)
    {
      basic_block_frequency_emit (file, fun);
      branch_probability_emit_and_reset (file);
    }

  fprintf (file,
	   "\n\t.maxstack %u\n"
	   "} // %s\n",
	   compute_max_stack (fun),
	   lang_hooks.decl_printable_name (fun->decl, 1));
  TREE_ASM_WRITTEN (fun->decl) = 1;
}

/* Gate function of the CIL assembly emission pass.  */

static bool
emit_cil_gate (void)
{
  return (current_function_decl != NULL) && !flag_syntax_only;
}

/* Entry point of the CIL assembly emission pass.  */

static unsigned int
emit_cil (void)
{
  emit_cil_1 (asm_out_file, cfun);

  return 0;
}

/* Define the parameters of the CIL assembly emission pass.  */

struct gimple_opt_pass pass_emit_cil =
{
 {
  GIMPLE_PASS,                          /* type */
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
  0,                                    /* todo_flags_start */
  0                                    /* todo_flags_finish */
 }
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
  return TARGET_EMIT_VCG
	 && (current_function_decl != NULL)
	 && !flag_syntax_only;
}

static unsigned int
emit_cil_vcg (void)
{
  emit_cil_vcg_1 (stdout);

  return 0;
}

/* Define the parameters of the CIL_VCG pass.  */

struct gimple_opt_pass pass_emit_cil_vcg =
{
 {
  GIMPLE_PASS,                          /* type */
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
  0,                                    /* todo_flags_start */
  0                                     /* todo_flags_finish */
 }
};


/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * End:
 */
