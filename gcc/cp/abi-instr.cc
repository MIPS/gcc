/* ABI Instrumentation
   Copyright (C) 2000-2013 Free Software Foundation, Inc.
   Written by Dodji Seketeli <dodji@redhat.com>

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"

#ifdef HAVE_abigail

#include "system.h"
#include "coretypes.h"
#include <map>
#include <tr1/unordered_map>
#include <tr1/memory>
#include <string>
#include <fstream>
#include "cgraph.h"
#include "cp-tree.h"
#include "abg-writer.h"
#include "abg-config.h"
#include "abi-instr.h"

using std::tr1::shared_ptr;
using std::tr1::static_pointer_cast;
using std::tr1::unordered_map;
using std::string;
using std::list;
using std::vector;

struct type_or_decl_hash
{
  size_t
  operator () (const_tree t) const
  {
    if (t == NULL_TREE)
      return 0;
    if (TYPE_P (t))
      return TYPE_HASH (t);

    char tclass = TREE_CODE_CLASS (TREE_CODE (t));
    gcc_assert (tclass == tcc_declaration);
    return iterative_hash_expr (t, 0);
  }
};

struct type_or_decl_equal
{
  bool
  operator () (const_tree t0, const_tree t1) const
  {
    if (TYPE_P (t0))
      return same_type_p (CONST_CAST_TREE (t0), CONST_CAST_TREE (t1));
    return operand_equal_p (t0, t1, 0);
  }
};
typedef unordered_map <const_tree,
		       shared_ptr <abigail::class_decl>,
		       type_or_decl_hash,
		       type_or_decl_equal> classes_wip_map;

typedef unordered_map <const_tree,
		       shared_ptr <abigail::scope_decl>,
		       type_or_decl_hash, type_or_decl_equal> scope_map;

typedef unordered_map <const_tree,
		       shared_ptr <abigail::type_base>,
		       type_or_decl_hash,
		       type_or_decl_equal>  type_map;

typedef unordered_map <const_tree,
		       shared_ptr <abigail::decl_base>,
		       type_or_decl_hash,
		       type_or_decl_equal> decl_map;

static abigail::translation_unit	*tu;
static abigail::config			*conf;
static scope_map			*tree_2_scope_map;
static classes_wip_map			*wip_classes_map;
static type_map			*tree_2_type_map;
static decl_map			*tree_2_decl_map;

static void deallocate_stuff ();
static abigail::translation_unit& get_cur_tu ();
static const abigail::config& get_conf ();
static scope_map& get_tree_2_scope_map ();
static type_map& get_tree_2_type_map ();
static shared_ptr<abigail::scope_decl> gen_scope_of (const_tree );
static abigail::decl_base::visibility convert_visibility (symbol_visibility);
static abigail::location convert_location (source_location);
static abigail::decl_base::visibility get_decl_visibility (const_tree);
static abigail::location get_location (const_tree);
static abigail::decl_base::binding get_decl_binding (const_tree);
static string get_tree_name (const_tree);
static string get_mangled_name (const_tree);
static size_t get_int_constant_value (const_tree);
static abigail::class_decl::access_specifier get_access (const_tree);
static bool should_instr_function (const_tree);
static shared_ptr <abigail::scope_decl> gen_scope_decl
(const_tree, shared_ptr <abigail::scope_decl>);
static shared_ptr <abigail::function_type> gen_function_type
(const_tree t,
 shared_ptr <abigail::class_decl> base_type =
 shared_ptr<abigail::class_decl> ());
static shared_ptr <abigail::function_decl> gen_function_decl
(const_tree t,
 shared_ptr<abigail::class_decl> base_type =
 shared_ptr<abigail::class_decl> ());
static shared_ptr<abigail::class_decl::data_member> gen_data_member
(const_tree);
static shared_ptr<abigail::class_decl::member_function> gen_member_function
(const_tree, shared_ptr<abigail::class_decl>);
static shared_ptr<abigail::type_base> gen_type_in_scope
(const_tree, shared_ptr <abigail::scope_decl>);
static shared_ptr<abigail::type_base> gen_type (const_tree);
static shared_ptr<abigail::decl_base> gen_decl_in_scope
(const_tree, shared_ptr <abigail::scope_decl>);
static shared_ptr<abigail::decl_base> gen_decl(const_tree);

// Deallocate the dynamic memory used by globals in this translation unit.
static void
deallocate_stuff ()
{
  delete tu; tu = 0;
  delete tree_2_scope_map; tree_2_scope_map = 0;
  delete conf; conf = 0;
  delete wip_classes_map; wip_classes_map = 0;
  delete tree_2_type_map; tree_2_type_map = 0;
  delete tree_2_decl_map; tree_2_decl_map = 0;
}

// Returns the current translation unit.  This function allocates it,
// if necessary.
static abigail::translation_unit&
get_cur_tu ()
{
  if (!tu)
    tu = new abigail::translation_unit (main_input_filename);

  return *tu;
}

// Returns the instance of configuration type used by the libabigail
// library.
static const abigail::config&
get_conf ()
{
  if (!conf)
    conf = new abigail::config;

  return *conf;
}

// Allocates (if necessary) and return the map that associates trees
// representing a scope and their matching instance of libabigail
// scope.
static scope_map&
get_tree_2_scope_map ()
{
  if (!tree_2_scope_map)
    tree_2_scope_map = new scope_map;

  return *tree_2_scope_map;
}

// Allocates (if necessary) and return the map that associates trees
// representing a type and their matching instance of libabigail type.
static type_map&
get_tree_2_type_map ()
{
  if (!tree_2_type_map)
    tree_2_type_map = new type_map;

  return *tree_2_type_map;
}

// Allocates (if necessary) and return the map that associates trees
// representing a class that is currently being instrumented, and the
// matching abigail class_decl.
static classes_wip_map&
get_wip_classes_map ()
{
  if (!wip_classes_map)
    wip_classes_map = new classes_wip_map;

  return *wip_classes_map;
}

// Generate (if necessary) and return an instance of libabigail scope
// type representing the scope of T.
static shared_ptr<abigail::scope_decl>
gen_scope_of (const_tree t)
{
  shared_ptr<abigail::scope_decl> result;
  if (t == NULL_TREE)
    return result;

  tree context = NULL_TREE;
  if (TYPE_P (t))
    context = TYPE_CONTEXT (t);
  else if (DECL_P (t))
    context = DECL_CONTEXT (t);

  if (SCOPE_FILE_SCOPE_P (context))
    return get_cur_tu ().get_global_scope ();

  gcc_assert (TREE_CODE (context) == NAMESPACE_DECL
	      || TYPE_P (context));

  scope_map& m = get_tree_2_scope_map ();
  scope_map::const_iterator i = m.find (context);
  if (i != m.end ())
    return i->second;

  result = gen_scope_decl (context, gen_scope_of (context));

  return result;
}

// Convert a symbol_visibility into a libabigail visibility.
static abigail::decl_base::visibility
convert_visibility (symbol_visibility v)
{
  switch (v)
    {
    case VISIBILITY_DEFAULT:
      return abigail::decl_base::VISIBILITY_DEFAULT;
    case VISIBILITY_PROTECTED:
      return abigail::decl_base::VISIBILITY_PROTECTED;
    case VISIBILITY_HIDDEN:
      return abigail::decl_base::VISIBILITY_HIDDEN;
    case VISIBILITY_INTERNAL:
      return abigail::decl_base::VISIBILITY_INTERNAL;
    }
  return abigail::decl_base::VISIBILITY_NONE;
}

// Convert a GCC source location into an instance of libabigail
// location.
static abigail::location
convert_location (source_location l)
{
  if (l <= BUILTINS_LOCATION)
    return abigail::location ();

  expanded_location e = expand_location (l);
  return get_cur_tu ().get_loc_mgr ().create_new_location (e.file,
							   e.line,
							   e.column);
}

// Return a libabigail visibility of the GCC DECL.
static abigail::decl_base::visibility
get_decl_visibility (const_tree decl)
{
  if (decl == NULL_TREE)
    return abigail::decl_base::VISIBILITY_NONE;
  gcc_assert (DECL_P (decl));
  return convert_visibility (DECL_VISIBILITY (decl));
}

// Return the libabigail location of T.
static abigail::location
get_location (const_tree t)
{
  if (t != NULL_TREE)
    {
      if (DECL_P (t))
	return convert_location (DECL_SOURCE_LOCATION (t));
      if (TYPE_P (t) && TYPE_NAME (t))
	return convert_location (DECL_SOURCE_LOCATION (TYPE_NAME (t)));
      if (EXPR_P (t) && EXPR_HAS_LOCATION (t))
	return convert_location (EXPR_LOCATION (t));
    }
  return abigail::location ();
}

// Return the libabigail binding of DECL.
static abigail::decl_base::binding
get_decl_binding (const_tree decl)
{
  if (decl != NULL)
    {
      gcc_assert (DECL_P (decl));

      if (DECL_WEAK (decl))
	return abigail::decl_base::BINDING_WEAK;
      if ((!TYPE_P (CP_DECL_CONTEXT (decl)) && !DECL_NAMESPACE_SCOPE_P (decl))
	  || TREE_CODE (decl) == PARM_DECL)
	return abigail::decl_base::BINDING_LOCAL;
      return abigail::decl_base::BINDING_GLOBAL;
    }
  return abigail::decl_base::BINDING_NONE;
}

// Return the name of the type, decl, or identifier represented by T.
static string
get_tree_name (const_tree t)
{
  string identifier;
  if (t == NULL_TREE || t == error_mark_node)
    identifier = "";
  else if (TREE_CODE (t) == IDENTIFIER_NODE)
    identifier.assign(IDENTIFIER_POINTER (t), IDENTIFIER_LENGTH(t));
  else if (DECL_P (t))
    identifier = get_tree_name (DECL_NAME (t));
  else if (TYPE_P (t))
    identifier = get_tree_name (TYPE_IDENTIFIER (t));
  else
    identifier = "";

  return identifier;
}

// Return the mangled name of T in the string NAME.
// Note that T might be a decl or a type.
static string
get_mangled_name (const_tree t)
{
  string name;
  if (!t || t == error_mark_node)
    return name;

  if (TYPE_P (t))
    t = TYPE_NAME (t);

  if (!DECL_P (t))
    return name;

  name = get_tree_name (DECL_ASSEMBLER_NAME (CONST_CAST_TREE (t)));
  return name;
}

// Return the value of an INTEGER_CST tree.
//
// Note that this returns 0 if T is NULL.
static size_t
get_int_constant_value (const_tree t)
{
  if (t == NULL_TREE)
    return 0;

  gcc_assert (TREE_CODE (t) == INTEGER_CST);

  return int_cst_value (t);
}

// Return the libabigail representation of the access specifier for
// the tree T.
static abigail::class_decl::access_specifier
get_access(const_tree t)
{
  abigail::class_decl::access_specifier result =
    abigail::class_decl::no_access;
  if (t == NULL_TREE)
    ;
  else if (TREE_PROTECTED (t))
    result = abigail::class_decl::protected_access;
  else if (TREE_PRIVATE (t))
    result = abigail::class_decl::private_access;
  else
    result = abigail::class_decl::public_access;

  return result;
}

// Return true iff the function FN should be instrumented.
static bool
should_instr_function (const_tree fn)
{
  bool result = false;

  if (fn
      && TREE_CODE (fn) == FUNCTION_DECL
      && TREE_PUBLIC (fn)
      && TREE_ASM_WRITTEN (fn)
      // && !DECL_ABSTRACT (fn)
      && !DECL_ARTIFICIAL (fn))
    result = true;

  return result;
}

// Generate a libabigail representation of the data member represented
// by T.  T must be either a VAR_DECL or a FIELD_DECL.
static shared_ptr<abigail::class_decl::data_member>
gen_data_member (const_tree t)
{
  shared_ptr<abigail::class_decl::data_member> result;

  if (t == NULL_TREE
      || DECL_ARTIFICIAL (t)
      || (DECL_P (t) && DECL_NAME (t) == NULL_TREE))
    return result;

  shared_ptr<abigail::type_base> type = gen_type (TREE_TYPE (t));
  if (!type)
    // FIXME: log the that we couldn't generate a type for this.
    return result;

  if (TREE_CODE (t) == FIELD_DECL)
    {
      shared_ptr<abigail::class_decl::data_member> m
	(new abigail::class_decl::data_member
	 (get_tree_name (t), type, get_access (t), get_location (t),
	  /*mangled_name=*/"", abigail::decl_base::VISIBILITY_DEFAULT,
	  abigail::decl_base::BINDING_NONE, !!DECL_SIZE (t),
	  /*is_static=*/false,
	  int_bit_position (t)));
      result = m;
    }
    else if (TREE_CODE (t) == VAR_DECL)
      {
	shared_ptr<abigail::class_decl::data_member> m
	  (new abigail::class_decl::data_member
	   (get_tree_name (t), type, get_access (t), get_location (t),
	    get_mangled_name (t), get_decl_visibility (t),
	    get_decl_binding (t), !!DECL_SIZE (t),
	    /*is_static=*/true,
	    /*offset_in_bits*/0));
	result = m;
      }

  return result;
}

// Generate the libabigail representation of a member function
// represented by T.  T must be a FUNTION_DECL.
static shared_ptr<abigail::class_decl::member_function>
gen_member_function (const_tree t,
		     shared_ptr <abigail::class_decl> base_type)
{
  shared_ptr<abigail::class_decl::member_function> result;

  if (t == NULL_TREE
      || TREE_CODE (t) != FUNCTION_DECL
      || TREE_CODE (TREE_TYPE (t)) != METHOD_TYPE)
    return result;

  shared_ptr<abigail::function_decl> fn_decl = gen_function_decl (t, base_type);

  result.reset (new abigail::class_decl::member_function
		(fn_decl, get_access (t),
		 get_int_constant_value (DECL_VINDEX (t)),
		 /*is_static=*/false, DECL_CONSTRUCTOR_P (t),
		 DECL_DESTRUCTOR_P (t), DECL_CONST_MEMFUNC_P (t)));

  return result;
}

// Generate (if necessary) and return an instance of the libabigail
// scope decl type, for T that is supposed to be a GCC scope tree;
// that is either a namespace or a class.  The resulting scope is
// itself added to SCOPE.
static shared_ptr <abigail::scope_decl>
gen_scope_decl (const_tree t,
		shared_ptr <abigail::scope_decl> scope)
{
  shared_ptr <abigail::scope_decl> result;
  if (t == NULL_TREE
      || (TREE_CODE (t) != NAMESPACE_DECL
	  && !CLASS_TYPE_P (t)))
      return result;

  if (TREE_CODE (t) == NAMESPACE_DECL)
    {
      result.reset
	(new abigail::namespace_decl (IDENTIFIER_POINTER (DECL_NAME (t)),
				      get_location (t),
				      get_decl_visibility (t)));
      add_decl_to_scope (result, scope);
    }
  else
    {
      // FIXME: Generate class declaration here.
    }
  return result;
}

// Generate a libabigail representation of a function type (or a
// method type) tree represented by T.
//
// T must be a FUNCTION_TYPE or a METHOD_TYPE.
//
// For a method type, BASE_TYPE is the type of the class the resulting
// function is a member of.  Otherwise, BASE_TYPE is null.
static shared_ptr <abigail::function_type>
gen_function_type (const_tree t,
		   shared_ptr <abigail::class_decl> base_type)
{
  shared_ptr <abigail::function_type> result;

  if (t == NULL_TREE || (TREE_CODE (t) != FUNCTION_TYPE
			 && TREE_CODE (t) != METHOD_TYPE))
    return result;

  if (TREE_CODE (t) == METHOD_TYPE && !base_type)
    return result;

  vector<shared_ptr <abigail::function_decl::parameter> > parms;
  tree parm_desc = TYPE_ARG_TYPES (t);
  if (TREE_CODE (t) == METHOD_TYPE)
    parm_desc = TREE_CHAIN (parm_desc);
  for (parm_desc = TYPE_ARG_TYPES (t);
       parm_desc;
       parm_desc = TREE_CHAIN (parm_desc))
    {
      if (parm_desc == void_list_node)
	break;

      shared_ptr <abigail::type_base> parm_type =
	gen_type (TREE_VALUE (parm_desc));

      shared_ptr <abigail::function_decl::parameter> parm
	(new abigail::function_decl::parameter (parm_type, "",
						abigail::location()));
      parms.push_back (parm);
    }

  // A list of parameters not ending with an ellipsis must end
  // with void_list_node.
  if (!parm_desc)
    {
      shared_ptr <abigail::type_base> empty_type;
      shared_ptr <abigail::function_decl::parameter> variadic_marker_parm
	(new abigail::function_decl::parameter
	 (empty_type, "", abigail::location(),
	  /* variadic_marker=*/true));
      parms.push_back (variadic_marker_parm);
    }

  shared_ptr <abigail::type_base> return_type = gen_type (TREE_TYPE (t));

  if (TREE_CODE (t) == FUNCTION_TYPE)
    result.reset
      (new abigail::function_type (return_type, parms,
				   get_int_constant_value (TYPE_SIZE (t)),
				   TYPE_ALIGN (t)));
  else if (TREE_CODE (t)== METHOD_TYPE)
    result.reset
      (new abigail::method_type (return_type,
				 base_type,
				 parms,
				 get_int_constant_value (TYPE_SIZE (t)),
				 TYPE_ALIGN (t)));

  return result;
}

// Generate a libabigail representation of a function decl.
//
// T must be a FUNTION_DECL.
//
// For a method decl, BASE_TYPE is the type of the class the return
// function_decl is a method of.  Otherwise BASE_TYPE is NULL.
static shared_ptr <abigail::function_decl>
gen_function_decl(const_tree t, shared_ptr <abigail::class_decl> base_type)
{
  shared_ptr <abigail::function_decl> result;

  if (t == NULL_TREE || TREE_CODE (t) != FUNCTION_DECL)
    return result;

  shared_ptr <abigail::type_base> fn_type =
    gen_function_type (TREE_TYPE (t), base_type);
  if (!fn_type)
    return result;

  if (DECL_NONSTATIC_MEMBER_FUNCTION_P (t))
    result.reset (new abigail::class_decl::method_decl
		  (get_tree_name (t), fn_type,
		   DECL_DECLARED_INLINE_P (t),
		   get_location (t),
		   get_mangled_name (t),
		   get_decl_visibility (t),
		   get_decl_binding (t)));
  else
    result.reset (new abigail::function_decl
		  (get_tree_name (t), fn_type,
		   DECL_DECLARED_INLINE_P (t),
		   get_location (t),
		   get_mangled_name (t),
		   get_decl_visibility (t),
		   get_decl_binding (t)));

  return result;
}


// Generate (if necessary) and return an instance of libabigail decl
// for T that is supposed to be a GCC decl tree.  The resulting
// instance of libabigail decl is added to the scope SCOPE.
static shared_ptr <abigail::decl_base>
gen_decl_in_scope (const_tree t,
		   shared_ptr <abigail::scope_decl> scope)
{
  shared_ptr <abigail::decl_base> result;
  if (t == NULL_TREE || !DECL_P (t))
    return result;

  switch (TREE_CODE (t))
    {
    case VAR_DECL:
      {
	shared_ptr <abigail::type_base> type = gen_type (TREE_TYPE (t));
	if (!type)
	  // FIXME: we should log that we do not generate the decl as
	  // we cannot generate its type.
	  break;

	shared_ptr<abigail::var_decl> v
	  (new abigail::var_decl (IDENTIFIER_POINTER (DECL_NAME (t)),
				  type, get_location (t),
				  IDENTIFIER_POINTER
				  (DECL_ASSEMBLER_NAME (CONST_CAST_TREE (t))),
				  get_decl_visibility (t),
				  get_decl_binding (t)));
	add_decl_to_scope (v, scope);
	result = v;
      }
      break;

    case FIELD_DECL:
      // This should be handled by gen_data_member called from
      // inside gen_type_in_scope for the RECORD_TYPE case
      gcc_unreachable ();

    case NAMESPACE_DECL:
      {
	shared_ptr<abigail::namespace_decl> n
	  (new abigail::namespace_decl (IDENTIFIER_POINTER (DECL_NAME (t)),
					get_location (t),
					get_decl_visibility (t)));
	add_decl_to_scope (n, scope);
	result = n;
      }
      break;

    case FUNCTION_DECL:
      {
	shared_ptr <abigail::function_decl> fn_decl = gen_function_decl (t);
	if (fn_decl)
	  {
	    add_decl_to_scope (fn_decl, scope);
	    result = fn_decl;
	  }
      }
      break;

    default:
      break;
    }
  return result;
}

// Generate (if necessary) and return an instance of libabigail type
// for T that is supposed to be a GCC type tree.  The resulting
// libabigail type is added to the scope SCOPE.
static shared_ptr<abigail::type_base>
gen_type_in_scope (const_tree t,
		   shared_ptr <abigail::scope_decl> scope)
{
  shared_ptr <abigail::type_base> result;
  if (t == NULL_TREE || !TYPE_P (t))
    return result;

  type_map& m = get_tree_2_type_map ();
  type_map::const_iterator i = m.find (t);
  if (i != m.end ())
    return i->second;

  //FIXME: build qualified types.

  switch (TREE_CODE (t))
    {
    case BOOLEAN_TYPE:
    case INTEGER_TYPE:
    case NULLPTR_TYPE:
    case VOID_TYPE:
      {
	shared_ptr<abigail::type_decl> type_declaration
	  (new abigail::type_decl (get_tree_name (t),
				   get_int_constant_value (TYPE_SIZE (t)),
				   TYPE_ALIGN (t), get_location (t),
				   get_mangled_name (t),
				   get_decl_visibility (TYPE_NAME (t))));

	add_decl_to_scope (type_declaration, scope);
	result = type_declaration;
      }
      break;

    case ENUMERAL_TYPE:
      {
	shared_ptr <abigail::type_base> underlying_type;
	if (ENUM_UNDERLYING_TYPE (t))
	  underlying_type = gen_type (ENUM_UNDERLYING_TYPE (t));

	list<abigail::enum_type_decl::enumerator> enumerators;
	for (tree e = TYPE_VALUES (t); e ; e = TREE_CHAIN (e))
	  enumerators.push_back
	    (abigail::enum_type_decl::enumerator (get_tree_name
						  (TREE_PURPOSE (e)),
						  get_int_constant_value
						  (DECL_INITIAL
						   (TREE_VALUE (e)))));

	abigail::location loc = get_location (t);

	shared_ptr <abigail::enum_type_decl> enum_type_decl
	  (new abigail::enum_type_decl (get_tree_name (t),
					loc, underlying_type, enumerators,
					get_mangled_name (t),
					get_decl_visibility (TYPE_NAME (t))));

	add_decl_to_scope (enum_type_decl, scope);
	result = enum_type_decl;
      }
      break;

    case POINTER_TYPE:
    case REFERENCE_TYPE:
      {
	shared_ptr <abigail::type_base> pointed_to = gen_type (TREE_TYPE (t));
	if (pointed_to)
	  {
	    abigail::location loc = get_location (t);
	    if (TREE_CODE (t) == POINTER_TYPE)
	      {
		shared_ptr <abigail::pointer_type_def> pointer_type
		  (new abigail::pointer_type_def
		   (pointed_to,
		    get_int_constant_value (TYPE_SIZE (t)),
		    TYPE_ALIGN (t), loc));

		add_decl_to_scope (pointer_type, scope);
		result = pointer_type;
	      }
	    else
	      {
		shared_ptr <abigail::reference_type_def> reference_type
		  (new abigail::reference_type_def
		   (pointed_to, !TYPE_REF_IS_RVALUE (t),
		    get_int_constant_value (TYPE_SIZE (t)),
		    TYPE_ALIGN (t), loc));

		add_decl_to_scope (reference_type, scope);
		result = reference_type;
	      }
	  }
      }
      break;

    case FUNCTION_TYPE:
      result = gen_function_type (t);
      break;

    case METHOD_TYPE:
      // This must be handled by gen_function_type, called from within
      // gen_type_in_scope.
      gcc_unreachable ();

    case RECORD_TYPE:
      {
	// FIXME: The fact that we are putting the TYPE_MAIN_VARIANT
	// in the map is a hack until we have built support for
	// variant types (typedefs, cv qual'ed type etc)
	classes_wip_map::const_iterator c =
	  get_wip_classes_map ().find(TYPE_MAIN_VARIANT (t));
	if (c != get_wip_classes_map ().end ())
	  {
	    // Someone is referring to this record type but the
	    // type is not defined yet -- it's currently being
	    // defined.  So we let's forward declare it a this point.

	    shared_ptr <abigail::class_decl> class_type
	      (new abigail::class_decl (get_tree_name (t)));
	    add_decl_to_scope (class_type, scope);
	    result = class_type;
	    break;
	  }

	shared_ptr <abigail::class_decl> class_type
	  (new abigail::class_decl (get_tree_name (t),
				    get_int_constant_value (TYPE_SIZE (t)),
				    TYPE_ALIGN (t), get_location (t),
				    get_decl_visibility (TYPE_NAME (t))));

	get_wip_classes_map ()[TYPE_MAIN_VARIANT (t)] = class_type;

	for (tree m = TYPE_FIELDS (t); m; m = DECL_CHAIN (m))
	  {
	    shared_ptr <abigail::class_decl::data_member> member =
	      gen_data_member (m);
	    if (member)
	      class_type->add_data_member (member);
	  }

	for (tree m = TYPE_METHODS (t); m; m = DECL_CHAIN (m))
	  {
	    if (should_instr_function (m))
	      {
		shared_ptr <abigail::class_decl::member_function> member =
		  gen_member_function (m, class_type);
		if (member)
		  class_type->add_member_function (member);
	      }
	  }

	add_decl_to_scope(class_type, scope);
	result = class_type;
	get_wip_classes_map ().erase (t);
      }
      break;

    default:
      break;
    }

  if (result)
    m[t] = result;

  return result;
}

// Generate (if necessary) and return an instance of libabigail decl
// for T that is supposed to be a GCC decl tree.
static shared_ptr<abigail::decl_base>
gen_decl(const_tree t)
{
  shared_ptr <abigail::decl_base> result;
  if (t == NULL_TREE)
    return result;

  // As the decl is going to refer to its type, we want the type to be
  // defined in the serialized output prior to its use.  So try to
  // generate the type first.
  if (!gen_type (TREE_TYPE (t)))
    // FIXME: we should somehow signal that we couldn't generate this
    // type, so we are not going to emit the decl.
    return result;

  shared_ptr <abigail::scope_decl> scope = gen_scope_of (t);
  if (scope)
    result = gen_decl_in_scope (t, scope);

  return result;
}

// Generate (if necessary) and return an instance of libabigail type
// for T that is supposed to be a GCC type tree.
static shared_ptr<abigail::type_base>
gen_type (const_tree t)
{
  shared_ptr<abigail::scope_decl> scope = gen_scope_of (t);
  if (scope)
    return gen_type_in_scope (t, scope);
  return shared_ptr<abigail::type_base> ();
}

// -- The public entry points of this translation unit start here.

// All initialization stuff should go here.
void
abi_instr_init ()
{
}

// Anything that should be done at the very end of the life time of
// this module should go here.
void
abi_instr_finish ()
{
  abi_instr_dump_file ();
  deallocate_stuff ();
}

// Build a libabigail representation of type T and add it to the
// libabigail representation of the current translation unit.  Return
// TRUE upon successful completion, false otherwise.
bool
abi_instr_emit_type (const_tree t)
{
  gcc_assert (TYPE_P (t));

  return gen_type (t);
}

// Build a libabigail representation of the variables in the VARS
// array and add them to the libabigail representation of the current
// translation unit.  Return TRUE upon successful completion, false
// otherwise.
bool
abi_instr_emit_vars (tree *vars, int len)
{
  for (int i = 0; i < len; ++i)
    {
      tree var = vars[i];
      if (var == NULL_TREE || TREE_CODE (var) != VAR_DECL)
	continue;

      if (varpool_node *n = varpool_get_node (var))
	{
	  if (!n->analyzed)
	    continue;
	}
      else
	continue;

      if (!gen_decl (var))
	{
	  ;//FIXME: log that the generation of this variable failed
	   //somehow
	}
    }
  return true;
}

// Build a libabigail representation of the function FN and add it to
// the libabigail repesentation of the current translation unit.
// Return TRUE upon successful completion, false otherwise.
bool
abi_instr_emit_function(const_tree fn)
{
  bool result = false;

  if (should_instr_function (fn))
    result = gen_decl (fn);

  return result;
}
// Serialize the libabigail representation of the current translation
// unit into a file named after the name of the main input file.
void
abi_instr_dump_file ()
{
  if (!tu)
    return;

  string out_path = dump_base_name;
  out_path += get_conf ().get_tu_instr_suffix ();
  std::ofstream of (out_path.c_str (), std::ios_base::trunc);
  if (!of.is_open ())
    fatal_error ("can%'t open %s for writing: %m", out_path.c_str ());

  bool is_ok = abigail::writer::write_to_ostream (get_cur_tu (), of);
  of.close ();
  if (!is_ok)
    error ("could not dump abi instrumentation to file %s: %m",
	   out_path.c_str ());
}
#endif /* HAVE_abigail*/
