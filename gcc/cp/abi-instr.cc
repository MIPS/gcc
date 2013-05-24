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

struct tree_hash
{
  size_t
  operator () (const_tree t) const
  {
    if (t == NULL_TREE)
      return 0;
    if (TYPE_P (t))
      return TYPE_HASH (t);
    return iterative_hash_expr (t, 0);
  }
};


struct tree_equal
{
  bool
  operator () (const_tree t0, const_tree t1) const
  {
    return operand_equal_p (t0, t1, 0);
  }
};

typedef unordered_map <const_tree,
		       shared_ptr <abigail::scope_decl>,
		       tree_hash, tree_equal> tree_scope_decl_map;

static shared_ptr <abigail::translation_unit> tu;
static shared_ptr<abigail::config> conf;
static shared_ptr <tree_scope_decl_map> tree_2_scope_map;

static void
deallocate_stuff ()
{
  tu.reset ();
  tree_2_scope_map.reset ();
  conf.reset ();
}

void
abi_instr_init ()
{
}

void
abi_instr_finish ()
{
  abi_instr_dump_file ();
  deallocate_stuff ();
}

static abigail::translation_unit&
get_cur_tu ()
{
  if (!tu)
    tu.reset (new abigail::translation_unit (main_input_filename));

  return *tu;
}

static const abigail::config&
get_conf ()
{
  if (!conf)
    conf.reset (new abigail::config);

  return *conf;
}

static tree_scope_decl_map&
get_tree_2_scope_map ()
{
  if (!tree_2_scope_map)
    tree_2_scope_map.reset (new tree_scope_decl_map);

  return *tree_2_scope_map;
}

static shared_ptr<abigail::scope_decl>
get_scope (const_tree t)
{
  shared_ptr<abigail::scope_decl> nil;
  if (t == NULL_TREE)
    return nil;

  tree_scope_decl_map& s = get_tree_2_scope_map ();
  tree_scope_decl_map::const_iterator i = s.find (t);
  if (i == s.end ())
    {
      if ((TYPE_P (t) && TYPE_FILE_SCOPE_P (t))
	  || (DECL_P (t) && DECL_FILE_SCOPE_P (t)))
	return get_cur_tu ().get_global_scope ();
      return shared_ptr<abigail::scope_decl> ();
    }
  return i->second;
}

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

static abigail::location
convert_location (source_location l)
{
  expanded_location e = expand_location (l);
  return get_cur_tu ().get_loc_mgr ().create_new_location (e.file,
							   e.line,
							   e.column);
}

static abigail::decl_base::visibility
decl_visibility (const_tree decl)
{
  if (decl == NULL_TREE)
    return abigail::decl_base::VISIBILITY_NONE;
  gcc_assert (DECL_P (decl));
  return convert_visibility (DECL_VISIBILITY (decl));
}

static abigail::location
tree_location (const_tree t)
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

static shared_ptr<abigail::type_decl>
gen_basic_type_in_scope (const_tree t,
			 shared_ptr<abigail::scope_decl> scope)
{
  if (t == NULL_TREE || !TYPE_P (t))
    return shared_ptr<abigail::type_decl> ();

  switch (TREE_CODE (t))
    {
    case BOOLEAN_TYPE:
    case INTEGER_TYPE:
    case NULLPTR_TYPE:
    case VOID_TYPE:
      {
	abigail::location aloc = tree_location (t);

	shared_ptr<abigail::type_decl> type_declaration
	  (new abigail::type_decl (IDENTIFIER_POINTER
				   (DECL_NAME (TYPE_NAME (t))),
				   int_cst_value (TYPE_SIZE (t)),
				   TYPE_ALIGN (t), aloc,
				   IDENTIFIER_POINTER
				   (DECL_ASSEMBLER_NAME (TYPE_NAME (t))),
				   decl_visibility (TYPE_NAME (t))));

	add_decl_to_scope (type_declaration, scope);
	return type_declaration;
      }
    default:
      break;
    }
  return shared_ptr<abigail::type_decl> ();
}

static shared_ptr<abigail::type_decl>
gen_type_decl (const_tree t)
{
  shared_ptr<abigail::scope_decl> scope = get_scope (t);
  if (scope)
    return gen_basic_type_in_scope (t, scope);
  /* FIXME: if scope is null, then generate abi instr for the
     scope.  */
  return shared_ptr<abigail::type_decl> ();
}

/* Build a representation of type T, in libabigail.  Return true iff
   the type was emitted.  */
bool
abi_instr_emit_type (const_tree t)
{
  gcc_assert (TYPE_P (t));

  return gen_type_decl (t);
}

bool
abi_instr_emit_vars (tree *vars, int len)
{
  for (int i = 0; i < len; ++i)
    {
      tree var = vars[i];
      shared_ptr<abigail::type_decl> t = gen_type_decl (TREE_TYPE (var));
      abigail::location loc = tree_location (var);

      shared_ptr<abigail::var_decl> v
	(new abigail::var_decl (IDENTIFIER_POINTER
				(DECL_NAME (var)),
				t, loc,
				IDENTIFIER_POINTER
				(DECL_ASSEMBLER_NAME (var)),
				decl_visibility (var),
				get_decl_binding (var)));
      add_decl_to_scope (v, get_scope (var));
    }
  return true;
}

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
