// Write a JNI stub.

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
#include "header/jni.hh"
#include "header/jnistub.hh"

jni_stub_generator::jni_stub_generator (compiler *c, directory_cache &dirs)
  : jni_code_generator (c, dirs)
{
}

std::string
jni_stub_generator::file_name (const std::string &base)
{
  return base + ".c";
}

void
jni_stub_generator::write_header (std::ostream &out,
				  const std::string &base)
{
  out << "/* This file was created by 'fixme'." << std::endl
      << "   It is intended to give you a head start on implementing native"
      << std::endl
      << "   methods using JNI.  Be aware: running 'fixme' once more for this"
      << std::endl
      << "   class will overwrite any edits you have made to this file. */"
      << std::endl
      << std::endl
      << "#include <jni.h>" << std::endl
      << "#include \"" << base << ".h\"" << std::endl;
}

void
jni_stub_generator::write_field (std::ostream &, const std::string &,
				 model_field *)
{
  // Nothing.
}

void
jni_stub_generator::write_trailer (std::ostream &, const std::string &)
{
  // Nothing.
}

void
jni_stub_generator::write_method (std::ostream &out,
				  model_method *meth,
				  const std::string &class_name,
				  bool long_format)
{
  // This is the blank line separating functions, or separating the
  // header and the first function.
  out << std::endl;

  write (out, meth->get_return_type ());
  out << std::endl;
  out << "Java_" << class_name << "_";
  out << translate (meth, meth->get_name ());
  if (long_format)
    {
      std::string sig = meth->get_descriptor ();
      assert (sig[0] == '(');
      int roff = sig.rfind (')');
      assert (roff >= 1);
      sig = sig.substr (1, roff - 1);
      out << "_" << translate (meth, sig);
    }

  out << " (JNIEnv *env, "
      << (meth->static_p () ? "jclass klass" : "jobject self");

  std::list<ref_variable_decl> params = meth->get_parameters ();
  int count = 0;
  for (std::list<ref_variable_decl>::const_iterator i = params.begin ();
       i != params.end ();
       ++i)
    {
      out << ", ";
      write (out, (*i)->type ());
      std::string name = (*i)->get_name ();
      if (name.empty ())
	out << " arg" << count++;
      else
	out << " " << name;
    }
  out << ")" << std::endl
      << "{" << std::endl
      << "}" << std::endl;
}
