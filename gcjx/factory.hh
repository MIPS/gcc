// Classes that know how to load Java classes.

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

#ifndef GCJX_FACTORY_HH
#define GCJX_FACTORY_HH

#include "typedefs.hh"

class byte_buffer;
class class_instance_creator;
class model_class;
class zip_entry_reader;

/// This is the base class for all class factories.  A class factory
/// is a source of class-instantiate objects.  A typical class factory
/// will understand how to find sources in a single way, for instance
/// file system lookup.
class class_factory
{
  // Whether we're part of the sourcepath.
  bool source_only;

public:

  virtual ~class_factory ()
  {
  }

  /// Return an instance of a class factory appropriate for the given
  /// type of component (folder, JAR/ZIP file, shared library, etc.).
  /// @param source_only whether this is part of the sourcepath.
  static class_factory *get_class_factory (const std::string &, bool source_only);

  /// Return a new class_instance_creator representing a .java file,
  /// given the name of the .java file.
  virtual class_instance_creator *find_source_file (const std::string &) = 0;

  /// Return a new class_instance_creator representing a .java file
  /// for a given fully-qualified class name.
  virtual class_instance_creator *find_source_file (const std::list<std::string> &)
    = 0;

  /// Return a new class_instance_creator representing a .class file
  /// (or other derived class source) for a given fully-qualified
  /// class name.  (The name is a bit incorrect as it isn't necessary
  /// that we find an actual file.)
  virtual class_instance_creator *find_derived_file (const std::list<std::string> &)
    = 0;

  bool is_source_only () const;
};

// Can instantiate from either .class or .java.
class directory_class_factory : public class_factory
{
  friend class_factory *class_factory::get_class_factory (const std::string &, bool);

private:

  // Directory we search.
  std::string directory;

  directory_class_factory (const std::string &);

  class_instance_creator *find_source_file (const std::string &);
  class_instance_creator *find_source_file (const std::list<std::string> &);
  class_instance_creator *find_derived_file (const std::list<std::string> &);
};

class jar_class_factory : public class_factory
{
  friend class_factory *class_factory::get_class_factory (const std::string &, bool);

private:

  // The JAR or ZIP file.
  std::string file;

  // The data from the archive file.
  byte_buffer* archive;

  // The cached ZIP directory entries.
  std::map<std::string, zip_entry_reader *> zip_entries;

  jar_class_factory (const std::string &);

  ~jar_class_factory ();

  void open_zip_file ();
  void read_zip_archive ();
  void read_zip_file_header ();
  void process_zip_central_directory ();
  void process_zip_entries (const uint8 *, uint16);
  const uint8 *process_zip_entry (const uint8 *);
  const uint8* find_end_of_zip_central_directory ();

  io_error make_error (const std::string &);
  void check_magic (const uint8 *buffer, const uint32 magic);

  zip_entry_reader *find_zip_entry (const std::string &);

  class_instance_creator *find_source_file (const std::string &);
  class_instance_creator *find_source_file (const std::list<std::string> &);
  class_instance_creator *find_derived_file (const std::list<std::string> &);
};

// Load classes from a gcj-compiled .so.
class solib_class_factory : public class_factory
{
  friend class_factory *class_factory::get_class_factory (const std::string &, bool);

private:

  // The solib.
  std::string file;

  solib_class_factory (const std::string &);

  class_instance_creator *find_source_file (const std::string &)
  {
    return NULL;
  }

  class_instance_creator *find_source_file (const std::list<std::string> &)
  {
    // We don't allow source files in an solib, it doesn't make sense.
    return NULL;
  }

  class_instance_creator *find_derived_file (const std::list<std::string> &);
};

/// This is like a class_factory, but not derived from one.  It
/// handles a list of such factories and presents a single exported
/// method which can be used to read a class from some source.
class classpath_class_factory
{
  // List of factories to try.
  std::list<class_factory *> factories;

public:

  classpath_class_factory (const std::list<class_factory *> &f)
    : factories (f)
  {
  }

  ~classpath_class_factory ();

  void load_class (const std::list<std::string> &);

  /// Find a java source file on the class path, parse it, and
  /// schedule it for eventual semantic analysis and code generation.
  /// Returns false if the file could not be found, true otherwise.
  /// Note that this returns true if the file fails to parse -- error
  /// messages from this are caught and handled elsewhere.
  bool load_source_file (const std::string &);
};

#endif // GCJX_FACTORY_HH
