/* Language-dependent hooks for LTO.
   Copyright 2006 Free Software Foundation, Inc.
   Contributed by CodeSourcery, Inc.

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
#include "flags.h"
#include "tm.h"
#include "tree.h"
#include "expr.h"
#include "target.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "debug.h"
#include "lto-tree.h"
#include "lto.h"
#include "tree-inline.h"
#include "gimple.h"
#include "toplev.h"
#include "libfuncs.h"
#include "except.h"
#include "cgraph.h"
#include "lto/common.h"
#include "lto-tree-in.h"

static tree handle_noreturn_attribute (tree *, tree, tree, int, bool *);
static tree handle_const_attribute (tree *, tree, tree, int, bool *);
static tree handle_malloc_attribute (tree *, tree, tree, int, bool *);
static tree handle_pure_attribute (tree *, tree, tree, int, bool *);
static tree handle_novops_attribute (tree *, tree, tree, int, bool *);
static tree handle_nonnull_attribute (tree *, tree, tree, int, bool *);
static tree handle_nothrow_attribute (tree *, tree, tree, int, bool *);
static tree handle_sentinel_attribute (tree *, tree, tree, int, bool *);
static tree handle_type_generic_attribute (tree *, tree, tree, int, bool *);
static tree handle_format_attribute (tree *, tree, tree, int, bool *);
static tree handle_format_arg_attribute (tree *, tree, tree, int, bool *);

/* Table of machine-independent attributes supported in GIMPLE.
   
   FIXME lto, this started as a copy of c_common_attribute_table, as
   initially LTO only really supports C-like languages.  Only
   attributes that are used in builtins are included (See
   builtin-attrs.def).  Every other attribute is already streamed with
   their corresponding DECLs.  Perhaps one solution is to stream out
   builtin tables as well, though this seems wasteful.

   When other languages are incorporated this needs to be adapted for
   the new languages.  */
const struct attribute_spec lto_attribute_table[] =
{
  /* { name, min_len, max_len, decl_req, type_req, fn_type_req, handler } */
  /* FIXME: logically, noreturn attributes should be listed as
     "false, true, true" and apply to function types.  But implementing this
     would require all the places in the compiler that use TREE_THIS_VOLATILE
     on a decl to identify non-returning functions to be located and fixed
     to check the function type instead.  */
  { "noreturn",               0, 0, true,  false, false,
			      handle_noreturn_attribute },
  /* The same comments as for noreturn attributes apply to const ones.  */
  { "const",                  0, 0, true,  false, false,
			      handle_const_attribute },
  { "malloc",                 0, 0, true,  false, false,
			      handle_malloc_attribute },
  { "pure",                   0, 0, true,  false, false,
			      handle_pure_attribute },
  { "no vops",                0, 0, true,  false, false,
			      handle_novops_attribute },
  { "nonnull",                0, -1, false, true, true,
			      handle_nonnull_attribute },
  { "nothrow",                0, 0, true,  false, false,
			      handle_nothrow_attribute },
  { "sentinel",               0, 1, false, true, true,
			      handle_sentinel_attribute },
  { "type generic",           0, 0, false, true, true,
			      handle_type_generic_attribute },
  { NULL,                     0, 0, false, false, false, NULL }
};

/* Give the specifications for the format attributes, used by C and all
   descendants.  */

const struct attribute_spec lto_format_attribute_table[] =
{
  /* { name, min_len, max_len, decl_req, type_req, fn_type_req, handler } */
  { "format",                 3, 3, false, true,  true,
			      handle_format_attribute },
  { "format_arg",             1, 1, false, true,  true,
			      handle_format_arg_attribute },
  { NULL,                     0, 0, false, false, false, NULL }
};

enum built_in_attribute
{
#define DEF_ATTR_NULL_TREE(ENUM) ENUM,
#define DEF_ATTR_INT(ENUM, VALUE) ENUM,
#define DEF_ATTR_IDENT(ENUM, STRING) ENUM,
#define DEF_ATTR_TREE_LIST(ENUM, PURPOSE, VALUE, CHAIN) ENUM,
#include "builtin-attrs.def"
#undef DEF_ATTR_NULL_TREE
#undef DEF_ATTR_INT
#undef DEF_ATTR_IDENT
#undef DEF_ATTR_TREE_LIST
  ATTR_LAST
};

static GTY(()) tree built_in_attributes[(int) ATTR_LAST];

/* Builtin types.  */

enum lto_builtin_type
{
#define DEF_PRIMITIVE_TYPE(NAME, VALUE) NAME,
#define DEF_FUNCTION_TYPE_0(NAME, RETURN) NAME,
#define DEF_FUNCTION_TYPE_1(NAME, RETURN, ARG1) NAME,
#define DEF_FUNCTION_TYPE_2(NAME, RETURN, ARG1, ARG2) NAME,
#define DEF_FUNCTION_TYPE_3(NAME, RETURN, ARG1, ARG2, ARG3) NAME,
#define DEF_FUNCTION_TYPE_4(NAME, RETURN, ARG1, ARG2, ARG3, ARG4) NAME,
#define DEF_FUNCTION_TYPE_5(NAME, RETURN, ARG1, ARG2, ARG3, ARG4, ARG5) NAME,
#define DEF_FUNCTION_TYPE_6(NAME, RETURN, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) NAME,
#define DEF_FUNCTION_TYPE_7(NAME, RETURN, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) NAME,
#define DEF_FUNCTION_TYPE_VAR_0(NAME, RETURN) NAME,
#define DEF_FUNCTION_TYPE_VAR_1(NAME, RETURN, ARG1) NAME,
#define DEF_FUNCTION_TYPE_VAR_2(NAME, RETURN, ARG1, ARG2) NAME,
#define DEF_FUNCTION_TYPE_VAR_3(NAME, RETURN, ARG1, ARG2, ARG3) NAME,
#define DEF_FUNCTION_TYPE_VAR_4(NAME, RETURN, ARG1, ARG2, ARG3, ARG4) NAME,
#define DEF_FUNCTION_TYPE_VAR_5(NAME, RETURN, ARG1, ARG2, ARG3, ARG4, ARG6) \
  NAME,
#define DEF_POINTER_TYPE(NAME, TYPE) NAME,
#include "builtin-types.def"
#undef DEF_PRIMITIVE_TYPE
#undef DEF_FUNCTION_TYPE_0
#undef DEF_FUNCTION_TYPE_1
#undef DEF_FUNCTION_TYPE_2
#undef DEF_FUNCTION_TYPE_3
#undef DEF_FUNCTION_TYPE_4
#undef DEF_FUNCTION_TYPE_5
#undef DEF_FUNCTION_TYPE_6
#undef DEF_FUNCTION_TYPE_7
#undef DEF_FUNCTION_TYPE_VAR_0
#undef DEF_FUNCTION_TYPE_VAR_1
#undef DEF_FUNCTION_TYPE_VAR_2
#undef DEF_FUNCTION_TYPE_VAR_3
#undef DEF_FUNCTION_TYPE_VAR_4
#undef DEF_FUNCTION_TYPE_VAR_5
#undef DEF_POINTER_TYPE
  BT_LAST
};

typedef enum lto_builtin_type builtin_type;

static GTY(()) tree builtin_types[(int) BT_LAST + 1];

/* FIXME: eventually these should be stuck into an array or merged with
   c_global_trees.  */
static GTY(()) tree string_type_node;
static GTY(()) tree const_string_type_node;
static GTY(()) tree wint_type_node;
static GTY(()) tree intmax_type_node;
static GTY(()) tree uintmax_type_node;
static GTY(()) tree signed_size_type_node;

/* FIXME lto.  Dummy flags that we need to include builtins.def.
   These need to go away somehow.  */
int flag_no_builtin;
int flag_no_nonansi_builtin;
int flag_isoc94;
int flag_isoc99;

/* Attribute handlers.  */

/* Handle a "noreturn" attribute; arguments as in
   struct attribute_spec.handler.  */

static tree
handle_noreturn_attribute (tree *node, tree ARG_UNUSED (name),
			   tree ARG_UNUSED (args), int ARG_UNUSED (flags),
			   bool * ARG_UNUSED (no_add_attrs))
{
  tree type = TREE_TYPE (*node);

  /* See FIXME comment in c_common_attribute_table.  */
  if (TREE_CODE (*node) == FUNCTION_DECL)
    TREE_THIS_VOLATILE (*node) = 1;
  else if (TREE_CODE (type) == POINTER_TYPE
	   && TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
    TREE_TYPE (*node)
      = build_pointer_type
	(build_type_variant (TREE_TYPE (type),
			     TYPE_READONLY (TREE_TYPE (type)), 1));
  else
    gcc_unreachable ();

  return NULL_TREE;
}


/* Handle a "const" attribute; arguments as in
   struct attribute_spec.handler.  */

static tree
handle_const_attribute (tree *node, tree ARG_UNUSED (name),
			tree ARG_UNUSED (args), int ARG_UNUSED (flags),
			bool * ARG_UNUSED (no_add_attrs))
{
  tree type = TREE_TYPE (*node);

  /* See FIXME comment on noreturn in c_common_attribute_table.  */
  if (TREE_CODE (*node) == FUNCTION_DECL)
    TREE_READONLY (*node) = 1;
  else if (TREE_CODE (type) == POINTER_TYPE
	   && TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
    TREE_TYPE (*node)
      = build_pointer_type
	(build_type_variant (TREE_TYPE (type), 1,
			     TREE_THIS_VOLATILE (TREE_TYPE (type))));
  else
    gcc_unreachable ();

  return NULL_TREE;
}


/* Handle a "malloc" attribute; arguments as in
   struct attribute_spec.handler.  */

static tree
handle_malloc_attribute (tree *node, tree ARG_UNUSED (name),
			 tree ARG_UNUSED (args), int ARG_UNUSED (flags),
			 bool * ARG_UNUSED (no_add_attrs))
{
  if (TREE_CODE (*node) == FUNCTION_DECL
      && POINTER_TYPE_P (TREE_TYPE (TREE_TYPE (*node))))
    DECL_IS_MALLOC (*node) = 1;
  else
    gcc_unreachable ();

  return NULL_TREE;
}


/* Handle a "pure" attribute; arguments as in
   struct attribute_spec.handler.  */

static tree
handle_pure_attribute (tree *node, tree ARG_UNUSED (name),
		       tree ARG_UNUSED (args), int ARG_UNUSED (flags),
		       bool * ARG_UNUSED (no_add_attrs))
{
  if (TREE_CODE (*node) == FUNCTION_DECL)
    DECL_PURE_P (*node) = 1;
  else
    gcc_unreachable ();

  return NULL_TREE;
}


/* Handle a "no vops" attribute; arguments as in
   struct attribute_spec.handler.  */

static tree
handle_novops_attribute (tree *node, tree ARG_UNUSED (name),
			 tree ARG_UNUSED (args), int ARG_UNUSED (flags),
			 bool *ARG_UNUSED (no_add_attrs))
{
  gcc_assert (TREE_CODE (*node) == FUNCTION_DECL);
  DECL_IS_NOVOPS (*node) = 1;
  return NULL_TREE;
}


/* Helper for nonnull attribute handling; fetch the operand number
   from the attribute argument list.  */

static bool
get_nonnull_operand (tree arg_num_expr, unsigned HOST_WIDE_INT *valp)
{
  /* Verify the arg number is a constant.  */
  if (TREE_CODE (arg_num_expr) != INTEGER_CST
      || TREE_INT_CST_HIGH (arg_num_expr) != 0)
    return false;

  *valp = TREE_INT_CST_LOW (arg_num_expr);
  return true;
}

/* Handle the "nonnull" attribute.  */

static tree
handle_nonnull_attribute (tree *node, tree ARG_UNUSED (name),
			  tree args, int ARG_UNUSED (flags),
			  bool * ARG_UNUSED (no_add_attrs))
{
  tree type = *node;
  unsigned HOST_WIDE_INT attr_arg_num;

  /* If no arguments are specified, all pointer arguments should be
     non-null.  Verify a full prototype is given so that the arguments
     will have the correct types when we actually check them later.  */
  if (!args)
    {
      gcc_assert (TYPE_ARG_TYPES (type));
      return NULL_TREE;
    }

  /* Argument list specified.  Verify that each argument number references
     a pointer argument.  */
  for (attr_arg_num = 1; args; args = TREE_CHAIN (args))
    {
      tree argument;
      unsigned HOST_WIDE_INT arg_num = 0, ck_num;

      if (!get_nonnull_operand (TREE_VALUE (args), &arg_num))
	gcc_unreachable ();

      argument = TYPE_ARG_TYPES (type);
      if (argument)
	{
	  for (ck_num = 1; ; ck_num++)
	    {
	      if (!argument || ck_num == arg_num)
		break;
	      argument = TREE_CHAIN (argument);
	    }

	  gcc_assert (argument
		      && TREE_CODE (TREE_VALUE (argument)) == POINTER_TYPE);
	}
    }

  return NULL_TREE;
}


/* Handle a "nothrow" attribute; arguments as in
   struct attribute_spec.handler.  */

static tree
handle_nothrow_attribute (tree *node, tree ARG_UNUSED (name),
			  tree ARG_UNUSED (args), int ARG_UNUSED (flags),
			  bool * ARG_UNUSED (no_add_attrs))
{
  if (TREE_CODE (*node) == FUNCTION_DECL)
    TREE_NOTHROW (*node) = 1;
  else
    gcc_unreachable ();

  return NULL_TREE;
}


/* Handle a "sentinel" attribute.  */

static tree
handle_sentinel_attribute (tree *node, tree ARG_UNUSED (name), tree args,
			   int ARG_UNUSED (flags),
			   bool * ARG_UNUSED (no_add_attrs))
{
  tree params = TYPE_ARG_TYPES (*node);
  gcc_assert (params);

  while (TREE_CHAIN (params))
    params = TREE_CHAIN (params);

  gcc_assert (!VOID_TYPE_P (TREE_VALUE (params)));

  if (args)
    {
      tree position = TREE_VALUE (args);
      gcc_assert (TREE_CODE (position) == INTEGER_CST);
      if (tree_int_cst_lt (position, integer_zero_node))
	gcc_unreachable ();
    }

  return NULL_TREE;
}

/* Handle a "type_generic" attribute.  */

static tree
handle_type_generic_attribute (tree *node, tree ARG_UNUSED (name),
			       tree ARG_UNUSED (args), int ARG_UNUSED (flags),
			       bool * ARG_UNUSED (no_add_attrs))
{
  tree params;
  
  /* Ensure we have a function type.  */
  gcc_assert (TREE_CODE (*node) == FUNCTION_TYPE);
  
  params = TYPE_ARG_TYPES (*node);
  while (params && ! VOID_TYPE_P (TREE_VALUE (params)))
    params = TREE_CHAIN (params);

  /* Ensure we have a variadic function.  */
  gcc_assert (!params);

  return NULL_TREE;
}

/* Handle a "format" attribute; arguments as in
   struct attribute_spec.handler.  */

static tree
handle_format_attribute (tree * ARG_UNUSED (node), tree ARG_UNUSED (name),
			 tree ARG_UNUSED (args), int ARG_UNUSED (flags),
			 bool *no_add_attrs)
{
  *no_add_attrs = true;
  return NULL_TREE;
}


/* Handle a "format_arg" attribute; arguments as in
   struct attribute_spec.handler.  */

tree
handle_format_arg_attribute (tree * ARG_UNUSED (node), tree ARG_UNUSED (name),
			     tree ARG_UNUSED (args), int ARG_UNUSED (flags),
			     bool *no_add_attrs)
{
  *no_add_attrs = true;
  return NULL_TREE;
}


/* Cribbed from c-common.c.  */

static void
def_fn_type (builtin_type def, builtin_type ret, bool var, int n, ...)
{
  tree args = NULL, t;
  va_list list;
  int i;

  va_start (list, n);
  for (i = 0; i < n; ++i)
    {
      builtin_type a = va_arg (list, builtin_type);
      t = builtin_types[a];
      if (t == error_mark_node)
	goto egress;
      args = tree_cons (NULL_TREE, t, args);
    }
  va_end (list);

  args = nreverse (args);
  if (!var)
    args = chainon (args, void_list_node);

  t = builtin_types[ret];
  if (t == error_mark_node)
    goto egress;
  t = build_function_type (t, args);

 egress:
  builtin_types[def] = t;
}

/* Used to help initialize the builtin-types.def table.  When a type of
   the correct size doesn't exist, use error_mark_node instead of NULL.
   The later results in segfaults even when a decl using the type doesn't
   get invoked.  */

static tree
builtin_type_for_size (int size, bool unsignedp)
{
  tree type = lang_hooks.types.type_for_size (size, unsignedp);
  return type ? type : error_mark_node;
}

/* Support for DEF_BUILTIN.  */

static void
def_builtin_1 (enum built_in_function fncode, const char *name,
	       enum built_in_class fnclass, tree fntype, tree libtype,
	       bool both_p, bool fallback_p, bool nonansi_p,
	       tree fnattrs, bool implicit_p)
{
  tree decl;
  const char *libname;

  if (fntype == error_mark_node)
    return;

  libname = name + strlen ("__builtin_");
  decl = add_builtin_function (name, fntype, fncode, fnclass,
			       (fallback_p ? libname : NULL),
			       fnattrs);

  if (both_p
      && !flag_no_builtin
      && !(nonansi_p && flag_no_nonansi_builtin))
    add_builtin_function (libname, libtype, fncode, fnclass,
			  NULL, fnattrs);

  built_in_decls[(int) fncode] = decl;
  if (implicit_p)
    implicit_built_in_decls[(int) fncode] = decl;
}


/* Initialize the attribute table for all the supported builtins.  */

static void
lto_init_attributes (void)
{
  /* Fill in the built_in_attributes array.  */
#define DEF_ATTR_NULL_TREE(ENUM)				\
  built_in_attributes[(int) ENUM] = NULL_TREE;
#define DEF_ATTR_INT(ENUM, VALUE)				\
  built_in_attributes[(int) ENUM] = build_int_cst (NULL_TREE, VALUE);
#define DEF_ATTR_IDENT(ENUM, STRING)				\
  built_in_attributes[(int) ENUM] = get_identifier (STRING);
#define DEF_ATTR_TREE_LIST(ENUM, PURPOSE, VALUE, CHAIN)	\
  built_in_attributes[(int) ENUM]			\
    = tree_cons (built_in_attributes[(int) PURPOSE],	\
		 built_in_attributes[(int) VALUE],	\
		 built_in_attributes[(int) CHAIN]);
#include "builtin-attrs.def"
#undef DEF_ATTR_NULL_TREE
#undef DEF_ATTR_INT
#undef DEF_ATTR_IDENT
#undef DEF_ATTR_TREE_LIST
}


static void
lto_define_builtins (tree va_list_ref_type_node ATTRIBUTE_UNUSED,
		     tree va_list_arg_type_node ATTRIBUTE_UNUSED)
{
#define DEF_PRIMITIVE_TYPE(ENUM, VALUE) \
  builtin_types[ENUM] = VALUE;
#define DEF_FUNCTION_TYPE_0(ENUM, RETURN) \
  def_fn_type (ENUM, RETURN, 0, 0);
#define DEF_FUNCTION_TYPE_1(ENUM, RETURN, ARG1) \
  def_fn_type (ENUM, RETURN, 0, 1, ARG1);
#define DEF_FUNCTION_TYPE_2(ENUM, RETURN, ARG1, ARG2) \
  def_fn_type (ENUM, RETURN, 0, 2, ARG1, ARG2);
#define DEF_FUNCTION_TYPE_3(ENUM, RETURN, ARG1, ARG2, ARG3) \
  def_fn_type (ENUM, RETURN, 0, 3, ARG1, ARG2, ARG3);
#define DEF_FUNCTION_TYPE_4(ENUM, RETURN, ARG1, ARG2, ARG3, ARG4) \
  def_fn_type (ENUM, RETURN, 0, 4, ARG1, ARG2, ARG3, ARG4);
#define DEF_FUNCTION_TYPE_5(ENUM, RETURN, ARG1, ARG2, ARG3, ARG4, ARG5)	\
  def_fn_type (ENUM, RETURN, 0, 5, ARG1, ARG2, ARG3, ARG4, ARG5);
#define DEF_FUNCTION_TYPE_6(ENUM, RETURN, ARG1, ARG2, ARG3, ARG4, ARG5, \
			    ARG6)					\
  def_fn_type (ENUM, RETURN, 0, 6, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6);
#define DEF_FUNCTION_TYPE_7(ENUM, RETURN, ARG1, ARG2, ARG3, ARG4, ARG5, \
			    ARG6, ARG7)					\
  def_fn_type (ENUM, RETURN, 0, 7, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7);
#define DEF_FUNCTION_TYPE_VAR_0(ENUM, RETURN) \
  def_fn_type (ENUM, RETURN, 1, 0);
#define DEF_FUNCTION_TYPE_VAR_1(ENUM, RETURN, ARG1) \
  def_fn_type (ENUM, RETURN, 1, 1, ARG1);
#define DEF_FUNCTION_TYPE_VAR_2(ENUM, RETURN, ARG1, ARG2) \
  def_fn_type (ENUM, RETURN, 1, 2, ARG1, ARG2);
#define DEF_FUNCTION_TYPE_VAR_3(ENUM, RETURN, ARG1, ARG2, ARG3) \
  def_fn_type (ENUM, RETURN, 1, 3, ARG1, ARG2, ARG3);
#define DEF_FUNCTION_TYPE_VAR_4(ENUM, RETURN, ARG1, ARG2, ARG3, ARG4) \
  def_fn_type (ENUM, RETURN, 1, 4, ARG1, ARG2, ARG3, ARG4);
#define DEF_FUNCTION_TYPE_VAR_5(ENUM, RETURN, ARG1, ARG2, ARG3, ARG4, ARG5) \
  def_fn_type (ENUM, RETURN, 1, 5, ARG1, ARG2, ARG3, ARG4, ARG5);
#define DEF_POINTER_TYPE(ENUM, TYPE) \
  builtin_types[(int) ENUM] = build_pointer_type (builtin_types[(int) TYPE]);

#include "builtin-types.def"

#undef DEF_PRIMITIVE_TYPE
#undef DEF_FUNCTION_TYPE_1
#undef DEF_FUNCTION_TYPE_2
#undef DEF_FUNCTION_TYPE_3
#undef DEF_FUNCTION_TYPE_4
#undef DEF_FUNCTION_TYPE_5
#undef DEF_FUNCTION_TYPE_6
#undef DEF_FUNCTION_TYPE_VAR_0
#undef DEF_FUNCTION_TYPE_VAR_1
#undef DEF_FUNCTION_TYPE_VAR_2
#undef DEF_FUNCTION_TYPE_VAR_3
#undef DEF_FUNCTION_TYPE_VAR_4
#undef DEF_FUNCTION_TYPE_VAR_5
#undef DEF_POINTER_TYPE
  builtin_types[(int) BT_LAST] = NULL_TREE;

  lto_init_attributes ();

#define DEF_BUILTIN(ENUM, NAME, CLASS, TYPE, LIBTYPE, BOTH_P, FALLBACK_P,	\
		    NONANSI_P, ATTRS, IMPLICIT, COND)				\
    if (NAME && COND)								\
      def_builtin_1 (ENUM, NAME, CLASS, builtin_types[(int) TYPE],		\
		     builtin_types[(int) LIBTYPE], BOTH_P, FALLBACK_P,		\
		     NONANSI_P, built_in_attributes[(int) ATTRS], IMPLICIT);
#include "builtins.def"
#undef DEF_BUILTIN

  targetm.init_builtins ();

  build_common_builtin_nodes ();
}

static GTY(()) tree registered_builtin_types;

/* A chain of builtin functions that we need to recognize.  We will
   assume that all other function names we see will be defined by the
   user's program.  */
static GTY(()) tree registered_builtin_fndecls;

/* Language hooks.  */

#define DEFAULT_LTRANS_DRIVER "ltrans-driver"
#define DEFAULT_LTRANS_DRIVER_LEN (strlen (DEFAULT_LTRANS_DRIVER) + 1)

static unsigned int
lto_init_options (unsigned int argc ATTRIBUTE_UNUSED,
		  const char **argv)
{
  const char *p;
  char *q;

  /* The following code initializes ltrans_driver to its default value.
     We don't necessarily know where we're installed, but lto1 and ltans-driver
     should be in the same directory.  If argv[0] contains the full path to
     lto1, use it.  Otherwise, we assume ltrans-driver is reachable via the
     PATH environment variable.  */

  p = argv[0] + strlen (argv[0]);
  while (p != argv[0] && !IS_DIR_SEPARATOR (p[-1]))
    --p;

  if (p != argv[0])
    {
      size_t pathlen = p - argv[0];

      /* The string at argv[0] is a full path name.  Use it to locate the
         LTRANS driver in the same directory.  */
      q = XNEWVEC (char, pathlen + DEFAULT_LTRANS_DRIVER_LEN);
      ltrans_driver = q;
      strncpy (q, argv[0], p - argv[0]);
      q += pathlen;
    }
  else
    {
      q = XNEWVEC (char, DEFAULT_LTRANS_DRIVER_LEN);
      ltrans_driver = q;
    }
  strncpy (q, DEFAULT_LTRANS_DRIVER, DEFAULT_LTRANS_DRIVER_LEN);

  /* Always operate in unit-at-time mode so that we can defer
     decisions about what to output.  */
  flag_unit_at_a_time = 1;

  /* Enable exceptions by default.  */
  flag_exceptions = 1;

  return CL_LTO;
}

/* Handle command-line option SCODE.  If the option takes an argument, it is
   stored in ARG, which is otherwise NULL.  VALUE holds either a numerical
   argument or a binary value indicating whether the positive or negative form
   of the option was supplied.  */

const char *resolution_file_name;
static int
lto_handle_option (size_t scode, const char *arg, int value ATTRIBUTE_UNUSED)
{
  enum opt_code code = (enum opt_code) scode;
  int result = 1;

  switch (code)
    {
    case OPT_resolution:
      resolution_file_name = arg;
      result = 1;
      break;

    default:
      break;
    }

  return result;
}

/* Perform post-option processing.  Does additional initialization based on
   command-line options.  PFILENAME is the main input filename.  Returns false
   to enable subsequent back-end initialization.  */

static bool
lto_post_options (const char **pfilename ATTRIBUTE_UNUSED)
{
  /* FIXME lto: We have stripped enough type and other
     debugging information out of the IR that it may
     appear ill-formed to dwarf2out, etc.  We must not
     attempt to generate debug info in lto1.  A more
     graceful solution would disable the option flags
     rather than ignoring them, but we'd also have to
     worry about default debugging options.  */
  write_symbols = NO_DEBUG;
  debug_info_level = DINFO_LEVEL_NONE;

  /* -fltrans and -fwpa are mutually exclusive.  Check for that here.  */
  if (flag_wpa && flag_ltrans)
    error ("-fwpa and -fltrans are mutually exclusive.");

  if (flag_ltrans)
    flag_generate_lto = 0;

  if (flag_wpa)
    flag_generate_lto = 1;

  /* Initialize the compiler back end.  */
  return false;
}

static bool 
lto_mark_addressable (tree t)
{
  mark_addressable (t);
  return true;
}

/* Return an integer type with PRECISION bits of precision,
   that is unsigned if UNSIGNEDP is nonzero, otherwise signed.  */

static tree
lto_type_for_size (unsigned precision, int unsignedp)
{
  if (precision == TYPE_PRECISION (integer_type_node))
    return unsignedp ? unsigned_type_node : integer_type_node;

  if (precision == TYPE_PRECISION (signed_char_type_node))
    return unsignedp ? unsigned_char_type_node : signed_char_type_node;

  if (precision == TYPE_PRECISION (short_integer_type_node))
    return unsignedp ? short_unsigned_type_node : short_integer_type_node;

  if (precision == TYPE_PRECISION (long_integer_type_node))
    return unsignedp ? long_unsigned_type_node : long_integer_type_node;

  if (precision == TYPE_PRECISION (long_long_integer_type_node))
    return (unsignedp
	    ? long_long_unsigned_type_node
	    : long_long_integer_type_node);

  if (precision <= TYPE_PRECISION (intQI_type_node))
    return unsignedp ? unsigned_intQI_type_node : intQI_type_node;

  if (precision <= TYPE_PRECISION (intHI_type_node))
    return unsignedp ? unsigned_intHI_type_node : intHI_type_node;

  if (precision <= TYPE_PRECISION (intSI_type_node))
    return unsignedp ? unsigned_intSI_type_node : intSI_type_node;

  if (precision <= TYPE_PRECISION (intDI_type_node))
    return unsignedp ? unsigned_intDI_type_node : intDI_type_node;

  return NULL_TREE;
}


/* Return a data type that has machine mode MODE.
   If the mode is an integer,
   then UNSIGNEDP selects between signed and unsigned types.
   If the mode is a fixed-point mode,
   then UNSIGNEDP selects between saturating and nonsaturating types.  */

static tree
lto_type_for_mode (enum machine_mode mode, int unsigned_p)
{
  tree t;

  /* This hook is called by the middle-end.  For example,
     assign_stack_local_1 uses this hook to determine whether
     additional alignment is required for stack variables for which no
     explicit alignment is provided.  */
  if (SCALAR_INT_MODE_P (mode))
    return lto_type_for_size (GET_MODE_BITSIZE (mode), unsigned_p);

  if (mode == TYPE_MODE (float_type_node))
    return float_type_node;

  if (mode == TYPE_MODE (double_type_node))
    return double_type_node;

  if (mode == TYPE_MODE (long_double_type_node))
    return long_double_type_node;

  if (mode == TYPE_MODE (void_type_node))
    return void_type_node;

  if (mode == TYPE_MODE (build_pointer_type (char_type_node)))
    return (unsigned_p
	    ? make_unsigned_type (GET_MODE_PRECISION (mode))
	    : make_signed_type (GET_MODE_PRECISION (mode)));

  if (mode == TYPE_MODE (build_pointer_type (integer_type_node)))
    return (unsigned_p
	    ? make_unsigned_type (GET_MODE_PRECISION (mode))
	    : make_signed_type (GET_MODE_PRECISION (mode)));

  if (COMPLEX_MODE_P (mode))
    {
      enum machine_mode inner_mode;
      tree inner_type;

      if (mode == TYPE_MODE (complex_float_type_node))
	return complex_float_type_node;
      if (mode == TYPE_MODE (complex_double_type_node))
	return complex_double_type_node;
      if (mode == TYPE_MODE (complex_long_double_type_node))
	return complex_long_double_type_node;

      if (mode == TYPE_MODE (complex_integer_type_node) && !unsigned_p)
	return complex_integer_type_node;

      inner_mode = GET_MODE_INNER (mode);
      inner_type = lto_type_for_mode (inner_mode, unsigned_p);
      if (inner_type != NULL_TREE)
	return build_complex_type (inner_type);
    }
  else if (VECTOR_MODE_P (mode))
    {
      enum machine_mode inner_mode = GET_MODE_INNER (mode);
      tree inner_type = lto_type_for_mode (inner_mode, unsigned_p);
      if (inner_type != NULL_TREE)
	return build_vector_type_for_mode (inner_type, mode);
    }

  for (t = registered_builtin_types; t; t = TREE_CHAIN (t))
    if (TYPE_MODE (TREE_VALUE (t)) == mode)
      return TREE_VALUE (t);

  return NULL_TREE;
}

static int
lto_global_bindings_p (void) 
{
  return cfun == NULL;
}

static void
lto_set_decl_assembler_name (tree decl)
{
  /* This is almost the same as lhd_set_decl_assembler_name, except that
     we need to uniquify file-scope names, even if they are not
     TREE_PUBLIC, to avoid conflicts between individual files.  */
  tree id;

  if (TREE_PUBLIC (decl))
    id = targetm.mangle_decl_assembler_name (decl, DECL_NAME (decl));
  else
    {
      const char *name = IDENTIFIER_POINTER (DECL_NAME (decl));
      char *label;

      ASM_FORMAT_PRIVATE_NAME (label, name, DECL_UID (decl));
      id = get_identifier (label);
    }

  SET_DECL_ASSEMBLER_NAME (decl, id);
}

static tree
lto_pushdecl (tree t ATTRIBUTE_UNUSED)
{
  /* Do nothing, since we get all information from DWARF and LTO
     sections.  */
  return NULL_TREE;
}

static tree
lto_getdecls (void)
{
  return registered_builtin_fndecls;
}

static void
lto_write_globals (void)
{
  tree *vec = VEC_address (tree, lto_global_var_decls);
  int len = VEC_length (tree, lto_global_var_decls);
  emit_debug_global_declarations (vec, len);
  VEC_free (tree, gc, lto_global_var_decls);
}

static tree
lto_builtin_function (tree decl)
{
  /* Record it.  */
  TREE_CHAIN (decl) = registered_builtin_fndecls;
  registered_builtin_fndecls = decl;

  return decl;
}

static int
lto_types_compatible_p (tree type1, tree type2)
{
  if (TREE_CODE (type1) == RECORD_TYPE
      && TREE_CODE (type2) == RECORD_TYPE)
    {
      /* Check structural equality.  */
      tree f1, f2;

      for (f1 = TYPE_FIELDS (type1), f2 = TYPE_FIELDS (type2);
	   f1 && f2;
	   f1 = TREE_CHAIN (f1), f2 = TREE_CHAIN (f2))
	{
	  if (TREE_CODE (f1) != TREE_CODE (f2)
	      || DECL_NAME (f1) != DECL_NAME (f2))
	    break;
	}

      return f1 && f2 ? 0 : 1;
    }

  return TYPE_MAIN_VARIANT (type1) == TYPE_MAIN_VARIANT (type2);
}

static void
lto_register_builtin_type (tree type, const char *name)
{
  tree decl;

  decl = build_decl (TYPE_DECL, get_identifier (name), type);
  DECL_ARTIFICIAL (decl) = 1;
  if (!TYPE_NAME (type))
    TYPE_NAME (type) = decl;

  registered_builtin_types = tree_cons (0, type, registered_builtin_types);
}

/* Build nodes that would have be created by the C front-end; necessary
   for including builtin-types.def and ultimately builtins.def.  */

static void
lto_build_c_type_nodes (void)
{
  gcc_assert (void_type_node);

  void_list_node = build_tree_list (NULL_TREE, void_type_node);
  string_type_node = build_pointer_type (char_type_node);
  const_string_type_node
    = build_pointer_type (build_qualified_type (char_type_node, TYPE_QUAL_CONST));

  if (strcmp (SIZE_TYPE, "unsigned int") == 0)
    {
      intmax_type_node = integer_type_node;
      uintmax_type_node = unsigned_type_node;
      signed_size_type_node = integer_type_node;
    }
  else if (strcmp (SIZE_TYPE, "long unsigned int") == 0)
    {
      intmax_type_node = long_integer_type_node;
      uintmax_type_node = long_unsigned_type_node;
      signed_size_type_node = long_integer_type_node;
    }
  else
    gcc_unreachable ();

  wint_type_node = unsigned_type_node;
  pid_type_node = integer_type_node;
}


/* Return the runtime type for type T.  For LTO, we assume that each
   front end has generated the appropriate runtime types (see
   output_eh_region), so there is nothing for us to do here.  */

static tree
lto_eh_runtime_type (tree t)
{
  return t;
}


/* Initialize EH support.  */

static void
lto_init_eh (void)
{
  eh_personality_libfunc = init_one_libfunc (USING_SJLJ_EXCEPTIONS
					     ? "__gcc_personality_sj0"
					     : "__gcc_personality_v0");
  default_init_unwind_resume_libfunc ();
  lang_eh_runtime_type = lto_eh_runtime_type;
}


/* Perform LTO-specific initialization.  */

static bool
lto_init (void)
{
  /* We need to generate LTO if running in WPA mode.  */
  flag_generate_lto = flag_wpa;

  /* Initialize libcpp line maps for gcc_assert to work.  */
  linemap_add (line_table, LC_RENAME, 0, NULL, 0);

  /* Create the basic integer types.  */
  build_common_tree_nodes (flag_signed_char, /*signed_sizetype=*/false);

  /* Tell the middle end what type to use for the size of objects.  */
  if (strcmp (SIZE_TYPE, "unsigned int") == 0)
    {
      set_sizetype (unsigned_type_node);
      size_type_node = unsigned_type_node;
    }
  else if (strcmp (SIZE_TYPE, "long unsigned int") == 0)
    {
      set_sizetype (long_unsigned_type_node);
      size_type_node = long_unsigned_type_node;
    }
  else
    gcc_unreachable();

  /* Create other basic types.  */
  build_common_tree_nodes_2 (/*short_double=*/false);
  lto_build_c_type_nodes ();
  gcc_assert (va_list_type_node);

  if (TREE_CODE (va_list_type_node) == ARRAY_TYPE)
    {
      tree x = build_pointer_type (TREE_TYPE (va_list_type_node));
      lto_define_builtins (x, x);
    }
  else
    {
      lto_define_builtins (va_list_type_node,
			   build_reference_type (va_list_type_node));
    }

  targetm.init_builtins ();
  build_common_builtin_nodes ();

  /* Initialize LTO-specific data structures.  */
  lto_global_var_decls = VEC_alloc (tree, gc, 256);
  in_lto_p = true;

  if (flag_exceptions)
    lto_init_eh ();

  return true;
}

/* Initialize tree structures required by the LTO front end.  */

static void lto_init_ts (void)
{
  tree_contains_struct[NAMESPACE_DECL][TS_DECL_MINIMAL] = 1;
}


/* Overwrite the information in NODE based on FILE_DATA, TAG, FLAGS,
   STACK_SIZE and SELF_INSNS.  This is called either to initialize
   NODE or to replace the values in it, for instance becasue the first
   time we saw it, the function body was not available but now it
   is.  */

static void
input_overwrite_node (struct lto_file_decl_data* file_data,
		      struct cgraph_node *node,
		      enum LTO_cgraph_tags tag,
		      unsigned HOST_WIDEST_INT flags,
		      unsigned int stack_size,
		      unsigned int self_insns)
{
  node->aux = (void *)tag;
  node->local.inline_summary.estimated_self_stack_size = stack_size;
  node->local.inline_summary.self_insns = self_insns;
  node->global.insns = self_insns;
  node->local.lto_file_data = file_data;

  /* This list must be in the reverse order that they are set in
     output_node.  */
  node->local.vtable_method = lto_get_flag (&flags);
  node->local.for_functions_valid = lto_get_flag (&flags);
  node->local.redefined_extern_inline = lto_get_flag (&flags);
  node->local.disregard_inline_limits = lto_get_flag (&flags);
  node->local.inlinable = lto_get_flag (&flags);
  node->local.finalized = lto_get_flag (&flags);
  node->local.externally_visible = lto_get_flag (&flags);
  node->local.local = lto_get_flag (&flags);
  node->needed = lto_get_flag (&flags);
  node->analyzed = node->local.finalized;
  node->lowered = node->local.finalized;
}

/* Read a node from input_block IB.  TAG is the node's tag just read. 
   Return the node read or overwriten.  NODES points to a vector of nodes
   read so far.  */
 
static struct cgraph_node *
input_node (struct lto_file_decl_data* file_data,
	    struct lto_input_block *ib,
	    enum LTO_cgraph_tags tag,
	    VEC(cgraph_node_ptr, heap) *nodes)
{
  tree fn_decl;
  struct cgraph_node *node, *master_clone;
  unsigned int flags;
  int stack_size = 0;
  int self_insns = 0;
  unsigned decl_index;
  bool clone_p;
  int estimated_stack_size = 0;
  int stack_frame_offset = 0;
  int ref = LCC_NOT_FOUND;
  int insns = 0;
  int estimated_growth = 0;
  bool inlined = false;

  LTO_DEBUG_TOKEN ("clone_p");
  clone_p = lto_input_uleb128 (ib);

  if (clone_p)
    {
      LTO_DEBUG_TOKEN ("master");
      master_clone = VEC_index (cgraph_node_ptr, nodes, lto_input_sleb128 (ib));
      gcc_assert (master_clone);
      node = cgraph_clone_input_node (master_clone);
    }
  else
    {
      decl_index = lto_input_uleb128 (ib);
      fn_decl = lto_file_decl_data_get_fn_decl (file_data, decl_index);
      LTO_DEBUG_FN_NAME (fn_decl);
      node = cgraph_node (fn_decl);
    }

  LTO_DEBUG_TOKEN ("flags");
  flags = lto_input_uleb128 (ib);
  
  if (tag == LTO_cgraph_avail_node)
    {
      LTO_DEBUG_TOKEN ("stack_size");
      stack_size = lto_input_sleb128 (ib);
      LTO_DEBUG_TOKEN ("self_insns");
      self_insns = lto_input_sleb128 (ib);
    }
	  
  /* Read additional global data for LTRANS. */
  if (flag_ltrans)
    {
      LTO_DEBUG_TOKEN ("estimated_stack_size");
      estimated_stack_size = lto_input_sleb128 (ib);
      LTO_DEBUG_TOKEN ("stack_frame_offset");
      stack_frame_offset = lto_input_sleb128 (ib);
      LTO_DEBUG_TOKEN ("inlined_to");
      ref = lto_input_sleb128 (ib);

      LTO_DEBUG_TOKEN ("insns");
      insns = lto_input_sleb128 (ib);
      LTO_DEBUG_TOKEN ("estimated_growth");
      estimated_growth = lto_input_sleb128 (ib);
      LTO_DEBUG_TOKEN ("inlined");
      inlined = lto_input_uleb128 (ib);
    }

  gcc_assert (!node->aux);

  input_overwrite_node (file_data, node, tag, flags, stack_size,
			self_insns);
  if (flag_ltrans)
    {
      node->global.estimated_stack_size = estimated_stack_size;
      node->global.stack_frame_offset = stack_frame_offset;
      node->global.insns = insns;
      if (ref != LCC_NOT_FOUND)
	node->global.inlined_to = VEC_index (cgraph_node_ptr, nodes, ref);
      else
	node->global.inlined_to = NULL;
      node->global.estimated_growth = estimated_growth;
      node->global.inlined = inlined;
    }

  return node;
}

/* Read an edge from IB.  NODES points to a vector of previously read
   nodes for decoding caller and callee of the edge to be read.  */

static void
input_edge (struct lto_input_block *ib, VEC(cgraph_node_ptr, heap) *nodes)
{
  struct cgraph_node *caller, *callee;
  struct cgraph_edge *edge;
  unsigned int stmt_id;
  unsigned int count;
  unsigned int freq;
  unsigned int nest;
  cgraph_inline_failed_t inline_failed;
  unsigned HOST_WIDEST_INT flags;
  tree prevailing;
  enum ld_plugin_symbol_resolution caller_resolution;

  LTO_DEBUG_TOKEN ("caller");
  caller = VEC_index (cgraph_node_ptr, nodes, lto_input_sleb128 (ib));
  gcc_assert (caller);
  gcc_assert (caller->decl);

  LTO_DEBUG_TOKEN ("callee");
  callee = VEC_index (cgraph_node_ptr, nodes, lto_input_sleb128 (ib));
  gcc_assert (callee);
  gcc_assert (callee->decl);

  caller_resolution = lto_symtab_get_resolution (caller->decl);

  /* FIXME lto: The following assert would currently fail for  extern inline
     functions. */

/*   gcc_assert (caller_resolution == LDPR_PREVAILING_DEF */
/* 	      || caller_resolution == LDPR_PREVAILING_DEF_IRONLY */
/* 	      || caller_resolution == LDPR_PREEMPTED_REG */
/* 	      || caller_resolution == LDPR_PREEMPTED_IR); */

  LTO_DEBUG_TOKEN ("stmt");
  stmt_id = lto_input_uleb128 (ib);
  LTO_DEBUG_TOKEN ("inline_failed");
  inline_failed = lto_input_uleb128 (ib);
  LTO_DEBUG_TOKEN ("count");
  count = lto_input_uleb128 (ib);
  LTO_DEBUG_TOKEN ("frequency");
  freq = lto_input_uleb128 (ib);
  LTO_DEBUG_TOKEN ("loop_next");
  nest = lto_input_uleb128 (ib);
  LTO_DEBUG_TOKEN ("flags");
  flags = lto_input_uleb128 (ib);

  /* If the caller was preempted, don't create the edge. */
  if (caller_resolution == LDPR_PREEMPTED_REG
      || caller_resolution == LDPR_PREEMPTED_IR)
      return;

  /* Make sure the callee is the prevailing decl. */
  prevailing = lto_symtab_prevailing_decl (callee->decl);

  /* FIXME lto: remove this once extern inline in handled in lgen. */
  if (caller_resolution != LDPR_PREVAILING_DEF
      && caller_resolution != LDPR_PREVAILING_DEF_IRONLY
      && caller_resolution != LDPR_PREEMPTED_REG
      && caller_resolution != LDPR_PREEMPTED_IR)
    {
      /* If we have a extern inline, make sure it is the prevailing. */
      gcc_assert (prevailing == callee->decl);
    }

  if (prevailing != callee->decl)
    {
      /* We cannot replace a clone! */
      gcc_assert (callee == cgraph_node (callee->decl));


      callee = cgraph_node (prevailing);
      gcc_assert (callee);
    }

  edge = cgraph_create_edge (caller, callee, NULL, count, freq, nest);
  edge->lto_stmt_uid = stmt_id;
  edge->inline_failed = inline_failed;

  /* This list must be in the reverse order that they are set in
     output_edge.  */
  edge->call_stmt_cannot_inline_p = lto_get_flag (&flags);
  edge->indirect_call = lto_get_flag (&flags);
}

/* Input a cgraph from IB using the info in FILE_DATA.  */

static void
input_cgraph_1 (struct lto_file_decl_data* file_data,
		struct lto_input_block *ib)
{
  enum LTO_cgraph_tags tag;
  VEC(cgraph_node_ptr, heap) *nodes = NULL;
  VEC(cgraph_node_ptr, heap) *del_list = NULL;
  struct cgraph_node *node;
  unsigned i;

  tag = lto_input_uleb128 (ib);
  while (tag)
    {
      LTO_DEBUG_INDENT (tag);

      if (tag == LTO_cgraph_edge)
	  input_edge (ib, nodes);
      else 
	{
	  node = input_node (file_data, ib, tag, nodes);
	  gcc_assert (node);
	  gcc_assert (node->decl);
	  VEC_safe_push (cgraph_node_ptr, heap, nodes, node);
	}

      LTO_DEBUG_UNDENT();
      tag = lto_input_uleb128 (ib);
    }

  for (node = cgraph_nodes; node; node = node->next)
    {
      tree prevailing = lto_symtab_prevailing_decl (node->decl);

      if (prevailing != node->decl)
	VEC_safe_push (cgraph_node_ptr, heap, del_list, node);
    }

   for (i = 0; VEC_iterate (cgraph_node_ptr, del_list, i, node); i++)
     cgraph_remove_node (node);

   for (node = cgraph_nodes; node; node = node->next)
     if (cgraph_decide_is_function_needed (node))
       cgraph_mark_needed_node (node);

  VEC_free (cgraph_node_ptr, heap, nodes);
  VEC_free (cgraph_node_ptr, heap, del_list);
}

/* Input and merge the cgraph from each of the .o files passed to
   lto1.  */

static void
input_cgraph (void)
{
  struct lto_file_decl_data ** file_data_vec 
    = lto_get_file_decl_data ();
  struct lto_file_decl_data * file_data;
  unsigned int j = 0;
  struct cgraph_node *node;

#ifdef LTO_STREAM_DEBUGGING
  lto_debug_context.tag_names = LTO_cgraph_tag_names;
  lto_debug_context.stream_name = "cgraph";
#endif

  while ((file_data = file_data_vec[j++]))
    {
      const char *data;
      size_t len;
      struct lto_input_block *ib
	= lto_create_simple_input_block (file_data, 
					 LTO_section_cgraph, 
					 &data, &len);
      input_cgraph_1 (file_data, ib);
      lto_destroy_simple_input_block (file_data, 
				      LTO_section_cgraph, 
				      ib, data, len);
    } 

  /* Clear out the aux field that was used to store enough state to
     tell which nodes should be overwritten.  */
  for (node = cgraph_nodes; node; node = node->next)
    {
      gcc_assert (node->local.lto_file_data);
      node->aux = NULL;
    }
}

#undef LANG_HOOKS_NAME
#define LANG_HOOKS_NAME "GNU GIMPLE"
#undef LANG_HOOKS_INIT_OPTIONS
#define LANG_HOOKS_INIT_OPTIONS lto_init_options
#undef LANG_HOOKS_HANDLE_OPTION
#define LANG_HOOKS_HANDLE_OPTION lto_handle_option
#undef LANG_HOOKS_POST_OPTIONS
#define LANG_HOOKS_POST_OPTIONS lto_post_options
#undef LANG_HOOKS_MARK_ADDRESSABLE
#define LANG_HOOKS_MARK_ADDRESSABLE lto_mark_addressable
#undef LANG_HOOKS_TYPE_FOR_MODE
#define LANG_HOOKS_TYPE_FOR_MODE lto_type_for_mode
#undef LANG_HOOKS_TYPE_FOR_SIZE
#define LANG_HOOKS_TYPE_FOR_SIZE lto_type_for_size
#undef LANG_HOOKS_SET_DECL_ASSEMBLER_NAME
#define LANG_HOOKS_SET_DECL_ASSEMBLER_NAME lto_set_decl_assembler_name
#undef LANG_HOOKS_GLOBAL_BINDINGS_P
#define LANG_HOOKS_GLOBAL_BINDINGS_P lto_global_bindings_p
#undef LANG_HOOKS_PUSHDECL
#define LANG_HOOKS_PUSHDECL lto_pushdecl
#undef LANG_HOOKS_GETDECLS
#define LANG_HOOKS_GETDECLS lto_getdecls
#undef LANG_HOOKS_WRITE_GLOBALS
#define LANG_HOOKS_WRITE_GLOBALS lto_write_globals
#undef LANG_HOOKS_REGISTER_BUILTIN_TYPE
#define LANG_HOOKS_REGISTER_BUILTIN_TYPE lto_register_builtin_type
#undef LANG_HOOKS_BUILTIN_FUNCTION
#define LANG_HOOKS_BUILTIN_FUNCTION lto_builtin_function
#undef LANG_HOOKS_INIT
#define LANG_HOOKS_INIT lto_init
#undef LANG_HOOKS_PARSE_FILE
#define LANG_HOOKS_PARSE_FILE lto_main
#undef LANG_HOOKS_CALLGRAPH_EXPAND_FUNCTION
#define LANG_HOOKS_CALLGRAPH_EXPAND_FUNCTION tree_rest_of_compilation
#undef LANG_HOOKS_REDUCE_BIT_FIELD_OPERATIONS
#define LANG_HOOKS_REDUCE_BIT_FIELD_OPERATIONS true
#undef LANG_HOOKS_TYPES_COMPATIBLE_P
#define LANG_HOOKS_TYPES_COMPATIBLE_P lto_types_compatible_p

/* Attribute hooks.  */
#undef LANG_HOOKS_COMMON_ATTRIBUTE_TABLE
#define LANG_HOOKS_COMMON_ATTRIBUTE_TABLE lto_attribute_table
#undef LANG_HOOKS_FORMAT_ATTRIBUTE_TABLE
#define LANG_HOOKS_FORMAT_ATTRIBUTE_TABLE lto_format_attribute_table

#undef LANG_HOOKS_BEGIN_SECTION
#define LANG_HOOKS_BEGIN_SECTION lto_elf_begin_section
#undef LANG_HOOKS_APPEND_DATA
#define LANG_HOOKS_APPEND_DATA lto_elf_append_data
#undef LANG_HOOKS_END_SECTION
#define LANG_HOOKS_END_SECTION lto_elf_end_section
#undef LANG_HOOKS_INPUT_CGRAPH
#define LANG_HOOKS_INPUT_CGRAPH input_cgraph

#undef LANG_HOOKS_INIT_TS
#define LANG_HOOKS_INIT_TS lto_init_ts

const struct lang_hooks lang_hooks = LANG_HOOKS_INITIALIZER;

/* Language hooks that are not part of lang_hooks.  */

tree
convert (tree type ATTRIBUTE_UNUSED, tree expr ATTRIBUTE_UNUSED)
{
  gcc_unreachable ();
}

/* Tree walking support.  */

static enum lto_tree_node_structure_enum
lto_tree_node_structure (union lang_tree_node *t ATTRIBUTE_UNUSED)
{
  return TS_LTO_GENERIC;
}

#include "ggc.h"
#include "gtype-lto.h"
#include "gt-lto-lto-lang.h"
