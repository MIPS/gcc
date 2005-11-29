// Represent the class statement.

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

#ifndef GCJX_MODEL_CLASS_HH
#define GCJX_MODEL_CLASS_HH

typedef std::multimap<std::string, ref_method> AllMethods;

/// A wrapper iterator that hides the fact that we store all_methods as
/// a multimap.  This means calling code doesn't have to access it->second,
/// making it possible to use these iterators where you're expecting a trivial
/// iterator.
class AllMethodsIterator
{
public:
  AllMethodsIterator (const AllMethods::const_iterator &it)
    : it (it)
  {
  }

  const ref_method &operator* () const
  {
    return it->second;
  }

  AllMethodsIterator &operator++ ()
  {
    ++it;
    return *this;
  }

  friend bool operator!= (const AllMethodsIterator& lhs,
			  const AllMethodsIterator& rhs)
  {
    return (lhs.it != rhs.it);
  }

private:
  AllMethods::const_iterator it;
};

/// This represents a class declaration.  It has attributes covering
/// all the needed functionality (fields, methods, etc).  It also
/// knows the inheritance rules and other things like that.
///
/// A model_class is also used as the type of a class.  In particular,
/// for ordinary classes, the model_class is both the declaration and
/// the type.  Parameterizations of the class -- including the
/// parameterization where each type variable "maps to itself" -- are
/// represented by model_class_instance.
class model_class : public model_type, public Iname, public IContext,
		    public IDeprecatable, public IAnnotatable,
		    public IModifiable, public IMember, public IScope
{
protected:

  // States.
  enum resolution_state_value
  {
    NONE,
    DOING_SUPERCLASS,
    PRE_MEMBERS,
    DOING_MEMBERS,
    POST_MEMBERS,
    DONE
  };

  enum resolution_state_value resolution_state;

  // Class name.
  std::string name;

  // Assigned name for anonymous or local classes.  This is the same
  // as `name' for other classes.
  mutable std::string assigned_name;

  // Superclass, or NULL if this class is java.lang.Object.
  ref_forwarding_type superclass;

  // Type parameters.
  model_parameters type_parameters;

  // If this is a generic class, the corresponding raw class.
  // Otherwise, NULL.
  owner<model_class> raw_class;

  // All directly implemented interfaces.
  std::list<ref_forwarding_type> interfaces;

  // Methods in this class.
  std::list<ref_method> methods;

  // All methods in this class, including inherited methods.
  AllMethods all_methods;
  // Like all_methods, but sorted properly for vtable generation
  // (i.e., source order and inheritance order).
  // FIXME this should not be needed.
  std::list<model_method *> source_order_methods;

  // Member classes declared here.
  std::map< std::string, owner<model_class> > member_classes;
  // Anonymous member classes aren't put in the map.
  std::list< owner<model_class> > anon_classes;
  // All named member classes, including inherited ones.
  std::multimap<std::string, model_class *> all_member_classes;

  // Instance initializers.
  std::list<ref_stmt> instance_inits;

  // Static initializers
  std::list<ref_stmt> static_inits;

  // Fields in this class.
  std::list<ref_field> fields;

  // This maps names onto fields.  This includes fields inherited
  // from superclasses and superinterfaces.
  std::multimap<std::string, model_field *> field_map;
  // This is like field_map, but fields with the same named are
  // mapped onto an ambiguous field object.
  std::map<std::string, model_field_base *> ambiguous_field_map;


  // True for interfaces.
  bool interface;

  // True for anonymous classes.
  bool anonymous;

  // True for local classes (classes local to a method).
  bool local;

  // True if this class is defined in a static context.
  bool static_context;

  // Our compilation unit.
  model_unit *compilation_unit;

  // A counter used when generating anonymous and local class names.
  int counter;

  // If a local or anonymous class refers to a final variable in its
  // containing method, we will need to capture these variables in the
  // synthetic constructor's argument list.  This maps such variables
  // onto the corresponding new fields.
  std::map<model_variable_decl *, ref_field> synthetic_fields;

  // This is a plain list of all the variables we have captured.  We
  // maintain this separate from 'synthetic_fields' to ensure a fixed
  // ordering of the variables.  FIXME.
  std::list<model_variable_decl *> captured_variables;

  // The field 'this$0'.
  ref_field this_0;

  // The field '$assertionsDisabled'.
  ref_field dollar_assertionsDisabled;

  // True if this class has been referenced.
  bool used;

  // The 'finit$' method, if one is needed.
  ref_method finit_;

  // The 'class$' method, if one is needed.
  ref_method class_;

  // All the instances of this class.
  model_instance_cache<model_class_instance> instance_cache;

  // This holds any accessor methods we had to create.  This maps the
  // referenced member to the accessor.
  std::map<IMember *, ref_method> accessors;
  // Like 'accessors', but holds settor methods for fields.
  std::map<IMember *, ref_method> settors;

  void compute_descriptor ();
  std::string generate_name ();
  std::string generate_synthetic_field_name (const std::string &prefix);
  std::string generate_synthetic_method_name (const std::string &prefix,
                                              ref_method &m);
  std::string get_signature_fragment ();

  // Get the part of the signature that represents the actual (not
  // formal) type parameters.  This is only implemented by subclasses,
  // the default returns the empty string.
  virtual std::string get_signature_map_fragment ()
  {
    return "";
  }

  std::string get_assigned_name () const;

  // Add an implicit constructor if one wasn't found.
  virtual ref_method add_implicit_constructor ();
  // Add 'finit$' if it is required.
  void create_finit_method ();
  // Add '<clinit>' if it is required.  Return true if it was created.
  bool create_clinit_method ();

  // Add arguments to a constructor to handle captured variables.
  // Also update the constructor's body to initialize the captured
  // fields.
  void add_captured_constructor_arguments (resolution_scope *,
					   const ref_method &constructor,
					   bool = true);

  // Add an initializer to a list of statements.
  void add (std::list<ref_stmt> &, const owner<model_field> &);

  void add_method (const ref_method &);
  void massage_modifiers (const ref_modifier_list &mods);
  void compute_super_types (std::list<model_class *> &);
  void check_interface_instances (model_class *,
				  std::map<model_class *, model_class *> &);
  bool method_inheritable_p (model_method *, model_class *);
  template <typename IteratorT>
  void inherit_methods (const IteratorT &begin, const IteratorT& end,
                        model_class *, bool);
  void inherit_methods (model_class *, bool = false);
  void inherit_fields (model_class *, bool = false);
  void inherit_types (model_class *, bool = false);
  void inherit_package_private_members ();
  void resolve_members (resolution_scope *);
  void compute_object_interface_methods ();
  void check_members ();
  void do_resolve_classes (resolution_scope *);
  void resolve_one_method (const ref_method &);
  void check_uninherited_abstract_methods ();
  void push_on_scope_for_class (resolution_scope *);
  void check_serialization_fields ();

  /// Check whether this method was referenced and emit a warning if
  /// not.
  void check_referenced (resolution_scope *);

  // Check to see if a list of statements has any non-trivial
  // initializers.
  bool check_init_list (const std::list<ref_stmt> &);

  // An ad hoc method for subclasses to override.  Called during
  // class ("envelope") resolution.
  virtual void resolve_classes_hook (resolution_scope *)
  {
  }

  // Another ad hoc hook.  FIXME.
  virtual void resolve_member_hook (resolution_scope *)
  {
  }

  virtual void ensure_classes_inherited (resolution_scope *);

  // Helper method used when creating a parameterized instance.
  void set_this_0 (const ref_field &t0)
  {
    this_0 = t0;
  }

  annotation_kind get_annotation_kind () const
  {
    return ANNOTATE_TYPE;
  }

  // FIXME: this is ugly.
  friend class model_class_instance;
  friend class model_raw_class;

public:

  model_class (const location &w)
    : model_type (w),
      IModifiable (),
      resolution_state (NONE),
      interface (false),
      anonymous (false),
      local (false),
      static_context (false),
      compilation_unit (NULL),
      counter (0),
      used (false)
  {
  }

  void set_type_parameters (const std::list<ref_type_variable> &tps)
  {
    type_parameters.set_type_parameters (tps);
  }

  void set_type_parameters (const model_parameters &tps)
  {
    type_parameters = tps;
  }

  const model_parameters &get_type_parameters () const
  {
    return type_parameters;
  }

  void set_name (const std::string &n)
  {
    name = n;
  }

  std::string get_name () const
  {
    return name;
  }

  /// Like get_name() but returns a constructed name if the class is
  /// inner or anonymous.
  std::string get_output_name () const;

  /// Returns this class' name in the form "java.lang.Object".
  /// Member classes are separated using '$'.
  std::string get_fully_qualified_name ();

  /// Like get_fully_qualified_name(), but returns this class' name in
  /// the form "java/lang/Object".
  std::string get_fully_qualified_name_with_slashes ();

  /// Like get_fully_qualified_name(), but member classes are
  /// separated using '.'.
  std::string get_canonical_name ();

  void implicit_modifier (modifier_t);

  // Ambiguity prevention.
  bool final_p () const
  {
    return IModifiable::final_p ();
  }

  void set_anonymous ()
  {
    anonymous = true;
    generate_name ();
  }

  bool anonymous_p () const
  {
    return anonymous;
  }

  void set_local ()
  {
    local = true;
  }

  bool local_p () const
  {
    return local;
  }

  void set_superclass (const ref_forwarding_type &super)
  {
    superclass = super;
  }

  model_class *get_superclass () const
  {
    // This class might be Object.
    if (! superclass)
      return NULL;
    return assert_cast<model_class *> (superclass->type ());
  }

  void set_compilation_unit (model_unit *cu)
  {
    compilation_unit = cu;
  }

  model_unit *get_compilation_unit () const
  {
    return compilation_unit;
  }

  model_package *get_package () const;

  void set_interface ()
  {
    interface = true;
  }

  bool interface_p () const
  {
    return interface;
  }

  void set_implements (const std::list<ref_forwarding_type> &ext)
  {
    interfaces = ext;
  }

  std::list<ref_forwarding_type> &get_interfaces ()
  {
    return interfaces;
  }

  void add (const ref_method &m);

  void add (const ref_field &f);

  void add (const owner<model_class> &k);

  void add_static_initializer (const ref_block &b);

  void add_instance_initializer (const ref_block &b);

  std::list<ref_method> get_methods () const
  {
    return methods;
  }

  bool has_method_with_descriptor_p (const std::string &name,
                                     const std::string &descriptor) const;

  AllMethodsIterator begin_all_methods () const
  {
    return all_methods.begin ();
  }

  AllMethodsIterator end_all_methods () const
  {
    return all_methods.end ();
  }

  bool has_any_conflicting_interface_method_p (model_class *other) const;

  std::list<ref_field> get_fields () const
  {
    return fields;
  }

  std::list<model_field *> get_all_fields () const;

  bool reference_p () const
  {
    return true;
  }

  bool primitive_p () const
  {
    return false;
  }

  // Return true if this is a member class.
  bool member_p () const
  {
    return declaring_class != NULL;
  }

  // Return true if this is an inner class.
  bool inner_p () const
  {
    return declaring_class != NULL && ! static_p ();
  }

  // Return true if this class is a parameterized specialization of a
  // generic class.
  virtual bool parameterized_p () const
  {
    return false;
  }

  // Return true if this class is a type variable.
  virtual bool type_variable_p () const
  {
    return false;
  }

  // Return true if this class is a wildcard class.
  virtual bool wildcard_p () const
  {
    return false;
  }

  // Return true if this class is a raw type.
  virtual bool raw_p () const
  {
    return false;
  }

  // Return true if this class was defined in a static context.
  bool static_context_p () const
  {
    return static_context;
  }

  // Indicate that this class was defined in a static context.
  void set_static_context ()
  {
    assert (local || anonymous);
    static_context = true;
  }

  // For a member or local class, return the next outermost lexically
  // enclosing class.  For a top-level class, return NULL.
  model_class *get_lexically_enclosing_class () const
  {
    return declaring_class;
  }

  // Return the enclosing top-level class.  If this class is a
  // top-level class, return 'this'.
  model_class *get_top_level_class () const
  {
    model_class *result = (model_class *) this;
    while (result->declaring_class)
      result = result->declaring_class;
    return result;
  }

  /// Return true if MEMBER is defined within the body of this class.
  bool has_as_member_of_p (model_class *member);

  bool assignable_from_p (model_type *other);

  /// This returns true if this type contains (in the type argument
  /// sense) the type OTHER.
  virtual bool contains_p (model_class *other);

  std::string to_string (const jvalue &) const
  {
    abort ();
  }

  bool package_p () const
  {
    return false;
  }

  Iname *find_name (const std::string &);

  // This resolves the superclass, implemented interfaces and similar
  // things.  It does not resolve method bodies, field types, or the
  // like.
  void resolve_classes ();

  // Like the above, but valid only for local and anonymous classes.
  virtual void resolve_classes (resolution_scope *);

  // Resolve classes and members.  This means the types of the members
  // are found, and inheritance is processed.  Resolution of method
  // bodies is not done.
  void resolve_members ();

  virtual void resolve (resolution_scope *);

  void find_members (const std::string &memname,
		     std::set<model_class *> &result,
		     IContext *request);

  void find_members (const std::string &memname,
		     std::set<model_field *> &result,
		     IContext *request,
		     model_class *qualifier);

  void find_members (const std::string &memname,
		     std::set<model_method *> &result,
		     IContext *request,
		     model_class *qualifier);

  virtual std::string get_pretty_name () const;

  /// Add the class members needed for 'assert' support.  This will
  /// add a new '$assertionsDisabled' field, and will also add the
  /// static constructor code used to initialize it.  The field is
  /// returned.
  model_field *add_assert_members ();

  /// Add the constructor to an anonymous class.  Calling this on a
  /// non-anonymous class, or calling this more than once, is invalid.
  /// The first argument, if not null, is the qualifier for the
  /// 'super' call.
  void add_anonymous_constructor (resolution_scope *,
				  const ref_expression &,
				  const std::list<ref_expression> &);

  /// If this class captures any local variables, then we need to
  /// update the arguments to any constructor invocation to add the
  /// missing arguments.  This method will update the list of
  /// expressions to include any required local variable references.
  /// The list of expressions is assumed to already have been
  /// resolved, and the new additions are resolved before adding.
  void add_capturing_arguments (resolution_scope *,
				std::list<ref_expression> &);

  /// If this class is deprecated, and deprecation warnings are
  /// enabled, issue a warning message.  The argument is the element
  /// requesting the check; any warning is issued against this
  /// element.
  void check_deprecated (model_element *);

  /// Indicate that this field has been referenced.
  void set_used ()
  {
    used = true;
  }

  /// Apply the type map to this generic type and return a
  /// parameterized instance.  Multiple calls to this with the
  /// equivalent type map will all return the same result.
  virtual model_class *apply_type_map (model_element *,
				       const model_type_map &);

  /// Create a parameterized instance of this class given a list of
  /// argument types.  This should only be called on a model_class,
  /// not on an instance of a subclass; to apply a type map
  /// composition, use apply_type_map.
  model_class_instance *create_instance (model_element *,
					 const std::list<model_class *> &);

  /// Ensure that this class lexically encloses class INNER.
  void ensure_enclosing (model_class *inner);

  /// Add all member and anonymous classes to the list.
  /// This is a bogus API...
  void add_classes_to_list (std::list< owner<model_class> > &the_list)
  {
    for (std::map< std::string, owner<model_class> >::const_iterator it
	   = member_classes.begin ();
	 it != member_classes.end ();
	 ++it)
      the_list.push_back ((*it).second);
    the_list.insert (the_list.end (), anon_classes.begin (),
		     anon_classes.end ());
  }

  /// Add a synthetic field that captures a local variable for an
  /// anonymous or local class.  Returns the new field.
  ref_field add_captured_variable (model_variable_decl *);

  /// Return the field 'this$0'.  If the field doesn't exist yet,
  /// create it.
  ref_field get_this_0 ();

  /// Return the 'finit$' method, if one exists for this class.  This
  /// will return NULL if there is no method.
  model_method *get_finit_method () const
  {
    return finit_.get ();
  }

  /// This is called after code generation to clean up this class.
  /// This involves removing data which is not needed any more.  This
  /// will not remove anything that is visible from outside this
  /// class.  So, e.g., a method body might be freed but the method
  /// itself will not.
  void clean_up ();

  /// Return the 'class$' method, which is used to implement the
  /// 'name.class' construct.
  model_method *add_class_members ();

  /// This is called by 'new' to check whether this class can be
  /// instantiated.  This method should throw an exception if this is
  /// not possible.  The argument is the requester and is used for
  /// error reporting.
  virtual void check_instantiation (model_element *);

  /// Ensure an accessor exists to reference the field.  The second
  /// argument determines whether the access is a reference or an
  /// assignment.  See trampoline_required_p().  This returns the
  /// accessor or settor method.
  model_method *get_accessor (model_field *, bool);

  /// Likewise, but for a method call.
  model_method *get_accessor (model_method *);

  void look_up_name (const std::string &name,
		     std::set<model_class *> &result,
		     IContext *request,
		     model_class *qualifier);

  void look_up_name (const std::string &name,
		     std::set<model_variable_decl *> &result,
		     IContext *request,
		     model_class *qualifier);

  void look_up_name (const std::string &name,
		     std::set<model_method *> &result,
		     IContext *request,
		     model_class *qualifier);

  /// Set up the resolution_scope so that this class is current.
  /// Handles pushing parents as needed.
  void push_on_scope (resolution_scope *);

  static_result is_static_scope () const
  {
    return static_context_p () ? STATIC_CONTEXT : NOT_STATIC_CONTEXT;
  }

  std::string get_signature ();

  /// This returns the "class signature".  This differs from the
  /// signature in that it is used to describe a class declaration,
  /// not a class type.  It incorporates different information: the
  /// formal type parameters and the signatures of the superclass and
  /// superinterfaces.
  std::string get_class_signature ();

  /// Return a list of variables captured by this class.
  std::list<model_variable_decl *> get_captured_variables () const
  {
    return captured_variables;
  }

  const std::list<model_method *> &get_sorted_methods () const
  {
    return source_order_methods;
  }

  model_type *erasure ();

  void visit (visitor *);
};

#endif // GCJX_MODEL_CLASS_HH
