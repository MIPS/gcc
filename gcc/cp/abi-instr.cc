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
#include "cp-tree.h"
#include "abg-writer.h"
#include "abg-config.h"
#include "abi-instr.h"

using std::tr1::shared_ptr;
using std::tr1::unordered_map;
using std::string;
using std::list;

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
static shared_ptr <abigail::scope_decl> gen_scope_decl (const_tree,
							shared_ptr <abigail::scope_decl>);
static shared_ptr<abigail::type_base> gen_type_in_scope (const_tree,
							 shared_ptr <abigail::scope_decl>);
static shared_ptr<abigail::type_base> gen_type (const_tree);
static shared_ptr<abigail::decl_base> gen_decl_in_scope (const_tree,
							shared_ptr <abigail::scope_decl>);
static shared_ptr<abigail::decl_base> gen_decl(const_tree);

// Deallocate the dynamic memory used by globals in this translation unit.
static void
deallocate_stuff ()
{
  delete tu; tu = 0;
  delete tree_2_scope_map; tree_2_scope_map = 0;
  delete conf; conf = 0;
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
      if (TYPE_P (t))
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

// Generate (if necessary) and return an instance of libabigail decl
// for T that is supposed to be a GCC decl tree.  The resulting
// instance of libabigail decl is added to the scope SCOPE.
static shared_ptr <abigail::decl_base>
gen_decl_in_scope (const_tree t,
		   shared_ptr <abigail::scope_decl> scope)
{
  if (t == NULL_TREE || !DECL_P (t))
    return shared_ptr <abigail::decl_base> ();

  switch (TREE_CODE (t))
    {
    case VAR_DECL:
      {
	shared_ptr <abigail::type_base> type = gen_type (TREE_TYPE (t));
	if (!type)
	  // FIXME: we should log that we do not generate the decl as
	  // we cannot generate its type.
	  return shared_ptr <abigail::decl_base> ();

	shared_ptr<abigail::var_decl> v
	  (new abigail::var_decl (IDENTIFIER_POINTER (DECL_NAME (t)),
				  type, get_location (t),
				  IDENTIFIER_POINTER
				  (DECL_ASSEMBLER_NAME (CONST_CAST_TREE (t))),
				  get_decl_visibility (t),
				  get_decl_binding (t)));
	add_decl_to_scope (v, scope);
	return v;
      }
    case NAMESPACE_DECL:
      {
	shared_ptr<abigail::namespace_decl> n
	  (new abigail::namespace_decl (IDENTIFIER_POINTER (DECL_NAME (t)),
					get_location (t),
					get_decl_visibility (t)));
	add_decl_to_scope (n, scope);
	return n;
      }
    default:
      break;
    }
  return shared_ptr <abigail::decl_base> ();
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

  switch (TREE_CODE (t))
    {
    case BOOLEAN_TYPE:
    case INTEGER_TYPE:
    case NULLPTR_TYPE:
    case VOID_TYPE:
      {
	abigail::location aloc = get_location (t);

	shared_ptr<abigail::type_decl> type_declaration
	  (new abigail::type_decl (IDENTIFIER_POINTER (TYPE_IDENTIFIER (t)),
				   int_cst_value (TYPE_SIZE (t)),
				   TYPE_ALIGN (t), aloc,
				   IDENTIFIER_POINTER
				   (DECL_ASSEMBLER_NAME (TYPE_NAME (t))),
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
	    (abigail::enum_type_decl::enumerator (IDENTIFIER_POINTER
						  (TREE_PURPOSE (e)),
						  int_cst_value
						  (DECL_INITIAL
						   (TREE_VALUE (e)))));

	abigail::location loc = get_location (t);

	shared_ptr <abigail::enum_type_decl> enum_type_decl
	  (new abigail::enum_type_decl (IDENTIFIER_POINTER
					(TYPE_IDENTIFIER (t)),
					loc, underlying_type, enumerators,
					IDENTIFIER_POINTER
					(DECL_ASSEMBLER_NAME (TYPE_NAME (t))),
					get_decl_visibility (TYPE_NAME (t))));

	add_decl_to_scope (enum_type_decl, scope);
	result = enum_type_decl;
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
      if (!gen_decl (var))
	return false;
    }
  return true;
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
