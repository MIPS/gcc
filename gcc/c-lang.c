/* Language-specific hook definitions for C front end.
   Copyright (C) 1991, 1995, 1997, 1998,
   1999, 2000, 2001 Free Software Foundation, Inc.

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */


#include "config.h"
#include "system.h"
#include "tree.h"
#include "function.h"
#include "input.h"
#include "toplev.h"
#include "diagnostic.h"
#include "output.h"
#include "flags.h"
#include "ggc.h"
#include "rtl.h"
#include "expr.h"
#include "c-tree.h"
#include "c-lex.h"
#include "cpplib.h"

static int c_tree_printer PARAMS ((output_buffer *));
static size_t lang_type_size_fetcher PARAMS ((const void *v,
                                             type_definition_p td));
static int lang_type_more_fields PARAMS ((struct field_definition_s *fp,
                                         const void *v,
                                         unsigned n, code_type c,
                                         type_definition_p td));

static int c_missing_noreturn_ok_p PARAMS ((tree));
static void c_init PARAMS ((void));
static void c_init_options PARAMS ((void));
static void c_post_options PARAMS ((void));
static void c_tree_prewrite_hook PARAMS ((const void *));

/* Each front end provides its own.  */
struct lang_hooks lang_hooks = {c_init,
				NULL, /* c_finish */
				c_init_options,
				c_decode_option,
				c_post_options};

/* Post-switch processing.  */
static void
c_post_options ()
{
  cpp_post_options (parse_in);
}

static void
c_init_options ()
{
  /* Make identifier nodes long enough for the language-specific slots.  */
  set_identifier_size (sizeof (struct lang_identifier));

  parse_in = cpp_create_reader (ident_hash, CLK_GNUC89);

  /* Mark as "unspecified".  */
  flag_bounds_check = -1;
}

static const struct field_definition_s c_tree_field_defs[] = {
  { 'd', 0xFF, offsetof (union tree_node, decl.lang_specific), 
    lang_decl_type_def },
  { 't', 0xFF, offsetof (union tree_node, type.lang_specific), 
    lang_type_type_def },
  { IDENTIFIER_NODE << 8, 0xFF00,
    offsetof (struct lang_identifier, global_value), tree_type_def },
  { IDENTIFIER_NODE << 8, 0xFF00,
    offsetof (struct lang_identifier, local_value), tree_type_def },
  { IDENTIFIER_NODE << 8, 0xFF00,
    offsetof (struct lang_identifier, label_value), tree_type_def },
  { IDENTIFIER_NODE << 8, 0xFF00,
    offsetof (struct lang_identifier, implicit_decl), tree_type_def },
  { IDENTIFIER_NODE << 8, 0xFF00,
    offsetof (struct lang_identifier, error_locus), tree_type_def },
  { IDENTIFIER_NODE << 8, 0xFF00,
    offsetof (struct lang_identifier, limbo_value), tree_type_def },
  NO_MORE_FIELDS
};

static const struct field_definition_s c_lang_decl_field_defs[] = {
  { 0, 0, offsetof (struct lang_decl, base.saved_tree), tree_type_def },
  NO_MORE_FIELDS
};

static size_t 
lang_type_size_fetcher (v, td)
     const void *v;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  struct lang_type *t = (struct lang_type *)v;
  return sizeof (*t) - sizeof (tree) + t->len * sizeof (tree);
}

static int 
lang_type_more_fields (fp, v, n, c, td)
     struct field_definition_s *fp;
     const void *v;
     unsigned n;
     code_type c ATTRIBUTE_UNUSED;
     type_definition_p td ATTRIBUTE_UNUSED;
{
  struct lang_type *t = (struct lang_type *)v;
  if (n < (unsigned)t->len)
    {
      fp->offset = offsetof (struct lang_type, elts) + n * sizeof (tree);
      fp->type = tree_type_def;
      return 1;
    }
  return 0;
}

/* A function to be called for each tree node before they are written out
   by the precompiled headers mechanism.  Currently we use this to note
   that the fields are not sorted, as PCH fails to preserve address order
   for identifiers.  */

static void
c_tree_prewrite_hook (t_v)
     const void *t_v;
{
  tree t = (tree)t_v;
  if (TYPE_P (t))
    TYPE_LANG_SPECIFIC (t) = 0;
}


static const struct field_definition_s c_lang_function_field_defs[] = {
  
  NO_MORE_FIELDS
};


static void
c_init ()
{
  c_common_lang_init ();

  /* If still unspecified, make it match -std=c99
     (allowing for -pedantic-errors).  */
  if (mesg_implicit_function_declaration < 0)
    {
      if (flag_isoc99)
	mesg_implicit_function_declaration = flag_pedantic_errors ? 2 : 1;
      else
	mesg_implicit_function_declaration = 0;
    }

  save_lang_status = &push_c_function_context;
  restore_lang_status = &pop_c_function_context;
  mark_lang_status = &mark_c_function_context;
  lang_expand_expr = &c_expand_expr;
  lang_safe_from_p = &c_safe_from_p;
  diagnostic_format_decoder (global_dc) = &c_tree_printer;
  lang_expand_decl_stmt = &c_expand_decl_stmt;
  lang_missing_noreturn_ok_p = &c_missing_noreturn_ok_p;

  add_tree_fields (c_tree_field_defs);
  tree_type_def->prewrite_hook = c_tree_prewrite_hook;
  lang_decl_type_def->size = sizeof (struct lang_decl);
  lang_decl_type_def->field_definitions = c_lang_decl_field_defs;
  lang_type_type_def->size = sizeof (struct lang_type) - sizeof (tree);
  lang_type_type_def->size_fetcher = lang_type_size_fetcher;
  lang_type_type_def->more_fields = lang_type_more_fields;

  c_parse_init ();
  pch_init ();
}

const char *
lang_identify ()
{
  return "c";
}

int
lang_toplevel_p ()
{
  return global_bindings_p ();
}

void
print_lang_statistics ()
{
}

/* used by print-tree.c */

void
lang_print_xnode (file, node, indent)
     FILE *file ATTRIBUTE_UNUSED;
     tree node ATTRIBUTE_UNUSED;
     int indent ATTRIBUTE_UNUSED;
{
}

/* Used by c-lex.c, but only for objc.  */

tree
lookup_interface (arg)
     tree arg ATTRIBUTE_UNUSED;
{
  return 0;
}

tree
is_class_name (arg)
    tree arg ATTRIBUTE_UNUSED;
{
  return 0;
}

void
maybe_objc_check_decl (decl)
     tree decl ATTRIBUTE_UNUSED;
{
}

int
maybe_objc_comptypes (lhs, rhs, reflexive)
     tree lhs ATTRIBUTE_UNUSED;
     tree rhs ATTRIBUTE_UNUSED;
     int reflexive ATTRIBUTE_UNUSED;
{
  return -1;
}

tree
maybe_building_objc_message_expr ()
{
  return 0;
}

int
recognize_objc_keyword ()
{
  return 0;
}

/* Used by c-typeck.c (build_external_ref), but only for objc.  */

tree
lookup_objc_ivar (id)
     tree id ATTRIBUTE_UNUSED;
{
  return 0;
}

#if !defined(ASM_OUTPUT_CONSTRUCTOR) || !defined(ASM_OUTPUT_DESTRUCTOR)
extern tree static_ctors;
extern tree static_dtors;

static tree start_cdtor		PARAMS ((int));
static void finish_cdtor	PARAMS ((tree));

static tree
start_cdtor (method_type)
     int method_type;
{
  tree fnname = get_file_function_name (method_type);
  tree void_list_node_1 = build_tree_list (NULL_TREE, void_type_node);
  tree body;

  start_function (void_list_node_1,
		  build_nt (CALL_EXPR, fnname,
			    tree_cons (NULL_TREE, NULL_TREE, void_list_node_1),
			    NULL_TREE),
		  NULL_TREE);
  store_parm_decls ();

  current_function_cannot_inline
    = "static constructors and destructors cannot be inlined";

  body = c_begin_compound_stmt ();

  pushlevel (0);
  clear_last_expr ();
  add_scope_stmt (/*begin_p=*/1, /*partial_p=*/0);

  return body;
}

static void
finish_cdtor (body)
     tree body;
{
  tree scope;
  tree block;

  scope = add_scope_stmt (/*begin_p=*/0, /*partial_p=*/0);
  block = poplevel (0, 0, 0);
  SCOPE_STMT_BLOCK (TREE_PURPOSE (scope)) = block;
  SCOPE_STMT_BLOCK (TREE_VALUE (scope)) = block;

  RECHAIN_STMTS (body, COMPOUND_BODY (body));

  finish_function (0);
}
#endif

/* Called at end of parsing, but before end-of-file processing.  */

void
finish_file ()
{
  if (pch_file)
    lang_write_pch ();
#ifndef ASM_OUTPUT_CONSTRUCTOR
  if (static_ctors)
    {
      tree body = start_cdtor ('I');

      for (; static_ctors; static_ctors = TREE_CHAIN (static_ctors))
	c_expand_expr_stmt (build_function_call (TREE_VALUE (static_ctors),
						 NULL_TREE));

      finish_cdtor (body);
    }
#endif
#ifndef ASM_OUTPUT_DESTRUCTOR
  if (static_dtors)
    {
      tree body = start_cdtor ('D');

      for (; static_dtors; static_dtors = TREE_CHAIN (static_dtors))
	c_expand_expr_stmt (build_function_call (TREE_VALUE (static_dtors),
						 NULL_TREE));

      finish_cdtor (body);
    }
#endif

  if (back_end_hook)
    (*back_end_hook) (getdecls ());
  
  {
    int flags;
    FILE *stream = dump_begin (TDI_all, &flags);

    if (stream)
      {
	dump_node (getdecls (), flags & ~TDF_SLIM, stream);
	dump_end (TDI_all, stream);
      }
  }
}

/* Called during diagnostic message formatting process to print a
   source-level entity onto BUFFER.  The meaning of the format specifiers
   is as follows:
   %D: a general decl,
   %F: a function declaration,
   %T: a type.

   These format specifiers form a subset of the format specifiers set used
   by the C++ front-end.
   Please notice when called, the `%' part was already skipped by the
   diagnostic machinery.  */
static int
c_tree_printer (buffer)
     output_buffer *buffer;
{
  tree t = va_arg (output_buffer_format_args (buffer), tree);

  switch (*output_buffer_text_cursor (buffer))
    {
    case 'D':
    case 'F':
    case 'T':
      {
        const char *n = DECL_NAME (t)
          ? (*decl_printable_name) (t, 2)
          : "({anonymous})";
        output_add_string (buffer, n);
      }
      return 1;

    default:
      return 0;
    }
}

static int
c_missing_noreturn_ok_p (decl)
     tree decl;
{
  /* A missing noreturn is not ok for freestanding implementations and
     ok for the `main' function in hosted implementations.  */
  return flag_hosted && MAIN_NAME_P (DECL_ASSEMBLER_NAME (decl));
}
void
lang_write_pch ()
{
  c_write_pch ();
}


