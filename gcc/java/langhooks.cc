/* Java(TM) language-specific utility routines.
   Copyright (C) 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004
   Free Software Foundation, Inc.

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
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

Java and all Java-based marks are trademarks or registered trademarks
of Sun Microsystems, Inc. in the United States and other countries.
The Free Software Foundation is independent of Sun Microsystems, Inc.  */

#include "tree/glue.hh"

struct lang_identifier 
{
  struct tree_identifier id;
};

struct language_function GTY (())
{
  int unused;
};

struct lang_type GTY (())
{
  int unused;
};

struct lang_decl GTY (())
{
  int unused;
};

union lang_tree_node
  GTY ((desc ("0")))
{
  union tree_node GTY ((tag ("0"))) generic;
};



/* Table indexed by tree code giving a string containing a character
   classifying the tree code.  */

#define DEFTREECODE(SYM, NAME, TYPE, LENGTH) TYPE,

const enum tree_code_class tree_code_type[] = {
#include "tree.def"
  tcc_exceptional
};
#undef DEFTREECODE

/* Table indexed by tree code giving number of expression
   operands beyond the fixed part of the node structure.
   Not used for types or decls.  */

#define DEFTREECODE(SYM, NAME, TYPE, LENGTH) LENGTH,

const unsigned char tree_code_length[] = {
#include "tree.def"
  0
};
#undef DEFTREECODE

/* Names of tree components.
   Used for printing out the tree and error messages.  */
#define DEFTREECODE(SYM, NAME, TYPE, LEN) NAME,

const char *const tree_code_name[] = {
#include "tree.def"
  "@@dummy"
};
#undef DEFTREECODE



#undef LANG_HOOKS_NAME
#define LANG_HOOKS_NAME "GNU Java"

#undef LANG_HOOKS_INIT
#define LANG_HOOKS_INIT gcjx::init
#undef LANG_HOOKS_FINISH
#define LANG_HOOKS_FINISH gcjx::finish
#undef LANG_HOOKS_INIT_OPTIONS
#define LANG_HOOKS_INIT_OPTIONS gcjx::init_options
#undef LANG_HOOKS_HANDLE_OPTION
#define LANG_HOOKS_HANDLE_OPTION gcjx::handle_option
#undef LANG_HOOKS_POST_OPTIONS
#define LANG_HOOKS_POST_OPTIONS gcjx::post_options
#undef LANG_HOOKS_PARSE_FILE
#define LANG_HOOKS_PARSE_FILE gcjx::parse_file

#undef LANG_HOOKS_MARK_ADDRESSABLE
#define LANG_HOOKS_MARK_ADDRESSABLE gcjx::mark_addressable
#undef LANG_HOOKS_TRUTHVALUE_CONVERSION
#define LANG_HOOKS_TRUTHVALUE_CONVERSION gcjx::truthvalue_conversion
/* #undef LANG_HOOKS_DUP_LANG_SPECIFIC_DECL */
/* #define LANG_HOOKS_DUP_LANG_SPECIFIC_DECL gcjx::dup_lang_specific_decl */
/* #undef LANG_HOOKS_DECL_PRINTABLE_NAME */
/* #define LANG_HOOKS_DECL_PRINTABLE_NAME lang_printable_name */
/* #undef LANG_HOOKS_PRINT_ERROR_FUNCTION */
/* #define LANG_HOOKS_PRINT_ERROR_FUNCTION	gcjx::print_error_function */
/* #undef LANG_HOOKS_CAN_USE_BIT_FIELDS_P */
/* #define LANG_HOOKS_CAN_USE_BIT_FIELDS_P gcjx::can_use_bit_fields_p */

#undef LANG_HOOKS_TYPE_FOR_MODE
#define LANG_HOOKS_TYPE_FOR_MODE gcjx::type_for_mode
#undef LANG_HOOKS_TYPE_FOR_SIZE
#define LANG_HOOKS_TYPE_FOR_SIZE gcjx::type_for_size
#undef LANG_HOOKS_SIGNED_TYPE
#define LANG_HOOKS_SIGNED_TYPE gcjx::signed_type
#undef LANG_HOOKS_UNSIGNED_TYPE
#define LANG_HOOKS_UNSIGNED_TYPE gcjx::unsigned_type
#undef LANG_HOOKS_SIGNED_OR_UNSIGNED_TYPE
#define LANG_HOOKS_SIGNED_OR_UNSIGNED_TYPE gcjx::signed_or_unsigned_type

#undef LANG_HOOKS_GLOBAL_BINDINGS_P
#define LANG_HOOKS_GLOBAL_BINDINGS_P NULL
#undef LANG_HOOKS_INSERT_BLOCK
#define LANG_HOOKS_INSERT_BLOCK NULL
#undef LANG_HOOKS_PUSHDECL
#define LANG_HOOKS_PUSHDECL NULL
#undef LANG_HOOKS_GETDECLS
#define LANG_HOOKS_GETDECLS gcjx::getdecls
#undef LANG_HOOKS_DECL_OK_FOR_SIBCALL
#define LANG_HOOKS_DECL_OK_FOR_SIBCALL gcjx::decl_ok_for_sibcall


#undef LANG_HOOKS_BUILTIN_FUNCTION
#define LANG_HOOKS_BUILTIN_FUNCTION gcjx::builtin_function

/* #undef LANG_HOOKS_TREE_DUMP_DUMP_TREE_FN */
/* #define LANG_HOOKS_TREE_DUMP_DUMP_TREE_FN gcjx::dump_tree */

/* #undef LANG_HOOKS_GIMPLIFY_EXPR */
/* #define LANG_HOOKS_GIMPLIFY_EXPR gcjx::gimplify_expr */

/* #undef LANG_HOOKS_TREE_INLINING_WALK_SUBTREES */
/* #define LANG_HOOKS_TREE_INLINING_WALK_SUBTREES gcjx::tree_inlining_walk_subtrees */


/* #undef LANG_HOOKS_GET_CALLEE_FNDECL */
/* #define LANG_HOOKS_GET_CALLEE_FNDECL gcjx::get_callee_fndecl */

#undef LANG_HOOKS_CALLGRAPH_EXPAND_FUNCTION
#define LANG_HOOKS_CALLGRAPH_EXPAND_FUNCTION gcjx::expand_function

/* #undef LANG_HOOKS_CLEAR_BINDING_STACK */
/* #define LANG_HOOKS_CLEAR_BINDING_STACK gcjx::clear_binding_stack */

/* Each front end provides its own.  */
const struct lang_hooks lang_hooks = LANG_HOOKS_INITIALIZER;



/* Return a data type that has machine mode MODE.
   If the mode is an integer,
   then UNSIGNEDP selects between signed and unsigned types.  */
tree
gcjx::type_for_mode (enum machine_mode mode, int unsignedp)
{
  if (mode == TYPE_MODE (type_jint))
    return unsignedp ? type_juint : type_jint;
  if (mode == TYPE_MODE (type_jlong))
    return unsignedp ? type_julong : type_jlong;
  if (mode == TYPE_MODE (type_jshort))
    return unsignedp ? type_jushort : type_jshort;
  if (mode == TYPE_MODE (type_jbyte))
    return unsignedp ? type_jubyte : type_jbyte;
  if (mode == TYPE_MODE (type_jfloat))
    return type_jfloat;
  if (mode == TYPE_MODE (type_jdouble))
    return type_jdouble;

  return 0;
}

/* Return an integer type with BITS bits of precision,
   that is unsigned if UNSIGNEDP is nonzero, otherwise signed.  */
tree
gcjx::type_for_size (unsigned bits, int unsignedp)
{
  if (bits <= TYPE_PRECISION (type_jbyte))
    return unsignedp ? type_jubyte : type_jbyte;
  if (bits <= TYPE_PRECISION (type_jshort))
    return unsignedp ? type_jushort : type_jshort;
  if (bits <= TYPE_PRECISION (type_jint))
    return unsignedp ? type_juint : type_jint;
  if (bits <= TYPE_PRECISION (type_jlong))
    return unsignedp ? type_julong : type_jlong;
  return 0;
}

/* Return a type the same as TYPE except unsigned or
   signed according to UNSIGNEDP.  */
tree
gcjx::signed_or_unsigned_type (int unsignedp, tree type)
{
  if (! INTEGRAL_TYPE_P (type))
    return type;
  if (TYPE_PRECISION (type) == TYPE_PRECISION (type_jint))
    return unsignedp ? type_juint : type_jint;
  if (TYPE_PRECISION (type) == TYPE_PRECISION (type_jbyte))
    return unsignedp ? type_jubyte : type_jbyte;
  if (TYPE_PRECISION (type)
      == TYPE_PRECISION (type_jshort))
    return unsignedp ? type_jushort : type_jshort;
  if (TYPE_PRECISION (type) == TYPE_PRECISION (type_jlong))
    return unsignedp ? type_julong : type_jlong;
  return type;
}

/* Return a signed type the same as TYPE in other respects.  */
tree
gcjx::signed_type (tree type)
{
  return signed_or_unsigned_type (0, type);
}

/* Return an unsigned type the same as TYPE in other respects.  */
tree
gcjx::unsigned_type (tree type)
{
  return signed_or_unsigned_type (1, type);
}

/* Mark EXP saying that we need to be able to take the
   address of it; it should not be allocated in a register.
   Value is true if successful.  */
bool
gcjx::mark_addressable (tree exp)
{
  tree x = exp;
  while (1)
    switch (TREE_CODE (x))
      {
      case ADDR_EXPR:
      case COMPONENT_REF:
      case ARRAY_REF:
      case REALPART_EXPR:
      case IMAGPART_EXPR:
	x = TREE_OPERAND (x, 0);
	break;

      case TRUTH_ANDIF_EXPR:
      case TRUTH_ORIF_EXPR:
      case COMPOUND_EXPR:
	x = TREE_OPERAND (x, 1);
	break;

      case COND_EXPR:
	return (mark_addressable (TREE_OPERAND (x, 1))
		&& mark_addressable (TREE_OPERAND (x, 2)));

      case CONSTRUCTOR:
	TREE_ADDRESSABLE (x) = 1;
	return true;

      case INDIRECT_REF:
	/* We sometimes add a cast *(TYPE*)&FOO to handle type and mode
	   incompatibility problems.  Handle this case by marking FOO.  */
	if (TREE_CODE (TREE_OPERAND (x, 0)) == NOP_EXPR
	    && TREE_CODE (TREE_OPERAND (TREE_OPERAND (x, 0), 0)) == ADDR_EXPR)
	  {
	    x = TREE_OPERAND (TREE_OPERAND (x, 0), 0);
	    break;
	  }
	if (TREE_CODE (TREE_OPERAND (x, 0)) == ADDR_EXPR)
	  {
	    x = TREE_OPERAND (x, 0);
	    break;
	  }
	return true;

      case VAR_DECL:
      case CONST_DECL:
      case PARM_DECL:
      case RESULT_DECL:
      case FUNCTION_DECL:
	TREE_ADDRESSABLE (x) = 1;
	/* Falls through.  */
      default:
	return true;
    }
}

tree
gcjx::truthvalue_conversion (tree expr)
{
  /* It is simpler and generates better code to have only TRUTH_*_EXPR
     or comparison expressions as truth values at this level.

     This function should normally be identity for Java.  */

  switch (TREE_CODE (expr))
    {
    case EQ_EXPR:   case NE_EXPR:   case UNEQ_EXPR: case LTGT_EXPR:
    case LE_EXPR:   case GE_EXPR:   case LT_EXPR:   case GT_EXPR:
    case UNLE_EXPR: case UNGE_EXPR: case UNLT_EXPR: case UNGT_EXPR:
    case ORDERED_EXPR: case UNORDERED_EXPR:
    case TRUTH_ANDIF_EXPR:
    case TRUTH_ORIF_EXPR:
    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_XOR_EXPR:
    case TRUTH_NOT_EXPR:
    case ERROR_MARK:
      return expr;

    case INTEGER_CST:
      return integer_zerop (expr) ? boolean_false_node : boolean_true_node;

    case REAL_CST:
      return real_zerop (expr) ? boolean_false_node : boolean_true_node;

    case NEGATE_EXPR:
    case ABS_EXPR:
    case FLOAT_EXPR:
      /* These don't change whether an object is nonzero or zero.  */
      return truthvalue_conversion (TREE_OPERAND (expr, 0));

    case COND_EXPR:
      /* Distribute the conversion into the arms of a COND_EXPR.  */
      return fold
	(build3 (COND_EXPR, type_jboolean,
		 TREE_OPERAND (expr, 0),
		 truthvalue_conversion (TREE_OPERAND (expr, 1)),
		 truthvalue_conversion (TREE_OPERAND (expr, 2))));

    case NOP_EXPR:
      /* If this is widening the argument, we can ignore it.  */
      if (TYPE_PRECISION (TREE_TYPE (expr))
          >= TYPE_PRECISION (TREE_TYPE (TREE_OPERAND (expr, 0))))
        return truthvalue_conversion (TREE_OPERAND (expr, 0));
      /* fall through to default */

    default:
      return fold (build2 (NE_EXPR, type_jboolean, expr, boolean_false_node));
    }
}

/* Java calls can't, in general, be sibcalls because we need an
   accurate stack trace in order to guarantee correct operation of
   methods such as Class.forName(String) and
   SecurityManager.getClassContext().  */
bool
gcjx::decl_ok_for_sibcall (tree decl)
{
  // FIXME
  return true;
  //  return decl != NULL && DECL_CONTEXT (decl) == output_class;
}

void
gcjx::expand_function (tree fndecl)
{
  tree_rest_of_compilation (fndecl, 0);
}

tree
gcjx::getdecls ()
{
  return NULL_TREE;
}

/* Create an expression whose value is that of EXPR,
   converted to type TYPE.  The TREE_TYPE of the value
   is always TYPE.  This function implements all reasonable
   conversions; callers should filter out those that are
   not permitted by the language being compiled.  */
tree
convert (tree type, tree expr)
{
  enum tree_code code = TREE_CODE (type);

  if (!expr)
   return error_mark_node;

  if (type == TREE_TYPE (expr)
      || TREE_CODE (expr) == ERROR_MARK)
    return expr;
  if (TREE_CODE (TREE_TYPE (expr)) == ERROR_MARK)
    return error_mark_node;
  if (code == VOID_TYPE)
    return build1 (CONVERT_EXPR, type, expr);
  if (code == BOOLEAN_TYPE)
    return fold (build1 (NOP_EXPR, type, expr));
  if (code == INTEGER_TYPE)
    {
      /* fold very helpfully sets the overflow status if a type
	 overflows in a narrowing integer conversion, but Java doesn't
	 care.  */
      tree tmp = fold (convert_to_integer (type, expr));
      TREE_OVERFLOW (tmp) = 0;
      return tmp;
    }	  
  if (code == REAL_TYPE)
    return fold (convert_to_real (type, expr));
  if (code == CHAR_TYPE)
    return fold (build1 (NOP_EXPR, type, expr));
  if (code == POINTER_TYPE)
    return fold (convert_to_pointer (type, expr));
  error ("conversion to non-scalar type requested");
  return error_mark_node;
}

#include "gt-java-langhooks.h"
