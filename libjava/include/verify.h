/* Declarations to interface libgcj with bytecode verifier.
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

// This is glue between libjava and the verifier.

#ifndef JV_VERIFY_H
#define JV_VERIFY_H

#include "java-cpool.h"
#include "java/lang/Class.h"

typedef _Jv_Constants *vfy_constants;

typedef _Jv_Utf8Const *vfy_string;

typedef jclass vfy_jclass;

typedef jchar vfy_uint_16;

typedef _Jv_InterpException vfy_exception;

typedef _Jv_InterpMethod vfy_method;

inline vfy_constants *vfy_get_constants (vfy_method *method)
{
  return &method->defining_class->constants;
}

inline void *vfy_alloc (size_t bytes)
{
  return _Jv_Malloc (bytes);
}

inline void vfy_free (void *mem)
{
  _Jv_Free (mem);
}

inline bool vfy_strings_equal (vfy_string one, vfy_string two)
{
  return _Jv_equalUtf8Consts (one, two);
}

inline const char *vfy_string_bytes (vfy_string str)
{
  return str->chars();
}

inline vfy_string vfy_init_name (void)
{
  return gcj::init_name;
}

inline vfy_string vfy_clinit_name (void)
{
  return gcj:clinit_name;
}

int vfy_count_arguments (vfy_string signature)
{
  return _Jv_count_arguments (signature, false);
}

inline vfy_string vfy_get_signature (vfy_method *method)
{
  return method->self->signature;
}

inline vfy_string vfy_get_method_name (vfy_method *method)
{
  return method->self->name;
}

inline bool vfy_is_static (vfy_method *method)
{
  return (method->self->accflags & ACC_STATIC) != 0;
}

inline const unsigned char *vfy_get_bytecode (vfy_method *method)
{
  return method->bytecode ();
}

inline vfy_exception *vfy_get_exceptions (vfy_method *method)
{
  return method->block->get_exceptions ();
}

inline void vfy_get_exception (vfy_exception *table, int index, int *handler,
			       int *start, int *end, int *handler_type)
{
  *handler = table[index].handler_pc.i;
  *start = table[index].start_pc.i;
  *end = table[index].end_pc.i;
  *handler_type = table[index].handler_type.i;
}

inline int vfy_tag (vfy_constants *pool, int index)
{
  return pool->tag[index];
}

inline void vfy_load_indexes (vfy_constants *pool, int index,
			      vfy_uint_16 *index0, vfy_uint_16 *index1)
{
  _Jv_loadIndexes (pool->data, *index0, *index1);
}

inline int vfy_get_constants_size (vfy_method *method)
{
  return method->defining_class->constants.size;
}

inline vfy_string vfy_get_pool_string (vfy_constants *pool, int index)
{
  return pool->data[index].utf8;
}

inline vfy_jclass vfy_find_class (vfy_method *method, vfy_jclass context,
				  vfy_string name)
{
  using namespace java::lang;
  ClassLoader *loader = context->getClassLoaderInternal();
  // We might see either kind of name.  Sigh.
  vfy_jclass klass;
  if (name->first() == 'L' && name->limit()[-1] == ';')
    klass = _Jv_FindClassFromSignature (name->chars(), loader);
  else
    klass = Class::forName (_Jv_NewStringUtf8Const (name), false, loader);
  return klass;
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
  for (int i = 0; i < k->method_count; ++i)
    {
      if (_Jv_equalUtf8Consts (k->methods[i].name, method_name)
	  && _Jv_equalUtf8Consts (k->methods[i].signature, method_descriptor))
	return true;
    }
  return false;
}

inline vfy_jclass vfy_get_pool_resolved_class (vfy_method *method,
					       vfy_constants *pool,
					       int index)
{
  return vfy_find_class (method, method->declaring_class,
			 pool->data[index].utf8);
}

inline vfy_string vfy_get_pool_class (vfy_method *method, vfy_constants *pool,
				      int index)
{
  // FIXME: is pool linked at this point?
  return pool->data[index].utf8;
}

// Return true if the class declares a field with the given name.  The
// type of the field doesn't matter.
inline bool vfy_class_has_field_p (vfy_jclass type, vfy_string name)
{
  for (int i = 0; i < type->field_count; ++i)
    {
      if (_Jv_equalUtf8Consts (type->fields[i].name, name))
	return true;
    }
  return false;
}

inline vfy_string vfy_make_string (const char *s, int len)
{
  return _Jv_makeUtf8Const (s, len);
}

inline vfy_string vfy_get_class_name (vfy_jclass klass)
{
  return klass->name;
}

inline char vfy_get_primitive_char (vfy_jclass klass)
{
  return char (klass->method_count);
}


typedef int vfy_iface_iterator;

inline vfy_iface_iterator vfy_iface_iterator_begin (vfy_jclass klass)
{
  return 0;
}

inline bool vfy_iface_iterator_done (vfy_jclass klass, vfy_iface_iterator iter)
{
  return iter == klass->interface_count;
}

inline vfy_jclass vfy_get_interface (vfy_jclass klass, vfy_iface_iterator iter)
{
  return klass->interface_count;
}

inline bool vfy_is_array (vfy_jclass klass)
{
  return klass->isArray ();
}

inline bool vfy_is_interface (vfy_jclass klass)
{
  return klass->isInterface ();
}

// FIXME: what to do for void?
inline bool vfy_is_primitive (vfy_jclass klass)
{
  return klass->isPrimitive ();
}

inline vfy_jclass vfy_get_superclass (vfy_jclass klass)
{
  return klass->getSuperclass ();
}

inline vfy_jclass vfy_get_array_class (vfy_jclass klass)
{
  return _Jv_GetArrayClass (klass, klass->getClassLoaderInternal ());
}

inline vfy_jclass vfy_get_component_type (vfy_jclass klass)
{
  return klass->getComponentType ();
}

inline bool vfy_is_abstract (vfy_jclass klass)
{
  return (klass->getModifiers () & ACC_ABSTRACT) != 0;
}

inline vfy_jclass vfy_object_type (void)
{
  return &java::lang::Object::class$
}

inline vfy_jclass vfy_string_type (void)
{
  return &java::lang::String::class$
}

inline vfy_jclass vfy_throwable_type (void)
{
  return &java::lang::Throwable::class$
}

inline vfy_jclass vfy_class_type (void)
{
  return &java::lang::Class::class$
}

__attribute__ ((__noreturn__))
inline int vfy_fail (const char *message, int pc, vfy_jclass,
		     vfy_method *method)
{
  using namespace java::lang;
  StringBuffer *buf = new StringBuffer ();

  buf->append (JvNewStringLatin1 ("verification failed"));
  if (pc != -1)
    {
      buf->append (JvNewStringLatin1 (" at PC "));
      buf->append (pc);
    }

  buf->append (JvNewStringLatin1 (" in method "));
  buf->append (current_class->getName());
  buf->append ((jchar) '.');
  buf->append (method->get_method()->name->toString());
  buf->append ((jchar) '(');
  buf->append (method->get_method()->signature->toString());
  buf->append ((jchar) ')');

  buf->append (JvNewStringLatin1 (": "));
  buf->append (JvNewStringLatin1 (message));
  throw new java::lang::VerifyError (buf->toString ());
}

// Return the primitive type corresponding to the argument to
// `newarray'.
inline vfy_jclass vfy_get_primitive_type (int type)
{
  switch (type)
    {
    case 4:
      return JvPrimClass (boolean);
    case 5:
      return JvPrimClass (char);
    case 6:
      return JvPrimClass (float);
    case 7:
      return JvPrimClass (double);
    case 8:
      return JvPrimClass (byte);
    case 9:
      return JvPrimClass (short);
    case 10:
      return JvPrimClass (int);
    case 11:
      return JvPrimClass (long);
    }
  return NULL;
}

// Return true if this method comes from a 1.5 class file.
inline bool vfy_is_15 (vfy_method *method)
{
  return false;			/* FIXME */
}

#define VFY_PRIMITIVE_CLASS(name) JvPrimClass (name)

extern void _Jv_VerifyMethod (vfy_method *meth);

#endif // JV_VERIFY_H
