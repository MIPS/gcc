// Represent a method's 'throws' clause.

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

#ifndef GCJX_MODEL_THROWSCLAUSE_HH
#define GCJX_MODEL_THROWSCLAUSE_HH

/// This represents the 'throws' clause of a method declaration.  It
/// holds a list of types that are declared as being thrown.
class model_throws_clause
{
  // The types.
  std::list<ref_forwarding_type> decls;

public:

  // Default constructor.
  model_throws_clause ()
  {
  }

  /// This constructor creates a new 'throws' clause by applying a
  /// type map.
  model_throws_clause (const model_throws_clause &, model_element *,
		       const model_type_map &);

  /// This constructor will merge all the throws clauses from a set of
  /// methods.
  model_throws_clause (const std::set<model_method *> &);

  /// Set the list of types for this throws clause.
  model_throws_clause &operator= (const std::list<ref_forwarding_type> &n)
  {
    decls = n;
    return *this;
  }

  /// Return the list of throws as forwarding types.
  std::list<ref_forwarding_type> get () const
  {
    return decls;
  }

  /// Return the list of throws as a set of types.
  std::set<model_type *> get_as_set () const;

  /// Add a type to the throws clause.
  void add (const ref_forwarding_type &t)
  {
    decls.push_back (t);
  }

  /// Resolve the throws clause.
  void resolve (resolution_scope *);

  /// Return true if the type matches one of the throws items.
  bool handled_p (model_type *) const;

  /// Return true if every checked exception in this throws clause is
  /// also in the argument throws clause.
  bool subset_p (const model_throws_clause &) const;

  /// Propagate 'throws' clauses to the current catcher.
  void propagate_throws (resolution_scope *);

  /// Return a 1.5-style signature for this clause.
  std::string get_signature ();
};

#endif // GCJX_MODEL_THROWSCLAUSE_HH
