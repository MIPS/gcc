/* Definitions for GCC.  Part of the machine description for cil32.

   Copyright (C) 2006 Free Software Foundation, Inc.

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
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "insn-attr.h"
#include "flags.h"
#include "tree.h"
#include "expr.h"
#include "except.h"
#include "function.h"
#include "toplev.h"
#include "recog.h"
#include "reload.h"
#include "tm_p.h"
#include "debug.h"
#include "output.h"
#include "target.h"
#include "target-def.h"
#include "ggc.h"
#include "optabs.h"
#include "langhooks.h"
#include "gen-cil.h"

/* Per-function machine data.  */
struct machine_function GTY(())
{
  char dummy;
};

static tree cil32_handle_function_attribute (tree *, tree, tree, int, bool *);
static void cil32_file_start (void);
static void cil32_file_end (void);
static void cil32_named_section (const char *, unsigned int, tree);
static void cil32_globalize_label (FILE *, const char *);
static bool cil32_assemble_integer (rtx, unsigned int, int);

static void cil32_init_builtins (void);
static tree cil32_build_builtin_va_list (void);
static tree cil32_gimplify_va_arg (tree, tree, tree*, tree*);

static bool cil32_vector_mode_supported_p (enum machine_mode);

const struct attribute_spec cil32_attribute_table[];

/* Initialize the GCC target structure.  */
#undef TARGET_ATTRIBUTE_TABLE
#define TARGET_ATTRIBUTE_TABLE cil32_attribute_table

#undef TARGET_INIT_BUILTINS
#define TARGET_INIT_BUILTINS cil32_init_builtins


#undef TARGET_ASM_FILE_START
#define TARGET_ASM_FILE_START cil32_file_start
#undef TARGET_ASM_FILE_END
#define TARGET_ASM_FILE_END cil32_file_end
#undef	TARGET_ASM_NAMED_SECTION
#define TARGET_ASM_NAMED_SECTION cil32_named_section
#undef TARGET_ASM_GLOBALIZE_LABEL
#define TARGET_ASM_GLOBALIZE_LABEL cil32_globalize_label
#undef  TARGET_ASM_INTEGER
#define TARGET_ASM_INTEGER cil32_assemble_integer

#undef TARGET_BUILD_BUILTIN_VA_LIST
#define TARGET_BUILD_BUILTIN_VA_LIST cil32_build_builtin_va_list
#undef TARGET_GIMPLIFY_VA_ARG_EXPR
#define TARGET_GIMPLIFY_VA_ARG_EXPR cil32_gimplify_va_arg

#undef TARGET_VECTOR_MODE_SUPPORTED_P
#define TARGET_VECTOR_MODE_SUPPORTED_P cil32_vector_mode_supported_p


struct gcc_target targetm = TARGET_INITIALIZER;

/* CIL32 Code */

static tree
cil32_handle_function_attribute (tree *node, tree name,
				 tree args ATTRIBUTE_UNUSED,
				 int flags ATTRIBUTE_UNUSED,
				 bool *no_add_attrs)
{
  if (TREE_CODE (*node) != FUNCTION_DECL)
    {
      warning (OPT_Wattributes, "%qs attribute only applies to functions",
	       IDENTIFIER_POINTER (name));
      *no_add_attrs = true;
      return NULL_TREE;
    }

  if (strcmp (IDENTIFIER_POINTER (name), "pinvoke") == 0)
    cil_add_pinvoke(*node);

  return NULL_TREE;
}

/* Table of valid machine attributes.  */
const struct attribute_spec cil32_attribute_table[] =
{
  /* { name, min_len, max_len, decl_req, type_req, fn_type_req, handler } */
  { "assembly_name", 1,1,false,false,false, cil32_handle_function_attribute },
  { "cil_name",      1,1,false,false,false, cil32_handle_function_attribute },
  { "cil_strattr",   1,1,false,false,false, cil32_handle_function_attribute },
  { "pinvoke",       1,2,false,false,false, cil32_handle_function_attribute },
  { NULL,            0,0,false,false,false, NULL }
};

static void
cil32_file_start (void)
{
  gen_cil_init ();
}

static void
cil32_file_end (void)
{
  gen_cil_fini ();
}

static void
cil32_named_section (const char *name ATTRIBUTE_UNUSED,
                     unsigned int flags ATTRIBUTE_UNUSED,
                     tree decl ATTRIBUTE_UNUSED)
{
}

static void
cil32_globalize_label (FILE *stream ATTRIBUTE_UNUSED,
                       const char *name ATTRIBUTE_UNUSED)
{
}

static bool
cil32_assemble_integer (rtx x ATTRIBUTE_UNUSED,
                        unsigned int size ATTRIBUTE_UNUSED,
                        int aligned_p ATTRIBUTE_UNUSED)
{
  return true;
}

tree cil32_builtin_va_start_decl;
tree cil32_builtin_va_arg_decl;
tree cil32_builtin_va_end_decl;
tree cil32_builtin_va_copy_decl;
tree cil32_builtin_is_LE_decl;

tree cil32_va_list_type = NULL_TREE;
tree cil32_arg_iterator_type;

/* constructors of vector types. */
/* float */
tree cil32_v2sf_ctor;
tree cil32_v4sf_ctor;
/* 32-bit integer */
tree cil32_v4qi_ctor;
tree cil32_v2hi_ctor;
/* 64-bit integer */
tree cil32_v8qi_ctor;
tree cil32_v4hi_ctor;
tree cil32_v2si_ctor;
/* 128-bit integer */
tree cil32_v4si_ctor;
tree cil32_v8hi_ctor;
tree cil32_v16qi_ctor;


static void
cil32_build_builtin_types (void)
{
  if (cil32_va_list_type == NULL_TREE) {
    tree f_dummy, record, type_decl;

    record = (*lang_hooks.types.make_type) (RECORD_TYPE);
    type_decl = build_decl (TYPE_DECL, get_identifier ("__arg_iterator"), record);

    f_dummy = build_decl (FIELD_DECL, get_identifier ("__dummy"), ptr_type_node);

    DECL_FIELD_CONTEXT (f_dummy) = record;

    TREE_CHAIN (record) = type_decl;
    TYPE_NAME (record)  = type_decl;
    TYPE_FIELDS (record) = f_dummy;

    layout_type (record);

    cil32_arg_iterator_type = record;

    cil32_va_list_type = build_pointer_type (cil32_arg_iterator_type);
  }
}

static void
cil32_init_builtins (void)
{
  tree arglist;

  cil32_build_builtin_types ();

  arglist = build_tree_list (NULL_TREE, cil32_va_list_type);
  cil32_builtin_va_start_decl = add_builtin_function ("__builtin_cil_va_start",
                                                      build_function_type (void_type_node,
                                                                           arglist),
                                                      CIL32_BUILT_IN_VA_START,
                                                      BUILT_IN_MD,
                                                      NULL,
                                                      NULL_TREE);
  arglist = build_tree_list (NULL_TREE, ptr_type_node);
  arglist = tree_cons (NULL_TREE, cil32_va_list_type, arglist);
  cil32_builtin_va_arg_decl = add_builtin_function ("__builtin_cil_va_arg",
                                                    build_function_type (ptr_type_node,
                                                                         arglist),
                                                    CIL32_BUILT_IN_VA_ARG,
                                                    BUILT_IN_MD,
                                                    NULL,
                                                    NULL_TREE);
  arglist = build_tree_list (NULL_TREE, cil32_va_list_type);
  cil32_builtin_va_end_decl = add_builtin_function ("__builtin_cil_va_end",
                                                    build_function_type (void_type_node,
                                                                         arglist),
                                                    CIL32_BUILT_IN_VA_END,
                                                    BUILT_IN_MD,
                                                    NULL,
                                                    NULL_TREE);
  arglist = build_tree_list (NULL_TREE, cil32_va_list_type);
  arglist = tree_cons (NULL_TREE, cil32_va_list_type, arglist);
  cil32_builtin_va_copy_decl = add_builtin_function ("__builtin_cil_va_copy",
                                                     build_function_type (void_type_node,
                                                                          arglist),
                                                     CIL32_BUILT_IN_VA_COPY,
                                                     BUILT_IN_MD,
                                                     NULL,
                                                     NULL_TREE);
  cil32_builtin_is_LE_decl = add_builtin_function ("__builtin_isLittleEndian",
                                                   build_function_type (integer_type_node,
                                                                        NULL_TREE),
                                                   CIL32_BUILT_IN_IS_LITTLE_ENDIAN,
                                                   BUILT_IN_MD,
                                                   NULL,
                                                   NULL_TREE);

  arglist = build_tree_list (NULL_TREE, float_type_node);
  arglist = tree_cons (NULL_TREE, float_type_node, arglist);
  cil32_v2sf_ctor = add_builtin_function ("V2SF_ctor1",
                                          build_function_type (build_vector_type (float_type_node, 2),
                                                               arglist),
                                          CIL32_V2SF_CTOR,
                                          BUILT_IN_MD,
                                          NULL,
                                          NULL_TREE);

  arglist = build_tree_list (NULL_TREE, float_type_node);
  arglist = tree_cons (NULL_TREE, float_type_node, arglist);
  arglist = tree_cons (NULL_TREE, float_type_node, arglist);
  arglist = tree_cons (NULL_TREE, float_type_node, arglist);
  cil32_v4sf_ctor = add_builtin_function ("V4SF_ctor1",
                                          build_function_type (build_vector_type (float_type_node, 4),
                                                               arglist),
                                          CIL32_V4SF_CTOR,
                                          BUILT_IN_MD,
                                          NULL,
                                          NULL_TREE);

  arglist = build_tree_list (NULL_TREE, unsigned_intQI_type_node);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  cil32_v4qi_ctor = add_builtin_function ("V4QI_ctor1",
                                          build_function_type (build_vector_type (intQI_type_node, 4),
                                                               arglist),
                                          CIL32_V4QI_CTOR,
                                          BUILT_IN_MD,
                                          NULL,
                                          NULL_TREE);

  arglist = build_tree_list (NULL_TREE, unsigned_intHI_type_node);
  arglist = tree_cons (NULL_TREE, intHI_type_node, arglist);
  cil32_v2hi_ctor = add_builtin_function ("V2HI_ctor1",
                                          build_function_type (build_vector_type (intHI_type_node, 2),
                                                               arglist),
                                          CIL32_V2HI_CTOR,
                                          BUILT_IN_MD,
                                          NULL,
                                          NULL_TREE);

  arglist = build_tree_list (NULL_TREE, unsigned_intQI_type_node);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  cil32_v8qi_ctor = add_builtin_function ("V8QI_ctor1",
                                          build_function_type (build_vector_type (intQI_type_node, 8),
                                                               arglist),
                                          CIL32_V8QI_CTOR,
                                          BUILT_IN_MD,
                                          NULL,
                                          NULL_TREE);

  arglist = build_tree_list (NULL_TREE, unsigned_intHI_type_node);
  arglist = tree_cons (NULL_TREE, unsigned_intHI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intHI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intHI_type_node, arglist);
  cil32_v4hi_ctor = add_builtin_function ("V4HI_ctor1",
                                          build_function_type (build_vector_type (intHI_type_node, 4),
                                                               arglist),
                                          CIL32_V4HI_CTOR,
                                          BUILT_IN_MD,
                                          NULL,
                                          NULL_TREE);

  arglist = build_tree_list (NULL_TREE, unsigned_intSI_type_node);
  arglist = tree_cons (NULL_TREE, unsigned_intSI_type_node, arglist);
  cil32_v2si_ctor = add_builtin_function ("V2SI_ctor1",
                                          build_function_type (build_vector_type (intSI_type_node, 2),
                                                               arglist),
                                          CIL32_V2SI_CTOR,
                                          BUILT_IN_MD,
                                          NULL,
                                          NULL_TREE);

  arglist = build_tree_list (NULL_TREE, unsigned_intSI_type_node);
  arglist = tree_cons (NULL_TREE, unsigned_intSI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intSI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intSI_type_node, arglist);
  cil32_v4si_ctor = add_builtin_function ("V4SI_ctor1",
                                          build_function_type (build_vector_type (intSI_type_node, 4),
                                                               arglist),
                                          CIL32_V4SI_CTOR,
                                          BUILT_IN_MD,
                                          NULL,
                                          NULL_TREE);

  arglist = build_tree_list (NULL_TREE, unsigned_intHI_type_node);
  arglist = tree_cons (NULL_TREE, unsigned_intHI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intHI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intHI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intHI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intHI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intHI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intHI_type_node, arglist);
  cil32_v8hi_ctor = add_builtin_function ("V8HI_ctor1",
                                          build_function_type (build_vector_type (intHI_type_node, 8),
                                                               arglist),
                                          CIL32_V8HI_CTOR,
                                          BUILT_IN_MD,
                                          NULL,
                                          NULL_TREE);

  arglist = build_tree_list (NULL_TREE, unsigned_intQI_type_node);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  arglist = tree_cons (NULL_TREE, unsigned_intQI_type_node, arglist);
  cil32_v16qi_ctor = add_builtin_function ("V16QI_ctor1",
                                           build_function_type (build_vector_type (intQI_type_node, 16),
                                                                arglist),
                                           CIL32_V16QI_CTOR,
                                           BUILT_IN_MD,
                                           NULL,
                                           NULL_TREE);
}

static tree
cil32_build_builtin_va_list (void)
{
  cil32_build_builtin_types ();
  return cil32_va_list_type;
}

static tree
cil32_gimplify_va_arg (tree valist, tree type, tree *pre_p ATTRIBUTE_UNUSED, tree *post_p ATTRIBUTE_UNUSED)
{
  tree fcall;
  tree ptr_type = build_pointer_type (type);
  tree arglist = build_tree_list (NULL_TREE, build_int_cstu (ptr_type, 0));
  arglist = tree_cons (NULL_TREE, valist, arglist);
  fcall = build_function_call_expr (cil32_builtin_va_arg_decl, arglist);
  TREE_TYPE (fcall) = ptr_type;
  return build1 (INDIRECT_REF, type, fcall);
}

/* Target hook for vector_mode_supported_p.  */
static bool
cil32_vector_mode_supported_p (enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return true;
}

#include "gt-cil32.h"
