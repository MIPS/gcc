// Loading classes.

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

#include "reader/mmapbuffer.hh"
#include "reader/readbuffer.hh"

#include <sstream>

#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

// Needed on DOS-based systems, including MS Windows.
#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

#include <zlib.h>

#include "source/parse.hh"
#include "factory.hh"
#include "reader/reader.hh"
#include "reader/classbytes.hh"
#include "reader/fdreader.hh"
#include "reader/zereader.hh"
#include "reader/source.hh"

#ifdef FIXME_USE_SOLIB

static inline bool
solib_file_p (const std::string &name)
{
  int name_len = name.length ();

  // FIXME:
  //   1. Fix this for case-insensitive file systems.
  //   2. Fix this for other shared library types (DLL, dyso, etc.)
  //   3. Ensure that name actually represents an existing shared library.
  return (name_len > 3 && name.compare (name_len - 3, 3, ".so") == 0);
}

#endif // FIXME_USE_SOLIB

class_factory *
class_factory::get_class_factory (const std::string &name, bool source_only)
{
  class_factory *ret_factory = NULL;

  if (jar_file_p (name))
    ret_factory = new jar_class_factory (name);
#ifdef FIXME_USE_SOLIB
  else if (solib_file_p (name))
    ret_factory = new solib_class_factory (name);
#endif // FIXME_USE_SOLIB
  else
    ret_factory = new directory_class_factory (name);

  if (ret_factory != NULL)
    ret_factory->source_only = source_only;

  return ret_factory;
}

bool
class_factory::is_source_only () const
{
  return source_only;
}



directory_class_factory::directory_class_factory (const std::string &d)
  : class_factory (),
    directory (d)
{
  // fixme ensure that D really is a directory...?
}

class_instance_creator *
directory_class_factory::find_source_file (const std::string &name)
{
  // FIXME should cache directory contents, gcj does this.
  std::string file (directory);
  if (! file.empty ())
    file += FILE_SEPARATOR;
  file += name;

  int fd = open (file.c_str (), O_RDONLY);
  if (fd < 0)
    return NULL;

  return new source_file_creator (file, new fd_reader (file, fd));
}

class_instance_creator *
directory_class_factory::find_source_file (const std::list<std::string> &name)
{
  return find_source_file (join (name, FILE_SEPARATOR) + ".java");
}

class_instance_creator *
directory_class_factory::find_derived_file (const std::string &name)
{
  assert (! is_source_only ());
  std::string file = directory;
  if (! file.empty ())
    file += FILE_SEPARATOR;
  file += name;
  int fd = open (file.c_str (), O_RDONLY | O_BINARY);
  if (fd < 0)
    return NULL;
  return new class_byte_creator (file, new fd_reader (file, fd));
}

class_instance_creator *
directory_class_factory::find_derived_file (const std::list<std::string> &name)
{
  return find_derived_file (join (name, FILE_SEPARATOR) + ".class");
}



/// Converts a 16-bit little-endian value to a native 16-bit quantity.
static inline uint16
make_u2 (const uint8 bytes[])
{
  return ((uint16) bytes[1] << 8
          | (uint16) bytes[0]);
}

/// Converts a 32-bit little-endian value to a native 32-bit quantity.
static inline uint32
make_u4 (const uint8 bytes[])
{
  return ((uint32) bytes[3] << 24
          | (uint32) bytes[2] << 16
          | (uint32) bytes[1] << 8
          | (uint32) bytes[0]);
}

/// Returns the zip_entry_reader for the given filename, if found
/// within this archive, else NULL.
zip_entry_reader *
jar_class_factory::find_zip_entry (const std::string &name)
{
  std::map<std::string, zip_entry_reader *>::iterator r
    = zip_entries.find (name);
  
  if (r != zip_entries.end ())
    return (*r).second;
  else
    return NULL;
}

/// Reads in the current ZIP archive and populates the cached zip
/// entries map.  Throws an io_error on failure.
void
jar_class_factory::read_zip_archive ()
{
  open_zip_file ();
  read_zip_file_header ();
  process_zip_central_directory ();
}

void
jar_class_factory::open_zip_file ()
{
  int arch_fd = open (file.c_str (), O_RDONLY | O_BINARY);
  if (arch_fd == -1)
    {
      std::ostringstream oss;
      oss << "couldn't open file: " << strerror (errno);
      throw make_error (oss.str ());
    }

  archive = new mmap_byte_buffer (location (file.c_str ()), arch_fd);
  close (arch_fd);
}

void
jar_class_factory::check_magic (const uint8* buffer, const uint32 magic)
{
  uint32 found = make_u4 (buffer);
  if (found != magic)
    {
      throw make_error ("found %1 instead of magic %2; corrupt JAR file?")
        % to_hex_string (found)
        % to_hex_string (magic);
    }
}

// Length of local file header.
static const int LREC_SIZE = 26;
// Length of central directory header.
static const int CREC_SIZE = 42;
// Length of end-of-central-directory record.
static const int ECREC_SIZE = 18;

void
jar_class_factory::read_zip_file_header ()
{
  static const uint32 ZIP_MAGIC = 0x04034b50;
  check_magic (archive->get (), ZIP_MAGIC);
}

/// Positions us just after the central directory magic, or throws an
/// io_error if we couldn't find the central directory.
const uint8*
jar_class_factory::find_end_of_zip_central_directory ()
{
  size_t end_offset = archive->get_length ();
  for (off_t guess_offset = end_offset - (ECREC_SIZE + 4);
       guess_offset > 0;
       --guess_offset)
    {
      const uint8* buffer = archive->get () + guess_offset;
      static const uint32 ECREC_MAGIC = 0x06054b50;
      if (make_u4 (buffer) == ECREC_MAGIC)
        return buffer;
    }

  throw make_error ("didn't find end-of-central-directory magic anywhere");
}

void
jar_class_factory::process_zip_central_directory ()
{
  const uint8* buffer = find_end_of_zip_central_directory ();

  // Useful offsets within the end of central directory record.
  const int TOTAL_ENTRIES_CENTRAL_DIR = 10;
  const int SIZE_CENTRAL_DIRECTORY = 12;
  const int OFFSET_CENTRAL_DIRECTORY = 16;

  // Now read in the number of files in this archive and the
  // size and location of the central directory.
  uint16 count = make_u2 (&buffer[TOTAL_ENTRIES_CENTRAL_DIR]);
  /*uint32 dir_size =*/ make_u4 (&buffer[SIZE_CENTRAL_DIRECTORY]);
  uint32 dir_offset = make_u4 (&buffer[OFFSET_CENTRAL_DIRECTORY]);

  process_zip_entries ((archive->get () + dir_offset), count);
}

const uint8 *
jar_class_factory::process_zip_entry (const uint8 *dir_ptr)
{
  // Process the central header for the file.
  static const uint32 CREC_MAGIC = 0x02014b50;
  check_magic (&dir_ptr[0], CREC_MAGIC);

  // Useful offsets within a central directory header.
  const int C_COMPRESSION_METHOD = 6;
  const int C_LAST_MOD_FILE_TIME = 8;
  const int C_COMPRESSED_SIZE = 16;
  const int C_UNCOMPRESSED_SIZE = 20;
  const int C_FILENAME_LENGTH = 24;
  const int C_EXTRA_FIELD_LENGTH = 26;
  const int C_RELATIVE_OFFSET_LOCAL_HEADER = 38;

  uint16 compression_method = make_u2 (&dir_ptr[4 + C_COMPRESSION_METHOD]);
  uint32 compressed_size
    = make_u4 (&dir_ptr[4 + C_COMPRESSED_SIZE]);
  uint32 uncompressed_size
    = make_u4 (&dir_ptr[4 + C_UNCOMPRESSED_SIZE]);

  uint32 dos_mod_time = make_u4 (&dir_ptr[4 + C_LAST_MOD_FILE_TIME]);

  uint16 filename_length = make_u2 (&dir_ptr[4 + C_FILENAME_LENGTH]);
  uint16 extra_field_length
    = make_u2 (&dir_ptr[4 + C_EXTRA_FIELD_LENGTH]);
  uint32 file_header_offset
    = make_u4 (&dir_ptr[4 + C_RELATIVE_OFFSET_LOCAL_HEADER]);

  // Check the magic in the local header for the file.
  const uint8* local_buffer = (archive->get () + file_header_offset);
  static const uint32 LREC_MAGIC = 0x04034b50;
  check_magic (&local_buffer[0], LREC_MAGIC);

  // Useful offsets within a local file header.
  const int L_FILENAME_LENGTH = 22;
  const int L_EXTRA_FIELD_LENGTH = 24;

  uint16 local_filename_length
    = make_u2 (&local_buffer[4 + L_FILENAME_LENGTH]);
  uint16 local_extra_field_length
    = make_u2 (&local_buffer[4 + L_EXTRA_FIELD_LENGTH]);

  uint32 file_start
    = file_header_offset + (4 + LREC_SIZE)
      + local_filename_length + local_extra_field_length;

  std::string filename ((char *)(&dir_ptr[CREC_SIZE + 4]), filename_length);

  // Note that we use the name of the zip file for dependency tracking
  // here.
  zip_entry_reader *ze_reader
    = new zip_entry_reader (file, archive->get () + file_start,
                            compression_method,
                            compressed_size,
                            uncompressed_size,
                            dos_mod_time);

  zip_entries[filename] = ze_reader;
  return (dir_ptr + CREC_SIZE + 4 + filename_length + extra_field_length);
}
      
void
jar_class_factory::process_zip_entries (const uint8 *central_directory,
					uint16 count)
{
  const uint8 *dir_ptr = central_directory;
  for (int i = 0; i < count; i++)
    {
      dir_ptr = process_zip_entry (dir_ptr);
    }
}

void
jar_class_factory::read_all ()
{
  // If the was a failure, we already issued the error message, so we
  // don't need to do anything here.
  if (! archive)
    return;

  compiler *comp = global->get_compiler ();
  bool handle_resource = comp->handles_resources_p ();
  for (std::map<std::string, zip_entry_reader *>::const_iterator i
	 = zip_entries.begin ();
       i != zip_entries.end ();
       ++i)
    {
      const std::string &name ((*i).first);
      zip_entry_reader *reader = (*i).second;

      class_instance_creator *source = NULL;
      if (class_file_p (name))
	source = new class_byte_creator (name, reader);
      else if (java_file_p (name))
	source = new source_file_creator (name, reader);
      else
	{
	  // Must be a resource file.
	  if (handle_resource)
	    comp->compile_resource (name, reader);
	  continue;
	}

      // Compile the class or java code.
      source->apply (true);
    }
}

jar_class_factory::jar_class_factory (const std::string &f)
  : class_factory (),
    file (f),
    archive (NULL)
{
  try
    {
      read_zip_archive ();
    }
  catch (io_error &ex)
    {
      std::cerr << ex;
      delete archive;
      archive = NULL;
    }
}

/// Creates an io_error initialized with the given message and with a
/// location appropriate to the JAR file we're trying to read.
io_error
jar_class_factory::make_error (const std::string &message)
{
  return io_error (location (file.c_str ()), message.c_str ());
}

class_instance_creator *
jar_class_factory::find_source_file (const std::string &name)
{
  zip_entry_reader *ze_reader = find_zip_entry (name);

  if (ze_reader != NULL)
    return new source_file_creator (name, ze_reader);
  else
    return NULL;
}

class_instance_creator *
jar_class_factory::find_source_file (const std::list<std::string> &name)
{
  return find_source_file (join (name, FILE_SEPARATOR) + ".java");
}

class_instance_creator *
jar_class_factory::find_derived_file (const std::string &name)
{
  assert (! is_source_only());
  zip_entry_reader *ze_reader = find_zip_entry (name);
  if (ze_reader == NULL)
    return NULL;
  return new class_byte_creator (name, ze_reader);
}

class_instance_creator *
jar_class_factory::find_derived_file (const std::list<std::string> &name)
{
  return find_derived_file (join (name, FILE_SEPARATOR) + ".class");
}

jar_class_factory::~jar_class_factory ()
{
  delete archive;
}



#ifdef FIXME_USE_SOLIB

solib_class_factory::solib_class_factory (const std::string &f)
  : class_factory (),
    file (f)
{
  // fixme open the .so file here
}

class_instance_creator *
solib_class_factory::find_source_file (const std::string &name)
{
  return NULL;
}

class_instance_creator *
solib_class_factory::find_source_file (const std::list<std::string> &name)
{
  return NULL;
}

class_instance_creator *
solib_class_factory::find_derived_file (const std::list<std::string> &name)
{
  assert (! is_source_only());
  return NULL;
}

#endif // FIXME_USE_SOLIB



classpath_class_factory::~classpath_class_factory ()
{
  for (std::list<class_factory *>::const_iterator i = factories.begin ();
       i != factories.end ();
       ++i)
    delete *i;
}

void
classpath_class_factory::load_class (const std::list<std::string> &qualname)
{
  // Search the class path for .java files and derived files
  // representing the same class.
  class_instance_creator *java_source = NULL;
  class_instance_creator *class_source = NULL;
  for (std::list<class_factory *>::const_iterator i = factories.begin ();
       (! java_source || ! class_source) && i != factories.end ();
       ++i)
    {
      class_factory *factory = *i;
      if (! java_source)
	java_source = factory->find_source_file (qualname);
      if (! class_source && ! factory->is_source_only())
	class_source = factory->find_derived_file (qualname);
    }

  // Pick the source we plan to use.
  class_instance_creator *source;
  bool emit_code = false;
  if (java_source && class_source)
    {
      if (java_source->get_mtime () > class_source->get_mtime ())
	{
	  // FIXME: tell the user?  gcj can.
	  delete class_source;
	  source = java_source;
	  emit_code = true;
	}
      else
	{
	  delete java_source;
	  source = class_source;
	}
    }
  else if (java_source)
    {
      source = java_source;
      emit_code = true;
    }
  else if (class_source)
    source = class_source;
  else
    return;

  source->apply (emit_code);
  delete source;
}

bool
classpath_class_factory::load_source_file (const std::string &name)
{
  if (jar_file_p (name))
    {
      // Read the entire jar file, parsing each .class or .java file,
      // and handing other files to the compiler as resources if
      // desired.
      jar_class_factory fac (name);
      fac.read_all ();
      // We always return true since the factory itself would have
      // done something on error.  FIXME: not the best API here...
      return true;
    }

  bool is_derived = class_file_p (name);
  class_instance_creator *source = NULL;
  for (std::list<class_factory *>::const_iterator i = factories.begin ();
       ! source && i != factories.end ();
       ++i)
    {
      // FIXME: the API is a bit wrong here, maybe.
      // Instead we could return byte sources and wrap them
      // appropriately here.
      // But... would this properly handle the .so case?
      if (is_derived)
	source = (*i)->find_derived_file (name);
      else
	source = (*i)->find_source_file (name);
    }

  if (! source)
    return false;

  source->apply (true);
  delete source;

  return true;
}
