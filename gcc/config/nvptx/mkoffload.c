/* Offload image generation tool for PTX.

   Copyright (C) 2014-2015 Free Software Foundation, Inc.

   Contributed by Nathan Sidwell <nathan@codesourcery.com> and
   Bernd Schmidt <bernds@codesourcery.com>.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

/* Munges PTX assembly into a C source file defining the PTX code as a
   string.

   This is not a complete assembler.  We presume the source is well
   formed from the compiler and can die horribly if it is not.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "intl.h"
#include <libgen.h>
#include "obstack.h"
#include "diagnostic.h"
#include "collect-utils.h"
#include "gomp-constants.h"

const char tool_name[] = "nvptx mkoffload";

#define COMMENT_PREFIX "#"

struct id_map
{
  id_map *next;
  char *ptx_name;
};

static id_map *func_ids, **funcs_tail = &func_ids;
static id_map *var_ids, **vars_tail = &var_ids;

/* Files to unlink.  */
static const char *ptx_name;
static const char *ptx_cfile_name;

/* Shows if we should compile binaries for i386 instead of x86-64.  */
bool target_ilp32 = false;

/* Delete tempfiles.  */

/* Unlink a temporary file unless requested otherwise.  */

void
maybe_unlink (const char *file)
{
  if (!save_temps)
    {
      if (unlink_if_ordinary (file)
	  && errno != ENOENT)
	fatal_error (input_location, "deleting file %s: %m", file);
    }
  else
    fprintf (stderr, "[Leaving %s]\n", file);
}

void
tool_cleanup (bool)
{
  if (ptx_cfile_name)
    maybe_unlink (ptx_cfile_name);
  if (ptx_name)
    maybe_unlink (ptx_name);
}

/* Add or change the value of an environment variable, outputting the
   change to standard error if in verbose mode.  */
static void
xputenv (const char *string)
{
  if (verbose)
    fprintf (stderr, "%s\n", string);
  putenv (CONST_CAST (char *, string));
}


static void
record_id (const char *p1, id_map ***where)
{
  const char *end = strchr (p1, '\n');
  if (!end)
    fatal_error (input_location, "malformed ptx file");

  id_map *v = XNEW (id_map);
  size_t len = end - p1;
  v->ptx_name = XNEWVEC (char, len + 1);
  memcpy (v->ptx_name, p1, len);
  v->ptx_name[len] = '\0';
  v->next = NULL;
  id_map **tail = *where;
  *tail = v;
  *where = &v->next;
}

/* Read the whole input file.  It will be NUL terminated (but
   remember, there could be a NUL in the file itself.  */

static const char *
read_file (FILE *stream, size_t *plen)
{
  size_t alloc = 16384;
  size_t base = 0;
  char *buffer;

  if (!fseek (stream, 0, SEEK_END))
    {
      /* Get the file size.  */
      long s = ftell (stream);
      if (s >= 0)
	alloc = s + 100;
      fseek (stream, 0, SEEK_SET);
    }
  buffer = XNEWVEC (char, alloc);

  for (;;)
    {
      size_t n = fread (buffer + base, 1, alloc - base - 1, stream);

      if (!n)
	break;
      base += n;
      if (base + 1 == alloc)
	{
	  alloc *= 2;
	  buffer = XRESIZEVEC (char, buffer, alloc);
	}
    }
  buffer[base] = 0;
  *plen = base;
  return buffer;
}

/* Parse STR, saving found tokens into PVALUES and return their number.
   Tokens are assumed to be delimited by ':'.  */
static unsigned
parse_env_var (const char *str, char ***pvalues)
{
  const char *curval, *nextval;
  char **values;
  unsigned num = 1, i;

  curval = strchr (str, ':');
  while (curval)
    {
      num++;
      curval = strchr (curval + 1, ':');
    }

  values = (char **) xmalloc (num * sizeof (char *));
  curval = str;
  nextval = strchr (curval, ':');
  if (nextval == NULL)
    nextval = strchr (curval, '\0');

  for (i = 0; i < num; i++)
    {
      int l = nextval - curval;
      values[i] = (char *) xmalloc (l + 1);
      memcpy (values[i], curval, l);
      values[i][l] = 0;
      curval = nextval + 1;
      nextval = strchr (curval, ':');
      if (nextval == NULL)
	nextval = strchr (curval, '\0');
    }
  *pvalues = values;
  return num;
}

/* Auxiliary function that frees elements of PTR and PTR itself.
   N is number of elements to be freed.  If PTR is NULL, nothing is freed.
   If an element is NULL, subsequent elements are not freed.  */
static void
free_array_of_ptrs (void **ptr, unsigned n)
{
  unsigned i;
  if (!ptr)
    return;
  for (i = 0; i < n; i++)
    {
      if (!ptr[i])
	break;
      free (ptr[i]);
    }
  free (ptr);
  return;
}

/* Check whether NAME can be accessed in MODE.  This is like access,
   except that it never considers directories to be executable.  */
static int
access_check (const char *name, int mode)
{
  if (mode == X_OK)
    {
      struct stat st;

      if (stat (name, &st) < 0 || S_ISDIR (st.st_mode))
	return -1;
    }

  return access (name, mode);
}

static void
process (FILE *in, FILE *out)
{
  size_t len;
  const char *input = read_file (in, &len);

  fprintf (out, "static const char ptx_code[] = \n \"");
  for (size_t i = 0; i < len; i++)
    {
      char c = input[i];
      bool nl = false;
      switch (c)
	{
	case '\0':
	  putc ('\\', out);
	  c = '0';
	  break;
	case '\r':
	  continue;
	case '\n':
	  putc ('\\', out);
	  c = 'n';
	  nl = true;
	  break;
	case '"':
	case '\\':
	  putc ('\\', out);
	  break;

	case '/':
	  if (strncmp (input + i, "//:VAR_MAP ", 11) == 0)
	    record_id (input + i + 11, &vars_tail);
	  if (strncmp (input + i, "//:FUNC_MAP ", 12) == 0)
	    record_id (input + i + 12, &funcs_tail);
	  break;

	default:
	  break;
	}
      putc (c, out);
      if (nl)
	fputs ("\"\n\t\"", out);
    }
  fprintf (out, "\";\n\n");

  fprintf (out, "static const char *const var_mappings[] = {\n");
  for (id_map *id = var_ids; id; id = id->next)
    fprintf (out, "\t\"%s\"%s\n", id->ptx_name, id->next ? "," : "");
  fprintf (out, "};\n\n");
  fprintf (out, "static const char *const func_mappings[] = {\n");
  for (id_map *id = func_ids; id; id = id->next)
    fprintf (out, "\t\"%s\"%s\n", id->ptx_name, id->next ? "," : "");
  fprintf (out, "};\n\n");

  fprintf (out,
	   "static const struct nvptx_tdata {\n"
	   "  const char *ptx_src;\n"
	   "  __SIZE_TYPE__ ptx_len;\n"
	   "  const char *const *var_names;\n"
	   "  __SIZE_TYPE__ var_num;\n"
	   "  const char *const *fn_names;\n"
	   "  __SIZE_TYPE__ fn_num;\n"
	   "} target_data = {\n"
	   "  ptx_code, sizeof (ptx_code),\n"
	   "  var_mappings,"
	   "  sizeof (var_mappings) / sizeof (var_mappings[0]),\n"
	   "  func_mappings,"
	   "  sizeof (func_mappings) / sizeof (func_mappings[0])\n"
	   "};\n\n");

  fprintf (out, "#ifdef __cplusplus\n");
  fprintf (out, "extern \"C\" {\n");
  fprintf (out, "#endif\n");

  fprintf (out, "extern void GOMP_offload_register"
	   " (const void *, int, const void *);\n");

  fprintf (out, "#ifdef __cplusplus\n");
  fprintf (out, "}\n");
  fprintf (out, "#endif\n");

  fprintf (out, "extern const void *const __OFFLOAD_TABLE__[];\n\n");
  fprintf (out, "static __attribute__((constructor)) void init (void)\n{\n");
  fprintf (out, "  GOMP_offload_register (__OFFLOAD_TABLE__, %d,\n",
	   GOMP_DEVICE_NVIDIA_PTX);
  fprintf (out, "                         &target_data);\n");
  fprintf (out, "};\n");
}

static void
compile_native (const char *infile, const char *outfile, const char *compiler)
{
  const char *collect_gcc_options = getenv ("COLLECT_GCC_OPTIONS");
  if (!collect_gcc_options)
    fatal_error (input_location,
		 "environment variable COLLECT_GCC_OPTIONS must be set");

  struct obstack argv_obstack;
  obstack_init (&argv_obstack);
  obstack_ptr_grow (&argv_obstack, compiler);
  obstack_ptr_grow (&argv_obstack, target_ilp32 ? "-m32" : "-m64");
  obstack_ptr_grow (&argv_obstack, infile);
  obstack_ptr_grow (&argv_obstack, "-c");
  obstack_ptr_grow (&argv_obstack, "-o");
  obstack_ptr_grow (&argv_obstack, outfile);
  obstack_ptr_grow (&argv_obstack, NULL);

  const char **new_argv = XOBFINISH (&argv_obstack, const char **);
  fork_execute (new_argv[0], CONST_CAST (char **, new_argv), true);
  obstack_free (&argv_obstack, NULL);
}

int
main (int argc, char **argv)
{
  FILE *in = stdin;
  FILE *out = stdout;
  const char *outname = 0;

  progname = "mkoffload";
  diagnostic_initialize (global_dc, 0);

  char *collect_gcc = getenv ("COLLECT_GCC");
  if (collect_gcc == NULL)
    fatal_error (input_location, "COLLECT_GCC must be set.");
  const char *gcc_path = dirname (ASTRDUP (collect_gcc));
  const char *gcc_exec = basename (ASTRDUP (collect_gcc));

  size_t len = (strlen (gcc_path) + 1
		+ strlen (GCC_INSTALL_NAME)
		+ 1);
  char *driver = XALLOCAVEC (char, len);

  if (strcmp (gcc_exec, collect_gcc) == 0)
    /* collect_gcc has no path, so it was found in PATH.  Make sure we also
       find accel-gcc in PATH.  */
    gcc_path = NULL;

  int driver_used = 0;
  if (gcc_path != NULL)
    driver_used = sprintf (driver, "%s/", gcc_path);
  sprintf (driver + driver_used, "%s", GCC_INSTALL_NAME);

  bool found = false;
  if (gcc_path == NULL)
    found = true;
  else if (access_check (driver, X_OK) == 0)
    found = true;
  else
    {
      /* Don't use alloca pointer with XRESIZEVEC.  */
      driver = NULL;
      /* Look in all COMPILER_PATHs for GCC_INSTALL_NAME.  */
      char **paths = NULL;
      unsigned n_paths;
      n_paths = parse_env_var (getenv ("COMPILER_PATH"), &paths);
      for (unsigned i = 0; i < n_paths; i++)
	{
	  len = strlen (paths[i]) + 1 + strlen (GCC_INSTALL_NAME) + 1;
	  driver = XRESIZEVEC (char, driver, len);
	  sprintf (driver, "%s/%s", paths[i], GCC_INSTALL_NAME);
	  if (access_check (driver, X_OK) == 0)
	    {
	      found = true;
	      break;
	    }
	}
      free_array_of_ptrs ((void **) paths, n_paths);
    }

  if (!found)
    fatal_error (input_location,
		 "offload compiler %s not found", GCC_INSTALL_NAME);

  /* We may be called with all the arguments stored in some file and
     passed with @file.  Expand them into argv before processing.  */
  expandargv (&argc, &argv);

  /* Find out whether we should compile binaries for i386 or x86-64.  */
  for (int i = argc - 1; i > 0; i--)
    if (strncmp (argv[i], "-foffload-abi=", sizeof ("-foffload-abi=") - 1) == 0)
      {
	if (strstr (argv[i], "ilp32"))
	  target_ilp32 = true;
	else if (!strstr (argv[i], "lp64"))
	  fatal_error (input_location,
		       "unrecognizable argument of option -foffload-abi");
	break;
      }

  struct obstack argv_obstack;
  obstack_init (&argv_obstack);
  obstack_ptr_grow (&argv_obstack, driver);
  obstack_ptr_grow (&argv_obstack, "-xlto");
  obstack_ptr_grow (&argv_obstack, target_ilp32 ? "-m32" : "-m64");
  obstack_ptr_grow (&argv_obstack, "-lgomp");
  char *collect_mkoffload_opts = getenv ("COLLECT_MKOFFLOAD_OPTIONS");
  if (collect_mkoffload_opts)
    {
      char *str = collect_mkoffload_opts;
      char *p;
      while ((p = strchr (str, ' ')) != 0)
	{
	  *p = '\0';
	  obstack_ptr_grow (&argv_obstack, str);
	  str = p + 1;
	}
      obstack_ptr_grow (&argv_obstack, str);
    }

  bool fopenacc = false;
  for (int ix = 1; ix != argc; ix++)
    {
      if (!strcmp (argv[ix], "-v"))
	verbose = true;
      else if (!strcmp (argv[ix], "-save-temps"))
	save_temps = true;
      else if (!strcmp (argv[ix], "-fopenacc"))
	fopenacc = true;

      if (!strcmp (argv[ix], "-o") && ix + 1 != argc)
	outname = argv[++ix];
      else
	obstack_ptr_grow (&argv_obstack, argv[ix]);
    }

  ptx_cfile_name = make_temp_file (".c");

  out = fopen (ptx_cfile_name, "w");
  if (!out)
    fatal_error (input_location, "cannot open '%s'", ptx_cfile_name);

  /* PR libgomp/65099: Currently, we only support offloading in 64-bit
     configurations, and only for OpenACC offloading.  */
  if (!target_ilp32 && fopenacc)
    {
      ptx_name = make_temp_file (".mkoffload");
      obstack_ptr_grow (&argv_obstack, "-o");
      obstack_ptr_grow (&argv_obstack, ptx_name);
      obstack_ptr_grow (&argv_obstack, NULL);
      const char **new_argv = XOBFINISH (&argv_obstack, const char **);

      char *execpath = getenv ("GCC_EXEC_PREFIX");
      char *cpath = getenv ("COMPILER_PATH");
      char *lpath = getenv ("LIBRARY_PATH");
      unsetenv ("GCC_EXEC_PREFIX");
      unsetenv ("COMPILER_PATH");
      unsetenv ("LIBRARY_PATH");

      fork_execute (new_argv[0], CONST_CAST (char **, new_argv), true);
      obstack_free (&argv_obstack, NULL);

      xputenv (concat ("GCC_EXEC_PREFIX=", execpath, NULL));
      xputenv (concat ("COMPILER_PATH=", cpath, NULL));
      xputenv (concat ("LIBRARY_PATH=", lpath, NULL));

      in = fopen (ptx_name, "r");
      if (!in)
	fatal_error (input_location, "cannot open intermediate ptx file");

      process (in, out);
    }

  fclose (out);

  compile_native (ptx_cfile_name, outname, collect_gcc);

  utils_cleanup (false);

  return 0;
}
