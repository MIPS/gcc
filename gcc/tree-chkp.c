/* Pointer Bounds Checker insrumentation pass.
   Copyright (C) 2013 Free Software Foundation, Inc.
   Contributed by Ilya Enkovich (ilya.enkovich@intel.com)

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree-core.h"
#include "stor-layout.h"
#include "varasm.h"
#include "tree.h"
#include "target.h"
#include "tree-iterator.h"
#include "tree-cfg.h"
#include "langhooks.h"
#include "tree-pass.h"
#include "hashtab.h"
#include "pointer-set.h"
#include "diagnostic.h"
#include "ggc.h"
#include "output.h"
#include "internal-fn.h"
#include "is-a.h"
#include "predict.h"
#include "cfgloop.h"
#include "stringpool.h"
#include "tree-ssa-alias.h"
#include "tree-ssanames.h"
#include "tree-ssa-operands.h"
#include "tree-ssa-address.h"
#include "tree-ssa.h"
#include "ipa-inline.h"
#include "basic-block.h"
#include "tree-ssa-loop-niter.h"
#include "gimple-expr.h"
#include "gimple.h"
#include "tree-phinodes.h"
#include "gimple-ssa.h"
#include "ssa-iterators.h"
#include "gimple-pretty-print.h"
#include "gimple-iterator.h"
#include "gimplify.h"
#include "gimplify-me.h"
#include "print-tree.h"
#include "expr.h"
#include "tree-ssa-propagate.h"
#include "gimple-fold.h"
#include "tree-chkp.h"
#include "gimple-walk.h"
#include "rtl.h" /* For MEM_P.  */
#include "tree-dfa.h"
#include <sstream>

/*  Pointer Bounds Checker instruments code with memory checks to find
    out-of-bounds memory accesses.  Checks are performed by computing
    bounds for each pointer and then comparing address of accessed
    memory before pointer dereferencing.

    1. Function clones.

    In instrumented code each pointer is provided with bounds.  For input
    pointer parameters it means we also have bounds passed.  For calls it
    means we have additional bounds arguments for pointer arguments.

    To have all IPA optimizations working correctly we have to express
    dataflow between passed and received bounds explicitly via additional
    entries in function declaration arguments list and in function type.
    Since we may have both instrumented and not instrumented code at the
    same time, we cannot replace all original functions with their
    instrumented variants.  Therefore we create clones (versions) instead.

    Instrumentation clones creation is a separate IPA pass which is a part
    of early local passes.  Clones are created after SSA is built (because
    instrumentation pass works on SSA) and before any transformations
    which may change pointer flow and therefore lead to incorrect code
    instrumentation (possibly causing false bounds check failures).

    Instrumentation clones have pointer bounds arguments added rigth after
    pointer arguments.  Clones have assembler name of the original
    function with suffix added.  New assembler name is in transparent
    alias chain with the original name.  Thus we expect all calls to the
    original and instrumented functions look similar in assembler.

    During instrumentation versioning pass we create instrumented versions
    of all function with body and also for all their aliases and thunks.
    Clones for functions with no body are created on demand (usually
    during call instrumentation).

    Original and instrumented function nodes are connected with IPA
    reference IPA_REF_CHKP.  It is mostly done to have reachability
    analysis working correctly.  We may have no references to the
    instrumented function in the code but it still should be counted
    as reachable if the original function is reachable.

    When original function bodies are not needed anymore we release
    them and transform functions into a special kind of thunks.  Each
    thunk has a call edge to the instrumented version.  These thunks
    help to keep externally visible instrumented functions visible
    when linker reolution files are used.  Linker has no info about
    connection between original and instrumented function and
    therefore we may wrongly decide (due to difference in assember
    names) that instrumented function version is local and can be
    removed.

    2. Instrumentation.

    There are few things to instrument:

    a) Memory accesses - add checker calls to check address of accessed memory
    against bounds of dereferenced pointer.  Obviously safe memory
    accesses like static variable access does not have to be instrumented
    with checks.

    Example:

      val_2 = *p_1;

      with 4 bytes access is transformed into:

      __builtin___chkp_bndcl (__bound_tmp.1_3, p_1);
      D.1_4 = p_1 + 3;
      __builtin___chkp_bndcu (__bound_tmp.1_3, D.1_4);
      val_2 = *p_1;

      where __bound_tmp.1_3 are bounds computed for pointer p_1,
      __builtin___chkp_bndcl is a lower bound check and
      __builtin___chkp_bndcu is an upper bound check.

    b) Pointer stores.

    When pointer is stored in memory we need to store its bounds.  To
    achieve compatibility of instrumented code with regular codes
    we have to keep data layout and store bounds in special bound tables
    via special checker call.  Implementation of bounds table may vary for
    different platforms.  It has to associate pointer value and its
    location (it is required because we may have two equal pointers
    with different bounds stored in different places) with bounds.
    Another checker builtin allows to get bounds for specified pointer
    loaded from specified location.

    Example:

      buf1[i_1] = &buf2;

      is transformed into:

      buf1[i_1] = &buf2;
      D.1_2 = &buf1[i_1];
      __builtin___chkp_bndstx (D.1_2, &buf2, __bound_tmp.1_2);

      where __bound_tmp.1_2 are bounds of &buf2.

    c) Static initialization.

    The special case of pointer store is static pointer initialization.
    Bounds initialization is performed in a few steps:
      - register all static initializations in front-end using
      chkp_register_var_initializer
      - when file compilation finishes we create functions with special
      attribute 'chkp ctor' and put explicit initialization code
      (assignments) for all statically initialized pointers.
      - when checker constructor is compiled checker pass adds required
      bounds initialization for all statically initialized pointers
      - since we do not actually need excess pointers initialization
      in checker constructor we remove such assignments from them

    d) Calls.

    For each call in the code we should add additional arguments to pass
    bounds for pointer arguments.  We determine type of call arguments
    using arguments list from function declaration; if function
    declaration is not available we use function type; otherwise
    (e.g. for unnamed arguments) we use type of passed value. Function
    declaration/type is replaced with the instrumented one.

    Example:

      val_1 = foo (&buf1, &buf2, &buf1, 0);

      is translated into:

      val_1 = foo.chkp (&buf1, __bound_tmp.1_2, &buf2, __bound_tmp.1_3,
                        &buf1, __bound_tmp.1_2, 0);

    e) Returns.

    If function returns a pointer value we have to return bounds also.
    A new operand was added for return statement to hold returned bounds.

    Example:

      return &_buf1;

      is transformed into

      return &_buf1, __bound_tmp.1_1;

    3. Bounds computation.

    Compiler is fully responsible for computing bounds to be used for each
    memory access.  The first step for bounds computation is to find the
    origin of pointer dereferenced for memory access.  Basing on pointer
    origin we define a way to compute its bounds.  There are just few
    possible cases:

    a) Pointer is returned by call.

    In this case we use corresponding checker builtin method to obtain returned
    bounds.

    Example:

      buf_1 = malloc (size_2);
      foo (buf_1);

      is translated into:

      buf_1 = malloc (size_2);
      __bound_tmp.1_3 = __builtin___chkp_bndret (buf_1);
      foo (buf_1, __bound_tmp.1_3);

    b) Pointer is an address of an object.

    In this case compiler tries to compute objects size and create corresponding
    bounds.  If object has incomplete type then special checker builtin is used to
    obtain its size at runtime.

    Example:

      foo ()
      {
        <unnamed type> __bound_tmp.3;
	static int buf[100];

	<bb 3>:
	__bound_tmp.3_2 = __builtin___chkp_bndmk (&buf, 400);

	<bb 2>:
	return &buf, __bound_tmp.3_2;
      }

    Example:

      Address of an object 'extern int buf[]' with incomplete type is
      returned.

      foo ()
      {
        <unnamed type> __bound_tmp.4;
	long unsigned int __size_tmp.3;

	<bb 3>:
	__size_tmp.3_4 = __builtin_ia32_sizeof (buf);
	__bound_tmp.4_3 = __builtin_ia32_bndmk (&buf, __size_tmp.3_4);

	<bb 2>:
	return &buf, __bound_tmp.4_3;
      }

    c) Pointer is the result of object narrowing.

    It happens when we use pointer to an object to compute pointer to a part
    of an object.  E.g. we take pointer to a field of a structure. In this
    case we perform bounds intersection using bounds of original object and
    bounds of object's part (which are computed basing on its type).

    There may be some debatable questions about when narrowing should occur
    and when it should not.  To avoid false bound violations in correct
    programs we do not perform narrowing when address of an array element is
    obtained (it has address of the whole array) and when address of the first
    structure field is obtained (because it is guaranteed to be equal to
    address of the whole structure and it is legal to cast it back to structure).

    Default narrowing behavior may be changed using compiler flags.

    Example:

      In this example address of the second structure field is returned.

      foo (struct A * p, __bounds_type __bounds_of_p)
      {
        <unnamed type> __bound_tmp.3;
	int * _2;
	int * _5;

	<bb 2>:
	_5 = &p_1(D)->second_field;
	__bound_tmp.3_6 = __builtin___chkp_bndmk (_5, 4);
	__bound_tmp.3_8 = __builtin___chkp_intersect (__bound_tmp.3_6,
	                                              __bounds_of_p_3(D));
	_2 = &p_1(D)->second_field;
	return _2, __bound_tmp.3_8;
      }

    Example:

      In this example address of the first field of array element is returned.

      foo (struct A * p, , __bounds_type __bounds_of_p, int i)
      {
	long unsigned int _3;
	long unsigned int _4;
	struct A * _6;
	int * _7;

	<bb 2>:
	_3 = (long unsigned int) i_1(D);
	_4 = _3 * 8;
	_6 = p_5(D) + _4;
	_7 = &_6->first_field;
	return _7, __bounds_of_p_2(D);
      }


    d) Pointer is the result of pointer arithmetic or type cast.

    In this case bounds of the base pointer are used.

    e) Pointer is loaded from the memory.

    In this case we just need to load bounds from the bounds table.

    Example:

      foo ()
      {
        <unnamed type> __bound_tmp.3;
	static int * buf;
	int * _2;

	<bb 2>:
	_2 = buf;
	__bound_tmp.3_4 = __builtin___chkp_bndldx (&buf, _2);
	return _2, __bound_tmp.3_4;
      }

*/

typedef void (*assign_handler)(tree, tree, void *);

enum check_type
{
  CHECK_LOWER_BOUND,
  CHECK_UPPER_BOUND
};

struct pol_item
{
  tree cst;
  tree var;
};

struct address_t
{
  vec<struct pol_item> pol;
};

/* Structure to hold check informtation.  */
struct check_info
{
  /* Type of the check.  */
  check_type type;
  /* Address used for the check.  */
  address_t addr;
  /* Bounds used for the check.  */
  tree bounds;
  /* Check statement.  Can be NULL for removed checks.  */
  gimple stmt;
};

/* Structure to hold checks information for BB.  */
struct bb_checks
{
  vec<struct check_info, va_heap, vl_ptr> checks;
};

static tree chkp_get_zero_bounds ();
static tree chkp_find_bounds (tree ptr, gimple_stmt_iterator *iter);
static tree chkp_find_bounds_loaded (tree ptr, tree ptr_src,
				     gimple_stmt_iterator *iter);
static tree chkp_find_bounds_abnormal (tree ptr, tree phi, edge e);
static void chkp_collect_value (tree ssa_name, address_t &res);
static void chkp_parse_array_and_component_ref (tree node, tree *ptr,
						tree *elt, bool *safe,
						bool *bitfield,
						tree *bounds,
						gimple_stmt_iterator *iter,
						bool innermost_bounds);

#define chkp_bndldx_fndecl \
  (targetm.builtin_chkp_function (BUILT_IN_CHKP_BNDLDX))
#define chkp_bndstx_fndecl \
  (targetm.builtin_chkp_function (BUILT_IN_CHKP_BNDSTX))
#define chkp_checkl_fndecl \
  (targetm.builtin_chkp_function (BUILT_IN_CHKP_BNDCL))
#define chkp_checku_fndecl \
  (targetm.builtin_chkp_function (BUILT_IN_CHKP_BNDCU))
#define chkp_bndmk_fndecl \
  (targetm.builtin_chkp_function (BUILT_IN_CHKP_BNDMK))
#define chkp_ret_bnd_fndecl \
  (targetm.builtin_chkp_function (BUILT_IN_CHKP_BNDRET))
#define chkp_intersect_fndecl \
  (targetm.builtin_chkp_function (BUILT_IN_CHKP_INTERSECT))
#define chkp_narrow_bounds_fndecl \
  (targetm.builtin_chkp_function (BUILT_IN_CHKP_NARROW))
#define chkp_sizeof_fndecl \
  (targetm.builtin_chkp_function (BUILT_IN_CHKP_SIZEOF))
#define chkp_extract_lower_fndecl \
  (targetm.builtin_chkp_function (BUILT_IN_CHKP_EXTRACT_LOWER))
#define chkp_extract_upper_fndecl \
  (targetm.builtin_chkp_function (BUILT_IN_CHKP_EXTRACT_UPPER))

static vec<struct bb_checks, va_heap, vl_ptr> check_infos = vNULL;

static GTY (()) tree chkp_uintptr_type;

static GTY (()) tree chkp_zero_bounds_var;
static GTY (()) tree chkp_none_bounds_var;

static GTY (()) basic_block entry_block;
static GTY (()) tree zero_bounds;
static GTY (()) tree none_bounds;
static GTY (()) tree incomplete_bounds;
static GTY (()) tree tmp_var;
static GTY (()) tree size_tmp_var;

struct pointer_set_t *chkp_invalid_bounds;
struct pointer_set_t *chkp_completed_bounds_set;
struct pointer_map_t *chkp_reg_bounds;
struct pointer_map_t *chkp_reg_addr_bounds;
struct pointer_map_t *chkp_incomplete_bounds_map;
struct pointer_map_t *chkp_bounds_map;
struct pointer_map_t *chkp_static_var_bounds;

static bool in_chkp_pass;

static GTY ((if_marked ("tree_vec_map_marked_p"),
	     param_is (struct tree_vec_map)))
     htab_t chkp_abnormal_phi_copies;
static GTY ((if_marked ("tree_map_marked_p"),
	     param_is (struct tree_map))) htab_t chkp_size_decls;

#define CHKP_BOUND_TMP_NAME "__bound_tmp"
#define CHKP_SIZE_TMP_NAME "__size_tmp"
#define CHKP_SIZE_OF_SYMBOL_PREFIX "__chkp_size_of_"
#define CHKP_BOUNDS_OF_SYMBOL_PREFIX "__chkp_bounds_of_"
#define CHKP_STRING_BOUNDS_PREFIX "__chkp_string_bounds_"
#define CHKP_VAR_BOUNDS_PREFIX "__chkp_var_bounds_"
#define CHKP_ZERO_BOUNDS_VAR_NAME "__chkp_zero_bounds"
#define CHKP_NONE_BOUNDS_VAR_NAME "__chkp_none_bounds"

/* Static checker constructors may become very large and their
   compilation with optimization may take too much time.
   Therefore we put a limit to number of statements in one
   construcor.  Tests with 100 000 statically initialized
   pointers showed following compilation times on Sandy Bridge
   server (used -O2):
   limit    100 => ~18 sec.
   limit    300 => ~22 sec.
   limit   1000 => ~30 sec.
   limit   3000 => ~49 sec.
   limit   5000 => ~55 sec.
   limit  10000 => ~76 sec.
   limit 100000 => ~532 sec.  */
#define MAX_STMTS_IN_STATIC_CHKP_CTOR (optimize > 0 ? 5000 : 100000)

struct chkp_ctor_stmt_list
{
  tree stmts;
  int avail;
};

/* Return 1 if function FNDECL is instrumented by Pointer
   Bounds Checker.  */
bool
chkp_function_instrumented_p (tree fndecl)
{
  return fndecl
    && lookup_attribute ("chkp instrumented", DECL_ATTRIBUTES (fndecl));
}

/* Mark function FNDECL as instrumented.  */
void
chkp_function_mark_instrumented (tree fndecl)
{
  if (chkp_function_instrumented_p (fndecl))
    return;

  DECL_ATTRIBUTES (fndecl)
    = tree_cons (get_identifier ("chkp instrumented"), NULL,
		 DECL_ATTRIBUTES (fndecl));
}


/* Build clone of FNDECL with added bound params.  */
static tree
chkp_build_instrumented_fndecl (tree fndecl)
{
  tree new_decl = copy_node (fndecl);
  tree new_name;
  std::string s;

  /* We want called_as_built_in recall instrumented calls
     to instrumented built-in functions.  Therefore use
     DECL_NAME for cloning instead of DECL_ASSEMBLER_NAME.

     DECL_ASSEMBLER NAME is the same because instrumented
     function replaces the original version.  */
  s = IDENTIFIER_POINTER (DECL_NAME (fndecl));
  s += ".chkp";
  DECL_NAME (new_decl) = get_identifier (s.c_str ());

  /* References to the original and to the instrumented version
     should look the same in the output assembly.  And we cannot
     use the same assembler name for the instrumented version
     because it conflicts with decl merging algorithms during LTO.
     Achieve the result by using transparent alias name for the
     instrumented version.  */
  s = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (fndecl));
  s += ".chkp";
  new_name = get_identifier (s.c_str ());
  IDENTIFIER_TRANSPARENT_ALIAS (new_name) = 1;
  TREE_CHAIN (new_name) = DECL_ASSEMBLER_NAME (fndecl);
  SET_DECL_ASSEMBLER_NAME (new_decl, new_name);

  /* For functions with body versioning will make a copy of arguments.
     For functions with no body we need to do it here.  */
  if (!gimple_has_body_p (fndecl))
    DECL_ARGUMENTS (new_decl) = copy_list (DECL_ARGUMENTS (fndecl));

  /* We are going to modify attributes list and therefore should
     make own copy.  */
  DECL_ATTRIBUTES (new_decl) = copy_list (DECL_ATTRIBUTES (fndecl));

  return new_decl;
}


/* Fix operands of attribute from ATTRS list named ATTR_NAME.
   Integer operands are replaced with values according to
   INDEXES map having LEN elements.  For operands out of len
   we just add DELTA.  */

static void
chkp_map_attr_arg_indexes (tree attrs, const char *attr_name,
			   unsigned *indexes, int len, int delta)
{
  tree attr = lookup_attribute (attr_name, attrs);
  tree op;

  if (!attr)
    return;

  TREE_VALUE (attr) = copy_list (TREE_VALUE (attr));
  for (op = TREE_VALUE (attr); op; op = TREE_CHAIN (op))
    {
      int idx;

      if (TREE_CODE (TREE_VALUE (op)) != INTEGER_CST)
	continue;

      idx = TREE_INT_CST_LOW (TREE_VALUE (op));

      /* If idx exceeds indexes length then we just
	 keep it at the same distance from the last
	 known arg.  */
      if (idx > len)
	idx += delta;
      else
	idx = indexes[idx - 1] + 1;
      TREE_VALUE (op) = build_int_cst (TREE_TYPE (TREE_VALUE (op)), idx);
    }
}

/* Make a copy of function type ORIG_TYPE adding pointer
   bounds as additional arguments.  */

static tree
chkp_copy_function_type_adding_bounds (tree orig_type)
{
  tree type;
  tree arg_type, attrs, t;
  unsigned len = list_length (TYPE_ARG_TYPES (orig_type));
  unsigned *indexes = XALLOCAVEC (unsigned, len);
  unsigned idx = 0, new_idx = 0;

  for (arg_type = TYPE_ARG_TYPES (orig_type);
       arg_type;
       arg_type = TREE_CHAIN (arg_type))
    if (TREE_VALUE (arg_type) == void_type_node)
      continue;
    else if (BOUNDED_TYPE_P (TREE_VALUE (arg_type))
	     || pass_by_reference (NULL, TYPE_MODE (TREE_VALUE (arg_type)),
				   TREE_VALUE (arg_type), true)
	     || chkp_type_has_pointer (TREE_VALUE (arg_type)))
      break;

  /* We may use original type if there are no bounds passed.  */
  if (!arg_type)
    return orig_type;

  type = copy_node (orig_type);
  TYPE_ARG_TYPES (type) = copy_list (TYPE_ARG_TYPES (type));

  for (arg_type = TYPE_ARG_TYPES (type);
       arg_type;
       arg_type = TREE_CHAIN (arg_type))
    {
      indexes[idx++] = new_idx++;

      /* pass_by_reference returns 1 for void type,
	 so check for it first.  */
      if (TREE_VALUE (arg_type) == void_type_node)
	continue;
      else if (BOUNDED_TYPE_P (TREE_VALUE (arg_type))
	       || pass_by_reference (NULL, TYPE_MODE (TREE_VALUE (arg_type)),
				     TREE_VALUE (arg_type), true))
	{
	  tree new_type = build_tree_list (NULL_TREE,
					   pointer_bounds_type_node);
	  TREE_CHAIN (new_type) = TREE_CHAIN (arg_type);
	  TREE_CHAIN (arg_type) = new_type;

	  arg_type = TREE_CHAIN (arg_type);
	  new_idx++;
	}
      else if (chkp_type_has_pointer (TREE_VALUE (arg_type)))
	{
	  bitmap slots = chkp_find_bound_slots (TREE_VALUE (arg_type));
	  bitmap_iterator bi;
	  unsigned bnd_no;

	  EXECUTE_IF_SET_IN_BITMAP (slots, 0, bnd_no, bi)
	    {
	      tree new_type = build_tree_list (NULL_TREE,
					       pointer_bounds_type_node);
	      TREE_CHAIN (new_type) = TREE_CHAIN (arg_type);
	      TREE_CHAIN (arg_type) = new_type;

	      arg_type = TREE_CHAIN (arg_type);
	      new_idx++;
	    }
	  BITMAP_FREE (slots);
	}
    }

  /* If function type has attribute with arg indexes then
     we have to copy it fixing attribute ops.  Map for
     fixing is in indexes array.  */
  attrs = TYPE_ATTRIBUTES (type);
  if (lookup_attribute ("nonnull", attrs)
      || lookup_attribute ("format", attrs)
      || lookup_attribute ("format_arg", attrs))
    {
      int delta = new_idx - len;
      attrs = copy_list (TYPE_ATTRIBUTES (type));
      chkp_map_attr_arg_indexes (attrs, "nonnull", indexes, len, delta);
      chkp_map_attr_arg_indexes (attrs, "format", indexes, len, delta);
      chkp_map_attr_arg_indexes (attrs, "format_arg", indexes, len, delta);
      TYPE_ATTRIBUTES (type) = attrs;
    }

  t = TYPE_MAIN_VARIANT (orig_type);
  if (orig_type != t)
    {
      TYPE_MAIN_VARIANT (type) = t;
      TYPE_NEXT_VARIANT (type) = TYPE_NEXT_VARIANT (t);
      TYPE_NEXT_VARIANT (t) = type;
    }
  else
    {
      TYPE_MAIN_VARIANT (type) = type;
      TYPE_NEXT_VARIANT (type) = NULL;
    }


  return type;
}

/* For given function NODE add bounds arguments to arguments
   list.  */
static void
chkp_add_bounds_params_to_function (struct cgraph_node *node)
{
  tree arg;

  for (arg = DECL_ARGUMENTS (node->decl); arg; arg = DECL_CHAIN (arg))
    if (BOUNDED_P (arg))
      {
	std::string new_name = CHKP_BOUNDS_OF_SYMBOL_PREFIX;
	if (DECL_NAME (arg))
	  new_name += IDENTIFIER_POINTER (DECL_NAME (arg));
	else
	  {
	    char uid[10];
	    snprintf (uid, 10, "D.%u", DECL_UID (arg));
	    new_name += uid;
	  }

	tree new_arg = build_decl (DECL_SOURCE_LOCATION (arg), PARM_DECL,
				   get_identifier (new_name.c_str ()),
				   pointer_bounds_type_node);
	DECL_ARG_TYPE (new_arg) = pointer_bounds_type_node;
	DECL_CONTEXT (new_arg) = DECL_CONTEXT (arg);
	DECL_CHAIN (new_arg) = DECL_CHAIN (arg);
	DECL_CHAIN (arg) = new_arg;

	arg = DECL_CHAIN (arg);

      }
    else if (chkp_type_has_pointer (TREE_TYPE (arg)))
      {
	tree orig_arg = arg;
	bitmap slots = chkp_find_bound_slots (TREE_TYPE (arg));
	bitmap_iterator bi;
	unsigned bnd_no;

	EXECUTE_IF_SET_IN_BITMAP (slots, 0, bnd_no, bi)
	  {
	    std::ostringstream ss;
	    ss << CHKP_BOUNDS_OF_SYMBOL_PREFIX;
	    if (DECL_NAME (orig_arg))
	      ss << IDENTIFIER_POINTER (DECL_NAME (orig_arg));
	    else
	      ss << "D." << DECL_UID (arg);
	    ss << "__" << (bnd_no * POINTER_SIZE / BITS_PER_UNIT);

	    tree new_arg = build_decl (DECL_SOURCE_LOCATION (orig_arg),
				       PARM_DECL,
				       get_identifier (ss.str ().c_str ()),
				       pointer_bounds_type_node);
	    DECL_ARG_TYPE (new_arg) = pointer_bounds_type_node;
	    DECL_CONTEXT (new_arg) = DECL_CONTEXT (orig_arg);
	    DECL_CHAIN (new_arg) = DECL_CHAIN (arg);
	    DECL_CHAIN (arg) = new_arg;

	    arg = DECL_CHAIN (arg);
	  }
	BITMAP_FREE (slots);
      }

  TREE_TYPE (node->decl) =
    chkp_copy_function_type_adding_bounds (TREE_TYPE (node->decl));
}

/* Return clone created for instrumentation of NODE or NULL.  */
static struct cgraph_node *
chkp_maybe_create_clone (tree fndecl)
{
  struct cgraph_node *node = cgraph_get_create_node (fndecl), *clone;

  gcc_assert (!node->instrumentation_clone);

  clone = node->instrumented_version;

  if (!clone)
    {
      tree new_decl = chkp_build_instrumented_fndecl (fndecl);
      struct cgraph_edge *e;
      struct ipa_ref *ref;
      int i;

      clone = cgraph_copy_node_for_versioning (node, new_decl, vNULL, NULL); 
      clone->externally_visible = node->externally_visible;
      clone->local = node->local;
      clone->address_taken = node->address_taken;
      clone->thunk = node->thunk;
      clone->alias = node->alias;
      clone->weakref = node->weakref;
      clone->cpp_implicit_alias = node->cpp_implicit_alias;

      if (gimple_has_body_p (fndecl))
	{
	  /* If function will not be instrumented, then it's instrumented
	     version is a thunk for the original.  */
	  if (lookup_attribute ("bnd_legacy", DECL_ATTRIBUTES (fndecl))
	      || (flag_chkp_instrument_marked_only
		  && !lookup_attribute ("bnd_instrument", DECL_ATTRIBUTES (fndecl))))
	    {
	      clone->thunk.thunk_p = true;
	      clone->thunk.add_pointer_bounds_args = true;
	      cgraph_create_edge (clone, node, NULL, 0, CGRAPH_FREQ_BASE);
	    }
	  else
	    {
	      tree_function_versioning (fndecl, new_decl, NULL, false,
					NULL, false, NULL, NULL);
	      clone->lowered = true;
	    }
	}

      chkp_add_bounds_params_to_function (clone);
      clone->instrumented_version = node;
      clone->orig_decl = fndecl;
      clone->instrumentation_clone = true;
      node->instrumented_version = clone;

      /* Clones have the same comdat group as originals.  */
      if (node->same_comdat_group
	  || DECL_ONE_ONLY (node->decl))
	symtab_add_to_same_comdat_group (clone, node);

      /* Thunks have no body to instrument, therefore mark them as
	 already instrumented.  */
      if (clone->thunk.thunk_p)
	chkp_function_mark_instrumented (clone->decl);

      if (gimple_has_body_p (fndecl))
	cgraph_call_function_insertion_hooks (clone);

      /* Clone all aliases.  */
      for (i = 0; ipa_ref_list_referring_iterate (&node->ref_list, i, ref); i++)
	if (ref->use == IPA_REF_ALIAS)
	  {
	    struct cgraph_node *alias = dyn_cast <cgraph_node> (ref->referring);
	    struct cgraph_node *chkp_alias
	      = chkp_maybe_create_clone (alias->decl);
	    ipa_record_reference (chkp_alias, clone, IPA_REF_ALIAS, NULL);
	  }

      /* Clone all thunks.  */
      for (e = node->callers; e; e = e->next_caller)
	if (e->caller->thunk.thunk_p)
	  {
	    struct cgraph_node *thunk
	      = chkp_maybe_create_clone (e->caller->decl);
	    /* Redirect thunk clone edge to the node clone.  */
	    cgraph_redirect_edge_callee (thunk->callees, clone);
	  }

      /* For aliases and thunks we should make sure target is cloned
	 to have proper references and edges.  */
      if (node->thunk.thunk_p)
	chkp_maybe_create_clone (node->callees->callee->decl);
      else if (node->alias)
	{
	  struct cgraph_node *target;

	  ref = ipa_ref_list_first_reference (&node->ref_list);
	  if (ref)
	    chkp_maybe_create_clone (ref->referred->decl);

	  if (node->alias_target)
	    {
	      if (TREE_CODE (node->alias_target) == FUNCTION_DECL)
		{
		  target = chkp_maybe_create_clone (node->alias_target);
		  clone->alias_target = target->decl;
		}
	      else
		clone->alias_target = node->alias_target;
	    }
	}

      /* Add IPA reference.  It's main role is to keep instrumented
	 version reachable while original node is reachable.  */
      ref = ipa_record_reference (node, clone, IPA_REF_CHKP, NULL);
    }

  return clone;
}

/* Create versions of functions to be instrumented.  */
static unsigned int
chkp_versioning (void)
{
  struct cgraph_node *node;

  FOR_EACH_DEFINED_FUNCTION (node)
    {
      if (!node->instrumentation_clone
	  && !node->instrumented_version
	  && !node->alias
	  && !node->thunk.thunk_p
	  && !lookup_attribute ("bnd_legacy", DECL_ATTRIBUTES (node->decl))
	  && (!flag_chkp_instrument_marked_only
	      || lookup_attribute ("bnd_instrument",
				   DECL_ATTRIBUTES (node->decl))))
	chkp_maybe_create_clone (node->decl);
    }

  /* Mark all aliases and thunks of functions with no instrumented
     version as legacy function.  */
  FOR_EACH_DEFINED_FUNCTION (node)
    {
      if (!node->instrumentation_clone
	  && !node->instrumented_version
	  && (node->alias || node->thunk.thunk_p)
	  && !lookup_attribute ("bnd_legacy", DECL_ATTRIBUTES (node->decl)))
	DECL_ATTRIBUTES (node->decl)
	  = tree_cons (get_identifier ("bnd_legacy"), NULL,
		       DECL_ATTRIBUTES (node->decl));
    }


  return 0;
}

/* In this pass we remove bodies of functions having
   instrumented version.  Functions with removed bodies
   become a special kind of thunks to provide a connection
   between calls to the original version and instrumented
   function.  */

static unsigned int
chkp_produce_thunks (void)
{
  struct cgraph_node *node;

  FOR_EACH_DEFINED_FUNCTION (node)
    {
      if (!node->instrumentation_clone
	  && node->instrumented_version
	  && gimple_has_body_p (node->decl)
	  && gimple_has_body_p (node->instrumented_version->decl))
	{
	  cgraph_release_function_body (node);
	  cgraph_node_remove_callees (node);
	  ipa_remove_all_references (&node->ref_list);

	  node->thunk.thunk_p = true;
	  node->thunk.add_pointer_bounds_args = true;
	  cgraph_create_edge (node, node->instrumented_version, NULL,
			      0, CGRAPH_FREQ_BASE);
	  ipa_record_reference (node, node->instrumented_version,
				IPA_REF_CHKP, NULL);
	}
    }

  symtab_remove_unreachable_nodes (true, dump_file);

  return 0;
}

/* Mark statement S to not be instrumented.  */
static void
chkp_mark_stmt (gimple s)
{
  gimple_set_plf (s, GF_PLF_1, true);
}

/* Mark statement S to be instrumented.  */
static void
chkp_unmark_stmt (gimple s)
{
  gimple_set_plf (s, GF_PLF_1, false);
}

/* Return 1 if statement S should not be instrumented.  */
static bool
chkp_marked_stmt_p (gimple s)
{
  return gimple_plf (s, GF_PLF_1);
}

/* Get var to be used for bound temps.  */
static tree
chkp_get_tmp_var (void)
{
  if (!tmp_var)
    tmp_var = create_tmp_reg (pointer_bounds_type_node, CHKP_BOUND_TMP_NAME);

  return tmp_var;
}

/* Get SSA_NAME to be used as temp.  */
static tree
chkp_get_tmp_reg (gimple stmt)
{
  if (in_chkp_pass)
    return make_ssa_name (chkp_get_tmp_var (), stmt);

  return make_temp_ssa_name (pointer_bounds_type_node, stmt,
			     CHKP_BOUND_TMP_NAME);
}

/* Get var to be used for size temps.  */
static tree
chkp_get_size_tmp_var (void)
{
  if (!size_tmp_var)
    size_tmp_var = create_tmp_reg (chkp_uintptr_type, CHKP_SIZE_TMP_NAME);

  return size_tmp_var;
}

/* Register bounds BND for address of OBJ.  */
static void
chkp_register_addr_bounds (tree obj, tree bnd)
{
  tree *slot;

  if (bnd == incomplete_bounds)
    return;

  slot = (tree *)pointer_map_insert (chkp_reg_addr_bounds, obj);
  *slot = bnd;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Regsitered bound ");
      print_generic_expr (dump_file, bnd, 0);
      fprintf (dump_file, " for address of ");
      print_generic_expr (dump_file, obj, 0);
      fprintf (dump_file, "\n");
    }
}

/* Return bounds registered for address of OBJ.  */
static tree
chkp_get_registered_addr_bounds (tree obj)
{
  tree *slot = (tree *)pointer_map_contains (chkp_reg_addr_bounds, obj);
  return slot ? *slot : NULL_TREE;
}

/* Mark BOUNDS as completed.  */
static void
chkp_mark_completed_bounds (tree bounds)
{
  pointer_set_insert (chkp_completed_bounds_set, bounds);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Marked bounds ");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, " as completed\n");
    }
}

/* Return 1 if BOUNDS were marked as completed and 0 otherwise.  */
static bool
chkp_completed_bounds (tree bounds)
{
  return pointer_set_contains (chkp_completed_bounds_set, bounds);
}

/* Clear comleted bound marks.  */
static void
chkp_erase_completed_bounds (void)
{
  pointer_set_destroy (chkp_completed_bounds_set);
  chkp_completed_bounds_set = pointer_set_create ();
}

/* Mark BOUNDS associated with PTR as incomplete.  */
static void
chkp_register_incomplete_bounds (tree bounds, tree ptr)
{
  tree *slot = (tree *)pointer_map_insert (chkp_incomplete_bounds_map, bounds);
  *slot = ptr;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Regsitered incomplete bounds ");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, " for ");
      print_generic_expr (dump_file, ptr, 0);
      fprintf (dump_file, "\n");
    }
}

/* Return 1 if BOUNDS are incomplete and 0 otherwise.  */
static bool
chkp_incomplete_bounds (tree bounds)
{
  void **slot;

  if (bounds == incomplete_bounds)
    return true;

  if (chkp_completed_bounds (bounds))
    return false;

  slot = pointer_map_contains (chkp_incomplete_bounds_map, bounds);

  return slot != NULL;
}

/* Clear incomleted bound marks.  */
static void
chkp_erase_incomplete_bounds (void)
{
  pointer_map_destroy (chkp_incomplete_bounds_map);
  chkp_incomplete_bounds_map = pointer_map_create ();
}

/* Build and return bndmk call which creates bounds for structure
   pointed by PTR.  Structure should have complete type.  */
tree
chkp_make_bounds_for_struct_addr (tree ptr)
{
  tree type = TREE_TYPE (ptr);
  tree size;

  gcc_assert (POINTER_TYPE_P (type));

  size = TYPE_SIZE (TREE_TYPE (type));

  gcc_assert (size);

  return build_call_nary (pointer_bounds_type_node,
			  build_fold_addr_expr (chkp_bndmk_fndecl),
			  2, ptr, size);
}

/* Traversal function for chkp_may_finish_incomplete_bounds.
   Set RES to 0 if at least one argument of phi statement
   defining bounds (passed in KEY arg) is unknown.
   Traversal stops when first unknown phi argument is found.  */
static bool
chkp_may_complete_phi_bounds (const void *key, void **slot ATTRIBUTE_UNUSED,
			      void *res)
{
  const_tree bounds = (const_tree)key;
  gimple phi;
  unsigned i;

  gcc_assert (TREE_CODE (bounds) == SSA_NAME);

  phi = SSA_NAME_DEF_STMT (bounds);

  gcc_assert (phi && gimple_code (phi) == GIMPLE_PHI);

  for (i = 0; i < gimple_phi_num_args (phi); i++)
    {
      tree phi_arg = gimple_phi_arg_def (phi, i);
      if (!phi_arg)
	{
	  *((bool *)res) = false;
	  /* Do not need to traverse further.  */
	  return false;
	}
    }

  return true;
}

/* Return 1 if all phi nodes created for bounds have their
   arguments computed.  */
static bool
chkp_may_finish_incomplete_bounds (void)
{
  bool res = true;

  pointer_map_traverse (chkp_incomplete_bounds_map,
			chkp_may_complete_phi_bounds,
			&res);

  return res;
}

/* Helper function for chkp_finish_incomplete_bounds.
   Recompute args for bounds phi node.  */
static bool
chkp_recompute_phi_bounds (const void *key, void **slot,
			   void *res ATTRIBUTE_UNUSED)
{
  tree bounds = const_cast<tree> ((const_tree)key);
  tree ptr = *(tree *)slot;
  gimple bounds_phi;
  gimple ptr_phi;
  unsigned i;

  gcc_assert (TREE_CODE (bounds) == SSA_NAME);
  gcc_assert (TREE_CODE (ptr) == SSA_NAME);

  bounds_phi = SSA_NAME_DEF_STMT (bounds);
  ptr_phi = SSA_NAME_DEF_STMT (ptr);

  gcc_assert (bounds_phi && gimple_code (bounds_phi) == GIMPLE_PHI);
  gcc_assert (ptr_phi && gimple_code (ptr_phi) == GIMPLE_PHI);

  for (i = 0; i < gimple_phi_num_args (bounds_phi); i++)
    {
      tree ptr_arg = gimple_phi_arg_def (ptr_phi, i);
      edge e = gimple_phi_arg_edge (ptr_phi, i);
      tree bound_arg;

      /* This bounds computation is final for the PHI node.
	 We have to create bound copies for abnormal edges
	 to avoid problem in SSA names coalescing.  */
      if (e->flags & EDGE_ABNORMAL)
	bound_arg = chkp_find_bounds_abnormal (ptr_arg, bounds, e);
      else
	bound_arg = chkp_find_bounds (ptr_arg, NULL);

      add_phi_arg (bounds_phi, bound_arg,
		   gimple_phi_arg_edge (ptr_phi, i),
		   UNKNOWN_LOCATION);
    }

  return true;
}

/* Mark BOUNDS as invalid.  */
static void
chkp_mark_invalid_bounds (tree bounds)
{
  pointer_set_insert (chkp_invalid_bounds, bounds);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Marked bounds ");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, " as invalid\n");
    }
}

/* Return 1 if BOUNDS were marked as invalid and 0 otherwise.  */
static bool
chkp_valid_bounds (tree bounds)
{
  if (bounds == zero_bounds || bounds == none_bounds)
    return false;

  return !pointer_set_contains (chkp_invalid_bounds, bounds);
}

/* Helper function for chkp_finish_incomplete_bounds.
   Check all arguments of phi nodes trying to find
   valid completed bounds.  If there is at least one
   such arg then bounds produced by phi node are marked
   as valid completed bounds and all phi args are
   recomputed.  */
static bool
chkp_find_valid_phi_bounds (const void *key, void **slot, void *res)
{
  tree bounds = const_cast<tree> ((const_tree)key);
  gimple phi;
  unsigned i;

  gcc_assert (TREE_CODE (bounds) == SSA_NAME);

  if (chkp_completed_bounds (bounds))
    return true;

  phi = SSA_NAME_DEF_STMT (bounds);

  gcc_assert (phi && gimple_code (phi) == GIMPLE_PHI);

  for (i = 0; i < gimple_phi_num_args (phi); i++)
    {
      tree phi_arg = gimple_phi_arg_def (phi, i);

      gcc_assert (phi_arg);

      if (chkp_valid_bounds (phi_arg) && !chkp_incomplete_bounds (phi_arg))
	{
	  *((bool *)res) = true;
	  chkp_mark_completed_bounds (bounds);
	  chkp_recompute_phi_bounds (key, slot, NULL);
	  return true;
	}
    }

  return true;
}

/* Helper function for chkp_finish_incomplete_bounds.
   Marks all incompleted bounds as invalid.  */
static bool
chkp_mark_invalid_bounds_walker (const void *key,
				 void **slot ATTRIBUTE_UNUSED,
				 void *res ATTRIBUTE_UNUSED)
{
  tree bounds = const_cast<tree> ((const_tree)key);

  if (!chkp_completed_bounds (bounds))
    {
      chkp_mark_invalid_bounds (bounds);
      chkp_mark_completed_bounds (bounds);
    }
  return true;
}

/* When all bound phi nodes have all their args computed
   we have enough info to find valid bounds.  We iterate
   through all incompleted bounds searching for valid
   bounds.  Found valid bounds are marked as completed
   and all remaining incompleted bounds are recomputed.
   Process continues until no new valid bounds may be
   found.  All remained incompleted bounds are marked as
   invalid (i.e. have no valid source of bounds).  */
static void
chkp_finish_incomplete_bounds (void)
{
  bool found_valid;

  while (found_valid)
    {
      found_valid = false;

      pointer_map_traverse (chkp_incomplete_bounds_map,
			    chkp_find_valid_phi_bounds,
			    &found_valid);

      if (found_valid)
	pointer_map_traverse (chkp_incomplete_bounds_map,
			      chkp_recompute_phi_bounds,
			      NULL);
    }

  pointer_map_traverse (chkp_incomplete_bounds_map,
			chkp_mark_invalid_bounds_walker,
			NULL);
  pointer_map_traverse (chkp_incomplete_bounds_map,
			chkp_recompute_phi_bounds,
			&found_valid);

  chkp_erase_completed_bounds ();
  chkp_erase_incomplete_bounds ();
}

/* Return 1 if type TYPE is a pointer type or a
   structure having a pointer type as one of its fields.
   Otherwise return 0.  */
bool
chkp_type_has_pointer (const_tree type)
{
  bool res = false;

  if (BOUNDED_TYPE_P (type))
    res = true;
  else if (RECORD_OR_UNION_TYPE_P (type))
    {
      tree field;

      for (field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
	if (TREE_CODE (field) == FIELD_DECL)
	  res = res || chkp_type_has_pointer (TREE_TYPE (field));
    }
  else if (TREE_CODE (type) == ARRAY_TYPE)
    res = chkp_type_has_pointer (TREE_TYPE (type));

  return res;
}

unsigned
chkp_type_bounds_count (const_tree type)
{
  unsigned res = 0;

  if (!type)
    res = 0;
  else if (BOUNDED_TYPE_P (type))
    res = 1;
  else if (RECORD_OR_UNION_TYPE_P (type))
    {
      bitmap have_bound = chkp_find_bound_slots (type);
      res = bitmap_count_bits (have_bound);
      BITMAP_FREE (have_bound);
    }

  return res;
}

/* Get bounds associated with NODE via
   chkp_set_bounds call.  */
tree
chkp_get_bounds (tree node)
{
  tree *slot;

  if (!chkp_bounds_map)
    return NULL_TREE;

  slot = (tree *)pointer_map_contains (chkp_bounds_map, node);
  return slot ? *slot : NULL_TREE;
}

/* Associate bounds VAL with NODE.  */
void
chkp_set_bounds (tree node, tree val)
{
  tree *slot;

  if (!chkp_bounds_map)
    chkp_bounds_map = pointer_map_create ();

  slot = (tree *)pointer_map_insert (chkp_bounds_map, node);
  *slot = val;
}

/* Check if statically initialized variable VAR require
   static bounds initilization.  If VAR is added into
   bounds initlization list then 1 is returned. Otherwise
   return 0.  */
extern bool
chkp_register_var_initializer (tree var)
{
  if (!flag_check_pointer_bounds)
    return false;

  gcc_assert (TREE_CODE (var) == VAR_DECL);
  gcc_assert (DECL_INITIAL (var)
	      && DECL_INITIAL (var) != error_mark_node);

  if (TREE_STATIC (var)
      && chkp_type_has_pointer (TREE_TYPE (var)))
    {
      varpool_node_for_decl (var)->need_bounds_init = 1;
      return true;
    }

  return false;
}

/* Helper function for chkp_finish_file.

   Add new modification statement (RHS is assigned to LHS)
   into list of static initilizer statementes (passed in ARG).
   If statements list becomes too big, emit checker constructor
   and start the new one.  */
static void
chkp_add_modification_to_stmt_list (tree lhs,
				    tree rhs,
				    void *arg)
{
  struct chkp_ctor_stmt_list *stmts = (struct chkp_ctor_stmt_list *)arg;
  tree modify;

  if (!useless_type_conversion_p (TREE_TYPE (lhs), TREE_TYPE (rhs)))
    rhs = build1 (CONVERT_EXPR, TREE_TYPE (lhs), rhs);

  modify = build2 (MODIFY_EXPR, TREE_TYPE (lhs), lhs, rhs);
  append_to_statement_list (modify, &stmts->stmts);

  stmts->avail--;
}

/* Build and return ADDR_EXPR for specified object OBJ.  */
static tree
chkp_build_addr_expr (tree obj)
{
  return TREE_CODE (obj) == TARGET_MEM_REF
    ? tree_mem_ref_addr (ptr_type_node, obj)
    : build_fold_addr_expr (obj);
}

/* Helper function for chkp_finish_file.
   Initialize bound variable BND_VAR with bounds of variable
   VAR to statements list STMTS.  If statements list becomes
   too big, emit checker constructor and start the new one.  */
static void
chkp_output_static_bounds (tree bnd_var, tree var,
			   struct chkp_ctor_stmt_list *stmts)
{
  tree lb, ub, size;

  if (TREE_CODE (var) == STRING_CST)
    {
      lb = build1 (CONVERT_EXPR, size_type_node, chkp_build_addr_expr (var));
      size = build_int_cst (size_type_node, TREE_STRING_LENGTH (var) - 1);
    }
  else if (DECL_SIZE (var)
	   && !chkp_variable_size_type (TREE_TYPE (var)))
    {
      /* Compute bounds using statically known size.  */
      lb = build1 (CONVERT_EXPR, size_type_node, chkp_build_addr_expr (var));
      size = size_binop (MINUS_EXPR, DECL_SIZE_UNIT (var), size_one_node);
    }
  else
    {
      /* Compute bounds using dynamic size.  */
      tree call;

      lb = build1 (CONVERT_EXPR, size_type_node, chkp_build_addr_expr (var));
      call = build1 (ADDR_EXPR,
		     build_pointer_type (TREE_TYPE (chkp_sizeof_fndecl)),
		     chkp_sizeof_fndecl);
      size = build_call_nary (TREE_TYPE (TREE_TYPE (chkp_sizeof_fndecl)),
			      call, 1, var);

      if (flag_chkp_zero_dynamic_size_as_infinite)
	{
	  tree max_size, cond;

	  max_size = build2 (MINUS_EXPR, size_type_node, size_zero_node, lb);
	  cond = build2 (NE_EXPR, boolean_type_node, size, size_zero_node);
	  size = build3 (COND_EXPR, size_type_node, cond, size, max_size);
	}

      size = size_binop (MINUS_EXPR, size, size_one_node);
    }

  ub = size_binop (PLUS_EXPR, lb, size);
  stmts->avail -= targetm.chkp_initialize_bounds (bnd_var, lb, ub,
						  &stmts->stmts);
  if (stmts->avail <= 0)
    {
      cgraph_build_static_cdtor ('B', stmts->stmts,
				 MAX_RESERVED_INIT_PRIORITY + 2);
      stmts->avail = MAX_STMTS_IN_STATIC_CHKP_CTOR;
      stmts->stmts = NULL;
    }
}

/* Register bounds BND for object PTR in global bounds table.  */
static void
chkp_register_bounds (tree ptr, tree bnd)
{
  tree *slot;

  if (!chkp_reg_bounds)
    return;

  /* Do nothing if bounds are incomplete_bounds
     because it means bounds will be recomputed.  */
  if (bnd == incomplete_bounds)
    return;

  slot = (tree *)pointer_map_insert (chkp_reg_bounds, ptr);
  *slot = bnd;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Regsitered bound ");
      print_generic_expr (dump_file, bnd, 0);
      fprintf (dump_file, " for pointer ");
      print_generic_expr (dump_file, ptr, 0);
      fprintf (dump_file, "\n");
    }
}

/* Get bounds registered for object PTR in global bounds table.  */
static tree
chkp_get_registered_bounds (tree ptr)
{
  tree *slot;

  if (!chkp_reg_bounds)
    return NULL_TREE;

  slot = (tree *)pointer_map_contains (chkp_reg_bounds, ptr);
  return slot ? *slot : NULL_TREE;
}

/* Add bound retvals to return statement pointed by GSI.  */

static void
chkp_add_bounds_to_ret_stmt (gimple_stmt_iterator *gsi)
{
  gimple ret = gsi_stmt (*gsi);
  tree retval = gimple_return_retval (ret);
  tree ret_decl = DECL_RESULT (cfun->decl);
  tree bounds;

  if (!retval)
    return;

  if (BOUNDED_P (ret_decl))
    {
      bounds = chkp_find_bounds (retval, gsi);
      chkp_register_bounds (ret_decl, bounds);
      gimple_return_set_retbnd (ret, bounds);
    }

  update_stmt (ret);
}

/* Force OP to be suitable for using as an argument for call.
   New statements (if any) go to SEQ.  */
static tree
chkp_force_gimple_call_op (tree op, gimple_seq *seq)
{
  gimple_seq stmts;
  gimple_stmt_iterator si;

  op = force_gimple_operand (unshare_expr (op), &stmts, true, NULL_TREE);

  for (si = gsi_start (stmts); !gsi_end_p (si); gsi_next (&si))
    chkp_mark_stmt (gsi_stmt (si));

  gimple_seq_add_seq (seq, stmts);

  return op;
}

/* Generate lower bound check for memory access by ADDR.
   Check is inserted before the position pointed by ITER.
   DIRFLAG indicates whether memory access is load or store.  */
static void
chkp_check_lower (tree addr, tree bounds,
		  gimple_stmt_iterator iter,
		  location_t location ATTRIBUTE_UNUSED,
		  tree dirflag)
{
  gimple_seq seq;
  gimple check;
  tree node;

  if (bounds == chkp_get_zero_bounds ())
    return;

  if (dirflag == integer_zero_node
      && !flag_chkp_check_read)
    return;

  if (dirflag == integer_one_node
      && !flag_chkp_check_write)
    return;

  seq = NULL;

  node = chkp_force_gimple_call_op (addr, &seq);

  check = gimple_build_call (chkp_checkl_fndecl, 2, node, bounds);
  chkp_mark_stmt (check);
  gimple_call_set_with_bounds (check, true);
  gimple_seq_add_stmt (&seq, check);

  gsi_insert_seq_before (&iter, seq, GSI_SAME_STMT);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      gimple before = gsi_stmt (iter);
      fprintf (dump_file, "Generated lower bound check for statement ");
      print_gimple_stmt (dump_file, before, 0, TDF_VOPS|TDF_MEMSYMS);
      fprintf (dump_file, "  ");
      print_gimple_stmt (dump_file, check, 0, TDF_VOPS|TDF_MEMSYMS);
    }
}

/* Generate upper bound check for memory access by ADDR.
   Check is inserted before the position pointed by ITER.
   DIRFLAG indicates whether memory access is load or store.  */
static void
chkp_check_upper (tree addr, tree bounds,
		  gimple_stmt_iterator iter,
		  location_t location ATTRIBUTE_UNUSED,
		  tree dirflag)
{
  gimple_seq seq;
  gimple check;
  tree node;

  if (bounds == chkp_get_zero_bounds ())
    return;

  if (dirflag == integer_zero_node
      && !flag_chkp_check_read)
    return;

  if (dirflag == integer_one_node
      && !flag_chkp_check_write)
    return;

  seq = NULL;

  node = chkp_force_gimple_call_op (addr, &seq);

  check = gimple_build_call (chkp_checku_fndecl, 2, node, bounds);
  chkp_mark_stmt (check);
  gimple_call_set_with_bounds (check, true);
  gimple_seq_add_stmt (&seq, check);

  gsi_insert_seq_before (&iter, seq, GSI_SAME_STMT);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      gimple before = gsi_stmt (iter);
      fprintf (dump_file, "Generated upper bound check for statement ");
      print_gimple_stmt (dump_file, before, 0, TDF_VOPS|TDF_MEMSYMS);
      fprintf (dump_file, "  ");
      print_gimple_stmt (dump_file, check, 0, TDF_VOPS|TDF_MEMSYMS);
    }
}

/* Generate lower and upper bound checks for memory access
   to memory slot [FIRST, LAST] againsr BOUNDS.  Checks
   are inserted before the position pointed by ITER.
   DIRFLAG indicates whether memory access is load or store.  */
static void
chkp_check_mem_access (tree first, tree last, tree bounds,
		       gimple_stmt_iterator iter,
		       location_t location,
		       tree dirflag)
{
  chkp_check_lower (first, bounds, iter, location, dirflag);
  chkp_check_upper (last, bounds, iter, location, dirflag);
}

/* Replace call to _bnd_chk_* pointed by GSI with
   bndcu and bndcl calls.  DIRFLAG determines whether
   check is for read or write.  */

void
chkp_replace_address_check_builtin (gimple_stmt_iterator *gsi,
				    tree dirflag)
{
  gimple_stmt_iterator call_iter = *gsi;
  gimple call = gsi_stmt (*gsi);
  tree fndecl = gimple_call_fndecl (call);
  tree addr = gimple_call_arg (call, 0);
  tree bounds = chkp_find_bounds (addr, gsi);

  if (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_CHECK_PTR_LBOUNDS
      || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_CHECK_PTR_BOUNDS)
    chkp_check_lower (addr, bounds, *gsi, gimple_location (call), dirflag);

  if (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_CHECK_PTR_UBOUNDS)
    chkp_check_upper (addr, bounds, *gsi, gimple_location (call), dirflag);

  if (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_CHECK_PTR_BOUNDS)
    {
      tree size = gimple_call_arg (call, 1);
      addr = fold_build_pointer_plus (addr, size);
      addr = fold_build_pointer_plus_hwi (addr, -1);
      chkp_check_upper (addr, bounds, *gsi, gimple_location (call), dirflag);
    }

  gsi_remove (&call_iter, true);
}

/* Replace call to _bnd_get_ptr_* pointed by GSI with
   corresponding bounds extract call.  */

void
chkp_replace_extract_builtin (gimple_stmt_iterator *gsi)
{
  gimple call = gsi_stmt (*gsi);
  tree fndecl = gimple_call_fndecl (call);
  tree addr = gimple_call_arg (call, 0);
  tree bounds = chkp_find_bounds (addr, gsi);
  gimple extract;

  if (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_GET_PTR_LBOUND)
    fndecl = chkp_extract_lower_fndecl;
  else if (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_GET_PTR_UBOUND)
    fndecl = chkp_extract_upper_fndecl;
  else
    gcc_unreachable ();

  extract = gimple_build_call (fndecl, 1, bounds);
  gimple_call_set_lhs (extract, gimple_call_lhs (call));
  chkp_mark_stmt (extract);

  gsi_replace (gsi, extract, false);
}

/* Return COMPONENT_REF accessing FIELD in OBJ.  */
static tree
chkp_build_component_ref (tree obj, tree field)
{
  tree res;

  /* If object is TMR then we do not use component_ref but
     add offset instead.  We need it to be able to get addr
     of the reasult later.  */
  if (TREE_CODE (obj) == TARGET_MEM_REF)
    {
      tree offs = TMR_OFFSET (obj);
      offs = fold_binary_to_constant (PLUS_EXPR, TREE_TYPE (offs),
				      offs, DECL_FIELD_OFFSET (field));

      gcc_assert (offs);

      res = copy_node (obj);
      TREE_TYPE (res) = TREE_TYPE (field);
      TMR_OFFSET (res) = offs;
    }
  else
    res = build3 (COMPONENT_REF, TREE_TYPE (field), obj, field, NULL_TREE);

  return res;
}

/* Return ARRAY_REF for array ARR and index IDX with
   specified element type ETYPE and element size ESIZE.  */
static tree
chkp_build_array_ref (tree arr, tree etype, tree esize,
		      unsigned HOST_WIDE_INT idx)
{
  tree index = build_int_cst (size_type_node, idx);
  tree res;

  /* If object is TMR then we do not use array_ref but
     add offset instead.  We need it to be able to get addr
     of the reasult later.  */
  if (TREE_CODE (arr) == TARGET_MEM_REF)
    {
      tree offs = TMR_OFFSET (arr);

      esize = fold_binary_to_constant (MULT_EXPR, TREE_TYPE (esize),
				     esize, index);
      gcc_assert(esize);

      offs = fold_binary_to_constant (PLUS_EXPR, TREE_TYPE (offs),
				    offs, esize);
      gcc_assert (offs);

      res = copy_node (arr);
      TREE_TYPE (res) = etype;
      TMR_OFFSET (res) = offs;
    }
  else
    res = build4 (ARRAY_REF, etype, arr, index, NULL_TREE, NULL_TREE);

  return res;
}

/* Return true when T can be shared.  */

static bool
chkp_can_be_shared (tree t)
{
  if (IS_TYPE_OR_DECL_P (t)
      || is_gimple_min_invariant (t)
      || TREE_CODE (t) == SSA_NAME
      || t == error_mark_node
      || TREE_CODE (t) == IDENTIFIER_NODE
      || TREE_CODE (t) == CASE_LABEL_EXPR
      || DECL_P (t))
    return true;

  return false;
}

/* Helper function for chkp_add_bounds_to_call_stmt.
   Fill ALL_BOUNDS output array with created bounds.

   OFFS is used for recursive calls and holds basic
   offset of TYPE in outer structure in bits.

   ITER points a position where bounds are searched.

   ALL_BOUNDS[i] is filled with elem bounds if there
   is a field in TYPE which has pointer type and offset
   equal to i * POINTER_SIZE in bits.  */
static void
chkp_find_bounds_for_elem (tree elem, tree *all_bounds,
			   HOST_WIDE_INT offs,
			   gimple_stmt_iterator *iter)
{
  tree type = TREE_TYPE (elem);

  if (BOUNDED_TYPE_P (type))
    {
      if (!all_bounds[offs / POINTER_SIZE])
	{
	  tree temp = make_temp_ssa_name (type, gimple_build_nop (), "");
	  gimple assign = gimple_build_assign (temp, elem);
	  gimple_stmt_iterator gsi;

	  gsi_insert_before (iter, assign, GSI_SAME_STMT);
	  gsi = gsi_for_stmt (assign);

	  all_bounds[offs / POINTER_SIZE] = chkp_find_bounds (temp, &gsi);
	}
    }
  else if (RECORD_OR_UNION_TYPE_P (type))
    {
      tree field;

      for (field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
	if (TREE_CODE (field) == FIELD_DECL)
	  {
	    tree base = chkp_can_be_shared (elem)
	      ? elem
	      : unshare_expr (elem);
	    tree field_ref = chkp_build_component_ref (base, field);
	    HOST_WIDE_INT field_offs
	      = TREE_INT_CST_LOW (DECL_FIELD_BIT_OFFSET (field));
	    if (DECL_FIELD_OFFSET (field))
	      field_offs += TREE_INT_CST_LOW (DECL_FIELD_OFFSET (field)) * 8;

	    chkp_find_bounds_for_elem (field_ref, all_bounds,
				       offs + field_offs, iter);
	  }
    }
  else if (TREE_CODE (type) == ARRAY_TYPE)
    {
      tree maxval = TYPE_MAX_VALUE (TYPE_DOMAIN (type));
      tree etype = TREE_TYPE (type);
      HOST_WIDE_INT esize = TREE_INT_CST_LOW (TYPE_SIZE (etype));
      unsigned HOST_WIDE_INT cur;

      if (!maxval || integer_minus_onep (maxval))
	return;

      for (cur = 0; cur <= TREE_INT_CST_LOW (maxval); cur++)
	{
	  tree base = chkp_can_be_shared (elem)
	    ? elem
	    : unshare_expr (elem);
	  tree arr_elem = chkp_build_array_ref (base, etype,
						TYPE_SIZE (etype),
						cur);
	  chkp_find_bounds_for_elem (arr_elem, all_bounds, offs + cur * esize,
				     iter);
	}
    }
}

/* Fill HAVE_BOUND output bitmap with information about
   bounds requred for object of type TYPE.

   OFFS is used for recursive calls and holds basic
   offset of TYPE in outer structure in bits.

   HAVE_BOUND[i] is set to 1 if there is a field
   in TYPE which has pointer type and offset
   equal to i * POINTER_SIZE - OFFS in bits.  */
void
chkp_find_bound_slots_1 (const_tree type, bitmap have_bound,
			 HOST_WIDE_INT offs)
{
  if (BOUNDED_TYPE_P (type))
    bitmap_set_bit (have_bound, offs / POINTER_SIZE);
  else if (RECORD_OR_UNION_TYPE_P (type))
    {
      tree field;

      for (field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
	if (TREE_CODE (field) == FIELD_DECL)
	  {
	    HOST_WIDE_INT field_offs
	      = TREE_INT_CST_LOW (DECL_FIELD_BIT_OFFSET (field));
	    if (DECL_FIELD_OFFSET (field))
	      field_offs += TREE_INT_CST_LOW (DECL_FIELD_OFFSET (field)) * 8;
	    chkp_find_bound_slots_1 (TREE_TYPE (field), have_bound,
				     offs + field_offs);
	  }
    }
  else if (TREE_CODE (type) == ARRAY_TYPE)
    {
      tree maxval = TYPE_MAX_VALUE (TYPE_DOMAIN (type));
      tree etype = TREE_TYPE (type);
      HOST_WIDE_INT esize = TREE_INT_CST_LOW (TYPE_SIZE (etype));
      unsigned HOST_WIDE_INT cur;

      if (!maxval || integer_minus_onep (maxval))
	return;

      for (cur = 0; cur <= TREE_INT_CST_LOW (maxval); cur++)
	chkp_find_bound_slots_1 (etype, have_bound, offs + cur * esize);
    }
}

/* Return bitmap holding information about bounds for
   type TYPE.  See chkp_find_bound_slots_1 for more
   details.

   Caller is responsible for deallocation of returned
   bitmap.  */
bitmap
chkp_find_bound_slots (const_tree type)
{
  bitmap res = BITMAP_ALLOC (NULL);
  chkp_find_bound_slots_1 (type, res, 0);
  return res;
}

/* Add bound arguments to call statement pointed by GSI.
   Also performs a replacement of user checker builtins calls
   with internal ones.  */

static void
chkp_add_bounds_to_call_stmt (gimple_stmt_iterator *gsi)
{
  gimple call = gsi_stmt (*gsi);
  unsigned arg_no = 0;
  tree fndecl = gimple_call_fndecl (call);
  tree fntype;
  tree first_formal_arg;
  tree arg;
  bool use_fntype = false;
  tree op;
  ssa_op_iter iter;
  gimple new_call;

  /* Do nothing for internal functions.  */
  if (gimple_call_internal_p (call))
    return;

  fntype = TREE_TYPE (TREE_TYPE (gimple_call_fn (call)));

  /* Do nothing if back-end builtin is called.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_MD)
    return;

  /* Donothing for calls to legacy functions.  */
  if (fndecl
      && lookup_attribute ("bnd_legacy", DECL_ATTRIBUTES (fndecl)))
    return;

  /* Ignore CHKP_INIT_PTR_BOUNDS, CHKP_NULL_PTR_BOUNDS
     and CHKP_COPY_PTR_BOUNDS.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_INIT_PTR_BOUNDS
	  || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_NULL_PTR_BOUNDS
	  || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_COPY_PTR_BOUNDS
	  || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_SET_PTR_BOUNDS))
    return;

  /* Check user builtins are replaced with checks.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_CHECK_PTR_LBOUNDS
	  || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_CHECK_PTR_UBOUNDS
	  || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_CHECK_PTR_BOUNDS))
    {
      chkp_replace_address_check_builtin (gsi, integer_minus_one_node);
      return;
    }

  /* Check user builtins are replaced with bound extract.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_GET_PTR_LBOUND
	  || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_GET_PTR_UBOUND))
    {
      chkp_replace_extract_builtin (gsi);
      return;
    }

  /* BUILT_IN_CHKP_NARROW_PTR_BOUNDS call is replaced with
     target narrow bounds call.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_NARROW_PTR_BOUNDS)
    {
      tree arg = gimple_call_arg (call, 1);
      tree bounds = chkp_find_bounds (arg, gsi);

      gimple_call_set_fndecl (call, chkp_narrow_bounds_fndecl);
      gimple_call_set_arg (call, 1, bounds);
      update_stmt (call);

      return;
    }

  /* BUILT_IN_CHKP_STORE_PTR_BOUNDS call is replaced with
     bndstx call.  */
  if (fndecl && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_STORE_PTR_BOUNDS)
    {
      tree addr = gimple_call_arg (call, 0);
      tree ptr = gimple_call_arg (call, 1);
      tree bounds = chkp_find_bounds (ptr, gsi);
      gimple_stmt_iterator iter = gsi_for_stmt (call);

      chkp_build_bndstx (addr, ptr, bounds, gsi);
      gsi_remove (&iter, true);

      return;
    }

  if (!flag_chkp_instrument_calls)
    return;

  /* If function decl is available then use it for
     formal arguments list.  Otherwise use function type.  */
  if (fndecl && DECL_ARGUMENTS (fndecl))
    first_formal_arg = DECL_ARGUMENTS (fndecl);
  else
    {
      first_formal_arg = TYPE_ARG_TYPES (fntype);
      use_fntype = true;
    }

  /* Fill vector of new call args.  */
  vec<tree> new_args = vNULL;
  new_args.create (gimple_call_num_args (call));
  arg = first_formal_arg;
  for (arg_no = 0; arg_no < gimple_call_num_args (call); arg_no++)
    {
      tree call_arg = gimple_call_arg (call, arg_no);
      tree type;

      /* Get arg type using formal argument description
	 or actual argument type.  */
      if (arg)
	if (use_fntype)
	  if (TREE_VALUE (arg) != void_type_node)
	    {
	      type = TREE_VALUE (arg);
	      arg = TREE_CHAIN (arg);
	    }
	  else
	    type = TREE_TYPE (call_arg);
	else
	  {
	    type = TREE_TYPE (arg);
	    arg = TREE_CHAIN (arg);
	  }
      else
	type = TREE_TYPE (call_arg);

      new_args.safe_push (call_arg);

      if (BOUNDED_TYPE_P (type)
	  || pass_by_reference (NULL, TYPE_MODE (type), type, true))
	new_args.safe_push (chkp_find_bounds (call_arg, gsi));
      else if (chkp_type_has_pointer (type))
	{
	  HOST_WIDE_INT max_bounds
	    = TREE_INT_CST_LOW (TYPE_SIZE (type)) / POINTER_SIZE;
	  tree *all_bounds = (tree *)xmalloc (sizeof (tree) * max_bounds);
	  HOST_WIDE_INT bnd_no;

	  memset (all_bounds, 0, sizeof (tree) * max_bounds);

	  chkp_find_bounds_for_elem (call_arg, all_bounds, 0, gsi);

	  for (bnd_no = 0; bnd_no < max_bounds; bnd_no++)
	    if (all_bounds[bnd_no])
	      new_args.safe_push (all_bounds[bnd_no]);

           free (all_bounds);
	}
    }

  if (new_args.length () == gimple_call_num_args (call))
    new_call = call;
  else
    {
      new_call = gimple_build_call_vec (gimple_op (call, 1), new_args);
      gimple_call_set_lhs (new_call, gimple_call_lhs (call));
      gimple_call_copy_flags (new_call, call);
    }
  new_args.release ();

  /* If we call built-in function and pass no bounds then
     we do not need to change anything.  */
  if (new_call == call
      && fndecl
      && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && called_as_built_in (fndecl))
      return;

  /* For direct calls fndecl is replaced with instrumented version.  */
  if (fndecl)
    {
      tree new_decl = chkp_maybe_create_clone (fndecl)->decl;
      gimple_call_set_fndecl (new_call, new_decl);
      gimple_call_set_fntype (new_call, TREE_TYPE (new_decl));
    }
  /* For indirect call we should fix function pointer type if
     pass some bounds.  */
  else if (new_call != call)
    {
      tree type = gimple_call_fntype (call);
      type = chkp_copy_function_type_adding_bounds (type);
      gimple_call_set_fntype (new_call, type);
    }

  /* replace old call statement with the new one.  */
  if (call != new_call)
    {
      FOR_EACH_SSA_TREE_OPERAND (op, call, iter, SSA_OP_ALL_DEFS)
	{
	  SSA_NAME_DEF_STMT (op) = new_call;
	}
      gsi_replace (gsi, new_call, true);
    }
  else
    update_stmt (new_call);

  gimple_call_set_with_bounds (new_call, true);
}

/* Return entry block to be used for checker initilization code.
   Create new block if required.  */
static basic_block
chkp_get_entry_block (void)
{
  if (!entry_block)
    entry_block = split_block (ENTRY_BLOCK_PTR_FOR_FN (cfun), NULL)->dest;

  return entry_block;
}

/* Return constant static bounds var with specified LB and UB
   if such var exists in varpool.  Return NULL otherwise.  */
static tree
chkp_find_const_bounds_var (HOST_WIDE_INT lb,
			    HOST_WIDE_INT ub)
{
  tree val = targetm.chkp_make_bounds_constant (lb, ub);
  struct varpool_node *node;

  /* We expect bounds constant is represented as a complex value
     of two pointer sized integers.  */
  gcc_assert (TREE_CODE (val) == COMPLEX_CST);

  FOR_EACH_VARIABLE (node)
    if (POINTER_BOUNDS_P (node->decl)
	&& TREE_READONLY (node->decl)
	&& DECL_INITIAL (node->decl)
	&& TREE_CODE (DECL_INITIAL (node->decl)) == COMPLEX_CST
	&& tree_int_cst_equal (TREE_REALPART (DECL_INITIAL (node->decl)),
			       TREE_REALPART (val))
	&& tree_int_cst_equal (TREE_IMAGPART (DECL_INITIAL (node->decl)),
			       TREE_IMAGPART (val)))
      return node->decl;

  return NULL;
}

/* Return constant static bounds var with specified bounds LB and UB.
   If such var does not exists then new var is created with specified NAME.  */
static tree
chkp_make_static_const_bounds (HOST_WIDE_INT lb,
			       HOST_WIDE_INT ub,
			       const char *name)
{
  tree var;

  /* With LTO we may have constant bounds already in varpool.
     Try to find it.  */
  var = chkp_find_const_bounds_var (lb, ub);

  if (var)
    return var;

  var  = build_decl (UNKNOWN_LOCATION, VAR_DECL,
		     get_identifier (name), pointer_bounds_type_node);

  TREE_PUBLIC (var) = 1;
  TREE_USED (var) = 1;
  TREE_READONLY (var) = 1;
  TREE_STATIC (var) = 1;
  TREE_ADDRESSABLE (var) = 0;
  DECL_ARTIFICIAL (var) = 1;
  DECL_COMDAT (var) = 1;
  DECL_COMDAT_GROUP (var) = DECL_ASSEMBLER_NAME (var);
  DECL_READ_P (var) = 1;
  DECL_INITIAL (var) = targetm.chkp_make_bounds_constant (lb, ub);
  /* We may use this symbol during ctors generation in chkp_finish_file
     when all symbols are emitted.  Force output to avoid undefined
     symbols in ctors.  */
  varpool_node_for_decl (var)->force_output = 1;
  varpool_finalize_decl (var);

  return var;
}

/* Generate code to make bounds with specified lower bound LB and SIZE.
   if AFTER is 1 then code is inserted after position pointed by ITER
   otherwise code is inserted before position pointed by ITER.
   If ITER is NULL then code is added to entry block.  */
static tree
chkp_make_bounds (tree lb, tree size, gimple_stmt_iterator *iter, bool after)
{
  gimple_seq seq;
  gimple_stmt_iterator gsi;
  gimple stmt;
  tree bounds;

  if (iter)
    gsi = *iter;
  else
    gsi = gsi_start_bb (chkp_get_entry_block ());

  seq = NULL;

  lb = chkp_force_gimple_call_op (lb, &seq);
  size = chkp_force_gimple_call_op (size, &seq);

  stmt = gimple_build_call (chkp_bndmk_fndecl, 2, lb, size);
  chkp_mark_stmt (stmt);

  bounds = chkp_get_tmp_reg (stmt);
  gimple_call_set_lhs (stmt, bounds);

  gimple_seq_add_stmt (&seq, stmt);

  if (iter && after)
    gsi_insert_seq_after (&gsi, seq, GSI_SAME_STMT);
  else
    gsi_insert_seq_before (&gsi, seq, GSI_SAME_STMT);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Made bounds: ");
      print_gimple_stmt (dump_file, stmt, 0, TDF_VOPS|TDF_MEMSYMS);
      if (iter)
	{
	  fprintf (dump_file, "  inserted before statement: ");
	  print_gimple_stmt (dump_file, gsi_stmt (*iter), 0, TDF_VOPS|TDF_MEMSYMS);
	}
      else
	fprintf (dump_file, "  at function entry\n");
    }

  /* update_stmt (stmt); */

  return bounds;
}

/* Return var holding zero bounds.  */
tree
chkp_get_zero_bounds_var (void)
{
  if (!chkp_zero_bounds_var)
    chkp_zero_bounds_var
      = chkp_make_static_const_bounds (0, -1,
				       CHKP_ZERO_BOUNDS_VAR_NAME);
  return chkp_zero_bounds_var;
}

/* Return var holding none bounds.  */
static tree
chkp_get_none_bounds_var (void)
{
  if (!chkp_none_bounds_var)
    chkp_none_bounds_var
      = chkp_make_static_const_bounds (-1, 0,
				       CHKP_NONE_BOUNDS_VAR_NAME);
  return chkp_none_bounds_var;
}

/* Return SSA_NAME used to represent zero bounds.  */
static tree
chkp_get_zero_bounds (void)
{
  if (zero_bounds)
    return zero_bounds;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Creating zero bounds...");

  if ((flag_chkp_use_static_bounds && flag_chkp_use_static_const_bounds)
      || flag_chkp_use_static_const_bounds > 0)
    {
      gimple_stmt_iterator gsi = gsi_start_bb (chkp_get_entry_block ());
      gimple stmt;

      zero_bounds = chkp_get_tmp_reg (gimple_build_nop ());
      stmt = gimple_build_assign (zero_bounds, chkp_get_zero_bounds_var ());
      gsi_insert_before (&gsi, stmt, GSI_SAME_STMT);
    }
  else
    zero_bounds = chkp_make_bounds (integer_zero_node,
				    integer_zero_node,
				    NULL,
				    false);

  return zero_bounds;
}

/* Return SSA_NAME used to represent none bounds.  */
static tree
chkp_get_none_bounds (void)
{
  if (none_bounds)
    return none_bounds;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Creating none bounds...");


  if ((flag_chkp_use_static_bounds && flag_chkp_use_static_const_bounds)
      || flag_chkp_use_static_const_bounds > 0)
    {
      gimple_stmt_iterator gsi = gsi_start_bb (chkp_get_entry_block ());
      gimple stmt;

      none_bounds = chkp_get_tmp_reg (gimple_build_nop ());
      stmt = gimple_build_assign (none_bounds, chkp_get_none_bounds_var ());
      gsi_insert_before (&gsi, stmt, GSI_SAME_STMT);
    }
  else
    none_bounds = chkp_make_bounds (integer_minus_one_node,
				    build_int_cst (size_type_node, 2),
				    NULL,
				    false);

  return none_bounds;
}

/* Return bounds to be used as a result of operation which
   should not create poiunter (e.g. MULT_EXPR).  */
static tree
chkp_get_invalid_op_bounds (void)
{
  return chkp_get_zero_bounds ();
}

/* Return bounds to be used for loads of non-pointer values.  */
static tree
chkp_get_nonpointer_load_bounds (void)
{
  return chkp_get_zero_bounds ();
}

/* Build bounds returned by CALL.  */
static tree
chkp_build_returned_bound (gimple call)
{
  gimple_stmt_iterator gsi;
  tree bounds;
  gimple stmt;
  tree fndecl = gimple_call_fndecl (call);

  /* To avoid fixing alloca expands in targets we handle
     it separately.  */
  if (fndecl
      && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_ALLOCA
	  || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_ALLOCA_WITH_ALIGN))
    {
      tree size = gimple_call_arg (call, 0);
      tree lb = gimple_call_lhs (call);
      gimple_stmt_iterator iter = gsi_for_stmt (call);
      bounds = chkp_make_bounds (lb, size, &iter, true);
    }
  /* We know bounds returned by set_bounds builtin call.  */
  else if (fndecl
	   && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
	   && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_SET_PTR_BOUNDS)
    {
      tree lb = gimple_call_arg (call, 0);
      tree size = gimple_call_arg (call, 1);
      gimple_stmt_iterator iter = gsi_for_stmt (call);
      bounds = chkp_make_bounds (lb, size, &iter, true);
    }
  /* Similarly handle next_arg builtin.  */
  else if (fndecl
	   && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
	   && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_NEXT_ARG)
    {
      tree size = targetm.fn_abi_va_list_bounds_size (cfun->decl);
      if (size == integer_zero_node)
	bounds = chkp_get_zero_bounds ();
      else
	{
	  tree lb = gimple_call_lhs (call);
	  gimple_stmt_iterator iter = gsi_for_stmt (call);
	  bounds = chkp_make_bounds (lb, size, &iter, true);
	}
    }
  /* Detect bounds initialization calls.  */
  else if (fndecl
      && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_INIT_PTR_BOUNDS)
    bounds = chkp_get_zero_bounds ();
  /* Detect bounds nullification calls.  */
  else if (fndecl
      && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_NULL_PTR_BOUNDS)
    bounds = chkp_get_none_bounds ();
  /* Detect bounds copy calls.  */
  else if (fndecl
      && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
      && DECL_FUNCTION_CODE (fndecl) == BUILT_IN_CHKP_COPY_PTR_BOUNDS)
    {
      gimple_stmt_iterator iter = gsi_for_stmt (call);
      bounds = chkp_find_bounds (gimple_call_arg (call, 1), &iter);
    }
  else
    {
      gcc_assert (TREE_CODE (gimple_call_lhs (call)) == SSA_NAME);

      /* In general case build checker builtin call to
	 obtain returned bounds.  */
      stmt = gimple_build_call (chkp_ret_bnd_fndecl, 1,
				gimple_call_lhs (call));
      chkp_mark_stmt (stmt);

      gsi = gsi_for_stmt (call);
      gsi_insert_after (&gsi, stmt, GSI_SAME_STMT);

      bounds = chkp_get_tmp_reg (stmt);
      gimple_call_set_lhs (stmt, bounds);

      update_stmt (stmt);
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Built returned bounds (");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, ") for call: ");
      print_gimple_stmt (dump_file, call, 0, TDF_VOPS|TDF_MEMSYMS);
    }

  chkp_register_bounds (gimple_call_lhs (call), bounds);

  return bounds;
}

/* Return bounds used as returned by call
   which produced SSA name VAL.  */
gimple
chkp_retbnd_call_by_val (tree val)
{
  if (TREE_CODE (val) != SSA_NAME)
    return NULL;

  gcc_assert (gimple_code (SSA_NAME_DEF_STMT (val)) == GIMPLE_CALL);

  imm_use_iterator use_iter;
  use_operand_p use_p;
  FOR_EACH_IMM_USE_FAST (use_p, use_iter, val)
    if (gimple_code (USE_STMT (use_p)) == GIMPLE_CALL
	&& gimple_call_fndecl (USE_STMT (use_p)) == chkp_ret_bnd_fndecl)
      return USE_STMT (use_p);

  return NULL;
}

/* Check the next parameter for the given PARM is bounds
   and return it's default SSA_NAME (create if required).  */
static tree
chkp_get_next_bounds_parm (tree parm)
{
  tree bounds = TREE_CHAIN (parm);
  gcc_assert (POINTER_BOUNDS_P (bounds));
  bounds = ssa_default_def (cfun, bounds);	
  if (!bounds)
    {
      bounds = make_ssa_name (TREE_CHAIN (parm), gimple_build_nop ());
      set_ssa_default_def (cfun, TREE_CHAIN (parm), bounds);
    }
  return bounds;
}

/* Return bounds to be used for input argument PARM.  */
static tree
chkp_get_bound_for_parm (tree parm)
{
  tree decl = SSA_NAME_VAR (parm);
  tree bounds;

  gcc_assert (TREE_CODE (decl) == PARM_DECL);

  bounds = chkp_get_registered_bounds (parm);

  if (!bounds)
    bounds = chkp_get_registered_bounds (decl);

  if (!bounds)
    {
      tree orig_decl = cgraph_get_node (cfun->decl)->orig_decl;

      /* For static chain param we return zero bounds
	 because currently we do not check dereferences
	 of this pointer.  */
      /* ?? Is it a correct way to identify such parm?  */
      if (cfun->decl && DECL_STATIC_CHAIN (cfun->decl)
	  && DECL_ARTIFICIAL (decl))
	bounds = chkp_get_zero_bounds ();
      /* If non instrumented runtime is used then it may be useful
	 to use zero bounds for input arguments of main
	 function.  */
      else if (flag_chkp_zero_input_bounds_for_main
	       && strcmp (IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (orig_decl)),
			  "main") == 0)
	bounds = chkp_get_zero_bounds ();
      else if (BOUNDED_P (parm))
	{
	  bounds = chkp_get_next_bounds_parm (decl);
	  chkp_register_bounds (decl, bounds);

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Built arg bounds (");
	      print_generic_expr (dump_file, bounds, 0);
	      fprintf (dump_file, ") for arg: ");
	      print_node (dump_file, "", decl, 0);
	    }
	}
      else
	bounds = chkp_get_zero_bounds ();
    }

  if (!chkp_get_registered_bounds (parm))
    chkp_register_bounds (parm, bounds);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Using bounds ");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, " for parm ");
      print_generic_expr (dump_file, parm, 0);
      fprintf (dump_file, " of type ");
      print_generic_expr (dump_file, TREE_TYPE (parm), 0);
      fprintf (dump_file, ".\n");
    }

  return bounds;
}

/* Build and return CALL_EXPR for bndstx builtin with specified
   arguments.  */
tree
chkp_build_bndldx_call (tree addr, tree ptr)
{
  tree fn = build1 (ADDR_EXPR,
		    build_pointer_type (TREE_TYPE (chkp_bndldx_fndecl)),
		    chkp_bndldx_fndecl);
  tree call = build_call_nary (TREE_TYPE (TREE_TYPE (chkp_bndldx_fndecl)),
			       fn, 2, addr, ptr);
  CALL_WITH_BOUNDS_P (call) = true;
  return call;
}

/* Insert code to load bounds for PTR located by ADDR.
   Code is inserted after position pointed by GSI.
   Loaded bounds are returned.  */
static tree
chkp_build_bndldx (tree addr, tree ptr, gimple_stmt_iterator *gsi)
{
  gimple_seq seq;
  gimple stmt;
  tree bounds;

  seq = NULL;

  addr = chkp_force_gimple_call_op (addr, &seq);
  ptr = chkp_force_gimple_call_op (ptr, &seq);

  stmt = gimple_build_call (chkp_bndldx_fndecl, 2, addr, ptr);
  chkp_mark_stmt (stmt);
  bounds = chkp_get_tmp_reg (stmt);
  gimple_call_set_lhs (stmt, bounds);

  gimple_seq_add_stmt (&seq, stmt);

  gsi_insert_seq_after (gsi, seq, GSI_CONTINUE_LINKING);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Generated bndldx for pointer ");
      print_generic_expr (dump_file, ptr, 0);
      fprintf (dump_file, ": ");
      print_gimple_stmt (dump_file, stmt, 0, TDF_VOPS|TDF_MEMSYMS);
    }

  return bounds;
}

/* Build and return CALL_EXPR for bndstx builtin with specified
   arguments.  */
tree
chkp_build_bndstx_call (tree addr, tree ptr, tree bounds)
{
  tree fn = build1 (ADDR_EXPR,
		    build_pointer_type (TREE_TYPE (chkp_bndstx_fndecl)),
		    chkp_bndstx_fndecl);
  tree call = build_call_nary (TREE_TYPE (TREE_TYPE (chkp_bndstx_fndecl)),
			       fn, 3, ptr, bounds, addr);
  CALL_WITH_BOUNDS_P (call) = true;
  return call;
}

/* Insert code to store BOUNDS for PTR stored by ADDR.
   New statements are inserted after position pointed
   by GSI.  */
void
chkp_build_bndstx (tree addr, tree ptr, tree bounds,
		   gimple_stmt_iterator *gsi)
{
  gimple_seq seq;
  gimple stmt;

  seq = NULL;

  addr = chkp_force_gimple_call_op (addr, &seq);
  ptr = chkp_force_gimple_call_op (ptr, &seq);

  stmt = gimple_build_call (chkp_bndstx_fndecl, 3, ptr, bounds, addr);
  chkp_mark_stmt (stmt);
  gimple_call_set_with_bounds (stmt, true);

  gimple_seq_add_stmt (&seq, stmt);

  gsi_insert_seq_after (gsi, seq, GSI_CONTINUE_LINKING);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Generated bndstx for pointer store ");
      print_gimple_stmt (dump_file, gsi_stmt (*gsi), 0, TDF_VOPS|TDF_MEMSYMS);
      print_gimple_stmt (dump_file, stmt, 2, TDF_VOPS|TDF_MEMSYMS);
    }
}

/* Compute bounds for pointer NODE which was assigned in
   assignment statement ASSIGN.  Return computed bounds.  */
static tree
chkp_compute_bounds_for_assignment (tree node, gimple assign)
{
  enum tree_code rhs_code = gimple_assign_rhs_code (assign);
  tree rhs1 = gimple_assign_rhs1 (assign);
  tree bounds = NULL_TREE;
  gimple_stmt_iterator iter = gsi_for_stmt (assign);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Computing bounds for assignment: ");
      print_gimple_stmt (dump_file, assign, 0, TDF_VOPS|TDF_MEMSYMS);
    }

  switch (rhs_code)
    {
    case MEM_REF:
    case TARGET_MEM_REF:
    case COMPONENT_REF:
    case ARRAY_REF:
      /* We need to load bounds from the bounds table.  */
      bounds = chkp_find_bounds_loaded (node, rhs1, &iter);
      break;

    case VAR_DECL:
    case SSA_NAME:
    case ADDR_EXPR:
    case POINTER_PLUS_EXPR:
    case NOP_EXPR:
    case CONVERT_EXPR:
    case INTEGER_CST:
      /* Bounds are just propagated from RHS.  */
      bounds = chkp_find_bounds (rhs1, &iter);
      break;

    case VIEW_CONVERT_EXPR:
      /* Bounds are just propagated from RHS.  */
      bounds = chkp_find_bounds (TREE_OPERAND (rhs1, 0), &iter);
      break;

    case PARM_DECL:
      if (BOUNDED_P (rhs1))
	{
	  /* We need to load bounds from the bounds table.  */
	  bounds = chkp_build_bndldx (chkp_build_addr_expr (rhs1),
				      node, &iter);
	  TREE_ADDRESSABLE (rhs1) = 1;
	}
      else
	bounds = chkp_get_nonpointer_load_bounds ();
      break;

    case MINUS_EXPR:
    case PLUS_EXPR:
    case BIT_AND_EXPR:
    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
      {
	tree rhs2 = gimple_assign_rhs2 (assign);
	tree bnd1 = chkp_find_bounds (rhs1, &iter);
	tree bnd2 = chkp_find_bounds (rhs2, &iter);

	/* First we try to check types of operands.  If it
	   does not help then look at bound values.

	   If some bounds are incomplete and other are
	   not proven to be valid (i.e. also incomplete
	   or invalid because value is not pointer) then
	   resulting value is incomplete and will be
	   recomputed later in chkp_finish_incomplete_bounds.  */
	if (BOUNDED_P (rhs1)
	    && !BOUNDED_P (rhs2))
	  bounds = bnd1;
	else if (BOUNDED_P (rhs2)
		 && !BOUNDED_P (rhs1)
		 && rhs_code != MINUS_EXPR)
	  bounds = bnd2;
	else if (chkp_incomplete_bounds (bnd1))
	  if (chkp_valid_bounds (bnd2) && rhs_code != MINUS_EXPR
	      && !chkp_incomplete_bounds (bnd2))
	    bounds = bnd2;
	  else
	    bounds = incomplete_bounds;
	else if (chkp_incomplete_bounds (bnd2))
	  if (chkp_valid_bounds (bnd1)
	      && !chkp_incomplete_bounds (bnd1))
	    bounds = bnd1;
	  else
	    bounds = incomplete_bounds;
	else if (!chkp_valid_bounds (bnd1))
	  if (chkp_valid_bounds (bnd2) && rhs_code != MINUS_EXPR)
	    bounds = bnd2;
	  else if (bnd2 == chkp_get_zero_bounds ())
	    bounds = bnd2;
	  else
	    bounds = bnd1;
	else if (!chkp_valid_bounds (bnd2))
	  bounds = bnd1;
	else
	  /* Seems both operands may have valid bounds
	     (e.g. pointer minus pointer).  In such case
	     use default invalid op bounds.  */
	  bounds = chkp_get_invalid_op_bounds ();
      }
      break;

    case BIT_NOT_EXPR:
    case NEGATE_EXPR:
    case LSHIFT_EXPR:
    case RSHIFT_EXPR:
    case LROTATE_EXPR:
    case RROTATE_EXPR:
    case EQ_EXPR:
    case NE_EXPR:
    case LT_EXPR:
    case LE_EXPR:
    case GT_EXPR:
    case GE_EXPR:
    case MULT_EXPR:
    case RDIV_EXPR:
    case TRUNC_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case CEIL_DIV_EXPR:
    case ROUND_DIV_EXPR:
    case TRUNC_MOD_EXPR:
    case FLOOR_MOD_EXPR:
    case CEIL_MOD_EXPR:
    case ROUND_MOD_EXPR:
    case EXACT_DIV_EXPR:
    case FIX_TRUNC_EXPR:
    case FLOAT_EXPR:
    case REALPART_EXPR:
    case IMAGPART_EXPR:
      /* No valid bounds may be produced by these exprs.  */
      bounds = chkp_get_invalid_op_bounds ();
      break;

    case COND_EXPR:
      {
	tree val1 = gimple_assign_rhs2 (assign);
	tree val2 = gimple_assign_rhs3 (assign);
	tree bnd1 = chkp_find_bounds (val1, &iter);
	tree bnd2 = chkp_find_bounds (val2, &iter);
	gimple stmt;

	if (chkp_incomplete_bounds (bnd1) || chkp_incomplete_bounds (bnd2))
	  bounds = incomplete_bounds;
	else if (bnd1 == bnd2)
	  bounds = bnd1;
	else
	  {
	    if (!chkp_can_be_shared (rhs1))
	      rhs1 = unshare_expr (rhs1);

	    bounds = chkp_get_tmp_reg (assign);
	    stmt = gimple_build_assign_with_ops (COND_EXPR, bounds,
						  rhs1, bnd1, bnd2);
	    gsi_insert_after (&iter, stmt, GSI_SAME_STMT);

	    if (!chkp_valid_bounds (bnd1) && !chkp_valid_bounds (bnd2))
	      chkp_mark_invalid_bounds (bounds);
	  }
      }
      break;

    case MAX_EXPR:
    case MIN_EXPR:
      {
	tree rhs2 = gimple_assign_rhs2 (assign);
	tree bnd1 = chkp_find_bounds (rhs1, &iter);
	tree bnd2 = chkp_find_bounds (rhs2, &iter);

	if (chkp_incomplete_bounds (bnd1) || chkp_incomplete_bounds (bnd2))
	  bounds = incomplete_bounds;
	else if (bnd1 == bnd2)
	  bounds = bnd1;
	else
	  {
	    gimple stmt;
	    tree cond = build2 (rhs_code == MAX_EXPR ? GT_EXPR : LT_EXPR,
				boolean_type_node, rhs1, rhs2);
	    bounds = chkp_get_tmp_reg (assign);
	    stmt = gimple_build_assign_with_ops (COND_EXPR, bounds,
						  cond, bnd1, bnd2);

	    gsi_insert_after (&iter, stmt, GSI_SAME_STMT);

	    if (!chkp_valid_bounds (bnd1) && !chkp_valid_bounds (bnd2))
	      chkp_mark_invalid_bounds (bounds);
	  }
      }
      break;

    default:
      internal_error ("chkp_compute_bounds_for_assignment: "
		      "Unexpected RHS code %s",
		      get_tree_code_name (rhs_code));
    }

  gcc_assert (bounds);

  if (node)
    chkp_register_bounds (node, bounds);

  return bounds;
}

/* Compute bounds for ssa name NODE defined by DEF_STMT pointed by ITER.

   There are just few statement codes allowed: NOP (for default ssa names),
   ASSIGN, CALL, PHI, ASM.

   Return computed bounds.  */
static tree
chkp_get_bounds_by_definition (tree node, gimple def_stmt,
			       gimple_stmt_iterator *iter)
{
  tree var, bounds;
  enum gimple_code code = gimple_code (def_stmt);
  gimple stmt;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Searching for bounds for node: ");
      print_generic_expr (dump_file, node, 0);

      fprintf (dump_file, " using its definition: ");
      print_gimple_stmt (dump_file, def_stmt, 0, TDF_VOPS|TDF_MEMSYMS);
    }

  switch (code)
    {
    case GIMPLE_NOP:
      var = SSA_NAME_VAR (node);
      switch (TREE_CODE (var))
	{
	case PARM_DECL:
	  bounds = chkp_get_bound_for_parm (node);
	  break;

	case VAR_DECL:
	  /* For uninitialized pointers use none bounds.  */
	  bounds = chkp_get_none_bounds ();
	  chkp_register_bounds (node, bounds);
	  break;

	case RESULT_DECL:
	  {
	    tree base_type;

	    gcc_assert (TREE_CODE (TREE_TYPE (node)) == REFERENCE_TYPE);

	    base_type = TREE_TYPE (TREE_TYPE (node));

	    gcc_assert (TYPE_SIZE (base_type)
			&& TREE_CODE (TYPE_SIZE (base_type)) == INTEGER_CST
			&& tree_to_uhwi (TYPE_SIZE (base_type)) != 0);

	    bounds = chkp_make_bounds (node, TYPE_SIZE_UNIT (base_type),
				       NULL, false);
	    chkp_register_bounds (node, bounds);
	  }
	  break;

	default:
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Unexpected var with no definition\n");
	      print_generic_expr (dump_file, var, 0);
	    }
	  internal_error ("chkp_get_bounds_by_definition: Unexpected var of type %s",
			  get_tree_code_name (TREE_CODE (var)));
	}
      break;

    case GIMPLE_ASSIGN:
      bounds = chkp_compute_bounds_for_assignment (node, def_stmt);
      break;

    case GIMPLE_CALL:
      bounds = chkp_build_returned_bound (def_stmt);
      break;

    case GIMPLE_PHI:
      stmt = create_phi_node (chkp_get_tmp_var (), gimple_bb (def_stmt));
      bounds = gimple_phi_result (stmt);
      *iter = gsi_for_stmt (stmt);

      chkp_register_bounds (node, bounds);

      /* Created bounds do not have all phi args computed and
	 therefore we do not know if there is a valid source
	 of bounds for that node.  Therefore we mark bounds
	 as incomplete and then recompute them when all phi
	 args are computed.  */
      chkp_register_incomplete_bounds (bounds, node);
      break;

    case GIMPLE_ASM:
      bounds = chkp_get_zero_bounds ();
      chkp_register_bounds (node, bounds);
      break;

    default:
      internal_error ("chkp_get_bounds_by_definition: Unexpected GIMPLE code %s",
		      gimple_code_name[code]);
    }

  return bounds;
}

/* Return CALL_EXPR for bndmk with specified LOWER_BOUND and SIZE.  */
tree
chkp_build_make_bounds_call (tree lower_bound, tree size)
{
  tree call = build1 (ADDR_EXPR,
		      build_pointer_type (TREE_TYPE (chkp_bndmk_fndecl)),
		      chkp_bndmk_fndecl);
  return build_call_nary (TREE_TYPE (TREE_TYPE (chkp_bndmk_fndecl)),
			  call, 2, lower_bound, size);
}

/* Create static bounds var of specfified OBJ which is
   is either VAR_DECL or string constant.  */
static tree
chkp_make_static_bounds (tree obj)
{
  static int string_id = 1;
  static int var_id = 1;
  tree *slot;
  const char *var_name;
  char *bnd_var_name;
  tree bnd_var;

  /* First check if we already have required var.  */
  if (chkp_static_var_bounds)
    {
      slot = (tree *)pointer_map_contains (chkp_static_var_bounds, obj);
      if (slot)
	return *slot;
    }

  /* Build decl for bounds var.  */
  if (TREE_CODE (obj) == VAR_DECL)
    {
      if (DECL_IGNORED_P (obj))
	{
	  bnd_var_name = (char *) xmalloc (strlen (CHKP_VAR_BOUNDS_PREFIX) + 10);
	  sprintf (bnd_var_name, "%s%d", CHKP_VAR_BOUNDS_PREFIX, var_id++);
	}
      else
	{
	  var_name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (obj));

	  /* For hidden symbols we want to skip first '*' char.  */
	  if (*var_name == '*')
	    var_name++;

	  bnd_var_name = (char *) xmalloc (strlen (var_name)
					   + strlen (CHKP_BOUNDS_OF_SYMBOL_PREFIX) + 1);
	  strcpy (bnd_var_name, CHKP_BOUNDS_OF_SYMBOL_PREFIX);
	  strcat (bnd_var_name, var_name);
	}

      bnd_var = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			    get_identifier (bnd_var_name),
			    pointer_bounds_type_node);

      /* Address of the obj will be used as lower bound.  */
      TREE_ADDRESSABLE (obj) = 1;
    }
  else
    {
      bnd_var_name = (char *) xmalloc (strlen (CHKP_STRING_BOUNDS_PREFIX) + 10);
      sprintf (bnd_var_name, "%s%d", CHKP_STRING_BOUNDS_PREFIX, string_id++);

      bnd_var = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			    get_identifier (bnd_var_name),
			    pointer_bounds_type_node);
    }

  TREE_PUBLIC (bnd_var) = 0;
  TREE_USED (bnd_var) = 1;
  TREE_READONLY (bnd_var) = 0;
  TREE_STATIC (bnd_var) = 1;
  TREE_ADDRESSABLE (bnd_var) = 0;
  DECL_ARTIFICIAL (bnd_var) = 1;
  DECL_COMMON (bnd_var) = 1;
  DECL_COMDAT (bnd_var) = 1;
  DECL_READ_P (bnd_var) = 1;
  DECL_INITIAL (bnd_var) = chkp_build_addr_expr (obj);
  /* Force output similar to constant bounds.
     See chkp_make_static_const_bounds. */
  varpool_node_for_decl (bnd_var)->force_output = 1;
  /* Mark symbol as requiring bounds initialization.  */
  varpool_node_for_decl (bnd_var)->need_bounds_init = 1;
  varpool_finalize_decl (bnd_var);

  /* Add created var to the map to use it for other references
     to obj.  */
  if (!chkp_static_var_bounds)
    chkp_static_var_bounds = pointer_map_create ();

  slot = (tree *)pointer_map_insert (chkp_static_var_bounds, obj);
  *slot = bnd_var;

  return bnd_var;
}

static tree chkp_get_var_size_decl (tree var) ATTRIBUTE_UNUSED;

/* Return var holding size relocation for given VAR.  */
static tree
chkp_get_var_size_decl (tree var)
{
  struct tree_map **slot, *map;
  const char *var_name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (var));
  const char *size_suffix = "@SIZE";
  char *decl_name;
  char *size_name;
  tree size_decl, size_reloc;

  /* For hidden symbols we want to skip first '*' char.  */
  if (*var_name == '*')
    var_name++;

  /* Check if we have decl already.  */
  if (chkp_size_decls)
    {
      struct tree_map *res, in;
      in.base.from = var;
      in.hash = htab_hash_pointer (var);

      res = (struct tree_map *) htab_find_with_hash (chkp_size_decls,
						     &in, in.hash);

      if (res)
	return res->to;
    }

  /* No prepared decl was found.  Create new decl for var size.  */
  size_name = (char *) xmalloc (strlen (var_name) + strlen (size_suffix) + 1);
  strcpy (size_name, var_name);
  strcat (size_name, size_suffix);

  size_reloc = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			   get_identifier (size_name), chkp_uintptr_type);

  TREE_PUBLIC (size_reloc) = 1;
  TREE_USED (size_reloc) = 1;
  DECL_ARTIFICIAL (size_reloc) = 1;
  DECL_EXTERNAL (size_reloc) = 1;
  DECL_COMMON (size_reloc) = 1;

  decl_name = (char *) xmalloc (strlen (var_name)
			       + strlen (CHKP_SIZE_OF_SYMBOL_PREFIX) + 1);
  strcpy (decl_name, CHKP_SIZE_OF_SYMBOL_PREFIX);
  strcat (decl_name, var_name);

  size_decl = build_decl (UNKNOWN_LOCATION, VAR_DECL,
			 get_identifier (decl_name), chkp_uintptr_type);

  TREE_PUBLIC (size_decl) = 0;
  TREE_USED (size_decl) = 1;
  TREE_READONLY (size_decl) = 1;
  TREE_STATIC (size_decl) = 1;
  TREE_ADDRESSABLE (size_decl) = 1;
  DECL_ARTIFICIAL (size_decl) = 1;
  DECL_COMMON (size_decl) = 1;
  DECL_COMDAT (size_decl) = 1;
  DECL_READ_P (size_decl) = 1;
  DECL_INITIAL (size_decl) = chkp_build_addr_expr (size_reloc);
  /* Force output similar to constant bounds.
     See chkp_make_static_const_bounds. */
  varpool_node_for_decl (size_decl)->force_output = 1;
  varpool_finalize_decl (size_decl);

  free (size_name);
  free (decl_name);

  /* Add created decl to the global hash map.  */
  if (!chkp_size_decls)
    chkp_size_decls = htab_create_ggc (31, tree_map_hash, tree_map_eq, NULL);

  map = ggc_alloc_tree_map ();
  map->hash = htab_hash_pointer (var);
  map->base.from = var;
  map->to = size_decl;

  slot = (struct tree_map **)
    htab_find_slot_with_hash (chkp_size_decls, map, map->hash, INSERT);
  *slot = map;

  return size_decl;
}

/* When var has incomplete type we cannot get size to
   compute its bounds.  In such cases we use checker
   builtin call which determines object size at runtime.  */
static tree
chkp_generate_extern_var_bounds (tree var)
{
  tree bounds, size_reloc, lb, size, max_size, cond;
  gimple_stmt_iterator gsi;
  gimple_seq seq = NULL;
  gimple stmt;

  /* If instrumentation is not enabled for vars having
     incomplete type then just return zero bounds to avoid
     checks for this var.  */
  if (!flag_chkp_incomplete_type)
    return chkp_get_zero_bounds ();

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Generating bounds for extern symbol '");
      print_generic_expr (dump_file, var, 0);
      fprintf (dump_file, "'\n");
    }

  stmt = gimple_build_call (chkp_sizeof_fndecl, 1, var);

  size_reloc = create_tmp_reg (chkp_uintptr_type, CHKP_SIZE_TMP_NAME);
  gimple_call_set_lhs (stmt, size_reloc);

  gimple_seq_add_stmt (&seq, stmt);

  lb = chkp_build_addr_expr (var);
  size = make_ssa_name (chkp_get_size_tmp_var (), gimple_build_nop ());

  if (flag_chkp_zero_dynamic_size_as_infinite)
    {
      /* We should check that size relocation was resolved.
	 If it was not then use maximum possible size for the var.  */
      max_size = build2 (MINUS_EXPR, chkp_uintptr_type, integer_zero_node,
			 fold_convert (chkp_uintptr_type, lb));
      max_size = chkp_force_gimple_call_op (max_size, &seq);

      cond = build2 (NE_EXPR, boolean_type_node, size_reloc, integer_zero_node);
      stmt = gimple_build_assign_with_ops (COND_EXPR, size,
					   cond, size_reloc, max_size);
      gimple_seq_add_stmt (&seq, stmt);
    }
  else
    {
      stmt = gimple_build_assign (size, size_reloc);
      gimple_seq_add_stmt (&seq, stmt);
    }

  gsi = gsi_start_bb (chkp_get_entry_block ());
  gsi_insert_seq_after (&gsi, seq, GSI_CONTINUE_LINKING);

  bounds = chkp_make_bounds (lb, size, &gsi, true);

  return bounds;
}

/* Return 1 if TYPE has fields with zero size or fields
   marked with chkp_variable_size attribute.  */
bool
chkp_variable_size_type (tree type)
{
  bool res = false;
  tree field;

  if (RECORD_OR_UNION_TYPE_P (type))
    for (field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
      {
	if (TREE_CODE (field) == FIELD_DECL)
	  res = res
	    || lookup_attribute ("bnd_variable_size", DECL_ATTRIBUTES (field))
	    || chkp_variable_size_type (TREE_TYPE (field));
      }
  else
    res = !TYPE_SIZE (type)
      || TREE_CODE (TYPE_SIZE (type)) != INTEGER_CST
      || tree_to_uhwi (TYPE_SIZE (type)) == 0;

  return res;
}

/* Compute and return bounds for address of DECL which is
   one of VAR_DECL, PARM_DECL, RESULT_DECL.  */
static tree
chkp_get_bounds_for_decl_addr (tree decl)
{
  tree bounds;

  gcc_assert (TREE_CODE (decl) == VAR_DECL
	      || TREE_CODE (decl) == PARM_DECL
	      || TREE_CODE (decl) == RESULT_DECL);

  bounds = chkp_get_registered_addr_bounds (decl);

  if (bounds)
    return bounds;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Building bounds for address of decl ");
      print_generic_expr (dump_file, decl, 0);
      fprintf (dump_file, "\n");
    }

  /* Use zero bounds if size is unknown and checks for
     unknown sizes are restricted.  */
  if ((!DECL_SIZE (decl)
       || (chkp_variable_size_type (TREE_TYPE (decl))
	   && (TREE_STATIC (decl)
	       || DECL_EXTERNAL (decl)
	       || TREE_PUBLIC (decl))))
      && !flag_chkp_incomplete_type)
      return chkp_get_zero_bounds ();

  if (flag_chkp_use_static_bounds
      && TREE_CODE (decl) == VAR_DECL
      && (TREE_STATIC (decl)
	      || DECL_EXTERNAL (decl)
	      || TREE_PUBLIC (decl))
      && !DECL_THREAD_LOCAL_P (decl))
    {
      tree bnd_var = chkp_make_static_bounds (decl);
      gimple_stmt_iterator gsi = gsi_start_bb (chkp_get_entry_block ());
      gimple stmt;

      bounds = chkp_get_tmp_reg (gimple_build_nop ());
      stmt = gimple_build_assign (bounds, bnd_var);
      gsi_insert_before (&gsi, stmt, GSI_SAME_STMT);
    }
  else if (!DECL_SIZE (decl)
      || (chkp_variable_size_type (TREE_TYPE (decl))
	  && (TREE_STATIC (decl)
	      || DECL_EXTERNAL (decl)
	      || TREE_PUBLIC (decl))))
    {
      gcc_assert (TREE_CODE (decl) == VAR_DECL);
      bounds = chkp_generate_extern_var_bounds (decl);
    }
  else
    {
      tree lb = chkp_build_addr_expr (decl);
      bounds = chkp_make_bounds (lb, DECL_SIZE_UNIT (decl), NULL, false);
    }

  return bounds;
}

/* Compute and return bounds for constant string.  */
static tree
chkp_get_bounds_for_string_cst (tree cst)
{
  tree bounds;
  tree lb;
  tree size;

  gcc_assert (TREE_CODE (cst) == STRING_CST);

  bounds = chkp_get_registered_bounds (cst);

  if (bounds)
    return bounds;

  if ((flag_chkp_use_static_bounds && flag_chkp_use_static_const_bounds)
      || flag_chkp_use_static_const_bounds > 0)
    {
      tree bnd_var = chkp_make_static_bounds (cst);
      gimple_stmt_iterator gsi = gsi_start_bb (chkp_get_entry_block ());
      gimple stmt;

      bounds = chkp_get_tmp_reg (gimple_build_nop ());
      stmt = gimple_build_assign (bounds, bnd_var);
      gsi_insert_before (&gsi, stmt, GSI_SAME_STMT);
    }
  else
    {
      lb = chkp_build_addr_expr (cst);
      size = build_int_cst (chkp_uintptr_type, TREE_STRING_LENGTH (cst));
      bounds = chkp_make_bounds (lb, size, NULL, false);
    }

  chkp_register_bounds (cst, bounds);

  return bounds;
}

/* Generate code to instersect bounds BOUNDS1 and BOUNDS2 and
   return the result.  if ITER is not NULL then Code is inserted
   before position pointed by ITER.  Otherwise code is added to
   entry block.  */
static tree
chkp_intersect_bounds (tree bounds1, tree bounds2, gimple_stmt_iterator *iter)
{
  if (!bounds1 || bounds1 == chkp_get_zero_bounds ())
    return bounds2 ? bounds2 : bounds1;
  else if (!bounds2 || bounds2 == chkp_get_zero_bounds ())
    return bounds1;
  else
    {
      gimple_seq seq;
      gimple stmt;
      tree bounds;

      seq = NULL;

      stmt = gimple_build_call (chkp_intersect_fndecl, 2, bounds1, bounds2);
      chkp_mark_stmt (stmt);

      bounds = chkp_get_tmp_reg (stmt);
      gimple_call_set_lhs (stmt, bounds);

      gimple_seq_add_stmt (&seq, stmt);

      /* We are probably doing narrowing for constant expression.
	 In such case iter may be undefined.  */
      if (!iter)
	{
	  gimple_stmt_iterator gsi = gsi_last_bb (chkp_get_entry_block ());
	  iter = &gsi;
	  gsi_insert_seq_after (iter, seq, GSI_SAME_STMT);
	}
      else
	gsi_insert_seq_before (iter, seq, GSI_SAME_STMT);

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Bounds intersection: ");
	  print_gimple_stmt (dump_file, stmt, 0, TDF_VOPS|TDF_MEMSYMS);
	  fprintf (dump_file, "  inserted before statement: ");
	  print_gimple_stmt (dump_file, gsi_stmt (*iter), 0,
			     TDF_VOPS|TDF_MEMSYMS);
	}

      return bounds;
    }
}

/* Return 1 if we are allowed to narrow bounds for addressed FIELD
   and 0 othersize.  */
static bool
chkp_may_narrow_to_field (tree field)
{
  return DECL_SIZE (field) && TREE_CODE (DECL_SIZE (field)) == INTEGER_CST
    && tree_to_uhwi (DECL_SIZE (field)) != 0
    && (!DECL_FIELD_OFFSET (field)
	|| TREE_CODE (DECL_FIELD_OFFSET (field)) == INTEGER_CST)
    && (!DECL_FIELD_BIT_OFFSET (field)
	|| TREE_CODE (DECL_FIELD_BIT_OFFSET (field)) == INTEGER_CST)
    && !lookup_attribute ("bnd_variable_size", DECL_ATTRIBUTES (field))
    && !chkp_variable_size_type (TREE_TYPE (field));
}

/* Return 1 if bounds for FIELD should be narrowed to
   field's own size.  */
static bool
chkp_narrow_bounds_for_field (tree field)
{
  HOST_WIDE_INT offs;
  HOST_WIDE_INT bit_offs;

  if (!chkp_may_narrow_to_field (field))
    return false;

  /* Accesse to compiler generated fields should not cause
     bounds narrowing.  */
  if (DECL_ARTIFICIAL (field))
    return false;

  offs = tree_to_uhwi (DECL_FIELD_OFFSET (field));
  bit_offs = tree_to_uhwi (DECL_FIELD_BIT_OFFSET (field));

  return (flag_chkp_narrow_bounds
	  && (flag_chkp_first_field_has_own_bounds
	      || offs
	      || bit_offs));
}

/* Perform narrowing for BOUNDS using bounds computed for field
   access COMPONENT.  ITER meaning is the same as for
   chkp_intersect_bounds.  */
static tree
chkp_narrow_bounds_to_field (tree bounds, tree component,
			    gimple_stmt_iterator *iter)
{
  tree field = TREE_OPERAND (component, 1);
  tree size = DECL_SIZE_UNIT (field);
  tree field_ptr = chkp_build_addr_expr (component);
  tree field_bounds;

  field_bounds = chkp_make_bounds (field_ptr, size, iter, false);

  return chkp_intersect_bounds (field_bounds, bounds, iter);
}

/* Parse field or array access NODE.

   PTR ouput parameter holds a pointer to the outermost
   object.

   BITFIELD output parameter is set to 1 if bitfield is
   accessed and to 0 otherwise.  If it is 1 then ELT holds
   outer component for accessed bit field.

   SAFE outer parameter is set to 1 if access is safe and
   checks are not required.

   BOUNDS outer parameter holds bounds to be used to check
   access (may be NULL).

   If INNERMOST_BOUNDS is 1 then try to narrow bounds to the
   innermost accessed component.  */
static void
chkp_parse_array_and_component_ref (tree node, tree *ptr,
				    tree *elt, bool *safe,
				    bool *bitfield,
				    tree *bounds,
				    gimple_stmt_iterator *iter,
				    bool innermost_bounds)
{
  tree comp_to_narrow = NULL_TREE;
  tree last_comp = NULL_TREE;
  bool array_ref_found = false;
  tree *nodes;
  tree var;
  int len;
  int i;

  /* Compute tree height for expression.  */
  var = node;
  len = 1;
  while (TREE_CODE (var) == COMPONENT_REF
	 || TREE_CODE (var) == ARRAY_REF
	 || TREE_CODE (var) == VIEW_CONVERT_EXPR)
    {
      var = TREE_OPERAND (var, 0);
      len++;
    }

  gcc_assert (len > 1);

  /* It is more convenient for us to scan left-to-right,
     so walk tree again and put all node to nodes vector
     in reversed order.  */
  nodes = XALLOCAVEC (tree, len);
  nodes[len - 1] = node;
  for (i = len - 2; i >= 0; i--)
    nodes[i] = TREE_OPERAND (nodes[i + 1], 0);

  if (bounds)
    *bounds = NULL;
  *safe = true;
  *bitfield = (TREE_CODE (node) == COMPONENT_REF
	       && DECL_BIT_FIELD_TYPE (TREE_OPERAND (node, 1)));
  /* To get bitfield address we will need outer elemnt.  */
  if (*bitfield)
    *elt = nodes[len - 2];
  else
    *elt = NULL_TREE;

  /* If we have indirection in expression then compute
     outermost structure bounds.  Computed bounds may be
     narrowed later.  */
  if (TREE_CODE (nodes[0]) == MEM_REF || INDIRECT_REF_P (nodes[0]))
    {
      *safe = false;
      *ptr = TREE_OPERAND (nodes[0], 0);
      if (bounds)
	*bounds = chkp_find_bounds (*ptr, iter);
    }
  else
    {
      gcc_assert (TREE_CODE (var) == VAR_DECL
		  || TREE_CODE (var) == PARM_DECL
		  || TREE_CODE (var) == RESULT_DECL
		  || TREE_CODE (var) == STRING_CST
		  || TREE_CODE (var) == SSA_NAME);

      *ptr = chkp_build_addr_expr (var);
    }

  /* In this loop we are trying to find a field access
     requiring narrowing.  There are two simple rules
     for search:
     1.  Leftmost array_ref is chosen if any.
     2.  Rightmost suitable component_ref is chosen if innermost
     bounds are required and no array_ref exists.  */
  for (i = 1; i < len; i++)
    {
      var = nodes[i];

      if (TREE_CODE (var) == ARRAY_REF)
	{
	  *safe = false;
	  array_ref_found = true;
	  if (flag_chkp_narrow_bounds
	      && !flag_chkp_narrow_to_innermost_arrray
	      && (!last_comp
		  || chkp_may_narrow_to_field (TREE_OPERAND (last_comp, 1))))
	    {
	      comp_to_narrow = last_comp;
	      break;
	    }
	}
      else if (TREE_CODE (var) == COMPONENT_REF)
	{
	  tree field = TREE_OPERAND (var, 1);

	  if (innermost_bounds
	      && !array_ref_found
	      && chkp_narrow_bounds_for_field (field))
	    comp_to_narrow = var;
	  last_comp = var;

	  if (flag_chkp_narrow_bounds
	      && flag_chkp_narrow_to_innermost_arrray
	      && TREE_CODE (TREE_TYPE (field)) == ARRAY_TYPE)
	    {
	      if (bounds)
		*bounds = chkp_narrow_bounds_to_field (*bounds, var, iter);
	      comp_to_narrow = NULL;
	    }
	}
      else if (TREE_CODE (var) == VIEW_CONVERT_EXPR)
	/* Nothing to do for it.  */
	;
      else
	gcc_unreachable ();
    }

  if (comp_to_narrow && DECL_SIZE (TREE_OPERAND (comp_to_narrow, 1)) && bounds)
    *bounds = chkp_narrow_bounds_to_field (*bounds, comp_to_narrow, iter);

  if (innermost_bounds && bounds && !*bounds)
    *bounds = chkp_find_bounds (*ptr, iter);
}

/* Compute and returne bounds for address of OBJ.  */
static tree
chkp_make_addressed_object_bounds (tree obj, gimple_stmt_iterator *iter)
{
  tree bounds = chkp_get_registered_addr_bounds (obj);

  if (bounds)
    return bounds;

  switch (TREE_CODE (obj))
    {
    case VAR_DECL:
    case PARM_DECL:
    case RESULT_DECL:
      bounds = chkp_get_bounds_for_decl_addr (obj);
      break;

    case STRING_CST:
      bounds = chkp_get_bounds_for_string_cst (obj);
      break;

    case ARRAY_REF:
    case COMPONENT_REF:
      {
	tree elt;
	tree ptr;
	bool safe;
	bool bitfield;

	chkp_parse_array_and_component_ref (obj, &ptr, &elt, &safe,
					    &bitfield, &bounds, iter, true);

	gcc_assert (bounds);
      }
      break;

    case FUNCTION_DECL:
    case LABEL_DECL:
      bounds = chkp_get_zero_bounds ();
      break;

    case MEM_REF:
      bounds = chkp_find_bounds (TREE_OPERAND (obj, 0), iter);
      break;

    case REALPART_EXPR:
    case IMAGPART_EXPR:
      bounds = chkp_make_addressed_object_bounds (TREE_OPERAND (obj, 0), iter);
      break;

    default:
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "chkp_make_addressed_object_bounds: "
		   "unexpected object of type %s\n",
		   get_tree_code_name (TREE_CODE (obj)));
	  print_node (dump_file, "", obj, 0);
	}
      internal_error ("chkp_make_addressed_object_bounds: "
		      "Unexpected tree code %s",
		      get_tree_code_name (TREE_CODE (obj)));
    }

  chkp_register_addr_bounds (obj, bounds);

  return bounds;
}

/* Compute bounds for pointer PTR loaded from PTR_SRC.  Generate statements
   to compute bounds if required.  Computed bounds should be available at
   position pointed by ITER.

   If PTR_SRC is NULL_TREE then pointer definition is identified.

   If PTR_SRC is not NULL_TREE then ITER points to statements which loads
   PTR.  If PTR is a any memory reference then ITER points to a statement
   after which bndldx will be inserterd.  In both cases ITER will be updated
   to point to the inserted bndldx statement.  */

static tree
chkp_find_bounds_1 (tree ptr, tree ptr_src, gimple_stmt_iterator *iter)
{
  tree addr = NULL_TREE;
  tree bounds = NULL_TREE;

  if (!ptr_src)
    ptr_src = ptr;

  bounds = chkp_get_registered_bounds (ptr_src);

  if (bounds)
    return bounds;

  switch (TREE_CODE (ptr_src))
    {
    case MEM_REF:
    case VAR_DECL:
      if (BOUNDED_P (ptr_src))
	if (TREE_CODE (ptr) == VAR_DECL && DECL_REGISTER (ptr))
	  bounds = chkp_get_zero_bounds ();
	else
	  {
	    addr = chkp_build_addr_expr (ptr_src);
	    bounds = chkp_build_bndldx (addr, ptr, iter);
	  }
      else
	bounds = chkp_get_nonpointer_load_bounds ();
      break;

    case ARRAY_REF:
    case COMPONENT_REF:
      addr = get_base_address (ptr_src);
      if (DECL_P (addr)
	  || TREE_CODE (addr) == MEM_REF
	  || TREE_CODE (addr) == TARGET_MEM_REF)
	{
	  if (BOUNDED_P (ptr_src))
	    if (TREE_CODE (ptr) == VAR_DECL && DECL_REGISTER (ptr))
	      bounds = chkp_get_zero_bounds ();
	    else
	      {
		addr = chkp_build_addr_expr (ptr_src);
		bounds = chkp_build_bndldx (addr, ptr, iter);
	      }
	  else
	    bounds = chkp_get_nonpointer_load_bounds ();
	}
      else
	{
	  gcc_assert (TREE_CODE (addr) == SSA_NAME);
	  bounds = chkp_find_bounds (addr, iter);
	}
      break;

    case PARM_DECL:
      gcc_unreachable ();
      bounds = chkp_get_bound_for_parm (ptr_src);
      break;

    case TARGET_MEM_REF:
      addr = chkp_build_addr_expr (ptr_src);
      bounds = chkp_build_bndldx (addr, ptr, iter);
      break;

    case SSA_NAME:
      bounds = chkp_get_registered_bounds (ptr_src);
      if (!bounds)
	{
	  gimple def_stmt = SSA_NAME_DEF_STMT (ptr_src);
	  gimple_stmt_iterator phi_iter;

	  bounds = chkp_get_bounds_by_definition (ptr_src, def_stmt, &phi_iter);

	  gcc_assert (bounds);

	  if (gimple_code (def_stmt) == GIMPLE_PHI)
	    {
	      unsigned i;

	      for (i = 0; i < gimple_phi_num_args (def_stmt); i++)
		{
		  tree arg = gimple_phi_arg_def (def_stmt, i);
		  tree arg_bnd;
		  gimple phi_bnd;

		  arg_bnd = chkp_find_bounds (arg, NULL);

		  /* chkp_get_bounds_by_definition created new phi
		     statement and phi_iter points to it.

		     Previous call to chkp_find_bounds could create
		     new basic block and therefore change phi statement
		     phi_iter points to.  */
		  phi_bnd = gsi_stmt (phi_iter);

		  add_phi_arg (phi_bnd, arg_bnd,
			       gimple_phi_arg_edge (def_stmt, i),
			       UNKNOWN_LOCATION);
		}

	      /* If all bound phi nodes have their arg computed
		 then we may finish its computation.  See
		 chkp_finish_incomplete_bounds for more details.  */
	      if (chkp_may_finish_incomplete_bounds ())
		chkp_finish_incomplete_bounds ();
	    }

	  gcc_assert (bounds == chkp_get_registered_bounds (ptr_src)
		      || chkp_incomplete_bounds (bounds));
	}
      break;

    case ADDR_EXPR:
      bounds = chkp_make_addressed_object_bounds (TREE_OPERAND (ptr_src, 0), iter);
      break;

    case INTEGER_CST:
      if (integer_zerop (ptr_src))
	bounds = chkp_get_none_bounds ();
      else
	bounds = chkp_get_invalid_op_bounds ();
      break;

    default:
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "chkp_find_bounds: unexpected ptr of type %s\n",
		   get_tree_code_name (TREE_CODE (ptr_src)));
	  print_node (dump_file, "", ptr_src, 0);
	}
      internal_error ("chkp_find_bounds: Unexpected tree code %s",
		      get_tree_code_name (TREE_CODE (ptr_src)));
    }

  if (!bounds)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (stderr, "chkp_find_bounds: cannot find bounds for pointer\n");
	  print_node (dump_file, "", ptr_src, 0);
	}
      internal_error ("chkp_find_bounds: Cannot find bounds for pointer");
    }

  return bounds;
}

/* Normal case for bounds search without forced narrowing.  */
static tree
chkp_find_bounds (tree ptr, gimple_stmt_iterator *iter)
{
  return chkp_find_bounds_1 (ptr, NULL_TREE, iter);
}

/* Search bounds for pointer PTR loaded from PTR_SRC
   by statement *ITER points to.  */
static tree
chkp_find_bounds_loaded (tree ptr, tree ptr_src, gimple_stmt_iterator *iter)
{
  return chkp_find_bounds_1 (ptr, ptr_src, iter);
}

/* Search for bounds for PTR to be used in abnormal PHI node.
   Similar to regular bounds search but create bound copy to
   be used over abnormal edge.  */
static tree
chkp_find_bounds_abnormal (tree ptr, tree phi, edge e)
{
  tree bounds = chkp_find_bounds_1 (ptr, NULL_TREE, NULL);
  tree copy = NULL;
  gimple assign;
  gimple_stmt_iterator gsi;
  struct tree_vec_map *found, in;
  vec<tree, va_gc> **copies = NULL;
  unsigned int i;

  if (gimple_code (SSA_NAME_DEF_STMT (bounds)) == GIMPLE_PHI
      && bounds == phi)
    return bounds;

  /* Check for existing bound copies created for specified
     PHI bounds.  */
  in.base.from = phi;
  found = (struct tree_vec_map *)
    htab_find_with_hash (chkp_abnormal_phi_copies, &in,
			 htab_hash_pointer (phi));

  if (found)
    {
      copies = &found->to;
      for (i = 0; i < (*copies)->length (); i++)
	{
	  tree ssa = (**copies)[i];
	  gimple def = SSA_NAME_DEF_STMT (ssa);
	  if (gimple_assign_rhs1 (def) == bounds
	      && gimple_bb (def) == e->src)
	    {
	      copy = ssa;
	      break;
	    }
	}
    }

  /* If copy was not found then create it and store into
     vector of copies for PHI.  */
  if (!copy)
    {
      copy = chkp_get_tmp_reg (gimple_build_nop ());
      assign = gimple_build_assign (copy, bounds);

      gsi = gsi_start_bb (e->src);
      while (!gsi_end_p (gsi) && !stmt_ends_bb_p (gsi_stmt (gsi)))
	gsi_next (&gsi);

      if (!gsi_end_p (gsi) && stmt_ends_bb_p (gsi_stmt (gsi)))
	gsi_insert_before (&gsi, assign, GSI_SAME_STMT);
      else
	gsi_insert_after (&gsi, assign, GSI_SAME_STMT);

      if (!copies)
	{
	  void **loc;

	  found = ggc_alloc_tree_vec_map ();
	  found->base.from = phi;
	  found->to = NULL;
	  loc = htab_find_slot_with_hash (chkp_abnormal_phi_copies, found,
					  htab_hash_pointer (phi),
					  INSERT);
	  *(struct tree_vec_map **) loc = found;

	  copies = &found->to;
	}

      vec_safe_push (*copies, copy);
    }

  /* After bounds are replaced with their copy in abnormal PHI,
     we need to recompute usage in abnormal phi flag.
     Current copy creation algorithm allows original bounds usage
     in abnormal phi only if it is a result of this phi.  */
  SSA_NAME_OCCURS_IN_ABNORMAL_PHI (bounds) = 0;
  if (gimple_code (SSA_NAME_DEF_STMT (bounds)) == GIMPLE_PHI)
    {
      gimple def = SSA_NAME_DEF_STMT (bounds);
      for (i = 0; i < gimple_phi_num_args (def); i++)
	{
	  tree arg = gimple_phi_arg_def (def, i);
	  edge e = gimple_phi_arg_edge (def, i);
	  if ((e->flags & EDGE_ABNORMAL)
	      && arg == bounds)
	    {
	      SSA_NAME_OCCURS_IN_ABNORMAL_PHI (bounds) = 1;
	      break;
	    }
	}
    }

  return copy;
}

/* Helper function which checks type of RHS and finds all pointers in
   it.  For each found pointer we build it's accesses in LHS and RHS
   objects and then call HANDLER for them.  Function is used to copy
   or initilize bounds for copied object.  */
static void
chkp_walk_pointer_assignments (tree lhs, tree rhs, void *arg,
			       assign_handler handler)
{
  tree type = TREE_TYPE (lhs);

  /* We have nothing to do with clobbers.  */
  if (TREE_CLOBBER_P (rhs))
    return;

  if (BOUNDED_TYPE_P (type))
    handler (lhs, rhs, arg);
  else if (RECORD_OR_UNION_TYPE_P (type))
    {
      tree field;

      if (TREE_CODE (rhs) == CONSTRUCTOR)
	{
	  unsigned HOST_WIDE_INT cnt;
	  tree val;

	  FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (rhs), cnt, field, val)
	    {
	      if (chkp_type_has_pointer (TREE_TYPE (field)))
		{
		  tree lhs_field = chkp_build_component_ref (lhs, field);
		  chkp_walk_pointer_assignments (lhs_field, val, arg, handler);
		}
	    }
	}
      else
	for (field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
	  if (TREE_CODE (field) == FIELD_DECL
	      && chkp_type_has_pointer (TREE_TYPE (field)))
	    {
	      tree rhs_field = chkp_build_component_ref (rhs, field);
	      tree lhs_field = chkp_build_component_ref (lhs, field);
	      chkp_walk_pointer_assignments (lhs_field, rhs_field, arg, handler);
	    }
    }
  else if (TREE_CODE (type) == ARRAY_TYPE)
    {
      unsigned HOST_WIDE_INT cur = 0;
      tree maxval = TYPE_MAX_VALUE (TYPE_DOMAIN (type));
      tree etype = TREE_TYPE (type);
      tree esize = TYPE_SIZE (etype);

      if (TREE_CODE (rhs) == CONSTRUCTOR)
	{
	  unsigned HOST_WIDE_INT cnt;
	  tree purp, val, lhs_elem;

	  FOR_EACH_CONSTRUCTOR_ELT (CONSTRUCTOR_ELTS (rhs), cnt, purp, val)
	    {
	      if (purp && TREE_CODE (purp) == RANGE_EXPR)
		{
		  tree lo_index = TREE_OPERAND (purp, 0);
		  tree hi_index = TREE_OPERAND (purp, 1);

		  for (cur = (unsigned)tree_to_uhwi (lo_index);
		       cur <= (unsigned)tree_to_uhwi (hi_index);
		       cur++)
		    {
		      lhs_elem = chkp_build_array_ref (lhs, etype, esize, cur);
		      chkp_walk_pointer_assignments (lhs_elem, val, arg, handler);
		    }
		}
	      else
		{
		  if (purp)
		    {
		      gcc_assert (TREE_CODE (purp) == INTEGER_CST);
		      cur = tree_to_uhwi (purp);
		    }

		  lhs_elem = chkp_build_array_ref (lhs, etype, esize, cur++);

		  chkp_walk_pointer_assignments (lhs_elem, val, arg, handler);
		}
	    }
	}
      /* Copy array only when size is known.  */
      else if (maxval && !integer_minus_onep (maxval))
	for (cur = 0; cur <= TREE_INT_CST_LOW (maxval); cur++)
	  {
	    tree lhs_elem = chkp_build_array_ref (lhs, etype, esize, cur);
	    tree rhs_elem = chkp_build_array_ref (rhs, etype, esize, cur);
	    chkp_walk_pointer_assignments (lhs_elem, rhs_elem, arg, handler);
	  }
    }
  else
    internal_error("chkp_walk_pointer_assignments: unexpected RHS type: %s",
		   get_tree_code_name (TREE_CODE (type)));
}

/* Add code to copy bounds for assignment of RHS to LHS.
   ARG is an iterator pointing ne code position.  */
static void
chkp_copy_bounds_for_elem (tree lhs, tree rhs, void *arg)
{
  gimple_stmt_iterator *iter = (gimple_stmt_iterator *)arg;
  tree bounds = chkp_find_bounds (rhs, iter);
  tree addr = chkp_build_addr_expr(lhs);

  chkp_build_bndstx (addr, rhs, bounds, iter);
}

/* Emit static bound initilizers and size vars.  */
void
chkp_finish_file (void)
{
  struct varpool_node *node;
  struct chkp_ctor_stmt_list stmts;

  if (seen_error ())
    return;

  /* Iterate through varpool and generate bounds initialization
     constructors for all statically initialized pointers.  */
  stmts.avail = MAX_STMTS_IN_STATIC_CHKP_CTOR;
  stmts.stmts = NULL;
  FOR_EACH_VARIABLE (node)
    /* Check that var is actually emitted and we need and may initialize
       its bounds.  */
    if (node->need_bounds_init
	&& !POINTER_BOUNDS_P (node->decl)
	&& DECL_RTL (node->decl)
	&& MEM_P (DECL_RTL (node->decl))
	&& TREE_ASM_WRITTEN (node->decl))
      {
	chkp_walk_pointer_assignments (node->decl,
				       DECL_INITIAL (node->decl),
				       &stmts,
				       chkp_add_modification_to_stmt_list);

	if (stmts.avail <= 0)
	  {
	    cgraph_build_static_cdtor ('P', stmts.stmts,
				       MAX_RESERVED_INIT_PRIORITY + 3);
	    stmts.avail = MAX_STMTS_IN_STATIC_CHKP_CTOR;
	    stmts.stmts = NULL;
	  }
      }

  if (stmts.stmts)
    cgraph_build_static_cdtor ('P', stmts.stmts,
			       MAX_RESERVED_INIT_PRIORITY + 3);

  /* Iterate through varpool and generate bounds initialization
     constructors for all static bounds vars.  */
  stmts.avail = MAX_STMTS_IN_STATIC_CHKP_CTOR;
  stmts.stmts = NULL;
  FOR_EACH_VARIABLE (node)
    if (node->need_bounds_init
	&& POINTER_BOUNDS_P (node->decl)
	&& TREE_ASM_WRITTEN (node->decl))
      {
	tree bnd = node->decl;
	tree var;

	gcc_assert (DECL_INITIAL (bnd)
		    && TREE_CODE (DECL_INITIAL (bnd)) == ADDR_EXPR);

	var = TREE_OPERAND (DECL_INITIAL (bnd), 0);
	chkp_output_static_bounds (bnd, var, &stmts);
      }

  if (stmts.stmts)
    cgraph_build_static_cdtor ('B', stmts.stmts,
			       MAX_RESERVED_INIT_PRIORITY + 2);

  if (chkp_size_decls)
    htab_delete (chkp_size_decls);
  if (chkp_static_var_bounds)
    pointer_map_destroy (chkp_static_var_bounds);
  if (chkp_bounds_map)
    pointer_map_destroy (chkp_bounds_map);
}

/* An instrumentation function which is called for each statement
   having memory access we want to instrument.  It inserts check
   code and bounds copy code.

   ITER points to statement to instrument.

   NODE holds memory access in statement to check.

   LOC holds the location information for statement.

   DIRFLAGS determines whether access is read or write.

   ACCESS_OFFS should be added to address used in NODE
   before check.

   ACCESS_SIZE holds size of checked access.

   SAFE indicates if NODE access is safe and should not be
   checked.  */
static void
chkp_process_stmt (gimple_stmt_iterator *iter, tree node,
		   location_t loc, tree dirflag,
		   tree access_offs, tree access_size,
		   bool safe)
{
  tree node_type = TREE_TYPE (node);
  tree size = access_size ? access_size : TYPE_SIZE_UNIT (node_type);
  tree addr_first = NULL_TREE; /* address of the first accessed byte */
  tree addr_last = NULL_TREE; /* address of the last accessed byte */
  tree ptr = NULL_TREE; /* a pointer used for dereference */
  tree bounds = NULL_TREE;

  /* We do not need instrumentation for clobbers.  */
  if (dirflag == integer_one_node
      && gimple_code (gsi_stmt (*iter)) == GIMPLE_ASSIGN
      && TREE_CLOBBER_P (gimple_assign_rhs1 (gsi_stmt (*iter))))
    return;

  switch (TREE_CODE (node))
    {
    case ARRAY_REF:
    case COMPONENT_REF:
      {
	bool bitfield;
	tree elt;

	if (safe)
	  {
	    /* We are not going to generate any checks, so do not
	       generate bounds as well.  */
	    addr_first = chkp_build_addr_expr (node);
	    break;
	  }

	chkp_parse_array_and_component_ref (node, &ptr, &elt, &safe,
					    &bitfield, &bounds, iter, false);

	/* Break if there is no dereference and operation is safe.  */

	if (bitfield)
          {
            tree field = TREE_OPERAND (node, 1);

            if (TREE_CODE (DECL_SIZE_UNIT (field)) == INTEGER_CST)
              size = DECL_SIZE_UNIT (field);

	    if (elt)
	      elt = chkp_build_addr_expr (elt);
            addr_first = fold_convert_loc (loc, ptr_type_node, elt ? elt : ptr);
            addr_first = fold_build_pointer_plus_loc (loc,
						      addr_first,
						      byte_position (field));
          }
        else
          addr_first = chkp_build_addr_expr (node);
      }
      break;

    case INDIRECT_REF:
      ptr = TREE_OPERAND (node, 0);
      addr_first = ptr;
      break;

    case MEM_REF:
      ptr = TREE_OPERAND (node, 0);
      addr_first = chkp_build_addr_expr (node);
      break;

    case TARGET_MEM_REF:
      ptr = TMR_BASE (node);
      addr_first = chkp_build_addr_expr (node);
      break;

    case ARRAY_RANGE_REF:
      printf("ARRAY_RANGE_REF\n");
      debug_gimple_stmt(gsi_stmt(*iter));
      debug_tree(node);
      gcc_unreachable ();
      break;

    case BIT_FIELD_REF:
      {
	tree offs, rem, bpu;

	gcc_assert (!access_offs);
	gcc_assert (!access_size);

	bpu = fold_convert (size_type_node, bitsize_int (BITS_PER_UNIT));
	offs = fold_convert (size_type_node, TREE_OPERAND (node, 2));
	rem = size_binop_loc (loc, TRUNC_MOD_EXPR, offs, bpu);
	offs = size_binop_loc (loc, TRUNC_DIV_EXPR, offs, bpu);

	size = fold_convert (size_type_node, TREE_OPERAND (node, 1));
        size = size_binop_loc (loc, PLUS_EXPR, size, rem);
        size = size_binop_loc (loc, CEIL_DIV_EXPR, size, bpu);
        size = fold_convert (size_type_node, size);

	chkp_process_stmt (iter, TREE_OPERAND (node, 0), loc,
			 dirflag, offs, size, safe);
	return;
      }
      break;

    case VAR_DECL:
    case RESULT_DECL:
    case PARM_DECL:
      if (dirflag != integer_one_node
	  || DECL_REGISTER (node))
	return;

      safe = true;
      addr_first = chkp_build_addr_expr (node);
      break;

    default:
      return;
    }

  /* If addr_last was not computed then use (addr_first + size - 1)
     expression to compute it.  */
  if (!addr_last)
    {
      addr_last = fold_build_pointer_plus_loc (loc, addr_first, size);
      addr_last = fold_build_pointer_plus_hwi_loc (loc, addr_last, -1);
    }

  /* Shift both first_addr and last_addr by access_offs if specified.  */
  if (access_offs)
    {
      addr_first = fold_build_pointer_plus_loc (loc, addr_first, access_offs);
      addr_last = fold_build_pointer_plus_loc (loc, addr_last, access_offs);
    }

  /* Generate bndcl/bndcu checks if memory access is not safe.  */
  if (!safe)
    {
      gimple_stmt_iterator stmt_iter = *iter;

      if (!bounds)
	bounds = chkp_find_bounds (ptr, iter);

      chkp_check_mem_access (addr_first, addr_last, bounds,
			     stmt_iter, loc, dirflag);
    }

  /* We need to store bounds in case pointer is stored.  */
  if (dirflag == integer_one_node
      && chkp_type_has_pointer (node_type)
      && flag_chkp_store_bounds)
    {
      gimple stmt = gsi_stmt (*iter);
      tree rhs1 = gimple_assign_rhs1 (stmt);
      enum tree_code rhs_code = gimple_assign_rhs_code (stmt);

      if (get_gimple_rhs_class (rhs_code) == GIMPLE_SINGLE_RHS)
	chkp_walk_pointer_assignments (node, rhs1, iter,
				       chkp_copy_bounds_for_elem);
      else
	{
	  bounds = chkp_compute_bounds_for_assignment (NULL_TREE, stmt);
	  chkp_build_bndstx (addr_first, rhs1, bounds, iter);
	}
    }
}

/* Add code to copy bounds for all pointers copied
   in ASSIGN created during inline of EDGE.  */
void
chkp_copy_bounds_for_assign (gimple assign, struct cgraph_edge *edge)
{
  tree lhs = gimple_assign_lhs (assign);
  tree rhs = gimple_assign_rhs1 (assign);
  gimple_stmt_iterator iter = gsi_for_stmt (assign);

  if (!flag_chkp_store_bounds)
    return;

  chkp_walk_pointer_assignments (lhs, rhs, &iter, chkp_copy_bounds_for_elem);

  /* We should create edges for all created calls to bndldx and bndstx.  */
  while (gsi_stmt (iter) != assign)
    {
      gimple stmt = gsi_stmt (iter);
      if (gimple_code (stmt) == GIMPLE_CALL)
	{
	  tree fndecl = gimple_call_fndecl (stmt);
	  struct cgraph_node *callee = cgraph_get_create_node (fndecl);
	  struct cgraph_edge *new_edge;

	  gcc_assert (fndecl == chkp_bndstx_fndecl
		      || fndecl == chkp_bndldx_fndecl
		      || fndecl == chkp_ret_bnd_fndecl);

	  new_edge = cgraph_create_edge (edge->caller, callee, stmt,
					 edge->count, edge->frequency);
	  new_edge->frequency = compute_call_stmt_bb_frequency
	    (edge->caller->decl, gimple_bb (stmt));
	}
      gsi_prev (&iter);
    }
}

/* Some code transformation made during instrumentation pass
   may put code into inconsistent state.  Here we find and fix
   such flaws.  */
static void
chkp_fix_cfg ()
{
  basic_block bb;
  gimple_stmt_iterator i;

  /* We could insert some code right after stmt which ends bb.
     We wanted to put this code on fallthru edge but did not
     add new edges from the beginning because it may cause new
     phi node creation which may be incorrect due to incomplete
     bound phi nodes.  */
  FOR_ALL_BB_FN (bb, cfun)
    for (i = gsi_start_bb (bb); !gsi_end_p (i); gsi_next (&i))
      {
	gimple stmt = gsi_stmt (i);
	gimple_stmt_iterator next = i;

	gsi_next (&next);

	if (stmt_ends_bb_p (stmt)
	    && !gsi_end_p (next))
	  {
	    edge fall = find_fallthru_edge (bb->succs);
	    basic_block dest = NULL;
	    int flags = 0;

	    gcc_assert (fall);

	    /* We cannot split abnormal edge.  Therefore we
	       store its params, make it regular and then
	       rebuild abnormal edge after split.  */
	    if (fall->flags & EDGE_ABNORMAL)
	      {
		flags = fall->flags & ~EDGE_FALLTHRU;
		dest = fall->dest;

		fall->flags &= ~EDGE_COMPLEX;
	      }

	    while (!gsi_end_p (next))
	      {
		gimple next_stmt = gsi_stmt (next);
		gsi_remove (&next, false);
		gsi_insert_on_edge (fall, next_stmt);
	      }

	    gsi_commit_edge_inserts ();

	    /* Re-create abnormal edge.  */
	    if (dest)
	      make_edge (bb, dest, flags);
	  }
      }
}

/* Walker callback for chkp_replace_function_pointers.  Replaces
   function pointer in the specified operand with pointer to the
   instrumented function version.  */
static tree
chkp_replace_function_pointer (tree *op, int *walk_subtrees,
			       void *data ATTRIBUTE_UNUSED)
{
  if (TREE_CODE (*op) == FUNCTION_DECL
      && !lookup_attribute ("bnd_legacy", DECL_ATTRIBUTES (*op)))
    {
      struct cgraph_node *node = cgraph_get_create_node (*op);

      if (!node->instrumentation_clone)
	chkp_maybe_create_clone (*op);

      *op = node->instrumented_version->decl;
      *walk_subtrees = 0;
    }

  return NULL;
}

/* This function searches for function pointers in statement
   pointed by GSI and replaces them with pointers to instrumented
   function versions.  */
static void
chkp_replace_function_pointers (gimple_stmt_iterator *gsi)
{
  gimple stmt = gsi_stmt (*gsi);
  /* For calls we want to walk call args only.  */
  if (gimple_code (stmt) == GIMPLE_CALL)
    {
      unsigned i;
      for (i = 0; i < gimple_call_num_args (stmt); i++)
	walk_tree (gimple_call_arg_ptr (stmt, i),
		   chkp_replace_function_pointer, NULL, NULL);
    }
  else
    walk_gimple_stmt (gsi, NULL, chkp_replace_function_pointer, NULL);
}

/* This function requests intrumentation for all statements
   working with memory, calls and rets.  It also removes
   excess statements from static initializers.  */
static void
chkp_instrument_function (void)
{
  basic_block bb, next;
  gimple_stmt_iterator i;
  enum gimple_rhs_class grhs_class;
  bool safe = lookup_attribute ("chkp ctor", DECL_ATTRIBUTES (cfun->decl));

  bb = ENTRY_BLOCK_PTR_FOR_FN (cfun)->next_bb;
  do
    {
      next = bb->next_bb;
      for (i = gsi_start_bb (bb); !gsi_end_p (i); )
        {
          gimple s = gsi_stmt (i);

	  /* Skip statement marked to not be instrumented.  */
	  if (chkp_marked_stmt_p (s))
	    {
	      gsi_next (&i);
	      continue;
	    }

	  chkp_replace_function_pointers (&i);

          switch (gimple_code (s))
            {
            case GIMPLE_ASSIGN:
	      chkp_process_stmt (&i, gimple_assign_lhs (s),
				 gimple_location (s), integer_one_node,
				 NULL_TREE, NULL_TREE, safe);
	      chkp_process_stmt (&i, gimple_assign_rhs1 (s),
				 gimple_location (s), integer_zero_node,
				 NULL_TREE, NULL_TREE, safe);
	      grhs_class = get_gimple_rhs_class (gimple_assign_rhs_code (s));
	      if (grhs_class == GIMPLE_BINARY_RHS)
		chkp_process_stmt (&i, gimple_assign_rhs2 (s),
				   gimple_location (s), integer_zero_node,
				   NULL_TREE, NULL_TREE, safe);
              break;

            case GIMPLE_RETURN:
              if (gimple_return_retval (s) != NULL_TREE)
                {
                  chkp_process_stmt (&i, gimple_return_retval (s),
				     gimple_location (s),
				     integer_zero_node,
				     NULL_TREE, NULL_TREE, safe);

		  /* Additionall we need to add bounds
		     to return statement.  */
		  chkp_add_bounds_to_ret_stmt (&i);
                }
              break;

	    case GIMPLE_CALL:
	      chkp_add_bounds_to_call_stmt (&i);
	      break;

            default:
              ;
            }

	  gsi_next (&i);

	  /* We do not need any actual pointer stores in checker
	     static initializer.  */
	  if (lookup_attribute ("chkp ctor", DECL_ATTRIBUTES (cfun->decl))
	      && gimple_code (s) == GIMPLE_ASSIGN
	      && gimple_store_p (s))
	    {
	      gimple_stmt_iterator del_iter = gsi_for_stmt (s);
	      gsi_remove (&del_iter, true);
	      unlink_stmt_vdef (s);
	      release_defs(s);
	    }
        }
      bb = next;
    }
  while (bb);

  /* Some input params may have bounds and be address taken.  In this case
     we should store incomping bounds into bounds table.  */
  tree arg;
  if (flag_chkp_store_bounds)
    for (arg = DECL_ARGUMENTS (cfun->decl); arg; arg = DECL_CHAIN (arg))
      if (TREE_ADDRESSABLE (arg))
	{
	  if (BOUNDED_P (arg))
	    {
	      tree bounds = chkp_get_next_bounds_parm (arg);
	      tree def_ptr = ssa_default_def (cfun, arg);
	      gimple_stmt_iterator iter
		= gsi_start_bb (chkp_get_entry_block ());
	      chkp_build_bndstx (chkp_build_addr_expr (arg),
				 def_ptr ? def_ptr : arg,
				 bounds, &iter);

	      /* Skip bounds arg.  */
	      arg = TREE_CHAIN (arg);
	    }
	  else if (chkp_type_has_pointer (TREE_TYPE (arg)))
	    {
	      tree orig_arg = arg;
	      bitmap slots = chkp_find_bound_slots (TREE_TYPE (arg));
	      gimple_stmt_iterator iter
		= gsi_start_bb (chkp_get_entry_block ());
	      bitmap_iterator bi;
	      unsigned bnd_no;

	      EXECUTE_IF_SET_IN_BITMAP (slots, 0, bnd_no, bi)
		{
		  tree bounds = chkp_get_next_bounds_parm (arg);
		  HOST_WIDE_INT offs = bnd_no * POINTER_SIZE / BITS_PER_UNIT;
		  tree addr = chkp_build_addr_expr (orig_arg);
		  tree ptr = build2 (MEM_REF, ptr_type_node, addr,
				     build_int_cst (ptr_type_node, offs));
		  chkp_build_bndstx (chkp_build_addr_expr (ptr), ptr,
				     bounds, &iter);

		  arg = DECL_CHAIN (arg);
		}
	      BITMAP_FREE (slots);
	    }
	}
}

/* Find init/null/copy_ptr_bounds calls and replace them
   with assignments.  It should allow better code
   optimization.  */

static void
chkp_remove_useless_builtins ()
{
  basic_block bb, next;
  gimple_stmt_iterator gsi;

  bb = ENTRY_BLOCK_PTR_FOR_FN (cfun)->next_bb;
  do
    {
      next = bb->next_bb;
      for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
        {
          gimple stmt = gsi_stmt (gsi);
	  tree fndecl;
	  enum built_in_function fcode;

	  /* Find builtins returning first arg and replace
	     them with assignments.  */
	  if (gimple_code (stmt) == GIMPLE_CALL
	      && (fndecl = gimple_call_fndecl (stmt))
	      && DECL_BUILT_IN_CLASS (fndecl) == BUILT_IN_NORMAL
	      && (fcode = DECL_FUNCTION_CODE (fndecl))
	      && (fcode == BUILT_IN_CHKP_INIT_PTR_BOUNDS
		  || fcode == BUILT_IN_CHKP_NULL_PTR_BOUNDS
		  || fcode == BUILT_IN_CHKP_COPY_PTR_BOUNDS
		  || fcode == BUILT_IN_CHKP_SET_PTR_BOUNDS))
	    {
	      tree res = gimple_call_arg (stmt, 0);
	      if (!update_call_from_tree (&gsi, res))
		gimplify_and_update_call_from_tree (&gsi, res);
	      stmt = gsi_stmt (gsi);
	      update_stmt (stmt);
	    }
        }
      bb = next;
    }
  while (bb);
}

/* Initialize pass.  */
static void
chkp_init (void)
{
  basic_block bb;
  gimple_stmt_iterator i;

  in_chkp_pass = true;

  for (bb = ENTRY_BLOCK_PTR_FOR_FN (cfun)->next_bb; bb; bb = bb->next_bb)
    for (i = gsi_start_bb (bb); !gsi_end_p (i); gsi_next (&i))
      chkp_unmark_stmt (gsi_stmt (i));

  chkp_invalid_bounds = pointer_set_create ();
  chkp_completed_bounds_set = pointer_set_create ();
  if (chkp_reg_bounds)
    pointer_map_destroy (chkp_reg_bounds);
  chkp_reg_bounds = pointer_map_create ();
  chkp_reg_addr_bounds = pointer_map_create ();
  chkp_incomplete_bounds_map = pointer_map_create ();
  if (chkp_bounds_map)
    pointer_map_destroy (chkp_bounds_map);
  chkp_bounds_map = pointer_map_create ();
  chkp_abnormal_phi_copies = htab_create_ggc (31, tree_map_base_hash,
					      tree_vec_map_eq, NULL);

  entry_block = NULL;
  zero_bounds = NULL_TREE;
  none_bounds = NULL_TREE;
  incomplete_bounds = integer_zero_node;
  tmp_var = NULL_TREE;
  size_tmp_var = NULL_TREE;

  chkp_uintptr_type = lang_hooks.types.type_for_mode (ptr_mode, true);

  /* We create these constant bounds once for each object file.
     These symbols go to comdat section and result in single copy
     of each one in the final binary.  */
  chkp_get_zero_bounds_var ();
  chkp_get_none_bounds_var ();
}

/* Finalize instrumentation pass.  */
static void
chkp_fini (void)
{
  in_chkp_pass = false;

  pointer_set_destroy (chkp_invalid_bounds);
  pointer_set_destroy (chkp_completed_bounds_set);
  pointer_map_destroy (chkp_reg_addr_bounds);
  pointer_map_destroy (chkp_incomplete_bounds_map);
}

/* Main instrumentation pass function.  */
static unsigned int
chkp_execute (void)
{
  chkp_init ();

  chkp_instrument_function ();

  chkp_remove_useless_builtins ();

  chkp_function_mark_instrumented (cfun->decl);

  chkp_fix_cfg ();

  chkp_fini ();

  return 0;
}

/* Instrumentation pass gate.  */
static bool
chkp_gate (void)
{
  return cgraph_get_node (cfun->decl)->instrumentation_clone
    || lookup_attribute ("chkp ctor", DECL_ATTRIBUTES (cfun->decl));
}

/* Comparator for pol_item structures I1 and I2 to be used
   to find items with equal var.  Also used for polynomial
   sorting.  */
int
chkp_pol_item_compare (const void *i1, const void *i2)
{
  const struct pol_item *p1 = (const struct pol_item *)i1;
  const struct pol_item *p2 = (const struct pol_item *)i2;

  if (p1->var == p2->var)
    return 0;
  else if (p1->var > p2->var)
    return 1;
  else
    return -1;
}

/* Find plynomial item in ADDR with var equal to VAR
   and return its index.  Return -1 if item was not
   found.  */
int
chkp_pol_find (address_t &addr, tree var)
{
  int left = 0;
  int right = addr.pol.length () - 1;
  int n;

  while (right >= left)
    {
      n = (left + right) / 2;

      if (addr.pol[n].var == var
	  || (var && addr.pol[n].var
	      && TREE_CODE (var) == ADDR_EXPR
	      && TREE_CODE (addr.pol[n].var) == ADDR_EXPR
	      && TREE_OPERAND (var, 0) == TREE_OPERAND (addr.pol[n].var, 0)))
	return n;
      else if (addr.pol[n].var > var)
	right = n - 1;
      else
	left = n + 1;
    }

  return -1;
}

/* Return constant CST extended to size type.  */
tree
chkp_extend_const (tree cst)
{
  if (TYPE_PRECISION (TREE_TYPE (cst)) < TYPE_PRECISION (size_type_node))
    return build_int_cst_type (size_type_node, tree_to_shwi (cst));

  return cst;
}

/* Add polynomial item CST * VAR to ADDR.  */
void
chkp_add_addr_item (address_t &addr, tree cst, tree var)
{
  int n = chkp_pol_find (addr, var);

  cst = chkp_extend_const (cst);

  if (n < 0)
    {
      struct pol_item item;
      item.cst = cst;
      item.var = var;

      addr.pol.safe_push (item);
      addr.pol.qsort (&chkp_pol_item_compare);
    }
  else
    {
      addr.pol[n].cst = fold_build2 (PLUS_EXPR, TREE_TYPE (addr.pol[n].cst),
				     addr.pol[n].cst, cst);
      if (TREE_CODE (addr.pol[n].cst) == INTEGER_CST
	  && integer_zerop (addr.pol[n].cst))
	addr.pol.ordered_remove (n);
    }
}

/* Subtract polynomial item CST * VAR from ADDR.  */
void
chkp_sub_addr_item (address_t &addr, tree cst, tree var)
{
  int n = chkp_pol_find (addr, var);

  cst = chkp_extend_const (cst);

  if (n < 0)
    {
      struct pol_item item;
      item.cst = fold_build2 (MINUS_EXPR, TREE_TYPE (cst),
			      integer_zero_node, cst);
      item.var = var;

      addr.pol.safe_push (item);
      addr.pol.qsort (&chkp_pol_item_compare);
    }
  else
    {
      addr.pol[n].cst = fold_build2 (MINUS_EXPR, TREE_TYPE (addr.pol[n].cst),
				     addr.pol[n].cst, cst);
      if (TREE_CODE (addr.pol[n].cst) == INTEGER_CST
	  && integer_zerop (addr.pol[n].cst))
	addr.pol.ordered_remove (n);
    }
}

/* Add address DELTA to ADDR.  */
void
chkp_add_addr_addr (address_t &addr, address_t &delta)
{
  unsigned int i;
  for (i = 0; i < delta.pol.length (); i++)
    chkp_add_addr_item (addr, delta.pol[i].cst, delta.pol[i].var);
}

/* Subtract address DELTA from ADDR.  */
void
chkp_sub_addr_addr (address_t &addr, address_t &delta)
{
  unsigned int i;
  for (i = 0; i < delta.pol.length (); i++)
    chkp_sub_addr_item (addr, delta.pol[i].cst, delta.pol[i].var);
}

/* Mutiply address ADDR by integer constant MULT.  */
void
chkp_mult_addr (address_t &addr, tree mult)
{
  unsigned int i;
  for (i = 0; i < addr.pol.length (); i++)
    addr.pol[i].cst = fold_build2 (MULT_EXPR, TREE_TYPE (addr.pol[i].cst),
				   addr.pol[i].cst, mult);
}

/* Return 1 if we may prove ADDR has a constant value with
   determined sign, which is put into *SIGN.  Otherwise
   return 0.  */
bool
chkp_is_constant_addr (const address_t &addr, int *sign)
{
  *sign = 0;

  if (addr.pol.length () == 0)
    return true;
  else if (addr.pol.length () > 1)
    return false;
  else if (addr.pol[0].var)
    return false;
  else if (integer_zerop (addr.pol[0].cst))
    *sign = 0;
  else if  (tree_int_cst_sign_bit (addr.pol[0].cst))
    *sign = -1;
  else
    *sign = 1;

  return true;
}

/* Dump ADDR into dump_file.  */
void
chkp_print_addr (const address_t &addr)
{
  unsigned int n = 0;
  for (n = 0; n < addr.pol.length (); n++)
    {
      if (n > 0)
	fprintf (dump_file, " + ");

      if (addr.pol[n].var == NULL_TREE)
	print_generic_expr (dump_file, addr.pol[n].cst, 0);
      else
	{
	  if (TREE_CODE (addr.pol[n].cst) != INTEGER_CST
	      || !integer_onep (addr.pol[n].cst))
	    {
	      print_generic_expr (dump_file, addr.pol[n].cst, 0);
	      fprintf (dump_file, " * ");
	    }
	  print_generic_expr (dump_file, addr.pol[n].var, 0);
	}
    }
}

/* Compute value of PTR and put it into address RES.
   PTR has to be ADDR_EXPR.  */
void
chkp_collect_addr_value (tree ptr, address_t &res)
{
  tree obj = TREE_OPERAND (ptr, 0);
  address_t addr;

  switch (TREE_CODE (obj))
    {
    case INDIRECT_REF:
      chkp_collect_value (TREE_OPERAND (obj, 0), res);
      break;

    case MEM_REF:
      chkp_collect_value (TREE_OPERAND (obj, 0), res);
      addr.pol.create (0);
      chkp_collect_value (TREE_OPERAND (obj, 1), addr);
      chkp_add_addr_addr (res, addr);
      addr.pol.release ();
      break;

    case ARRAY_REF:
      chkp_collect_value (build_fold_addr_expr (TREE_OPERAND (obj, 0)), res);
      addr.pol.create (0);
      chkp_collect_value (TREE_OPERAND (obj, 1), addr);
      chkp_mult_addr (addr, array_ref_element_size (obj));
      chkp_add_addr_addr (res, addr);
      addr.pol.release ();
      break;

    case COMPONENT_REF:
      {
	tree str = TREE_OPERAND (obj, 0);
	tree field = TREE_OPERAND (obj, 1);
	chkp_collect_value (build_fold_addr_expr (str), res);
	addr.pol.create (0);
	chkp_collect_value (component_ref_field_offset (obj), addr);
	chkp_add_addr_addr (res, addr);
	addr.pol.release ();
	if (DECL_FIELD_BIT_OFFSET (field))
	  {
	    addr.pol.create (0);
	    chkp_collect_value (fold_build2 (TRUNC_DIV_EXPR, size_type_node,
					     DECL_FIELD_BIT_OFFSET (field),
					     size_int (BITS_PER_UNIT)),
			   addr);
	    chkp_add_addr_addr (res, addr);
	    addr.pol.release ();
	  }
      }
      break;

    default:
      chkp_add_addr_item (res, integer_one_node, ptr);
      break;
    }
}

/* Compute value of PTR and put it into address RES.  */
void
chkp_collect_value (tree ptr, address_t &res)
{
  gimple def_stmt;
  enum gimple_code code;
  enum tree_code rhs_code;
  address_t addr;
  tree rhs1;

  if (TREE_CODE (ptr) == INTEGER_CST)
    {
      chkp_add_addr_item (res, ptr, NULL);
      return;
    }
  else if (TREE_CODE (ptr) == ADDR_EXPR)
    {
      chkp_collect_addr_value (ptr, res);
      return;
    }
  else if (TREE_CODE (ptr) != SSA_NAME)
    {
      chkp_add_addr_item (res, integer_one_node, ptr);
      return;
    }

  /* Now we handle the case when polynomial is computed
     for SSA NAME.  */
  def_stmt = SSA_NAME_DEF_STMT (ptr);
  code = gimple_code (def_stmt);

  /* Currently we do not walk through statements other
     than assignment.  */
  if (code != GIMPLE_ASSIGN)
    {
      chkp_add_addr_item (res, integer_one_node, ptr);
      return;
    }

  rhs_code = gimple_assign_rhs_code (def_stmt);
  rhs1 = gimple_assign_rhs1 (def_stmt);

  switch (rhs_code)
    {
    case SSA_NAME:
    case INTEGER_CST:
    case ADDR_EXPR:
      chkp_collect_value (rhs1, res);
      break;

    case PLUS_EXPR:
    case POINTER_PLUS_EXPR:
      chkp_collect_value (rhs1, res);
      addr.pol.create (0);
      chkp_collect_value (gimple_assign_rhs2 (def_stmt), addr);
      chkp_add_addr_addr (res, addr);
      addr.pol.release ();
      break;

    case MINUS_EXPR:
      chkp_collect_value (rhs1, res);
      addr.pol.create (0);
      chkp_collect_value (gimple_assign_rhs2 (def_stmt), addr);
      chkp_sub_addr_addr (res, addr);
      addr.pol.release ();
      break;

    case MULT_EXPR:
      if (TREE_CODE (rhs1) == SSA_NAME
	  && TREE_CODE (gimple_assign_rhs2 (def_stmt)) == INTEGER_CST)
	{
	  chkp_collect_value (rhs1, res);
	  chkp_mult_addr (res, gimple_assign_rhs2 (def_stmt));
	}
      else if (TREE_CODE (gimple_assign_rhs2 (def_stmt)) == SSA_NAME
	       && TREE_CODE (rhs1) == INTEGER_CST)
	{
	  chkp_collect_value (gimple_assign_rhs2 (def_stmt), res);
	  chkp_mult_addr (res, rhs1);
	}
      else
	chkp_add_addr_item (res, integer_one_node, ptr);
      break;

    default:
      chkp_add_addr_item (res, integer_one_node, ptr);
      break;
    }
}

/* Fill check_info structure *CI with information about
   check STMT.  */
void
chkp_fill_check_info (gimple stmt, struct check_info *ci)
{
  ci->addr.pol.create (0);
  ci->bounds = gimple_call_arg (stmt, 1);
  chkp_collect_value (gimple_call_arg (stmt, 0), ci->addr);
  ci->type = (gimple_call_fndecl (stmt) == chkp_checkl_fndecl
	     ? CHECK_LOWER_BOUND
	     : CHECK_UPPER_BOUND);
  ci->stmt = stmt;
}

/* Release structures holding check information
   for current function.  */
void
chkp_release_check_info (void)
{
  unsigned int n, m;

  if (check_infos.exists ())
    {
      for (n = 0; n < check_infos.length (); n++)
	{
	  for (m = 0; m < check_infos[n].checks.length (); m++)
	    if (check_infos[n].checks[m].addr.pol.exists ())
	      check_infos[n].checks[m].addr.pol.release ();
	  check_infos[n].checks.release ();
	}
      check_infos.release ();
    }
}

/* Create structures to hold check information
   for current function.  */
void
chkp_init_check_info (void)
{
  struct bb_checks empty_bbc;
  int n;

  empty_bbc.checks = vNULL;

  chkp_release_check_info ();

  check_infos.create (last_basic_block_for_fn (cfun));
  for (n = 0; n < last_basic_block_for_fn (cfun); n++)
    {
      check_infos.safe_push (empty_bbc);
      check_infos.last ().checks.create (0);
    }
}

/* Find all checks in current function and store info about them
   in check_infos.  */
void
chkp_gather_checks_info (void)
{
  basic_block bb;
  gimple_stmt_iterator i;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Gathering information about checks...\n");

  chkp_init_check_info ();

  FOR_EACH_BB_FN (bb, cfun)
    {
      struct bb_checks *bbc = &check_infos[bb->index];

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Searching checks in BB%d...\n", bb->index);

      for (i = gsi_start_bb (bb); !gsi_end_p (i); gsi_next (&i))
        {
	  gimple stmt = gsi_stmt (i);

	  if (gimple_code (stmt) != GIMPLE_CALL)
	    continue;

	  if (gimple_call_fndecl (stmt) == chkp_checkl_fndecl
	      || gimple_call_fndecl (stmt) == chkp_checku_fndecl)
	    {
	      struct check_info ci;

	      chkp_fill_check_info (stmt, &ci);
	      bbc->checks.safe_push (ci);

	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file, "Adding check information:\n");
		  fprintf (dump_file, "  bounds: ");
		  print_generic_expr (dump_file, ci.bounds, 0);
		  fprintf (dump_file, "\n  address: ");
		  chkp_print_addr (ci.addr);
		  fprintf (dump_file, "\n  check: ");
		  print_gimple_stmt (dump_file, stmt, 0, 0);
		}
	    }
	}
    }
}

/* Return 1 if check CI against BOUNDS always pass,
   -1 if check CI against BOUNDS always fails and
   0 if we cannot compute check result.  */
int
chkp_get_check_result (struct check_info *ci, tree bounds)
{
  gimple bnd_def;
  address_t bound_val;
  int sign, res = 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Trying to compute result of the check\n");
      fprintf (dump_file, "  check: ");
      print_gimple_stmt (dump_file, ci->stmt, 0, 0);
      fprintf (dump_file, "  address: ");
      chkp_print_addr (ci->addr);
      fprintf (dump_file, "\n  bounds: ");
      print_generic_expr (dump_file, bounds, 0);
      fprintf (dump_file, "\n");
    }

  if (TREE_CODE (bounds) != SSA_NAME)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: bounds tree code is not ssa_name\n");
      return 0;
    }

  if (bounds == zero_bounds)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: always pass with zero bounds\n");
      return 1;
    }

  if (bounds == none_bounds)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: always fails with none bounds\n");
      return -1;
    }

  bnd_def = SSA_NAME_DEF_STMT (bounds);
  /* Currently we handle cases when bounds are result of bndmk
     or loaded static bounds var.  */
  if (gimple_code (bnd_def) == GIMPLE_CALL
      && gimple_call_fndecl (bnd_def) == chkp_bndmk_fndecl)
    {
      bound_val.pol.create (0);
      chkp_collect_value (gimple_call_arg (bnd_def, 0), bound_val);
      if (ci->type == CHECK_UPPER_BOUND)
	{
	  address_t size_val;
	  size_val.pol.create (0);
	  chkp_collect_value (gimple_call_arg (bnd_def, 1), size_val);
	  chkp_add_addr_addr (bound_val, size_val);
	  size_val.pol.release ();
	  chkp_add_addr_item (bound_val, integer_minus_one_node, NULL);
	}
    }
  else if (gimple_code (bnd_def) == GIMPLE_ASSIGN
	   && gimple_assign_rhs1 (bnd_def) == chkp_zero_bounds_var)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: always pass with zero bounds\n");
      return 1;
    }
  else if (gimple_code (bnd_def) == GIMPLE_ASSIGN
	   && gimple_assign_rhs1 (bnd_def) == chkp_none_bounds_var)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: always fails with none bounds\n");
      return -1;
    }
  else if (gimple_code (bnd_def) == GIMPLE_ASSIGN
	   && TREE_CODE (gimple_assign_rhs1 (bnd_def)) == VAR_DECL)
    {
      tree bnd_var = gimple_assign_rhs1 (bnd_def);
      tree var;
      tree size;

      if (!DECL_INITIAL (bnd_var)
	  || DECL_INITIAL (bnd_var) == error_mark_node)
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file, "  result: cannot compute bounds\n");
	  return 0;
	}

      gcc_assert (TREE_CODE (DECL_INITIAL (bnd_var)) == ADDR_EXPR);
      var = TREE_OPERAND (DECL_INITIAL (bnd_var), 0);

      bound_val.pol.create (0);
      chkp_collect_value (DECL_INITIAL (bnd_var), bound_val);
      if (ci->type == CHECK_UPPER_BOUND)
	{
	  if (TREE_CODE (var) == VAR_DECL)
	    {
	      if (DECL_SIZE (var)
		  && !chkp_variable_size_type (TREE_TYPE (var)))
		size = DECL_SIZE_UNIT (var);
	      else
		{
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    fprintf (dump_file, "  result: cannot compute bounds\n");
		  return 0;
		}
	    }
	  else
	    {
	      gcc_assert (TREE_CODE (var) == STRING_CST);
	      size = build_int_cst (size_type_node,
				    TREE_STRING_LENGTH (var));
	    }

	  address_t size_val;
	  size_val.pol.create (0);
	  chkp_collect_value (size, size_val);
	  chkp_add_addr_addr (bound_val, size_val);
	  size_val.pol.release ();
	  chkp_add_addr_item (bound_val, integer_minus_one_node, NULL);
	}
    }
  else
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: cannot compute bounds\n");
      return 0;
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "  bound value: ");
      chkp_print_addr (bound_val);
      fprintf (dump_file, "\n");
    }

  chkp_sub_addr_addr (bound_val, ci->addr);

  if (!chkp_is_constant_addr (bound_val, &sign))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: cannot compute result\n");

      res = 0;
    }
  else if (sign == 0
	   || (ci->type == CHECK_UPPER_BOUND && sign > 0)
	   || (ci->type == CHECK_LOWER_BOUND && sign < 0))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: always pass\n");

      res = 1;
    }
  else
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  result: always fail\n");

      res = -1;
    }

  bound_val.pol.release ();

  return res;
}

/* Try to compare bounds value and address value
   used in the check CI.  If we can prove that check
   always pass then remove it.  */
void
chkp_remove_check_if_pass (struct check_info *ci)
{
  int result = 0;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Trying to remove check: ");
      print_gimple_stmt (dump_file, ci->stmt, 0, 0);
    }

  result = chkp_get_check_result (ci, ci->bounds);

  if (result == 1)
    {
      gimple_stmt_iterator i = gsi_for_stmt (ci->stmt);

      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  action: delete check (always pass)\n");

      gsi_remove (&i, true);
      unlink_stmt_vdef (ci->stmt);
      release_defs (ci->stmt);
      ci->stmt = NULL;
    }
  else if (result == -1)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  action: keep check (always fail)\n");
    }
  else if (result == 0)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "  action: keep check (cannot compute result)\n");
    }
}

/* For bounds used in CI check if bounds are produced by
   intersection and we may use outer bounds instead.  If
   transformation is possible then fix check statement and
   recompute its info.  */
void
chkp_use_outer_bounds_if_possible (struct check_info *ci)
{
  gimple bnd_def;
  tree bnd1, bnd2, bnd_res = NULL;
  int check_res1, check_res2;

  if (TREE_CODE (ci->bounds) != SSA_NAME)
    return;

  bnd_def = SSA_NAME_DEF_STMT (ci->bounds);
  if (gimple_code (bnd_def) != GIMPLE_CALL
      || gimple_call_fndecl (bnd_def) != chkp_intersect_fndecl)
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Check if bounds intersection is redundant: \n");
      fprintf (dump_file, "  check: ");
      print_gimple_stmt (dump_file, ci->stmt, 0, 0);
      fprintf (dump_file, "  intersection: ");
      print_gimple_stmt (dump_file, bnd_def, 0, 0);
      fprintf (dump_file, "\n");
    }

  bnd1 = gimple_call_arg (bnd_def, 0);
  bnd2 = gimple_call_arg (bnd_def, 1);

  check_res1 = chkp_get_check_result (ci, bnd1);
  check_res2 = chkp_get_check_result (ci, bnd2);
  if (check_res1 == 1)
    bnd_res = bnd2;
  else if (check_res1 == -1)
    bnd_res = bnd1;
  else if (check_res2 == 1)
    bnd_res = bnd1;
  else if (check_res2 == -1)
    bnd_res = bnd2;

  if (bnd_res)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "  action: use ");
	  print_generic_expr (dump_file, bnd2, 0);
	  fprintf (dump_file, " instead of ");
	  print_generic_expr (dump_file, ci->bounds, 0);
	}

      ci->bounds = bnd_res;
      gimple_call_set_arg (ci->stmt, 1, bnd_res);
      update_stmt (ci->stmt);
    }
}

/*  Try to find checks whose bounds were produced by intersection
    which does not affect check result.  In such check outer bounds
    are used instead.  It allows to remove excess intersections
    and helps to compare checks.  */
void
chkp_remove_excess_intersections (void)
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Searching for redundant bounds intersections...\n");

  FOR_EACH_BB_FN (bb, cfun)
    {
      struct bb_checks *bbc = &check_infos[bb->index];
      unsigned int no;

      /* Iterate throw all found checks in BB.  */
      for (no = 0; no < bbc->checks.length (); no++)
	if (bbc->checks[no].stmt)
	  chkp_use_outer_bounds_if_possible (&bbc->checks[no]);
    }
}

/*  Try to remove all checks which are known to alwyas pass.  */
void
chkp_remove_constant_checks (void)
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Searching for redundant checks...\n");

  FOR_EACH_BB_FN (bb, cfun)
    {
      struct bb_checks *bbc = &check_infos[bb->index];
      unsigned int no;

      /* Iterate throw all found checks in BB.  */
      for (no = 0; no < bbc->checks.length (); no++)
	if (bbc->checks[no].stmt)
	  chkp_remove_check_if_pass (&bbc->checks[no]);
    }
}

/* Compare two checks CI1 and CI2 to find redundant one.
   CI1 is known to dominate CI2.  POSTDOM indicated if
   CI2 postdominateds CI1.

   Few conditions are checked to find redundant check:
     1.  Checks has the same type
     2.  Checks use the same bounds
     3.  One check fail means other check fail
     4.  Stronger check is always executed if weaker one is executed

   If redundant check is found it is removed. If removed check is CI1
   then CI2 is moved to CI1's position to avoid bound violation happened
   before check is executed.  */
void
chkp_compare_checks (struct check_info *ci1,
		    struct check_info *ci2,
		    bool postdom)
{
  address_t delta;
  int sign;

  if (ci1->type != ci2->type)
    return;

  if (ci1->bounds != ci2->bounds)
    return;

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "  Comparing checks...\n");
      fprintf (dump_file, "    First check: ");
      print_gimple_stmt (dump_file, ci1->stmt, 0, 0);
      fprintf (dump_file, "    Second check: ");
      print_gimple_stmt (dump_file, ci2->stmt, 0, 0);
    }

  delta.pol = ci1->addr.pol.copy ();
  chkp_sub_addr_addr (delta, ci2->addr);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "    Delta: ");
      chkp_print_addr (delta);
      fprintf (dump_file, "\n");
    }

  if (!chkp_is_constant_addr (delta, &sign))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "    Action: skip (delta is not constant)\n");
    }
  else
    {
      if (sign)
	{
	  if ((sign > 0 && ci1->type == CHECK_UPPER_BOUND)
	      || (sign < 0 && ci1->type == CHECK_LOWER_BOUND))
	    {
	      gimple_stmt_iterator i = gsi_for_stmt (ci2->stmt);

	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "    Action: delete the second check\n");

	      gsi_remove (&i, true);
	      unlink_stmt_vdef (ci2->stmt);
	      release_defs (ci2->stmt);
	      ci2->stmt = NULL;
	    }
	  else if (postdom)
	    {
	      gimple_stmt_iterator i = gsi_for_stmt (ci2->stmt);
	      gimple_seq seq = NULL;
	      tree addr = gimple_call_arg (ci1->stmt, 0);
	      unsigned int n;

	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "    Action: replace the first "
			 "check with the second one\n");

	      gsi_remove (&i, true);
	      unlink_stmt_vdef (ci2->stmt);
	      release_defs (ci2->stmt);
	      ci2->stmt = NULL;

	      for (n = 0; n < delta.pol.length (); n++)
		if (delta.pol[n].var == NULL)
		  {
		    tree tmp = fold_build2 (MINUS_EXPR,
					    TREE_TYPE (delta.pol[n].cst),
					    integer_zero_node,
					    delta.pol[n].cst);
		    addr = fold_build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
					addr, convert_to_ptrofftype (tmp));
		  }
		else if (integer_onep (delta.pol[n].cst))
		  {
		    tree tmp = fold_build2 (MINUS_EXPR,
					    TREE_TYPE (delta.pol[n].var),
					    integer_zero_node,
					    delta.pol[n].var);
		    addr = build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
				   addr, convert_to_ptrofftype (tmp));
		  }
		else if (tree_int_cst_compare (delta.pol[n].cst,
					       integer_minus_one_node) == 0)
		  addr = build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
				 addr, convert_to_ptrofftype (delta.pol[n].var));
		else
		  {
		    tree tmp = fold_build2 (MULT_EXPR,
					    TREE_TYPE (delta.pol[n].var),
					    delta.pol[n].var,
					    delta.pol[n].cst);
		    tmp = fold_build2 (MINUS_EXPR, TREE_TYPE (tmp),
				       integer_zero_node, tmp);
		    addr = fold_build2 (POINTER_PLUS_EXPR, TREE_TYPE (addr),
					addr, convert_to_ptrofftype (tmp));
		  }

	      addr = force_gimple_operand (unshare_expr (addr), &seq,
					   true, NULL_TREE);

	      i = gsi_for_stmt (ci1->stmt);
	      gsi_insert_seq_before (&i, seq, GSI_CONTINUE_LINKING);
	      gimple_call_set_arg (ci1->stmt, 0, addr);
	      update_stmt (ci1->stmt);

	      ci1->addr.pol.release ();
	      chkp_fill_check_info (ci1->stmt, ci1);
	    }
	  else
	    {
	      if (dump_file && (dump_flags & TDF_DETAILS))
		fprintf (dump_file, "    Action: skip (the first check is "
			 "not post-dominanted by the second check)\n");
	    }
	}
      else
	{
	  gimple_stmt_iterator i = gsi_for_stmt (ci2->stmt);

	  if (dump_file && (dump_flags & TDF_DETAILS))
	    fprintf (dump_file,
		     "    Action: delete the second check (same addresses)\n");

	  gsi_remove (&i, true);
	  unlink_stmt_vdef (ci2->stmt);
	  release_defs (ci2->stmt);
	  ci2->stmt = NULL;
	}
    }

  delta.pol.release ();
}

/* Here we try to find checks which are covered by other checks
   and thus can be removed.  To do it we simply find all pairs of
   checks where the first check dominates the second one and
   call chkp_compare_checks to find and remove redundant ones.  */
void
chkp_remove_redundant_checks (void)
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Searching for redundant checks...\n");

  FOR_EACH_BB_FN (bb, cfun)
    {
      struct bb_checks *bbc = &check_infos[bb->index];
      unsigned int no;

      /* Iterate throw all found checks in BB.  */
      for (no = 0; no < bbc->checks.length (); no++)
	if (bbc->checks[no].stmt)
	  {
	    vec<basic_block> dom_bbs;
	    unsigned bb_no, other;

	    /* Compare check with all other following checks in this BB.  */
	    for (other = no + 1; other < bbc->checks.length (); other++)
	      if (bbc->checks[other].stmt)
		chkp_compare_checks (&bbc->checks[no], &bbc->checks[other],
				    true);

	    /* Now compare with checks in BBs dominated by current one.  */
	    dom_bbs = get_all_dominated_blocks (CDI_DOMINATORS, bb);
	    for (bb_no = 0; bb_no < dom_bbs.length (); bb_no++)
	      {
		struct bb_checks *dom_bbc = &check_infos[dom_bbs[bb_no]->index];

		if (dom_bbs[bb_no] == bb)
		  continue;

		for (other = 0; other < dom_bbc->checks.length (); other++)
		  if (dom_bbc->checks[other].stmt)
		    chkp_compare_checks (&bbc->checks[no],
					&dom_bbc->checks[other],
					dominated_by_p (CDI_POST_DOMINATORS, bb,
							dom_bbs[bb_no]));
	      }
	  }
    }
}

/* Return fast version of string function FNCODE.  */
tree
chkp_get_nobnd_fndecl (enum built_in_function fncode)
{
  /* Check if we are allowed to use fast string functions.  */
  if (!flag_chkp_use_fast_string_functions)
    return NULL_TREE;

  switch (fncode)
    {
    case BUILT_IN_MEMCPY:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMCPY_NOBND);

    case BUILT_IN_MEMPCPY:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMPCPY_NOBND);

    case BUILT_IN_MEMMOVE:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMMOVE_NOBND);

    case BUILT_IN_MEMSET:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMSET_NOBND);

    case BUILT_IN_CHKP_MEMCPY_NOCHK:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMCPY_NOBND_NOCHK);

    case BUILT_IN_CHKP_MEMPCPY_NOCHK:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMPCPY_NOBND_NOCHK);

    case BUILT_IN_CHKP_MEMMOVE_NOCHK:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMMOVE_NOBND_NOCHK);

    case BUILT_IN_CHKP_MEMSET_NOCHK:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMSET_NOBND_NOCHK);

    default:
      return NULL_TREE;
    }
}


/* Return no-check version of string function FNCODE.  */
tree
chkp_get_nochk_fndecl (enum built_in_function fncode)
{
  /* Check if we are allowed to use fast string functions.  */
  if (!flag_chkp_use_nochk_string_functions)
    return NULL_TREE;

  switch (fncode)
    {
    case BUILT_IN_MEMCPY:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMCPY_NOCHK);

    case BUILT_IN_MEMPCPY:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMPCPY_NOCHK);

    case BUILT_IN_MEMMOVE:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMMOVE_NOCHK);

    case BUILT_IN_MEMSET:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMSET_NOCHK);

    case BUILT_IN_CHKP_MEMCPY_NOBND:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMCPY_NOBND_NOCHK);

    case BUILT_IN_CHKP_MEMPCPY_NOBND:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMPCPY_NOBND_NOCHK);

    case BUILT_IN_CHKP_MEMMOVE_NOBND:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMMOVE_NOBND_NOCHK);

    case BUILT_IN_CHKP_MEMSET_NOBND:
      return builtin_decl_implicit (BUILT_IN_CHKP_MEMSET_NOBND_NOCHK);

    default:
      return NULL_TREE;
    }
}

/* Find memcpy, mempcpy, memmove and memset calls, perform
   checks before call and then call no_chk version of
   functions.  We do it on O2 to enable inlining of these
   functions during expand.

   Also try to find memcpy, mempcpy, memmove and memset calls
   which are known to not write pointers to memory and use
   faster function versions for them.  */
void
chkp_optimize_string_function_calls (void)
{
  basic_block bb;

  if (dump_file && (dump_flags & TDF_DETAILS))
    fprintf (dump_file, "Searching for replacable string function calls...\n");

  FOR_EACH_BB_FN (bb, cfun)
    {
      gimple_stmt_iterator i;

      for (i = gsi_start_bb (bb); !gsi_end_p (i); gsi_next (&i))
        {
	  gimple stmt = gsi_stmt (i);
	  tree fndecl;

	  if (gimple_code (stmt) != GIMPLE_CALL
	      || !gimple_call_with_bounds_p (stmt))
	    continue;

	  fndecl = gimple_call_fndecl (stmt);

	  if (!fndecl || DECL_BUILT_IN_CLASS (fndecl) != BUILT_IN_NORMAL)
	    continue;

	  if (DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMCPY
	      || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMPCPY
	      || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMMOVE
	      || DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMSET)
	    {
	      tree dst = gimple_call_arg (stmt, 0);
	      tree dst_bnd = gimple_call_arg (stmt, 1);
	      bool is_memset = DECL_FUNCTION_CODE (fndecl) == BUILT_IN_MEMSET;
	      tree size = gimple_call_arg (stmt, is_memset ? 3 : 4);
	      tree fndecl_nochk;
	      gimple_stmt_iterator j;
	      basic_block check_bb;
	      edge fall;
	      address_t size_val;
	      int sign;
	      bool known;

	      /* We may replace call with corresponding __chkp_*_nobnd
		 call in case destination pointer base type is not
		 void or pointer.  */
	      if (POINTER_TYPE_P (TREE_TYPE (dst))
		  && !VOID_TYPE_P (TREE_TYPE (TREE_TYPE (dst)))
		  && !chkp_type_has_pointer (TREE_TYPE (TREE_TYPE (dst))))
		{
		  tree fndecl_nobnd
		    = chkp_get_nobnd_fndecl (DECL_FUNCTION_CODE (fndecl));

		  if (fndecl_nobnd)
		    fndecl = fndecl_nobnd;
		}

	      fndecl_nochk = chkp_get_nochk_fndecl (DECL_FUNCTION_CODE (fndecl));

	      if (fndecl_nochk)
		fndecl = fndecl_nochk;

	      gimple_call_set_fndecl (stmt, fndecl);

	      /* If there is no nochk version of function then
		 do nothing.  Otherwise insert checks before
		 the call.  */
	      if (!fndecl_nochk)
		continue;

	      /* If size passed to call is known and > 0
		 then we may insert checks unconditionally.  */
	      size_val.pol.create (0);
	      chkp_collect_value (size, size_val);
	      known = chkp_is_constant_addr (size_val, &sign);
	      size_val.pol.release ();

	      /* If we are not sure size is not zero then we have
		 to perform runtiome check for size and perform
		 checks only when size is not zero.  */
	      if (!known)
		{
		  gimple check = gimple_build_cond (NE_EXPR,
						    size,
						    size_zero_node,
						    NULL_TREE,
						    NULL_TREE);

		  /* Split block before string function call.  */
		  j = i;
		  gsi_prev (&j);
		  fall = split_block (bb, gsi_stmt (j));
		  bb = fall->src;

		  /* Add size check.  */
		  j = gsi_last_bb (bb);
		  if (gsi_end_p (j))
		    gsi_insert_before (&j, check, GSI_SAME_STMT);
		  else
		    gsi_insert_after (&j, check, GSI_SAME_STMT);

		  /* Create basic block for checks.  */
		  check_bb = create_empty_bb (bb);
		  make_edge (bb, check_bb, EDGE_TRUE_VALUE);
		  make_single_succ_edge (check_bb, fall->dest, EDGE_FALLTHRU);

		  /* Fix edge for splitted bb.  */
		  fall->flags = EDGE_FALSE_VALUE;
		  fall->count = bb->count;
		  fall->probability = REG_BR_PROB_BASE;

		  /* Update dominance info.  */
		  if (dom_info_available_p (CDI_DOMINATORS))
		    {
		      set_immediate_dominator (CDI_DOMINATORS, check_bb, bb);
		      set_immediate_dominator (CDI_DOMINATORS, fall->dest, bb);
		    }

		  /* Update loop info.  */
		  if (current_loops)
		    add_bb_to_loop (check_bb, bb->loop_father);

		  /* Set position for checks.  */
		  j = gsi_last_bb (check_bb);

		  /* The block was splitted and therefore we
		     need to set iterator to its end.  */
		  i = gsi_last_bb (bb);
		}
	      /* If size is known to be zero then no checks
		 should be performed.  */
	      else if (!sign)
		continue;
	      else
		j = i;

	      size = size_binop (MINUS_EXPR, size, size_one_node);
	      if (!is_memset)
		{
		  tree src = gimple_call_arg (stmt, 2);
		  tree src_bnd = gimple_call_arg (stmt, 3);

		  chkp_check_mem_access (src, fold_build_pointer_plus (src, size),
					 src_bnd, j, gimple_location (stmt),
					 integer_zero_node);
		}

	      chkp_check_mem_access (dst, fold_build_pointer_plus (dst, size),
				     dst_bnd, j, gimple_location (stmt),
				     integer_one_node);

	    }
	}
    }
}

/* Intrumentation pass inserts most of bounds creation code
   in the header of the function.  We want to move bounds
   creation closer to bounds usage to reduce bounds lifetime.
   We also try to avoid bounds creation code on paths where
   bounds are not used.  */
void
chkp_reduce_bounds_lifetime (void)
{
  basic_block bb = FALLTHRU_EDGE (ENTRY_BLOCK_PTR_FOR_FN (cfun))->dest;
  gimple_stmt_iterator i;

  for (i = gsi_start_bb (bb); !gsi_end_p (i); )
    {
      gimple dom_use, use_stmt, stmt = gsi_stmt (i);
      basic_block dom_bb;
      ssa_op_iter iter;
      imm_use_iterator use_iter;
      use_operand_p use_p;
      tree op;
      bool want_move = false;
      bool deps = false;

      if (gimple_code (stmt) == GIMPLE_CALL
	  && gimple_call_fndecl (stmt) == chkp_bndmk_fndecl)
	want_move = true;

      if (gimple_code (stmt) == GIMPLE_ASSIGN
	  && POINTER_BOUNDS_P (gimple_assign_lhs (stmt))
	  && gimple_assign_rhs_code (stmt) == VAR_DECL)
	want_move = true;

      if (!want_move)
	{
	  gsi_next (&i);
	  continue;
	}

      /* Check we do not increase other values lifetime.  */
      FOR_EACH_PHI_OR_STMT_USE (use_p, stmt, iter, SSA_OP_USE)
	{
	  op = USE_FROM_PTR (use_p);

	  if (TREE_CODE (op) == SSA_NAME
	      && gimple_code (SSA_NAME_DEF_STMT (op)) != GIMPLE_NOP)
	    deps = true;
	}

      if (deps)
	{
	  gsi_next (&i);
	  continue;
	}

      /* Check all usages of bounds.  */
      if (gimple_code (stmt) == GIMPLE_CALL)
	op = gimple_call_lhs (stmt);
      else
	{
	  gcc_assert (gimple_code (stmt) == GIMPLE_ASSIGN);
	  op = gimple_assign_lhs (stmt);
	}

      dom_use = NULL;
      dom_bb = NULL;

      FOR_EACH_IMM_USE_STMT (use_stmt, use_iter, op)
	{
	  if (dom_bb &&
	      dominated_by_p (CDI_DOMINATORS,
			      dom_bb, gimple_bb (use_stmt)))
	    {
	      dom_use = use_stmt;
	      dom_bb = NULL;
	    }
	  else if (dom_bb)
	    dom_bb = nearest_common_dominator (CDI_DOMINATORS, dom_bb,
					       gimple_bb (use_stmt));
	  else if (!dom_use)
	    dom_use = use_stmt;
	  else if (stmt_dominates_stmt_p (use_stmt, dom_use))
	    dom_use = use_stmt;
	  else if (!stmt_dominates_stmt_p (dom_use, use_stmt)
		   /* If dom_use and use_stmt are PHI nodes in one BB
		      then it is OK to keep any of them as dom_use.
		      stmt_dominates_stmt_p returns 0 for such
		      combination, so check it here manually.  */
		   && (gimple_code (dom_use) != GIMPLE_PHI
		       || gimple_code (use_stmt) != GIMPLE_PHI
		       || gimple_bb (use_stmt) != gimple_bb (dom_use))
		   )
	    {
	      dom_bb = nearest_common_dominator (CDI_DOMINATORS,
						 gimple_bb (use_stmt),
						 gimple_bb (dom_use));
	      dom_use = NULL;
	    }
	}

      /* In case there is a single use, just move bounds
	 creation to the use.  */
      if (dom_use || dom_bb)
	{
	  if (dump_file && (dump_flags & TDF_DETAILS))
	    {
	      fprintf (dump_file, "Moving creation of ");
	      print_generic_expr (dump_file, op, 0);
	      fprintf (dump_file, " down to its use.\n");
	    }

	  if (dom_use && gimple_code (dom_use) == GIMPLE_PHI)
	    {
	      dom_bb = get_immediate_dominator (CDI_DOMINATORS,
						gimple_bb (dom_use));
	      dom_use = NULL;
	    }

	  if (dom_bb == bb
	      || (dom_use && gimple_bb (dom_use) == bb))
	    {
		  if (dump_file && (dump_flags & TDF_DETAILS))
		    fprintf (dump_file, "Cannot move statement bacause there is no "
			     "suitable dominator block other than entry block.\n");

		  gsi_next (&i);
	    }
	  else
	    {
	      if (dom_bb)
		{
		  gimple_stmt_iterator last = gsi_last_bb (dom_bb);
		  if (!gsi_end_p (last) && stmt_ends_bb_p (gsi_stmt (last)))
		    gsi_move_before (&i, &last);
		  else
		    gsi_move_after (&i, &last);
		}
	      else
		{
		  gimple_stmt_iterator gsi = gsi_for_stmt (dom_use);
		  gsi_move_before (&i, &gsi);
		}

	      update_stmt (stmt);
	    }
	}
      else
	gsi_next (&i);
    }
}

/* Initilize checker optimization pass.  */
void
chkp_opt_init (void)
{
  check_infos.create (0);

  calculate_dominance_info (CDI_DOMINATORS);
  calculate_dominance_info (CDI_POST_DOMINATORS);

  /* With LTO constant bounds vars may be not initialized by now.
     Get constant bounds vars to handle their assignments during
     optimizations.  */
  chkp_get_zero_bounds_var ();
  chkp_get_none_bounds_var ();
}

/* Finalise checker optimization  pass.  */
void
chkp_opt_fini (void)
{
  chkp_fix_cfg ();

  free_dominance_info (CDI_DOMINATORS);
  free_dominance_info (CDI_POST_DOMINATORS);
}

/* Checker optimization pass function.  */
unsigned int
chkp_opt_execute (void)
{
  chkp_opt_init();

  /* This optimization may introduce new checks
     and thus we put it before checks search.  */
  chkp_optimize_string_function_calls ();

  chkp_gather_checks_info ();

  chkp_remove_excess_intersections ();

  chkp_remove_constant_checks ();

  chkp_remove_redundant_checks ();

  chkp_reduce_bounds_lifetime ();

  chkp_release_check_info ();

  chkp_opt_fini ();

  return 0;
}

/* Pass gate.  */
bool
chkp_opt_gate (void)
{
  return chkp_function_instrumented_p (cfun->decl)
    && (flag_chkp_optimize > 0
	|| (flag_chkp_optimize == -1 && optimize > 0));
}

namespace {

const pass_data pass_data_chkp_opt =
{
  GIMPLE_PASS, /* type */
  "chkpopt", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  true, /* gate */
  true, /* execute */
  TV_NONE, /* tv_id */
  PROP_ssa | PROP_cfg, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  TODO_verify_flow | TODO_verify_stmts
  | TODO_update_ssa /* todo_flags_finish */
};

const pass_data pass_data_chkp =
{
  GIMPLE_PASS, /* type */
  "chkp", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  true, /* gate */
  true, /* execute */
  TV_NONE, /* tv_id */
  PROP_ssa | PROP_cfg, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  TODO_verify_flow | TODO_verify_stmts
  | TODO_update_ssa /* todo_flags_finish */
};

const pass_data pass_data_ipa_chkp_versioning =
{
  SIMPLE_IPA_PASS, /* type */
  "chkp_versioning", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  true, /* gate */
  true, /* execute */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0 /* todo_flags_finish */
};

const pass_data pass_data_ipa_chkp_produce_thunks =
{
  SIMPLE_IPA_PASS, /* type */
  "chkp_cleanup", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  false, /* gate */
  true, /* execute */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0 /* todo_flags_finish */
};

class pass_chkp : public gimple_opt_pass
{
public:
  pass_chkp (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_chkp, ctxt)
  {}

  /* opt_pass methods: */
  opt_pass * clone () { return new pass_chkp (m_ctxt); }
  bool gate () { return chkp_gate (); }
  unsigned int execute () { return chkp_execute (); }

}; // class pass_chkp

class pass_ipa_chkp_versioning : public simple_ipa_opt_pass
{
public:
  pass_ipa_chkp_versioning (gcc::context *ctxt)
    : simple_ipa_opt_pass (pass_data_ipa_chkp_versioning, ctxt)
  {}

  /* opt_pass methods: */
  opt_pass * clone () { return new pass_ipa_chkp_versioning (m_ctxt); }
  bool gate () { return flag_check_pointer_bounds; }
  unsigned int execute () { return chkp_versioning (); }

}; // class pass_ipa_chkp_versioning

class pass_ipa_chkp_produce_thunks : public simple_ipa_opt_pass
{
public:
  pass_ipa_chkp_produce_thunks (gcc::context *ctxt)
    : simple_ipa_opt_pass (pass_data_ipa_chkp_produce_thunks, ctxt)
  {}

  /* opt_pass methods: */
  opt_pass * clone () { return new pass_ipa_chkp_produce_thunks (m_ctxt); }
  unsigned int execute () { return chkp_produce_thunks (); }

}; // class pass_chkp_produce_thunks

class pass_chkp_opt : public gimple_opt_pass
{
public:
  pass_chkp_opt (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_chkp_opt, ctxt)
  {}

  /* opt_pass methods: */
  opt_pass * clone () { return new pass_chkp_opt (m_ctxt); }
  bool gate () { return chkp_opt_gate (); }
  unsigned int execute () { return chkp_opt_execute (); }

}; // class pass_chkp_opt

} // anon namespace

gimple_opt_pass *
make_pass_chkp (gcc::context *ctxt)
{
  return new pass_chkp (ctxt);
}

simple_ipa_opt_pass *
make_pass_ipa_chkp_versioning (gcc::context *ctxt)
{
  return new pass_ipa_chkp_versioning (ctxt);
}

simple_ipa_opt_pass *
make_pass_ipa_chkp_produce_thunks (gcc::context *ctxt)
{
  return new pass_ipa_chkp_produce_thunks (ctxt);
}

gimple_opt_pass *
make_pass_chkp_opt (gcc::context *ctxt)
{
  return new pass_chkp_opt (ctxt);
}

#include "gt-tree-chkp.h"
