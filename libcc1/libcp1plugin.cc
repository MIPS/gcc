/* Library interface to C++ front end.
   Copyright (C) 2014-2016 Free Software Foundation, Inc.

   This file is part of GCC.  As it interacts with GDB through libcc1,
   they all become a single program as regards the GNU GPL's requirements.

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

#include <cc1plugin-config.h>

#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#include "../gcc/config.h"

#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#include "gcc-plugin.h"
#include "system.h"
#include "coretypes.h"
#include "stringpool.h"

#include "gcc-interface.h"
#include "hash-set.h"
#include "machmode.h"
#include "vec.h"
#include "double-int.h"
#include "input.h"
#include "alias.h"
#include "symtab.h"
#include "options.h"
#include "wide-int.h"
#include "inchash.h"
#include "tree.h"
#include "fold-const.h"
#include "stor-layout.h"
#include "cp-tree.h"
#include "toplev.h"
#include "timevar.h"
#include "hash-table.h"
#include "tm.h"
#include "c-family/c-pragma.h"
// #include "c-lang.h"
#include "diagnostic.h"
#include "langhooks.h"
#include "langhooks-def.h"
#include "decl.h"

#include "callbacks.hh"
#include "connection.hh"
#include "marshall-cp.hh"
#include "rpc.hh"

#ifdef __GNUC__
#pragma GCC visibility push(default)
#endif
int plugin_is_GPL_compatible;
#ifdef __GNUC__
#pragma GCC visibility pop
#endif



// This is put into the lang hooks when the plugin starts.

static void
plugin_print_error_function (diagnostic_context *context, const char *file,
			     diagnostic_info *diagnostic)
{
  if (current_function_decl != NULL_TREE
      && DECL_NAME (current_function_decl) != NULL_TREE
      && strcmp (IDENTIFIER_POINTER (DECL_NAME (current_function_decl)),
		 GCC_FE_WRAPPER_FUNCTION) == 0)
    return;
  lhd_print_error_function (context, file, diagnostic);
}



static unsigned long long
convert_out (tree t)
{
  return (unsigned long long) (uintptr_t) t;
}

static tree
convert_in (unsigned long long v)
{
  return (tree) (uintptr_t) v;
}



struct decl_addr_value
{
  tree decl;
  tree address;
};

struct decl_addr_hasher : free_ptr_hash<decl_addr_value>
{
  static inline hashval_t hash (const decl_addr_value *);
  static inline bool equal (const decl_addr_value *, const decl_addr_value *);
};

inline hashval_t
decl_addr_hasher::hash (const decl_addr_value *e)
{
  return DECL_UID (e->decl);
}

inline bool
decl_addr_hasher::equal (const decl_addr_value *p1, const decl_addr_value *p2)
{
  return p1->decl == p2->decl;
}



struct string_hasher : nofree_ptr_hash<const char>
{
  static inline hashval_t hash (const char *s)
  {
    return htab_hash_string (s);
  }

  static inline bool equal (const char *p1, const char *p2)
  {
    return strcmp (p1, p2) == 0;
  }
};



struct plugin_context : public cc1_plugin::connection
{
  plugin_context (int fd);

  // Map decls to addresses.
  hash_table<decl_addr_hasher> address_map;

  // A collection of trees that are preserved for the GC.
  hash_table< nofree_ptr_hash<tree_node> > preserved;

  // File name cache.
  hash_table<string_hasher> file_names;

  // Perform GC marking.
  void mark ();

  // Preserve a tree during the plugin's operation.
  tree preserve (tree t)
  {
    tree_node **slot = preserved.find_slot (t, INSERT);
    *slot = t;
    return t;
  }

  source_location get_source_location (const char *filename,
				       unsigned int line_number)
  {
    if (filename == NULL)
      return UNKNOWN_LOCATION;

    filename = intern_filename (filename);
    linemap_add (line_table, LC_ENTER, false, filename, line_number);
    source_location loc = linemap_line_start (line_table, line_number, 0);
    linemap_add (line_table, LC_LEAVE, false, NULL, 0);
    return loc;
  }

private:

  // Add a file name to FILE_NAMES and return the canonical copy.
  const char *intern_filename (const char *filename)
  {
    const char **slot = file_names.find_slot (filename, INSERT);
    if (*slot == NULL)
      {
	/* The file name must live as long as the line map, which
	   effectively means as long as this compilation.  So, we copy
	   the string here but never free it.  */
	*slot = xstrdup (filename);
      }
    return *slot;
  }
};

static plugin_context *current_context;



plugin_context::plugin_context (int fd)
  : cc1_plugin::connection (fd),
    address_map (30),
    preserved (30),
    file_names (30)
{
}

void
plugin_context::mark ()
{
  for (hash_table<decl_addr_hasher>::iterator it = address_map.begin ();
       it != address_map.end ();
       ++it)
    {
      ggc_mark ((*it)->decl);
      ggc_mark ((*it)->address);
    }

  for (hash_table< nofree_ptr_hash<tree_node> >::iterator
	 it = preserved.begin (); it != preserved.end (); ++it)
    ggc_mark (&*it);
}

static void
plugin_binding_oracle (enum cp_oracle_request kind, tree identifier)
{
  enum gcc_cp_oracle_request request;

  gcc_assert (current_context != NULL);

  switch (kind)
    {
    case CP_ORACLE_SYMBOL:
      request = GCC_CP_ORACLE_SYMBOL;
      break;
    case CP_ORACLE_TAG:
      request = GCC_CP_ORACLE_TAG;
      break;
    case CP_ORACLE_LABEL:
      request = GCC_CP_ORACLE_LABEL;
      break;
    default:
      abort ();
    }

  int ignore;
  cc1_plugin::call (current_context, "binding_oracle", &ignore,
		    request, IDENTIFIER_POINTER (identifier));
}

static void
plugin_pragma_user_expression (cpp_reader *)
{
  cp_binding_oracle = plugin_binding_oracle;

  /* Make the function containing the user expression a global
     friend, so as to bypass access controls in it.  */
  if (at_function_scope_p ())
    add_to_global_friend_list (current_function_decl);
}

static void
plugin_init_extra_pragmas (void *, void *)
{
  c_register_pragma ("GCC", "user_expression", plugin_pragma_user_expression);
}



static decl_addr_value
build_decl_addr_value (tree decl, gcc_address address)
{
  decl_addr_value value = {
    decl,
    build_int_cst_type (ptr_type_node, address)
  };
  return value;
}

static decl_addr_value *
record_decl_address (plugin_context *ctx, decl_addr_value value)
{
  decl_addr_value **slot = ctx->address_map.find_slot (&value, INSERT);
  gcc_assert (*slot == NULL);
  *slot
    = static_cast<decl_addr_value *> (xmalloc (sizeof (decl_addr_value)));
  **slot = value;
  /* We don't want GCC to warn about e.g. static functions
     without a code definition.  */
  TREE_NO_WARNING (value.decl) = 1;
  return *slot;
}

// Maybe rewrite a decl to its address.
static tree
address_rewriter (tree *in, int *walk_subtrees, void *arg)
{
  plugin_context *ctx = (plugin_context *) arg;

  if (!DECL_P (*in) || DECL_NAME (*in) == NULL_TREE)
    return NULL_TREE;

  decl_addr_value value;
  value.decl = *in;
  decl_addr_value *found_value = ctx->address_map.find (&value);
  if (found_value != NULL)
    ;
  else if (HAS_DECL_ASSEMBLER_NAME_P (*in))
    {
      gcc_address address;

      if (!cc1_plugin::call (ctx, "address_oracle", &address,
			     IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (*in))))
	return NULL_TREE;
      if (address == 0)
	return NULL_TREE;

      // Insert the decl into the address map in case it is referenced
      // again.
      value = build_decl_addr_value (value.decl, address);
      found_value = record_decl_address (ctx, value);
    }
  else
    return NULL_TREE;

  if (found_value->address != error_mark_node)
    {
      // We have an address for the decl, so rewrite the tree.
      tree ptr_type = build_pointer_type (TREE_TYPE (*in));
      *in = fold_build1 (INDIRECT_REF, TREE_TYPE (*in),
			 fold_build1 (CONVERT_EXPR, ptr_type,
				      found_value->address));
    }

  *walk_subtrees = 0;

  return NULL_TREE;
}

// When generating code for gdb, we want to be able to use absolute
// addresses to refer to otherwise external objects that gdb knows
// about.  gdb passes in these addresses when building decls, and then
// before gimplification we go through the trees, rewriting uses to
// the equivalent of "*(TYPE *) ADDR".
static void
rewrite_decls_to_addresses (void *function_in, void *)
{
  tree function = (tree) function_in;

  // Do nothing if we're not in gdb.
  if (current_context == NULL)
    return;

  walk_tree (&DECL_SAVED_TREE (function), address_rewriter, current_context,
	     NULL);
}



static inline tree
safe_pushtag (tree name, tree type, tag_scope scope)
{
  void (*save_oracle) (enum cp_oracle_request, tree identifier);

  save_oracle = cp_binding_oracle;
  cp_binding_oracle = NULL;

  tree ret = pushtag (name, type, scope);

  cp_binding_oracle = save_oracle;

  return ret;
}

static inline tree
safe_pushdecl_maybe_friend (tree decl, bool is_friend)
{
  void (*save_oracle) (enum cp_oracle_request, tree identifier);

  save_oracle = cp_binding_oracle;
  cp_binding_oracle = NULL;

  tree ret = pushdecl_maybe_friend (decl, is_friend);

  cp_binding_oracle = save_oracle;

  return ret;
}



int
plugin_push_namespace (cc1_plugin::connection *self,
		       const char *name)
{
  (void)self;

  if (name && !*name)
    push_to_top_level ();
  else
    push_namespace (name ? get_identifier (name) : NULL);

  return 0;
}

int
plugin_pop_namespace (cc1_plugin::connection *self)
{
  (void)self;

  if (toplevel_bindings_p () && current_namespace == global_namespace)
    pop_from_top_level ();
  else
    pop_namespace ();

  return 0;
}

gcc_decl
plugin_get_current_binding_level (cc1_plugin::connection *self)
{
  (void)self;

  tree decl;

  if (at_namespace_scope_p ())
    decl = current_namespace;
  else if (at_function_scope_p ())
    decl = current_function_decl;
  else if (at_class_scope_p ())
    decl = TYPE_NAME (current_class_type);
  else
    gcc_unreachable ();

  return convert_out (decl);
}


gcc_decl
plugin_new_decl (cc1_plugin::connection *self,
		 const char *name,
		 enum gcc_cp_symbol_kind sym_kind,
		 gcc_type sym_type_in,
		 const char *substitution_name,
		 gcc_address address,
		 const char *filename,
		 unsigned int line_number)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  { // FIXME in gdb:
    char *nname = (char*)strrchr (name, ':');
    if (nname)
      name = nname + 1;
    char *nameend = (char*)strchr (name, '(');
    if (nameend)
      {
	*nameend = 0;
	name = strdupa (name);
	*nameend = '(';
      }
  }
  tree identifier = get_identifier (name);
  enum tree_code code;
  tree decl;
  tree sym_type = convert_in (sym_type_in);
  enum gcc_cp_symbol_kind sym_flags;
  sym_flags = (enum gcc_cp_symbol_kind) (sym_kind & GCC_CP_FLAG_MASK);
  sym_kind = (enum gcc_cp_symbol_kind) (sym_kind & GCC_CP_SYMBOL_MASK);

  switch (sym_kind)
    {
    case GCC_CP_SYMBOL_FUNCTION:
      code = FUNCTION_DECL;
      gcc_assert (!(sym_flags & ~GCC_CP_FLAG_MASK_FUNCTION));
      break;

    case GCC_CP_SYMBOL_VARIABLE:
      code = VAR_DECL;
      gcc_assert (!(sym_flags & ~GCC_CP_FLAG_MASK_VARIABLE));
      break;

    case GCC_CP_SYMBOL_TYPEDEF:
      code = TYPE_DECL;
      gcc_assert (!sym_flags);
      break;

    case GCC_CP_SYMBOL_LABEL:
      // FIXME: we aren't ready to handle labels yet.
      // It isn't clear how to translate them properly
      // and in any case a "goto" isn't likely to work.
      gcc_assert (!sym_flags);
      return convert_out (error_mark_node);

      /* FIXME: add GCC_CP_SYMBOL_CLASS.  */

    default:
      abort ();
    }

  /* FIXME: check for a template parameter list scope.  */

  source_location loc = ctx->get_source_location (filename, line_number);
  bool class_member_p = at_class_scope_p ();
  bool ctor = false, dtor = false, assop = false;
  tree_code opcode = ERROR_MARK;

  if (code == FUNCTION_DECL)
    {
      if (sym_flags & GCC_CP_FLAG_SPECIAL_FUNCTION)
	{
#define CHARS2(f,s) (((unsigned char)f << CHAR_BIT) | (unsigned char)s)
	  switch (CHARS2 (name[0], name[1]))
	    {
	    case CHARS2 ('C', '1'): // in-charge constructor
	      identifier = complete_ctor_identifier;
	      ctor = true;
	      break;
	    case CHARS2 ('C', '2'): // not-in-charge constructor
	      identifier = base_ctor_identifier;
	      ctor = true;
	      break;
	    case CHARS2 ('D', '0'): // deleting destructor
	      identifier = deleting_dtor_identifier;
	      dtor = true;
	      break;
	    case CHARS2 ('D', '1'): // in-charge destructor
	      identifier = complete_dtor_identifier;
	      dtor = true;
	      break;
	    case CHARS2 ('D', '2'): // not-in-charge destructor
	      identifier = base_dtor_identifier;
	      dtor = true;
	      break;
	    case CHARS2 ('n', 'w'): // operator new
	      opcode = NEW_EXPR;
	      break;
	    case CHARS2 ('n', 'a'): // operator new[]
	      opcode = VEC_NEW_EXPR;
	      break;
	    case CHARS2 ('d', 'l'): // operator delete
	      opcode = DELETE_EXPR;
	      break;
	    case CHARS2 ('d', 'a'): // operator delete[]
	      opcode = VEC_DELETE_EXPR;
	      break;
	    case CHARS2 ('p', 's'): // operator + (unary)
	      opcode = PLUS_EXPR;
	      break;
	    case CHARS2 ('n', 'g'): // operator - (unary)
	      opcode = MINUS_EXPR;
	      break;
	    case CHARS2 ('a', 'd'): // operator & (unary)
	      opcode = BIT_AND_EXPR;
	      break;
	    case CHARS2 ('d', 'e'): // operator * (unary)
	      opcode = MULT_EXPR;
	      break;
	    case CHARS2 ('c', 'o'): // operator ~
	      opcode = BIT_NOT_EXPR;
	      break;
	    case CHARS2 ('p', 'l'): // operator +
	      opcode = PLUS_EXPR;
	      break;
	    case CHARS2 ('m', 'i'): // operator -
	      opcode = MINUS_EXPR;
	      break;
	    case CHARS2 ('m', 'l'): // operator *
	      opcode = MULT_EXPR;
	      break;
	    case CHARS2 ('d', 'v'): // operator /
	      opcode = TRUNC_DIV_EXPR;
	      break;
	    case CHARS2 ('r', 'm'): // operator %
	      opcode = TRUNC_MOD_EXPR;
	      break;
	    case CHARS2 ('a', 'n'): // operator &
	      opcode = BIT_AND_EXPR;
	      break;
	    case CHARS2 ('o', 'r'): // operator |
	      opcode = BIT_IOR_EXPR;
	      break;
	    case CHARS2 ('e', 'o'): // operator ^
	      opcode = BIT_XOR_EXPR;
	      break;
	    case CHARS2 ('a', 'S'): // operator =
	      opcode = NOP_EXPR;
	      assop = true;
	      break;
	    case CHARS2 ('p', 'L'): // operator +=
	      opcode = PLUS_EXPR;
	      assop = true;
	      break;
	    case CHARS2 ('m', 'I'): // operator -=
	      opcode = MINUS_EXPR;
	      assop = true;
	      break;
	    case CHARS2 ('m', 'L'): // operator *=
	      opcode = MULT_EXPR;
	      assop = true;
	      break;
	    case CHARS2 ('d', 'V'): // operator /=
	      opcode = TRUNC_DIV_EXPR;
	      assop = true;
	      break;
	    case CHARS2 ('r', 'M'): // operator %=
	      opcode = TRUNC_MOD_EXPR;
	      assop = true;
	      break;
	    case CHARS2 ('a', 'N'): // operator &=
	      opcode = BIT_AND_EXPR;
	      assop = true;
	      break;
	    case CHARS2 ('o', 'R'): // operator |=
	      opcode = BIT_IOR_EXPR;
	      assop = true;
	      break;
	    case CHARS2 ('e', 'O'): // operator ^=
	      opcode = BIT_XOR_EXPR;
	      assop = true;
	      break;
	    case CHARS2 ('l', 's'): // operator <<
	      opcode = LSHIFT_EXPR;
	      break;
	    case CHARS2 ('r', 's'): // operator >>
	      opcode = RSHIFT_EXPR;
	      break;
	    case CHARS2 ('l', 'S'): // operator <<=
	      opcode = LSHIFT_EXPR;
	      assop = true;
	      break;
	    case CHARS2 ('r', 'S'): // operator >>=
	      opcode = RSHIFT_EXPR;
	      assop = true;
	      break;
	    case CHARS2 ('e', 'q'): // operator ==
	      opcode = EQ_EXPR;
	      break;
	    case CHARS2 ('n', 'e'): // operator !=
	      opcode = NE_EXPR;
	      break;
	    case CHARS2 ('l', 't'): // operator <
	      opcode = LT_EXPR;
	      break;
	    case CHARS2 ('g', 't'): // operator >
	      opcode = GT_EXPR;
	      break;
	    case CHARS2 ('l', 'e'): // operator <=
	      opcode = LE_EXPR;
	      break;
	    case CHARS2 ('g', 'e'): // operator >=
	      opcode = GE_EXPR;
	      break;
	    case CHARS2 ('n', 't'): // operator !
	      opcode = TRUTH_NOT_EXPR;
	      break;
	    case CHARS2 ('a', 'a'): // operator &&
	      opcode = TRUTH_ANDIF_EXPR;
	      break;
	    case CHARS2 ('o', 'o'): // operator ||
	      opcode = TRUTH_ORIF_EXPR;
	      break;
	    case CHARS2 ('p', 'p'): // operator ++
	      opcode = POSTINCREMENT_EXPR;
	      break;
	    case CHARS2 ('m', 'm'): // operator --
	      opcode = POSTDECREMENT_EXPR;
	      break;
	    case CHARS2 ('c', 'm'): // operator ,
	      opcode = COMPOUND_EXPR;
	      break;
	    case CHARS2 ('p', 'm'): // operator ->*
	      opcode = MEMBER_REF;
	      break;
	    case CHARS2 ('p', 't'): // operator ->
	      opcode = COMPONENT_REF;
	      break;
	    case CHARS2 ('c', 'l'): // operator ()
	      opcode = CALL_EXPR;
	      break;
	    case CHARS2 ('i', 'x'): // operator []
	      opcode = ARRAY_REF;
	      break;
	    case CHARS2 ('c', 'v'): // operator <T> (conversion operator)
	      identifier = mangle_conv_op_name_for_type (TREE_TYPE (sym_type));
	      break;
	      // C++11-only:
	    case CHARS2 ('l', 'i'): // operator "" <id>
	      {
		char *id = (char *)name + 2;
		bool freeid = false;
		if (*id >= '0' && *id <= '9')
		  {
		    unsigned len = 0;
		    do
		      {
			len *= 10;
			len += id[0] - '0';
			id++;
		      }
		    while (*id && *id >= '0' && *id <= '9');
		    id = xstrndup (id, len);
		    freeid = true;
		  }
		identifier = ansi_litopname (id);
		if (freeid)
		  free (id);
	      }
	      break;
	    case CHARS2 ('q', 'u'): // ternary operator, not overloadable.
	    default:
	      gcc_unreachable ();
	    }

	  if (opcode != ERROR_MARK)
	    {
	      if (assop)
		identifier = ansi_assopname (opcode);
	      else
		identifier = ansi_opname (opcode);
	    }
	}
      decl = build_lang_decl_loc (loc, code,
				  (ctor || dtor)
				  ? DECL_NAME (TYPE_NAME (current_class_type))
				  : identifier, sym_type);
      SET_DECL_LANGUAGE (decl, lang_cplusplus); // FIXME: current_lang_name is lang_name_c while compiling an extern "C" function, and we haven't switched to a global context at this point, and this breaks function overloading.
      if (TREE_CODE (sym_type) == METHOD_TYPE)
	DECL_ARGUMENTS (decl) = build_this_parm (current_class_type,
						 cp_type_quals (sym_type));
      for (tree arg = TREE_CODE (sym_type) == METHOD_TYPE
	     ? TREE_CHAIN (TYPE_ARG_TYPES (sym_type))
	     : TYPE_ARG_TYPES (sym_type);
	   arg && arg != void_list_node;
	   arg = TREE_CHAIN (arg))
	{
	  tree parm = cp_build_parm_decl (NULL_TREE, TREE_VALUE (arg));
	  DECL_CHAIN (parm) = DECL_ARGUMENTS (decl);
	  DECL_ARGUMENTS (decl) = parm;
	}
      DECL_ARGUMENTS (decl) = nreverse (DECL_ARGUMENTS (decl));
      if (class_member_p)
	{
	  if (TREE_CODE (sym_type) == FUNCTION_TYPE)
	    DECL_STATIC_FUNCTION_P (decl) = 1;
	  if (sym_flags & GCC_CP_FLAG_VIRTUAL_FUNCTION)
	    {
	      DECL_VIRTUAL_P (decl) = 1;
	      if (sym_flags & GCC_CP_FLAG_PURE_VIRTUAL_FUNCTION)
		DECL_PURE_VIRTUAL_P (decl) = 1;
	      if (sym_flags & GCC_CP_FLAG_FINAL_VIRTUAL_FUNCTION)
		DECL_FINAL_P (decl) = 1;
	    }
	  else
	    gcc_assert (!(sym_flags & (GCC_CP_FLAG_PURE_VIRTUAL_FUNCTION
				       | GCC_CP_FLAG_FINAL_VIRTUAL_FUNCTION)));
	}
      else
	{
	  gcc_assert (!(sym_flags & (GCC_CP_FLAG_VIRTUAL_FUNCTION
				     | GCC_CP_FLAG_PURE_VIRTUAL_FUNCTION
				     | GCC_CP_FLAG_FINAL_VIRTUAL_FUNCTION)));
	  gcc_assert (!ctor && !dtor && !assop);
	}
      if (sym_flags & GCC_CP_FLAG_EXPLICIT_FUNCTION)
	DECL_NONCONVERTING_P (decl) = 1;
      if (sym_flags & GCC_CP_FLAG_DEFAULTED_FUNCTION)
	{
	  DECL_INITIAL (decl) = ridpointers[(int)RID_DEFAULT];
	  DECL_DEFAULTED_FN (decl) = 1;
	}
      if (sym_flags & GCC_CP_FLAG_DELETED_FUNCTION)
	{
	  // DECL_INITIAL (decl) = ridpointers[(int)RID_DELETE];
	  DECL_DELETED_FN (decl) = 1;
	  DECL_DECLARED_INLINE_P (decl) = 1;
	  DECL_INITIAL (decl) = error_mark_node;
	}
      if (ctor || dtor)
	{
	  tree fns = NULL_TREE;
	  if (ctor)
	    {
	      DECL_CONSTRUCTOR_P (decl) = 1;
	      fns = CLASSTYPE_CONSTRUCTORS (current_class_type);
	    }
	  if (dtor)
	    {
	      DECL_DESTRUCTOR_P (decl) = 1;
	      fns = CLASSTYPE_DESTRUCTORS (current_class_type);
	    }
	  DECL_CONTEXT (decl) = FROB_CONTEXT (current_class_type);
	  maybe_retrofit_in_chrg (decl);
	  for (; fns; fns = OVL_NEXT (fns))
	    {
	      tree fn = OVL_CURRENT (fns);
	      if (TREE_TYPE (decl) == TREE_TYPE (fn))
		{
		  decl = fn;
		  break;
		}
	    }
	  if (!fns)
	    {
	      finish_member_declaration (decl);
	      /* ctors and dtors clones are chained after DECL.
		 However, we create the clones before TYPE_METHODS is
		 reversed.  We test for cloned methods after reversal,
		 however, and the test requires the clones to follow
		 DECL.  So, we reverse the chain of clones now, so
		 that it will come out in the right order after
		 reversal.  */
	      tree save = DECL_CHAIN (decl);
	      DECL_CHAIN (decl) = NULL_TREE;
	      clone_function_decl (decl, /*update_method_vec_p=*/1);
	      gcc_assert (TYPE_METHODS (current_class_type) == decl);
	      TYPE_METHODS (current_class_type)
		= nreverse (TYPE_METHODS (current_class_type));
	      DECL_CHAIN (decl) = save;
	    }
	  /* Reverse the method chain temporarily, so that we can find
	     the clones after DECL.  The clones are supposed to be
	     introduced one right after the other, so truncating the
	     list at DECL for the temporary reversal will yield a very
	     short list.  */
	  tree save = DECL_CHAIN (decl), abstract_decl = decl;
	  DECL_CHAIN (decl) = NULL_TREE;
	  TYPE_METHODS (current_class_type)
	    = nreverse (TYPE_METHODS (current_class_type));
	  for (decl = DECL_CHAIN (decl); decl; decl = DECL_CHAIN (decl))
	    if (DECL_NAME (decl) == identifier)
	      break;
	  TYPE_METHODS (current_class_type)
	    = nreverse (TYPE_METHODS (current_class_type));
	  DECL_CHAIN (abstract_decl) = save;
	}
      else
	{
	  if ((sym_flags & GCC_CP_FLAG_SPECIAL_FUNCTION)
	      && opcode != ERROR_MARK)
	    SET_OVERLOADED_OPERATOR_CODE (decl, opcode);
	  if (assop)
	    DECL_ASSIGNMENT_OPERATOR_P (decl) = true;
	}
    }
  else if (class_member_p)
    {
      decl = build_lang_decl_loc (loc, code, identifier, sym_type);

      if (TREE_CODE (decl) == VAR_DECL)
	{
	  DECL_THIS_STATIC (decl) = 1;
	  // The remainder of this block does the same as:
	  // set_linkage_for_static_data_member (decl);
	  TREE_PUBLIC (decl) = 1;
	  TREE_STATIC (decl) = 1;
	  DECL_INTERFACE_KNOWN (decl) = 1;

	  // FIXME: sym_flags & GCC_CP_FLAG_THREAD_LOCAL_VARIABLE
	  gcc_assert (!(sym_flags & GCC_CP_FLAG_THREAD_LOCAL_VARIABLE));

	  if (sym_flags & GCC_CP_FLAG_CONSTEXPR_VARIABLE)
	    DECL_DECLARED_CONSTEXPR_P (decl) = true;
	}
    }
  else
    {
      decl = build_decl (loc, code, identifier, sym_type);

      if (TREE_CODE (decl) == VAR_DECL)
	{
	  // FIXME: sym_flags & GCC_CP_FLAG_THREAD_LOCAL_VARIABLE
	  gcc_assert (!(sym_flags & GCC_CP_FLAG_THREAD_LOCAL_VARIABLE));

	  if (sym_flags & GCC_CP_FLAG_CONSTEXPR_VARIABLE)
	    DECL_DECLARED_CONSTEXPR_P (decl) = true;
	}
    }
  TREE_USED (decl) = 1;
  TREE_ADDRESSABLE (decl) = 1;

  if (class_member_p)
    DECL_CONTEXT (decl) = FROB_CONTEXT (current_class_type);
  else if (at_namespace_scope_p ())
    DECL_CONTEXT (decl) = FROB_CONTEXT (current_decl_namespace ());

  if (sym_kind != GCC_CP_SYMBOL_TYPEDEF)
    {
      decl_addr_value value;

      DECL_EXTERNAL (decl) = 1;
      value.decl = decl;
      if (substitution_name != NULL)
	{
	  // If the translator gave us a name without a binding,
	  // we can just substitute error_mark_node, since we know the
	  // translator will be reporting an error anyhow.
	  value.address
	    = lookup_name (get_identifier (substitution_name));
	  if (value.address == NULL_TREE)
	    value.address = error_mark_node;
	}
      else if (address)
	value.address = build_int_cst_type (ptr_type_node, address);
      else
	value.address = NULL;
      if (value.address)
	record_decl_address (ctx, value);
    }

  if (class_member_p)
    {
      if (code == FUNCTION_DECL)
	grok_special_member_properties (decl);
      if (!(ctor || dtor))
	finish_member_declaration (decl);
    }
  else
    decl = safe_pushdecl_maybe_friend (decl, false);

  rest_of_decl_compilation (decl, toplevel_bindings_p (), 0);

  return convert_out (ctx->preserve (decl));
}

gcc_type
plugin_build_pointer_type (cc1_plugin::connection *,
			   gcc_type base_type)
{
  // No need to preserve a pointer type as the base type is preserved.
  return convert_out (build_pointer_type (convert_in (base_type)));
}

gcc_type
plugin_build_reference_type (cc1_plugin::connection *,
			     gcc_type base_type_in,
			     enum gcc_cp_ref_qualifiers rquals)
{
  bool rval;

  switch (rquals)
    {
    case GCC_CP_REF_QUAL_LVALUE:
      rval = false;
      break;
    case GCC_CP_REF_QUAL_RVALUE:
      rval = true;
      break;
    case GCC_CP_REF_QUAL_NONE:
    default:
      gcc_unreachable ();
    }

  tree rtype = cp_build_reference_type (convert_in (base_type_in), rval);

  return convert_out (rtype);
}

static tree
start_class_def (tree type,
		 const gcc_vbase_array *base_classes)
{
  tree bases = NULL;
  if (base_classes)
    {
      for (int i = 0; i < base_classes->n_elements; i++)
	{
	  tree base = finish_base_specifier
	    (convert_in (base_classes->elements[i]),
	     access_default_node, base_classes->virtualp[i]);
	  TREE_CHAIN (base) = bases;
	  bases = base;
	}
      bases = nreverse (bases);
    }
  xref_basetypes (type, bases);
  begin_class_definition (type);
  return type;
}

static tree
build_named_class_type (enum tree_code code,
			const char *name,
			const gcc_vbase_array *base_classes,
			source_location loc)
{
  tree type = make_class_type (code);
  tree id = name ? get_identifier (name) : make_anon_name ();
  tree type_decl = build_decl (loc, TYPE_DECL, id, type);
  TYPE_NAME (type) = type_decl;
  TYPE_STUB_DECL (type) = type_decl;
  safe_pushtag (id, type, ts_current);

  return start_class_def (type, base_classes);
}

gcc_type
plugin_start_new_class_type (cc1_plugin::connection *self,
			     const char *name,
			     const gcc_vbase_array *base_classes,
			     const char *filename,
			     unsigned int line_number)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);

  tree type = build_named_class_type (RECORD_TYPE, name, base_classes,
				      ctx->get_source_location (filename,
								line_number));

  return convert_out (ctx->preserve (type));
}

gcc_type
plugin_start_new_union_type (cc1_plugin::connection *self,
			     const char *name,
			     const char *filename,
			     unsigned int line_number)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);

  tree type = build_named_class_type (UNION_TYPE, name, NULL,
				      ctx->get_source_location (filename,
								line_number));

  return convert_out (ctx->preserve (type));
}

int
plugin_new_field (cc1_plugin::connection *,
		  const char *field_name,
		  gcc_type field_type_in,
		  enum gcc_cp_field_flags field_flags,
		  unsigned long bitsize,
		  unsigned long bitpos)
{
  tree record_or_union_type = current_class_type;
  tree field_type = convert_in (field_type_in);

  gcc_assert (RECORD_OR_UNION_CODE_P (TREE_CODE (record_or_union_type)));

  /* Note that gdb does not preserve the location of field decls, so
     we can't provide a decent location here.  */
  tree decl = build_decl (BUILTINS_LOCATION, FIELD_DECL,
			  get_identifier (field_name), field_type);
  DECL_FIELD_CONTEXT (decl) = record_or_union_type;

  if ((field_flags & GCC_CP_FIELD_MUTABLE) != 0)
    DECL_MUTABLE_P (decl) = 1;

  if (TREE_CODE (field_type) == INTEGER_TYPE
      && TYPE_PRECISION (field_type) != bitsize)
    {
      DECL_BIT_FIELD_TYPE (decl) = field_type;
      TREE_TYPE (decl)
	= c_build_bitfield_integer_type (bitsize, TYPE_UNSIGNED (field_type));
    }

  DECL_MODE (decl) = TYPE_MODE (TREE_TYPE (decl));

  // There's no way to recover this from DWARF.
  SET_DECL_OFFSET_ALIGN (decl, TYPE_PRECISION (pointer_sized_int_node));

  tree pos = bitsize_int (bitpos);
  pos_from_bit (&DECL_FIELD_OFFSET (decl), &DECL_FIELD_BIT_OFFSET (decl),
		DECL_OFFSET_ALIGN (decl), pos);

  DECL_SIZE (decl) = bitsize_int (bitsize);
  DECL_SIZE_UNIT (decl) = size_int ((bitsize + BITS_PER_UNIT - 1)
				    / BITS_PER_UNIT);

  DECL_CHAIN (decl) = TYPE_FIELDS (record_or_union_type);
  TYPE_FIELDS (record_or_union_type) = decl;

  return 1;
}

int
plugin_finish_record_or_union (cc1_plugin::connection *,
			       unsigned long size_in_bytes)
{
  tree record_or_union_type = current_class_type;

  gcc_assert (RECORD_OR_UNION_CODE_P (TREE_CODE (record_or_union_type)));

  finish_struct (record_or_union_type, NULL);

  gcc_assert (compare_tree_int (TYPE_SIZE_UNIT (record_or_union_type),
				size_in_bytes) == 0);

  // FIXME: end_template_decl if it's a template?  I don't think so,
  // we're only defining specializations.  -lxo

  return 1;
}

gcc_type
plugin_start_new_enum_type (cc1_plugin::connection *self,
			    const char *name,
			    gcc_type underlying_int_type_in,
			    int scoped_enum_p,
			    const char *filename,
			    unsigned int line_number)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  tree underlying_int_type = convert_in (underlying_int_type_in);

  if (underlying_int_type == error_mark_node)
    return convert_out (error_mark_node);

  bool is_new_type = false;

  tree id = name ? get_identifier (name) : make_anon_name ();

  tree type = start_enum (id, NULL_TREE,
			  underlying_int_type,
			  /* attributes = */ NULL_TREE,
			  !!scoped_enum_p, &is_new_type);

  gcc_assert (is_new_type);

  source_location loc = ctx->get_source_location (filename, line_number);
  tree type_decl = build_decl (loc, TYPE_DECL, id, type);
  TYPE_NAME (type) = type_decl;
  TYPE_STUB_DECL (type) = type_decl;

  safe_pushtag (DECL_NAME (type_decl), type, ts_current);

  return convert_out (ctx->preserve (type));
}

int
plugin_build_add_enum_constant (cc1_plugin::connection *,
				gcc_type enum_type_in,
				const char *name,
				unsigned long value)
{
  tree enum_type = convert_in (enum_type_in);

  gcc_assert (TREE_CODE (enum_type) == ENUMERAL_TYPE);

  build_enumerator (get_identifier (name), build_int_cst (enum_type, value),
		    enum_type, NULL_TREE, BUILTINS_LOCATION);

  return 1;
}

int
plugin_finish_enum_type (cc1_plugin::connection *,
			 gcc_type enum_type_in)
{
  tree enum_type = convert_in (enum_type_in);

  finish_enum_value_list (enum_type);
  finish_enum (enum_type);

  return 1;
}

gcc_type
plugin_build_function_type (cc1_plugin::connection *self,
			    gcc_type return_type_in,
			    const struct gcc_type_array *argument_types_in,
			    int is_varargs)
{
  tree *argument_types;
  tree return_type = convert_in (return_type_in);
  tree result;

  argument_types = new tree[argument_types_in->n_elements];
  for (int i = 0; i < argument_types_in->n_elements; ++i)
    argument_types[i] = convert_in (argument_types_in->elements[i]);

  if (is_varargs)
    result = build_varargs_function_type_array (return_type,
						argument_types_in->n_elements,
						argument_types);
  else
    result = build_function_type_array (return_type,
					argument_types_in->n_elements,
					argument_types);

  delete[] argument_types;

  plugin_context *ctx = static_cast<plugin_context *> (self);
  return convert_out (ctx->preserve (result));
}

gcc_type
plugin_add_function_default_args (cc1_plugin::connection *self,
				  gcc_type function_type_in,
				  const struct gcc_cp_function_default_args *defaults)
{
  tree function_type = convert_in (function_type_in);

  gcc_assert (TREE_CODE (function_type) == FUNCTION_TYPE);

  if (!defaults || !defaults->n_elements)
    return function_type_in;

  tree pargs = TYPE_ARG_TYPES (function_type);
  tree nargs = NULL_TREE;

  /* Build a reversed copy of the list of default-less arguments in
     NARGS.  At the end of the loop, PARGS will point to the end of
     the argument list, or to the first argument that had a default
     value.  */
  while (pargs && TREE_VALUE (pargs) != void_list_node
	 && !TREE_PURPOSE (pargs))
    {
      nargs = tree_cons (NULL_TREE, TREE_VALUE (pargs), nargs);
      pargs = TREE_CHAIN (pargs);
    }

  /* Set the defaults in the now-leading NARGS, taking into account
     that NARGS is reversed but DEFAULTS->elements isn't.  */
  tree ndargs = nargs;
  int i = defaults->n_elements;
  while (i--)
    {
      gcc_assert (ndargs);
      tree deflt = convert_in (defaults->elements[i]);
      gcc_assert (deflt);
      TREE_PURPOSE (ndargs) = deflt;
      ndargs = TREE_CHAIN (ndargs);
    }

  /* Finally, reverse NARGS, and append the remaining PARGS that
     already had defaults.  */
  nargs = nreverse (nargs);
  nargs = chainon (nargs, pargs);

  tree result = build_function_type (TREE_TYPE (function_type), nargs);

  /* Copy exceptions, attributes and whatnot.  */
  result = build_exception_variant (result,
				    TYPE_RAISES_EXCEPTIONS (function_type));
  result = cp_build_type_attribute_variant (result,
					    TYPE_ATTRIBUTES (function_type));

  plugin_context *ctx = static_cast<plugin_context *> (self);
  return convert_out (ctx->preserve (result));
}

gcc_type
plugin_build_exception_spec_variant (cc1_plugin::connection *self,
				     gcc_type function_type_in,
				     const struct gcc_type_array *except_types_in)
{
  tree function_type = convert_in (function_type_in);
  tree except_types = NULL_TREE;

  if (!except_types_in)
    except_types = noexcept_false_spec;
  else if (!except_types_in->n_elements)
    except_types = empty_except_spec;
  else
    for (int i = 0; i < except_types_in->n_elements; i++)
      except_types = add_exception_specifier (except_types,
					      convert_in
					      (except_types_in->elements[i]),
					      0);

  function_type = build_exception_variant (function_type,
					   except_types);

  plugin_context *ctx = static_cast<plugin_context *> (self);
  return convert_out (ctx->preserve (function_type));
}

gcc_type
plugin_build_method_type (cc1_plugin::connection *self,
			  gcc_type class_type_in,
			  gcc_type func_type_in,
			  enum gcc_cp_qualifiers quals_in,
			  enum gcc_cp_ref_qualifiers rquals_in)
{
  tree class_type = convert_in (class_type_in);
  tree func_type = convert_in (func_type_in);
  cp_cv_quals quals = 0;
  cp_ref_qualifier rquals;

  if ((quals_in & GCC_CP_QUALIFIER_CONST) != 0)
    quals |= TYPE_QUAL_CONST;
  if ((quals_in & GCC_CP_QUALIFIER_VOLATILE) != 0)
    quals |= TYPE_QUAL_VOLATILE;
  gcc_assert ((quals_in & GCC_CP_QUALIFIER_RESTRICT) == 0);

  switch (rquals_in)
    {
    case GCC_CP_REF_QUAL_NONE:
      rquals = REF_QUAL_NONE;
      break;
    case GCC_CP_REF_QUAL_LVALUE:
      rquals = REF_QUAL_LVALUE;
      break;
    case GCC_CP_REF_QUAL_RVALUE:
      rquals = REF_QUAL_RVALUE;
      break;
    default:
      gcc_unreachable ();
    }
  
  tree method_type = class_type
    ? build_memfn_type (func_type, class_type, quals, rquals)
    : apply_memfn_quals (func_type, quals, rquals);
  
  plugin_context *ctx = static_cast<plugin_context *> (self);
  return convert_out (ctx->preserve (method_type));
}

gcc_type
plugin_build_pointer_to_member_type (cc1_plugin::connection *self,
				     gcc_type class_type_in,
				     gcc_type member_type_in)
{
  tree class_type = convert_in (class_type_in);
  tree member_type = convert_in (member_type_in);

  tree memptr_type = build_ptrmem_type (class_type, member_type);

  plugin_context *ctx = static_cast<plugin_context *> (self);
  return convert_out (ctx->preserve (memptr_type));
}

/* Abuse an unused field of the dummy template parms entry to hold the
   parm list.  */
#define TP_PARM_LIST TREE_TYPE (current_template_parms)

int
plugin_start_new_template_decl (cc1_plugin::connection *self ATTRIBUTE_UNUSED)
{
  begin_template_parm_list ();

  TP_PARM_LIST = NULL_TREE;

  return 0;
}

gcc_typedecl
plugin_type_decl (cc1_plugin::connection *,
		  gcc_type type_in)
{
  tree type = convert_in (type_in);

  tree name = TYPE_NAME (type);
  gcc_assert (name);

  return convert_out (name);
}

gcc_typedecl
plugin_new_template_typename_parm (cc1_plugin::connection *self,
				   const char *id,
				   int /* bool */ pack_p,
				   gcc_typedecl default_type,
				   const char *filename,
				   unsigned int line_number)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  source_location loc = ctx->get_source_location (filename, line_number);

  tree parm = finish_template_type_parm (class_type_node, get_identifier (id));
  parm = build_tree_list (convert_in (default_type), parm);

  gcc_assert (!(pack_p && default_type));

  /* Create a type and a decl for the type parm, and add the decl to
     TP_PARM_LIST.  */
  TP_PARM_LIST = process_template_parm (TP_PARM_LIST, loc, parm,
					/* is_non_type = */ false, pack_p);

  /* Return the type of the newly-added parm decl.  */
  return convert_out (ctx->preserve (TREE_TYPE (tree_last (TP_PARM_LIST))));
}

gcc_utempl
plugin_new_template_template_parm (cc1_plugin::connection *self,
				   const char *id,
				   int /* bool */ pack_p,
				   gcc_utempl default_templ,
				   const char *filename,
				   unsigned int line_number)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  source_location loc = ctx->get_source_location (filename, line_number);

  /* Finish the template parm list that started this template parm.  */
  end_template_parm_list (TP_PARM_LIST);

  tree parm = finish_template_template_parm (class_type_node,
					     get_identifier (id));
  parm = build_tree_list (convert_in (default_templ), parm);

  gcc_assert (!(pack_p && default_templ));

  /* Create a type and a decl for the template parm, and add the decl
     to TP_PARM_LIST.  */
  TP_PARM_LIST = process_template_parm (TP_PARM_LIST, loc, parm,
					/* is_non_type = */ false, pack_p);

  /* Return the decl of the newly-added template template parm.  */
  return convert_out (ctx->preserve (tree_last (TP_PARM_LIST)));
}

gcc_decl
plugin_new_template_value_parm (cc1_plugin::connection *self,
				gcc_type type,
				const char *id,
				gcc_expr default_value,
				const char *filename,
				unsigned int line_number)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  source_location loc = ctx->get_source_location (filename, line_number);

  cp_declarator declarator;
  memset (&declarator, 0, sizeof (declarator));
  // &declarator = make_id_declarator (NULL, get_identifier (id), sfk_none):
  declarator.kind = cdk_id;
  declarator.u.id.qualifying_scope = NULL;
  declarator.u.id.unqualified_name = get_identifier (id);
  declarator.u.id.sfk = sfk_none;

  cp_decl_specifier_seq declspec;
  memset (&declspec, 0, sizeof (declspec));
  // cp_parser_set_decl_spec_type (&declspec, convert_in (type), -token-, false):
  declspec.any_specifiers_p = declspec.any_type_specifiers_p = true;
  declspec.type = convert_in (type);
  declspec.locations[ds_type_spec] = loc;

  tree parm = grokdeclarator (&declarator, &declspec, TPARM, 0, 0);
  parm = build_tree_list (convert_in (default_value), parm);

  /* Create a type and a decl for the template parm, and add the decl
     to TP_PARM_LIST.  */
  TP_PARM_LIST = process_template_parm (TP_PARM_LIST, loc, parm,
					/* is_non_type = */ true, false);

  /* Return the newly-added parm decl.  */
  return convert_out (ctx->preserve (tree_last (TP_PARM_LIST)));
}

static tree
targlist (const gcc_cp_template_args *targs)
{
  int n = targs->n_elements;
  tree vec = make_tree_vec (n);
  while (n--)
    {
      switch (targs->kinds[n])
	{
	case GCC_CP_TPARG_VALUE:
	  TREE_VEC_ELT (vec, n) = convert_in (targs->elements[n].value);
	  break;
	case GCC_CP_TPARG_CLASS:
	  TREE_VEC_ELT (vec, n) = convert_in (targs->elements[n].type);
	  break;
	case GCC_CP_TPARG_TEMPL:
	  TREE_VEC_ELT (vec, n) = convert_in (targs->elements[n].templ);
	  break;
	case GCC_CP_TPARG_PACK:
	  TREE_VEC_ELT (vec, n) = convert_in (targs->elements[n].pack);
	  break;
	default:
	  gcc_unreachable ();
	}
    }
  return vec;
}

gcc_typedecl
plugin_new_dependent_typename (cc1_plugin::connection *self,
			       gcc_typedecl enclosing_type,
			       const char *id,
			       const gcc_cp_template_args *targs)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  tree type = convert_in (enclosing_type);
  tree name = get_identifier (id);
  if (targs)
    name = build_min_nt_loc (/*loc=*/0, TEMPLATE_ID_EXPR,
			     name, targlist (targs));
  tree res = make_typename_type (type, name, typename_type,
				 /*complain=*/tf_error);
  return convert_out (ctx->preserve (res));
}

gcc_utempl
plugin_new_dependent_class_template (cc1_plugin::connection *self,
				     gcc_typedecl enclosing_type,
				     const char *id)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  tree type = convert_in (enclosing_type);
  tree name = get_identifier (id);
  tree res = make_unbound_class_template (type, name, NULL_TREE,
					  /*complain=*/tf_error);
  return convert_out (ctx->preserve (res));
}

gcc_typedecl
plugin_new_dependent_typespec (cc1_plugin::connection *self,
			       gcc_utempl template_decl,
			       const gcc_cp_template_args *targs)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  tree type = convert_in (template_decl);
  tree decl = finish_template_type (type, targlist (targs),
				    /*entering_scope=*/false);
  return convert_out (ctx->preserve (decl));
}

gcc_decl
plugin_new_dependent_value_expr (cc1_plugin::connection *self,
				 gcc_typedecl enclosing_type,
				 const char *id,
				 const gcc_cp_template_args *targs)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  tree type = convert_in (enclosing_type);
  tree name = get_identifier (id);
  if (targs)
    name = lookup_template_function (name, targlist (targs));
  tree res = build_qualified_name (NULL_TREE, type, name, !!targs);
  return convert_out (ctx->preserve (res));
}

gcc_expr
plugin_literal_expr (cc1_plugin::connection *self,
		     gcc_type type, unsigned long value)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  tree t = convert_in (type);
  tree val = build_int_cst_type (t, (unsigned HOST_WIDE_INT) value);
  return convert_out (ctx->preserve (val));
}

gcc_expr
plugin_decl_expr (cc1_plugin::connection *, gcc_decl decl_in)
{
  tree decl = convert_in (decl_in);
  gcc_assert (DECL_P (decl));
  return convert_out (decl);
}

gcc_expr
plugin_unary_value_expr (cc1_plugin::connection *self,
			 const char *unary_op,
			 gcc_expr operand)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  tree op0 = convert_in (operand);
  tree_code opcode = ERROR_MARK;
  switch (CHARS2 (unary_op[0], unary_op[1]))
    {
    case CHARS2 ('p', 's'): // operator + (unary)
      opcode = UNARY_PLUS_EXPR;
      break;
    case CHARS2 ('n', 'g'): // operator - (unary)
      opcode = NEGATE_EXPR;
      break;
    case CHARS2 ('a', 'd'): // operator & (unary)
      /* FIXME: how do we distinguish taking the address of a data
	 member from creating a pointer-to-member value?  Both would
	 take the same decl as the operand, unless we require
	 different expr codes for e.g. this->member and
	 class::member.  */
      opcode = ADDR_EXPR;
      break;
    case CHARS2 ('d', 'e'): // operator * (unary)
      opcode = INDIRECT_REF;
      break;
    case CHARS2 ('c', 'o'): // operator ~
      opcode = BIT_NOT_EXPR;
      break;
    case CHARS2 ('n', 't'): // operator !
      opcode = TRUTH_NOT_EXPR;
      break;
      /* FIXME: __real__, __imag__.  */
    case CHARS2 ('p', 'p'): // operator ++
    case CHARS2 ('m', 'm'): // operator --
    default:
      gcc_unreachable ();
    }
  processing_template_decl++;
  tree val = build_x_unary_op (/*loc=*/0, opcode, op0, tf_error);
  processing_template_decl--;
  return convert_out (ctx->preserve (val));
}

gcc_expr
plugin_binary_value_expr (cc1_plugin::connection *self,
			  const char *binary_op,
			  gcc_expr operand1,
			  gcc_expr operand2)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  tree op0 = convert_in (operand1);
  tree op1 = convert_in (operand2);
  tree_code opcode = ERROR_MARK;
  switch (CHARS2 (binary_op[0], binary_op[1]))
    {
    case CHARS2 ('p', 'l'): // operator +
      opcode = PLUS_EXPR;
      break;
    case CHARS2 ('m', 'i'): // operator -
      opcode = MINUS_EXPR;
      break;
    case CHARS2 ('m', 'l'): // operator *
      opcode = MULT_EXPR;
      break;
    case CHARS2 ('d', 'v'): // operator /
      opcode = TRUNC_DIV_EXPR;
      break;
    case CHARS2 ('r', 'm'): // operator %
      opcode = TRUNC_MOD_EXPR;
      break;
    case CHARS2 ('a', 'n'): // operator &
      opcode = BIT_AND_EXPR;
      break;
    case CHARS2 ('o', 'r'): // operator |
      opcode = BIT_IOR_EXPR;
      break;
    case CHARS2 ('e', 'o'): // operator ^
      opcode = BIT_XOR_EXPR;
      break;
    case CHARS2 ('l', 's'): // operator <<
      opcode = LSHIFT_EXPR;
      break;
    case CHARS2 ('r', 's'): // operator >>
      opcode = RSHIFT_EXPR;
      break;
    case CHARS2 ('e', 'q'): // operator ==
      opcode = EQ_EXPR;
      break;
    case CHARS2 ('n', 'e'): // operator !=
      opcode = NE_EXPR;
      break;
    case CHARS2 ('l', 't'): // operator <
      opcode = LT_EXPR;
      break;
    case CHARS2 ('g', 't'): // operator >
      opcode = GT_EXPR;
      break;
    case CHARS2 ('l', 'e'): // operator <=
      opcode = LE_EXPR;
      break;
    case CHARS2 ('g', 'e'): // operator >=
      opcode = GE_EXPR;
      break;
    case CHARS2 ('a', 'a'): // operator &&
      opcode = TRUTH_ANDIF_EXPR;
      break;
    case CHARS2 ('o', 'o'): // operator ||
      opcode = TRUTH_ORIF_EXPR;
      break;
    case CHARS2 ('c', 'm'): // operator ,
      opcode = COMPOUND_EXPR;
      break;
    case CHARS2 ('p', 'm'): // operator ->*
      opcode = MEMBER_REF;
      break;
    case CHARS2 ('p', 't'): // operator ->
      opcode = COMPONENT_REF;
      break;
    case CHARS2 ('i', 'x'): // operator []
      opcode = ARRAY_REF;
      break;
    default:
      gcc_unreachable ();
    }
  processing_template_decl++;
  tree val = build_x_binary_op (/*loc=*/0, opcode, op0, ERROR_MARK,
				op1, ERROR_MARK, NULL, tf_error);
  processing_template_decl--;
  return convert_out (ctx->preserve (val));
}

gcc_expr
plugin_ternary_value_expr (cc1_plugin::connection *self,
			   const char *ternary_op,
			   gcc_expr operand1,
			   gcc_expr operand2,
			   gcc_expr operand3)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  tree op0 = convert_in (operand1);
  tree op1 = convert_in (operand2);
  tree op2 = convert_in (operand3);
  gcc_assert (CHARS2 (ternary_op[0], ternary_op[1])
	      == CHARS2 ('q', 'u')); // ternary operator
  processing_template_decl++;
  tree val = build_x_conditional_expr (/*loc=*/0, op0, op1, op2, tf_error);
  processing_template_decl--;
  return convert_out (ctx->preserve (val));
}

gcc_expr
plugin_unary_type_expr (cc1_plugin::connection *self,
			const char *unary_op,
			gcc_typedecl operand)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  tree type = convert_in (operand);
  tree_code opcode = ERROR_MARK;
  switch (CHARS2 (unary_op[0], unary_op[1]))
    {
      /* FIXME: implement sizeof, alignof, ...  */
    default:
      gcc_unreachable ();
    }
  processing_template_decl++;
  tree val = cxx_sizeof_or_alignof_type (type, opcode, true);
  processing_template_decl--;
  return convert_out (ctx->preserve (val));
}

gcc_expr
plugin_type_value_expr (cc1_plugin::connection *self,
			const char *binary_op,
			gcc_typedecl /* operand1 */,
			gcc_expr /* operand2 */)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
#if 0
  tree type = convert_in (operand1);
  tree expr = convert_in (operand2);
  tree_code opcode = ERROR_MARK;
#endif
  switch (CHARS2 (binary_op[0], binary_op[1]))
    {
      /* FIXME: implement type casts, ...  */
    default:
      gcc_unreachable ();
    }
  processing_template_decl++;
  tree val = NULL_TREE;
  processing_template_decl--;
  return convert_out (ctx->preserve (val));
}

gcc_typedecl
plugin_expr_type (cc1_plugin::connection *,
		  gcc_expr operand)
{
  tree op0 = convert_in (operand);
  tree type = TREE_TYPE (op0);
  if (type)
    type = TYPE_NAME (type);
  return convert_out (type);
}

gcc_decl
plugin_specialize_function_template (cc1_plugin::connection *self,
				     gcc_decl template_decl,
				     const gcc_cp_template_args *targs,
				     gcc_address address,
				     const char *filename,
				     unsigned int line_number)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  source_location loc = ctx->get_source_location (filename, line_number);
  tree name = convert_in (template_decl);
  tree targsl = targlist (targs);
  
  tree fnid = lookup_template_function (name, targsl);
  if (TREE_CODE (fnid) == TEMPLATE_ID_EXPR)
    SET_EXPR_LOCATION (fnid, loc);

  tree decl = tsubst (name, targsl, tf_error, NULL_TREE);
  DECL_SOURCE_LOCATION (decl) = loc;

  record_decl_address (ctx, build_decl_addr_value (decl, address));

  return convert_out (ctx->preserve (decl));
}

gcc_type
plugin_start_specialize_class_template (cc1_plugin::connection *self,
					gcc_decl template_decl,
					const gcc_cp_template_args *args,
					const gcc_vbase_array *base_classes,
					const char *filename,
					unsigned int line_number)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  source_location loc = ctx->get_source_location (filename, line_number);
  tree name = convert_in (template_decl);

  // begin_specialization (); // hopefully we don't really need this

  tree tdecl = finish_template_type (name, targlist (args), false);;
  DECL_SOURCE_LOCATION (tdecl) = loc;
  
  tree type = start_class_def (TREE_TYPE (tdecl), base_classes);

  return convert_out (ctx->preserve (type));
}

/* Return a builtin type associated with BUILTIN_NAME.  */

static tree
safe_lookup_builtin_type (const char *builtin_name)
{
  tree result = NULL_TREE;

  if (!builtin_name)
    return result;

  result = identifier_global_value (get_identifier (builtin_name));

  if (!result)
    return result;

  gcc_assert (TREE_CODE (result) == TYPE_DECL);
  result = TREE_TYPE (result);
  return result;
}

gcc_type
plugin_int_type (cc1_plugin::connection *self,
		 int is_unsigned, unsigned long size_in_bytes,
		 const char *builtin_name)
{
  tree result;

  if (builtin_name)
    {
      result = safe_lookup_builtin_type (builtin_name);
      gcc_assert (!result || TREE_CODE (result) == INTEGER_TYPE);
    }
  else
    result = c_common_type_for_size (BITS_PER_UNIT * size_in_bytes,
				     is_unsigned);

  if (result == NULL_TREE)
    result = error_mark_node;
  else
    {
      gcc_assert (!TYPE_UNSIGNED (result) == !is_unsigned);
      gcc_assert (TREE_CODE (TYPE_SIZE (result)) == INTEGER_CST);
      gcc_assert (TYPE_PRECISION (result) == BITS_PER_UNIT * size_in_bytes);

      plugin_context *ctx = static_cast<plugin_context *> (self);
      ctx->preserve (result);
    }
  return convert_out (result);
}

gcc_type
plugin_char_type (cc1_plugin::connection *)
{
  return convert_out (char_type_node);
}

gcc_type
plugin_float_type (cc1_plugin::connection *,
		   unsigned long size_in_bytes,
		   const char *builtin_name)
{
  if (builtin_name)
    {
      tree result = safe_lookup_builtin_type (builtin_name);

      if (!result)
	return convert_out (error_mark_node);

      gcc_assert (TREE_CODE (result) == REAL_TYPE);
      gcc_assert (BITS_PER_UNIT * size_in_bytes == TYPE_PRECISION (result));

      return convert_out (result);
    }

  if (BITS_PER_UNIT * size_in_bytes == TYPE_PRECISION (float_type_node))
    return convert_out (float_type_node);
  if (BITS_PER_UNIT * size_in_bytes == TYPE_PRECISION (double_type_node))
    return convert_out (double_type_node);
  if (BITS_PER_UNIT * size_in_bytes == TYPE_PRECISION (long_double_type_node))
    return convert_out (long_double_type_node);
  return convert_out (error_mark_node);
}

gcc_type
plugin_void_type (cc1_plugin::connection *)
{
  return convert_out (void_type_node);
}

gcc_type
plugin_bool_type (cc1_plugin::connection *)
{
  return convert_out (boolean_type_node);
}

gcc_type
plugin_get_nullptr_type (cc1_plugin::connection *)
{
  return convert_out (nullptr_type_node);
}

gcc_expr
plugin_get_nullptr_constant (cc1_plugin::connection *)
{
  return convert_out (nullptr_node);
}

gcc_type
plugin_build_array_type (cc1_plugin::connection *self,
			 gcc_type element_type_in, int num_elements)
{
  tree element_type = convert_in (element_type_in);
  tree result;

  if (num_elements == -1)
    result = build_array_type (element_type, NULL_TREE);
  else
    result = build_array_type_nelts (element_type, num_elements);

  plugin_context *ctx = static_cast<plugin_context *> (self);
  return convert_out (ctx->preserve (result));
}

gcc_type
plugin_build_dependent_array_type (cc1_plugin::connection *self,
				   gcc_type element_type_in,
				   gcc_expr num_elements_in)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  tree element_type = convert_in (element_type_in);
  tree size = convert_in (num_elements_in);
  tree name = get_identifier ("dependent array type");

  processing_template_decl++;
  tree itype = compute_array_index_type (name, size, tf_error);
  tree type = build_cplus_array_type (element_type, itype);
  processing_template_decl--;

  return convert_out (ctx->preserve (type));
}

gcc_type
plugin_build_vla_array_type (cc1_plugin::connection *self,
			     gcc_type element_type_in,
			     const char *upper_bound_name)
{
  tree element_type = convert_in (element_type_in);
  tree upper_bound = lookup_name (get_identifier (upper_bound_name));
  tree size = fold_build2 (PLUS_EXPR, TREE_TYPE (upper_bound), upper_bound,
			   build_one_cst (TREE_TYPE (upper_bound)));
  tree range = compute_array_index_type (NULL_TREE, size,
					 tf_warning_or_error);

  tree result = build_cplus_array_type (element_type, range);

  plugin_context *ctx = static_cast<plugin_context *> (self);
  return convert_out (ctx->preserve (result));
}

gcc_type
plugin_build_qualified_type (cc1_plugin::connection *,
			     gcc_type unqualified_type_in,
			     enum gcc_cp_qualifiers qualifiers)
{
  tree unqualified_type = convert_in (unqualified_type_in);
  cp_cv_quals quals = 0;

  if ((qualifiers & GCC_CP_QUALIFIER_CONST) != 0)
    quals |= TYPE_QUAL_CONST;
  if ((qualifiers & GCC_CP_QUALIFIER_VOLATILE) != 0)
    quals |= TYPE_QUAL_VOLATILE;
  if ((qualifiers & GCC_CP_QUALIFIER_RESTRICT) != 0)
    quals |= TYPE_QUAL_RESTRICT;

  gcc_assert ((TREE_CODE (unqualified_type) != METHOD_TYPE
	       && TREE_CODE (unqualified_type) != REFERENCE_TYPE)
	      || quals == 0);

  return convert_out (build_qualified_type (unqualified_type, quals));
}

gcc_type
plugin_build_complex_type (cc1_plugin::connection *self,
			   gcc_type base_type)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  return convert_out (ctx->preserve (build_complex_type (convert_in (base_type))));
}

gcc_type
plugin_build_vector_type (cc1_plugin::connection *self,
			  gcc_type base_type, int nunits)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  return convert_out (ctx->preserve (build_vector_type (convert_in (base_type),
							nunits)));
}

int
plugin_build_constant (cc1_plugin::connection *self, gcc_type type_in,
		       const char *name, unsigned long value,
		       const char *filename, unsigned int line_number)
{
  plugin_context *ctx = static_cast<plugin_context *> (self);
  tree cst, decl;
  tree type = convert_in (type_in);

  cst = build_int_cst (type, value);
  if (!TYPE_READONLY (type))
    type = build_qualified_type (type, TYPE_QUAL_CONST);
  decl = build_decl (ctx->get_source_location (filename, line_number),
		     VAR_DECL, get_identifier (name), type);
  TREE_STATIC (decl) = 1;
  TREE_READONLY (decl) = 1;
  cp_finish_decl (decl, cst, true, NULL, LOOKUP_ONLYCONVERTING);
  safe_pushdecl_maybe_friend (decl, false);

  return 1;
}

gcc_type
plugin_error (cc1_plugin::connection *,
	      const char *message)
{
  error ("%s", message);
  return convert_out (error_mark_node);
}



// Perform GC marking.

static void
gc_mark (void *, void *)
{
  if (current_context != NULL)
    current_context->mark ();
}

#ifdef __GNUC__
#pragma GCC visibility push(default)
#endif

int
plugin_init (struct plugin_name_args *plugin_info,
	     struct plugin_gcc_version *)
{
  long fd = -1;
  for (int i = 0; i < plugin_info->argc; ++i)
    {
      if (strcmp (plugin_info->argv[i].key, "fd") == 0)
	{
	  char *tail;
	  errno = 0;
	  fd = strtol (plugin_info->argv[i].value, &tail, 0);
	  if (*tail != '\0' || errno != 0)
	    fatal_error (input_location,
			 "%s: invalid file descriptor argument to plugin",
			 plugin_info->base_name);
	  break;
	}
    }
  if (fd == -1)
    fatal_error (input_location,
		 "%s: required plugin argument %<fd%> is missing",
		 plugin_info->base_name);

  current_context = new plugin_context (fd);

  // Handshake.
  cc1_plugin::protocol_int version;
  if (!current_context->require ('H')
      || ! ::cc1_plugin::unmarshall (current_context, &version))
    fatal_error (input_location,
		 "%s: handshake failed", plugin_info->base_name);
  if (version != GCC_CP_FE_VERSION_0)
    fatal_error (input_location,
		 "%s: unknown version in handshake", plugin_info->base_name);

  register_callback (plugin_info->base_name, PLUGIN_PRAGMAS,
		     plugin_init_extra_pragmas, NULL);
  register_callback (plugin_info->base_name, PLUGIN_PRE_GENERICIZE,
		     rewrite_decls_to_addresses, NULL);
  register_callback (plugin_info->base_name, PLUGIN_GGC_MARKING,
		     gc_mark, NULL);

  lang_hooks.print_error_function = plugin_print_error_function;

#define GCC_METHOD0(R, N)			\
  {						\
    cc1_plugin::callback_ftype *fun		\
      = cc1_plugin::callback<R, plugin_ ## N>;	\
    current_context->add_callback (# N, fun);	\
  }
#define GCC_METHOD1(R, N, A)				\
  {							\
    cc1_plugin::callback_ftype *fun			\
      = cc1_plugin::callback<R, A, plugin_ ## N>;	\
    current_context->add_callback (# N, fun);		\
  }
#define GCC_METHOD2(R, N, A, B)				\
  {							\
    cc1_plugin::callback_ftype *fun			\
      = cc1_plugin::callback<R, A, B, plugin_ ## N>;	\
    current_context->add_callback (# N, fun);		\
  }
#define GCC_METHOD3(R, N, A, B, C)			\
  {							\
    cc1_plugin::callback_ftype *fun			\
      = cc1_plugin::callback<R, A, B, C, plugin_ ## N>;	\
    current_context->add_callback (# N, fun);		\
  }
#define GCC_METHOD4(R, N, A, B, C, D)		\
  {						\
    cc1_plugin::callback_ftype *fun		\
      = cc1_plugin::callback<R, A, B, C, D,	\
			     plugin_ ## N>;	\
    current_context->add_callback (# N, fun);	\
  }
#define GCC_METHOD5(R, N, A, B, C, D, E)	\
  {						\
    cc1_plugin::callback_ftype *fun		\
      = cc1_plugin::callback<R, A, B, C, D, E,	\
			     plugin_ ## N>;	\
    current_context->add_callback (# N, fun);	\
  }
#define GCC_METHOD7(R, N, A, B, C, D, E, F, G)		\
  {							\
    cc1_plugin::callback_ftype *fun			\
      = cc1_plugin::callback<R, A, B, C, D, E, F, G,	\
			     plugin_ ## N>;		\
    current_context->add_callback (# N, fun);		\
  }

#include "gcc-cp-fe.def"

#undef GCC_METHOD0
#undef GCC_METHOD1
#undef GCC_METHOD2
#undef GCC_METHOD3
#undef GCC_METHOD4
#undef GCC_METHOD5
#undef GCC_METHOD7

  return 0;
}
