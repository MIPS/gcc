// Parameters for a generic method or class.

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

#ifndef GCJX_MODEL_PARAMETERS_HH
#define GCJX_MODEL_PARAMETERS_HH

class model_class;

/// This represents type parameters for a class or a method.
class model_parameters : public IScope
{
  // Type parameters, or empty list if none.
  std::list<ref_type_variable> type_parameters;

  // FIXME: simplest to just give model_class full access.
  friend class model_class;
  friend class model_class_instance;

public:

  model_parameters ()
  {
  }

  void set_type_parameters (const std::list<ref_type_variable> &ts)
  {
    type_parameters = ts;
  }

  std::list<ref_type_variable>::const_iterator begin () const
  {
    return type_parameters.begin ();
  }

  std::list<ref_type_variable>::const_iterator end () const
  {
    return type_parameters.end ();
  }

  bool empty () const
  {
    return type_parameters.empty ();
  }

  unsigned int size () const
  {
    return type_parameters.size ();
  }

  /// Create a type map that maps our type variables onto the other
  /// type variables.  Returns true if this was successful, false
  /// otherwise.
  bool create_type_map (model_type_map &, const model_parameters &) const;

  void create_type_map (model_type_map &, model_element *,
			const std::list<model_class *> &);

  void resolve_classes (resolution_scope *scope);

  void look_up_name (const std::string &name,
		     std::set<model_class *> &result,
		     IContext *request,
		     model_class *qualifier);

  /// Return a string representing these parameters in a way that the
  /// user would understand.
  std::string get_pretty_name () const;

  /// Return a signature that represents these type parameters.
  std::string get_signature ();
};

/// This is a cache of instances of a given class, either a
/// model_class_instance or a model_method.
template<typename T>
class model_instance_cache
{
  // FIXME: this implies copying the type map...
  typedef std::list< std::pair< model_type_map, owner<T> > >
    data_type;

  // List of all the known instances.
  data_type instances;

public:

  /// Look up an instantiation.  If the element is found in the cache,
  /// returns it.  Otherwise, returns NULL.
  T *find_instance (const model_type_map &type_map);

  /// Add an element to the cache.
  void add_instance (const model_type_map &type_map,
		     const owner<T> &instance);
};

#endif // GCJX_MODEL_PARAMETERS_HH
