/* Struct-reorg optimizations.
   Copyright (C) 2016-2017 Free Software Foundation, Inc.
   Contributed by Andrew Pinski  <apinski@cavium.com>

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

#ifndef IPA_STRUCT_REORG_H
#define IPA_STRUCT_REORG_H

namespace struct_reorg {

const int max_split = 2;

template <typename type>
struct auto_vec_del : auto_vec<type*>
{
  ~auto_vec_del();
};

template <typename T>
auto_vec_del<T>::~auto_vec_del(void)
{
  unsigned i;
  T *t;
  FOR_EACH_VEC_ELT (*this, i, t)
    {
      delete t;
    }
}

enum escape_type
{
  does_not_escape,
#define DEF_ESCAPE(ENUM, TEXT) ENUM,
#include "escapes.def"
  escape_max_escape
};

const char *escape_type_string[escape_max_escape - 1] =
{
#define DEF_ESCAPE(ENUM, TEXT) TEXT,
#include "escapes.def"
};

struct srfield;
struct srtype;
struct sraccess;
struct srdecl;
struct srfunction;

struct srfunction
{
  cgraph_node *node;
  auto_vec<srdecl*> args;
  auto_vec<srdecl*> globals;
  auto_vec_del<srdecl> decls;
  srdecl *record_decl (srtype *, tree, int arg);

  srfunction *old;
  cgraph_node *newnode;
  srfunction *newf;

  // Constructors
  srfunction (cgraph_node *n);

  // Methods
  void add_arg (srdecl *arg);
  void dump (FILE *file);
  void simple_dump (FILE *file);

  bool check_args (void);
  void create_new_decls (void);
  srdecl *find_decl (tree);
};

struct srglobal : private srfunction
{
  srglobal ()
    : srfunction (NULL)
  {
  }

  using srfunction::dump;
  using srfunction::create_new_decls;
  using srfunction::find_decl;
  using srfunction::record_decl;
  using srfunction::decls;
};

struct srtype
{
  tree type;
  auto_vec_del<srfield> fields;

  // array of fields that use this type.
  auto_vec<srfield*> field_sites;

  // array of functions which use directly the type
  auto_vec<srfunction*> functions;

  auto_vec_del<sraccess> accesses;
  bool chain_type;

private:
  escape_type escapes;
public:

  tree newtype[max_split];
  bool visited;

  // Constructors
  srtype(tree type);

  // Methods
  void dump (FILE *file);
  void simple_dump (FILE *file);
  void add_function (srfunction *);
  void add_access (sraccess *a)
  {
    accesses.safe_push (a);
  }
  void add_field_site (srfield *);

  srfield *find_field (unsigned HOST_WIDE_INT offset);

  bool create_new_type (void);
  void analyze (void);
  void mark_escape (escape_type, gimple *stmt);
  bool has_escaped (void)
  {
    return escapes != does_not_escape;
  }
  const char *escape_reason (void)
  {
    if (!has_escaped())
      return NULL;
    return escape_type_string[escapes-1];
  }
  bool escaped_rescusive (void)
  {
    return escapes == escape_rescusive_type;
  }
  bool has_new_type (void)
  {
    return newtype[0] && newtype[0] != type;
  }
};

struct srfield
{
  unsigned HOST_WIDE_INT offset;
  tree fieldtype;
  tree fielddecl;
  srtype *base;
  srtype *type;

  unsigned clusternum;

  tree newfield[max_split];

  // Constructors
  srfield (tree field, srtype *base);

  // Methods
  void dump (FILE *file);
  void simple_dump (FILE *file);

  void create_new_fields (tree newtype[max_split],
			  tree newfields[max_split],
			  tree newlast[max_split]);
};

struct sraccess
{
  gimple *stmt;
  cgraph_node *node;

  srtype *type;
  // NULL field means the whole type is accessed
  srfield *field;

  // constructors
  sraccess (gimple *s, cgraph_node *n, srtype *t, srfield *f = NULL)
    : stmt (s),
      node (n),
      type (t),
      field (f)
  {}

  // Methods
  void dump (FILE *file);
};

struct srdecl
{
  srtype *type;
  tree decl;
  tree func;
  /* -1 : not an argument
     -2 : static chain */
  int argumentnum;

  bool visited;

  tree newdecl[max_split];

  // Constructors
  srdecl (srtype *type, tree decl, int argumentnum = -1);

  // Methods
  void dump (FILE *file);
  bool has_new_decl (void)
  {
    return newdecl[0] && newdecl[0] != decl;
  }
};


} // namespace struct_reorg

#endif
