// Represent a block scope.

// Copyright (C) 2004 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// gcjx is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// gcjx is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with gcjx; see the file COPYING.LIB.  If
// not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#include "typedefs.hh"

model_block_scope::~model_block_scope ()
{
  // It is convenient to check usage of variables when they go out of
  // scope.  For one thing this is the point at which we're certain
  // all references have been processed.
  for (std::map<std::string, model_variable_decl *>::const_iterator i
	 = variable_map.begin ();
       i != variable_map.end ();
       ++i)
    (*i).second->check_referenced (scope);
}

void
model_block_scope::look_up_name (const std::string &name,
				 std::set<model_class *> &result,
				 IContext *,
				 model_class *)
{
  std::map<std::string, model_class *>::const_iterator f
    = class_map.find (name);
  if (f != class_map.end ())
    result.insert ((*f).second);
}

void
model_block_scope::look_up_name (const std::string &name,
				 std::set<model_variable_decl *> &result,
				 IContext *,
				 model_class *)
{
  std::map<std::string, model_variable_decl *>::const_iterator f
    = variable_map.find (name);
  if (f != variable_map.end ())
    result.insert ((*f).second);
}

void
model_block_scope::add_binding (model_variable_decl *var)
{
  std::map<std::string, model_variable_decl *>::const_iterator f
    = variable_map.find (var->get_name ());
  assert (f == variable_map.end ());
  variable_map[var->get_name ()] = var;
}

void
model_block_scope::add_binding (model_class *klass)
{
  // Anonymous classes don't get added.
  if (klass->get_name () != "")
    {
      std::map<std::string, model_class *>::const_iterator f
	= class_map.find (klass->get_name ());
      assert (f == class_map.end ());
      class_map[klass->get_name ()] = klass;
    }
}
