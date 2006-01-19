// A constant pool being generated.

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
#include "bytecode/outpool.hh"
#include "bytecode/poolreader.hh"

outgoing_constant_pool::outgoing_constant_pool (model_class *k, bytecode_stream &b)
  : capacity (1),
    offset (b.get_offset ()),
    finished (false),
    the_class (k),
    bytes (b),
    fake_unit (NULL)
{
  // Reserve some space for the capacity.
  bytes.put2 (0);
}

outgoing_constant_pool::~outgoing_constant_pool ()
{
  if (fake_unit)
    delete fake_unit;
}

void
outgoing_constant_pool::finish ()
{
  bytes.put2_at (offset, capacity);
  finished = true;
}

template<typename T>
int
outgoing_constant_pool::check (std::map<T, int> &the_map, T the_value)
{
  typename std::map<T, int>::iterator iter = the_map.find (the_value);
  if (iter != the_map.end ())
    return (*iter).second;

  // It is ok to call 'add' after the pool has been written, but only
  // if you call knowing that the data is already there.
  assert (! finished);

  return -1;
}

template<typename T>
int
outgoing_constant_pool::check_qual (std::map<T, type_map_type> &the_map,
				    model_type *the_type, T the_value)
{
  typename std::map<T, type_map_type>::iterator iter
    = the_map.find (the_value);
  if (iter != the_map.end ())
    return check ((*iter).second, the_type);
  assert (! finished);
  return -1;
}

void
outgoing_constant_pool::increase_capacity (size_t amount)
{
  capacity += amount;
  if (capacity > 65535)
    throw the_class->error ("constant pool contains more than 65535 elements");
}

int
outgoing_constant_pool::add_utf (const std::string &str)
{
  int index = check (utf_map, str);
  if (index < 0)
    {
      size_t length = str.length ();
      if (length > 65535)
        throw the_class->error ("UTF-8 constant too long (byte count=%u1)")
          % length;
      bytes.put (CONSTANT_Utf8);
      bytes.put2 (length);
      bytes.put (length, str.c_str ());

      index = capacity;
      increase_capacity (1);
      utf_map[str] = index;
    }
  return index;
}

int
outgoing_constant_pool::add_name_and_type (const std::string &name,
					 const std::string &type)
{
  int nindex = add_utf (name);
  int tindex = add_utf (type);
  int code = (nindex << 16) | tindex;
  int index = check (name_type_map, code);
  if (index < 0)
    {
      bytes.put (CONSTANT_NameAndType);
      bytes.put2 (nindex);
      bytes.put2 (tindex);

      index = capacity;
      increase_capacity (1);
      name_type_map[code] = index;
    }
  return index;
}

int
outgoing_constant_pool::add (model_type *type)
{
  assert (type != null_type);
  int index = check (type_map, type);
  if (index < 0)
    {
      if (type->reference_p () && ! type->array_p ())
	{
	  model_class *k = assert_cast<model_class *> (type);
	  if (k->get_lexically_enclosing_class () != NULL)
	    {
	      nested_classes.insert (k);
	      // Ensure that various strings and other data end up in
	      // the constant pool before we write it.  We'll
	      // recompute this information later, so we needn't
	      // explicitly save it.
	      add_utf ("InnerClasses");
	      if (k->member_p ())
		add (k->get_lexically_enclosing_class ());
	      if (! k->anonymous_p ())
		add_utf (k->get_name ());
	    }
	}

      std::string external_name = get_descriptor (type);
      index = add_utf (external_name);
      bytes.put (CONSTANT_Class);
      bytes.put2 (index);

      index = capacity;
      increase_capacity (1);
      type_map[type] = index;
    }
  return index;
}

void
outgoing_constant_pool::add_if_inner (model_type *t)
{
  if (t->reference_p ())
    {
      model_class *k = assert_cast<model_class *> (t);
      if (k->get_lexically_enclosing_class () != NULL)
	add (k);
    }
}

int
outgoing_constant_pool::add (model_type *qual, model_method *meth)
{
  int index = check_qual (method_map, qual, meth);
  if (index < 0)
    {
      // Always add types that need an InnerClasses entry.
      std::list<ref_variable_decl> vars = meth->get_parameters ();
      for (std::list<ref_variable_decl>::const_iterator i = vars.begin ();
	   i != vars.end ();
	   ++i)
	add_if_inner ((*i)->type ());
      add_if_inner (meth->get_return_type ());

      int tindex = add_name_and_type (meth->get_name (),
				      meth->get_descriptor ());
      int kindex = add (qual);
      bytes.put (qual->interface_p () ? CONSTANT_InterfaceMethodref
		 : CONSTANT_Methodref);
      bytes.put2 (kindex);
      bytes.put2 (tindex);

      index = capacity;
      increase_capacity (1);
      type_map_type &submap = method_map[meth];
      submap[qual] = index;
    }
  return index;
}

int
outgoing_constant_pool::add (model_type *qual, model_field *fld)
{
  int index = check_qual (field_map, qual, fld);
  if (index < 0)
    {
      // Always add types that need an InnerClasses entry.
      model_type *fldtype = fld->type ();
      add_if_inner (fldtype);

      int tindex = add_name_and_type (fld->get_name (),
				      fldtype->get_descriptor ());
      int kindex = add (qual);
      bytes.put (CONSTANT_Fieldref);
      bytes.put2 (kindex);
      bytes.put2 (tindex);

      index = capacity;
      increase_capacity (1);
      type_map_type &submap = field_map[fld];
      submap[qual] = index;
    }
  return index;
}

int
outgoing_constant_pool::add (jint val)
{
  int index = check (int_map, val);
  if (index < 0)
    {
      bytes.put (CONSTANT_Integer);
      bytes.put4 (val);

      index = capacity;
      increase_capacity (1);
      int_map[val] = index;
    }
  return index;
}

int
outgoing_constant_pool::add (jlong val)
{
  int index = check (long_map, val);
  if (index < 0)
    {
      bytes.put (CONSTANT_Long);
      jint words[2];
      long_to_words (words, val);
      bytes.put4 (words[0]);
      bytes.put4 (words[1]);

      index = capacity;
      increase_capacity (2);
      long_map[val] = index;
    }
  return index;
}

int
outgoing_constant_pool::add (jfloat val)
{
  // Use Java's canonical NaN value.
  jint w;
  if (isnan (val))
    w = 0x7fc00000;
  else
    w = float_to_word (val);
  int index = check (float_map, w);
  if (index < 0)
    {
      bytes.put (CONSTANT_Float);
      bytes.put4 (w);

      index = capacity;
      increase_capacity (1);
      float_map[w] = index;
    }
  return index;
}

int
outgoing_constant_pool::add (jdouble val)
{
  jint words[2];

  // Use Java's canonical NaN value.
  if (isnan (val))
    {
      words[0] = 0x7ff80000;
      words[1] = 0;
    }
  else
    double_to_words (words, val);

  // Note that order of words here doesn't matter, since this
  // representation is for internal use only.
  jlong lval = words_to_long (words[0], words[1]);

  int index = check (double_map, lval);
  if (index < 0)
    {
      bytes.put (CONSTANT_Double);
      bytes.put4 (words[0]);
      bytes.put4 (words[1]);

      index = capacity;
      increase_capacity (2);
      double_map[lval] = index;
    }
  return index;
}

int
outgoing_constant_pool::add (const std::string &str)
{
  int index = check (string_map, str);
  if (index < 0)
    {
      index = add_utf (str);
      bytes.put (CONSTANT_String);
      bytes.put2 (index);

      index = capacity;
      increase_capacity (1);
      string_map[str] = index;
    }
  return index;
}

bool
outgoing_constant_pool::inner_classes_p ()
{
  return ! nested_classes.empty ();
}

void
outgoing_constant_pool::write_inner_classes ()
{
  // Here we freely use 'add', assuming that the entries have all been
  // added already.
  bytes.put2 (nested_classes.size ());

  for (std::set<model_class *>::const_iterator i = nested_classes.begin ();
       i != nested_classes.end ();
       ++i)
    {
      model_class *k = *i;

      bytes.put2 (add (k));
      bytes.put2 (k->member_p ()
		  ? add (k->get_lexically_enclosing_class ())
		  : 0);
      bytes.put2 (k->anonymous_p () ? 0 : add_utf (k->get_name ()));

      int mods = (k->get_modifiers ()
		  & (ACC_PUBLIC | ACC_PRIVATE | ACC_PROTECTED
		     | ACC_STATIC | ACC_FINAL | ACC_INTERFACE
		     | ACC_ABSTRACT));
      bytes.put2 (mods);
    }
}

int
outgoing_constant_pool::size ()
{
  return 2 + 8 * nested_classes.size ();
}

void
outgoing_constant_pool::update_descriptor (std::map<std::string, model_class *> &desc_map,
					 model_type *t)
{
  if (dynamic_cast<model_class *> (t) != NULL)
    {
      model_class *k = assert_cast<model_class *> (t);
      std::string name = get_descriptor (k);
      desc_map[name] = k;

      // Add both kinds of name, as the verifier might do
      // funny things.
      std::string name2 = k->get_descriptor ();
      if (name2 != name)
	desc_map[name2] = k;
    }
}

void
outgoing_constant_pool::update_descriptor (std::map<std::string, model_class *> &desc_map,
					 model_method *meth)
{

  std::list<ref_variable_decl> vars = meth->get_parameters ();
  for (std::list<ref_variable_decl>::const_iterator j = vars.begin ();
       j != vars.end ();
       ++j)
    update_descriptor (desc_map, (*j)->type ());
  update_descriptor (desc_map, meth->get_return_type ());
}

model_unit_class *
outgoing_constant_pool::get_fake_compilation_unit (const location &where,
						 model_class *declarer)
{
  assert (finished);

  if (fake_unit == NULL)
    {
      const uint8 *loc = (const uint8 *) bytes.get_bytes ();
      constant_pool_reader reader (where, &loc[offset],
				   bytes.get_offset () - offset);
      constant_pool *pool = reader.parse (&loc);

      std::map<std::string, model_class *> descriptor_map;

      // Add all known types to the map.
      for (type_map_type::const_iterator i = type_map.begin ();
	   i != type_map.end ();
	   ++i)
	{
	  model_type *t = (*i).first;
	  update_descriptor (descriptor_map, t);
	}

      // Now add all field types.
      for (std::map<model_field *, type_map_type>::const_iterator i
	     = field_map.begin ();
	   i != field_map.end ();
	   ++i)
	update_descriptor (descriptor_map, (*i).first->type ());

      // Now add all type from methods: return type and argument
      // types.
      for (std::map<model_method *, type_map_type>::const_iterator i
	     = method_map.begin ();
	   i != method_map.end ();
	   ++i)
	update_descriptor (descriptor_map, (*i).first);

      // And finally, types for all methods declared in this class.
      AllMethodsIterator end = declarer->end_all_methods ();
      for (AllMethodsIterator i = declarer->begin_all_methods ();
	   i != end;
	   ++i)
	update_descriptor (descriptor_map, (*i).get ());

      fake_unit = new model_unit_fake (where, pool, descriptor_map);
    }
  return fake_unit;
}

std::string
outgoing_constant_pool::get_descriptor (model_type *type)
{
  if (type->array_p () || type->primitive_p ()
      || type == primitive_void_type)
    return type->get_descriptor ();

  assert (type->reference_p ());
  model_class *k = assert_cast<model_class *> (type);
  return k->get_fully_qualified_name_with_slashes ();
}
