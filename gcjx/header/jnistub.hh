// Write a JNI stub.

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

#ifndef GCJX_HEADER_JNISTUB_HH
#define GCJX_HEADER_JNISTUB_HH

class jni_stub_generator : public jni_code_generator
{
protected:

  std::string file_name (const std::string &);
  void write_header (std::ostream &, const std::string &);
  void write_trailer (std::ostream &, const std::string &);
  void write_method (std::ostream &, model_method *,
		     const std::string &, bool);
  void write_field (std::ostream &, const std::string &,
		    model_field *);

public:

  /// Construct a new JNI stub generator.  Argument is the compiler
  /// being used to generate code.
  jni_stub_generator (compiler *, directory_cache &dirs);
};

#endif // GCJX_HEADER_JNISTUB_HH
