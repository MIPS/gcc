// Write a class file.

// Copyright (C) 2004, 2005, 2006 Free Software Foundation, Inc.
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
#include "bytecode/cpool.hh"
#include "bytecode/classwriter.hh"
#include "bytecode/locals.hh"
#include "bytecode/block.hh"
#include "bytecode/attribute.hh"
#include "bytecode/generate.hh"
#include "bytecode/byteutil.hh"
#include "bytecode/bridge.hh"

#include <fcntl.h>

class_writer::class_writer (model_class *k)
  : the_class (k),
    writer (),
    pool (NULL),
    target_15 (global->get_compiler ()->target_15 ())
{
}

class_writer::~class_writer ()
{
  delete pool;
}

void
class_writer::check_type (model_element *request, model_type *type)
{
  int n_dims = 0;
  for (; type->array_p (); type = type->element_type ())
    ++n_dims;
  if (n_dims > 255)
    throw request->error ("class file format only allows arrays with "
			  "a maximum of 255 dimensions");
}

void
class_writer::classify_annotations (const std::list<ref_annotation> &annos,
				    std::list<model_annotation *> &class_annos,
				    std::list<model_annotation *> &runtime_annos)
{
  for (std::list<ref_annotation>::const_iterator i = annos.begin ();
       i != annos.end ();
       ++i)
    {
      model_annotation_type *t
	= assert_cast<model_annotation_type *> ((*i)->type ());
      annotation_retention policy = t->get_retention_policy ();

      if (policy == RETAIN_SOURCE)
	{
	  // Nothing.
	}
      else if (policy == RETAIN_CLASS)
	class_annos.push_back ((*i).get ());
      else
	{
	  assert (policy == RETAIN_RUNTIME);
	  runtime_annos.push_back ((*i).get ());
	}
    }
}

void
class_writer::maybe_write_annotations (IAnnotatable *element,
				       bytecode_attribute_list *attrs)
{
  std::list<ref_annotation> annos = element->get_annotations ();
  std::list<model_annotation *> class_annos, runtime_annos;

  classify_annotations (annos, class_annos, runtime_annos);

  if (! class_annos.empty ())
    attrs->push_back (new annotation_attribute (pool,
						"RuntimeInvisibleAnnotations",
						class_annos));

  if (! runtime_annos.empty ())
    attrs->push_back (new annotation_attribute (pool,
						"RuntimeVisibleAnnotations",
						runtime_annos));
}

void
class_writer::maybe_write_parameter_annotations (model_method *meth,
						 bytecode_attribute_list *attrs)
{
  std::list<ref_variable_decl> params = meth->get_parameters ();
  bool any_class = false;
  bool any_runtime = false;
  std::list< std::list<model_annotation *> > all_class, all_runtime;
  for (std::list<ref_variable_decl>::const_iterator i = params.begin ();
       i != params.end ();
       ++i)
    {
      std::list<ref_annotation> annos = (*i)->get_annotations ();
      std::list<model_annotation *> class_annos, runtime_annos;
      classify_annotations (annos, class_annos, runtime_annos);

      all_class.push_back (class_annos);
      all_runtime.push_back (runtime_annos);

      if (! class_annos.empty ())
	any_class = true;
      if (! runtime_annos.empty ())
	any_runtime = true;
    }

  if (any_class)
    attrs->push_back (new parameter_attribute (pool,
					       "RuntimeInvisibleParameterAnnotations",
					       all_class));

  if (any_runtime)
    attrs->push_back (new parameter_attribute (pool,
					       "RuntimeVisibleParameterAnnotations",
					       all_runtime));
}

void
class_writer::write_field (model_field *field,
			   bytecode_attribute_list *attrs)
{
  int mods = field->get_modifiers ();
  if (! target_15)
    mods &= ~ACC_SYNTHETIC;
  writer.put2 (mods);

  // The items are added to the pool above, so this doesn't cause
  // the pool to actually change.
  writer.put2 (pool->add_utf (field->get_name ()));
  writer.put2 (pool->add_utf (field->type ()->get_descriptor ()));

  attrs->emit (writer);
}

void
class_writer::write_method (model_method *meth, bytecode_attribute_list *attrs,
			    bool is_bridge)
{
  int mods;
  if (is_bridge)
    {
      meth = meth->get_override ();
      mods = ACC_BRIDGE | meth->get_modifiers ();
    }
  else
    mods = meth->get_modifiers ();

  if (target_15)
    {
      if (meth->varargs_p ())
	mods |= ACC_VARARGS;	// FIXME: correct for bridge methods?
    }
  else
    mods &= ~ACC_SYNTHETIC;
  writer.put2 (mods);

  writer.put2 (pool->add_utf (meth->get_name ()));
  writer.put2 (pool->add_utf (meth->get_descriptor ()));

  attrs->emit (writer);
}

void
class_writer::write (directory_cache &dircache)
{
  std::string outname = dircache.add (the_class, ".class");
  if (global->get_compiler ()->verbose ())
    std::cout << " [writing " << outname << "]" << std::endl;

  writer.put4 (MAGIC);

  writer.put2 (target_15 ? MINOR_1_5 : MINOR_1_4);
  writer.put2 (target_15 ? MAJOR_1_5 : MAJOR_1_4);

  pool = new outgoing_constant_pool (the_class, writer);

  int class_index = pool->add (the_class);

  model_class *super = the_class->get_superclass ();
  // In the .class file, an interface must have Object as its
  // superclass.  Internally we represent interfaces as not having a
  // superclass, so we fix this up here.
  if (the_class->interface_p ())
    super = global->get_compiler ()->java_lang_Object ();
  int super_index = super ? pool->add (super) : 0;

  std::list<ref_forwarding_type> &ifaces = the_class->get_interfaces ();
  std::list<int> interface_indices;
  for (std::list<ref_forwarding_type>::const_iterator i = ifaces.begin ();
       i != ifaces.end ();
       ++i)
    interface_indices.push_back (pool->add ((*i)->type ()));

  // Attributes we handle.
  bytecode_attribute_list class_attributes;

  if (the_class->deprecated_p ())
    class_attributes.push_back (new simple_name_attribute (pool,
							   "Deprecated"));
  if (target_15)
    {
      std::string sig = the_class->get_class_signature ();
      // For Object this will be the empty string.
      if (sig != "")
	class_attributes.push_back (new utf8_attribute (pool, "Signature",
							sig));
    }

  if (global->get_compiler ()->target_debug_source ())
    {
      std::string filename
	= the_class->get_compilation_unit ()->get_file_name ();
      unsigned int ri = filename.rfind (FILE_SEPARATOR);
      std::string basename = (ri == std::string::npos
			      ? filename
			      : filename.substr (ri + 1));
      bytecode_attribute *attr = new utf8_attribute (pool, "SourceFile",
						     basename);
      class_attributes.push_back (attr);
    }

  maybe_write_annotations (the_class, &class_attributes);

  // First we generate code for the methods.  Then we write the bytes
  // for the fields and the methods in a separate step.  We do this
  // because code generation might require adding a new field.

  std::list<ref_method> methods = the_class->get_methods ();
  std::list<bytecode_attribute_list *> method_attrs;
  int method_count = 0;
  for (std::list<ref_method>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    {
      bytecode_attribute_list *attrs = new bytecode_attribute_list ();
      ++method_count;

      // One last check on the method.
      std::list<ref_variable_decl> params = (*i)->get_parameters ();
      int len = 0;
      if (! (*i)->static_p ())
	++len;
      for (std::list<ref_variable_decl>::const_iterator j = params.begin ();
	   j != params.end ();
	   ++j)
	{
	  check_type ((*i).get (), (*j)->type ());
	  len += wide_p ((*j)->type ()) ? 2 : 1;
	}
      if (len > 255)
	throw (*i)->error ("method requires more than 255 words of arguments");

      check_type ((*i).get (), (*i)->get_return_type ());

      pool->add_utf ((*i)->get_name ());
      pool->add_utf ((*i)->get_descriptor ());
      if (! (*i)->abstract_p () && ! (*i)->native_p ())
	{
	  bytecode_generator *gen = new bytecode_generator ((*i).get (), pool);
	  gen->generate ();
	  attrs->push_back (new code_attribute (pool, gen));
	}

      if (target_15)
	attrs->push_back (new utf8_attribute (pool, "Signature",
					      (*i)->get_signature ()));

      if (! target_15 && (*i)->synthetic_p ())
	attrs->push_back (new simple_name_attribute (pool, "Synthetic"));
      if ((*i)->deprecated_p ())
	attrs->push_back (new simple_name_attribute (pool, "Deprecated"));

      std::list<ref_forwarding_type> excs = (*i)->get_throws ();
      if (! excs.empty ())
	attrs->push_back (new exceptions_attribute (pool, excs));

      maybe_write_parameter_annotations ((*i).get (), attrs);
      maybe_write_annotations ((*i).get (), attrs);

      method_attrs.push_back (attrs);

      // If this method needs a bridge, create it and push it on the
      // list here.
      if (target_15 && bridge_method::required_p ((*i).get ()))
	{
	  attrs = new bytecode_attribute_list ();
	  attrs->push_back (new bridge_method (pool, (*i).get ()));
	  method_attrs.push_back (attrs);
	  ++method_count;

	  model_method *m = (*i)->get_override ();
	  pool->add_utf (m->get_name ());
	  pool->add_utf (m->get_descriptor ());
	}
    }

  std::list<ref_field> fields = the_class->get_fields ();
  std::list<bytecode_attribute_list *> field_attrs;
  for (std::list<ref_field>::const_iterator i = fields.begin ();
       i != fields.end ();
       ++i)
    {
      check_type ((*i).get (), (*i)->type ());

      // We don't bother saving the indices here, we just recompute
      // them later.  This is ok since the constant pool caches the
      // values.
      pool->add_utf ((*i)->get_name ());
      pool->add_utf ((*i)->type ()->get_descriptor ());

      bytecode_attribute_list *attrs = new bytecode_attribute_list ();

      if ((*i)->constant_p ())
	attrs->push_back (new field_value_attribute (pool, (*i).get ()));
      if (target_15)
	attrs->push_back (new utf8_attribute (pool, "Signature",
					      (*i)->get_signature ()));
      if (! target_15 && (*i)->synthetic_p ())
	attrs->push_back (new simple_name_attribute (pool, "Synthetic"));
      if ((*i)->deprecated_p ())
	attrs->push_back (new simple_name_attribute (pool, "Deprecated"));

      maybe_write_annotations ((*i).get (), attrs);

      field_attrs.push_back (attrs);
    }

  // Ensure that all the inner classes related to us appear in the
  // constant pool.
  std::list<ref_class> inners;
  the_class->add_classes_to_list (inners);
  if (the_class->member_p ())
    inners.push_back (the_class->get_lexically_enclosing_class ());
  for (std::list<ref_class>::const_iterator i = inners.begin ();
       i != inners.end ();
       ++i)
    pool->add ((*i).get ());

  // The constant pool is complete, so tidy it up.
  pool->finish ();

  // Emit class information.
  int mods = the_class->get_modifiers ();
  // Inner classes have modifiers like top-level classes, where the
  // only valid values are public and package-private.
  if ((mods & ACC_ACCESS) != 0 && (mods & ACC_ACCESS) != ACC_PUBLIC)
    mods &= ~ACC_ACCESS;
  if (! the_class->interface_p () && ! the_class->enum_p ())
    {
      // This should be set by all modern compilers.
      mods |= ACC_SUPER;
    }
  // Handle annotation and enums types.
  if (the_class->annotation_p ())
    mods |= ACC_ANNOTATION;
  if (the_class->enum_p ())
    mods |= ACC_ENUM;
  // We only use ACC_SYNTHETIC for 1.5 bytecode.
  if (! target_15)
    mods &= ~ACC_SYNTHETIC;
  writer.put2 (mods);

  writer.put2 (class_index);
  writer.put2 (super_index);
  writer.put2 (interface_indices.size ());
  for (std::list<int>::const_iterator i = interface_indices.begin ();
       i != interface_indices.end ();
       ++i)
    writer.put2 (*i);

  int fsize = fields.size ();
  if (fsize > 65535)
    throw the_class->error ("class contains more than 65535 fields");
  writer.put2 (fsize);
  {
    std::list<bytecode_attribute_list *>::const_iterator attr_i
      = field_attrs.begin ();
    for (std::list<ref_field>::const_iterator i = fields.begin ();
	 i != fields.end ();
	 ++i)
      {
	write_field ((*i).get (), *attr_i);
	delete *attr_i;
	++attr_i;
      }
    assert (attr_i == field_attrs.end ());
  }

  if (method_count > 65535)
    throw the_class->error ("class contains more than 65535 methods");
  writer.put2 (method_count);
  {
    std::list<bytecode_attribute_list *>::const_iterator attr_i
      = method_attrs.begin ();
    for (std::list<ref_method>::const_iterator i = methods.begin ();
	 i != methods.end ();
	 ++i)
      {
	model_method *m = (*i).get ();
	write_method (m, *attr_i);
	delete *attr_i;
	++attr_i;

	// When we made the 'method_attrs' list, we pushed a second
	// entry for any method requiring a bridge.
	if (target_15 && bridge_method::required_p (m))
	  {
	    write_method (m, *attr_i, true);
	    delete *attr_i;
	    ++attr_i;
	  }
      }
    assert (attr_i == method_attrs.end ());
  }

  // We can only know at the end whether we need an InnerClasses
  // attribute.
  if (pool->inner_classes_p ())
    class_attributes.push_back (new inner_classes_attribute (pool));
  class_attributes.emit (writer);

  // We write to a temporary file first, then rename once we are done.
  // That way if another compiler is running at the same time it won't
  // see a partially-written file.
  std::string temporary_name = outname + ".tmp";
  int fd = ::open (temporary_name.c_str (), O_WRONLY | O_CREAT | O_TRUNC,
		   0644);
  // FIXME: error checking of system calls from here to end of
  // function.
  assert (fd >= 0);
  int n = ::write (fd, writer.get_bytes (), writer.get_offset ());
  assert (n == writer.get_offset ());
  n = ::close (fd);
  assert (n != -1);

  // If a file named by the string pointed to by 'new' exists prior to
  // the call to the 'rename' function, the behavior is
  // implementation-defined.  ISO 9899-1990 7.9.4.2.  For example, on
  // Win32 with MSVCRT, it is an error.
  unlink (outname.c_str ());
  n = rename (temporary_name.c_str (), outname.c_str ());
  // FIXME
  assert (n != -1);
}
