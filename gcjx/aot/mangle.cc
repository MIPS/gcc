// Name mangler that is compatible with g++.

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
#include "aot/mangle.hh"

char
mangler::get_type_name (model_type *t)
{
  assert (t->primitive_p () || t == primitive_void_type);

  if (t == primitive_boolean_type)
    return 'b';
  if (t == primitive_byte_type)
    return 'c';
  if (t == primitive_char_type)
    return 'w';
  if (t == primitive_double_type)
    return 'd';
  if (t == primitive_float_type)
    return 'f';
  if (t == primitive_int_type)
    return 'i';
  if (t == primitive_long_type)
    return 'x';
  if (t == primitive_short_type)
    return 's';
  if (t == primitive_void_type)
    return 'v';

  abort ();
}

void
mangler::emit_saved (int n)
{
  // For N == 0 we emit "S_".
  // Otherwise we emit S<N>_, where <N> is
  // the base 36 representation of N - 1.
  result += "S";
  if (n > 0)
    {
      --n;
      char out[20];
      int where = 19;
      out[where] = '\0';
      while (n > 0)
	{
	  int digit = n % 36;
	  out[--where] = digit > 9 ? ('A' + n - 10) : ('0' + n);
	  n /= 36;
	}
      if (where == 19)
	out[--where] = '0';
      result += &out[where];
    }
  result += "_";
}

int
mangler::find_compression (model_element *elt, bool is_pointer)
{
  int index = 0;
  for (std::vector<cache_entry>::const_iterator i = compression_table.begin ();
       i != compression_table.end ();
       ++i, ++index)
    {
      const cache_entry &ce (*i);
      if (elt == ce.element && is_pointer == ce.is_pointer)
	return index;
    }
  return -1;
}

void
mangler::insert (model_element *elt, bool is_pointer)
{
  cache_entry ent;
  ent.element = elt;
  ent.is_pointer = is_pointer;
  compression_table.push_back (ent);
}

void
mangler::update (const std::string &name)
{
  // If we see a substring that is a literal "__U", we have to
  // remangle it as "__U_".  This keeps track of where we are in the
  // substring.
  int state = 0;
  const char *search = "__U";

  std::ostringstream output;
  unsigned int i = 0;
  while (i < name.length ())
    {
      jchar ch;
      unsigned char c = name[i++];
      unsigned char c1, c2;
      if (c < 128)
	ch = c;
      else
	{
	  if ((c & 0xe0) == 0xc0)
	    {
	      assert (i < name.length ());  // FIXME
	      c1 = name[i++];
	      if ((c1 & 0xc0) == 0x80)
		{
		  jchar r = (jchar) (((c & 0x1f) << 6) + (c1 & 0x3f));
		  /* Check for valid 2-byte characters.  We explicitly
		     allow \0 because this encoding is common in the
		     Java world.  */
		  if (r == 0 || (r >= 0x80 && r <= 0x7ff))
		    ch = r;
		}
	      else
		abort ();	// FIXME
	    }
	  else if ((c & 0xf0) == 0xe0)
	    {
	      assert (i < name.length ());  // FIXME
	      c1 = name[i++];
	      if ((c1 & 0xc0) == 0x80)
		{
		  assert (i < name.length ());  // FIXME
		  c2 = name[i++];
		  if ((c2 & 0xc0) == 0x80)
		    {
		      jchar r =  (jchar) (((  c & 0xf) << 12) + 
					  (( c1 & 0x3f) << 6)
					  + (c2 & 0x3f));
		      /* Check for valid 3-byte characters.
			 Don't allow surrogate, \ufffe or \uffff.  */
		      if (r >= 0x800
			  && ! (r >= 0xd800 && r <= 0xdfff)
			  && r != 0xfffe && r != 0xffff)
			ch = r;
		      else
			abort (); // FIXME
		    }
		  else
		    abort ();	// FIXME
		}
	      else
		abort ();	// FIXME
	    }
	  else
	    abort ();		// FIXME
	}

      if (ch == search[state])
	{
	  ++state;
	  if (search[state] == '\0')
	    {
	      // Emit the mangled form.
	      output << "__U_";
	      state = 0;
	    }
	}
      else
	{
	  // Emit any characters we recognized.
	  for (int i = 0; i < state; ++i)
	    output << search[i];
	  state = 0;

	  if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
	      || (ch >= '0' && ch <= '9') || ch == '_' || ch == '$')
	    {
	      // ASCII character, just emit.
	      output << char (ch);
	    }
	  else
	    {
	      // "Unicode" character, must mangle.
	      char buf[20];
	      sprintf (buf, "__U%x_", int (ch));
	      output << buf;
	    }
	}
    }

  // Emit any characters we recognized.
  for (int i = 0; i < state; ++i)
    output << search[i];

  std::string s = output.str ();
  char buf[20];
  sprintf (buf, "%d", s.length ());

  result += buf + s;
}

void
mangler::update_cxx (const std::string &name)
{
  update (name);
  // If we have a member whose name is a C++ keyword, we append '$'.
  // Note that, strangely, we do this after encoding the length of the
  // member name.  E.g., we end up with things like "6delete$" rather
  // than "7delete$".
  if (cxx_keyword_p (name))
    result = result + "$";
}

void
mangler::update (model_package *p)
{
  int n = find_compression (p, false);
  if (n >= 0)
    emit_saved (n);
  else
    {
      if (p->get_parent ())
	update (p->get_parent ());
      std::string name = p->get_simple_name ();
      if (! name.empty ())
	{
	  update (name);
	  insert (p, false);
	}
    }
}

void
mangler::update_array (model_array_type *t)
{
  int n = find_compression (t, true);
  if (n >= 0)
    emit_saved (n);
  else
    {
      // Handle the 'JArray' part specially, by representing the
      // element as NULL.
      result += "P";
      n = find_compression (NULL, true);
      if (n >= 0)
	emit_saved (n);
      else
	{
	  insert (NULL, true);
	  result += "6JArrayI";
	  update (t->element_type (), true);
	  result += "E";
	  insert (t, false);
	  insert (t, true);
	}
    }
}

void
mangler::update (model_type *t, bool is_pointer)
{
  if (t->primitive_p () || t == primitive_void_type)
    result += get_type_name (t);
  else if (t->array_p ())
    {
      // Always a pointer in this case.
      assert (is_pointer);
      update_array (assert_cast<model_array_type *> (t));
    }
  else
    {
      bool enter = false;
      bool enter_pointer = false;
      if (is_pointer)
	{
	  int n = find_compression (t, true);
	  if (n >= 0)
	    {
	      emit_saved (n);
	      return;
	    }
	  result += "P";
	  enter_pointer = true;
	}
      int n = find_compression (t, false);
      if (n >= 0)
	emit_saved (n);
      else
	{
	  result += "N";
	  model_class *k = assert_cast<model_class *> (t);
	  if (k->get_package ())
	    update (k->get_package ());
	  update (k->get_output_name ());
	  enter = true;
	  // This is a hack: we know only the outer-most class
	  // reference will be called with is_pointer == false.
	  if (is_pointer)
	    result += "E";
	}

      if (enter)
	insert (t, false);
      if (enter_pointer)
	insert (t, true);
    }
}

mangler::mangler (model_type *t)
  : result ("_Z")
{
  assert (t->reference_p ());
  update (t, false);
  // We assume this is a reference to 'Name.class'.
  result += "6class$E";
}

mangler::mangler (model_method *m)
  : result ("_Z")
{
  // Emit the declaring class.  Note that we might be called for a
  // method that is declared for an array type.  In this case we
  // substitute the Object variant.  This is a bit strange in that
  // really we will never emit references to these names.  FIXME: fix
  // in the caller?
  model_class *decl = m->get_declaring_class ();
  if (decl->array_p ())
    decl = global->get_compiler ()->java_lang_Object ();
  update (decl, false);

  // Emit the name, or the special name we use for a constructor.
  if (m->constructor_p ())
    result += "C1";
  else
    update_cxx (m->get_name ());

  result += "E";

  // Emit the argument types.
  std::list<ref_variable_decl> params = m->get_parameters ();
  // Special case for no arguments.
  if (params.empty ())
    update (primitive_void_type, true);
  else
    {
      for (std::list<ref_variable_decl>::const_iterator i = params.begin ();
	   i != params.end ();
	   ++i)
	update ((*i)->type (), true);
    }
}

mangler::mangler (model_field *f)
  : result ("_Z")
{
  update (f->get_declaring_class (), false);
  update_cxx (f->get_name ());
  result += "E";
}

mangler::mangler (model_class *declaring, const char *fieldname)
  : result ("_Z")
{
  update (declaring, false);
  update (fieldname);
  result += "E";
}

mangler::mangler (model_class *klass, bool)
  : result ("_ZTV")
{
  update (klass, false);
  result += "E";
}
