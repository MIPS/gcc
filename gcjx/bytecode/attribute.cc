// Attribute for bytecode output.

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
#include "bytecode/outpool.hh"
#include "bytecode/locals.hh"
#include "bytecode/block.hh"
#include "bytecode/attribute.hh"
#include "bytecode/generate.hh"

static int emit_annotation_value (bytecode_stream *writer,
				  outgoing_constant_pool *pool,
				  model_expression *expr);
static int emit_annotation (bytecode_stream *writer,
			    outgoing_constant_pool *pool,
			    model_annotation *anno);

bytecode_attribute::bytecode_attribute (outgoing_constant_pool *p,
					const std::string &n)
  : pool (p),
    name (n)
{
  // Set up for later emission.
  pool->add_utf (name);
}

bytecode_attribute::~bytecode_attribute ()
{
}

void
bytecode_attribute::emit (bytecode_stream &writer)
{
  writer.put2 (pool->add_utf (name));
  writer.put4 (size ());
}



void
simple_name_attribute::emit (bytecode_stream &writer)
{
  bytecode_attribute::emit (writer);
}



utf8_attribute::utf8_attribute (outgoing_constant_pool *p,
				const std::string &n,
				const std::string &v)
  : bytecode_attribute (p, n),
    value (v)
{
  // Set up for later emission.
  pool->add_utf (value);
}

void
utf8_attribute::emit (bytecode_stream &writer)
{
  bytecode_attribute::emit (writer);
  writer.put2 (pool->add_utf (value));
}



inner_classes_attribute::inner_classes_attribute (outgoing_constant_pool *p)
  : bytecode_attribute (p, "InnerClasses")
{
  assert (pool->inner_classes_p ());
}

void
inner_classes_attribute::emit (bytecode_stream &writer)
{
  bytecode_attribute::emit (writer);
  pool->write_inner_classes ();
}

int
inner_classes_attribute::size ()
{
  return pool->size ();
}



exceptions_attribute::exceptions_attribute (outgoing_constant_pool *p,
					    const std::list<ref_forwarding_type> &e)
  : bytecode_attribute (p, "Exceptions"),
    excs (e)
{
  assert (! excs.empty ());
  for (std::list<ref_forwarding_type>::const_iterator i = excs.begin ();
       i != excs.end ();
       ++i)
    pool->add (*i);
}

void
exceptions_attribute::emit (bytecode_stream &writer)
{
  bytecode_attribute::emit (writer);
  writer.put2 (excs.size ());
  for (std::list<ref_forwarding_type>::const_iterator i = excs.begin ();
       i != excs.end ();
       ++i)
    writer.put2 (pool->add (*i));
}



code_attribute::code_attribute (outgoing_constant_pool *p, bytecode_generator *g)
  : bytecode_attribute (p, "Code"),
    gen (g)
{
}

void
code_attribute::emit (bytecode_stream &writer)
{
  bytecode_attribute::emit (writer);
  gen->write (&writer);
}

int
code_attribute::size ()
{
  return gen->bytecode_size ();
}



field_value_attribute::field_value_attribute (outgoing_constant_pool *p,
					      model_field *field)
  : bytecode_attribute (p, "ConstantValue"),
    index (-1)
{
  model_type *type = field->type ();
  if (type == primitive_double_type)
    index = pool->add (jdouble (field->value ()));
  else if (type == primitive_float_type)
    index = pool->add (jfloat (field->value ()));
  else if (type == primitive_long_type)
    index = pool->add (jlong (field->value ()));
  else if (type == primitive_int_type || type == primitive_short_type
	   || type == primitive_char_type || type == primitive_byte_type
	   || type == primitive_boolean_type)
    {
      // FIXME: the cast.
      model_primitive_base *itype
	= assert_cast<model_primitive_base *> (primitive_int_type);
      index = pool->add (jint (itype->convert (type, field->value ())));
    }
  else
    {
      assert (type == global->get_compiler ()->java_lang_String ());
      index = pool->add (field->string_value ());
    }
  assert (index != -1);
}

void
field_value_attribute::emit (bytecode_stream &writer)
{
  bytecode_attribute::emit (writer);
  writer.put2 (index);
}



template<typename T>
static void
emit_primitive (bytecode_stream *writer, outgoing_constant_pool *pool,
		model_type *type, T val)
{
  int index = pool->add (val);
  if (writer)
    {
      writer->put (type->get_descriptor ()[0]);
      writer->put2 (index);
    }
}

static int
emit_annotation_value (bytecode_stream *writer, outgoing_constant_pool *pool,
		       model_expression *expr)
{
  int result = 0;

  model_type *type = expr->type ();
  if (type == primitive_boolean_type)
    {
      jboolean val = jboolean (expr->value ());
      emit_primitive (writer, pool, type, jint (val));
      result += 3;
    }
  else if (type == primitive_boolean_type || type == primitive_char_type
	   || type == primitive_int_type || type == primitive_short_type)
    {
      // FIXME: ugly cast.
      model_primitive_base *it
	= assert_cast<model_primitive_base *> (primitive_int_type);
      jint val = jint (it->convert (type, expr->value ()));
      emit_primitive (writer, pool, type, val);
      result += 3;
    }
  else if (type == primitive_long_type)
    {
      jlong val = expr->value ();
      emit_primitive (writer, pool, type, val);
      result += 3;
    }
  else if (type == primitive_float_type)
    {
      jfloat val = expr->value ();
      emit_primitive (writer, pool, type, val);
      result += 3;
    }
  else if (type == primitive_double_type)
    {
      jdouble val = expr->value ();
      emit_primitive (writer, pool, type, val);
      result += 3;
    }
  else if (type == global->get_compiler ()->java_lang_String ())
    {
      std::string val = expr->string_value ();
      int index = pool->add_utf (val);
      if (writer)
	{
	  writer->put ('s');
	  writer->put2 (index);
	}
      result += 3;
    }
  else if (type == global->get_compiler ()->java_lang_Class ())
    {
      model_class_ref *ref = assert_cast<model_class_ref *> (expr);
      int index = pool->add_utf (ref->get_class ()->get_descriptor ());
      if (writer)
	{
	  writer->put ('c');
	  writer->put2 (index);
	}
      result += 3;
    }
  else if (type->array_p ())
    {
      model_annotation_initializer *init
	= assert_cast<model_annotation_initializer *> (expr);
      std::list<ref_expression> array = init->get_initializers ();

      if (writer)
	{
	  writer->put ('[');
	  writer->put2 (array.size ());
	}
      result += 3;

      for (std::list<ref_expression>::const_iterator i = array.begin ();
	   i != array.end ();
	   ++i)
	result += emit_annotation_value (writer, pool, (*i).get ());
    }
  else if (type->annotation_p ())
    {
      model_annotation *annot = assert_cast<model_annotation *> (expr);
      if (writer)
	writer->put ('@');
      ++result;

      result += emit_annotation (writer, pool, annot);
    }
  else
    {
      model_memberref_forward *b
	= assert_cast<model_memberref_forward *> (expr);
      expr = b->get_real ();
      model_field_ref *fr = assert_cast<model_field_ref *> (expr);
      model_field *fld = fr->get_field ();

      // FIXME: what about inner classes here?
      int type_index
	= pool->add_utf (fld->get_declaring_class ()->get_descriptor ());
      int name_index = pool->add_utf (fld->get_name ());

      if (writer)
	{
	  writer->put ('e');
	  writer->put2 (type_index);
	  writer->put2 (name_index);
	}

      result += 5;
    }

  return result;
}

// Write an annotation and return the size.  If WRITER is NULL, don't
// write, just compute the size and update the constant pool.
static int
emit_annotation (bytecode_stream *writer, outgoing_constant_pool *pool,
		 model_annotation *anno)
{
  int result = 0;

  int val = pool->add_utf (anno->type ()->get_descriptor ());
  if (writer)
    writer->put2 (val);
  result += 2;

  std::list<ref_annotation_value> vals = anno->get_arguments ();
  if (writer)
    writer->put2 (vals.size ());
  result += 2;

  for (std::list<ref_annotation_value>::const_iterator i = vals.begin ();
       i != vals.end ();
       ++i)
    {
      val = pool->add_utf ((*i)->get_name ());
      if (writer)
	writer->put2 (val);
      result += 2;

      result += emit_annotation_value (writer, pool, (*i)->get_value ());
    }

  return result;
}

annotation_attribute::annotation_attribute (outgoing_constant_pool *p,
					    const std::string &n,
					    const std::list<model_annotation *> &as)
  : bytecode_attribute (p, n),
    len (0),
    annos (as)
{
  for (std::list<model_annotation *>::const_iterator i = annos.begin ();
       i != annos.end ();
       ++i)
    len += emit_annotation (NULL, pool, *i);
  len += 2;
}

void
annotation_attribute::emit (bytecode_stream &writer)
{
  bytecode_attribute::emit (writer);
  writer.put2 (annos.size ());
  for (std::list<model_annotation *>::const_iterator i = annos.begin ();
       i != annos.end ();
       ++i)
    emit_annotation (&writer, pool, *i);
}

parameter_attribute::parameter_attribute (outgoing_constant_pool *p,
					  const std::string &n,
					  const std::list< std::list<model_annotation *> > &as)
  : bytecode_attribute (p, n),
    len (0),
    annos (as)
{
  for (std::list< std::list<model_annotation *> >::const_iterator j
	 = annos.begin ();
       j != annos.end ();
       ++j)
    {
      const std::list<model_annotation *> &inner (*j);
      for (std::list<model_annotation *>::const_iterator i = inner.begin ();
	   i != inner.end ();
	   ++i)
	len += emit_annotation (NULL, pool, *i);
      len += 2;
    }
  len += 2;
}

void
parameter_attribute::emit (bytecode_stream &writer)
{
  bytecode_attribute::emit (writer);
  writer.put2 (annos.size ());
  for (std::list< std::list<model_annotation *> >::const_iterator j
	 = annos.begin ();
       j != annos.end ();
       ++j)
    {
      const std::list<model_annotation *> &inner (*j);
      writer.put2 (inner.size ());
      for (std::list<model_annotation *>::const_iterator i = inner.begin ();
	   i != inner.end ();
	   ++i)
	emit_annotation (&writer, pool, *i);
    }
}

annotation_default_attribute::annotation_default_attribute (outgoing_constant_pool *p,
							    model_expression *e)
  : bytecode_attribute (p, "AnnotationDefault"),
    len (0),
    expr (e)
{
  len = emit_annotation_value (NULL, pool, expr);
}

void
annotation_default_attribute::emit (bytecode_stream &writer)
{
  bytecode_attribute::emit (writer);
  emit_annotation_value (&writer, pool, expr);
}



bytecode_attribute_list::bytecode_attribute_list ()
{
}

bytecode_attribute_list::~bytecode_attribute_list ()
{
  for (std::list<bytecode_attribute *>::const_iterator i = attrs.begin ();
       i != attrs.end ();
       ++i)
    delete *i;
}

void
bytecode_attribute_list::emit (bytecode_stream &writer)
{
  writer.put2 (attrs.size ());
  for (std::list<bytecode_attribute *>::const_iterator i = attrs.begin ();
       i != attrs.end ();
       ++i)
    (*i)->emit (writer);
}

int
bytecode_attribute_list::size ()
{
  int result = 2;
  for (std::list<bytecode_attribute *>::const_iterator i = attrs.begin ();
       i != attrs.end ();
       ++i)
    {
      // '6' is 2 byte for the name and 4 for the length.
      result += 6 + (*i)->size ();
    }
  return result;
}
