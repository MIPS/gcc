/* ACLE support for AArch64 SVE
   Copyright (C) 2018 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#define IN_TARGET_CODE 1

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "langhooks.h"
#include "stringpool.h"
#include "rtl.h"
#include "tm_p.h"
#include "memmodel.h"
#include "insn-codes.h"
#include "optabs.h"
#include "expr.h"
#include "diagnostic.h"
#include "recog.h"
#include "basic-block.h"
#include "function.h"
#include "fold-const.h"
#include "gimple.h"
#include "tree-vector-builder.h"
#include "rtx-vector-builder.h"
#include "stor-layout.h"
#include "emit-rtl.h"

namespace aarch64_sve {

/* Enumerates the SVE predicate and (data) vector types, together called
   "vector types" for brevity.  */
enum vector_type {
#define DEF_SVE_TYPE(ACLE_NAME, NCHARS, ABI_NAME, SCALAR_TYPE) \
  VECTOR_TYPE_ ## ACLE_NAME,
#include "aarch64-sve-builtins.def"
  NUM_VECTOR_TYPES
};

/* Describes the various uses of a governing predicate.  */
enum predication {
  /* No governing predicate present.  */
  PRED_none,

  /* Merging predication: copy inactive lanes from the first data argument
     to the vector result.  */
  PRED_m,

  /* "Don't care" predication: set inactive lanes of the vector result
     to arbitrary values.  */
  PRED_x,

  /* "Zero" predication: set inactive lanes of the vector result to zero.  */
  PRED_z,

  NUM_PREDS
};

/* Classifies functions into "shapes".  The idea is to take all the
   signatures for an overloaded function, remove the governing predicate
   (if any), and classify what's left based on:

   - the number of arguments (which is always fixed for a given overload)

   - the process of determining the types in the signature from the type
     suffixes (including the types that are not affected by the suffixes)

   - which arguments must be integer constant expressions, and what range
     those arguments have

   - the process for mapping overloaded names to "full" names.

   In the comments below, _t0 represents the first type suffix and _t1
   represents the second.  Square brackets enclose characters that are
   present only in the full name, not the overloaded name.  Governing
   predicate arguments and predicate suffixes are not shown.  */
enum function_shape {
  /* sv<t0>_t svfoo[_t0]().  */
  SHAPE_inherent,

  /* sv<t0>_t svfoo[_n]_t0(<t0>_t).  */
  SHAPE_unary_n,

  /* sv<t0>_t svfoo_t0(<t0>_t, <t0>_t).  */
  SHAPE_binary_scalar,

  /* sv<t0>_t svfoo[_t0](sv<t0>_t, sv<t0>_t)
     sv<t0>_t svfoo[_n_t0](sv<t0>_t, <t0>_t).  */
  SHAPE_binary_opt_n,

  /* sv<t0>_t svfoo[_n_t0])(sv<t0>_t, uint64_t)

     The final argument must be an integer constant expression in the
     range [1, <t0>_BITS].  */
  SHAPE_shift_right_imm
};

/* Classifies an operation into "modes"; for example, to distinguish
   vector-scalar operations from vector-vector operations, or to
   distinguish between different addressing modes.

   The overloaded and full names of a function are determined by:
   - the base name ("svadd", etc.)
   - the mode
   - the type suffixes
   - the type of predication

   Thus the mode must fill in any information that isn't given by the
   other three.  */
enum function_mode {
  MODE_none,
  MODE_n
};

/* Enumerates the possible type suffixes.  Each suffix is associated with
   a vector type, but provides more information about element sizes in
   the case of predicates.  */
enum type_suffix {
#define DEF_SVE_TYPE_SUFFIX(NAME, ACLE_TYPE, BITS) \
  TYPE_SUFFIX_ ## NAME,
#include "aarch64-sve-builtins.def"
  NUM_TYPE_SUFFIXES
};

/* Combines two type suffixes.  */
typedef enum type_suffix type_suffix_pair[2];

/* Enumerates the function base names, such as "svadd".  */
enum function {
#define DEF_SVE_FUNCTION(NAME, SHAPE, TYPES, PRED) FUNC_ ## NAME,
#include "aarch64-sve-builtins.def"
  NUM_FUNCS
};

/* Static information about each single-predicate or single-vector
   ABI and ACLE type.  */
struct vector_type_info {
  /* The name of the type as declared by arm_sve.h.  */
  const char *acle_name;

  /* The name of the type specified in the AAPCS.  The type is always
     available under this name, even when arm_sve.h isn't included.  */
  const char *abi_name;

  /* The C++ mangling of ABI_NAME.  */
  const char *mangled_name;
};

/* Static information about a type suffix.  */
struct type_suffix_info {
  /* The suffix itself.  */
  const char *suffix;

  /* The associated ACLE vector or predicate type.  */
  vector_type type : 8;

  /* The number of bits and bytes in an element.  For predicates this
     measures the associated data elements.  */
  unsigned int elem_bits : 8;
  unsigned int elem_bytes : 8;

  /* True if the suffix is for an integer type.  */
  unsigned int integer_p : 1;
  /* True if the suffix is for an unsigned type.  */
  unsigned int unsigned_p : 1;
};

/* Static information about a set of functions.  */
struct function_group {
  /* The base name, as a string and an enum.  */
  const char *name;
  function func;

  /* The shape of the functions, as described above the enum definition.
     It's possible to have entries with the same base name but different
     shapes.  */
  function_shape shape;

  /* A list of the available predication types, and of the available
     type suffixes.  The function supports every permutation of the two.

     The list of predication types is terminated by NUM_PREDS while
     the list of type suffixes is terminated by two NUM_TYPE_SUFFIXES.
     The list of type suffixes is lexicographically ordered.  */
  const type_suffix_pair *types;
  const predication *preds;
};

/* Describes a fully-resolved function (i.e. one that has a unique full
   name).  */
struct GTY(()) function_instance {
  function_instance () {}
  function_instance (function, function_mode, const type_suffix_pair &,
		     predication);

  bool operator== (const function_instance &) const;
  bool operator!= (const function_instance &) const;
  hashval_t hash () const;

  tree scalar_type (unsigned int) const;
  tree vector_type (unsigned int) const;

  /* The explicit "enum"s are required for gengtype.  */
  enum function func;
  enum function_mode mode;
  type_suffix_pair types;
  enum predication pred;
};

/* Describes a function decl.  */
struct GTY(()) registered_function
{
  /* The ACLE function that the decl represents.  If OVERLOADED_P:

     - the mode for functions with an optional _n suffix is MODE_none
     - any type suffixes determined by resolving the overload are
       NUM_TYPE_SUFFIXES.  */
  function_instance instance;

  /* True if the decl represents an overloaded function that needs to be
     resolved by function_resolver.  */
  bool overloaded_p;

  /* The decl itself.  */
  tree decl;
};

/* Hash traits for registered_function.  */
struct registered_function_hasher : nofree_ptr_hash <registered_function>
{
  typedef function_instance compare_type;

  static hashval_t hash (value_type);
  static bool equal (value_type, const compare_type &);
};

/* A class for building the decls associated with:

     #pragma GCC aarch64 "arm_sve.h".  */
class arm_sve_h_builder
{
public:
  arm_sve_h_builder ();
  ~arm_sve_h_builder ();

  void register_type (vector_type);
  void build (const function_group &);

private:
  typedef void (arm_sve_h_builder::*function_signature)
    (const function_instance &, vec<tree> &);

  void build_all (function_signature, const function_group &, function_mode,
		  bool = false);
  void sig_inherent (const function_instance &, vec<tree> &);
  void sig_n_00 (const function_instance &, vec<tree> &);
  void scalar_sig_000 (const function_instance &, vec<tree> &);
  void sig_000 (const function_instance &, vec<tree> &);
  void sig_n_000 (const function_instance &, vec<tree> &);
  void sig_n_00i (const function_instance &, vec<tree> &);

  void apply_predication (const function_instance &, vec<tree> &);

  void add_function_instance (const function_instance &, vec<tree> &, bool);
  void add_overloaded_functions (const function_group &, function_mode);
  void add_overloaded_function (const function_instance &);

  registered_function &add_function (const function_instance &,
				     const char *, tree, tree, bool);
  tree get_attributes (const function_instance &);

  unsigned int get_explicit_types (function_shape);
  char *get_name (const function_instance &, bool);
  void append_name (const char *);
  char *finish_name ();

  /* The function type to use for functions that are resolved by
     function_resolver.  */
  tree m_overload_type;

  /* True if we should use create a separate decl for each instance of an
     overloaded function, instead of using function_resolver.  */
  bool m_direct_overloads;

  /* Used for building up function names.  */
  obstack m_string_obstack;
};

/* A class for resolving an overloaded function call.  */
class function_resolver
{
public:
  function_resolver (location_t, const registered_function &,
		     vec<tree, va_gc> &);
  tree resolve ();

private:
  tree resolve_uniform (unsigned int);
  tree resolve_uniform_imm (unsigned int, unsigned int);

  bool check_first_vector_argument (unsigned int, unsigned int &,
				    unsigned int &, vector_type &);
  bool check_num_arguments (unsigned int);
  bool check_argument (unsigned int, vector_type);
  vector_type require_vector_type (unsigned int);
  bool require_matching_type (unsigned int, vector_type);
  bool scalar_argument_p (unsigned int);
  bool require_integer_immediate (unsigned int);
  tree require_n_form (type_suffix, type_suffix = NUM_TYPE_SUFFIXES);
  tree require_form (function_mode, type_suffix,
		     type_suffix = NUM_TYPE_SUFFIXES);
  tree lookup_form (function_mode, type_suffix, type_suffix);
  tree get_argument_type (unsigned int);
  type_suffix get_type_suffix (vector_type);

  /* The location of the call.  */
  location_t m_location;

  /* The overloaded function.  */
  const registered_function &m_rfn;

  /* The arguments to the overloaded function.  */
  vec<tree, va_gc> &m_arglist;

  /* The static table entry for the overloaded function.  */
  const function_group &m_group;
};

/* A class for checking that the semantic constraints on a function call are
   satisfied, such as arguments being integer constant expressions with
   a particular range.  */
class function_checker
{
public:
  function_checker (location_t, const function_instance &, tree,
		    unsigned int, tree *);
  bool check ();

private:
  bool check_shift_right_imm ();

  bool require_immediate_range (unsigned int, HOST_WIDE_INT, HOST_WIDE_INT);

  /* The location of the call.  */
  location_t m_location;

  /* The non-overloaded function being called.  */
  const function_instance &m_fi;

  /* The function that the user called (which might be an overloaded form
     of M_FI).  */
  tree m_decl;

  /* The arguments to the function.  */
  unsigned int m_nargs;
  tree *m_args;

  /* The static table entry for the function.  */
  const function_group &m_group;
};

/* A class for folding a gimple function call.  */
class gimple_folder
{
public:
  gimple_folder (unsigned int, gcall *);
  gimple *fold ();

private:
  gimple *fold_ptrue ();

  /* The function being called.  */
  const function_instance &m_fi;

  /* The call we're folding.  */
  gcall *m_call;

  /* The result of the call, or null if none.  */
  tree m_lhs;
};

/* A class for expanding a function call into RTL.  */
class function_expander
{
public:
  function_expander (unsigned int, tree, rtx);
  rtx expand ();

private:
  rtx expand_abd ();
  rtx expand_add (unsigned int);
  rtx expand_asrd ();
  rtx expand_dup ();
  rtx expand_index ();
  rtx expand_max ();
  rtx expand_min ();
  rtx expand_mul ();
  rtx expand_ptrue ();
  rtx expand_qadd ();
  rtx expand_qsub ();
  rtx expand_sub (bool);

  rtx expand_signed_pred_op (rtx_code, rtx_code, int);
  rtx expand_signed_pred_op (int, int, int);
  rtx expand_via_unpred_direct_optab (optab, unsigned int = 0);
  rtx expand_via_unpred_insn (insn_code, unsigned int = 0);
  rtx expand_via_pred_direct_optab (optab, unsigned int);
  rtx expand_via_pred_insn (insn_code, unsigned int, unsigned int, bool);
  rtx expand_via_signed_unpred_insn (rtx_code, rtx_code);
  rtx expand_via_pred_x_insn (insn_code);
  rtx expand_pred_shift_right_imm (insn_code);

  void require_immediate_range (unsigned int, HOST_WIDE_INT, HOST_WIDE_INT);

  bool try_negating_argument (unsigned int, machine_mode);

  machine_mode get_mode (unsigned int);
  machine_mode get_pred_mode (unsigned int);

  void add_output_operand (machine_mode);
  void add_input_operand (rtx, machine_mode);
  void add_integer_operand (HOST_WIDE_INT);
  rtx generate_insn (insn_code);

  /* The function being called.  */
  const registered_function &m_rfn;
  const function_instance &m_fi;

  /* The function call expression.  */
  tree m_exp;

  /* The location of the call.  */
  location_t m_location;

  /* Where the result should go, if convenient.  */
  rtx m_target;

  /* The expanded arguments.  */
  auto_vec<rtx, 8> m_args;

  /* Used to build up the operands to an instruction.  */
  auto_vec<expand_operand, 8> m_ops;
};

/* Information about each single-predicate or single-vector type.  */
static const vector_type_info vector_types[] = {
#define DEF_SVE_TYPE(ACLE_NAME, NCHARS, ABI_NAME, SCALAR_TYPE) \
  { #ACLE_NAME, #ABI_NAME, #NCHARS #ABI_NAME },
#include "aarch64-sve-builtins.def"
};

/* The function name suffix associated with each predication type.  */
static const char *const pred_suffixes[NUM_PREDS + 1] = {
  "",
  "_m",
  "_x",
  "_z",
  ""
};

/* The function name suffix associated with each element type.  */
static const type_suffix_info type_suffixes[NUM_TYPE_SUFFIXES + 1] = {
#define DEF_SVE_TYPE_SUFFIX(NAME, ACLE_TYPE, BITS) \
  { "_" #NAME, VECTOR_TYPE_ ## ACLE_TYPE, BITS, BITS / BITS_PER_UNIT, \
    #NAME[0] == 's' || #NAME[0] == 'u', \
    #NAME[0] == 'u'},
#include "aarch64-sve-builtins.def"
  { "", NUM_VECTOR_TYPES, 0, 0, false, false }
};

/* _b8 _b16 _b32 _b64.  */
#define TYPES_all_pred(S, D) \
  S (b8), S (b16), S (b32), S (b64)

/* _f16 _f32 _f64.  */
#define TYPES_all_float(S, D) \
  S (f16), S (f32), S (f64)

/* _s8 _s16 _s32 _s64.  */
#define TYPES_all_signed(S, D) \
  S (s8), S (s16), S (s32), S (s64)

/* _u8 _u16 _u32 _u64.  */
#define TYPES_all_unsigned(S, D) \
  S (u8), S (u16), S (u32), S (u64)

/* _s8 _s16 _s32 _s64
   _u8 _u16 _u32 _u64.  */
#define TYPES_all_integer(S, D) \
  TYPES_all_signed (S, D), TYPES_all_unsigned (S, D)

/*     _f16 _f32 _f64
   _u8 _u16 _u32 _u64
   _s8 _s16 _s32 _s64.  */
#define TYPES_all_data(S, D) \
  TYPES_all_float (S, D), TYPES_all_integer (S, D)

/* Describe a pair of type suffixes in which only the first is used.  */
#define DEF_VECTOR_TYPE(X) { TYPE_SUFFIX_ ## X, NUM_TYPE_SUFFIXES }

/* Describe a pair of type suffixes in which both are used.  */
#define DEF_DOUBLE_TYPE(X, Y) { TYPE_SUFFIX_ ## X, TYPE_SUFFIX_ ## Y }

/* Create an array that can be used in aarch64-sve-builtins.def to
   select the type suffixes in TYPES_<NAME>.  */
#define DEF_SVE_TYPES_ARRAY(NAME) \
  static const type_suffix_pair types_##NAME[] { \
    TYPES_##NAME (DEF_VECTOR_TYPE, DEF_DOUBLE_TYPE), \
    { NUM_TYPE_SUFFIXES, NUM_TYPE_SUFFIXES } \
  }

DEF_SVE_TYPES_ARRAY (all_pred);
DEF_SVE_TYPES_ARRAY (all_unsigned);
DEF_SVE_TYPES_ARRAY (all_signed);
DEF_SVE_TYPES_ARRAY (all_float);
DEF_SVE_TYPES_ARRAY (all_integer);
DEF_SVE_TYPES_ARRAY (all_data);

/* Used by functions in aarch64-sve-builtins.def that have no governing
   predicate.  */
static const predication preds_none[] = { PRED_none, NUM_PREDS };

/* Used by functions in aarch64-sve-builtins.def that allow merging,
   zeroing and "don't care" predication.  */
static const predication preds_mxz[] = { PRED_m, PRED_x, PRED_z, NUM_PREDS };
static const predication preds_mxznone[] = { PRED_m, PRED_x, PRED_z,
					     PRED_none, NUM_PREDS };

/* A list of all SVE ACLE functions.  */
static const function_group function_groups[] = {
#define DEF_SVE_FUNCTION(NAME, SHAPE, TYPES, PREDS) \
  { #NAME, FUNC_##NAME, SHAPE_##SHAPE, types_##TYPES, preds_##PREDS },
#include "aarch64-sve-builtins.def"
};

/* The scalar type associated with each vector type.  */
static GTY(()) tree scalar_types[NUM_VECTOR_TYPES];

/* The single-predicate and single-vector types, with their built-in
   "__SV..._t" name.  */
static GTY(()) tree abi_vector_types[NUM_VECTOR_TYPES];

/* Same, but with the arm_sve.h "sv..._t" name.  */
static GTY(()) tree acle_vector_types[NUM_VECTOR_TYPES];

/* The list of all registered function decls, indexed by code.  */
static GTY(()) vec<registered_function *, va_gc> *registered_functions;

/* All registered function decls, hashed on the function_instance
   that they implement.  This is used for looking up implementations of
   overloaded functions.  */
static hash_table<registered_function_hasher> *function_table;

/* If TYPE is an ACLE vector type, return the associated vector_type,
   otherwise return NUM_VECTOR_TYPES.  */
static vector_type
find_vector_type (const_tree type)
{
  /* A linear search should be OK here, since the code isn't hot and
     the number of types is only small.  */
  for (unsigned int i = 0; i < ARRAY_SIZE (abi_vector_types); ++i)
    if (TYPE_MAIN_VARIANT (type) == abi_vector_types[i])
      return (vector_type) i;
  return NUM_VECTOR_TYPES;
}

/* Report that LOCATION has a call to DECL in which argument ARGNO
   was not an integer constant expression.  */
static void
report_non_ice (location_t location, tree decl, unsigned int argno)
{
  error_at (location, "argument %d of %qE must be an integer constant"
	    " expression", argno + 1, decl);
}

/* Report that LOCATION has a call to DECL in which argument ARGNO has
   the value ACTUAL, whereas the function requires a value in the range
   [MIN, MAX].  */
static void
report_out_of_range (location_t location, tree decl, unsigned int argno,
		     HOST_WIDE_INT actual, HOST_WIDE_INT min,
		     HOST_WIDE_INT max)
{
  error_at (location, "passing %wd to argument %d of %qE, which expects"
	    " a value in the range [%wd, %wd]", actual, argno + 1, decl,
	    min, max);
}

inline
function_instance::function_instance (function func_in,
				      function_mode mode_in,
				      const type_suffix_pair &types_in,
				      predication pred_in)
  : func (func_in), mode (mode_in), pred (pred_in)
{
  memcpy (types, types_in, sizeof (types));
}

inline bool
function_instance::operator== (const function_instance &other) const
{
  return (func == other.func
	  && mode == other.mode
	  && pred == other.pred
	  && types[0] == other.types[0]
	  && types[1] == other.types[1]);
}

inline bool
function_instance::operator!= (const function_instance &other) const
{
  return !operator== (other);
}

hashval_t
function_instance::hash () const
{
  inchash::hash h;
  h.add_int (func);
  h.add_int (mode);
  h.add_int (types[0]);
  h.add_int (types[1]);
  h.add_int (pred);
  return h.end ();
}

/* Return the scalar type associated with type suffix I.  */
inline tree
function_instance::scalar_type (unsigned int i) const
{
  return scalar_types[type_suffixes[types[i]].type];
}

/* Return the vector type associated with type suffix I.  */
inline tree
function_instance::vector_type (unsigned int i) const
{
  return acle_vector_types[type_suffixes[types[i]].type];
}

inline hashval_t
registered_function_hasher::hash (value_type value)
{
  return value->instance.hash ();
}

inline bool
registered_function_hasher::equal (value_type value, const compare_type &key)
{
  return value->instance == key;
}

arm_sve_h_builder::arm_sve_h_builder ()
{
  m_overload_type = build_function_type (void_type_node, void_list_node);
  m_direct_overloads = lang_GNU_CXX ();
  gcc_obstack_init (&m_string_obstack);
}

arm_sve_h_builder::~arm_sve_h_builder ()
{
  obstack_free (&m_string_obstack, NULL);
}

/* Register TYPE under its arm_sve.h name.  */
void
arm_sve_h_builder::register_type (vector_type type)
{
  tree vectype = abi_vector_types[type];
  tree id = get_identifier (vector_types[type].acle_name);
  tree decl = build_decl (input_location, TYPE_DECL, id, vectype);
  decl = lang_hooks.decls.pushdecl (decl);
  if (TREE_CODE (decl) == TYPE_DECL
      && TYPE_MAIN_VARIANT (TREE_TYPE (decl)) == vectype)
    vectype = TREE_TYPE (decl);
  acle_vector_types[type] = vectype;
}

/* Define all functions associated with GROUP.  */
void
arm_sve_h_builder::build (const function_group &group)
{
  switch (group.shape)
    {
    case SHAPE_unary_n:
      build_all (&arm_sve_h_builder::sig_n_00, group, MODE_n, true);
      break;

    case SHAPE_binary_scalar:
      build_all (&arm_sve_h_builder::scalar_sig_000, group, MODE_none);
      break;

    case SHAPE_binary_opt_n:
      add_overloaded_functions (group, MODE_none);
      build_all (&arm_sve_h_builder::sig_000, group, MODE_none);
      build_all (&arm_sve_h_builder::sig_n_000, group, MODE_n);
      break;

    case SHAPE_inherent:
      /* No overloaded functions here.  */
      build_all (&arm_sve_h_builder::sig_inherent, group, MODE_none);
      break;

    case SHAPE_shift_right_imm:
      add_overloaded_functions (group, MODE_n);
      build_all (&arm_sve_h_builder::sig_n_00i, group, MODE_n);
      break;
    }
}

/* Add a function instance for every type and predicate combination
   in GROUP.  Take the function base name from GROUP and the mode
   from MODE.  Use SIGNATURE to construct the function signature without
   a governing predicate, then use apply_predication to add in the
   predicate.  FORCE_DIRECT_OVERLOADS is true if there is a one-to-one
   mapping between "short" and "full" names, and if standard overload
   resolution therefore isn't necessary.  */
void
arm_sve_h_builder::build_all (function_signature signature,
			      const function_group &group,
			      function_mode mode,
			      bool force_direct_overloads)
{
  auto_vec<tree, 10> types;
  for (unsigned int pi = 0; group.preds[pi] != NUM_PREDS; ++pi)
    for (unsigned int ti = 0; group.types[ti][0] != NUM_TYPE_SUFFIXES; ++ti)
      {
	function_instance instance (group.func, mode, group.types[ti],
				    group.preds[pi]);
	(this->*signature) (instance, types);
	apply_predication (instance, types);
	add_function_instance (instance, types, force_direct_overloads);
	types.truncate (0);
      }
}

/* Describe the signature "sv<t0>_t svfoo[_t0]()" for INSTANCE in TYPES.  */
void
arm_sve_h_builder::sig_inherent (const function_instance &instance,
				 vec<tree> &types)
{
  types.quick_push (instance.vector_type (0));
}

/* Describe the signature "sv<t0>_t svfoo[_n_t0](<t0>_t)"
   for INSTANCE in TYPES.  */
void
arm_sve_h_builder::sig_n_00 (const function_instance &instance,
			      vec<tree> &types)
{
  types.quick_push (instance.vector_type (0));
  types.quick_push (instance.scalar_type (0));
}

/* Describe the signature "sv<t0>_t svfoo[_t0](<t0>_t, <t0>_t)"
   for INSTANCE in TYPES.  */
void
arm_sve_h_builder::scalar_sig_000 (const function_instance &instance,
				   vec<tree> &types)
{
  types.quick_push (instance.vector_type (0));
  types.quick_push (instance.scalar_type (0));
  types.quick_push (instance.scalar_type (0));
}

/* Describe the signature "sv<t0>_t svfoo[_t0](sv<t0>_t, sv<t0>_t)"
   for INSTANCE in TYPES.  */
void
arm_sve_h_builder::sig_000 (const function_instance &instance,
			    vec<tree> &types)
{
  for (unsigned int i = 0; i < 3; ++i)
    types.quick_push (instance.vector_type (0));
}

/* Describe the signature "sv<t0>_t svfoo[_n_t0](sv<t0>_t, <t0>_t)"
   for INSTANCE in TYPES.  */
void
arm_sve_h_builder::sig_n_000 (const function_instance &instance,
			      vec<tree> &types)
{
  for (unsigned int i = 0; i < 2; ++i)
    types.quick_push (instance.vector_type (0));
  types.quick_push (instance.scalar_type (0));
}

/* Describe the signature "sv<t0>_t svfoo[_n_t0](sv<t0>_t, uint64_t)"
   for INSTANCE in TYPES.  */
void
arm_sve_h_builder::sig_n_00i (const function_instance &instance,
			      vec<tree> &types)
{
  for (unsigned int i = 0; i < 2; ++i)
    types.quick_push (instance.vector_type (0));
  types.quick_push (scalar_types[VECTOR_TYPE_svuint64_t]);
}

/* If INSTANCE has a governing predicate, add it to the type signature
   in TYPES, where TYPES[0] is the return type, TYPES[1] is the first
   argument type, etc.  */
void
arm_sve_h_builder::apply_predication (const function_instance &instance,
				      vec<tree> &types)
{
  if (instance.pred != PRED_none)
    {
      types.quick_insert (1, acle_vector_types[VECTOR_TYPE_svbool_t]);
      /* For unary merge operations, the first argument is a vector with
	 the same type as the result.  */
      if (types.length () == 3 && instance.pred == PRED_m)
	types.quick_insert (1, types[0]);
    }
}

/* Add a built-in function for INSTANCE, with the function signature
   given by TYPES.  TYPES[0] is the return type, TYPES[1] is the first
   argument type, etc.  FORCE_DIRECT_OVERLOADS is true if there is a
   one-to-one mapping between "short" and "full" names, and if standard
   overload resolution therefore isn't necessary.  */
void
arm_sve_h_builder::add_function_instance (const function_instance &instance,
					  vec<tree> &types,
					  bool force_direct_overloads)
{
  /* Add the function under its full (unique) name.  */
  char *name = get_name (instance, false);
  tree fntype = build_function_type_array (types[0], types.length () - 1,
					   types.address () + 1);
  tree attrs = get_attributes (instance);
  registered_function &rfn = add_function (instance, name, fntype,
					   attrs, false);

  /* Enter the function into the hash table.  */
  hashval_t hash = instance.hash ();
  registered_function **rfn_slot
    = function_table->find_slot_with_hash (instance, hash, INSERT);
  gcc_assert (!*rfn_slot);
  *rfn_slot = &rfn;

  /* Also add the function under its overloaded alias, if we want
     a separate decl for each instance of an overloaded function.  */
  if (m_direct_overloads || force_direct_overloads)
    {
      char *overload_name = get_name (instance, true);
      if (strcmp (name, overload_name) != 0)
	add_function (instance, overload_name, fntype, attrs, true);
    }

  obstack_free (&m_string_obstack, name);
}

/* If we are using manual overload resolution, add one function decl
   for each overloaded function in GROUP.  Take the function base name
   from GROUP and the mode from MODE.  */
void
arm_sve_h_builder::add_overloaded_functions (const function_group &group,
					     function_mode mode)
{
  if (m_direct_overloads)
    return;

  unsigned int explicit_types = get_explicit_types (group.shape);
  for (unsigned int pi = 0; group.preds[pi] != NUM_PREDS; ++pi)
    {
      type_suffix_pair prev_types = { NUM_TYPE_SUFFIXES, NUM_TYPE_SUFFIXES };
      if (explicit_types == 0)
	/* One overloaded function for all type combinations.  */
	add_overloaded_function
	  (function_instance (group.func, mode, prev_types,
			      group.preds[pi]));
      else
	for (unsigned int ti = 0; group.types[ti][0] != NUM_TYPE_SUFFIXES;
	     ++ti)
	  {
	    /* Stub out the types that are determined by overload resolution.
	       The list of type suffixes is lexicographically ordered,
	       so we can check for duplicates by comparing this pair
	       against the previous one.  */
	    type_suffix_pair types = {
	      explicit_types & 1 ? group.types[ti][0] : NUM_TYPE_SUFFIXES,
	      explicit_types & 2 ? group.types[ti][1] : NUM_TYPE_SUFFIXES
	    };
	    if (ti > 0
		&& types[0] == prev_types[0]
		&& types[1] == prev_types[1])
	      continue;
	    add_overloaded_function
	      (function_instance (group.func, mode, types, group.preds[pi]));
	    memcpy (prev_types, types, sizeof (types));
	  }
    }
}

/* Add one function decl for INSTANCE, to be used with manual overloading.  */
void
arm_sve_h_builder::add_overloaded_function (const function_instance &instance)
{
  char *name = get_name (instance, true);
  add_function (instance, name, m_overload_type, NULL_TREE, true);
  obstack_free (&m_string_obstack, name);
}

/* Add a function NAME with type FNTYPE and attributes ATTRS.  INSTANCE
   describes what the function does and OVERLOADED_P indicates whether
   it is overloaded.  */
registered_function &
arm_sve_h_builder::add_function (const function_instance &instance,
				 const char *name, tree fntype, tree attrs,
				 bool overloaded_p)
{
  unsigned int code = vec_safe_length (registered_functions);
  code = (code << AARCH64_BUILTIN_SHIFT) | AARCH64_BUILTIN_SVE;
  tree decl = add_builtin_function_global_md (input_location, name, fntype,
					      code, NULL, attrs);

  registered_function &rfn = *ggc_alloc <registered_function> ();
  rfn.instance = instance;
  rfn.overloaded_p = overloaded_p;
  rfn.decl = decl;
  vec_safe_push (registered_functions, &rfn);

  return rfn;
}

/* Add attribute NAME to ATTRS.  */
static tree
add_attribute (const char *name, tree attrs)
{
  return tree_cons (get_identifier (name), NULL_TREE, attrs);
}

/* Return the appropriate function attributes for INSTANCE.  */
tree
arm_sve_h_builder::get_attributes (const function_instance &instance)
{
  tree attrs = NULL_TREE;
  switch (instance.func)
    {
    case FUNC_svabd:
    case FUNC_svadd:
    case FUNC_svasrd:
    case FUNC_svdup:
    case FUNC_svindex:
    case FUNC_svmax:
    case FUNC_svmin:
    case FUNC_svmul:
    case FUNC_svqadd:
    case FUNC_svqsub:
    case FUNC_svsub:
    case FUNC_svsubr:
      if (type_suffixes[instance.types[0]].integer_p)
	{
	  attrs = add_attribute ("const", attrs);
	  attrs = add_attribute ("nothrow", attrs);
	}
      else if (flag_non_call_exceptions && flag_trapping_math)
	/* Exceptions have to be preserved exactly.  */;
      else
	{
	  /* Only pure, not const, because it depends on the rounding mode.  */
	  attrs = add_attribute ("pure", attrs);
	  attrs = add_attribute ("nothrow", attrs);
	}
      break;

    case FUNC_svptrue:
      attrs = add_attribute ("const", attrs);
      attrs = add_attribute ("nothrow", attrs);
      break;

    case NUM_FUNCS:
      break;
    }
  return add_attribute ("leaf", attrs);
}

/* Return the bitmask of types that are always explicit in functions
   with shape SHAPE.  Bit 0 is set if the first type is always explicit
   and bit 1 is set if the second type is always explicit.  */
unsigned int
arm_sve_h_builder::get_explicit_types (function_shape shape)
{
  switch (shape)
    {
    case SHAPE_inherent:
    case SHAPE_unary_n:
    case SHAPE_binary_scalar:
      return 1;
    case SHAPE_binary_opt_n:
    case SHAPE_shift_right_imm:
      return 0;
    }
  gcc_unreachable ();
}

/* Return the overloaded or full function name for INSTANCE; OVERLOADED_P
   selects which.  */
char *
arm_sve_h_builder::get_name (const function_instance &instance,
			     bool overloaded_p)
{
  const function_group &group = function_groups[instance.func];
  unsigned int explicit_types
    = (overloaded_p ? get_explicit_types (group.shape) : 3);

  append_name (group.name);
  switch (instance.mode)
    {
    case MODE_none:
      break;

    case MODE_n:
      if (!overloaded_p)
	append_name ("_n");
      break;
    }
  if (explicit_types & 1)
    append_name (type_suffixes[instance.types[0]].suffix);
  if (explicit_types & 2)
    append_name (type_suffixes[instance.types[1]].suffix);
  append_name (pred_suffixes[instance.pred]);
  return finish_name ();
}

/* Add NAME to the end of the function name being built.  */
void
arm_sve_h_builder::append_name (const char *name)
{
  obstack_grow (&m_string_obstack, name, strlen (name));
}

/* Zero-terminate and complete the function name being built.  */
char *
arm_sve_h_builder::finish_name ()
{
  obstack_1grow (&m_string_obstack, 0);
  return (char *) obstack_finish (&m_string_obstack);
}

function_resolver::function_resolver (location_t location,
				      const registered_function &rfn,
				      vec<tree, va_gc> &arglist)
  : m_location (location), m_rfn (rfn), m_arglist (arglist),
    m_group (function_groups[rfn.instance.func])
{
}

/* Try to resolve the overloaded call.  Return the non-overloaded function
   decl on success and error_mark_node on failure.  */
tree
function_resolver::resolve ()
{
  switch (m_group.shape)
    {
    case SHAPE_binary_opt_n:
      return resolve_uniform (2);
    case SHAPE_shift_right_imm:
      return resolve_uniform_imm (2, 1);
    case SHAPE_unary_n:
      return NULL_TREE;
    case SHAPE_binary_scalar:
    case SHAPE_inherent:
      break;
    }
  gcc_unreachable ();
}

/* Resolve a function in which all vector and scalar types in the signature
   have the same element type.  NOPS is the number of vector and scalar
   arguments before predication is applied to the signature (1 for unary
   ops, 2 for binary, etc.).  */
tree
function_resolver::resolve_uniform (unsigned int nops)
{
  /* Check that we have the right number of arguments.  */
  unsigned int i, nargs;
  vector_type type;
  if (!check_first_vector_argument (nops, i, nargs, type))
    return error_mark_node;

  /* Handle subsequent arguments.  */
  for (; i < nargs; ++i)
    {
      /* Allow the final argument to be scalar, if an _n form exists.  */
      if (i == nargs - 1 && scalar_argument_p (i))
	return require_n_form (get_type_suffix (type));
      if (!require_matching_type (i, type))
	return error_mark_node;
    }
  return require_form (m_rfn.instance.mode, get_type_suffix (type));
}

/* Like resolve_uniform, except that the final NIMM arguments have
   type uint64_t and must be integer constant expressions.  */
tree
function_resolver::resolve_uniform_imm (unsigned int nops, unsigned int nimm)
{
  /* Check that we have the right number of arguments.  Also check the
     first vector argument and governing predicate.  */
  unsigned int i, nargs;
  vector_type type;
  if (!check_first_vector_argument (nops, i, nargs, type))
    return error_mark_node;

  /* Handle subsequent vector arguments.  */
  for (; i < nargs - nimm; ++i)
    if (!require_matching_type (i, type))
      return error_mark_node;

  /* Handle the final immediate arguments.  */
  for (; i < nargs; ++i)
    if (!require_integer_immediate (i))
      return error_mark_node;

  return require_form (m_rfn.instance.mode, get_type_suffix (type));
}

/* Check that the function is passed NOPS arguments plus the governing
   predicate (if applicable) and that the first argument besides the
   governing predicate is a vector.  Return true if so, otherwise
   report a suitable error.

   When returning true:
   - set I to the number of the first unchecked argument (past the first
     vector and past any governing predicate).
   - set NARGS to the number of arguments including any governing
     predicate.
   - set TYPE to the type of the vector argument.  */
bool
function_resolver::check_first_vector_argument (unsigned int nops,
						unsigned int &i,
						unsigned int &nargs,
						vector_type &type)
{
  i = 0;
  nargs = nops;
  type = NUM_VECTOR_TYPES;

  if (m_rfn.instance.pred != PRED_none)
    nargs += 1;
  if (!check_num_arguments (nargs))
    return false;

  /* Check the predicate argument.  */
  if (m_rfn.instance.pred != PRED_none)
    {
      if (!check_argument (i, VECTOR_TYPE_svbool_t))
	return false;
      i += 1;
    }

  /* The next argument is always a vector.  */
  type = require_vector_type (i);
  if (type == NUM_VECTOR_TYPES)
    return false;

  return true;
}

/* Require the function to have exactly EXPECTED arguments.  Return true
   if it does.  */
bool
function_resolver::check_num_arguments (unsigned int expected)
{
  if (m_arglist.length () < expected)
    error_at (m_location, "too few arguments to function %qE", m_rfn.decl);
  else if (m_arglist.length () > expected)
    error_at (m_location, "too many arguments to function %qE", m_rfn.decl);
  return m_arglist.length () == expected;
}

/* Require argument I to have some form of vector type.  Return the
   type of the vector on success and NUM_VECTOR_TYPES on failure.  */
vector_type
function_resolver::require_vector_type (unsigned int i)
{
  tree actual = get_argument_type (i);
  if (actual == error_mark_node)
    return NUM_VECTOR_TYPES;
  vector_type type = find_vector_type (actual);
  if (type == NUM_VECTOR_TYPES)
    error_at (m_location, "passing %qT to argument %d of %qE, which"
	      " expects an SVE vector type", actual, i + 1, m_rfn.decl);
  return type;
}

/* Require argument I to have the vector type associated with TYPE,
   in cases where this requirement holds for all uses of the function.
   Return true if the argument has the right form.  */
bool
function_resolver::check_argument (unsigned int i, vector_type type)
{
  tree expected = acle_vector_types[type];
  tree actual = get_argument_type (i);
  if (actual != error_mark_node
      && TYPE_MAIN_VARIANT (expected) != TYPE_MAIN_VARIANT (actual))
    {
      error_at (m_location, "passing %qT to argument %d of %qE, which"
		" expects %qT", actual, i + 1, m_rfn.decl, expected);
      return false;
    }
  return true;
}

/* Likewise, but TYPE is determined by matching previous arguments
   rather than being a fixed part of the function signature.  This changes
   the nature of the error messages.  */
bool
function_resolver::require_matching_type (unsigned int i, vector_type type)
{
  vector_type new_type = require_vector_type (i);
  if (new_type == NUM_VECTOR_TYPES)
    return false;

  if (type != new_type)
    {
      error_at (m_location, "passing %qT to argument %d of %qE, but"
		" previous arguments had type %qT",
		acle_vector_types[new_type], i + 1, m_rfn.decl,
		acle_vector_types[type]);
      return false;
    }
  return true;
}

/* Return true if argument I is a scalar integer or floating-point value.  */
bool
function_resolver::scalar_argument_p (unsigned int i)
{
  tree type = get_argument_type (i);
  return INTEGRAL_TYPE_P (type) || SCALAR_FLOAT_TYPE_P (type);
}

/* Check that argument I has a suitable form for an integer constant
   expression.  function_checker checks whether the argument is
   actually constant and has a suitable range.  */
bool
function_resolver::require_integer_immediate (unsigned int i)
{
  if (!scalar_argument_p (i))
    {
      report_non_ice (m_location, m_rfn.decl, i);
      return false;
    }
  return true;
}

/* Return the type of argument I, or error_mark_node if it isn't
   well-formed.  */
tree
function_resolver::get_argument_type (unsigned int i)
{
  tree arg = m_arglist[i];
  return arg == error_mark_node ? arg : TREE_TYPE (arg);
}

/* Return the type suffix associated with vector type TYPE, using _b
   in the case of svbool_t.  */
type_suffix
function_resolver::get_type_suffix (vector_type type)
{
  for (unsigned int i = 0; i < NUM_TYPE_SUFFIXES; ++i)
    if (type_suffixes[i].type == type)
      return type_suffix (i);
  gcc_unreachable ();
}

/* Require there to be an _n instance of the function with the type suffixes
   given by TYPE0 and TYPE1.  Return its function decl on success and
   error_mark_node on failure.  */
tree
function_resolver::require_n_form (type_suffix type0, type_suffix type1)
{
  tree res = lookup_form (MODE_n, type0, type1);
  if (res)
    return res;

  res = require_form (MODE_none, type0, type1);
  if (res)
    error_at (m_location, "%qE has no %qs form", res, "_n");
  return error_mark_node;
}

/* Require there to be an instance of the function with the mode given
   by MODE and with the type suffixes given by TYPE0 and TYPE1.  Return
   its function decl on success and error_mark_node on failure.  */
tree
function_resolver::require_form (function_mode mode, type_suffix type0,
				 type_suffix type1)
{
  tree res = lookup_form (mode, type0, type1);
  if (!res)
    {
      if (type1 == NUM_TYPE_SUFFIXES)
	error_at (m_location, "%qE has no form that takes %qT arguments",
		  m_rfn.decl, acle_vector_types[type_suffixes[type0].type]);
      else
	/* To be filled in when we have other cases.  */
	gcc_unreachable ();
      return error_mark_node;
    }
  return res;
}

/* See whether there is an instance of the function with the mode given
   by MODE and the type suffixes given by TYPE0 and TYPE1.  Return its
   function decl on success, otherwise return null.  */
tree
function_resolver::lookup_form (function_mode mode, type_suffix type0,
				type_suffix type1)
{
  type_suffix_pair types = { type0, type1 };
  function_instance instance (m_rfn.instance.func, mode, types,
			      m_rfn.instance.pred);
  registered_function *rfn
    = function_table->find_with_hash (instance, instance.hash ());
  return rfn ? rfn->decl : NULL_TREE;
}

function_checker::function_checker (location_t location,
				    const function_instance &fi, tree decl,
				    unsigned int nargs, tree *args)
  : m_location (location), m_fi (fi), m_decl (decl), m_nargs (nargs),
    m_args (args), m_group (function_groups[m_fi.func])
{
}

/* Perform semantic checks on the call.  Return true if the call is valid,
   otherwise report a suitable error.  */
bool
function_checker::check ()
{
  switch (m_group.shape)
    {
    case SHAPE_shift_right_imm:
      return check_shift_right_imm ();

    case SHAPE_unary_n:
    case SHAPE_inherent:
    case SHAPE_binary_opt_n:
    case SHAPE_binary_scalar:
      return true;
    }
  gcc_unreachable ();
}

/* Check a SHAPE_shift_right_imm call.  */
bool
function_checker::check_shift_right_imm ()
{
  unsigned int bits = type_suffixes[m_fi.types[0]].elem_bits;
  return require_immediate_range (2, 1, bits);
}

/* Check that argument ARGNO is an integer constant expression in the
   range [MIN, MAX].  */
bool
function_checker::require_immediate_range (unsigned int argno,
					   HOST_WIDE_INT min,
					   HOST_WIDE_INT max)
{
  if (m_nargs <= argno)
    return true;

  tree arg = m_args[argno];
  if (!tree_fits_shwi_p (arg))
    {
      report_non_ice (m_location, m_decl, argno);
      return false;
    }

  HOST_WIDE_INT actual = tree_to_shwi (arg);
  if (!IN_RANGE (actual, min, max))
    {
      report_out_of_range (m_location, m_decl, argno, actual, min, max);
      return false;
    }

  return true;
}

/* Register the built-in SVE ABI types, such as __SVBool_t.  */
static void
register_builtin_types ()
{
#define DEF_SVE_TYPE(ACLE_NAME, NCHARS, ABI_NAME, SCALAR_TYPE) \
  scalar_types[VECTOR_TYPE_ ## ACLE_NAME] = SCALAR_TYPE;
#include "aarch64-sve-builtins.def"

  for (unsigned int i = 0; i < NUM_VECTOR_TYPES; ++i)
    {
      tree eltype = scalar_types[i];
      tree vectype;
      if (eltype == boolean_type_node)
	vectype = build_truth_vector_type (BYTES_PER_SVE_VECTOR,
					   BYTES_PER_SVE_VECTOR);
      else
	{
	  unsigned int elbytes = tree_to_uhwi (TYPE_SIZE_UNIT (eltype));
	  poly_uint64 nunits = exact_div (BYTES_PER_SVE_VECTOR, elbytes);
	  vectype = build_vector_type (eltype, nunits);
	}
      vectype = build_distinct_type_copy (vectype);
      SET_TYPE_STRUCTURAL_EQUALITY (vectype);
      TYPE_ARTIFICIAL (vectype) = 1;
      abi_vector_types[i] = vectype;
      lang_hooks.types.register_builtin_type (vectype,
					      vector_types[i].abi_name);
    }
}

/* Initialize all compiler built-ins related to SVE.  */
void
init_builtins ()
{
  register_builtin_types ();
}

/* Return true if TYPE is a built-in type defined by the SVE ABI.  */
bool
builtin_type_p (const_tree type)
{
  return find_vector_type (type) != NUM_VECTOR_TYPES;
}

/* If TYPE is a builtin-type defined by the SVE ABI, return the mangled name,
   otherwise return NULL.  */
const char *
mangle_builtin_type (const_tree type)
{
  vector_type vtype = find_vector_type (type);
  if (vtype != NUM_VECTOR_TYPES)
    return vector_types[vtype].mangled_name;
  return NULL;
}

/* Implement #pragma GCC aarch64 "arm_sve.h".  */
void
handle_arm_sve_h ()
{
  if (function_table)
    {
      error ("duplicate definition of %qs", "arm_sve.h");
      return;
    }

  function_table = new hash_table<registered_function_hasher> (1023);
  arm_sve_h_builder builder;
  for (unsigned int i = 0; i < NUM_VECTOR_TYPES; ++i)
    builder.register_type (vector_type (i));
  for (unsigned int i = 0; i < ARRAY_SIZE (function_groups); ++i)
    builder.build (function_groups[i]);
}

/* Return the function decl with SVE function subcode CODE, or error_mark_node
   if no such function exists.  */
tree
builtin_decl (unsigned int code, bool)
{
  if (code >= vec_safe_length (registered_functions))
    return error_mark_node;
  return (*registered_functions)[code]->decl;
}

/* If we're implementing manual overloading, check whether the SVE
   function with subcode CODE is overloaded, and if so attempt to
   determine the corresponding non-overloaded function.  The call
   occurs at location LOCATION and has the arguments given by ARGLIST.

   If the call is erroneous, report an appropriate error and return
   error_mark_node.  Otherwise, if the function is overloaded, return
   the decl of the non-overloaded function.  Return NULL_TREE otherwise,
   indicating that the call should be processed in the normal way.  */
tree
resolve_overloaded_builtin (location_t location, unsigned int code,
			    vec<tree, va_gc> *arglist)
{
  if (code >= vec_safe_length (registered_functions))
    return NULL_TREE;

  registered_function &rfn = *(*registered_functions)[code];
  if (rfn.overloaded_p)
    return function_resolver (location, rfn, *arglist).resolve ();
  return NULL_TREE;
}

/* Perform any semantic checks needed for a call to the SVE function with
   subcode CODE, such as testing for integer constant expressions.
   The call occurs at location LOCATION and has NARGS arguments.
   ARGS gives the value of each argument and ARG_LOCATION gives
   their location.  FNDECL is the original function decl, before
   overload resolution.

   Return true if the call is valid, otherwise report a suitable error.  */
bool
check_builtin_call (location_t location, vec<location_t>, unsigned int code,
		    tree fndecl, unsigned int nargs, tree *args)
{
  registered_function &rfn = *(*registered_functions)[code];
  return function_checker (location, rfn.instance, fndecl,
			   nargs, args).check ();
}

/* Construct a folder for CALL, which calls the SVE function with
   subcode CODE.  */
gimple_folder::gimple_folder (unsigned int code, gcall *call)
  : m_fi ((*registered_functions)[code]->instance), m_call (call),
    m_lhs (gimple_call_lhs (call))
{
}

/* Try to fold the call.  Return the new statement on success and null
   on failure.  */
gimple *
gimple_folder::fold ()
{
  switch (m_fi.func)
    {
    case FUNC_svabd:
    case FUNC_svadd:
    case FUNC_svasrd:
    case FUNC_svdup:
    case FUNC_svindex:
    case FUNC_svmax:
    case FUNC_svmin:
    case FUNC_svmul:
    case FUNC_svqadd:
    case FUNC_svqsub:
    case FUNC_svsub:
    case FUNC_svsubr:
    case NUM_FUNCS:
      return NULL;

    case FUNC_svptrue:
      return fold_ptrue ();
    }
  gcc_unreachable ();
}

/* Fold a call to svptrue.  */
gimple *
gimple_folder::fold_ptrue ()
{
  tree vectype = m_fi.vector_type (0);
  tree elttype = TREE_TYPE (vectype);
  unsigned int num_bytes = type_suffixes[m_fi.types[0]].elem_bytes;

  /* The type is svbool_t regardless of TYPE, thus for b8 we want
     { 1, 1, 1, 1, ... }, for b16 we want { 1, 0, 1, 0, ... }, etc.  */
  tree_vector_builder builder (vectype, num_bytes, 1);
  builder.quick_push (build_int_cst (elttype, 1));
  for (unsigned int i = 1; i < num_bytes; ++i)
    builder.quick_push (build_int_cst (elttype, 0));
  return gimple_build_assign (m_lhs, builder.build ());
}

/* Attempt to fold STMT, given that it's a call to the SVE function
   with subcode CODE.  Return the new statement on success and null
   on failure.  */
gimple *
gimple_fold_builtin (unsigned int code, gcall *stmt)
{
  return gimple_folder (code, stmt).fold ();
}

/* Construct an expander for a call to the SVE function with subcode CODE.
   EXP is the call expression and TARGET is the preferred location for
   the result.  */
function_expander::function_expander (unsigned int code, tree exp, rtx target)
  : m_rfn (*(*registered_functions)[code]),
    m_fi (m_rfn.instance),
    m_exp (exp), m_location (EXPR_LOCATION (exp)), m_target (target)
{
}

/* Expand the call and return its lhs.  */
rtx
function_expander::expand ()
{
  unsigned int nargs = call_expr_nargs (m_exp);
  m_args.reserve (nargs);
  for (unsigned int i = 0; i < nargs; ++i)
    m_args.quick_push (expand_normal (CALL_EXPR_ARG (m_exp, i)));

  switch (m_fi.func)
    {
    case FUNC_svabd:
      return expand_abd ();

    case FUNC_svadd:
      return expand_add (1);

    case FUNC_svasrd:
      return expand_asrd ();

    case FUNC_svdup:
      return expand_dup ();

    case FUNC_svindex:
      return expand_index ();

    case FUNC_svmax:
      return expand_max ();

    case FUNC_svmin:
      return expand_min ();

    case FUNC_svmul:
      return expand_mul ();

    case FUNC_svptrue:
      return expand_ptrue ();

     case FUNC_svqadd:
      return expand_qadd ();

    case FUNC_svqsub:
      return expand_qsub ();

    case FUNC_svsub:
      return expand_sub (false);

    case FUNC_svsubr:
      return expand_sub (true);

    case NUM_FUNCS:
      break;
    }
  gcc_unreachable ();
}

/* Expand a call to svabd.  */
rtx
function_expander::expand_abd ()
{
  return expand_signed_pred_op (UNSPEC_COND_SABD, UNSPEC_COND_UABD,
				UNSPEC_COND_FABD);
}

/* Expand a call to svadd, or svsub(r) with a negated operand.
   MERGE_ARGNO is the argument that should be used as the fallback
   value in a merging operation.  */
rtx
function_expander::expand_add (unsigned int merge_argno)
{
  if (m_fi.pred == PRED_x)
    {
      if (type_suffixes[m_fi.types[0]].integer_p)
	return expand_via_unpred_direct_optab (add_optab);
      else
	{
	  /* Try to take advantage of unpredicated FP addition, rather than
	     simply treating _x as _m.  */
	  insn_code icode = code_for_aarch64_pred_add (get_mode (0));
	  return expand_via_pred_x_insn (icode);
	}
    }
  return expand_via_pred_direct_optab (cond_add_optab, merge_argno);
}

/* Expand a call to svasrd.  */
rtx
function_expander::expand_asrd ()
{
  return expand_pred_shift_right_imm (code_for_cond_asrd (get_mode (0)));
}

/* Expand a call to svdup.  */
rtx
function_expander::expand_dup ()
{
  if (m_fi.pred == PRED_none
      || m_fi.pred == PRED_x)
    return expand_via_unpred_direct_optab (vec_duplicate_optab, 1);
  else
    {
      insn_code icode;
      machine_mode mode = get_mode (0);
      if (valid_for_const_vector_p (GET_MODE_INNER (mode), m_args.last ()))
	{
	  icode = code_for_vcond_mask (get_mode (0), get_mode (0));
	  return expand_via_pred_insn (icode, 0, 1, true);
	}
      else
	{
	  icode = code_for_aarch64_sel_dup (get_mode (0));
	  return expand_via_pred_insn (icode, 1, 1, true);
	}
    }
}

/* Expand a call to svindex.  */
rtx
function_expander::expand_index ()
{
  return expand_via_unpred_direct_optab (vec_series_optab, 2);
}

/* Expand a call to svmax.  */
rtx
function_expander::expand_max ()
{
  return expand_signed_pred_op (SMAX, UMAX, UNSPEC_COND_FMAX);
}

/* Expand a call to svmin.  */
rtx
function_expander::expand_min ()
{
  return expand_signed_pred_op (SMIN, UMIN, UNSPEC_COND_FMIN);
}

/* Expand a call to svmul.  */
rtx
function_expander::expand_mul ()
{
  if (m_fi.pred == PRED_x)
    {
      insn_code icode = code_for_aarch64_pred_mul (get_mode (0));
      return expand_via_pred_x_insn (icode);
    }
  else
    return expand_via_pred_direct_optab (cond_smul_optab, 1);
}

/* Expand a call to sqadd.  */
rtx
function_expander::expand_qadd ()
{
  return expand_via_signed_unpred_insn (SS_PLUS, US_PLUS);
}

/* Expand a call to sqsub.  */
rtx
function_expander::expand_qsub ()
{
  return expand_via_signed_unpred_insn (SS_MINUS, US_MINUS);
}

/* Expand a call to svptrue.  */
rtx
function_expander::expand_ptrue ()
{
  unsigned int num_bytes = type_suffixes[m_fi.types[0]].elem_bytes;

  /* The type is svbool_t regardless of TYPE, thus for b8 we want
     { 1, 1, 1, 1, ... }, for b16 we want { 1, 0, 1, 0, ... }, etc.  */
  rtx_vector_builder builder (VNx16BImode, num_bytes, 1);
  builder.quick_push (const1_rtx);
  for (unsigned int i = 1; i < num_bytes; ++i)
    builder.quick_push (const0_rtx);
  return builder.build ();
}

/* Expand a call to svsub or svsubr; REVERSED_P says which.  */
rtx
function_expander::expand_sub (bool reversed_p)
{
  unsigned int merge_argno = 1;
  if (reversed_p)
    {
      std::swap (m_args[1], m_args[2]);
      merge_argno = 2;
    }

  machine_mode mode = get_mode (0);
  if (try_negating_argument (2, mode))
    return expand_add (merge_argno);

  if (m_fi.pred == PRED_x)
    {
      if (type_suffixes[m_fi.types[0]].integer_p)
	return expand_via_unpred_direct_optab (sub_optab);
      else
	{
	  /* Try to take advantage of unpredicated FP addition, rather than
	     simply treating _x as _m.  */
	  insn_code icode = code_for_aarch64_pred_sub (mode);
	  return expand_via_pred_x_insn (icode);
	}
    }
  return expand_via_pred_direct_optab (cond_sub_optab, merge_argno);
}

/* Implement the call using optab OP, which is an unpredicated direct
   (i.e. single-mode) optab.  The last NSCALAR inputs are scalar, and
   map to scalar operands in the underlying instruction.  */
rtx
function_expander::expand_via_unpred_direct_optab (optab op,
						   unsigned int nscalar)
{
  machine_mode mode = get_mode (0);
  insn_code icode = direct_optab_handler (op, mode);
  return expand_via_unpred_insn (icode, nscalar);
}

/* Implement the call using instruction ICODE.  The last NSCALAR inputs
   are scalar, and map to scalar operands in the underlying instruction.  */
rtx
function_expander::expand_via_unpred_insn (insn_code icode,
					   unsigned int nscalar)
{
  /* Discount the output operand.  */
  unsigned int nops = insn_data[icode].n_operands - 1;
  /* Drop the predicate argument in the case of _x predication.  */
  unsigned int bias = (m_fi.pred == PRED_x ? 1 : 0);
  machine_mode mode = get_mode (0);
  unsigned int i = 0;

  add_output_operand (mode);

  for (; i < nops - nscalar; ++i)
    add_input_operand (m_args[i + bias], mode);
  for (; i < nops; ++i)
    add_input_operand (m_args[i + bias], GET_MODE_INNER (mode));

  return generate_insn (icode);
}

/* Implement the call using optab OP, which is a predicated direct
   (i.e. single-mode) optab.  Merging forms of OP use argument MERGE_ARGNO
   as the fallback value.  */
rtx
function_expander::expand_via_pred_direct_optab (optab op,
						 unsigned int merge_argno)
{
  machine_mode mode = get_mode (0);
  insn_code icode = direct_optab_handler (op, mode);
  return expand_via_pred_insn (icode, 0, merge_argno, false);
}

/* Implement the call using instruction ICODE.  The instruction takes
   The last NSCALAR inputs are scalar, and map to scalar operands
   in the underlying instruction.  Merging forms use argument MERGE_ARGNO
   as the fallback value.  If PRED_LAST_P is true, predicated register is
   at the end.  */
rtx
function_expander::expand_via_pred_insn (insn_code icode,
					 unsigned int nscalar,
					 unsigned int merge_argno,
					 bool pred_last_p)
{
  /* Discount the output, predicate, and fallback value.  */
  unsigned int nops = insn_data[icode].n_operands - 3;
  machine_mode mode = get_mode (0);
  machine_mode pred_mode = get_pred_mode (0);

  add_output_operand (mode);
  if (nops == 1 && m_fi.pred == PRED_m)
    {
      /* For unary ops, the fallback value is provided by a separate
	 argument that is passed before the governing predicate.  */
      /* If the predicate should go first.  */
      if (!pred_last_p)
	add_input_operand (m_args[1], pred_mode);
      /* If the only input is vector or scalar.  */
      if (nscalar)
	add_input_operand (m_args[2], GET_MODE_INNER (mode));
      else
	add_input_operand (m_args[2], mode);
      add_input_operand (m_args[0], mode);
      /* If the predicate should go last.  */
      if (pred_last_p)
	add_input_operand (m_args[1], pred_mode);
    }
  else
    {
      unsigned int i = 0;
      /* If the predicate should go first.  */
      if (!pred_last_p)
	add_input_operand (m_args[0], pred_mode);
      /* First vector inputs.  */
      for (; i < nops - nscalar; ++i)
	add_input_operand (m_args[i + 1], mode);
      /* Rest are scalar.  */
      for (; i < nops; ++i)
	add_input_operand (m_args[i + 1], GET_MODE_INNER (mode));
      if (m_fi.pred == PRED_z)
	/* Use zero as the fallback value.  */
	add_input_operand (CONST0_RTX (mode), mode);
      else
	/* Use the first data input as the fallback value.  */
	add_input_operand (copy_rtx (m_ops[merge_argno + 1].value), mode);
      /* If the predicate should go last.  */
      if (pred_last_p)
	add_input_operand (m_args[0], pred_mode);
    }
  return generate_insn (icode);
}

/* Implement the call using instruction ICODE, which is a predicated
   operation that returns arbitrary values for inactive lanes.  */
rtx
function_expander::expand_via_pred_x_insn (insn_code icode)
{
  unsigned int nops = m_args.length () - 1;
  machine_mode mode = get_mode (0);
  machine_mode pred_mode = get_pred_mode (0);

  /* Add the normal operands.  */
  add_output_operand (mode);
  add_input_operand (m_args[0], pred_mode);
  for (unsigned int i = 0; i < nops; ++i)
    add_input_operand (m_args[i + 1], mode);

  /* Add a flag that indicates whether unpredicated instructions
     are allowed.  */
  rtx pred = m_ops[1].value;
  if (FLOAT_MODE_P (mode))
    {
      if (flag_trapping_math
	  && pred != CONST1_RTX (pred_mode))
	add_integer_operand (SVE_FORBID_NEW_FAULTS);
      else
	add_integer_operand (SVE_ALLOW_NEW_FAULTS);
    }

  return generate_insn (icode);
}

/* Implement the call using an @aarch64_cond instruction for _x
   predication and a @cond instruction for _z and _m predication.
   The integer instructions are parameterized by an rtx_code while
   the floating-point instructions are parameterized by an unspec code.
   CODE_FOR_SINT is the rtx_code for signed integer operations,
   CODE_FOR_UINT is the rtx_code for unsigned integer operations
   and UNSPEC_COND is the unspec code for floating-point operations.  */
rtx
function_expander::expand_signed_pred_op (rtx_code code_for_sint,
					  rtx_code code_for_uint,
					  int unspec_cond)
{
  insn_code icode;

  if (m_fi.pred == PRED_x)
    {
      if (type_suffixes[m_fi.types[0]].integer_p)
	{
	  if (type_suffixes[m_fi.types[0]].unsigned_p)
	    icode = code_for_aarch64_pred (code_for_uint, get_mode (0));
	  else
	    icode = code_for_aarch64_pred (code_for_sint, get_mode (0));
	}
      else
	icode = code_for_aarch64_pred (unspec_cond, get_mode (0));
      return expand_via_pred_x_insn (icode);
    }
  else
    {
      if (type_suffixes[m_fi.types[0]].integer_p)
	{
	  if (type_suffixes[m_fi.types[0]].unsigned_p)
	    icode = code_for_cond (code_for_uint, get_mode (0));
	  else
	    icode = code_for_cond (code_for_sint, get_mode (0));
	}
      else
	icode = code_for_cond (unspec_cond, get_mode (0));
      return expand_via_pred_insn (icode, 0, 1, false);
    }
}

/* Implement the call using an @aarch64_cond instruction for _x
   predication and a @cond instruction for _z and _m predication.
   The instructions are parameterized by an unspec.
   UNSPEC_FOR_SINT is the unspec code for signed integer operations,
   UNSPEC_FOR_UINT is the unspec code for unsigned integer operations
   and UNSPEC_FOR_FP is the unspec code for floating-point operations.  */
rtx
function_expander::expand_signed_pred_op (int unspec_for_sint,
					  int unspec_for_uint,
					  int unspec_for_fp)
{
  insn_code icode;

  if (m_fi.pred == PRED_x)
    {
      if (type_suffixes[m_fi.types[0]].integer_p)
	{
	  if (type_suffixes[m_fi.types[0]].unsigned_p)
	    icode = code_for_aarch64_pred (unspec_for_uint, get_mode (0));
	  else
	    icode = code_for_aarch64_pred (unspec_for_sint, get_mode (0));
	}
      else
	icode = code_for_aarch64_pred (unspec_for_fp, get_mode (0));
      return expand_via_pred_x_insn (icode);
    }
  else
    {
      if (type_suffixes[m_fi.types[0]].integer_p)
	{
	  if (type_suffixes[m_fi.types[0]].unsigned_p)
	    icode = code_for_cond (unspec_for_uint, get_mode (0));
	  else
	    icode = code_for_cond (unspec_for_sint, get_mode (0));
	}
      else
	icode = code_for_cond (unspec_for_fp, get_mode (0));
      return expand_via_pred_insn (icode, 0, 1, false);
    }
}

/* Implement the call using an @aarch64 instruction and the
   instructions are parameterized by an rtx_code.  CODE_FOR_SINT
   is the rtx_code for signed integer operations, CODE_FOR_UINT
   is the rtx_code for unsigned integer operations.  */
rtx
function_expander::expand_via_signed_unpred_insn (rtx_code code_for_sint,
						  rtx_code code_for_uint)
{
  insn_code icode;
  if (type_suffixes[m_fi.types[0]].unsigned_p)
    icode = code_for_aarch64 (code_for_uint, code_for_uint, get_mode (0));
  else
    icode = code_for_aarch64 (code_for_sint, code_for_sint, get_mode (0));
  return expand_via_unpred_insn (icode);
}

/* Expand a call to a SHAPE_shift_right_imm function using predicated
   instruction ICODE, which has the same operand order as conditional
   optabs like cond_add_optab.  */
rtx
function_expander::expand_pred_shift_right_imm (insn_code icode)
{
  require_immediate_range (2, 1, GET_MODE_UNIT_BITSIZE (get_mode (0)));
  return expand_via_pred_insn (icode, 0, 1, false);
}

/* Require that argument ARGNO is a constant integer in the range
   [MIN, MAX].  Report an appropriate error if it isn't and set
   the argument to a safe in-range value.  */
void
function_expander::require_immediate_range (unsigned int argno,
					    HOST_WIDE_INT min,
					    HOST_WIDE_INT max)
{
  if (!CONST_INT_P (m_args[argno]))
    report_non_ice (m_location, m_rfn.decl, argno);
  else
    {
      HOST_WIDE_INT actual = INTVAL (m_args[argno]);
      if (IN_RANGE (actual, min, max))
	return;
      report_out_of_range (m_location, m_rfn.decl, argno, actual, min, max);
    }
  m_args[argno] = GEN_INT (min);
}

/* Return true if argument I is a constant argument that can be negated
   at compile time, replacing it with the negated value if so.  MODE is the
   associated vector mode, but the argument could be a single element.  */
bool
function_expander::try_negating_argument (unsigned int i, machine_mode mode)
{
  rtx x = m_args[i];
  if (!VECTOR_MODE_P (GET_MODE (x)))
    mode = GET_MODE_INNER (mode);

  x = simplify_unary_operation (NEG, mode, x, mode);
  if (!x)
    return false;

  m_args[i] = x;
  return true;
}

/* Return the mode associated with type suffix I.  */
machine_mode
function_expander::get_mode (unsigned int i)
{
  return TYPE_MODE (m_fi.vector_type (i));
}

/* Return the predicate mode associated with type suffix I.  */
machine_mode
function_expander::get_pred_mode (unsigned int i)
{
  unsigned int elem_bytes = type_suffixes[m_fi.types[i]].elem_bytes;
  return aarch64_sve_pred_mode (elem_bytes).require ();
}

/* Add an output operand of mode MODE to the instruction, binding it
   to the preferred target rtx if possible.  */
void
function_expander::add_output_operand (machine_mode mode)
{
  m_ops.safe_grow (m_ops.length () + 1);
  create_output_operand (&m_ops.last (), m_target, mode);
}

/* Add an input operand with mode MODE to the instruction.  Calculate
   the value of the operand as follows:

   - If MODE is a vector and X is not, broadcast X to fill a vector of
     mode MODE.

   - Otherwise, if MODE is a predicate mode, coerce X to have that mode.
     In this case X is known to be VNx16BImode.

   - Otherwise use X directly.  The expand machinery checks that X is
     consistent with MODE.  */
void
function_expander::add_input_operand (rtx x, machine_mode mode)
{
  if (!VECTOR_MODE_P (GET_MODE (x)) && VECTOR_MODE_P (mode))
    x = expand_vector_broadcast (mode, x);
  else if (GET_MODE_CLASS (mode) == MODE_VECTOR_BOOL)
    {
      gcc_assert (GET_MODE (x) == VNx16BImode);
      x = gen_lowpart (mode, x);
    }
  m_ops.safe_grow (m_ops.length () + 1);
  create_input_operand (&m_ops.last (), x, mode);
}

/* Add an integer operand with value X to the instruction.  */
void
function_expander::add_integer_operand (HOST_WIDE_INT x)
{
  m_ops.safe_grow (m_ops.length () + 1);
  create_integer_operand (&m_ops.last (), x);
}

/* Generate instruction ICODE, given that its operands have already
   been added to M_OPS.  Return the value of the first operand.  */
rtx
function_expander::generate_insn (insn_code icode)
{
  expand_insn (icode, m_ops.length (), m_ops.address ());
  return m_ops[0].value;
}

/* Expand a call to the SVE function with subcode CODE.  EXP is the call
   expression and TARGET is the preferred location for the result.
   Return the value of the lhs.  */
rtx
expand_builtin (unsigned int code, tree exp, rtx target)
{
  return function_expander (code, exp, target).expand ();
}

}

using namespace aarch64_sve;

#include "gt-aarch64-sve-builtins.h"
