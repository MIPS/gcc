// ABI interface.

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

#ifndef GCC_JAVA_ABI_HH
#define GCC_JAVA_ABI_HH

class tree_builtins;

// These are the possible values for the 'state' field of the class
// structure.  Note that ordering is important here.  This code is
// shared with libgcj.
enum
{
  JV_STATE_NOTHING = 0,		// Set by compiler.

  JV_STATE_PRELOADING = 1,	// Can do _Jv_FindClass.
  JV_STATE_LOADING = 3,		// Has super installed.
  JV_STATE_READ = 4,		// Has been completely defined.
  JV_STATE_LOADED = 5,		// Has Miranda methods defined.

  JV_STATE_COMPILED = 6,	// This was a compiled class.

  JV_STATE_PREPARED = 7,	// Layout & static init done.
  JV_STATE_LINKED = 9,		// Strings interned.

  JV_STATE_IN_PROGRESS = 10,	// <clinit> running.

  JV_STATE_ERROR = 12,

  JV_STATE_DONE = 14		// Must be last.
};

#define _Jv_FIELD_UNRESOLVED_FLAG	0x8000

/// This class handles code generation for certain things.  This is
/// done on an ad hoc basis and lets us support multiple ABIs more
/// easily.  All the work is done by virtual functions which are
/// defined in subclasses.
class gcj_abi
{
public:

  gcj_abi ();

  virtual ~gcj_abi ();

  /// Create and return an expression that will call the indicated
  /// method.  Note that we explicitly do not handle a non-static
  /// reference to a static method.  That must be handled by the
  /// caller.
  ///
  /// @param builtins The mapper class
  /// @param current  The class in which this code occurs
  /// @param obj The receiver object for the call; for static methods
  ///            this must be NULL_TREE.
  /// @param args The argument list.
  /// @param meth The method to call.
  virtual tree build_method_call (tree_builtins *builtins,
				  aot_class *current,
				  tree obj, tree args, model_method *meth,
				  bool is_super) = 0;

  virtual tree build_method_call (tree_builtins *builtins,
				  aot_class *current,
				  tree obj, tree args,
				  const std::string &class_name,
				  const std::string &method_name,
				  const std::string &descriptor) = 0;

  /// Note that we explicitly do not handle a non-static reference to
  /// a static field.  That must be handled by the caller.
  virtual tree build_field_reference (tree_builtins *builtins,
				      aot_class *current,
				      tree obj, model_field *field) = 0;

  virtual tree build_field_reference (tree_builtins *builtins,
				      aot_class *current,
				      tree obj,
				      const std::string &classname,
				      const std::string &fieldname,
				      const std::string &descriptor) = 0;

  /// Return a tree representing a reference to some other class.
  virtual tree build_class_reference (tree_builtins *builtins,
				      aot_class *current,
				      const std::string &class_name) = 0;

  /// Return a tree representing a reference to some other class.
  virtual tree build_class_reference (tree_builtins *builtins,
				      aot_class *current,
				      model_type *other) = 0;

  /// Return a tree representing a reference to another class which is
  /// suitable for use in the Class object itself.  This is similar to
  /// build_class_reference() but is only used in Class and must be
  /// suitable for static emission in a CONSTRUCTOR node.
  virtual tree build_direct_class_reference (tree_builtins *builtins,
					     aot_class *current,
					     model_type *other) = 0;

  /// Return an expression that is used to create a new object given
  /// its type, constructor, and arguments to the constructor.
  virtual tree build_new (tree_builtins *builtins,
			  aot_class *current,
			  model_class *klass,
			  model_method *constructor, tree args) = 0;

  /// Return an expression representing the size of the class in
  /// bytes, or -1 if it can't be known until runtime.
  virtual tree get_size_in_bytes (tree) = 0;

  /// Return the initial 'state' value for a class compiled with this
  /// ABI.
  virtual int get_class_state () = 0;

  /// Return a pointer to the vtable for the indicated class.  If the
  /// bool argument is true, we are creating the Class object for
  /// KLASS and need a fully-filled-in vtable, not just a decl.
  virtual tree get_vtable (tree_builtins *builtins, model_class *klass) = 0;

  /// Return tree representing index into vtable where this method can
  /// be found.  Should return -1 for a static method or constructor.
  virtual tree get_vtable_index (aot_class *klass, model_method *method) = 0;

  /// Return the initializer representing a 'catch' type.
  virtual tree get_catch_initializer (tree_builtins *builtins,
				      model_class *klass) = 0;
};

/// This class handles C++ ABI code.
class cxx_abi : public gcj_abi
{
public:

  cxx_abi ()
    : gcj_abi ()
  {
  }

  tree build_method_call (tree_builtins *, aot_class *,
			  tree, tree, model_method *, bool);

  tree build_method_call (tree_builtins *builtins,
			  aot_class *current,
			  tree obj, tree args,
			  const std::string &class_name,
			  const std::string &method_name,
			  const std::string &descriptor);

  tree build_field_reference (tree_builtins *, aot_class *,
			      tree, model_field *);

  tree build_field_reference (tree_builtins *builtins,
			      aot_class *current,
			      tree obj,
			      const std::string &classname,
			      const std::string &fieldname,
			      const std::string &descriptor);

  tree build_class_reference (tree_builtins *, aot_class *,
			      const std::string &);

  tree build_class_reference (tree_builtins *builtins,
			      aot_class *current,
			      model_type *other);

  tree build_direct_class_reference (tree_builtins *builtins,
				     aot_class *current,
				     model_type *other)
  {
    return build_class_reference (builtins, current, other);
  }

  tree build_new (tree_builtins *, aot_class *, model_class *,
		  model_method *, tree);

  tree get_size_in_bytes (tree klass)
  {
    return size_in_bytes (klass);
  }

  int get_class_state ()
  {
    return JV_STATE_COMPILED;
  }

  tree get_vtable (tree_builtins *, model_class *);

  tree get_vtable_index (aot_class *klass, model_method *method);

  tree get_catch_initializer (tree_builtins *builtins,
			      model_class *klass);
};

/// This class handles the binary compatibility ABI.
class bc_abi : public gcj_abi
{
public:

  bc_abi ()
    : gcj_abi ()
  {
  }

  tree build_method_call (tree_builtins *, aot_class *,
			  tree, tree, model_method *, bool);

  tree build_method_call (tree_builtins *builtins,
			  aot_class *current,
			  tree obj, tree args,
			  const std::string &class_name,
			  const std::string &method_name,
			  const std::string &descriptor);

  tree build_field_reference (tree_builtins *, aot_class *,
			      tree, model_field *);

  tree build_field_reference (tree_builtins *builtins,
			      aot_class *current,
			      tree obj,
			      const std::string &classname,
			      const std::string &fieldname,
			      const std::string &descriptor);

  tree build_class_reference (tree_builtins *, aot_class *,
			      const std::string &);

  tree build_class_reference (tree_builtins *builtins,
			      aot_class *current,
			      model_type *other);

  tree build_direct_class_reference (tree_builtins *builtins,
				     aot_class *current,
				     model_type *other);

  tree build_new (tree_builtins *, aot_class *, model_class *,
		  model_method *, tree);

  tree get_size_in_bytes (tree klass)
  {
    return integer_minus_one_node;
  }

  int get_class_state ()
  {
    return JV_STATE_PRELOADING;
  }

  tree get_vtable (tree_builtins *, model_class *)
  {
    // The BC ABI lays out all vtables at runtime.
    return null_pointer_node;
  }

  tree get_vtable_index (aot_class *, model_method *)
  {
    return integer_minus_one_node;
  }

  tree get_catch_initializer (tree_builtins *builtins, model_class *klass);
};

#endif // GCC_JAVA_ABI_HH
