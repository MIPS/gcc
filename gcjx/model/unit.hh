// Represent a compilation unit.

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

#ifndef GCJX_MODEL_UNIT_HH
#define GCJX_MODEL_UNIT_HH

#include "source/lex.hh"

/// This is the base class of all compilation units.
class model_unit : public model_element, public IScope
{
protected:

  // Package for this unit.  There is a special object for when the
  // compilation unit is in the unnamed package.
  model_package *package;

  // Top-level classes and interfaces.
  // FIXME do we really own our types?
  std::list<ref_class> types;

  // File name associated with this compilation unit.
  std::string filename;

  // Lexer for this compilation unit.
  lexer *src_lexer;

  // True if we've been resolved.
  bool resolved;

  model_unit (const location &w)
    : model_element (w),
      package (NULL),
      src_lexer (NULL),
      resolved (false)
  {
  }

public:

  virtual ~model_unit ()
  {
    if (src_lexer)
      delete src_lexer;
  }

  void set_package (model_package *p)
  {
    package = p;
  }

  model_package *get_package () const
  {
    return package;
  }

  void set_file_name (const std::string &n)
  {
    filename = n;
  }

  std::string get_file_name () const
  {
    return filename;
  }

  void set_lexer (lexer *a_lexer)
  {
    src_lexer = a_lexer;
  }

  lexer *get_lexer () const
  {
    return src_lexer;
  }

  void add (const ref_class &typ)
  {
    types.push_back (typ);
    // The class might be a member type from a .class file, in which
    // case we don't want to register it with the package; we could
    // end up registering a duplicate that way.
    if (typ->get_declaring_class () == NULL)
      package->add (typ);
  }

  std::list<ref_class> get_types () const
  {
    return types;
  }

  /// Resolve this object, hooking up types as required.
  virtual void resolve (resolution_scope *) = 0;

  /// Find a class declared in this compilation unit.  The name will
  /// be a fully-qualified name as appears in a class file.
  virtual model_class *find_class_from_descriptor (resolution_scope *,
						   model_element *,
						   const std::string &)
  {
    abort ();
  }

  /// Check whether the imports in this compilation unit were used.
  virtual void check_imports ()
  {
  }

  /// Set up the resolution_scope so that this compilation unit is
  /// current.
  void push_on_scope (resolution_scope *scope)
  {
    scope->push_scope (this);
  }

  void visit (visitor *) = 0;
};

/// This represents a single .java source file.
class model_unit_source : public model_unit
{
  // Imports.
  std::list<ref_import> imports;

  typedef std::map<std::string, std::pair<model_import *, model_class *> >
    name_map_type;

  // Map simple names to types.  This is constructed during
  // resolution.
  name_map_type name_map;

  // A helper function when resolving.
  void check_dups (const std::string &, model_import *,
		   model_class *, name_map_type &);

public:

  model_unit_source (const location &w)
    : model_unit (w)
  {
  }

  void add (const ref_class &typ)
  {
    model_unit::add (typ);
  }

  void add (const ref_import &imp)
  {
    imports.push_back (imp);
  }

  // Find all the types referred to by the imports in this compilation
  // unit.  The big picture of resolution is that it is started at the
  // class level; if a class has a compilation unit then the unit is
  // resolved by one of the classes in the unit.
  void resolve (resolution_scope *);

  void check_imports ();

  void look_up_name (const std::string &name,
		     std::set<model_class *> &result,
		     IContext *request,
		     model_class *qualifier);

  void look_up_name (const std::string &name,
		     std::set<model_variable_decl *> &result,
		     IContext *request,
		     model_class *qualifier);

  void look_up_name (const std::string &name,
		     std::set<model_method *> &result,
		     IContext *request,
		     model_class *qualifier);

  void visit (visitor *);
};

/// This represents a single '.class' file.
class model_unit_class : public model_unit
{
public:

  // Objects of this type represent the InnerClasses attribute of
  // .class files.
  struct inner_class
  {
    // The name of the outer class, or empty for an anonymous or local
    // class.
    std::string outer;
    // The "plain" name of the inner class, or empty for anonymous
    // classes.
    std::string plain;
  };

protected:

  // The key is the full name of the inner class.
  std::map<std::string, inner_class> inners;

  // The constant pool associated with this file.
  constant_pool *cpool;

  // True if this class file is permitted to use extensions from 1.5.
  bool use_15;

  model_class *find_top_level_class (resolution_scope *, const std::string &,
				     bool = false);

  /// For subclass use only.
  model_unit_class (const location &w, constant_pool *pool)
    : model_unit (w),
      cpool (pool),
      use_15 (false)
  {
  }

public:

  model_unit_class (const location &w,
		    const std::map<std::string, inner_class> &ins,
		    constant_pool *pool)
    : model_unit (w),
      inners (ins),
      cpool (pool),
      use_15 (false)
  {
  }

  model_class *find_class_from_descriptor (resolution_scope *, model_element *,
					   const std::string &);

  void resolve (resolution_scope *);

  constant_pool *get_constant_pool () const
  {
    return cpool;
  }

  /// Call this to indicate that this class file may use 1.5
  /// extensions.
  void set_15 ()
  {
    use_15 = true;
  }

  /// Return true if this comes from a 1.5 class file.
  bool is_15_p () const
  {
    return use_15;
  }

  void visit (visitor *);
};

/// This class represents a "fake" .class-based compilation unit.
/// When we are verifying bytecode we just created, we make a dummy
/// .class compilation unit that knows how to map class descriptors to
/// actual classes, and that has its own constant pool.
class model_unit_fake : public model_unit_class
{
  /// Map class descriptors to classes.  There's no point to go
  /// through real lookup in this case, as we already have the needed
  /// classes available -- and if not, it represents a class writing
  /// bug which we should detect by *not* loading the class.
  std::map<std::string, model_class *> descriptor_map;

public:

  model_unit_fake (const location &w, constant_pool *pool,
		   const std::map<std::string, model_class *> &dm)
    : model_unit_class (w, pool),
      descriptor_map (dm)
  {
  }

  ~model_unit_fake ()
  {
    // We own our constant pool.
    delete cpool;
  }

  model_class *find_class_from_descriptor (resolution_scope *, model_element *,
					   const std::string &);

  void visit (visitor *);
};

#endif // GCJX_MODEL_UNIT_HH
