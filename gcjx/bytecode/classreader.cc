// Read a class and put it in the model.

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
#include "bytecode/classreader.hh"
#include "bytecode/cpool.hh"
#include "bytecode/signature.hh"
#include "bytecode/poolreader.hh"

#include <sstream>



struct attr_pair
{
  const char *name;
  class_known_attribute value;
};

static attr_pair known_attrs[] =
{
  { "SourceFile", ATTR_SOURCEFILE },
  { "ConstantValue", ATTR_CONSTANTVALUE },
  { "Code", ATTR_CODE },
  { "Exceptions", ATTR_EXCEPTIONS },
  { "LineNumberTable", ATTR_LINENUMBERTABLE },
  { "LocalVariableTable", ATTR_LOCALVARIABLETABLE },
  { "Deprecated", ATTR_DEPRECATED },
  { "Synthetic", ATTR_SYNTHETIC },
  { "EnclosingMethod", ATTR_ENCLOSINGMETHOD },
  { "Signature", ATTR_SIGNATURE },
  { "InnerClasses", ATTR_INNERCLASSES },
  { "LocalVariableTypeTable", ATTR_LOCALVARIABLETABLE },
  { "RuntimeVisibleAnnotations", ATTR_RUNTIMEVISIBLEANNOTATIONS },
  { "RuntimeInvisibleAnnotations", ATTR_RUNTIMEINVISIBLEANNOTATIONS },
  { "RuntimeVisibleParameterAnnotations",
    ATTR_RUNTIMEVISIBLEPARAMETERANNOTATIONS },
  { "RuntimeInvisibleParameterAnnotations",
    ATTR_RUNTIMEINVISIBLEPARAMETERANNOTATIONS },
  { "AnnotationDefault", ATTR_ANNOTATIONDEFAULT },
  { NULL, ATTR_UNKNOWN }
};



class_reader::class_reader (byte_buffer *b)
  : buffer (b),
    current (b->get ()),
    pool (NULL),
    is_member (false)
{
}

class_reader::~class_reader ()
{
  if (pool)
    delete pool;
}

class_file_error
class_reader::error (const char *fmt) const
{
  return class_file_error (where, fmt);
}

uint8
class_reader::read_u1 ()
{
  if (current - buffer->get () + 1 > (long) buffer->get_length ())
    throw error ("unexpected end of file");
  return *current++;
}

uint16
class_reader::read_u2 ()
{
  if (current - buffer->get () + 2 > (long) buffer->get_length ())
    throw error ("unexpected end of file");
  uint8 hi = *current++;
  uint8 lo = *current++;

  return (hi << 8) | lo;
}

uint32
class_reader::read_u4 ()
{
  if (current - buffer->get () + 4 > (long) buffer->get_length ())
    throw error ("unexpected end of file");
  uint8 one = *current++;
  uint8 two = *current++;
  uint8 three = *current++;
  uint8 four = *current++;

  return (one << 24) | (two << 16) | (three << 8) | four;
}



ref_expression
class_reader::parse_simple_annotation_value (uint8 match)
{
  ref_expression val;
  uint8 kind = parse_constant_value (val, true);
  if (kind != match)
    throw error ("annotation value had invalid type %1")
      % kind;
  return val;
}

ref_expression
class_reader::parse_annotation_value ()
{
  uint8 tag = read_u1 ();
  ref_expression val;
  switch (tag)
    {
    case 'B':
    case 'C':
    case 'I':
    case 'S':
    case 'Z':
      val = parse_simple_annotation_value (CONSTANT_Integer);
      break;
    case 'D':
      val = parse_simple_annotation_value (CONSTANT_Double);
      break;
    case 'F':
      val = parse_simple_annotation_value (CONSTANT_Float);
      break;
    case 'J':
      val = parse_simple_annotation_value (CONSTANT_Long);
      break;
    case 's':
      val = parse_simple_annotation_value (CONSTANT_Utf8);
      break;

    case 'e':
      {
	std::string class_name;
	std::string field_name;
	std::string descriptor;
	uint16 index = read_u2 ();
	pool->get_fieldref (index, class_name, field_name, descriptor);
	ref_forwarding_type base = new model_forwarding_full (where,
							      descriptor);
	// FIXME.
// 	val = new model_enum_field_ref (where, base, field_name);
      }
      break;

    case 'c':
      {
	uint16 index = read_u2 ();
	std::string cname = pool->get_class (index);
	ref_forwarding_type t = new model_forwarding_full (cname);
	val = new model_class_ref (where, t);
      }
      break;

    case '@':
      val = parse_annotation ();
      break;

    case '[':
      {
	uint16 count = read_u2 ();
	std::list<ref_expression> exprs;
	for (uint16 i = 0; i < count; ++i)
	  exprs.push_back (parse_annotation_value ());
	val = new model_annotation_initializer (where, exprs);
      }
      break;

    default:
      throw error ("unrecognized annotation tag %1")
	% tag;
    }

  return val;
}

ref_annotation
class_reader::parse_annotation ()
{
  uint16 index = read_u2 ();
  std::string anno_type_name = pool->get_utf8 (index);

  uint16 count = read_u2 ();
  std::list<ref_annotation_value> values;
  for (uint16 i = 0; i < count; ++i)
    {
      index = read_u2 ();
      std::string name = pool->get_utf8 (index);
      ref_expression expr = parse_annotation_value ();
      values.push_back (new model_annotation_value (where, name, expr));
    }

  return new model_annotation (where,
			       new model_forwarding_full (anno_type_name),
			       values);
}

void
class_reader::parse_annotations ()
{
  uint16 length = read_u2 ();
  for (uint16 i = 0; i < length; ++i)
    current_annotations.push_back (parse_annotation ());
}

void
class_reader::parse_parameter_annotations ()
{
  assert (current_method);

  std::list<ref_variable_decl> params = current_method->get_parameters ();
  uint8 count = read_u1 ();
  if (count != params.size ())
    throw error ("expected %1 parameter annotations, but found %2")
      % params.size () % count;

  for (std::list<ref_variable_decl>::const_iterator i = params.begin ();
       i != params.end ();
       ++i)
    {
      std::list<ref_annotation> annotations;
      uint16 length = read_u2 ();
      for (uint16 i = 0; i < length; ++i)
	annotations.push_back (parse_annotation ());
      // FIXME: do something with result
    }
}

void
class_reader::parse_annotation_default ()
{
  assert (current_annotation_member);

  ref_expression val = parse_annotation_value ();
  current_annotation_member->set_value (val);
}

uint8
class_reader::parse_constant_value (ref_expression &expr, bool use_utf)
{
  uint16 index = read_u2 ();

  uint8 kind = pool->tag (index);

  if (use_utf)
    {
      // For annotations, we need utf8, and not string.
      if (kind == CONSTANT_Utf8)
	kind = CONSTANT_String;
      else if (kind == CONSTANT_String)
	kind = CONSTANT_Utf8;
    }

  switch (kind)
    {
    case CONSTANT_Float:
      {
	jint val = jint (pool->value (index));
	expr = new model_float_literal (where, word_to_float (val));
	break;
      }

    case CONSTANT_Double:
      {
	jword bitshi = pool->value (index);
	jword bitslo = pool->value (index + 1);
	expr = new model_double_literal (where, words_to_double (bitshi,
								 bitslo));
	break;
      }

    case CONSTANT_Long:
      {
	jword bitshi = pool->value (index);
	jword bitslo = pool->value (index + 1);
	expr = new model_long_literal (where, words_to_long (bitshi, bitslo));
	break;
      }

    case CONSTANT_Integer:
      expr = new model_int_literal (where, jint (pool->value (index)));
      break;

    case CONSTANT_String:
      expr = new model_string_literal (where, pool->get_string (index));
      break;

    default:
      throw error ("invalid constant pool tag %1 for value of field %2")
	% kind % current_field.get ();
    }

  return kind;
}

void
class_reader::parse_constant_value ()
{
  assert (current_field);

  ref_expression init;
  parse_constant_value (init);
  current_field->set_initializer (init);
}

void
class_reader::parse_synthetic ()
{
  if (current_method)
    current_method->set_synthetic ();
  else if (current_field)
    current_field->set_synthetic ();
  else
    result->set_synthetic ();
}

void
class_reader::parse_deprecated ()
{
  if (current_method)
    current_method->set_deprecated (true);
  else if (current_field)
    current_field->set_deprecated (true);
  else
    result->set_deprecated (true);
}

void
class_reader::parse_exception_table ()
{
  uint16 tab_len = read_u2 ();

  model_bytecode_block::exception *excs
    = new model_bytecode_block::exception[tab_len];
  for (int i = 0; i < tab_len; ++i)
    {
      excs[i].handler = read_u2 ();
      excs[i].start = read_u2 ();
      excs[i].end = read_u2 ();
      excs[i].type = read_u2 ();
    }
  current_block->set_exceptions (tab_len, excs);
}

void
class_reader::parse_code ()
{
  assert (current_method);

  uint16 max_stack = read_u2 ();
  uint16 max_locals = read_u2 ();
  uint32 length = read_u4 ();

  if (1 /* FIXME: generating code for this class */)
    {
      current_block = new model_bytecode_block (where);
      current_block->set_max_stack (max_stack);
      current_block->set_max_locals (max_locals);
      current_block->set_bytecode (length, current);

      current_method->set_body (current_block);
    }

  // FIXME: length check needed here?
  current += length;

  parse_exception_table ();
  parse_attributes (ATTR_LINENUMBERTABLE | ATTR_LOCALVARIABLETABLE
		    | ATTR_LOCALVARIABLETYPETABLE);

  current_block = NULL;
}

void
class_reader::parse_inner_classes ()
{
  uint16 count = read_u2 ();

  for (int i = 0; i < count; ++i)
    {
      uint16 inner_info_index = read_u2 ();
      uint16 outer_info_index = read_u2 ();
      uint16 inner_name_index = read_u2 ();
      uint16 inner_access = read_u2 ();

      // JVM Spec says inner_info_index can be zero, but doesn't explain
      // what that means.
      if (inner_info_index == 0)
        throw error ("inner_info_index is 0");

      std::string inner = pool->get_class (inner_info_index);

      if (unsplit_name == inner)
	{
	  // This entry refers to our class.  In this case we need to look
	  // at the access bits and the plain name.
	  std::string plain_name = pool->get_utf8 (inner_name_index);
	  result->set_name (plain_name);
	  result->set_modifiers (inner_access);
	  if (inner_name_index == 0)
	    result->set_anonymous ();
	  else if (outer_info_index == 0)
	    result->set_local ();
	  else
	    is_member = true;
	}

      model_unit_class::inner_class k;
      if (outer_info_index != 0)
	k.outer = pool->get_class (outer_info_index);
      if (inner_name_index != 0)
	k.plain = pool->get_utf8 (inner_name_index);
      inner_infos[inner] = k;
    }
}

ref_forwarding_type
class_reader::one_type (const std::string &name, unsigned int &offset)
{
  unsigned int len = name.length ();
  int array_count = 0;

  while (offset < len && name[offset] == '[')
    {
      ++array_count;
      ++offset;
    }
  if (offset >= len)
    throw error ("invalid type descriptor %1") % name;

  ref_forwarding_type result;
  if (name[offset] == 'L')
    {
      unsigned int j;
      for (j = ++offset; j < len && name[j] != ';'; ++j)
	;
      if (j >= len)
	throw error ("invalid type descriptor %1") % name;
      result = new model_forwarding_full (name.substr (offset, j - offset));
      offset = j + 1;
    }
  else
    {
      // Primitive type, just look it up now.
      model_type *prim
	= classify_primitive_type_descriptor (name.substr (offset, 1));
      if (! prim)
	throw error ("invalid type descriptor %1") % name;
      assert (prim->primitive_p () || prim == primitive_void_type);
      result = new model_forwarding_resolved (where, prim);
      ++offset;
    }

  while (array_count-- > 0)
    result = result->array ();

  return result;
}

ref_forwarding_type
class_reader::parse_method_descriptor (const std::string &name,
				       std::list<ref_forwarding_type> &argtypes)
{
  unsigned int len = name.length ();

  unsigned int i = 0;
  if (len == 0 || name[i++] != '(')
    throw error ("invalid method descriptor %1") % name;

  while (i < len && name[i] != ')')
    {
      ref_forwarding_type elt = one_type (name, i);
      argtypes.push_back (elt);
    }
  // Skip the ')'.
  ++i;

  if (i >= len)
    throw error ("invalid method descriptor %1") % name;

  // Rest of the string is the return type.
  ref_forwarding_type result = one_type (name, i);
  if (i != name.length ())
    throw error ("invalid method descriptor %1") % name;

  return result;
}

// Handle the Exceptions attribute.
void
class_reader::parse_exceptions ()
{
  assert (current_method);
  // If we already saw some exceptions, it is because we parsed the
  // Signature attribute first.
  bool already_read = ! current_exceptions.empty ();
  uint16 count = read_u2 ();

  for (uint16 i = 0; i < count; ++i)
    {
      uint16 item = read_u2 ();
      std::string name = pool->get_class (item);
      if (! already_read)
	{
	  ref_forwarding_type etype (new model_forwarding_full (name));
	  current_exceptions.push_back (etype);
	}
    }
}

void
class_reader::parse_enclosing_method ()
{
  // FIXME: ensure local || anonymous class somehow.

  uint16 index = read_u2 ();
  uint16 method = read_u2 ();

  // FIXME: do something with these, at least check what they refer
  // to.
}

void
class_reader::parse_signature ()
{
  uint16 signature_index = read_u2 ();
  std::string signature = pool->get_utf8 (signature_index);

  signature_parser parser (signature, where);
  if (current_method)
    {
      std::list<ref_type_variable> type_parameters;
      ref_forwarding_type return_type;

      current_exceptions.clear ();
      parser.parse_method_signature (current_method_arguments,
				     type_parameters,
				     return_type,
				     current_exceptions);

      // A couple things we discover about the method are actually
      // handled in parse_method.  Others we can easily set here.
      current_method->set_type_parameters (type_parameters);
      current_method->set_return_type (return_type);
    }
  else if (current_field)
    current_field->set_declared_type (parser.parse_type ());
  else
    {
      // Signature for class.
      std::list<ref_type_variable> type_parameters;
      ref_forwarding_type super;
      std::list<ref_forwarding_type> ifaces;
      parser.parse_class_signature (type_parameters, super, ifaces);
      result->set_superclass (super);
      result->set_type_parameters (type_parameters);
      result->set_implements (ifaces);
    }
}

class_known_attribute
class_reader::find_attribute_type (const std::string &name)
{
  int i;
  for (i = 0; known_attrs[i].name != NULL; ++i)
    {
      if (name == known_attrs[i].name)
	break;
    }
  return known_attrs[i].value;
}

void
class_reader::parse_attributes (int mask)
{
  int count = read_u2 ();
  int seen = 0;

  for (int i = 0; i < count; ++i)
    {
      uint16 name_index = read_u2 ();
      uint32 len = read_u4 ();
      const uint8 *next_ptr = current + len;

      std::string name = pool->get_utf8 (name_index);
      class_known_attribute atype = find_attribute_type (name);
      // Just skip the special processing if it isn't found or if it
      // isn't applicable to what we're parsing at the moment.
      if (atype != 0 && (mask & atype) != 0)
	{
	  if (atype != ATTR_LOCALVARIABLETABLE
	      && atype != ATTR_LINENUMBERTABLE)
	    {
	      if ((seen & atype) != 0)
		throw error ("duplicate %1 attribute seen")
		  % name;
	    }
	  seen |= atype;

	  switch (atype)
	    {
	    case ATTR_LOCALVARIABLETABLE:
	    case ATTR_LINENUMBERTABLE:
	    case ATTR_SOURCEFILE:
	    case ATTR_LOCALVARIABLETYPETABLE:
	      // Ignore for now.
	      // FIXME
	      current = next_ptr;
	      break;

	    case ATTR_ANNOTATIONDEFAULT:
	      parse_annotation_default ();
	      break;

	    case ATTR_RUNTIMEVISIBLEPARAMETERANNOTATIONS:
	    case ATTR_RUNTIMEINVISIBLEPARAMETERANNOTATIONS:
	      // Note: can't be called for Code attribute.
	      assert (current_annotations.empty ());
	      parse_parameter_annotations ();
	      break;

	    case ATTR_RUNTIMEVISIBLEANNOTATIONS:
	    case ATTR_RUNTIMEINVISIBLEANNOTATIONS:
	      // Note: can't be called for Code attribute.
	      assert (current_annotations.empty ());
	      parse_annotations ();
	      break;

	    case ATTR_CONSTANTVALUE:
	      parse_constant_value ();
	      break;

	    case ATTR_CODE:
	      parse_code ();
	      break;

	    case ATTR_EXCEPTIONS:
	      parse_exceptions ();
	      break;

	    case ATTR_DEPRECATED:
	      parse_deprecated ();
	      break;

	    case ATTR_SYNTHETIC:
	      parse_synthetic ();
	      break;

	    case ATTR_ENCLOSINGMETHOD:
	      parse_enclosing_method ();
	      break;

	    case ATTR_SIGNATURE:
	      parse_signature ();
	      break;

	    case ATTR_INNERCLASSES:
	      parse_inner_classes ();
	      break;

	    default:
	      abort ();
	    }

	  // Each parse method must read all its information.
	  if (current != next_ptr)
	    throw error ("attribute %1 ends early")
	      % name;
	}

      current = next_ptr;
    }

  // Now set annotations.
  if (! current_annotations.empty ())
    {
      if (current_method)
	current_method->set_annotations (current_annotations);
      else if (current_field)
	current_field->set_annotations (current_annotations);
      else
	result->set_annotations (current_annotations);
      current_annotations.clear ();
    }
}

void
class_reader::parse_method ()
{
  uint16 flags = read_u2 ();
  uint16 name_index = read_u2 ();
  uint16 descriptor_index = read_u2 ();

  std::string name = pool->get_utf8 (name_index);
  std::string sig = pool->get_utf8 (descriptor_index);

  current_method_arguments.clear ();
  ref_forwarding_type return_type
    = parse_method_descriptor (sig, current_method_arguments);

  if (result->annotation_p ())
    {
      current_annotation_member
	= new model_annotation_member (where, name, NULL, return_type);
      current_annotation_member->set_from_class ();
      current_annotation_member->set_modifiers (flags);
    }
  else
    {
      current_method = (name == "<init>"
			? new model_constructor (where, result.get ())
			: new model_method (where, result.get ()));
      current_method->set_name (name);
      current_method->set_from_class ();
      current_method->set_return_type (return_type);
      current_method->set_modifiers (flags);
      if ((flags & ACC_VARARGS) != 0)
	current_method->set_varargs ();
    }

  current_exceptions.clear ();
  parse_attributes (ATTR_CODE | ATTR_EXCEPTIONS | ATTR_DEPRECATED
		    | ATTR_SYNTHETIC | ATTR_SIGNATURE
		    | ATTR_RUNTIMEVISIBLEANNOTATIONS
		    | ATTR_RUNTIMEINVISIBLEANNOTATIONS
		    | ATTR_RUNTIMEVISIBLEPARAMETERANNOTATIONS
		    | ATTR_RUNTIMEINVISIBLEPARAMETERANNOTATIONS
		    | ATTR_ANNOTATIONDEFAULT);
  if (! current_exceptions.empty ())
    current_method->set_throws (current_exceptions);

  if (result->annotation_p ())
    {
      model_annotation_type *anno
	= assert_cast<model_annotation_type *> (result.get ());
      anno->add_annotation_member (current_annotation_member);
      current_annotation_member = NULL;
    }
  else
    {
      // Create dummy parameters suitable for use as method arguments.
      std::list<ref_variable_decl> parameters;
      int count = 0;
      for (std::list<ref_forwarding_type>::const_iterator i
	     = current_method_arguments.begin ();
	   i != current_method_arguments.end ();
	   ++i)
	{
	  // FIXME: ought to look up actual name if it appears in class file.
	  std::ostringstream oss;
	  oss << "arg" << count;
	  ++count;

	  ref_variable_decl arg = new model_variable_decl (where, oss.str (),
							   *i, result.get ());
	  parameters.push_back (arg);
	}
      current_method->set_parameters (parameters);

      result->add (current_method);
      current_method = NULL;
    }
}

void
class_reader::parse_field ()
{
  uint16 flags = read_u2 ();
  uint16 name_index = read_u2 ();
  uint16 descriptor_index = read_u2 ();

  std::string name = pool->get_utf8 (name_index);
  std::string sig = pool->get_utf8 (descriptor_index);
  unsigned int off = 0;
  ref_forwarding_type vtype = one_type (sig, off);
  if (off != sig.length ())
    throw error ("malformed descriptor while parsing field %1")
      % name;

  current_field = new model_field (where, name, vtype, result.get ());
  current_field->set_from_class ();
  current_field->set_modifiers (flags);

  parse_attributes (ATTR_CONSTANTVALUE | ATTR_DEPRECATED | ATTR_SYNTHETIC
		    | ATTR_SIGNATURE | ATTR_RUNTIMEVISIBLEANNOTATIONS
		    | ATTR_RUNTIMEINVISIBLEANNOTATIONS);

  result->add (current_field);
  current_field = NULL;
}

model_class *
class_reader::find_outer_class (const std::string &name)
{
  std::map<std::string, model_unit_class::inner_class>::const_iterator i
    = inner_infos.find (name);

  // If we have a top-level class, look it up using the ordinary
  // approach.
  if (i == inner_infos.end ())
    {
      std::list<std::string> parts = split (name, '/');
      return global->get_compiler ()->find_class (parts);
    }

  model_class *outer = find_outer_class ((*i).second.outer);
  Iname *mem = outer->find_name ((*i).second.plain);
  return dynamic_cast<model_class *> (mem);
}

bool
class_reader::parse_header ()
{
  uint32 magic = read_u4 ();
  if (magic != MAGIC)
    throw error ("invalid magic number %1") % to_hex_string (magic);
  uint16 minor = read_u2 ();
  uint16 major = read_u2 ();
  if (major < MAJOR_1_1 || major > MAJOR_1_5
      || (major == MAJOR_1_5 && minor > MINOR_1_5))
    {
      std::ostringstream version;
      version << major << '.' << minor;
      throw error ("invalid class file version %1") % version.str ();
    }

  return major == MAJOR_1_5;
}

void
class_reader::parse_constant_pool ()
{
  constant_pool_reader reader (where, current,
			       (buffer->get_length () - (current
							 - buffer->get ()
							 + 1)));
  const uint8 *result;
  pool = reader.parse (&result);
  current = result;
}

void
class_reader::create_class ()
{
  if ((access_flags & ACC_ANNOTATION) != 0)
    result = new model_annotation_type (where);
  else if ((access_flags & ACC_ENUM) != 0)
    result = new model_enum (where);
  else
    result = new model_class (where);
  result->set_from_class ();
}

void
class_reader::parse_self ()
{
  access_flags = read_u2 ();

  create_class ();

  uint16 this_class = read_u2 ();
  uint16 super_class = read_u2 ();

  unsplit_name = pool->get_class (this_class);
  self_name = split (unsplit_name, '/');

  // fixme ensure that 'this_class' names the class we expect it to.

  uint16 must_set = 0;
  uint16 must_not_set = 0;
  if ((access_flags & ACC_INTERFACE) != 0)
    {
      result->set_interface ();

      // We check that the superclass name is set correct, but
      // internally we represent interfaces as not having a
      // superclass.
      if (super_class == 0
	  || pool->get_class (super_class) != "java/lang/Object")
	throw error ("interface %1 does not have java.lang.Object "
		     "as superclass")
	  % join (self_name, '.');
      must_set |= ACC_ABSTRACT;
      // The spec is unclear as to whether a synthetic interface is
      // allowed.  It contradicts itself on the topic of ACC_INTERFACE
      // versus ACC_ANNOTATION.  ACC_SUPER is also unclear, but we
      // assume it is needed.
      must_not_set |= ACC_FINAL | ACC_SYNTHETIC | ACC_ENUM;
    }
  else
    {
      if (super_class == 0)
	{
	  if (unsplit_name != "java/lang/Object")
	    throw error ("class %1 does not have superclass")
	      % self_name;
	}
      else
	{
	  std::string name = pool->get_class (super_class);
	  ref_forwarding_type super (new model_forwarding_full (name));
	  result->set_superclass (super);
	}

      must_set |= ACC_SUPER;
      must_not_set |= ACC_ANNOTATION;

      if ((access_flags & ACC_ABSTRACT) != 0
	  && (access_flags & ACC_FINAL) != 0)
	throw error ("abstract class also marked %<final%>");
    }

  if ((access_flags & must_set) != must_set
      || (access_flags & must_not_set) != 0)
    throw error ("invalid access flags for this class");

  // We could emit a warning here about unrecognized nonzero bits.

  result->set_modifiers (access_flags);

  // Now read interfaces.
  int count = read_u2 ();
  for (int i = 0; i < count; ++i)
    {
      uint16 index = read_u2 ();
      std::string name = pool->get_class (index);
      ref_forwarding_type iface (new model_forwarding_full (name));
      interfaces.push_back (iface);
    }
}

void
class_reader::parse_fields ()
{
  int field_count = read_u2 ();
  for (int i = 0; i < field_count; ++i)
    parse_field ();
}

void
class_reader::parse_methods ()
{
  int method_count = read_u2 ();
  for (int i = 0; i < method_count; ++i)
    parse_method ();
}

void
class_reader::parse_class_attributes ()
{
  parse_attributes (ATTR_SOURCEFILE | ATTR_DEPRECATED | ATTR_SYNTHETIC
		    | ATTR_ENCLOSINGMETHOD | ATTR_INNERCLASSES
		    | ATTR_SIGNATURE | ATTR_RUNTIMEVISIBLEANNOTATIONS
		    | ATTR_RUNTIMEINVISIBLEANNOTATIONS);
}

ref_unit
class_reader::parse (const std::string &filename)
{
  where = location (filename.c_str ());

  bool use_15 = parse_header ();
  parse_constant_pool ();
  parse_self ();
  parse_fields ();
  parse_methods ();
  parse_class_attributes ();

  if (! is_member)
    {
      result->set_name (get_simple_name (self_name));
      result->set_modifiers (access_flags);
    }

  result->set_implements (interfaces);

  ref_unit unit (new model_unit_class (where, inner_infos, pool));
  // Transfer ownership of constant pool.
  pool = NULL;
  unit->set_file_name (filename);
  if (use_15)
    {
      model_unit_class *uc = assert_cast<model_unit_class *> (unit.get ());
      uc->set_15 ();
    }

  if (is_member)
    {
      std::map<std::string, model_unit_class::inner_class>::const_iterator i
	= inner_infos.find (unsplit_name);
      assert (i != inner_infos.end ());
      model_class *oclass = find_outer_class ((*i).second.outer);
      assert (oclass);
      result->set_declaring_class (oclass);
      oclass->add (result);
    }

  std::list<std::string> pkg_name = drop_last_element (self_name);
  // FIXME set our enclosing class appropriately!
  if (pkg_name.empty ())
    unit->set_package (global->get_compiler ()->get_unnamed_package ());
  else
    // FIXME: unit.get() is bogus here
    unit->set_package (classify_package_name (unit.get (), pkg_name));
  // FIXME why should both of these be required?
  unit->add (result);
  result->set_compilation_unit (unit.get ());
  return unit;
}
