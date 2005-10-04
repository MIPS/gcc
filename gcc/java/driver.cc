// Initialization and compiler driving.

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

#include "java/glue.hh"

#include "bytecode/bytegen.hh"
#include "java/treegen.hh"
#include "header/cni.hh"
#include "source/ucs2.hh"

#ifndef TARGET_OBJECT_SUFFIX
# define TARGET_OBJECT_SUFFIX ".o"
#endif

// An object of this type is used to hold command line argument
// information.
struct arg_info
{
  // Classpath-related information.
  const char *bootclasspath;
  const char *classpath;
  std::list<std::string> dash_i_args;

  // Encoding for source files.
  const char *encoding;

  // Output directory.
  const char *output;

  // Input file name.
  const char *filename;

  // If --resource was given, the resource name.  If this is NULL then
  // we are not compiling a resource file.
  const char *resource_name;

  // Dependency tracking structure.
  struct deps *dependencies;
  // True if we should print phony targets.
  bool print_phonies;
  // True if we should include system files in dependency output.
  bool include_system_files;
  // True if we should write dependencies to a file (instead of to
  // stdout).
  bool deps_to_file;
  // Name of output file.  If not specified, and DEPS_TO_FILE is true,
  // then we compute the name of the file.
  const char *dependency_file;
  // Name of target to use, or NULL for the default.
  const char *target;

  arg_info ()
    : bootclasspath (NULL),
      classpath (NULL),
      encoding (NULL),
      output (NULL),
      filename (NULL),
      resource_name (NULL),
      dependencies (NULL),
      print_phonies (false),
      include_system_files (false),
      deps_to_file (false),
      dependency_file (NULL),
      target (NULL)
  {
  }

  ~arg_info ()
  {
    if (dependencies)
      deps_free (dependencies);
  }

  void init_dependencies (bool system)
  {
    if (! dependencies)
      dependencies = deps_init ();
    include_system_files = system;
  }
};


// With GCC we only have a single compiler.
static compiler *our_compiler;

// Global state for argument parsing.
static arg_info *arguments;


// Process java-specific compiler command-line options.  Return 0, but
// do not complain if the option is not recognized.
int
gcjx::handle_option (size_t scode, const char *arg, int value)
{
  assert (our_compiler);
  assert (arguments);

  enum opt_code code = (enum opt_code) scode;

  switch (code)
    {
    case OPT_I:
      arguments->dash_i_args.push_back (arg);
      break;

    case OPT_M:
      arguments->init_dependencies (true);
      break;

    case OPT_MD_:
      arguments->init_dependencies (true);
      arguments->deps_to_file = true;
      break;

    case OPT_MF:
      arguments->dependency_file = arg;
      break;

    case OPT_MM:
      arguments->init_dependencies (false);
      break;

    case OPT_MMD_:
      arguments->init_dependencies (false);
      arguments->deps_to_file = true;
      break;

    case OPT_MP:
      arguments->print_phonies = true;
      break;

    case OPT_MT:
      arguments->target = arg;
      break;

    case OPT_Wall:
      // Do nothing for -Wno-all.  FIXME: clear it instead?
      if (value)
	our_compiler->set_wall (WARN);
      break;

    case OPT_Wassert:
      our_compiler->set_warning (WARN_ASSERT, value ? WARN : OFF);
      break;

    case OPT_Wenum:
      our_compiler->set_warning (WARN_ENUM, value ? WARN : OFF);
      break;

    case OPT_Wcanonical_modifier_order:
      our_compiler->set_warning (WARN_CANONICAL_MODIFIER_ORDER,
				 value ? WARN : OFF);
      break;

    case OPT_Wredundant_modifier:
      our_compiler->set_warning (WARN_REDUNDANT_MODIFIER, value ? WARN : OFF);
      break;

    case OPT_Wdeprecated:
      our_compiler->set_warning (WARN_DEPRECATED, value ? WARN : OFF);
      break;

    case OPT_Wjava_lang_import:
      our_compiler->set_warning (WARN_JAVA_LANG_IMPORT, value ? WARN : OFF);
      break;

    case OPT_Wunused_import:
      our_compiler->set_warning (WARN_UNUSED_IMPORT, value ? WARN : OFF);
      break;

    case OPT_Wnon_static_context:
      our_compiler->set_warning (WARN_NON_STATIC_CONTEXT, value ? WARN : OFF);
      break;

    case OPT_Wdollar_identifier:
      our_compiler->set_warning (WARN_DOLLAR_IDENTIFIER, value ? WARN : OFF);
      break;

    case OPT_Wascii_escape:
      our_compiler->set_warning (WARN_ASCII_ESCAPE, value ? WARN : OFF);
      break;

    case OPT_Wconstructor_name:
      our_compiler->set_warning (WARN_CONSTRUCTOR_NAME, value ? WARN : OFF);
      break;

    case OPT_Wfinalize_spelling:
      our_compiler->set_warning (WARN_FINALIZE_SPELLING, value ? WARN : OFF);
      break;

    case OPT_Wassignment_no_effect:
      our_compiler->set_warning (WARN_ASSIGNMENT_NO_EFFECT,
				 value ? WARN : OFF);
      break;

    case OPT_Wstring_plus_char_array:
      our_compiler->set_warning (WARN_STRING_PLUS_CHAR_ARRAY,
				 value ? WARN : OFF);
      break;

    case OPT_Wfinally_abnormal_completion:
      our_compiler->set_warning (WARN_FINALLY_ABNORMAL_COMPLETION,
				 value ? WARN : OFF);
      break;

    case OPT_Wunneeded_instanceof:
      our_compiler->set_warning (WARN_UNNEEDED_INSTANCEOF, value ? WARN : OFF);
      break;

    case OPT_Wunused_private_members:
      our_compiler->set_warning (WARN_UNUSED_PRIVATE_MEMBERS,
				 value ? WARN : OFF);
      break;

    case OPT_Wbad_serialization_field:
      our_compiler->set_warning (WARN_BAD_SERIALIZATION_FIELD,
				 value ? WARN : OFF);
      break;

    case OPT_Wunneeded_else:
      our_compiler->set_warning (WARN_UNNEEDED_ELSE, value ? WARN : OFF);
      break;

    case OPT_Wbitwise_boolean:
      our_compiler->set_warning (WARN_BITWISE_BOOLEAN, value ? WARN : OFF);
      break;

    case OPT_Wunused_label:
      our_compiler->set_warning (WARN_UNUSED_LABEL, value ? WARN : OFF);
      break;

    case OPT_Wunneeded_extends:
      our_compiler->set_warning (WARN_UNNEEDED_EXTENDS, value ? WARN : OFF);
      break;

    case OPT_Warray_size:
      our_compiler->set_warning (WARN_ARRAY_SIZE, value ? WARN : OFF);
      break;

    case OPT_Wenclosing_access:
      our_compiler->set_warning (WARN_ENCLOSING_ACCESS, value ? WARN : OFF);
      break;

    case OPT_Wzero_shift:
      our_compiler->set_warning (WARN_ZERO_SHIFT, value ? WARN : OFF);
      break;

    case OPT_Wnegative_shift:
      our_compiler->set_warning (WARN_NEGATIVE_SHIFT, value ? WARN : OFF);
      break;

    case OPT_Wlarge_shift:
      our_compiler->set_warning (WARN_LARGE_SHIFT, value ? WARN : OFF);
      break;

    case OPT_Werror:
      our_compiler->warnings_are_errors = true;
      break;

    case OPT_fassert:
      our_compiler->feature_assert = value;
      break;

    case OPT_fenum:
      our_compiler->feature_enum = value;
      break;

    case OPT_fstatic_import:
      our_compiler->feature_static_import = value;
      break;

    case OPT_fenhanced_for:
      our_compiler->feature_enhanced_for = value;
      break;

    case OPT_fgenerics:
      our_compiler->feature_generics = value;
      break;

    case OPT_fboxing_conversion:
      our_compiler->feature_boxing_conversion = value;
      break;

    case OPT_fvarargs:
      our_compiler->feature_varargs = value;
      break;

    case OPT_fannotations:
      our_compiler->feature_annotations = value;
      break;

    case OPT_fgenerate_assert:
      our_compiler->target_assert = value;
      break;

    case OPT_fverify:
      our_compiler->target_verify = value;
      break;

    case OPT_fenable_assertions_:
      // add_enable_assert (arg, value);
      break;

    case OPT_fenable_assertions:
      // add_enable_assert ("", value);
      break;

    case OPT_fdisable_assertions_:
      // add_enable_assert (arg, !value);
      break;

    case OPT_fdisable_assertions:
      // add_enable_assert ("", !value);
      break;

    case OPT_fbootclasspath_:
      arguments->bootclasspath = arg;
      break;

    case OPT_fclasspath_:
      arguments->classpath = arg;
      break;

    case OPT_fcompile_resource_:
      arguments->resource_name = arg;
      break;

    case OPT_fdump_:
//       if (!dump_switch_p (arg))
// 	return 0;
      break;

    case OPT_fencoding_:
      arguments->encoding = arg;
      break;

    case OPT_fextdirs_:
      // jcf_path_extdirs_arg (arg);
      break;

    case OPT_finline_functions:
      flag_inline_functions = value;
      break;

    case OPT_foutput_class_dir_:
      arguments->output = arg;
      break;

    case OPT_version:
      // v_flag = 1;
      break;
      
    default:
      // Other Java options are assumed to be handled by our caller.
      // Non-Java options are a bug.
      assert ((cl_options[code].flags & CL_Java) != 0);
      break;
    }

  return 1;
}

unsigned int
gcjx::init_options (unsigned int, const char **)
{
  // Initialize libgcjx.  FIXME: poor naming.
  initialize_primitive_types ();

  // Set some flags that might be modified by command-line options.
  flag_bounds_check = 1;
  // FIXME: not sure if the user can reasonably change these two.
  // If not, should move to java_init().
  flag_exceptions = 1;
  flag_non_call_exceptions = 1;

  arguments = new arg_info ();
  our_compiler = new compiler ("gcj");

  return CL_Java;
}

static void
add_cp (std::list<class_factory *> &facs, const std::list<std::string> &path)
{
  for (std::list<std::string>::const_iterator i = path.begin ();
       i != path.end ();
       ++i)
    {
      class_factory *a_factory = class_factory::get_class_factory (*i, false);
      if (a_factory != NULL)
        facs.push_back (a_factory);
    }
}

bool
gcjx::post_options (const char **filename_ptr)
{
  assert (*filename_ptr);
  arguments->filename = *filename_ptr;
  
  global->set_compiler (our_compiler);

  // FIXME: GCC_EXEC_PREFIX handling.

  // Compute the class path.
  std::list<class_factory *> facs;
  if (arguments->bootclasspath != NULL)
    add_cp (facs, split (arguments->bootclasspath, ':'));
  else
    {
      std::list<std::string> bc;
      bc.push_back (LIBGCJ_ZIP_FILE);
      add_cp (facs, bc);
    }
  if (arguments->classpath != NULL)
    add_cp (facs, split (arguments->classpath, ':'));
  add_cp (facs, arguments->dash_i_args);
  if (arguments->bootclasspath == NULL
      && arguments->classpath == NULL
      && arguments->dash_i_args.empty ()
      && getenv ("CLASSPATH") != NULL)
    add_cp (facs, split (getenv ("CLASSPATH"), ':'));
  // We need an empty item to help us find absolute paths.
  add_cp (facs, split ("", ':'));

  our_compiler->set_class_factory (new classpath_class_factory (facs));

  // Compute the output directory.
  if (arguments->output == NULL)
    arguments->output = ".";
  our_compiler->set_output_directory (arguments->output);

  // Compute the source file encoding.
  our_compiler->set_encoding (arguments->encoding ? arguments->encoding
			      : compute_default_encoding ());

  // Set up code generators.  Note that JNI headers and stubs are
  // handled via a separate main().
  directory_cache &dircache (our_compiler->get_directory_cache ());
  // FIXME: should be possible to generate class files, headers, and
  // object code all at once
  if (flag_emit_class_files)
    {
      our_compiler->add_code_generator (new bytecode_code_generator (our_compiler,
								     dircache));
      if (flag_emit_cni_headers)
	our_compiler->add_code_generator (new cni_code_generator (our_compiler,
								  dircache));
    }
  else if (! flag_syntax_only)
    our_compiler->add_code_generator (new tree_code_generator (our_compiler,
							       dircache));
  return false;
}

bool
gcjx::init ()
{
  // In Java floating point operations never trap.
  flag_trapping_math = 0;

  // In Java arithmetic overflow always wraps around.
  flag_wrapv = 1;

  // Java requires left-to-right evaluation of subexpressions.
  flag_evaluation_order = 1;

  if (flag_inline_functions)
    flag_inline_trees = 1;

  // Force minimum function alignment if g++ uses the least
  // significant bit of function pointers to store the virtual
  // bit. This is required to keep vtables compatible.
  if (TARGET_PTRMEMFUNC_VBIT_LOCATION == ptrmemfunc_vbit_in_pfn
      && force_align_functions_log < 1)
    force_align_functions_log = 1;

  initialize_decls ();
  using_eh_for_cleanups ();

  return true;
}

void
gcjx::finish ()
{
  // Write out the dependencies.
  if (arguments->dependencies)
    {
      FILE *dep_file;
      if (! arguments->deps_to_file)
	dep_file = stdout;
      else
	{
	  std::string dep_filename;
	  if (arguments->dependency_file)
	    dep_filename = arguments->dependency_file;
	  else
	    {
	      const char *filename = arguments->filename;
	      const char *dot = strrchr (filename, '.');
	      if (dot == NULL)
		error ("couldn't determine target name for "
		       "dependency tracking");
	      else
		{
		  char *buf
		    = (char *) xmalloc (dot - filename
					+ 3 + sizeof (TARGET_OBJECT_SUFFIX));
		  strncpy (buf, filename, dot - filename);
		  // FIXME: special handling when generating .class.
		  // FIXME: blah blah
		  if (arguments->target == NULL)
		    {
		      strcpy (buf + (dot - filename), TARGET_OBJECT_SUFFIX);
		      deps_add_default_target (arguments->dependencies, buf);
		    }

		  buf[dot - filename] = '\0';
		  dep_filename = buf + std::string (".d");
		}
	    }

	  dep_file = fopen (dep_filename.c_str (), "w");
	  // FIXME: error handling.
	}

      // Add the dependencies.
      std::set<std::string> files = our_compiler->get_all_files_read ();
      for (std::set<std::string>::const_iterator i = files.begin ();
	   i != files.end ();
	   ++i)
	deps_add_dep (arguments->dependencies, (*i).c_str ());

      // Set the target.
      if (arguments->target != NULL)
	deps_add_target (arguments->dependencies, arguments->target, 1);

      // Write out the results.
      deps_write (arguments->dependencies, dep_file, 72);
      if (arguments->print_phonies)
	deps_phony_targets (arguments->dependencies, dep_file);

      if (dep_file != stdout)
	fclose (dep_file);
    }

  // We're done with the command-line arguments.
  delete arguments;
  arguments = NULL;

  global->set_compiler (NULL);
  delete our_compiler;
  our_compiler = NULL;
}

// Write out code to register classes we defined.
// The preferred mechanism is through the .jcr section, which contain
// a list of pointers to classes which get registered during constructor
// invocation time.
static void
register_classes (tree classes)
{
  if (TARGET_USE_JCR_SECTION)
    {
#ifdef JCR_SECTION_NAME
      named_section_flags (JCR_SECTION_NAME, SECTION_WRITE);
      assemble_align (POINTER_SIZE);
      for (tree iter = classes; iter != NULL_TREE; iter = TREE_CHAIN (iter))
	{
	  mark_decl_referenced (TREE_VALUE (iter));
	  assemble_integer (XEXP (DECL_RTL (TREE_VALUE (iter)), 0),
			    POINTER_SIZE / BITS_PER_UNIT, POINTER_SIZE, 1);
	}
#else
      // A target has defined TARGET_USE_JCR_SECTION, but doesn't have
      // a JCR_SECTION_NAME.
      abort ();
#endif
    }
  else
    {
      abort ();
//       tree klass, t, register_class_fn;

//       t = build_function_type_list (void_type_node, class_ptr_type, NULL);
//       t = build_decl (FUNCTION_DECL, get_identifier ("_Jv_RegisterClass"), t);
//       TREE_PUBLIC (t) = 1;
//       DECL_EXTERNAL (t) = 1;
//       register_class_fn = t;

//       for (klass = registered_class; klass; klass = TREE_CHAIN (klass))
// 	{
// 	  t = build_fold_addr_expr (klass);
// 	  t = tree_cons (NULL, t, NULL);
// 	  t = build_function_call_expr (register_class_fn, t);
// 	  append_to_statement_list (t, list_p);
// 	}
    }
}

// This handles the processing for a single file.  It feeds the
// contents of the file to the compiler as appropriate.  For instance,
// ".java" files are parsed and marked for code generation.
static void
process_one_file (const std::string &filename)
{
  if (arguments->resource_name)
    compile_resource_file (arguments->resource_name, filename.c_str ());
  else
    our_compiler->load_source_file (filename);
}

void
gcjx::parse_file (int)
{
  // Parse the source file.  If an "@-file" was specified, read it and
  // parse all the source files.
  if (flag_filelist_file)
    {
      std::list<std::string> names;
      try
	{
	  read_at_file (arguments->filename, names);
	}
      catch (io_error &ex)
	{
	  // FIXME: use fatal_error here?  Error handling in general
	  // needs some help.
	  std::cerr << ex;
	  exit (1);
	}
      for (std::list<std::string>::const_iterator i = names.begin ();
	   i != names.end ();
	   ++i)
	process_one_file (*i);
    }
  else
    process_one_file (arguments->filename);

  // Compile, then generate code if everything went ok.
  if (our_compiler->semantic_analysis ())
    our_compiler->generate_code ();

  register_classes (class_list);

  cgraph_finalize_compilation_unit ();
  cgraph_optimize ();
}
