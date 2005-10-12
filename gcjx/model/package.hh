// Representation of a package.

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

#ifndef GCJX_MODEL_PACKAGE_HH
#define GCJX_MODEL_PACKAGE_HH

class model_package : public model_element, public Iname, public IContext,
		      public IAnnotatable
{
protected:

  // The fully-qualified name of this package.  For the unnamed
  // package, this is empty.
  std::list<std::string> name;

  // The members of a package are all subpackages and all top-level
  // class and interface types declared in all the compilation units
  // of the package.
  std::map< std::string, owner<model_package> > subpackages;
  std::map<std::string, ref_class> types;

  // This is a cache of all class names we've been requested to find.
  // This is used to speed up lookups so we don't continually search
  // for "java.java".
  std::map<std::string, model_class *> name_cache;

  // Parent of this package, if any.
  model_package *parent;


  annotation_kind get_annotation_kind () const
  {
    // FIXME: throw exception if we're setting annotations for the second
    // time.
    // FIXME: we don't resolve our annotations anywhere!
    return ANNOTATE_PACKAGE;
  }

public:

  model_package (const location &w)
    : model_element (w),
      parent (NULL)
  {
  }

  void add (const owner<model_package> &pkg)
  {
    std::string n = ::get_simple_name (pkg->name);
    assert (types.find (n) == types.end ());
    assert (subpackages.find (n) == subpackages.end ());
    subpackages[n] = pkg;
    pkg->parent = this;
  }

  void add (const ref_class &);

  void set_name (const std::list<std::string> &fqname)
  {
    name = fqname;
  }

  bool package_p () const
  {
    return true;
  }

  model_package *find_package (const std::string &);
  model_class *find_class (const std::string &);

  Iname *find_name (const std::string &);

  // From IContext.
  model_package *get_package () const
  {
    return (model_package *) this;
  }

  // From IContext.  A package never has a lexically enclosing class.
  model_class *get_lexically_enclosing_class () const
  {
    return NULL;
  }

  model_package *get_parent () const
  {
    return parent;
  }

  std::string get_simple_name () const
  {
    return name.empty () ? std::string () : ::get_simple_name (name);
  }

  std::string get_fully_qualified_name () const
  {
    return join (name, '.');
  }

  std::string get_signature_fragment () const
  {
    return join (name, '/');
  }

  void visit (visitor *);
};

class model_unnamed_package : public model_package
{
public:

  model_unnamed_package ()
    : model_package (LOCATION_UNKNOWN)
  {
  }

  void visit (visitor *);
};

class model_primordial_package : public model_package
{
public:

  model_primordial_package ()
    : model_package (LOCATION_UNKNOWN)
  {
  }

  void visit (visitor *);
};

const format &operator% (const format &fmt, model_package *pkg);

#endif // GCJX_MODEL_PACKAGE_HH
