// Per-class wrapper for AOT compilers.

// Copyright (C) 2005 Free Software Foundation, Inc.
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

#include "typedefs.hh"
#include "aot/aotclass.hh"
#include "aot/aotfactory.hh"

aot_class::aot_class (aot_class_factory *cf, model_class *k)
  : factory (cf),
    klass (k),
    vtable_done (false)
{
}

void
aot_class::lay_out_vtable ()
{
  if (vtable_done)
    return;
  vtable_done = true;

  model_class *sk = klass->get_superclass ();
  aot_class *super = sk ? factory->get_class (sk) : NULL;

  if (super)
    vtable = super->vtable;

  model_class *objtype = global->get_compiler ()->java_lang_Object ();

  std::list<model_method *> methods = klass->get_sorted_methods ();
  for (std::list<model_method *>::const_iterator i = methods.begin ();
       i != methods.end ();
       ++i)
    {
      // Static methods, private methods, and constructors do not
      // appear in the vtable.  Note that final methods must appear
      // there, because they can be called using the vtable via
      // reflection.
      if ((*i)->static_p ()
	  || (*i)->constructor_p ()
	  || ((*i)->get_modifiers () & ACC_PRIVATE) != 0)
	continue;

      // Final methods in Object are not entered in the vtable as they
      // are never called virtually.  FIXME: conceivably, this could
      // change in Java, and this threatens binary compatibility.
      if ((*i)->final_p () && (*i)->get_declaring_class () == objtype)
	continue;

      model_class *declaring_class = (*i)->get_declaring_class ();
      if (declaring_class->interface_p ())
	{
	  // If this interface is implemented by some superclass, then
	  // it will already appear in the vtable and we need do
	  // nothing.
	  if (super && declaring_class->assignable_from_p (super->klass))
	    continue;
	}
      else if (declaring_class != klass)
	{
	  // The method was declared in some concrete superclass, so
	  // is already processed.
	  continue;
	}
      else if (super)
	{
	  // See if this method overrides some method in the
	  // superclass.
	  unsigned int index = 0;
	  std::string name = (*i)->get_name ();
	  for (std::vector<model_method *>::const_iterator j
		 = super->vtable.begin ();
	       j != super->vtable.end ();
	       ++j, ++index)
	    {
	      // Note that hides_or_overrides_p does not check the
	      // method name.  FIXME?
	      if (name == (*j)->get_name ()
		  && (*i)->hides_or_overrides_p (*j, klass))
		{
		  // Re-use the previous slot.
		  vtable[index] = *i;
		  break;
		}
	    }
	  if (index != super->vtable.size ())
	    continue;
	}

      vtable.push_back (*i);
    }

#ifdef DEBUG_VTABLE
  std::cerr << "=== " << klass->get_fully_qualified_name () << std::endl;
  int n = 0;
  for (std::vector<model_method *>::const_iterator i = vtable.begin ();
       i != vtable.end ();
       ++i, ++n)
    std::cerr << n << ": " << (*i)->get_pretty_name () << std::endl;
#endif // DEBUG_VTABLE
}

int
aot_class::add_item (const pool_entry &entry)
{
  int index = 0;
  for (std::vector<pool_entry>::const_iterator i = pool.begin ();
       i != pool.end ();
       ++i, ++index)
    {
      const pool_entry &it (*i);
      if (entry.tag == it.tag && entry.value == it.value)
	// We don't use index 0.
	return index + 1;
    }

  pool.push_back (entry);
  // Note that we return the new size, not 'pool.size() - 1', because
  // for historical reasons the 0th entry of the constant pool is not
  // used.
  return pool.size ();
}

int
aot_class::add (const std::string &s)
{
  pool_entry e;
  e.tag = CONSTANT_String;
  e.value = s;
  return add_item (e);
}

int
aot_class::add_class (const std::string &s)
{
  pool_entry e;
  e.tag = CONSTANT_Class;
  e.value = s;
  return add_item (e);
}

// FIXME: this should either be a method on model_type or a generally
// available helper function -- it is also used in gcc/java.
static std::string
get_descriptor (model_type *t)
{
  // Why do we do this?
  return join (split (t->get_descriptor (), '/'), '.');
}

int
aot_class::add (model_class *k)
{
  pool_entry e;
  e.tag = CONSTANT_Class;
  e.value = get_descriptor (k);
  return add_item (e);
}

int
aot_class::add_utf (const std::string &s)
{
  pool_entry e;
  e.tag = CONSTANT_Utf8;
  e.value = s;
  return add_item (e);
}

void
aot_class::add_type_assertion (model_class *base,
			       model_class *derived)
{
  // There's no need to do any assertion checking on array types.
  // They are completely understood at compile time.
  if (base->array_p () || derived->array_p ())
    return;

  type_assertion_entry e;
  e.key = JV_ASSERT_TYPES_COMPATIBLE;
  e.base = base;
  e.derived = derived;
  // FIXME: make sure there isn't a duplicate assertion.
  type_assertions.push_back (e);
}

void
aot_class::note_new (model_class *k)
{
  if (new_set.find (k) == new_set.end ())
    {
      type_assertion_entry e;
      e.key = JV_ASSERT_IS_INSTANTIABLE;
      e.base = k;
      e.derived = NULL;
      type_assertions.push_back (e);

      new_set.insert (k);
    }
}

int
aot_class::find_in_vtable (model_method *method)
{
  lay_out_vtable ();

  // An abstract method can itself be derived from an abstract method.
  // We strip them all off to get the method as actually declared.
  while (dynamic_cast<model_abstract_method *> (method))
    {
      model_abstract_method *mam
	= assert_cast<model_abstract_method *> (method);
      method = mam->get_original ();
    }

  int index = 0;
  for (std::vector<model_method *>::const_iterator i = vtable.begin ();
       i != vtable.end ();
       ++i, ++index)
    {
      if (*i == method)
	// Note: we have two extra vtable slots; one for the class
	// pointer and one for the GC descriptor.  We tell our caller
	// about the real offset, including these two.
	return index + 2;
    }
  abort ();
}

bool
aot_class::finalizeable_p ()
{
  for (model_class *t = klass; t; t = t->get_superclass ())
    {
      if (t->has_method_with_descriptor_p ("finalize", "()V"))
	return true;
    }
  return false;
}

int
aot_class::register_something (std::vector<model_element *> &the_map,
			       model_element *item)
{
  // Note that "entry 0" is a flag indicating whether the state is
  // initialized.
  int index = 1;
  for (std::vector<model_element *>::const_iterator i = the_map.begin ();
       i != the_map.end ();
       ++i, ++index)
    {
      if (*i == item)
	return index;
    }
  the_map.push_back (item);
  return index;
}

int
aot_class::register_indirect_call (model_method *m)
{
  assert (! m->get_declaring_class ()->interface_p ());
  if (m->static_p () || m->constructor_p ()
      || (m->get_modifiers () & ACC_PRIVATE) != 0
      || (m->final_p ()
	  && m->get_declaring_class () == global->get_compiler ()->java_lang_Object ()))
    return register_something (atable_map, m);
  return register_something (otable_map, m);
}

int
aot_class::register_interface_call (model_method *m)
{
  assert (m->get_declaring_class ()->interface_p ());
  return register_something (itable_map, m);
}

int
aot_class::register_field_reference (model_field *f)
{
  return register_something (f->static_p () ? atable_map : otable_map, f);
}
