/* Support Cygwin paths under MinGW.
   Copyright (C) 2006 Free Software Foundation, Inc.
   Written by CodeSourcery.

This file is part of the libiberty library.
Libiberty is free software; you can redistribute it and/or modify it
under the terms of the GNU Library General Public License as published
by the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Libiberty is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with libiberty; see the file COPYING.LIB.  If not, write
to the Free Software Foundation, Inc., 51 Franklin Street - Fifth
Floor, Boston, MA 02110-1301, USA.  */

#include <windows.h>
#include <assert.h>
#include <direct.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>
#include <process.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "libiberty.h"

/* The maximum number of wide characters in a path using the \\?\
   notation.  (Strictly, this is not the correct value to use for
   processing cygpath output, since there may be more than this number
   of multibyte characters but the path may still be short enough
   after conversion to wide characters.)  */
#define MAX_WPATH 32768

/* If non-zero, we have attempted to use cygpath.  CYGPATH_PEX may
   still be NULL, if cygpath is unavailable.  */
static int cygpath_initialized;

/* If non-NULL, an instance of cygpath connected via a pipe.  */
static struct pex_obj *cygpath_pex;

/* The input to cygpath.  */
static FILE *cygpath_in;

/* The output from cygpath.  */
static FILE *cygpath_out;

/* If non-NULL, a file to which path translations are logged.  */
static FILE *cygpath_log;

/* Record MESSAGE in the CYGPATH_LOG.  MESSAGE is a format string,
   which is expected to have a single "%s" field, to be replaced by
   ARG.  */
static void
cygpath_log_msg_arg (const char *message, const char *arg)
{
  if (!cygpath_log)
    return;
  fprintf (cygpath_log, "[%d] cygpath: ", _getpid ());
  fprintf (cygpath_log, message, arg);
  fprintf (cygpath_log, "\n");
  fflush (cygpath_log);
}

/* Record MESSAGE in the CYGPATH_LOG.  */
static void
cygpath_log_msg (const char *message)
{
  cygpath_log_msg_arg ("%s", message);
}

/* An error has occured.  Add the MESSAGE to the CYGPATH_LOG, noting
   the cause of the error based on errno.  */
static void
cygpath_perror (const char *message)
{
  if (!cygpath_log)
    return;
  fprintf (cygpath_log, "[%d] cygpath: error: %s: %s\n",
	   _getpid(), message, strerror (errno));
  fflush (cygpath_log);
}

/* Closes CYGPATH_PEX and frees all associated
   resoures.  */
static void
cygpath_close (void)
{
  /* Free resources.  */
  if (cygpath_out)
    {
      fclose (cygpath_out);
      cygpath_out = NULL;
    }
  if (cygpath_in)
    {
      fclose (cygpath_in);
      cygpath_in = NULL;
    }
  if (cygpath_pex)
    {
      pex_free (cygpath_pex);
      cygpath_pex = NULL;
    }
  if (cygpath_log)
    {
      cygpath_log_msg ("end");
      cygpath_log = NULL;
    }
}

/* CYG_PATH is a pointer to a Cygwin path.  This function converts the
   Cygwin path to a Windows path, storing the result in
   WIN32_PATH.  Returns true if the conversion was successful; false
   otherwise.  */
int
cygpath (const char *cyg_path, char win32_path[MAX_WPATH + 1])
{
  bool ok;
  bool retrying;

  /* Special-case the empty path.  cygpath cannot handle the empty
     path correctly.  It ignores the empty line, waiting for a
     non-empty line, which in turn causes an application using this
     function to appear stuck.  */
  if (cyg_path[0] == '\0')
    {
      win32_path[0] = '\0';
      return true;
    }

  retrying = false;

 retry:
  if (!cygpath_initialized)
    {
      const char *argv[] = { "cygpath", "-w", "-f", "-", NULL };
      const char *cygpath_path;
      const char *log;
      int err;

      /* If we are unable to invoke cygpath, we do not want to try
	 again.  So, we set the initialized flag at this point; if
	 errors occur during the invocation, it will remain set.  */
      cygpath_initialized = 1;
      /* Check to see if the user wants cygpath support.  */
      cygpath_path = getenv ("CYGPATH");
      if (!cygpath_path)
	/* The user doesn't need to support Cygwin paths.  */
	goto error;
      /* If debugging, open the log file.  */
      log = getenv ("CSL_DEBUG_CYGPATH");
      if (log && log[0])
	{
	  /* The log file is opened for "append" so that multiple
	     processes (perhaps invoked from "make") can share it.  */
	  cygpath_log = fopen (log, "a");
	  if (cygpath_log)
	    cygpath_log_msg ("begin");
	}
      /* If the environment variable is set to a non-empty string, use
	 that string as the path to cygpath.  */
      if (cygpath_path[0] != '\0')
	argv[0] = cygpath_path;
      /* Create the pex object.  */
      cygpath_pex = pex_init (PEX_SEARCH | PEX_USE_PIPES,
			      "cygpath", NULL);
      if (!cygpath_pex)
	goto error;
      /* Get the FILE we will use to write to the child.  */
      cygpath_in = pex_input_pipe (cygpath_pex, /*binary=*/0);
      if (!cygpath_in)
	goto error;
      /* Start the child process.  */
      if (pex_run (cygpath_pex, PEX_SEARCH | PEX_USE_PIPES,
		   argv[0], (char**) argv,
		   NULL, NULL,
		   &err) != NULL)
	goto error;
      /* Get the FILE we will use to read from the child.  */
      cygpath_out = pex_read_output (cygpath_pex, /*binary=*/1);
      if (!cygpath_out)
	goto error;
    }
  else if (!cygpath_pex)
    /* We previously tried to use cygpath, but something went wrong.  */
    return false;

  /* Write CYG_PATH to the child, on a line by itself.  */
  cygpath_log_msg_arg ("-> %s", cyg_path);
  if (fprintf (cygpath_in, "%s\n", cyg_path) < 0)
    {
      cygpath_perror ("write failed");
      goto error;
    }
  /* Flush the output.  (We cannot set the stream into line-buffered
     mode with setvbuf because Windows treats _IOLBF as a synonym for
     _IOFBF.)  */
  if (fflush (cygpath_in))
    cygpath_perror ("flush failed");
  /* Read the output.  */
  ok = true;
  while (1)
    {
      size_t pathlen;
      if (!fgets (win32_path, MAX_WPATH, cygpath_out))
	{
	  if (ferror (cygpath_out))
	    cygpath_perror ("read failed");
	  else
	    {
	      cygpath_log_msg ("error: EOF");
	      /* Unfortunately, cygpath sometimes crashes for no
		 apparent reason.  We give it two chances... */
	      if (!retrying)
		{
		  retrying = true;
		  cygpath_log_msg ("retrying");
		  cygpath_close ();
		  cygpath_initialized = 0;
		  goto retry;
		}
	      else
		{
		  /* Give up, but in case the error is specific to
		     this path, reopen at the next request.  */
		  cygpath_close ();
		  cygpath_initialized = 0;
		}
	    }
	  goto error;
	}
      pathlen = strlen (win32_path);
      if (pathlen == 0 && ok)
	/* This isn't a well-formed response from cygpath.  */
	goto error;
      if (win32_path[pathlen - 1] == '\n')
	{
	  win32_path[pathlen - 1] = '\0';
	  cygpath_log_msg_arg ("<- %s", win32_path);
	  break;
	}
      /* We didn't reach the end of the line.  There's no point in
	 trying to use this output, since we know the length of
	 paths are limited to MAX_WPATH characters, but we read the
	 entire line so that we are still in sync with
	 cygpath.  */
      ok = false;
      if (cygpath_log)
	cygpath_log_msg_arg ("error: invalid response: %s",
			     win32_path);
    }

  return ok;

 error:
  cygpath_close();
  return false;
}

/* Convert a multibyte string S to wide characters and return the
   malloced string.  */

static wchar_t *
convert_to_wide (const char *s)
{
  size_t len, len2;
  wchar_t *w;

  len = mbstowcs (NULL, s, 0);
  if (len == (size_t) -1)
    return NULL;
  w = XNEWVEC (wchar_t, len + 1);
  len2 = mbstowcs (w, s, len + 1);
  assert (len2 == len);
  return w;
}

/* Return true if PATH is a reserved Windows filename.  The list of these
   is taken from:
   http://msdn.microsoft.com/en-us/library/aa365247%28VS.85%29.aspx  */
static bool
windows_reserved_filename_p (const char *path)
{
  static const char * const names[]
    = { "CON", "PRN", "AUX", "NUL",
	"COM1", "COM2", "COM3", "COM4", "COM5",
	"COM6", "COM7", "COM8", "COM9", 
	"LPT1", "LPT2", "LPT3", "LPT4", "LPT5",
	"LPT6", "LPT7", "LPT8", "LPT9" };
  static int n_names = sizeof (names) / sizeof (names[0]);
  int i;

  for (i = 0; i < n_names; i++)
    if (strcasecmp (names[i], path) == 0)
      return true;
  return false;
}

/* Given a nonempty Windows path PATH, convert it to a wide-character
   path starting with \\?\ and return the malloced path; for empty
   paths, just return the converted path without adding \\?\.  Return
   NULL if the multibyte string cannot be converted to wide
   characters.  */

static wchar_t *
convert_long_path (const char *path)
{
  size_t len;
  wchar_t *wpath, *p, *q, *wpath_main;

  /* Convert the path to wide characters.  */
  wpath = convert_to_wide (path);
  if (wpath == NULL)
    return NULL;
  len = wcslen (wpath);

  /* If the path is empty, return it as-is.  */
  if (len == 0)
    return wpath;

  /* If the path already begins \\?\, return it as-is.  */
  if (wpath[0] == L'\\' && wpath[1] == L'\\'
      && wpath[2] == L'?' && wpath[3] == L'\\')
    return wpath;

  /* If the path is a Windows reserved filename, return it as-is.  */
  if (windows_reserved_filename_p (path))
    return wpath;

  /* Convert / to \.  */
  for (p = wpath; *p; p++)
    if (*p == L'/')
      *p = L'\\';

  /* Except for an initial \\, convert multiple consecutive \ to a
     single \.  */
  if (len > 1)
    {
      p = wpath + 2;
      q = wpath + 2;
      while (*p != L'\0')
	{
	  if (p[0] == L'\\' && p[-1] == L'\\')
	    p++;
	  else
	    *q++ = *p++;
	}
      *q++ = L'\0';
      len = wcslen (wpath);
    }

  /* If the path is absolute except for a drive letter, add that drive
     letter.  */
  if (wpath[0] == L'\\' && wpath[1] != L'\\')
    {
      int drive = _getdrive ();
      wchar_t *wpath_new;

      /* If there is no current drive (current directory a UNC path),
	 we cannot go further.  */
      if (drive < 1 || drive > 26)
	return wpath;

      wpath_new = XNEWVEC (wchar_t, len + 3);
      wpath_new[0] = L'A' - 1 + drive;
      wpath_new[1] = L':';
      wcscpy (wpath_new + 2, wpath);
      free (wpath);
      wpath = wpath_new;
      len = wcslen (wpath);
    }

  /* If the path is a relative path on a named drive, add the current
     directory for that drive.  */
  if (((wpath[0] >= L'A' && wpath[0] <= L'Z')
       || (wpath[0] >= L'a' && wpath[0] <= L'z'))
      && wpath[1] == L':'
      && wpath[2] != L'\\')
    {
      int drive;
      wchar_t *cwd, *wpath_new;
      size_t cwdlen;
      if (wpath[0] >= L'A' && wpath[0] <= L'Z')
	drive = wpath[0] - L'A' + 1;
      else
	drive = wpath[0] - L'a' + 1;
      cwd = _wgetdcwd (drive, NULL, 0);
      if (cwd == NULL)
	{
	  perror ("_wgetdcwd");
	  exit (EXIT_FAILURE);
	}
      cwdlen = wcslen (cwd);
      if (cwd[cwdlen - 1] == L'\\')
	{
	  wpath_new = XNEWVEC (wchar_t, cwdlen + len - 1);
	  wmemcpy (wpath_new, cwd, cwdlen);
	  wcscpy (wpath_new + cwdlen, wpath + 2);
	}
      else
	{
	  wpath_new = XNEWVEC (wchar_t, cwdlen + len);
	  wmemcpy (wpath_new, cwd, cwdlen);
	  wpath_new[cwdlen] = L'\\';
	  wcscpy (wpath_new + cwdlen + 1, wpath + 2);
	}
      free (cwd);
      free (wpath);
      wpath = wpath_new;
      len = wcslen (wpath);
    }

  /* If the path is a relative path without a drive specified, add the
     current directory.  */
  if (wpath[0] != L'\\'
      && !(((wpath[0] >= L'A' && wpath[0] <= L'Z')
	    || (wpath[0] >= L'a' && wpath[0] <= L'z'))
	   && wpath[1] == L':'))
    {
      wchar_t *cwd, *wpath_new;
      size_t cwdlen;
      cwd = _wgetcwd (NULL, 0);
      if (cwd == NULL)
	{
	  perror ("_wgetcwd");
	  exit (EXIT_FAILURE);
	}
      cwdlen = wcslen (cwd);
      if (cwd[cwdlen - 1] == L'\\')
	{
	  wpath_new = XNEWVEC (wchar_t, cwdlen + len + 1);
	  wmemcpy (wpath_new, cwd, cwdlen);
	  wcscpy (wpath_new + cwdlen, wpath);
	}
      else
	{
	  wpath_new = XNEWVEC (wchar_t, cwdlen + len + 2);
	  wmemcpy (wpath_new, cwd, cwdlen);
	  wpath_new[cwdlen] = L'\\';
	  wcscpy (wpath_new + cwdlen + 1, wpath);
	}
      free (cwd);
      free (wpath);
      wpath = wpath_new;
      len = wcslen (wpath);
    }

  /* The path is now absolute.  Prepend \\?\ or \\?\UNC\.  (_wgetcwd
     and _wgetdcwd do not return paths starting with \\?\; it is not
     possible for the current directory name to be so long that such a
     path would be required, because SetCurrentDirectoryW does not
     support long paths.)  */
  if (wpath[0] == L'\\')
    {
      wchar_t *wpath_new;
      assert (wpath[1] == L'\\');
      wpath_new = XNEWVEC (wchar_t, len + 7);
      wmemcpy (wpath_new, L"\\\\?\\UNC", 7);
      wcscpy (wpath_new + 7, wpath + 1);
      free (wpath);
      wpath = wpath_new;
      wpath_main = wcschr (wpath + 8, L'\\');
      if (wpath_main != NULL)
	wpath_main = wcschr (wpath_main + 1, L'\\');
      if (wpath_main == NULL)
	return wpath;
    }
  else
    {
      wchar_t *wpath_new;
      assert (wpath[1] == L':' && wpath[2] == L'\\');
      wpath_new = XNEWVEC (wchar_t, len + 5);
      wmemcpy (wpath_new, L"\\\\?\\", 4);
      wcscpy (wpath_new + 4, wpath);
      free (wpath);
      wpath = wpath_new;
      wpath_main = wpath + 6;
    }

  /* Remove . and .. components.  */
  p = wpath_main + 1;
  q = wpath_main + 1;
  while (*p != L'\0')
    {
      /* Copy the path component starting at P, or handle . or
	 .. appropriately.  Handling of .. is purely textual; this is
	 reported in GCC PR 61922 to be correct as regards symlinks,
	 unlike on Unix.  It may however result in a path that exists
	 even if the original involved nonexistent directories.  */
      assert (*p != L'\\');
      if (p[0] == L'.' && p[1] == L'.' && p[2] == L'\\')
	{
	  /* /foo/../ -> / */
	  if (q > wpath_main + 1)
	    {
	      q--;
	      assert (*q == L'\\');
	      while (q[-1] != L'\\')
		q--;
	    }
	  p += 3;
	}
      else if (p[0] == L'.' && p[1] == L'.' && p[2] == L'\0')
	{
	  /* /foo/.. (at end) -> (empty) */
	  if (q > wpath_main + 1)
	    {
	      q--;
	      assert (*q == L'\\');
	      while (q[-1] != L'\\')
		q--;
	    }
	  if (q > wpath_main + 1)
	    q--;
	  p += 2;
	}
      else if (p[0] == L'.' && p[1] == L'\\')
	/* /./ -> / */
	p += 2;
      else if (p[0] == L'.' && p[1] == L'\0')
	{
	  /* /. (at end) -> (empty) */
	  if (q > wpath_main + 1)
	    q--;
	  p++;
	}
      else
	{
	  /* Normal path component.  */
	  while (*p != L'\\' && *p != L'\0')
	    *q++ = *p++;
	  if (*p == L'\\')
	    *q++ = *p++;
	}
    }
  *q++ = L'\0';

  return wpath;
}

typedef struct
{
  DWORD dwRVAFunctionNameList;
  DWORD dwUseless1;
  DWORD dwUseless2;
  DWORD dwRVAModuleName;
  DWORD dwRVAFunctionAddressList;
} IMAGE_IMPORT_MODULE_DIRECTORY, *PIMAGE_IMPORT_MODULE_DIRECTORY;

#define IMAGE_DIRECTORY_ENTRY_IMPORT 1

/* Returns the handle for the MVCRT DLL, or NULL if it is not
   available.  */
static HMODULE
msvcrt_dll (void)
{
  static HMODULE dll = (HMODULE)(-1);
  PIMAGE_OPTIONAL_HEADER opth;
  PIMAGE_IMPORT_MODULE_DIRECTORY impdir;
  HANDLE image;

  /* After we call GetModuleHandle, DLL will be either a valid handle
     or NULL, so this check ensures that we only try to load the
     library once.  */
  if (dll != (HMODULE)(-1))
    return dll;

  dll = NULL;

  /* Check which C runtime is loaded by this executable.  Do that by
     looking at the import directory for DLLs the EXE links to that
     looks like msvcr*.  This catches e.g., `msvcr90.dll' and
     `msvcrt.dll'.  */

  image = GetModuleHandle (NULL);

  /* Get to the PE optional header.  */
  opth = (PIMAGE_OPTIONAL_HEADER)
    ((DWORD) image + ((PIMAGE_DOS_HEADER) image)->e_lfanew
     + sizeof (DWORD) + sizeof (IMAGE_FILE_HEADER));
  impdir = (PIMAGE_IMPORT_MODULE_DIRECTORY)
    (opth->ImageBase
     + opth->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

  for (; impdir->dwRVAModuleName; impdir++)
    {
      char *dllname;

      dllname = (char *) opth->ImageBase + impdir->dwRVAModuleName;
      if (strncasecmp (dllname, "msvcr", sizeof ("msvcr") - 1) == 0)
	{
	  dll = GetModuleHandle (dllname);
	  if (dll == NULL)
	    {
	      if (cygpath_log)
		cygpath_log_msg ("error: GetModuleHandle failed");
	    }
	  break;
	}
    }

  return dll;
}

/* Call the underlying MSVCRT fopen with PATH and MODE, and return
   what it returns.  */
static FILE *
msvcrt_fopen (const char *path, const char *mode)
{
  typedef FILE *(fopen_type)(const char *path,
			     const char *mode);

  static fopen_type *f = NULL;

  /* Get the address of "fopen".  */
  if (!f)
    {
      HMODULE dll = msvcrt_dll ();
      if (!dll)
	{
	  errno = ENOSYS;
	  return NULL;
	}
      f = (fopen_type *) GetProcAddress (dll, "fopen");
      if (!f)
	{
	  errno = ENOSYS;
	  return NULL;
	}
    }

  /* Call fopen.  */
  return (*f)(path, mode);
}

FILE *
fopen (const char *path, const char *mode)
{
  FILE *f;
  char win32_path[MAX_WPATH + 1];
  wchar_t *wpath = NULL, *wmode = NULL;

  /* Assume PATH is a Windows path.  */
  wpath = convert_long_path (path);
  wmode = convert_to_wide (mode);
  if (wpath != NULL && wmode != NULL)
    f = _wfopen (wpath, wmode);
  else
    f = msvcrt_fopen (path, mode);
  if (f || errno != ENOENT)
    goto out;
  /* Perhaps it is a Cygwin path?  */
  if (cygpath (path, win32_path))
    {
      free (wpath);
      wpath = convert_long_path (win32_path);
      if (wpath != NULL && wmode != NULL)
	f = _wfopen (wpath, wmode);
      else
	f = msvcrt_fopen (win32_path, mode);
    }
 out:
  free (wpath);
  free (wmode);
  return f;
}

int
open (const char *path, int oflag, ...)
{
  int fd;
  char win32_path[MAX_WPATH + 1];
  int pmode = 0;
  wchar_t *wpath = NULL;

  if ((oflag & _O_CREAT))
    {
      va_list ap;
      va_start (ap, oflag);
      pmode = va_arg (ap, int);
      va_end (ap);
    }

  /* Assume PATH is a Windows path.  */
  wpath = convert_long_path (path);
  if (wpath != NULL)
    fd = _wopen (wpath, oflag, pmode);
  else
    fd = _open (path, oflag, pmode);
  if (fd != -1 || errno != ENOENT)
    goto out;
  /* Perhaps it is a Cygwin path?  */
  if (cygpath (path, win32_path))
    {
      free (wpath);
      wpath = convert_long_path (win32_path);
      if (wpath != NULL)
	fd = _wopen (wpath, oflag, pmode);
      else
	fd = _open (win32_path, oflag, pmode);
    }
 out:
  free (wpath);
  return fd;
}

/* Given the WINDOWS_CODE (typically the result of GetLastError), set
   ERRNO to the corresponding error code.  If there is no obvious
   correspondence, ERRNO will be set to EACCES.  */
static void
set_errno_from_windows_code (DWORD windows_code)
{
  static const struct { DWORD w; int e; } mapping[] = {
    {ERROR_ACCESS_DENIED, EACCES},
    {ERROR_ACCOUNT_DISABLED, EACCES},
    {ERROR_ACCOUNT_RESTRICTION, EACCES},
    {ERROR_ALREADY_ASSIGNED, EBUSY},
    {ERROR_ALREADY_EXISTS, EEXIST},
    {ERROR_ARITHMETIC_OVERFLOW, ERANGE},
    {ERROR_BAD_COMMAND, EIO},
    {ERROR_BAD_DEVICE, ENODEV},
    {ERROR_BAD_DRIVER_LEVEL, ENXIO},
    {ERROR_BAD_EXE_FORMAT, ENOEXEC},
    {ERROR_BAD_FORMAT, ENOEXEC},
    {ERROR_BAD_LENGTH, EINVAL},
    {ERROR_BAD_PATHNAME, ENOENT},
    {ERROR_BAD_PIPE, EPIPE},
    {ERROR_BAD_UNIT, ENODEV},
    {ERROR_BAD_USERNAME, EINVAL},
    {ERROR_BROKEN_PIPE, EPIPE},
    {ERROR_BUFFER_OVERFLOW, ENOMEM},
    {ERROR_BUSY, EBUSY},
    {ERROR_BUSY_DRIVE, EBUSY},
    {ERROR_CALL_NOT_IMPLEMENTED, ENOSYS},
    {ERROR_CRC, EIO},
    {ERROR_CURRENT_DIRECTORY, EINVAL},
    {ERROR_DEVICE_IN_USE, EBUSY},
    {ERROR_DIR_NOT_EMPTY, EEXIST},
    {ERROR_DIRECTORY, ENOENT},
    {ERROR_DISK_CHANGE, EIO},
    {ERROR_DISK_FULL, ENOSPC},
    {ERROR_DRIVE_LOCKED, EBUSY},
    {ERROR_ENVVAR_NOT_FOUND, EINVAL},
    {ERROR_EXE_MARKED_INVALID, ENOEXEC},
    {ERROR_FILE_EXISTS, EEXIST},
    {ERROR_FILE_INVALID, ENODEV},
    {ERROR_FILE_NOT_FOUND, ENOENT},
    {ERROR_FILENAME_EXCED_RANGE, ENAMETOOLONG},
    {ERROR_GEN_FAILURE, EIO},
    {ERROR_HANDLE_DISK_FULL, ENOSPC},
    {ERROR_INSUFFICIENT_BUFFER,  ENOMEM},
    {ERROR_INVALID_ACCESS, EINVAL},
    {ERROR_INVALID_ADDRESS, EFAULT},
    {ERROR_INVALID_BLOCK, EFAULT},
    {ERROR_INVALID_DATA, EINVAL},
    {ERROR_INVALID_DRIVE, ENODEV},
    {ERROR_INVALID_EXE_SIGNATURE,  ENOEXEC},
    {ERROR_INVALID_FLAGS, EINVAL},
    {ERROR_INVALID_FUNCTION,  ENOSYS},
    {ERROR_INVALID_HANDLE, EBADF},
    {ERROR_INVALID_LOGON_HOURS,  EACCES},
    {ERROR_INVALID_NAME, ENOENT},
    {ERROR_INVALID_OWNER, EINVAL},
    {ERROR_INVALID_PARAMETER, EINVAL},
    {ERROR_INVALID_PASSWORD, EPERM},
    {ERROR_INVALID_PRIMARY_GROUP, EINVAL},
    {ERROR_INVALID_SIGNAL_NUMBER, EINVAL},
    {ERROR_INVALID_TARGET_HANDLE, EIO},
    {ERROR_INVALID_WORKSTATION, EACCES},
    {ERROR_IO_DEVICE, EIO},
    {ERROR_IO_INCOMPLETE, EINTR},
    {ERROR_LOCKED, EBUSY},
    {ERROR_LOGON_FAILURE, EACCES},
    {ERROR_MAPPED_ALIGNMENT, EINVAL},
    {ERROR_META_EXPANSION_TOO_LONG, E2BIG},
    {ERROR_MORE_DATA, EPIPE},
    {ERROR_NEGATIVE_SEEK, ESPIPE},
    {ERROR_NO_DATA, EPIPE},
    {ERROR_NO_MORE_SEARCH_HANDLES, EIO},
    {ERROR_NO_PROC_SLOTS, EAGAIN},
    {ERROR_NO_SUCH_PRIVILEGE, EACCES},
    {ERROR_NOACCESS, EFAULT},
    {ERROR_NONE_MAPPED, EINVAL},
    {ERROR_NOT_ENOUGH_MEMORY, ENOMEM},
    {ERROR_NOT_READY, ENODEV},
    {ERROR_NOT_SAME_DEVICE, EXDEV},
    {ERROR_OPEN_FAILED, EIO},
    {ERROR_OPERATION_ABORTED, EINTR},
    {ERROR_OUTOFMEMORY,  ENOMEM},
    {ERROR_PASSWORD_EXPIRED, EACCES},
    {ERROR_PATH_BUSY,  EBUSY},
    {ERROR_PATH_NOT_FOUND, ENOENT},
    {ERROR_PIPE_BUSY, EBUSY},
    {ERROR_PIPE_CONNECTED, EPIPE},
    {ERROR_PIPE_LISTENING, EPIPE},
    {ERROR_PIPE_NOT_CONNECTED, EPIPE},
    {ERROR_PRIVILEGE_NOT_HELD, EACCES},
    {ERROR_READ_FAULT, EIO},
    {ERROR_SEEK, ESPIPE},
    {ERROR_SEEK_ON_DEVICE, ESPIPE},
    {ERROR_SHARING_BUFFER_EXCEEDED, ENFILE},
    {ERROR_STACK_OVERFLOW, ENOMEM},
    {ERROR_SWAPERROR, ENOENT},
    {ERROR_TOO_MANY_MODULES, EMFILE},
    {ERROR_TOO_MANY_OPEN_FILES, EMFILE},
    {ERROR_UNRECOGNIZED_MEDIA,  ENXIO},
    {ERROR_UNRECOGNIZED_VOLUME,  ENODEV},
    {ERROR_WAIT_NO_CHILDREN,  ECHILD},
    {ERROR_WRITE_FAULT, EIO},
    {ERROR_WRITE_PROTECT, EROFS}
/*  MinGW does not define ETXTBSY as yet.
    {ERROR_LOCK_VIOLATION, ETXTBSY},
    {ERROR_SHARING_VIOLATION, ETXTBSY},
*/
  };

  size_t i;

  for (i = 0; i < sizeof (mapping)/sizeof (mapping[0]); ++i)
    if (mapping[i].w == windows_code)
      {
	errno = mapping[i].e;
	return;
      }

  /* Unrecognized error. Use EACCESS to have some error code,
     not misleading "No error" thing.  */
  errno = EACCES;
}

/* Call the underlying MSVCRT stat with PATH and MODE, and return
   what it returns.  */
int
msvcrt_stat (const char *path, struct stat *buffer)
{
  typedef int (stat_type)(const char *path,
			  struct stat *buffer);

  static stat_type *f = NULL;

  /* Get the address of "stat".  */
  if (!f)
    {
      HMODULE dll = msvcrt_dll ();
      if (!dll)
	{
	  errno = ENOSYS;
	  return -1;
	}
      if (sizeof (time_t) == 8)
	f = (stat_type *) GetProcAddress (dll, "_stat64i32");
      else
	{
	  f = (stat_type *) GetProcAddress (dll, "_stat32");
	  if (!f)
	    f = (stat_type *) GetProcAddress (dll, "_stat");
	}
      if (!f)
	{
	  errno = ENOSYS;
	  return -1;
	}
    }

  /* Call fopen.  */
  return (*f)(path, buffer);
}

/* Convert a Windows file time FTIME to time_t.  */

__extension__ static time_t
filetime_to_time_t (FILETIME ftime)
{
  unsigned long long t = ftime.dwHighDateTime;
  t <<= 32;
  t |= ftime.dwLowDateTime;
  t /= 10000000;
  return t - 11644473600ULL;
}

/* Implement a version of wstat that supports \\?\ paths, returning
   the appropriate result for PATH (known to be a \\?\ path) in
   *BUFFER.  */

static int
emulate_wstat (const wchar_t *path, struct stat *buffer)
{
  BOOL r;
  WIN32_FILE_ATTRIBUTE_DATA wdat;
  DWORD attr;

  r = GetFileAttributesExW (path, GetFileExInfoStandard, &wdat);
  if (r == 0)
    {
      set_errno_from_windows_code (GetLastError ());
      return -1;
    }

  buffer->st_dev = 0;
  buffer->st_ino = 0;
  attr = wdat.dwFileAttributes;
  buffer->st_mode = ((attr & FILE_ATTRIBUTE_DEVICE)
		     ? S_IFCHR
		     : (attr & FILE_ATTRIBUTE_DIRECTORY)
		     ? S_IFDIR
		     : S_IFREG);
  buffer->st_mode |= ((attr & FILE_ATTRIBUTE_READONLY)
		      ? (S_IRUSR | S_IWUSR)
		      : S_IRWXU);
  buffer->st_nlink = 1;
  buffer->st_uid = 0;
  buffer->st_gid = 0;
  buffer->st_rdev = 0;
  if (sizeof (DWORD) >= sizeof (buffer->st_size))
    {
      buffer->st_size = wdat.nFileSizeLow;
      if (wdat.nFileSizeHigh != 0
	  || buffer->st_size < 0
	  || (DWORD) buffer->st_size != wdat.nFileSizeLow)
	{
	  /* No EOVERFLOW in Windows.  */
	  errno = ERANGE;
	  return -1;
	}
    }
  else
    {
      buffer->st_size = wdat.nFileSizeHigh;
      buffer->st_size <<= 16;
      buffer->st_size <<= 16;
      buffer->st_size |= wdat.nFileSizeLow;
      if (buffer->st_size < 0)
	{
	  errno = ERANGE;
	  return -1;
	}
    }
  buffer->st_atime = filetime_to_time_t (wdat.ftLastAccessTime);
  buffer->st_mtime = filetime_to_time_t (wdat.ftLastWriteTime);
  buffer->st_ctime = filetime_to_time_t (wdat.ftCreationTime);
  return 0;
}

int
stat (const char *path, struct stat *buffer)
{
  int r;
  char win32_path[MAX_WPATH + 1];
  wchar_t *wpath = NULL;

  /* Assume PATH is a Windows path.  */
  wpath = convert_long_path (path);
  if (wpath != NULL)
    r = emulate_wstat (wpath, buffer);
  else
    r = msvcrt_stat (path, buffer);
  if (r != -1 || errno != ENOENT)
    goto out;
  /* Perhaps it is a Cygwin path?  */
  if (cygpath (path, win32_path))
    {
      free (wpath);
      wpath = convert_long_path (win32_path);
      if (wpath != NULL)
	r = emulate_wstat (wpath, buffer);
      else
	r = msvcrt_stat (win32_path, buffer);
    }
 out:
  free (wpath);
  return r;
}

int
access (const char *path, int mode)
{
  int r;
  char win32_path[MAX_WPATH + 1];
  wchar_t *wpath = NULL;

#ifdef _WIN32
  /* Some GNU tools mistakenly defined X_OK to 1 on Windows.  */
  mode = mode & ~1;
#endif
  /* Assume PATH is a Windows path.  */
  wpath = convert_long_path (path);
  if (wpath != NULL)
    r = _waccess (wpath, mode);
  else
    r = _access (path, mode);
  if (r != -1 || errno != ENOENT)
    goto out;
  /* Perhaps it is a Cygwin path?  */
  if (cygpath (path, win32_path))
    {
      free (wpath);
      wpath = convert_long_path (win32_path);
      if (wpath != NULL)
	r = _waccess (wpath, mode);
      else
	r = _access (win32_path, mode);
    }
 out:
  free (wpath);
  return r;
}

int rename (const char *oldpath, const char *newpath)
{
  BOOL r;
  int oldpath_converted = 0;
  char win32_oldpath[MAX_WPATH + 1];
  char win32_newpath[MAX_WPATH + 1];
  wchar_t *woldpath = NULL, *wnewpath = NULL;

  /* Older versions of the cygpath program called FindFirstFile, but
     not FindClose.  As a result, a long-running cygpath program ends
     up leaking these handles, and, as a result, the Windows kernel
     will not let us remove or rename things in directories.  Therefore,
     we kill the child cygpath program now.

     The defect in cygpath was corrected by this patch:

       http://cygwin.com/ml/cygwin-patches/2007-q1/msg00033.html

     but older versions of cygpath will be in use for the forseeable
     future.  */

  cygpath_close ();
  cygpath_initialized = 0;

  /* Assume all paths are Windows paths.  */
  woldpath = convert_long_path (oldpath);
  wnewpath = convert_long_path (newpath);
  if (woldpath != NULL && wnewpath != NULL)
    r = MoveFileExW (woldpath, wnewpath, MOVEFILE_REPLACE_EXISTING);
  else
    r = MoveFileEx (oldpath, newpath, MOVEFILE_REPLACE_EXISTING);
  if (r)
    goto ok;
  else if (GetLastError () != ERROR_PATH_NOT_FOUND)
    goto error;

  /* Perhaps the old path is a cygwin path?  */
  if (cygpath (oldpath, win32_oldpath))
    {
      oldpath_converted = 1;
      free (woldpath);
      woldpath = convert_long_path (win32_oldpath);
      if (woldpath != NULL && wnewpath != NULL)
	r = MoveFileExW (woldpath, wnewpath, MOVEFILE_REPLACE_EXISTING);
      else
	r = MoveFileEx (win32_oldpath, newpath, MOVEFILE_REPLACE_EXISTING);
      if (r)
	goto ok;
      else if (GetLastError () != ERROR_PATH_NOT_FOUND)
          goto error;
    }

  /* Perhaps the new path is a cygwin path?  */
  if (cygpath (newpath, win32_newpath))
    {
      free (wnewpath);
      wnewpath = convert_long_path (win32_newpath);
      if (woldpath != NULL && wnewpath != NULL)
	r = MoveFileExW (woldpath, wnewpath, MOVEFILE_REPLACE_EXISTING);
      else
	r = MoveFileEx (oldpath_converted ? win32_oldpath : oldpath,
			win32_newpath, MOVEFILE_REPLACE_EXISTING);
      if (r == TRUE)
	goto ok;
    }
error:
  free (woldpath);
  free (wnewpath);
  set_errno_from_windows_code (GetLastError ());
  return -1;
 ok:
  free (woldpath);
  free (wnewpath);
  return 0;
}

int remove (const char *pathname)
{
  int r;
  char win32_path[MAX_WPATH + 1];
  wchar_t *wpath = NULL;

  cygpath_close ();
  cygpath_initialized = 0;

  /* Assume PATH is a Windows path.  */
  wpath = convert_long_path (pathname);
  if (wpath != NULL)
    r = _wunlink (wpath);
  else
    r = _unlink (pathname);
  if (r != -1 || errno != ENOENT)
    goto out;
  /* Perhaps it is a Cygwin path?  */
  if (cygpath (pathname, win32_path))
    {
      free (wpath);
      wpath = convert_long_path (win32_path);
      if (wpath != NULL)
	r = _wunlink (wpath);
      else
	r = _unlink (win32_path);
    }
 out:
  free (wpath);
  return r;
}

int unlink(const char *pathname)
{
    return remove (pathname);
}

int
chdir (const char *path)
{
  int ret;
  char win32_path[MAX_WPATH + 1];

  /* Windows does not allow a working directory longer than MAX_PATH,
     so there is no point in converting to \\?\ notation.  */

  /* Assume PATH is a Windows path.  */
  ret = _chdir (path);
  if (ret != -1 || errno != ENOENT)
    return ret;
  /* Perhaps it is a Cygwin path?  */
  if (cygpath (path, win32_path))
    ret = _chdir (win32_path);
  return ret;
}
