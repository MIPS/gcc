// Write a JNI header.

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

#ifndef GCJX_HEADER_JNI_HH
#define GCJX_HEADER_JNI_HH

#include "codegen.hh"

class jni_code_generator : public code_generator
{
protected:

  // The compiler we're using.
  compiler *comp;

  void write (std::ostream &, model_type *);
  void add_char (char *, jchar, int *);
  std::string translate (model_element *, const std::string &);

  virtual std::string file_name (const std::string &);
  virtual void write_header (std::ostream &, const std::string &);
  virtual void write_trailer (std::ostream &, const std::string &);
  virtual void write_method (std::ostream &, model_method *,
			     const std::string &, bool);
  virtual void write_field (std::ostream &, const std::string &,
			    model_field *);

public:

  /// Construct a new JNI code generator.  Argument is the compiler
  /// being used to generate code.
  jni_code_generator (compiler *, directory_cache &);

  /// This is called to generate code for a class and write it.
  void generate (model_class *);
};

#endif // GCJX_HEADER_JNI_HH
