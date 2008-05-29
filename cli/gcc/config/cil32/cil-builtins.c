/*

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
#include "tree.h"
#include "langhooks.h"

#include "cil-builtins.h"

tree cil32_builtins[CIL32_MAX_BUILT_IN] = {NULL_TREE};

tree cil32_va_list_type = NULL_TREE;
tree cil32_arg_iterator_type;

static void
cil_build_builtin_types (void)
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
cil_add_builtin(enum cil32_builtin bi, const char *name, tree ret_type, int n, ...)
{
    tree arglist;
    va_list va;

    va_start (va, n);
    arglist = NULL_TREE;
    while (n-->0)
        arglist = tree_cons (NULL_TREE, va_arg(va, tree), arglist);
    va_end (va);

    arglist = tree_cons (NULL_TREE, void_type_node, arglist);

    arglist = nreverse (arglist);

    cil32_builtins[bi] = add_builtin_function (name,
                                               build_function_type (ret_type,
                                                                    arglist),
                                               bi,
                                               BUILT_IN_MD,
                                               NULL,
                                               NULL_TREE);
}

void
cil_init_builtins (void)
{
  /* Types */
  tree float2_type_node = build_vector_type (float_type_node, 2);
  tree float4_type_node = build_vector_type (float_type_node, 4);
  tree intQI4_type_node = build_vector_type (intQI_type_node, 4);
  tree intHI2_type_node = build_vector_type (intHI_type_node, 2);
  tree intQI8_type_node = build_vector_type (intQI_type_node, 8);
  tree intHI4_type_node = build_vector_type (intHI_type_node, 4);
  tree intSI2_type_node = build_vector_type (intSI_type_node, 2);
  tree intSI4_type_node = build_vector_type (intSI_type_node, 4);
  tree intHI8_type_node = build_vector_type (intHI_type_node, 8);
  tree intQI16_type_node = build_vector_type (intQI_type_node, 16);

  cil_build_builtin_types ();

#define DEF_CILBUILTIN(bid, name, ret_type, num_par, ...) \
  cil_add_builtin (CIL32_##bid, name, ret_type, num_par, ## __VA_ARGS__);
#include "cil-builtins.def"
#undef DEF_CILBUILTIN
}

tree
cil_build_builtin_va_list (void)
{
  cil_build_builtin_types ();
  return cil32_va_list_type;
}
