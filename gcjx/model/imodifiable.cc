// "modifiable" interface.

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

#include "typedefs.hh"

void
IModifiable::set_modifiers (const ref_modifier_list &mods)
{
  modifiers = mods->value ();
  massage_modifiers (mods);
}

void
IModifiable::implicitly_public ()
{
  if ((modifiers & ACC_ACCESS) == ACC_PUBLIC)
    {
      model_element *request = assert_cast<model_element *> (this);
      if (! from_class && ! synthetic_p ()
	  && global->get_compiler ()->warn_redundant_modifier ())
	std::cerr << request->warn (global->get_compiler ()->warn_redundant_modifier (),
				    "implicitly %<public%> member "
				    "redundantly declared %<public%>");
    }
  else if ((modifiers & ACC_ACCESS) != 0)
    {
      model_element *request = assert_cast<model_element *> (this);
      // ugh, "object"?
      std::cerr << request->error ("implicitly %<public%> member "
				   "also declared %1")
	% model_modifier_list::name (modifiers & ACC_ACCESS);
    }
  modifiers &= ~ACC_ACCESS;
  modifiers |= ACC_PUBLIC;
}

void
IModifiable::implicit_modifier (modifier_t addition)
{
  if ((modifiers & addition) != 0
      && ! from_class && ! synthetic_p ()
      && global->get_compiler ()->warn_redundant_modifier ())
    {
      model_element *request = assert_cast<model_element *> (this);
      const char *n = model_modifier_list::name (addition);
      std::cerr << request->warn (global->get_compiler ()->warn_redundant_modifier (),
				  "implicitly %1 member "
				  "redundantly declared %2")
	% n % n;
    }
  modifiers |= addition;
}
