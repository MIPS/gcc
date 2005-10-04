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
#include "buffer.hh"

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
tree_code_generator::compile_resource (const std::string &name,
				       byte_buffer *contents)
{
  char *buf = new char[name.length () + contents->get_length ()];
  strcpy (buf, name.c_str ());
  // We intentionally copy over the \0 byte here.
  memcpy (buf + name.length (), contents->get (), contents->get_length ());
  compile_resource_data (name.c_str (), buf, (int) contents->get_length ());
  // FIXME: can we delete BUF here?  For now just leak.
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
      // No need to do anything for abstract methods.
      // FIXME: Or... set line number??
      // We could generate stubs that throw an exception with
      // nice information...
      if (((*i)->get_modifiers () & ACC_ABSTRACT) != 0)
	continue;

      tree_generator gen (builtins, wrapper);
      tree method = gen.generate ((*i).get ());

      current_function_decl = method;
      allocate_struct_function (method);

      location end_loc = (*i)->get_method_end ();
      // FIXME: most likely bogus.
      location_t gcc_loc;
      LOCATION_FILE (gcc_loc) = end_loc.get_file ();
      LOCATION_LINE (gcc_loc) = end_loc.get_line ();
      if (LOCATION_LINE (gcc_loc) < 0)
	LOCATION_LINE (gcc_loc) = 0;
      cfun->function_end_locus = gcc_loc;

      dump_function (TDI_original, method);
      gimplify_function_tree (method);
      dump_function (TDI_generic, method);
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
      DECL_EXTERNAL (field) = 0;
      cgraph_varpool_finalize_decl (field);
    }

  class_object_creator creator (builtins, wrapper, TREE_TYPE (class_tree));
  tree class_obj = creator.get_class ();
  class_list = tree_cons (NULL_TREE, class_obj, class_list);
}
