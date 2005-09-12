// A single compilation, perhaps of multiple files.

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

#ifndef GCJX_COMPILER_HH
#define GCJX_COMPILER_HH

class classpath_class_factory;
class code_generator;
class reader;

/// A compiler flag is just a simple self-initializing bool wrapper
/// that also exposes a functional form, so we can more safely use it
/// as a public member of the compiler.
class compiler_flag
{
  /// The wrapped value.
  bool flag;

public:

  compiler_flag ()
    : flag (true)
  {
  }

  compiler_flag (bool v)
    : flag (v)
  {
  }

  bool operator() () const
  {
    return flag;
  }

  compiler_flag &operator= (bool nv)
  {
    flag = nv;
    return *this;
  }
};

// A "compiler" is used to compile all the classes in a single
// invocation.  It has its own class path, keeps track of types and
// packages, and other semi-global state.  It is conceivable to have
// multiple active compilers, each with their own universe of types.
// (Hence the only truly global types are primitives.)
class compiler : public warning_scope
{
  // Types of jobs that we add to the work list.
  typedef enum
  {
    PARSE,
    ANALYZE,
    GENERATE_CODE,
    PAUSE,
    DIE
  } job_type;

  // The state of the compiler object.  This is used to enforce
  // requirements of the compiler API, namely that some operations
  // must occur in a specific order.
  typedef enum
  {
    SETTING_OPTIONS,
    PARSING_FILES,
    ANALYZING_CLASSES,
    GENERATING_CODE
  } compiler_state;

  // A single job.  Note that jobs and the work list are only used
  // when multi-threaded; otherwise all actions are taken immediately.
  struct work_item
  {
    // What to do.
    job_type kind;
    // Name of file, for parsing.
    std::string filename;
    // Compilation unit for semantic analysis and code generation.
    model_unit *unit;

    work_item ()
      : kind (PARSE),
	unit (NULL)
    {
    }

    work_item (const work_item &other)
      : kind (other.kind),
	filename (other.filename),
	unit (other.unit)
    {
    }

    work_item (const std::string &n)
      : kind (PARSE),
	filename (n),
	unit (NULL)
    {
    }

    work_item (job_type k, model_unit *u = NULL)
      : kind (k),
	unit (u)
    {
    }

    work_item &operator= (const work_item &other)
    {
      kind = other.kind;
      filename = other.filename;
      unit = other.unit;
      return *this;
    }
  };


  // The monitor is held when modifying the work list.  The condition
  // is signalled when the work list has a new item.
  concurrence::exclusive_mutex work_monitor;
  concurrence::exclusive_condition work_condition;

  // Number of worker threads that have signed up.
  int multi_threaded;

  // Monitor and condition that are held when modifying multi_threaded
  // and other fields.
  concurrence::exclusive_mutex mt_monitor;
  concurrence::exclusive_condition mt_condition;

  // Work items.
  std::deque<work_item> work_list;

  // This monitor and condition are acquired when waiting for worker
  // threads to pause.
  concurrence::exclusive_mutex pause_monitor;
  concurrence::exclusive_condition pause_condition;

  // This is held when waiting for worker threads to pause.
  concurrence::exclusive_mutex pause_waiter;

  // The class path we search.
  classpath_class_factory *factory;

  // The root of all packages.  This is a special package, not the
  // same as the unnamed package.
  ref_package primordial_package;

  // The compiler has a single unnamed package.
  ref_unnamed_package unnamed_package;

  // All the compilation units we've read during compilation.  We keep
  // a list of these to ensure they aren't prematurely deleted.
  std::list<ref_unit> units;

  // This is a list of all compilation units for which code
  // generation has been requested.
  std::list<ref_unit> code_generation_units;

  // True if we plan to generate bytecode, false otherwise.  This is
  // used to decide whether compilation units corresponding to
  // out-of-date java sources should be pushed on the code generation
  // list.  FIXME: should be on a "-dependency" option or so.
  bool generating_bytecode;

  // True if everything is going ok.  False if we've encountered an
  // error.
  bool ok;

  // The name of the output directory.
  std::string output_dir;

  // An interface must inherit abstract "clones" of some public
  // functions of Object.  There is no good place to keep these clones
  // -- classes don't own inherited methods, and putting them in
  // Object itself would mean bloating every class for a single
  // special case.  So, we store them here.
  std::list<ref_method> object_interface_methods;

  // Directory cache that is shared between code generators.
  directory_cache dir_cache;

  // List of back ends we're using.  Each is owned by this class and
  // is deleted on exit.
  std::list<code_generator *> back_ends;

  // Encoding we're using when reading source files.
  std::string encoding;

  // Width of a tab character.
  int tab_width;

  // What we call ourselves in error messages.
  std::string name;

  // Our current state.
  compiler_state state;

  // True if this compiler can accept '.class' files.
  bool can_accept_classes;

  // True if method bodies from .class files should be kept.  This is
  // only meaningful when CAN_ACCEPT_CLASSES is true.
  bool need_class_method_bodies;

  // True if this compiler will accept resource files.
  bool can_accept_resources;

  // All the files read during this compilation.
  std::set<std::string> all_files_read;

  void do_load_source_file (const std::string &);
  void do_generate_code (model_unit *);
  void do_analyze_unit (model_unit *);
  void add_job (const work_item &);
  bool dispatch_job (const work_item &);

public:

  compiler (const std::string &name);

  ~compiler ();

  // The name passed to the constructor, for use in error messages.
  std::string get_name ();

  // These can be public as they require function notation to
  // meaningfully access their values.  We have one such cache object
  // for each class referred to by the compiler itself.
  class_cache java_lang_Object;
  class_cache java_io_Serializable;
  class_cache java_lang_Cloneable;
  class_cache java_lang_String;
  class_cache java_lang_Throwable;
  class_cache java_lang_Error;
  class_cache java_lang_RuntimeException;
  class_cache java_lang_Iterable;
  class_cache java_lang_Boolean;
  class_cache java_lang_Byte;
  class_cache java_lang_Short;
  class_cache java_lang_Character;
  class_cache java_lang_Integer;
  class_cache java_lang_Long;
  class_cache java_lang_Float;
  class_cache java_lang_Double;
  class_cache java_lang_Void;
  class_cache java_lang_AssertionError;
  class_cache java_lang_StringBuffer;
  class_cache java_lang_StringBuilder;
  class_cache java_lang_NullPointerException;
  class_cache java_lang_Class;
  class_cache java_lang_Enum;
  class_cache java_util_List;
  class_cache java_util_Iterator;
  class_cache gnu_gcj_runtime_StringBuffer;
  class_cache java_io_ObjectStreamField;
  class_cache java_lang_Exception;
  class_cache java_lang_ClassNotFoundException;
  class_cache java_lang_NoClassDefFoundError;
  class_cache java_lang_Deprecated;
  class_cache java_lang_SuppressWarnings;
  class_cache java_lang_Override;
  class_cache java_lang_annotation_Annotation;
  class_cache java_lang_annotation_Inherited;
  class_cache java_lang_annotation_Retention;
  class_cache java_lang_annotation_Target;
  class_cache java_lang_annotation_RetentionPolicy;
  class_cache java_lang_annotation_ElementType;

  // Flags for this compilation.  These can be public since they
  // expose a functional notation only.
  compiler_flag verbose;
  compiler_flag pedantic;
  compiler_flag warnings_are_errors;
  compiler_flag dump_methods;

  // These control whether a certain feature is recognized in the
  // source.
  compiler_flag feature_assert;
  compiler_flag feature_enum;
  compiler_flag feature_static_import;
  compiler_flag feature_enhanced_for;
  compiler_flag feature_generics;
  compiler_flag feature_boxing_conversion;
  compiler_flag feature_varargs;
  compiler_flag feature_annotations;

  // These control aspects of code generation.
  compiler_flag target_assert;
  compiler_flag target_debug;

  // Flags used only when compiling to bytecode.
  compiler_flag target_15;
  compiler_flag target_14;
  compiler_flag target_verify;

  void set_class_factory (classpath_class_factory *fac);

  void add_unit (const ref_unit &unit, bool emit_code = false);

  // Return the primordial package.
  model_package *get_primordial_package () const
  {
    return primordial_package.get ();
  }

  // Find a class or package given a fully-qualified name.
  Iname *find_name (const std::list<std::string> &name);

  // Find a class given a fully-qualified class name like
  // "java.lang.String", split into elements at "."
  model_class *find_class (const std::list<std::string> &name);

  // Find a class given a fully-qualified class name like
  // "java.lang.String".  In most situations it is preferable to add a
  // new class cache field.  This method will eventually be removed.
  model_class *find_class (const std::string &name);

  // Return the unnamed package for this compiler.
  model_unnamed_package *get_unnamed_package ()
  {
    return unnamed_package.get ();
  }

  /// Set the output directory.
  void set_output_directory (const std::string &d)
  {
    output_dir = d;

    // Set the directory on the shared cache.
    dir_cache.set_directory (output_dir);
  }

  /// Get the output directory.
  std::string get_output_directory () const
  {
    return output_dir;
  }

  /// Find a java source file on the class path, parse it, and
  /// schedule it for eventual semantic analysis and code generation.
  /// Throws an exception if the file could not be found or parsed for
  /// some reason.
  void load_source_file (const std::string &);

  /// Perform semantic analysis on all compilation units that are
  /// noted for code generation.
  /// @return true on success, false on failure
  bool semantic_analysis ();

  /// Generate code.
  /// @return true on success, false on failure
  bool generate_code ();

  /// This is called when an error message is emitted to indicate that
  /// this compilation has failed.
  void set_failed ()
  {
    ok = false;
  }

  /// Check the status of this compilation.
  bool failed_p () const
  {
    return ! ok;
  }


  /// This should only be called when resolving Object.
  void set_object_interface_methods (const std::list<ref_method> &l)
  {
    assert (! l.empty ());
    assert (object_interface_methods.empty ());
    object_interface_methods = l;
  }

  std::list<ref_method> get_object_interface_methods () const
  {
    assert (! object_interface_methods.empty ());
    return object_interface_methods;
  }


  /// These helper methods set the state of various compiler flags as
  /// a group.
  void set_source_1_5 ();
  void set_source_1_4 ();
  void set_source_1_3 ();
  void set_target_1_5 ();
  void set_target_1_4 ();
  void set_target_1_3 ();

  /// Return the directory cache used by this compiler, used when
  /// building code generators.
  directory_cache &get_directory_cache ()
  {
    return dir_cache;
  }

  /// Add a back end to the list of back ends for code generation.
  /// The object must be allocated with 'new' and will be deleted when
  /// this compiler is destroyed.
  void add_code_generator (code_generator *);

  /// Set the encoding being used for compilations.  If not set, the
  /// default is UTF-8.
  void set_encoding (const std::string &enc)
  {
    encoding = enc;
  }

  /// Return the encoding in use by this compiler.
  std::string get_encoding () const
  {
    return encoding;
  }

  /// Set the tab width for this compilation.  This is used when
  /// generating error messages.  This must be set before any parsing
  /// is done.
  void set_tab_width (int x)
  {
    tab_width = x;
  }

  /// Get the tab width used by this compilation.
  int get_tab_width () const
  {
    return tab_width;
  }

  /// Enable all the "reasonable" warnings.  The precise warnings
  /// enabled are determined by the maintainers and may change from
  /// release to release.  This sets all the related flags to the
  /// given warning state.
  void set_wall (warning_state);

  /// Register the current thread as a worker thread for the compiler.
  void work ();

  /// FIXME: this probably shouldn't be public...
  void pause_workers ();

  /// Return true if a class being read should preserve its method
  /// body.
  bool keep_class_method_body_p () const
  {
    return need_class_method_bodies;
  }

  /// Return true if this compilation can handle a resource file.
  bool handles_resources_p () const
  {
    return can_accept_resources;
  }

  /// Compile a resource file given its file name and its contents.
  void compile_resource (const std::string &, reader *);

  /// Indicate that a given file has been read during the course of
  /// this compilation.
  void note_file_read (const std::string &fn)
  {
    all_files_read.insert (fn);
  }

  /// Return a set of all files read.
  std::set<std::string> get_all_files_read ()
  {
    return all_files_read;
  }
};

#endif // GCJX_COMPILER_HH
