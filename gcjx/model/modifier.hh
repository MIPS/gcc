// Represent a list of modifiers.

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

#ifndef GCJX_MODEL_MODIFIER_HH
#define GCJX_MODEL_MODIFIER_HH

#include "source/token.hh"

#define ACC_PUBLIC 0x0001
#define ACC_PRIVATE 0x0002
#define ACC_PROTECTED 0x0004
#define ACC_STATIC 0x0008
#define ACC_FINAL 0x0010
#define ACC_SYNCHRONIZED 0x0020
#define ACC_SUPER 0x0020
#define ACC_VOLATILE 0x0040
#define ACC_BRIDGE 0x0040
#define ACC_TRANSIENT 0x0080
#define ACC_VARARGS 0x0080
#define ACC_NATIVE 0x0100
#define ACC_INTERFACE 0x0200
#define ACC_ABSTRACT 0x0400
#define ACC_STRICTFP 0x0800
#define ACC_SYNTHETIC 0x1000
#define ACC_ANNOTATION 0x2000
#define ACC_ENUM 0x4000

#define ACC_ACCESS (ACC_PUBLIC | ACC_PROTECTED | ACC_PRIVATE)

// This represents a modifier list.
class model_modifier_list
{
  // Reference count, see owner<> template.
  int refc;

  // For reference count access.
  template<typename T> friend class owner;

  // The modifiers.
  std::map<modifier_t, location> modifiers;

  // The current value of this set of modifiers.
  modifier_t mods;

  // Order of last modifier added.
  int current_order;

  static const char *name (modifier_t flag)
  {
    switch (flag)
      {
      case ACC_PUBLIC:
	return "public";
      case ACC_PROTECTED:
	return "protected";
      case ACC_PRIVATE:
	return "private";
      case ACC_STATIC:
	return "static";
      case ACC_ABSTRACT:
	return "abstract";
      case ACC_FINAL:
	return "final";
      case ACC_NATIVE:
	return "native";
      case ACC_SYNCHRONIZED:
	return "synchronized";
      case ACC_TRANSIENT:
	return "transient";
      case ACC_VOLATILE:
	return "volatile";
      case ACC_STRICTFP:
	return "strictfp";
      default:
	abort ();
      }
  }

  static modifier_t get_value (token t)
  {
    modifier_t result = 0;
    switch (t.get_type ())
      {
      case TOKEN_PUBLIC:
	result = ACC_PUBLIC;
	break;
      case TOKEN_PROTECTED:
	result = ACC_PROTECTED;
	break;
      case TOKEN_PRIVATE:
	result = ACC_PRIVATE;
	break;
      case TOKEN_STATIC:
	result = ACC_STATIC;
	break;
      case TOKEN_ABSTRACT:
	result = ACC_ABSTRACT;
	break;
      case TOKEN_FINAL:
	result = ACC_FINAL;
	break;
      case TOKEN_NATIVE:
	result = ACC_NATIVE;
	break;
      case TOKEN_SYNCHRONIZED:
	result = ACC_SYNCHRONIZED;
	break;
      case TOKEN_TRANSIENT:
	result = ACC_TRANSIENT;
	break;
      case TOKEN_VOLATILE:
	result = ACC_VOLATILE;
	break;
      case TOKEN_STRICT:
	result = ACC_STRICTFP;
	break;
      default:
	// Note that we do nothing in this case, due to how
	// modifier_token_p is implemented.
	break;
      }
    return result;
  }

  static int get_order (token t)
  {
    switch (t.get_type ())
      {
      case TOKEN_PUBLIC:
	return 0;
      case TOKEN_PROTECTED:
	return 1;
      case TOKEN_PRIVATE:
	return 2;
      case TOKEN_ABSTRACT:
	return 3;
      case TOKEN_STATIC:
	return 4;
      case TOKEN_FINAL:
	return 5;
      case TOKEN_TRANSIENT:
	return 6;
      case TOKEN_VOLATILE:
	return 7;
      case TOKEN_SYNCHRONIZED:
	return 8;
      case TOKEN_NATIVE:
	return 9;
      case TOKEN_STRICT:
	return 10;
      default:
	abort ();
      }
  }

  // For access to 'name'.  FIXME.
  friend class IModifiable;

public:

  model_modifier_list ()
    : refc (0),
      mods (0),
      current_order (-1)
  {
  }

  static bool modifier_token_p (token t)
  {
    return get_value (t) != 0;
  }

  void check (model_element *request, modifier_t valid,
	      const char *error_format);

  void access_check (model_element *request, const char *error_format);

  modifier_t value ()
  {
    return mods;
  }

  void add (token t);
};

#endif // GCJX_MODEL_MODIFIER_HH
