// Main program, at least for testing.

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
#include "bytecode/bytegen.hh"
#include "header/jni.hh"
#include "header/jnistub.hh"
#include "header/cni.hh"

#include <string.h>
#include <stdlib.h>

#include "source/tstream.hh"
#include "source/parse.hh"

typedef compiler_flag compiler::*CFLAG;

struct flag_and_name
{
  const char *name;
  CFLAG flag;
  const char *desc;
};

static flag_and_name features[] =
{
  { "assert", &compiler::feature_assert, "accept the 'assert' feature" },
  { "enum", &compiler::feature_enum, "accept the 'enum' feature" },
  { "static-import", &compiler::feature_static_import,
    "accept the 'static import' feature" },
  { "enhanced-for", &compiler::feature_enhanced_for,
    "accept the enhanced 'for' loop" },
  { "generics", &compiler::feature_generics, "accept generics" },
  { "boxing-conversion", &compiler::feature_boxing_conversion,
    "accept boxing conversion"},
  { "varargs", &compiler::feature_varargs, "accept variable-arity methods" },
  { "annotations", &compiler::feature_annotations, "accept annotations" },
  { "generate-assert", &compiler::target_assert,
    "generate code for 'assert'" },
  { "verify", &compiler::target_verify,
    "verify bytecode after creating it" },
  { "dump-methods", &compiler::dump_methods,
    "print method bodies to stdout (for debugging)" },
  { "dump-tree", &compiler::dump_tree,
    "print abstract syntax tree to stdout (for debugging)" },
  { NULL, NULL, NULL }
};



static command_line_error
make_error (const char *message)
{
  return command_line_error (LOCATION_UNKNOWN, message);
}

static void
die_unrecognized (const std::string &kind, const std::string &example)
{
  throw make_error ("unrecognized %u1 %2") % kind % example;
}

static void
no_arg (const std::string &name)
{
  throw make_error ("argument required for option %1") % name;
}



static void
print (const char *opt, const char *desc, int tabs, const char *pref = NULL)
{
  if (! pref)
    pref = "";
  std::cout << "  " << pref << opt;

  int w = strlen (pref) + strlen (opt);
  for (int i = 2; i < 8 * tabs - w; i += 8)
    std::cout << "\t";

  std::cout << desc << "\n";
}



class argument_parser
{
public:
  argument_parser (char *argv[])
    : comp (global->get_compiler ()),
      cni_generator (NULL)
  {
    for (; *argv; ++argv)
      {
        std::string arg (*argv);
        if (arg[0] == '@')
          read_at_file (arg.substr (1));
        else
          args.push_back (*argv);
      }
  }

  void
  read_at_file (const std::string &filename)
  {
    try
      {
        std::list<std::string> names;
        ::read_at_file (filename.c_str (), names);
        std::copy (names.begin (), names.end (), std::back_inserter (args));
      }
    catch (io_error &ex)
      {
        std::cerr << ex;
        exit (1);
      }
  }

  void
  handle_warning (const std::string &arg)
  {
    warning_state state = (arg[1] == 'W' ? WARN : ERROR);
    const char *name = arg.c_str ();
    if (arg.size () > 5 && ! strncmp (name, "-Wno-", 5))
      {
	state = OFF;
	name += 5;
      }
    else
      name += 2;
    warning_flag flag = warning_scope::find_warning (name);
    if (flag == WARN_NONE)
      die_unrecognized ("warning", arg);
    comp->set_warning (flag, state);
  }

  void
  handle_feature (const std::string &arg)
  {
    bool state = true;
    std::string name = arg.substr (2);
    if (name.find ("no-") == 0)
      {
        name = name.substr (3);
        state = false;
      }

    for (int i = 0; features[i].name; ++i)
      {
        if (name == features[i].name)
	  {
	    comp->*(features[i].flag) = state;
	    return;
	  }
      }
    die_unrecognized ("feature", arg);
  }

  void
  add_classpath (std::list<class_factory *> &facs, const std::string &path)
  {
    add_path (facs, path, false);
  }

  void
  add_sourcepath (std::list<class_factory *> &facs, const std::string &path)
  {
    add_path (facs, path, true);
  }

  void
  add_path (std::list<class_factory *> &facs, const std::string &path,
	    bool source_only)
  {
    std::list<std::string> split_path (split (path, ':'));
    add_path (facs, split_path, source_only);
  }
  
  void
  add_path (std::list<class_factory *> &facs, std::list<std::string> &path,
	    bool source_only)
  {
    for (std::list<std::string>::const_iterator i = path.begin ();
         i != path.end ();
         ++i)
      {
        class_factory *a_factory
	  = class_factory::get_class_factory (*i, source_only);
        if (a_factory != NULL)
          facs.push_back (a_factory);
      }
  }

  void
  help_warnings ()
  {
    std::ostream &os = std::cout;
    os << "gcjx supports a number of warnings." << std::endl
       << "Each warning can be enabled with '-Wname'" << std::endl
       << "and turned into an error with '-Ename'." << std::endl
       << "-Wno-name can be used to disable a warning or error" << std::endl
       << std::endl
       << "-Wall and -Eall turn on a set of generally suitable warnings"
       << std::endl
       << std::endl;

    warning_scope::print_warning_help (std::cout);

    exit (0);
  }

  void
  help_options ()
  {
    std::ostream &os = std::cout;
    os << "Usage: gcjx [OPTIONS] FILE..." << std::endl
       << "  A file whose name starts with '@' holds arguments (which can be\n"
       << "  any mix of options and files), separated by either spaces or\n"
       << "  newlines."
       << std::endl
       << std::endl;

    print ("-bootclasspath PATH", "set boot classpath", 3);
    print ("-classpath PATH", "set user classpath (default '.')", 3);
    print ("-IDIR", "append DIR to class path", 3);

    print ("-encoding NAME", "set source file encoding", 3);
    print ("-tabs WIDTH", "set tab width (default 8)", 3);
    print ("-d DIR", "set output directory", 3);
    print ("-deprecation", "like -Wdeprecated (javac compatibility)", 3);
    print ("-verbose", "be verbose", 3);
    print ("-error", "treat all warnings as errors", 3);
    print ("-pedantic", "be pedantically correct", 3);
    print ("-source 1.[345]", "choose source code compatibility", 3);
    print ("-target 1.[345]", "choose target VM compatibility", 3);
    print ("-g", "generate debugging information", 3);
    if (concurrence::available ())
      print ("-j N", "use N worker threads", 3);

    os << std::endl;

    print ("-o bytecode", "generate bytecode (default)", 3);
    print ("-o jni", "generate JNI headers", 3);
    print ("-o cni", "generate CNI headers", 3);
    print ("-o jnistub", "generate JNI stubs", 3);
    print ("-o none", "check syntax but do not generate code", 3);

    os << std::endl;

    print ("-cniclass CLASS", "set class for CNI text generators", 3);
    print ("-add TEXT", "print text in class body", 3);
    print ("-prepend TEXT", "print text before class declaration", 3);
    print ("-append CLASS", "print text after class declaration", 3);
    print ("-friend CLASS", "print text in class as friend declaration", 3);
    print ("-nostdheaders", "don't emit headers for Object or Class", 3);

    os << std::endl;

    print ("--help", "print help and exit", 3);
    print ("--help-warnings", "print help about warnings and exit", 3);
    print ("--version", "print version and exit", 3);

    os << std::endl;

    os << "Feature flags:" << std::endl;
    os << "    Each feature can also be deselected with '-fno-...'."
       << std::endl;
    for (int i = 0; features[i].name; ++i)
      print (features[i].name, features[i].desc, 3, "-f");

    exit (0);
  }

  void
  version ()
  {
    std::cout << "gcjx version -5.0" << std::endl;
    exit (0);
  }

  void
  set_source (const std::string &arg)
  {
    if (arg == "1.3")
      comp->set_source_1_3 ();
    else if (arg == "1.4")
      comp->set_source_1_4 ();
    else if (arg == "1.5")
      comp->set_source_1_5 ();
    else
      die_unrecognized ("source", arg);
    source_argument = arg;
  }

  void
  set_target (const std::string &arg)
  {
    if (arg == "1.3")
      comp->set_target_1_3 ();
    else if (arg == "1.4")
      comp->set_target_1_4 ();
    else if (arg == "1.5")
      comp->set_target_1_5 ();
    else
      die_unrecognized ("target", arg);
    target_argument = arg;
  }

  void
  set_tabs (const std::string &arg)
  {
    // FIXME: use strtol or something nice.
    int val = atoi (arg.c_str ());
    comp->set_tab_width (val);
  }
  
  bool
  is_form_of (std::deque<std::string>::iterator &it, const std::string &prefix,
	      bool use_equals = true)
  {
    std::string &arg = *it;
    
    // "-switch"
    if (arg == prefix)
      return true;

    // "-switch=..."
    if (strncmp (arg.c_str (), prefix.c_str (), prefix.length ()) != 0)
      return false;
    if (! use_equals)
      return true;
    size_t equals_pos = prefix.length ();
    if (arg.length () < equals_pos || arg[equals_pos] != '=')
      return false;
    return true;
  }
  
  std::string
  get_next_arg (std::deque<std::string>::iterator &it, const std::string &name)
  {
    if (++it == args.end ())
      no_arg (name);
    return *it;
  }
  
  std::string
  get_arg_for (std::deque<std::string>::iterator &it, const std::string &name,
	       bool use_equals = true)
  {
    std::string &first = *it;

    // -switch arg
    if (first == name)
      return get_next_arg (it, name);

    // -switch=arg
    return first.substr (name.length () + (use_equals ? 1 : 0));
  }

  void
  need_cni_generator (const std::string &name)
  {
    if (cni_generator == NULL)
      throw make_error ("no CNI header generator specified for option %1")
	% name;
  }

  void
  need_cni_class (const std::string &name)
  {
    if (action_class.empty ())
      throw make_error ("no %<-cniclass%> specified for option %1")
	% name;
  }

  std::deque<std::string>
  parse_args ()
  {
    std::string bootclasspath;
    std::string classpath;;
    std::string sourcepath;
    std::string output;
    
#ifdef BOOTCLASSPATH
    bootclasspath = BOOTCLASSPATH;
#endif

    std::deque<std::string> source_files;
    std::list<std::string> dash_i_args;
    std::set<std::string> outputs;
    std::string encoding = compute_default_encoding ();
    int n_threads = 0;

    std::deque<std::string>::iterator it = args.begin ();
    for (; it != args.end (); ++it)
      {
        std::string& arg (*it);

        // Non-arguments are just preserved.
        if (arg[0] != '-')
	  {
	    source_files.push_back (arg);
	    continue;
	  }

        if (arg == "--")
	  {
	    // All successive arguments are source files.
	    while (++it != args.end ())
	      source_files.push_back (*it);
	    break;
	  }

        if (arg == "-Wall" || arg == "-Eall")
	  comp->set_wall (arg[1] == 'W' ? WARN : ERROR);
        else if (arg[1] == 'W' || arg[1] == 'E')
	  {
	    // -W and -E options.
	    handle_warning (arg);
	  }
        else if (arg == "-deprecation")
          handle_warning ("-Wdeprecated");
        else if (arg[1] == 'f' && arg != "-friend")
	  handle_feature (arg);
        else if (arg == "-verbose")
	  comp->verbose = true;
        else if (arg == "-error")
	  comp->warnings_are_errors = true;
        else if (arg == "-pedantic")
	  comp->pedantic = true;
	else if (arg.length () >= 2 && ! strncmp (arg.c_str (), "-g" , 2))
	  {
	    // The real form is -g:none, or
	    // -g:{lines,vars,source}.
	    // Currently we don't differentiate this in the back end,
	    // so we just handle all-or-nothing.
	    comp->target_debug = (arg != "-g:none");
	  }
        else if (arg == "-d")
	  output = get_next_arg (it, arg);
	else if (is_form_of (it, "-bootclasspath"))
	  bootclasspath = get_arg_for (it, "-bootclasspath");
	else if (is_form_of (it, "-classpath"))
	  classpath = get_arg_for (it, "-classpath");
	else if (is_form_of (it, "-sourcepath"))
	  sourcepath = get_arg_for (it, "-sourcepath");
        else if (arg == "-o")
	  {
	    std::string otype (get_next_arg (it, arg));
	    if (outputs.find (otype) != outputs.end ())
	      throw make_error ("output type %1 already specified") % otype;

	    if (otype == "bytecode")
	      comp->add_code_generator (new bytecode_code_generator (comp,
								     comp->get_directory_cache ()));
	    else if (otype == "jni")
	      comp->add_code_generator (new jni_code_generator (comp,
							        comp->get_directory_cache ()));
	    else if (otype == "cni")
	      {
		cni_generator = new cni_code_generator (comp,
							comp->get_directory_cache ());
		comp->add_code_generator (cni_generator);
	      }
	    else if (otype == "jnistub")
	      comp->add_code_generator (new jni_stub_generator (comp,
							        comp->get_directory_cache ()));
	    else if (otype == "none")
	      {
		// Nothing.
	      }
	    else
	      die_unrecognized ("output type", otype);
	    outputs.insert (otype);
	  }
	else if (is_form_of (it, "-I", false))
	  dash_i_args.push_back (get_arg_for (it, "-I", false));
        else if (arg == "-j")
	  {
	    if (! concurrence::available ())
	      throw make_error ("threads not available on this platform");
	    std::string threads (get_next_arg (it, arg));
	    n_threads = atoi (threads.c_str ());
	  }
	else if (is_form_of (it, "-source"))
	  set_source (get_arg_for (it, "-source"));
        else if (is_form_of (it, "-target"))
	  set_target (get_arg_for (it, "-target"));
	else if (is_form_of (it, "-encoding"))
	  encoding = get_arg_for (it, "-encoding");
        else if (is_form_of (it, "-tabs"))
	  set_tabs (get_arg_for (it, "-tabs"));
	else if (arg == "-help" || arg == "--help")
	  help_options ();
        else if (arg == "-help-warnings" || arg == "--help-warnings")
	  help_warnings ();
        else if (arg == "-version" || arg == "--version")
	  version ();
	else if (arg == "-cniclass")
	  {
	    need_cni_generator ("-cniclass");
	    action_class = get_arg_for (it, "-cniclass");
	  }
	else if (arg == "-add")
	  {
	    need_cni_generator ("-add");
	    cni_generator->add_action (cni_code_generator::CNI_ADD,
				       action_class,
				       get_arg_for (it, "-add"));
	  }
	else if (arg == "-append")
	  {
	    need_cni_generator ("-append");
	    cni_generator->add_action (cni_code_generator::CNI_APPEND,
				       action_class,
				       get_arg_for (it, "-append"));
	  }
	else if (arg == "-friend")
	  {
	    need_cni_generator ("-friend");
	    cni_generator->add_action (cni_code_generator::CNI_FRIEND,
				       action_class,
				       get_arg_for (it, "-friend"));
	  }
	else if (arg == "-prepend")
	  {
	    need_cni_generator ("-prepend");
	    cni_generator->add_action (cni_code_generator::CNI_PREPEND,
				       action_class,
				       get_arg_for (it, "-prepend"));
	  }
	else if (arg == "-nostdheaders")
	  {
	    need_cni_generator ("-nostdheaders");
	    cni_generator->set_avoid_headers ();
	  }
        else
	  die_unrecognized ("option", arg);
      }

    // If -source was set, and -target was not, then -target defaults
    // to -source.
    if (! source_argument.empty () && target_argument.empty ())
      set_target (source_argument);

    std::list<class_factory *> facs;

    if (! bootclasspath.empty ())
      add_classpath (facs, bootclasspath);

    if (! classpath.empty ())
      add_classpath (facs, classpath);
    add_path (facs, dash_i_args, false);
    if (classpath.empty () && dash_i_args.empty ())
      {
        if (getenv ("CLASSPATH") != NULL)
	  add_classpath (facs, getenv ("CLASSPATH"));
	else
	  {
	    // Default class path.
	    add_classpath (facs, ".");
	  }
      }

    // javac acts as if there's an empty item at the head of its sourcepath,
    // regardless of whether the user supplied a -sourcepath argument. This
    // takes care of source files relative to the current directory, and
    // absolute names.
    // It's also the case that javac doesn't care if it's looking for C.java
    // and finds it in multiple source directories. The first one is taken.
    add_path (facs, "", true);
    if (! sourcepath.empty ())
      add_sourcepath (facs, sourcepath);

    comp->set_class_factory (new classpath_class_factory (facs));

    if (outputs.empty ())
      comp->add_code_generator (new bytecode_code_generator (comp,
							     comp->get_directory_cache ()));

    if (output.empty ())
      output = ".";
    comp->set_output_directory (output);
    comp->set_encoding (encoding);

    // Start the worker threads.
    for (int i = 0; i < n_threads; ++i)
      concurrence::make_thread (comp, &compiler::work).start ();

    return source_files;
  }
  
private:

  // The '-source' argument.  If not specified, the empty string.
  std::string source_argument;

  // Likewise for '-target'.
  std::string target_argument;

  compiler *comp;
  std::deque<std::string> args;

  // The CNI code generator and the current class for which we're
  // collecting actions.
  std::string action_class;
  cni_code_generator *cni_generator;
};

int
gcjx_main (char *argv[])
{
  argument_parser ap (++argv);
  std::deque<std::string> source_files = ap.parse_args ();

  if (source_files.empty ())
    throw make_error ("no input files");

  std::deque<std::string>::const_iterator it = source_files.begin ();
  for (; it != source_files.end (); ++it)
    global->get_compiler ()->load_source_file (*it);
  // Make sure we don't do any analysis before the parsing is done.
  global->get_compiler ()->pause_workers ();

  if (! global->get_compiler ()->semantic_analysis ())
    return 1;

  if (! global->get_compiler ()->generate_code ())
    return 1;
  return 0;
}

int
main (int argc, char **argv)
{
  initialize_primitive_types ();
  compiler *comp = new compiler ("gcjx");
  global->set_compiler (comp);
  
  try
    {
      return gcjx_main (argv);
    }
  catch (command_line_error &ex)
    {
      std::cerr << global->get_compiler ()->get_name () << ": " << ex;
      return 1;
    }
}
