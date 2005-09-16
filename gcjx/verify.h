/* Declarations to interface gcj with bytecode verifier.
   Copyright (C) 2003, 2004, 2005 Free Software Foundation, Inc.

This file is part of GCC.

gcjx is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

gcjx is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with gcjx; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

Java and all Java-based marks are trademarks or registered trademarks
of Sun Microsystems, Inc. in the United States and other countries.
The Free Software Foundation is independent of Sun Microsystems, Inc.  */

// This is glue between gcjx and the verifier.

#ifndef GCC_VERIFY_H
#define GCC_VERIFY_H

#include "typedefs.hh"
#include "bytecode/insns.hh"
#include "bytecode/constants.hh"

typedef constant_pool vfy_constants;

typedef char *vfy_string;

typedef model_type *vfy_jclass;

typedef uint16 vfy_uint_16;

typedef model_bytecode_block::exception vfy_exception;

// These must be kept in sync with verify.cc.
#define VERIFY_INSN_START 1
#define VERIFY_BRANCH_TARGET 2
#define VERIFY_SEEN 4


struct vfy_method
{
  model_method *method;
  model_bytecode_block *block;
  resolution_scope *scope;
  model_unit_class *unit;

  // These fields are referred to directly by the verifier.
  vfy_jclass defining_class;
  int max_stack;
  int max_locals;
  int code_length;
  int exc_count;

  vfy_method (model_method *m, model_bytecode_block *b,
	      resolution_scope *s, model_unit_class *u)
    : method (m),
      block (b),
       scope (s),
       unit (u)
  {
    defining_class = method->get_declaring_class ();
    max_stack = block->get_max_stack ();
    max_locals = block->get_max_locals ();
    code_length = block->get_code_length ();
    exc_count = block->get_exception_length ();
  }
};

inline vfy_constants *vfy_get_constants (vfy_method *method)
{
  return method->unit->get_constant_pool ();
}

inline void *vfy_alloc (size_t bytes)
{
  return new char[bytes];
}

inline void vfy_free (void *mem)
{
  // This is always ok in this context.
  delete [] (char *) mem;
}

inline bool vfy_strings_equal (vfy_string one, vfy_string two)
{
  return ! strcmp (one, two);
}

inline const char *vfy_string_bytes (vfy_string str)
{
  return str;
}

inline vfy_string vfy_init_name (void)
{
  return "<init>";
}

inline vfy_string vfy_clinit_name (void)
{
  return "<clinit>";
}

int vfy_count_arguments (vfy_string signature);

inline vfy_string vfy_get_signature (vfy_method *method)
{
  return const_cast<char *> (method->method->get_descriptor ().c_str ());
}

inline vfy_string vfy_get_method_name (vfy_method *method)
{
  return const_cast<char *> (method->method->get_name ().c_str ());
}

inline bool vfy_is_static (vfy_method *method)
{
  return method->method->static_p ();
}

inline const unsigned char *vfy_get_bytecode (vfy_method *method)
{
  return method->block->get_bytecode ();
}

inline vfy_exception *vfy_get_exceptions (vfy_method *method)
{
  return method->block->get_exceptions ();
}

inline void vfy_get_exception (vfy_exception *table, int index, int *handler,
			       int *start, int *end, int *handler_type)
{
  *handler = table[index].handler;
  *start = table[index].start;
  *end = table[index].end;
  *handler_type = table[index].type;
}

inline int vfy_tag (vfy_constants *pool, int index)
{
  return pool->tag (index);
}

inline void vfy_load_indexes (vfy_constants *pool, int index,
			      vfy_uint_16 *index0, vfy_uint_16 *index1)
{
  jword val = pool->value (index);
  *index0 = (val >> 16) & 0xffff;
  *index1 = val & 0xffff;
}

inline int vfy_get_constants_size (vfy_method *method)
{
  return vfy_get_constants (method)->size ();
}

inline vfy_string vfy_get_pool_string (vfy_constants *pool, int index)
{
  return const_cast<char *> (pool->get_utf8 (index).c_str ());
}

inline vfy_jclass vfy_find_class (vfy_method *method, vfy_jclass,
				  vfy_string name)
{
  int array_count = 0;
  int offset = 0;
  int len = strlen (name);

  for (; offset < len && name[offset] == '['; ++offset)
    ++array_count;

  // FIXME: throw error...
  if (offset == len)
    return NULL;

  int nlen = len - offset;

  vfy_jclass result = NULL;
  if (nlen == 1)
    {
      switch (name[offset])
	{
	case 'Z':
	  result = primitive_boolean_type;
	  break;
	case 'B':
	  result = primitive_byte_type;
	  break;
	case 'C':
	  result = primitive_char_type;
	  break;
	case 'S':
	  result = primitive_short_type;
	  break;
	case 'I':
	  result = primitive_int_type;
	  break;
	case 'J':
	  result = primitive_long_type;
	  break;
	case 'F':
	  result = primitive_float_type;
	  break;
	case 'D':
	  result = primitive_double_type;
	  break;
	case 'V':
	  result = primitive_void_type;
	  break;
	default:
	  // This is goofy but we handle either kind of descriptor
	  // here and we might be looking for a class with a
	  // 1-character name.
	  break;
	}
    }

  if (result == NULL)
    {
      if (name[offset] == 'L' && name[len - 1] == ';')
	{
	  ++offset;
	  nlen -= 2;
	}
      std::string name_str (name, offset, nlen);
      result = method->unit->find_class_from_descriptor (method->scope,
							 method->method,
							 name_str);
    }

  while (array_count-- > 0)
    result = result->array ();

  return result;
}

/**
 * Tests whether class k has a method with a given name and descriptor. Most
 * verifiers don't check this, so implementations are free to simply return
 * true regardless, and report the problem later with a NoSuchMethodException
 * at runtime or whatever. gjcx can use this to check the bytecode it emits.
 */
inline bool vfy_has_method (vfy_jclass k, vfy_string method_name,
                            vfy_string method_descriptor)
{
  model_class *klass = assert_cast<model_class *> (k);
  klass->resolve_members ();
  return klass->has_method_with_descriptor_p (method_name, method_descriptor);
}

inline vfy_jclass vfy_get_pool_resolved_class (vfy_method *method,
					       vfy_constants *pool,
					       int index)
{
  return vfy_find_class (method, method->method->get_declaring_class (),
			 const_cast<char *> (pool->get_class (index).c_str ()));
}

inline vfy_string vfy_get_pool_class (vfy_method *method, vfy_constants *pool,
				      int index)
{
  return const_cast<char *> (pool->get_class (index).c_str ());
}

// Return true if the class declares a field with the given name.  The
// type of the field doesn't matter.
inline bool vfy_class_has_field_p (vfy_jclass type, vfy_string name)
{
  std::set<model_field *> result;
  model_class *klass = assert_cast<model_class *> (type);
  klass->find_members (name, result, klass, klass);
  return ! result.empty ();
}

inline vfy_string vfy_make_string (const char *s, int len)
{
  char *r = new char[len + 1];
  strncpy (r, s, len);
  r[len] = '\0';
  return r;
}

inline vfy_string vfy_get_class_name (vfy_jclass klass)
{
  return const_cast<char *> (assert_cast<model_class *> (klass)->get_name ().c_str ());
}

inline char vfy_get_primitive_char (vfy_jclass klass)
{
  return klass->get_descriptor ().c_str ()[0];
}


typedef std::list<ref_forwarding_type>::const_iterator vfy_iface_iterator;

inline vfy_iface_iterator vfy_iface_iterator_begin (vfy_jclass klass)
{
  return assert_cast<model_class *> (klass)->get_interfaces ().begin ();
}

inline bool vfy_iface_iterator_done (vfy_jclass klass,
				     vfy_iface_iterator &iter)
{
  return iter == assert_cast<model_class *> (klass)->get_interfaces ().end ();
}

inline vfy_jclass vfy_get_interface (vfy_jclass, vfy_iface_iterator &iter)
{
  return (*iter)->type ();
}

inline bool vfy_is_array (vfy_jclass klass)
{
  return klass->array_p ();
}

inline bool vfy_is_interface (vfy_jclass klass)
{
  return klass->interface_p ();
}

// FIXME: what to do for void?
inline bool vfy_is_primitive (vfy_jclass klass)
{
  return klass->primitive_p ();
}

inline vfy_jclass vfy_get_superclass (vfy_jclass klass)
{
  return assert_cast<model_class *> (klass)->get_superclass ();
}

inline vfy_jclass vfy_get_array_class (vfy_jclass klass)
{
  return klass->array ();
}

inline vfy_jclass vfy_get_component_type (vfy_jclass klass)
{
  return klass->element_type ();
}

inline bool vfy_is_abstract (vfy_jclass klass)
{
  return assert_cast<model_class *> (klass)->abstract_p ();
}

inline vfy_jclass vfy_object_type (void)
{
  return global->get_compiler ()->java_lang_Object ();
}

inline vfy_jclass vfy_string_type (void)
{
  return global->get_compiler ()->java_lang_String ();
}

inline vfy_jclass vfy_throwable_type (void)
{
  return global->get_compiler ()->java_lang_Throwable ();
}

inline vfy_jclass vfy_class_type (void)
{
  return global->get_compiler ()->java_lang_Class ();
}

__attribute__ ((__noreturn__))
inline int vfy_fail (const char *message, int pc, vfy_jclass,
		     vfy_method *method)
{
  if (pc == -1)
    throw method->block->error ("verification failed in method %1: %u2")
      % method->method % message;
  throw method->block->error ("verification failed at PC %u1 "
			      "in method %2: %u3")
    % pc % method->method % message;
}

// Return the primitive type corresponding to the argument to
// `newarray'.
inline vfy_jclass vfy_get_primitive_type (int type)
{
  switch (type)
    {
    case 4:
      return primitive_boolean_type;
    case 5:
      return primitive_char_type;
    case 6:
      return primitive_float_type;
    case 7:
      return primitive_double_type;
    case 8:
      return primitive_byte_type;
    case 9:
      return primitive_short_type;
    case 10:
      return primitive_int_type;
    case 11:
      return primitive_long_type;
    }
  return NULL;
}

// Return true if this method comes from a 1.5 class file.
inline bool vfy_is_15 (vfy_method *method)
{
  return method->unit->is_15_p ();
}

// Do something with the 'flags' array.
// (A user could just delete[] it...)
inline void vfy_hand_off_flags (vfy_method *method, unsigned char *flags)
{
  method->block->set_flags (flags);
}

#define JV_CONSTANT_Class CONSTANT_Class
#define JV_CONSTANT_ResolvedClass CONSTANT_ResolvedClass
#define JV_CONSTANT_String CONSTANT_String
#define JV_CONSTANT_ResolvedString CONSTANT_ResolvedString
#define JV_CONSTANT_Integer CONSTANT_Integer
#define JV_CONSTANT_Float CONSTANT_Float
#define JV_CONSTANT_Long CONSTANT_Long
#define JV_CONSTANT_Double CONSTANT_Double
#define JV_CONSTANT_Fieldref CONSTANT_Fieldref
#define JV_CONSTANT_InterfaceMethodref CONSTANT_InterfaceMethodref
#define JV_CONSTANT_Methodref CONSTANT_Methodref

extern void _Jv_VerifyMethod (vfy_method *meth);

#endif // GCC_VERIFY_H
