// Write a CNI header.

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
#include "header/cni.hh"

#include <fstream>

cni_code_generator::cni_code_generator (compiler *c, directory_cache &dirs)
  : code_generator (dirs),
    comp (c)
{
}

std::string
cni_code_generator::cxxname (model_type *type, bool leading)
{
  std::string result;

  int count = 0;
  while (type->array_p ())
    {
      assert (leading);
      ++count;
      type = type->element_type ();
    }

  if (type->reference_p ())
    {
      model_class *klass = assert_cast<model_class *> (type);
      if (leading)
	result = "::";

      std::string qn = klass->get_fully_qualified_name ();
      for (unsigned int i = 0; i < qn.length (); ++i)
	{
	  if (qn[i] == '.')
	    result += "::";
	  else
	    result += qn[i];
	}

      if (leading)
	result += " *";
    }
  else if (type == primitive_void_type)
    result = "void";
  else
    result = "j" + type->get_pretty_name ();

  while (count > 0)
    {
      --count;
      // We use "< ::" to avoid trigraph troubles.
      result = "JArray< " + result + "> *";
    }

  return result;
}

void
cni_code_generator::update_modifiers (std::ostream &out,
				      modifier_t new_flags,
				      modifier_t &old_flags)
{
  if (new_flags != old_flags)
    {
      switch (new_flags)
	{
	case 0:
	  out << "public:  // actually package-private" << std::endl;
	  break;
	case ACC_PUBLIC:
	  out << "public:" << std::endl;
	  break;
	case ACC_PRIVATE:
	  out << "private:" << std::endl;
	  break;
	case ACC_PROTECTED:
	  out << "public:  // actually protected" << std::endl;
	  break;
	default:
	  abort ();
	}
      old_flags = new_flags;
    }
}

void
cni_code_generator::add (model_type *type,
			 std::set<model_class *> &class_set,
			 bool &array_seen,
			 model_package *lang_package,
			 model_package *util_package,
			 model_package *io_package,
			 model_class *super)
{
  while (type->array_p ())
    {
      array_seen = true;
      type = type->element_type ();
    }

  if (! type->reference_p ())
    return;
  model_class *k = assert_cast<model_class *> (type);
  if (k == super)
    return;

  // Check package and parent package.
  model_package *pack = k->get_package ();
  for (int i = 0; pack && i < 2; ++i, pack = pack->get_parent ())
    {
      if (pack == lang_package || pack == util_package || pack == io_package)
	return;
    }

  // At this point we have a class type which isn't automatically
  // declared.  So add it.
  class_set.insert (k);
}

void
cni_code_generator::indent (std::ostream &out, int howmuch)
{
  for (int i = 0; i < howmuch; ++i)
    out << " ";
}

void
cni_code_generator::open_package (std::ostream &out,
				  model_package *old_package,
				  model_package *new_package,
				  int &indentation)
{
  if (old_package == new_package)
    return;

  open_package (out, old_package, new_package->get_parent (), indentation);

  indent (out, indentation);
  out << "namespace " << new_package->get_simple_name () << std::endl;
  indent (out, indentation);
  out << "{" << std::endl;

  indentation += 2;
}

void
cni_code_generator::move_to_package (std::ostream &out,
				     model_package *old_package,
				     model_package *new_package,
				     int &indentation)
{
  assert (old_package != new_package);

  model_package *base = global->get_compiler ()->get_primordial_package ();

  // First, close packages until we get to some ancestor of the new
  // package.
  while (old_package != base)
    {
      model_package *p;
      for (p = new_package;
	   p != base && p != old_package;
	   p = p->get_parent ())
	;
      if (p == old_package)
	break;

      old_package = old_package->get_parent ();
      indentation -= 2;
      indent (out, indentation);
      out << "}" << std::endl;
    }

  // Now, open new packages until we get down to the new package.
  open_package (out, old_package, new_package, indentation);
}

void
cni_code_generator::write_namespaces (std::ostream &out,
				      const std::set<model_class *> &class_set)
{
  assert (! class_set.empty ());

  // Note that std::map<> is a sorted container.  We rely on this.
  std::map<std::string, model_class *> names;
  for (std::set<model_class *>::const_iterator i = class_set.begin ();
       i != class_set.end ();
       ++i)
    names[(*i)->get_pretty_name ()] = *i;

  out << std::endl;
  out << "extern \"Java\"" << std::endl;
  out << "{" << std::endl;

  model_package *current_package
    = global->get_compiler ()->get_primordial_package ();
  model_package *base = current_package;
  int indentation = 2;
  for (std::map<std::string, model_class *>::const_iterator i = names.begin ();
       i != names.end ();
       ++i)
    {
      model_class *klass = (*i).second;
      if (current_package != klass->get_package ())
	{
	  move_to_package (out, current_package, klass->get_package (),
			   indentation);
	  current_package = klass->get_package ();
	}

      indent (out, indentation);
      out << "class " << klass->get_name () << ";" << std::endl;
    }

  move_to_package (out, current_package, base, indentation);

  out << "}" << std::endl;
  out << std::endl;
}

void
cni_code_generator::write_includes (std::ostream &out,
				    model_class *klass,
				    const AllMethodsIterator & methods_begin,
				    const AllMethodsIterator & methods_end,
				    const std::list<ref_field> &fields)
{
  // These classes are chosen since they are already in 'compiler' and
  // come from the packages we have decided are listed by default.
  // See libjava/gcj/javaprims.h.
  model_package *lang_package = comp->java_lang_Object ()->get_package ();
  model_package *util_package = comp->java_util_List ()->get_package ();
  model_package *io_package = comp->java_io_Serializable ()->get_package ();

  bool array_seen = false;
  model_class *super = klass->get_superclass ();
  std::set<model_class *> class_set;

  // Add the class itself so it shows up in the declarations.
  add (klass, class_set, array_seen, lang_package,
       util_package, io_package, super);

  if (super)
    {
      std::string cname = super->get_fully_qualified_name ();
      // Ugly...
      for (unsigned int i = 0; i < cname.length (); ++i)
	{
	  if (cname[i] == '.')
	    cname[i] = '/';
	}
      out << "#include <" << cname
	  << ".h>" << std::endl;
    }

  for (AllMethodsIterator i = methods_begin; i != methods_end; ++i)
    {
      add ((*i)->get_return_type (), class_set, array_seen,
	   lang_package, util_package, io_package, super);

      std::list<ref_variable_decl> parms = (*i)->get_parameters ();
      for (std::list<ref_variable_decl>::const_iterator j = parms.begin ();
	   j != parms.end ();
	   ++j)
	add ((*j)->type (), class_set, array_seen,
	     lang_package, util_package, io_package, super);
    }

  for (std::list<ref_field>::const_iterator i = fields.begin ();
       i != fields.end ();
       ++i)
    add ((*i)->type (), class_set, array_seen,
	 lang_package, util_package, io_package, super);

  if (array_seen)
    out << "#include <gcj/array.h>" << std::endl;

  if (! class_set.empty ())
    write_namespaces (out, class_set);
}

void
cni_code_generator::write_method (std::ostream &out,
				  model_method *meth,
				  modifier_t &current_flags)
{
  if (meth->instance_initializer_p () || meth->static_initializer_p ())
    return;

  modifier_t new_flags = meth->get_modifiers () & ACC_ACCESS;
  update_modifiers (out, new_flags, current_flags);

  out << "  ";

#if 0
  // FIXME: not clear if this is a good idea, so for now I'm
  // commenting it out.  If this is enabled we should also allow for
  // deprecation of fields and classes.
  if (meth->deprecated_p ())
    out << "__attribute__ ((__deprecated__)) ";
#endif

  if (meth->constructor_p ())
    out << meth->get_declaring_class ()->get_name ();
  else
    {
      if (meth->static_p ())
	out << "static ";
      else if (! ((meth->get_modifiers () & (ACC_FINAL | ACC_PRIVATE)) != 0
		  || ((meth->get_declaring_class ()->get_modifiers ()
		       & ACC_FINAL) != 0)))
	out << "virtual ";
      out << cxxname (meth->get_return_type ());
      if (! meth->get_return_type ()->reference_p ())
	out << " ";
      out << meth->get_name ();
    }
  out << " (";

  std::list<ref_variable_decl> parms = meth->get_parameters ();
  bool first = true;
  for (std::list<ref_variable_decl>::const_iterator i = parms.begin ();
       i != parms.end ();
       ++i)
    {
      if (! first)
	out << ", ";
      first = false;
      out << cxxname ((*i)->type ());
    }

  out << ")";
  if (meth->abstract_p ())
    out << " = 0";
  out << ";" << std::endl;
}

void
cni_code_generator::write_field (std::ostream &out,
				 model_field *field,
				 modifier_t &current_flags)
{
  modifier_t new_flags = field->get_modifiers () & ACC_ACCESS;
  update_modifiers (out, new_flags, current_flags);

  out << "  ";
  if (field->static_p ())
    out << "static ";
  out << cxxname (field->type ());
  if (! field->type ()->reference_p ())
    out << " ";
  out << field->get_name () << ";" << std::endl;
}

void
cni_code_generator::generate (model_class *klass)
{
  std::string filename = directories.add (klass, ".h");
  std::ofstream out (filename.c_str ());

  if (global->get_compiler ()->verbose ())
    std::cout << " [writing " << filename << "]" << std::endl;

  std::string cname = klass->get_fully_qualified_name ();
  // Ugly...
  for (unsigned int i = 0; i < cname.length (); ++i)
    {
      if (cname[i] == '$' || cname[i] == '.')
	cname[i] = '_';
    }
  cname = "__" + cname + "__";

  out << "// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-"
      << std::endl
      << std::endl;

  out << "#ifndef " << cname << std::endl
      << "#define " << cname << std::endl
      << std::endl;

  std::list<ref_field> fields = klass->get_fields ();
  write_includes (out, klass, klass->begin_all_methods (),
                  klass->end_all_methods (), fields);
  out << std::endl;

  out << "class " << cxxname (klass, false);
  if (klass->get_superclass ())
    out << " : public ::" << cxxname (klass->get_superclass (), false);
  out << std::endl;
  out << "{" << std::endl;

  modifier_t current_flags = 0;

  AllMethodsIterator end = klass->end_all_methods ();
  for (AllMethodsIterator i = klass->begin_all_methods ();
       i != end;
       ++i)
    {
      // If the method is defined in a superclass, we don't need to do
      // anything.  But if it is defined in an interface, we must emit
      // it, since interfaces are not visible in the header.  It is ok
      // to redeclare a method that is declared in an interface and
      // already inherited via some superclass, since the C++ compiler
      // will recognize that as overriding.
      if ((*i)->get_declaring_class () != klass
	  && ! (*i)->get_declaring_class ()->interface_p ())
	continue;
      write_method (out, (*i).get (), current_flags);
    }

  for (std::list<ref_field>::const_iterator i = fields.begin ();
       i != fields.end ();
       ++i)
    write_field (out, (*i).get (), current_flags);

  // One final phony field.
  update_modifiers (out, modifier_t (ACC_PUBLIC), current_flags);
  out << "  static ::java::lang::Class class$;" << std::endl;

  out << "};" << std::endl
      << std::endl;

  out << "#endif // " << cname << std::endl;
}
