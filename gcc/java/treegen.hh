// Code generator for GENERIC.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
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

#ifndef GCC_JAVA_TREEGEN_HH
#define GCC_JAVA_TREEGEN_HH

class tree_builtins;

class tree_code_generator : public code_generator
{
  tree_builtins *builtins;

  // True before we have been asked to compile a class.
  bool first_time;

public:

  tree_code_generator (compiler *, directory_cache &);

  ~tree_code_generator ();

  void generate (model_class *);

  void compile_resource (const std::string &, byte_buffer *);

  bool handles_class_p () const
  {
    return true;
  }

  bool needs_class_method_bodies_p () const
  {
    return true;
  }

  bool handles_resources_p () const
  {
    return true;
  }
};

#endif // GCC_JAVA_TREEGEN_HH
