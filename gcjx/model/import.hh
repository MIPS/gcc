// Represent an import statement.

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

#ifndef GCJX_MODEL_IMPORT_HH
#define GCJX_MODEL_IMPORT_HH

// Base class of all import statements.
class model_import : public model_element
{
protected:

  // The text of the import.  For on-demand imports, the trailing "*"
  // is omitted.
  std::list<std::string> name;

  // True if this import has been referenced.
  bool used;

  static bool java_lang_p (Iname *);

  model_import (const location &w, const std::list<std::string> &n)
    : model_element (w),
      name (n),
      used (false)
  {
  }

public:

  // Resolve this import, connecting it to actual types if needed.
  virtual void resolve (resolution_scope *) = 0;

  virtual model_class *find_class (const std::string &name) = 0;

  virtual void find_field (const std::string &, IContext *,
			   std::set<model_variable_decl *> &)
  {
    // Nothing.
  }

  virtual void find_method (const std::string &, IContext *,
			    std::set<model_method *> &)
  {
    // Nothing.
  }

  virtual bool single_import_p () const = 0;

  virtual std::string get_simple_name () const
  {
    assert (single_import_p ());
    return ::get_simple_name (name);
  }

  std::string get_name () const
  {
    return join (name, '.');
  }

  virtual model_class *get_class_declaration () const
  {
    return NULL;
  }

  /// Indicate that this import has been referenced.
  void set_used ()
  {
    used = true;
  }

  /// Check whether we've been referenced and emit a warning if not.
  void check_referenced ();

  void visit (visitor *) = 0;
};

// "import foo"
class model_import_single : public model_import
{
  // The type to which we resolve.
  model_class *resolved_type;

public:

  model_import_single (const location &w, const std::list<std::string> &n)
    : model_import (w, n),
      resolved_type (NULL)
  {
  }

  void resolve (resolution_scope *);
  model_class *find_class (const std::string &name);

  model_class *get_class_declaration () const
  {
    return resolved_type;
  }

  bool single_import_p () const
  {
    return true;
  }

  void visit (visitor *);
};

// "import foo.*"
class model_import_on_demand : public model_import
{
  // The resolved package or type.
  Iname *resolved_type;

  // True if this is the implicit 'java.lang.*' import.
  bool implicit;

public:

  model_import_on_demand (const location &w,
			  const std::list<std::string> &n,
			  bool isimp = false)
    : model_import (w, n),
      resolved_type (NULL),
      implicit (isimp)
  {
    if (implicit)
      set_used ();
  }

  void resolve (resolution_scope *);
  model_class *find_class (const std::string &name);

  bool single_import_p () const
  {
    return false;
  }

  void visit (visitor *);
};

/// Base class for both types of static import.
class model_static_import_base : public model_import
{
protected:

  // The type to which the base name resolves resolve.
  model_class *resolved_type;

  // Helper for subclasses.
  model_class *search_for_class (const std::string &) const;

public:

  model_static_import_base (const location &w,
			    const std::list<std::string> &n)
    : model_import (w, n),
      resolved_type (NULL)
  {
  }

  void resolve (resolution_scope *);

  void find_field (const std::string &, IContext *,
		   std::set<model_variable_decl *> &);

  void find_method (const std::string &, IContext *,
		    std::set<model_method *> &);

  void visit (visitor *) = 0;
};

// "import static foo.bar"
class model_static_import_single : public model_static_import_base
{
  // Name of the method or field to import.
  std::string member_name;

public:

  model_static_import_single (const location &w,
			      const std::list<std::string> &n)
    : model_static_import_base (w, drop_last_element (n))
  {
    member_name = ::get_simple_name (n);
  }

  std::string get_simple_name () const
  {
    return member_name;
  }

  void resolve (resolution_scope *);

  model_class *find_class (const std::string &name);

  void find_field (const std::string &, IContext *,
		   std::set<model_variable_decl *> &);

  void find_method (const std::string &, IContext *,
		    std::set<model_method *> &);

  model_class *get_class_declaration () const;

  bool single_import_p () const
  {
    return true;
  }

  void visit (visitor *);
};

// "import static foo.bar.*"
class model_static_import_on_demand : public model_static_import_base
{
public:

  model_static_import_on_demand (const location &w,
				 const std::list<std::string> &n)
    : model_static_import_base (w, n)
  {
  }

  model_class *find_class (const std::string &name);

  bool single_import_p () const
  {
    return false;
  }

  void visit (visitor *);
};

#endif // GCJX_MODEL_IMPORT_HH
