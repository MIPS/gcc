// Code generator for GENERIC.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// GCC is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// GCC is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.

#include "java/glue.hh"

#include "codegen.hh"
#include "java/treegen.hh"
#include "java/tree.hh"

tree_code_generator::tree_code_generator (compiler *, directory_cache &dirs)
  : code_generator (dirs),
    builtins (new tree_builtins ()),
    first_time (true)
{
}

tree_code_generator::~tree_code_generator ()
{
  delete builtins;
}

void
tree_code_generator::generate (model_class *the_class)
{
  // The first time we're called to compile a class, we ask the
  // builtin object to initialize its map.
  if (first_time)
    {
      builtins->initialize_type_map ();
      first_time = false;
    }

  aot_class *wrapper = builtins->get_class (the_class);
  tree class_tree = builtins->lay_out_class (the_class);

  std::list<ref_method> methods = the_class->get_methods ();
  for (std::list<ref_method>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    {
      tree_generator gen (builtins, wrapper);
      tree method = gen.generate ((*i).get ());
      cgraph_finalize_function (method, 0);
    }

  std::list<ref_field> fields = the_class->get_fields ();
  for (std::list<ref_field>::const_iterator i = fields.begin ();
       i != fields.end ();
       ++i)
    {
      // Instance fields are handled in a different way.
      if (! (*i)->static_p ())
	continue;

      tree field = builtins->map_field ((*i).get ());
      cgraph_varpool_finalize_decl (field);
    }

  // FIXME: write out the class object now.
}
