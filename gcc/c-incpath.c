/* Set up combined include path chain for the preprocessor.
   Copyright (C) 1986, 1987, 1989, 1992, 1993, 1994, 1995, 1996, 1997, 1998,
   1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007
   Free Software Foundation, Inc.

   Broken out of cppinit.c and cppfiles.c and rewritten Mar 2003.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 3, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "machmode.h"
#include "target.h"
#include "tm.h"
#include "cpplib.h"
#include "prefix.h"
#include "intl.h"
#include "c-incpath.h"
#include "cppdefault.h"

/* Windows does not natively support inodes, and neither does MSDOS.
   Cygwin's emulation can generate non-unique inodes, so don't use it.
   VMS has non-numeric inodes.  */
#ifdef VMS
# define INO_T_EQ(A, B) (!memcmp (&(A), &(B), sizeof (A)))
# define INO_T_COPY(DEST, SRC) memcpy(&(DEST), &(SRC), sizeof (SRC))
#else
# if (defined _WIN32 && !defined (_UWIN)) || defined __MSDOS__
#  define INO_T_EQ(A, B) 0
# else
#  define INO_T_EQ(A, B) ((A) == (B))
# endif
# define INO_T_COPY(DEST, SRC) (DEST) = (SRC)
#endif

static const char dir_separator_str[] = { DIR_SEPARATOR, 0 };

static void add_env_var_paths (struct c_incpath *, const char *, int);
static void add_standard_paths (struct c_incpath *, const char *,
				const char *, const char *, int);
static void free_path (struct cpp_dir *, int);
static void merge_include_chains (struct c_incpath *, const char *,
				  cpp_reader *, int);
static void add_sysroot_to_chain (struct c_incpath *, const char *, int);
static struct cpp_dir *remove_duplicates (cpp_reader *,
					  struct cpp_dir *,
					  struct cpp_dir *,
					  struct cpp_dir *, int);

struct c_incpath
{
  /* Include chains heads and tails.  */
  struct cpp_dir *heads[4];
  struct cpp_dir *tails[4];
  bool quote_ignores_source_dir;
};

enum { REASON_QUIET = 0, REASON_NOENT, REASON_DUP, REASON_DUP_SYS };

struct c_incpath *
new_c_incpath (void)
{
  return XCNEW (struct c_incpath);
}

void
delete_c_incpath (struct c_incpath *incpath)
{
  /* FIXME: clean up the chains too... */
  XDELETE (incpath);
}

/* Free an element of the include chain, possibly giving a reason.  */
static void
free_path (struct cpp_dir *path, int reason)
{
  switch (reason)
    {
    case REASON_DUP:
    case REASON_DUP_SYS:
      fprintf (stderr, _("ignoring duplicate directory \"%s\"\n"), path->name);
      if (reason == REASON_DUP_SYS)
	fprintf (stderr,
 _("  as it is a non-system directory that duplicates a system directory\n"));
      break;

    case REASON_NOENT:
      fprintf (stderr, _("ignoring nonexistent directory \"%s\"\n"),
	       path->name);
      break;

    case REASON_QUIET:
    default:
      break;
    }

  free (path->name);
  free (path);
}

/* Read ENV_VAR for a PATH_SEPARATOR-separated list of file names; and
   append all the names to the search path CHAIN.  */
static void
add_env_var_paths (struct c_incpath *incpath, const char *env_var, int chain)
{
  char *p, *q, *path;

  GET_ENVIRONMENT (q, env_var);

  if (!q)
    return;

  for (p = q; *q; p = q + 1)
    {
      q = p;
      while (*q != 0 && *q != PATH_SEPARATOR)
	q++;

      if (p == q)
	path = xstrdup (".");
      else
	{
	  path = XNEWVEC (char, q - p + 1);
	  memcpy (path, p, q - p);
	  path[q - p] = '\0';
	}

      add_path (incpath, path, chain, chain == SYSTEM, false);
    }
}

/* Append the standard include chain defined in cppdefault.c.  */
static void
add_standard_paths (struct c_incpath *incpath,
		    const char *sysroot, const char *iprefix,
		    const char *imultilib, int cxx_stdinc)
{
  const struct default_include *p;
  int relocated = cpp_relocated();
  size_t len;

  if (iprefix && (len = cpp_GCC_INCLUDE_DIR_len) != 0)
    {
      /* Look for directories that start with the standard prefix.
	 "Translate" them, i.e. replace /usr/local/lib/gcc... with
	 IPREFIX and search them first.  */
      for (p = cpp_include_defaults; p->fname; p++)
	{
	  if (!p->cplusplus || cxx_stdinc)
	    {
	      /* Should we be translating sysrooted dirs too?  Assume
		 that iprefix and sysroot are mutually exclusive, for
		 now.  */
	      if (sysroot && p->add_sysroot)
		continue;
	      if (!strncmp (p->fname, cpp_GCC_INCLUDE_DIR, len))
		{
		  char *str = concat (iprefix, p->fname + len, NULL);
		  if (p->multilib && imultilib)
		    str = concat (str, dir_separator_str, imultilib, NULL);
		  add_path (incpath, str, SYSTEM, p->cxx_aware, false);
		}
	    }
	}
    }

  for (p = cpp_include_defaults; p->fname; p++)
    {
      if (!p->cplusplus || cxx_stdinc)
	{
	  char *str;

	  /* Should this directory start with the sysroot?  */
	  if (sysroot && p->add_sysroot)
	    str = concat (sysroot, p->fname, NULL);
	  else if (!p->add_sysroot && relocated
		   && strncmp (p->fname, cpp_PREFIX, cpp_PREFIX_len) == 0)
	    {
 	      static const char *relocated_prefix;
	      /* If this path starts with the configure-time prefix, 
		 but the compiler has been relocated, replace it 
		 with the run-time prefix.  The run-time exec prefix
		 is GCC_EXEC_PREFIX.  Compute the path from there back
		 to the toplevel prefix.  */
	      if (!relocated_prefix)
		{
		  char *dummy;
		  /* Make relative prefix expects the first argument
		     to be a program, not a directory.  */
		  dummy = concat (gcc_exec_prefix, "dummy", NULL);
		  relocated_prefix 
		    = make_relative_prefix (dummy,
					    cpp_EXEC_PREFIX,
					    cpp_PREFIX);
		}
	      str = concat (relocated_prefix,
			    p->fname + cpp_PREFIX_len, 
			    NULL);
	      str = update_path (str, p->component);
	    }
	  else
	    str = update_path (p->fname, p->component);

	  if (p->multilib && imultilib)
	    str = concat (str, dir_separator_str, imultilib, NULL);

	  add_path (incpath, str, SYSTEM, p->cxx_aware, false);
	}
    }
}

/* For each duplicate path in chain HEAD, keep just the first one.
   Remove each path in chain HEAD that also exists in chain SYSTEM.
   Set the NEXT pointer of the last path in the resulting chain to
   JOIN, unless it duplicates JOIN in which case the last path is
   removed.  Return the head of the resulting chain.  Any of HEAD,
   JOIN and SYSTEM can be NULL.  */

static struct cpp_dir *
remove_duplicates (cpp_reader *pfile, struct cpp_dir *head,
		   struct cpp_dir *system, struct cpp_dir *join,
		   int verbose)
{
  struct cpp_dir **pcur, *tmp, *cur;
  struct stat st;

  for (pcur = &head; *pcur; )
    {
      int reason = REASON_QUIET;

      cur = *pcur;

      if (stat (cur->name, &st))
	{
	  /* Dirs that don't exist are silently ignored, unless verbose.  */
	  if (errno != ENOENT)
	    cpp_errno (pfile, CPP_DL_ERROR, cur->name);
	  else
	    {
	      /* If -Wmissing-include-dirs is given, warn.  */
	      cpp_options *opts = cpp_get_options (pfile);
	      if (opts->warn_missing_include_dirs && cur->user_supplied_p)
		cpp_errno (pfile, CPP_DL_WARNING, cur->name);
	      reason = REASON_NOENT;
	    }
	}
      else if (!S_ISDIR (st.st_mode))
	cpp_error_with_line (pfile, CPP_DL_ERROR, 0, 0,
			     "%s: not a directory", cur->name);
      else
	{
	  INO_T_COPY (cur->ino, st.st_ino);
	  cur->dev  = st.st_dev;

	  /* Remove this one if it is in the system chain.  */
	  reason = REASON_DUP_SYS;
	  for (tmp = system; tmp; tmp = tmp->next)
	   if (INO_T_EQ (tmp->ino, cur->ino) && tmp->dev == cur->dev
	       && cur->construct == tmp->construct)
	      break;

	  if (!tmp)
	    {
	      /* Duplicate of something earlier in the same chain?  */
	      reason = REASON_DUP;
	      for (tmp = head; tmp != cur; tmp = tmp->next)
	       if (INO_T_EQ (cur->ino, tmp->ino) && cur->dev == tmp->dev
		   && cur->construct == tmp->construct)
		  break;

	      if (tmp == cur
		  /* Last in the chain and duplicate of JOIN?  */
		  && !(cur->next == NULL && join
		       && INO_T_EQ (cur->ino, join->ino)
		      && cur->dev == join->dev
		      && cur->construct == join->construct))
		{
		  /* Unique, so keep this directory.  */
		  pcur = &cur->next;
		  continue;
		}
	    }
	}

      /* Remove this entry from the chain.  */
      *pcur = cur->next;
      free_path (cur, verbose ? reason: REASON_QUIET);
    }

  *pcur = join;
  return head;
}

/* Add SYSROOT to any user-supplied paths in CHAIN starting with
   "=".  */

static void
add_sysroot_to_chain (struct c_incpath *incpath, const char *sysroot, int chain)
{
  struct cpp_dir *p;

  for (p = incpath->heads[chain]; p != NULL; p = p->next)
    if (p->name[0] == '=' && p->user_supplied_p)
      p->name = concat (sysroot, p->name + 1, NULL);
}

/* Merge the four include chains together in the order quote, bracket,
   system, after.  Remove duplicate dirs (as determined by
   INO_T_EQ()).

   We can't just merge the lists and then uniquify them because then
   we may lose directories from the <> search path that should be
   there; consider -iquote foo -iquote bar -Ifoo -Iquux.  It is
   however safe to treat -iquote bar -iquote foo -Ifoo -Iquux as if
   written -iquote bar -Ifoo -Iquux.  */

static void
merge_include_chains (struct c_incpath *incpath, const char *sysroot,
		      cpp_reader *pfile, int verbose)
{
  /* Add the sysroot to user-supplied paths starting with "=".  */
  if (sysroot)
    {
      add_sysroot_to_chain (incpath, sysroot, QUOTE);
      add_sysroot_to_chain (incpath, sysroot, BRACKET);
      add_sysroot_to_chain (incpath, sysroot, SYSTEM);
      add_sysroot_to_chain (incpath, sysroot, AFTER);
    }

  /* Join the SYSTEM and AFTER chains.  Remove duplicates in the
     resulting SYSTEM chain.  */
  if (incpath->heads[SYSTEM])
    incpath->tails[SYSTEM]->next = incpath->heads[AFTER];
  else
    incpath->heads[SYSTEM] = incpath->heads[AFTER];
  incpath->heads[SYSTEM] = remove_duplicates (pfile, incpath->heads[SYSTEM],
					      0, 0, verbose);

  /* Remove duplicates from BRACKET that are in itself or SYSTEM, and
     join it to SYSTEM.  */
  incpath->heads[BRACKET] = remove_duplicates (pfile, incpath->heads[BRACKET],
					       incpath->heads[SYSTEM],
					       incpath->heads[SYSTEM], verbose);

  /* Remove duplicates from QUOTE that are in itself or SYSTEM, and
     join it to BRACKET.  */
  incpath->heads[QUOTE] = remove_duplicates (pfile, incpath->heads[QUOTE],
					     incpath->heads[SYSTEM],
					     incpath->heads[BRACKET], verbose);

  /* If verbose, print the list of dirs to search.  */
  if (verbose)
    {
      struct cpp_dir *p;

      fprintf (stderr, _("#include \"...\" search starts here:\n"));
      for (p = incpath->heads[QUOTE];; p = p->next)
	{
	  if (p == incpath->heads[BRACKET])
	    fprintf (stderr, _("#include <...> search starts here:\n"));
	  if (!p)
	    break;
	  fprintf (stderr, " %s\n", p->name);
	}
      fprintf (stderr, _("End of search list.\n"));
    }
}

/* Use given -I paths for #include "..." but not #include <...>, and
   don't search the directory of the present file for #include "...".
   (Note that -I. -I- is not the same as the default setup; -I. uses
   the compiler's working dir.)  */
void
split_quote_chain (struct c_incpath *incpath)
{
  incpath->heads[QUOTE] = incpath->heads[BRACKET];
  incpath->tails[QUOTE] = incpath->tails[BRACKET];
  incpath->heads[BRACKET] = NULL;
  incpath->tails[BRACKET] = NULL;
  /* This is NOT redundant.  */
  incpath->quote_ignores_source_dir = true;
}

/* Add P to the chain specified by CHAIN.  */

void
add_cpp_dir_path (struct c_incpath *incpath, cpp_dir *p, int chain)
{
  if (incpath->tails[chain])
    incpath->tails[chain]->next = p;
  else
    incpath->heads[chain] = p;
  incpath->tails[chain] = p;
}

/* Add PATH to the include chain CHAIN. PATH must be malloc-ed and
   NUL-terminated.  */
void
add_path (struct c_incpath *incpath, char *path, int chain,
	  int cxx_aware, bool user_supplied_p)
{
  cpp_dir *p;

#if defined (HAVE_DOS_BASED_FILE_SYSTEM)
  /* Remove unnecessary trailing slashes.  On some versions of MS
     Windows, trailing  _forward_ slashes cause no problems for stat().
     On newer versions, stat() does not recognize a directory that ends
     in a '\\' or '/', unless it is a drive root dir, such as "c:/",
     where it is obligatory.  */
  int pathlen = strlen (path);
  char* end = path + pathlen - 1;
  /* Preserve the lead '/' or lead "c:/".  */
  char* start = path + (pathlen > 2 && path[1] == ':' ? 3 : 1);
  
  for (; end > start && IS_DIR_SEPARATOR (*end); end--)
    *end = 0;
#endif

  p = XNEW (cpp_dir);
  p->next = NULL;
  p->name = path;
  if (chain == SYSTEM || chain == AFTER)
    p->sysp = 1 + !cxx_aware;
  else
    p->sysp = 0;
  p->construct = 0;
  p->user_supplied_p = user_supplied_p;

  add_cpp_dir_path (incpath, p, chain);
}

/* Exported function to handle include chain merging, duplicate
   removal, and registration with cpplib.  */
void
register_include_chains (struct c_incpath *incpath,
			 cpp_reader *pfile, const char *sysroot,
			 const char *iprefix, const char *imultilib,
			 int stdinc, int cxx_stdinc, int verbose)
{
  static const char *const lang_env_vars[] =
    { "C_INCLUDE_PATH", "CPLUS_INCLUDE_PATH",
      "OBJC_INCLUDE_PATH", "OBJCPLUS_INCLUDE_PATH" };
  cpp_options *cpp_opts = cpp_get_options (pfile);
  size_t idx = (cpp_opts->objc ? 2: 0);

  if (cpp_opts->cplusplus)
    idx++;
  else
    cxx_stdinc = false;

  /* CPATH and language-dependent environment variables may add to the
     include chain.  */
  add_env_var_paths (incpath, "CPATH", BRACKET);
  add_env_var_paths (incpath, lang_env_vars[idx], SYSTEM);

  target_c_incpath.extra_pre_includes (incpath, sysroot, iprefix, stdinc);

  /* Finally chain on the standard directories.  */
  if (stdinc)
    add_standard_paths (incpath, sysroot, iprefix, imultilib, cxx_stdinc);

  target_c_incpath.extra_includes (incpath, sysroot, iprefix, stdinc);

  merge_include_chains (incpath, sysroot, pfile, verbose);

  cpp_set_include_chains (pfile, incpath->heads[QUOTE], incpath->heads[BRACKET],
			  incpath->quote_ignores_source_dir);
}
#if !(defined TARGET_EXTRA_INCLUDES) || !(defined TARGET_EXTRA_PRE_INCLUDES)
static void hook_void_charptr_charptr_int (struct c_incpath *incpath ATTRIBUTE_UNUSED,
					   const char *sysroot ATTRIBUTE_UNUSED,
					   const char *iprefix ATTRIBUTE_UNUSED,
					   int stdinc ATTRIBUTE_UNUSED)
{
}
#endif

#ifndef TARGET_EXTRA_INCLUDES
#define TARGET_EXTRA_INCLUDES hook_void_charptr_charptr_int
#endif
#ifndef TARGET_EXTRA_PRE_INCLUDES
#define TARGET_EXTRA_PRE_INCLUDES hook_void_charptr_charptr_int
#endif

struct target_c_incpath_s target_c_incpath = { TARGET_EXTRA_PRE_INCLUDES, TARGET_EXTRA_INCLUDES };

