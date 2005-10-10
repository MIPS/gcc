// Write a CNI header.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
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

#ifndef GCJX_HEADER_CNI_HH
#define GCJX_HEADER_CNI_HH

#include "codegen.hh"
#include "directory.hh"
#include "aot/aotfactory.hh"

class cni_code_generator : public code_generator
{
public:

  /// This type is used when registering a header-modifying action.
  typedef enum
  {
    CNI_ADD,
    CNI_APPEND,
    CNI_FRIEND,
    CNI_PREPEND
  } action;

protected:

  // Handy typedefs.
  typedef std::list<ref_method>::const_iterator method_iterator;
  typedef std::list< std::pair<action, std::string> > action_item_list;
  typedef std::map<std::string, action_item_list> action_map_type;

  // The compiler we're using.
  compiler *comp;

  // This maps class names to action items.
  action_map_type action_map;

  // An empty list that we use as needed.
  action_item_list empty_list;

  // True if we should emit headers for Object and Class.
  bool std_headers_ok;

  // We use an AOT class factory to lay out the vtable for us.
  aot_class_factory factory;


  std::string cxxname (model_type *, bool = true);
  void update_modifiers (std::ostream &, modifier_t, modifier_t &);
  void add (model_type *, std::set<model_class *> &,
	    bool &, model_package *, model_package *,
	    model_package *, model_class *);
  void write_include (std::ostream &, model_class *);
  void write_includes (std::ostream &, model_class *,
		       const method_iterator &, const method_iterator &,
		       const std::vector<model_method *> &,
		       const std::list<ref_field> &);
  void write_method (std::ostream &, model_method *, modifier_t &);
  void write_field (std::ostream &, model_field *, modifier_t &, bool &,
		    const std::set<std::string> &);

  void indent (std::ostream &, int);
  void open_package (std::ostream &, model_package *, model_package *, int &);
  void move_to_package (std::ostream &, model_package *, model_package *,
			int &);
  void write_namespaces (std::ostream &, const std::set<model_class *> &);
  void emit_actions (std::ostream &, action, const action_item_list &);

public:

  /// Construct a new CNI code generator.  Argument is the compiler
  /// being used to generate code.
  cni_code_generator (compiler *, directory_cache &);
  
  /// This is called to generate code for a class and write it.
  void generate (model_class *);	

  /// Add an action of the given type.  NAME is the name of the class
  /// to which the action applies.  TEXT is the text to emit at the
  /// indicated position.
  void add_action (action task, const std::string &name,
		   const std::string &text);

  bool handles_class_p () const
  {
    return true;
  }

  void set_avoid_headers ()
  {
    std_headers_ok = false;
  }
};

#endif // GCJX_HEADER_CNI_HH
