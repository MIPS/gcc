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

#include "typedefs.hh"
#include "header/cni.hh"
#include "aot/aotclass.hh"

#include <fstream>

cni_code_generator::cni_code_generator (compiler *c, directory_cache &dirs)
  : code_generator (dirs),
    comp (c),
    std_headers_ok (true)
{
}

void
cni_code_generator::emit_actions (std::ostream &out,
				  cni_code_generator::action what,
				  const action_item_list &items)
{
  bool any = false;
  for (action_item_list::const_iterator i = items.begin ();
       i != items.end ();
       ++i)
    {
      if ((*i).first != what)
	continue;

      out << "  ";
      if (what == CNI_FRIEND)
	out << "friend ";
      out << (*i).second << std::endl;
      any = true;
    }
  if (any)
    out << std::endl;
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
      out << "class " << klass->get_output_name () << ";" << std::endl;
    }

  move_to_package (out, current_package, base, indentation);

  out << "}" << std::endl;
  out << std::endl;
}

void
cni_code_generator::write_include (std::ostream &out, model_class *klass)
{
  std::string cname = klass->get_fully_qualified_name ();
  // Ugly...
  for (unsigned int i = 0; i < cname.length (); ++i)
    {
      if (cname[i] == '.')
	cname[i] = '/';
    }
  out << "#include <" << cname << ".h>" << std::endl;
}

void
cni_code_generator::write_includes (std::ostream &out,
				    model_class *klass,
				    const method_iterator &methods_begin,
				    const method_iterator &methods_end,
				    const std::vector<model_method *> &vtable,
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

  write_include (out, super ? super : comp->java_lang_Object ());

  // Add all our methods.
  for (method_iterator i = methods_begin; i != methods_end; ++i)
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

  // Add information from Miranda methods.
  for (std::vector<model_method *>::const_iterator i = vtable.begin ();
       i != vtable.end ();
       ++i)
    {
      if ((*i)->get_declaring_class () != klass
	  && ! (*i)->get_declaring_class ()->interface_p ())
	continue;

      // Ugly duplication here...
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
  if (meth->instance_initializer_p () || meth->static_initializer_p ()
      || (meth->get_modifiers () & ACC_SYNTHETIC) != 0)
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
    out << meth->get_declaring_class ()->get_output_name ();
  else
    {
      if (meth->static_p ())
	out << "static ";
      // Private methods don't get a vtable entry, but others do.
      // Interestingly, this makes CNI less efficient than the Java
      // C++ ABI in one case: calls to methods in a final class.
      // FIXME: decisions like this should be handled in a single
      // place, right now this logic is duplicated elsewhere.
      else if ((meth->get_modifiers () & ACC_PRIVATE) == 0)
	out << "virtual ";
      out << cxxname (meth->get_return_type ());
      if (! meth->get_return_type ()->reference_p ())
	out << " ";
      out << meth->get_name ();
      if (cxx_keyword_p (meth->get_name ()))
	out << "$";
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
				 modifier_t &current_flags,
				 bool &is_first,
				 const std::set<std::string> &method_names)
{
  modifier_t new_flags = field->get_modifiers () & ACC_ACCESS;
  update_modifiers (out, new_flags, current_flags);

  out << "  ";
  model_type *ftype = field->type ();
  if (field->static_p ())
    {
      out << "static ";
      if (field->constant_p () && ftype->integral_p ())
	out << "const ";
    }

  out << cxxname (ftype);
  if (! ftype->reference_p ())
    out << " ";
  if (is_first && ! field->static_p ())
    {
      is_first = false;
      out << "__attribute__ ((__aligned__ (__alignof ("
	  << cxxname (field->get_declaring_class ())
	  << ")))) ";
    }

  out << field->get_name ();
  if (method_names.find (field->get_name ()) != method_names.end ())
    out << "__";
  else if (cxx_keyword_p (field->get_name ()))
    out << "$";

  if (field->static_p () && field->constant_p () && ftype->integral_p ())
    {
      if (ftype == primitive_long_type)
	{
	  jlong val = jlong (field->get_initializer ()->value ());
	  out << " = " << val << "LL";
	}
      else
	{
	  model_primitive_base *bt
	    = assert_cast<model_primitive_base *> (primitive_int_type);
	  jint val = bt->convert (ftype, field->get_initializer ()->value ());
	  out << " = " << val << "L";
	}
    }

  out << ";" << std::endl;
}

void
cni_code_generator::add_action (cni_code_generator::action what,
				const std::string &name,
				const std::string &text)
{
  action_map_type::iterator it = action_map.find (name);
  if (it == action_map.end ())
    {
      action_item_list elt;
      elt.push_back (std::make_pair (what, text));
      action_map[name] = elt;
    }
  else
    (*it).second.push_back (std::make_pair (what, text));
}

void
cni_code_generator::generate (model_class *klass)
{
  std::string cname = klass->get_fully_qualified_name ();

  if (! std_headers_ok && (cname == "java.lang.Object"
			   || cname == "java.lang.Class"))
    return;

  std::string filename = directories.add (klass, ".h");
  std::ofstream out (filename.c_str ());

  if (global->get_compiler ()->verbose ())
    std::cout << " [writing " << filename << "]" << std::endl;

  action_map_type::const_iterator it = action_map.find (cname);
  bool found = it != action_map.end ();
  const action_item_list &actions (found ? (*it).second : empty_list);

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

  // Get fields for future use.
  std::list<ref_field> fields = klass->get_fields ();

  // We share the vtable layout code with the runtime.  This ensures
  // that the C++ compiler sees the same layout as gcj.
  aot_class *aotk = factory.get_class (klass);
  std::vector<model_method *> methods (aotk->get_vtable ());

  // Emit all needed #includes and namespace declarations.
  std::list<ref_method> local_methods = klass->get_methods ();
  write_includes (out, klass, local_methods.begin (), local_methods.end (),
		  methods, fields);
  out << std::endl;

  emit_actions (out, CNI_PREPEND, actions);

  out << "class " << cxxname (klass, false);
  if (klass->get_superclass ())
    out << " : public ::" << cxxname (klass->get_superclass (), false);
  else
    out << " : public ::java::lang::Object";
  out << std::endl;
  out << "{" << std::endl;

  // This ensures that we change the access for the very first member.
  modifier_t current_flags = ACC_ACCESS;

  std::set<std::string> method_names;
  std::set<model_method *> method_set;
  std::vector<model_method *>::const_iterator end = methods.end ();
  for (std::vector<model_method *>::const_iterator i = methods.begin ();
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
      write_method (out, *i, current_flags);
      method_names.insert ((*i)->get_name ());
      method_set.insert (*i);
    }

  // We have to make another pass to write out methods that don't
  // appear in the vtable.
  for (std::list<ref_method>::const_iterator i = local_methods.begin ();
       i != local_methods.end ();
       ++i)
    {
      if (method_set.find ((*i).get ()) == method_set.end ()
	  && ! (*i)->static_initializer_p ())
	write_method (out, (*i).get (), current_flags);
    }

  bool first = true;
  for (std::list<ref_field>::const_iterator i = fields.begin ();
       i != fields.end ();
       ++i)
    write_field (out, (*i).get (), current_flags, first, method_names);

  // One final phony field.
  update_modifiers (out, modifier_t (ACC_PUBLIC), current_flags);

  emit_actions (out, CNI_FRIEND, actions);
  emit_actions (out, CNI_ADD, actions);

  out << "  static ::java::lang::Class class$;" << std::endl;

  out << "}";
  if (klass->interface_p ())
    out << " __attribute__ ((__java_interface__))";

  out << ";" << std::endl
      << std::endl;

  emit_actions (out, CNI_APPEND, actions);

  out << "#endif // " << cname << std::endl;
}
