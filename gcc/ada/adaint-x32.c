/****************************************************************************
 *                                                                          *
 *                         GNAT COMPILER COMPONENTS                         *
 *                                                                          *
 *                               A D A I N T                                *
 *                                                                          *
 *                          C Implementation File                           *
 *                                                                          *
 *             Copyright (C) 2015, Free Software Foundation, Inc.           *
 *                                                                          *
 * GNAT is free software;  you can  redistribute it  and/or modify it under *
 * terms of the  GNU General Public License as published  by the Free Soft- *
 * ware  Foundation;  either version 3,  or (at your option) any later ver- *
 * sion.  GNAT is distributed in the hope that it will be useful, but WITH- *
 * OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE.                                     *
 *                                                                          *
 * As a special exception under Section 7 of GPL version 3, you are granted *
 * additional permissions described in the GCC Runtime Library Exception,   *
 * version 3.1, as published by the Free Software Foundation.               *
 *                                                                          *
 * You should have received a copy of the GNU General Public License and    *
 * a copy of the GCC Runtime Library Exception along with this program;     *
 * see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see    *
 * <http://www.gnu.org/licenses/>.                                          *
 *                                                                          *
 * GNAT was originally developed  by the GNAT team at  New York University. *
 * Extensive contributions were provided by Ada Core Technologies Inc.      *
 *                                                                          *
 ****************************************************************************/

/* This file contains those routines named by Import pragmas in
   packages in the GNAT hierarchy (especially GNAT.OS_Lib) and in
   package Osint.  Many of the subprograms in OS_Lib import standard
   library calls directly. This file contains all other routines.  */

/* Ensure access to errno is thread safe.  */
#define _REENTRANT
#define _THREAD_SAFE

#if !defined __linux__ && !defined __x86_64__ && !defined __ILP32__
#error Unsupported platform.
#endif

#ifdef IN_RTS
#include "tconfig.h"
#include "tsystem.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

/* We don't have libiberty, so use malloc.  */
#define xmalloc(S) malloc (S)
#define xrealloc(V,S) realloc (V,S)
#else
#include "config.h"
#include "system.h"
#include "version.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <utime.h>
#include <sys/wait.h>
#include <utime.h>

#include "adaint.h"

/* Define symbols O_BINARY and O_TEXT as harmless zeroes if they are not
   defined in the current system. On DOS-like systems these flags control
   whether the file is opened/created in text-translation mode (CR/LF in
   external file mapped to LF in internal file), but in Unix-like systems,
   no text translation is required, so these flags have no effect.  */

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef O_TEXT
#define O_TEXT 0
#endif

#ifndef HOST_EXECUTABLE_SUFFIX
#define HOST_EXECUTABLE_SUFFIX ""
#endif

#ifndef HOST_OBJECT_SUFFIX
#define HOST_OBJECT_SUFFIX ".o"
#endif

#ifndef PATH_SEPARATOR
#define PATH_SEPARATOR ':'
#endif

#ifndef DIR_SEPARATOR
#define DIR_SEPARATOR '/'
#endif

/* Check for cross-compilation.  */
#if defined (CROSS_COMPILE) || defined (CROSS_DIRECTORY_STRUCTURE)
#define IS_CROSS 1
int __gnat_is_cross_compiler = 1;
#else
#undef IS_CROSS
int __gnat_is_cross_compiler = 0;
#endif

char __gnat_dir_separator = DIR_SEPARATOR;

char __gnat_path_separator = PATH_SEPARATOR;

/* The GNAT_LIBRARY_TEMPLATE contains a list of expressions that define
   the base filenames that libraries specified with -lsomelib options
   may have. This is used by GNATMAKE to check whether an executable
   is up-to-date or not. The syntax is

     library_template ::= { pattern ; } pattern NUL
     pattern          ::= [ prefix ] * [ postfix ]

   These should only specify names of static libraries as it makes
   no sense to determine at link time if dynamic-link libraries are
   up to date or not. Any libraries that are not found are supposed
   to be up-to-date:

     * if they are needed but not present, the link
       will fail,

     * otherwise they are libraries in the system paths and so
       they are considered part of the system and not checked
       for that reason.

   ??? This should be part of a GNAT host-specific compiler
       file instead of being included in all user applications
       as well. This is only a temporary work-around for 3.11b.  */

#ifndef GNAT_LIBRARY_TEMPLATE
#define GNAT_LIBRARY_TEMPLATE "lib*.a"
#endif

const char *__gnat_library_template = GNAT_LIBRARY_TEMPLATE;

#include <sys/param.h>

#ifdef MAXPATHLEN
#define GNAT_MAX_PATH_LEN MAXPATHLEN
#else
#define GNAT_MAX_PATH_LEN 256
#endif

/* Used for runtime check that Ada constant File_Attributes_Size is no
   less than the actual size of struct file_attributes (see Osint
   initialization). */
int __gnat_size_of_file_attributes = sizeof (struct file_attributes);

/* The __gnat_max_path_len variable is used to export the maximum
   length of a path name to Ada code. max_path_len is also provided
   for compatibility with older GNAT versions, please do not use
   it. */

int __gnat_max_path_len = GNAT_MAX_PATH_LEN;
int max_path_len = GNAT_MAX_PATH_LEN;

/* Control whether we can use ACL on Windows.  */

int __gnat_use_acl = 1;

/* The following macro HAVE_READDIR_R should be defined if the
   system provides the routine readdir_r.
   ... but we never define it anywhere???  */
#undef HAVE_READDIR_R

#define MAYBE_TO_PTR32(argv) argv

/* Query information for the given file NAME and return it in STATBUF.
 * Returns 0 for success, or errno value for failure.
 */
static int
__gnat_stat (char *name, GNAT_STRUCT_STAT *statbuf)
{
  return GNAT_STAT (name, statbuf) == 0 ? 0 : errno;
}

static const char ATTR_UNSET = 127;

/* Reset the file attributes as if no system call had been performed */

static void
gnat_reset_attributes (struct file_attributes* attr)
{
  attr->exists     = ATTR_UNSET;
  attr->error      = EINVAL;

  attr->writable   = ATTR_UNSET;
  attr->readable   = ATTR_UNSET;
  attr->executable = ATTR_UNSET;

  attr->regular    = ATTR_UNSET;
  attr->symbolic_link = ATTR_UNSET;
  attr->directory = ATTR_UNSET;

  attr->timestamp = (OS_Time)-2;
  attr->file_length = -1;
}

void
__gnat_reset_attributes (uintptr_t attr_p)
{
  struct file_attributes* attr = (struct file_attributes *)attr_p;
  gnat_reset_attributes (attr);
}

int
__gnat_error_attributes (uintptr_t attr_p) {
  struct file_attributes* attr = (struct file_attributes *)attr_p;
  return attr->error;
}

OS_Time
__gnat_current_time (void)
{
  time_t res = time (NULL);
  return (OS_Time) res;
}

/* Return the current local time as a string in the ISO 8601 format of
   "YYYY-MM-DD HH:MM:SS.SS". The returned string is 22 + 1 (NULL) characters
   long. */

void
__gnat_current_time_string (uintptr_t result_p)
{
  char *result = (char *)result_p;
  const char *format = "%Y-%m-%d %H:%M:%S";
  /* Format string necessary to describe the ISO 8601 format */

  const time_t t_val = time (NULL);

  strftime (result, 22, format, localtime (&t_val));
  /* Convert the local time into a string following the ISO format, copying
     at most 22 characters into the result string. */

  result [19] = '.';
  result [20] = '0';
  result [21] = '0';
  /* The sub-seconds are manually set to zero since type time_t lacks the
     precision necessary for nanoseconds. */
}

void
__gnat_to_gm_time (uintptr_t p_time_p, uintptr_t p_year_p,
		   uintptr_t p_month_p, uintptr_t p_day_p,
		   uintptr_t p_hours_p, uintptr_t p_mins_p,
		   uintptr_t p_secs_p)
{
  OS_Time *p_time = (OS_Time *)p_time_p;
  int *p_year = (int *)p_year_p;
  int *p_month = (int *)p_month_p;
  int *p_day = (int *)p_day_p;
  int *p_hours = (int *)p_hours_p;
  int *p_mins = (int *)p_mins_p;
  int *p_secs = (int *)p_secs_p;
  struct tm *res;
  time_t time = (time_t) *p_time;

  res = gmtime (&time);
  if (res)
    {
      *p_year = res->tm_year;
      *p_month = res->tm_mon;
      *p_day = res->tm_mday;
      *p_hours = res->tm_hour;
      *p_mins = res->tm_min;
      *p_secs = res->tm_sec;
    }
  else
    *p_year = *p_month = *p_day = *p_hours = *p_mins = *p_secs = 0;
}

void
__gnat_to_os_time (uintptr_t p_time_p, int year, int month, int day,
		   int hours, int mins, int secs)
{
  OS_Time *p_time = (OS_Time *)p_time_p;
  struct tm v;

  v.tm_year  = year;
  v.tm_mon   = month;
  v.tm_mday  = day;
  v.tm_hour  = hours;
  v.tm_min   = mins;
  v.tm_sec   = secs;
  v.tm_isdst = -1;

  /* returns -1 of failing, this is s-os_lib Invalid_Time */

  *p_time = (OS_Time) mktime (&v);
}

/* Place the contents of the symbolic link named PATH in the buffer BUF,
   which has size BUFSIZ.  If PATH is a symbolic link, then return the number
   of characters of its content in BUF.  Otherwise, return -1.
   For systems not supporting symbolic links, always return -1.  */

int
__gnat_readlink (uintptr_t path_p ATTRIBUTE_UNUSED,
		 uintptr_t buf_p ATTRIBUTE_UNUSED,
		 size_t bufsiz ATTRIBUTE_UNUSED)
{
  char *path = (char *)path_p;
  char *buf = (char *)buf_p;
  return readlink (path, buf, bufsiz);
}

/* Creates a symbolic link named NEWPATH which contains the string OLDPATH.
   If NEWPATH exists it will NOT be overwritten.
   For systems not supporting symbolic links, always return -1.  */

int
__gnat_symlink (uintptr_t oldpath_p, uintptr_t newpath_p)
{
  char *oldpath = (char *)oldpath_p;
  char *newpath = (char *)newpath_p;
  return symlink (oldpath, newpath);
}

/* Try to lock a file, return 1 if success.  */

/* Version using link(), more secure over NFS.  */
/* See TN 6913-016 for discussion ??? */

int
__gnat_try_lock (uintptr_t dir_p, uintptr_t file_p)
{
  char *dir = (char *)dir_p;
  char *file = (char *)file_p;
  char full_path[256];
  char temp_file[256];
  GNAT_STRUCT_STAT stat_result;
  int fd;

  sprintf (full_path, "%s%c%s", dir, DIR_SEPARATOR, file);
  sprintf (temp_file, "%s%cTMP-%ld-%ld",
           dir, DIR_SEPARATOR, (long)getpid(), (long)getppid ());

  /* Create the temporary file and write the process number.  */
  fd = open (temp_file, O_CREAT | O_WRONLY, 0600);
  if (fd < 0)
    return 0;

  close (fd);

  /* Link it with the new file.  */
  link (temp_file, full_path);

  /* Count the references on the old one. If we have a count of two, then
     the link did succeed. Remove the temporary file before returning.  */
  __gnat_stat (temp_file, &stat_result);
  unlink (temp_file);
  return stat_result.st_nlink == 2;
}

/* Return the maximum file name length.  */

int
__gnat_get_maximum_file_name_length (void)
{
  return -1;
}

/* Return nonzero if file names are case sensitive.  */

static int file_names_case_sensitive_cache = -1;

int
__gnat_get_file_names_case_sensitive (void)
{
  if (file_names_case_sensitive_cache == -1)
    {
      const char *sensitive = getenv ("GNAT_FILE_NAME_CASE_SENSITIVE");

      if (sensitive != NULL
          && (sensitive[0] == '0' || sensitive[0] == '1')
          && sensitive[1] == '\0')
        file_names_case_sensitive_cache = sensitive[0] - '0';
      else
	{
	  /* By default, we suppose filesystems aren't case sensitive on
	     Windows and Darwin (but they are on arm-darwin).  */
	  file_names_case_sensitive_cache = 1;
	}
    }
  return file_names_case_sensitive_cache;
}

/* Return nonzero if environment variables are case sensitive.  */

int
__gnat_get_env_vars_case_sensitive (void)
{
  return 1;
}

char
__gnat_get_default_identifier_character_set (void)
{
  return '1';
}

/* Return the current working directory.  */

void
__gnat_get_current_dir (uintptr_t dir_p, uintptr_t length_p)
{
  char *dir = (char *)dir_p;
  int *length = (int *)length_p;
  getcwd (dir, *length);

  *length = strlen (dir);
  if (dir [*length - 1] != DIR_SEPARATOR)
    {
      dir [*length] = DIR_SEPARATOR;
      ++(*length);
    }
  dir[*length] = '\0';
}

/* Return the suffix for object files.  */

void
__gnat_get_object_suffix_ptr (uintptr_t len_p, uintptr_t value_p)
{
  int *len = (int *)len_p;
  const char **value = (const char **)value_p;
  *value = HOST_OBJECT_SUFFIX;

  if (*value == 0)
    *len = 0;
  else
    *len = strlen (*value);

  return;
}

/* Return the suffix for executable files.  */

void
__gnat_get_executable_suffix_ptr (uintptr_t len_p, uintptr_t value_p)
{
  int *len = (int *)len_p;
  const char **value = (const char **)value_p;
  *value = HOST_EXECUTABLE_SUFFIX;
  if (!*value)
    *len = 0;
  else
    *len = strlen (*value);

  return;
}

/* Return the suffix for debuggable files. Usually this is the same as the
   executable extension.  */

void
__gnat_get_debuggable_suffix_ptr (uintptr_t len_p, uintptr_t value_p)
{
  int *len = (int *)len_p;
  const char **value = (const char **)value_p;
  *value = HOST_EXECUTABLE_SUFFIX;

  if (*value == 0)
    *len = 0;
  else
    *len = strlen (*value);

  return;
}

/* Returns the OS filename and corresponding encoding.  */

void
__gnat_os_filename (uintptr_t filename_p,
		    uintptr_t w_filename_p ATTRIBUTE_UNUSED,
		    uintptr_t os_name_p,
		    uintptr_t o_length_p,
		    uintptr_t encoding_p ATTRIBUTE_UNUSED,
		    uintptr_t e_length_p)
{
  char *filename = (char *)filename_p;
  char *os_name = ( char *)os_name_p;
  int *o_length = (int *)o_length_p;
  int *e_length = (int *)e_length_p;
  strcpy (os_name, filename);
  *o_length = strlen (filename);
  *e_length = 0;
}

/* Delete a file.  */

int
__gnat_unlink (uintptr_t path_p)
{
  char *path = (char *)path_p;
  return unlink (path);
}

/* Rename a file.  */

int
__gnat_rename (uintptr_t from_p, uintptr_t to_p)
{
  char *from = (char *)from_p;
  char *to = (char *)to_p;
  return rename (from, to);
}

/* Changing directory.  */

int
__gnat_chdir (uintptr_t path_p)
{
  char *path = (char *)path_p;
  return chdir (path);
}

/* Removing a directory.  */

int
__gnat_rmdir (uintptr_t path_p)
{
  char *path = (char *)path_p;
  return rmdir (path);
}

#include <wchar.h>

int
__gnat_fputwc(int c, uintptr_t stream_p)
{
  FILE *stream = (FILE *)stream_p;
  return fputwc ((wchar_t)c, stream);
}

FILE *
__gnat_fopen (uintptr_t path_p, uintptr_t mode_p,
	      int encoding ATTRIBUTE_UNUSED)
{
  char *path = (char *)path_p;
  char *mode = (char *)mode_p;
  return GNAT_FOPEN (path, mode);
}

FILE *
__gnat_freopen (uintptr_t path_p,
		uintptr_t mode_p,
		uintptr_t stream_p,
		int encoding ATTRIBUTE_UNUSED)
{
  char *path = (char *)path_p;
  char *mode = (char *)mode_p;
  FILE *stream = (FILE *)stream_p;
  return freopen (path, mode, stream);
}

int
__gnat_open_read (uintptr_t path_p, int fmode)
{
  char *path = (char *)path_p;
  int fd;
  int o_fmode = O_BINARY;

  if (fmode)
    o_fmode = O_TEXT;

  fd = GNAT_OPEN (path, O_RDONLY | o_fmode);

  return fd < 0 ? -1 : fd;
}

#define PERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int
__gnat_open_rw (uintptr_t path_p, int fmode)
{
  char *path = (char *)path_p;
  int fd;
  int o_fmode = O_BINARY;

  if (fmode)
    o_fmode = O_TEXT;

  fd = GNAT_OPEN (path, O_RDWR | o_fmode, PERM);

  return fd < 0 ? -1 : fd;
}

int
__gnat_open_create (uintptr_t path_p, int fmode)
{
  char *path = (char *)path_p;
  int fd;
  int o_fmode = O_BINARY;

  if (fmode)
    o_fmode = O_TEXT;

  fd = GNAT_OPEN (path, O_WRONLY | O_CREAT | O_TRUNC | o_fmode, PERM);

  return fd < 0 ? -1 : fd;
}

int
__gnat_create_output_file (uintptr_t path_p)
{
  char *path = (char *)path_p;
  int fd;
  fd = GNAT_OPEN (path, O_WRONLY | O_CREAT | O_TRUNC | O_TEXT, PERM);

  return fd < 0 ? -1 : fd;
}

int
__gnat_create_output_file_new (uintptr_t path_p)
{
  char *path = (char *)path_p;
  int fd;
  fd = GNAT_OPEN (path, O_WRONLY | O_CREAT | O_TRUNC | O_TEXT | O_EXCL, PERM);

  return fd < 0 ? -1 : fd;
}

int
__gnat_open_append (uintptr_t path_p, int fmode)
{
  char *path = (char *)path_p;
  int fd;
  int o_fmode = O_BINARY;

  if (fmode)
    o_fmode = O_TEXT;

  fd = GNAT_OPEN (path, O_WRONLY | O_CREAT | O_APPEND | o_fmode, PERM);

  return fd < 0 ? -1 : fd;
}

/*  Open a new file.  Return error (-1) if the file already exists.  */

int
__gnat_open_new (uintptr_t path_p, int fmode)
{
  char *path = (char *)path_p;
  int fd;
  int o_fmode = O_BINARY;

  if (fmode)
    o_fmode = O_TEXT;

  fd = GNAT_OPEN (path, O_WRONLY | O_CREAT | O_EXCL | o_fmode, PERM);

  return fd < 0 ? -1 : fd;
}

/* Open a new temp file.  Return error (-1) if the file already exists.  */

int
__gnat_open_new_temp (uintptr_t path_p, int fmode)
{
  char *path = (char *)path_p;
  int fd;
  int o_fmode = O_BINARY;

  strcpy (path, "GNAT-XXXXXX");

#if (defined (__FreeBSD__) || defined (__NetBSD__) || defined (__OpenBSD__) \
  || defined (linux) || defined(__GLIBC__)) && !defined (__vxworks)
  return mkstemp (path);
#elif defined (__Lynx__)
  mktemp (path);
#else
  if (mktemp (path) == NULL)
    return -1;
#endif

  if (fmode)
    o_fmode = O_TEXT;

  fd = GNAT_OPEN (path, O_WRONLY | O_CREAT | O_EXCL | o_fmode, PERM);
  return fd < 0 ? -1 : fd;
}

int
__gnat_open (uintptr_t path_p, int fmode)
{
  char *path = (char *)path_p;
  int fd;

  fd = GNAT_OPEN (path, fmode, PERM);

  return fd < 0 ? -1 : fd;
}

/****************************************************************
 ** Perform a call to GNAT_STAT or GNAT_FSTAT, and extract as much information
 ** as possible from it, storing the result in a cache for later reuse
 ****************************************************************/

static void
gnat_stat_to_attr (int fd, char* name, struct file_attributes* attr)
{
  GNAT_STRUCT_STAT statbuf;
  int ret, error;

  if (fd != -1) {
    /* GNAT_FSTAT returns -1 and sets errno for failure */
    ret = GNAT_FSTAT (fd, &statbuf);
    error = ret ? errno : 0;

  } else {
    /* __gnat_stat returns errno value directly */
    error = __gnat_stat (name, &statbuf);
    ret = error ? -1 : 0;
  }

  /*
   * A missing file is reported as an attr structure with error == 0 and
   * exists == 0.
   */

  if (error == 0 || error == ENOENT)
    attr->error = 0;
  else
    attr->error = error;

  attr->regular   = (!ret && S_ISREG (statbuf.st_mode));
  attr->directory = (!ret && S_ISDIR (statbuf.st_mode));

  if (!attr->regular)
    attr->file_length = 0;
  else
    /* st_size may be 32 bits, or 64 bits which is converted to long. We
       don't return a useful value for files larger than 2 gigabytes in
       either case. */
    attr->file_length = statbuf.st_size;  /* all systems */

  attr->exists = !ret;

  /* on Windows requires extra system call, see __gnat_is_readable_file_attr */
  attr->readable   = (!ret && (statbuf.st_mode & S_IRUSR));
  attr->writable   = (!ret && (statbuf.st_mode & S_IWUSR));
  attr->executable = (!ret && (statbuf.st_mode & S_IXUSR));

  if (ret != 0) {
     attr->timestamp = (OS_Time)-1;
  } else {
     attr->timestamp = (OS_Time)statbuf.st_mtime;
  }
}

/****************************************************************
 ** Return the number of bytes in the specified file
 ****************************************************************/

static __int64
gnat_file_length_attr (int fd, char *name, struct file_attributes *attr)
{
  if (attr->file_length == -1)
    gnat_stat_to_attr (fd, name, attr);
  return attr->file_length;
}

__int64
__gnat_file_length_attr (int fd, uintptr_t name_p, uintptr_t attr_p)
{
  char *name = (char *)name_p;
  struct file_attributes *attr = (struct file_attributes *) attr_p;
  return gnat_file_length_attr (fd, name, attr);
}

__int64
__gnat_file_length (int fd)
{
  struct file_attributes attr;
  gnat_reset_attributes (&attr);
  return gnat_file_length_attr (fd, NULL, &attr);
}

long
__gnat_file_length_long (int fd)
{
  struct file_attributes attr;
  gnat_reset_attributes (&attr);
  return (long)gnat_file_length_attr (fd, NULL, &attr);
}

__int64
__gnat_named_file_length (uintptr_t name_p)
{
  char *name = (char *)name_p;
  struct file_attributes attr;
  gnat_reset_attributes (&attr);
  return gnat_file_length_attr (-1, name, &attr);
}

/* Create a temporary filename and put it in string pointed to by
   TMP_FILENAME.  */

void
__gnat_tmp_name (uintptr_t tmp_filename_p)
{
  char *tmp_filename = (char *)tmp_filename_p;
#define MAX_SAFE_PATH 1000
  char *tmpdir = getenv ("TMPDIR");

  /* If tmpdir is longer than MAX_SAFE_PATH, revert to default value to avoid
     a buffer overflow.  */
  if (tmpdir == NULL || strlen (tmpdir) > MAX_SAFE_PATH)
#ifdef __ANDROID__
    strcpy (tmp_filename, "/cache/gnat-XXXXXX");
#else
    strcpy (tmp_filename, "/tmp/gnat-XXXXXX");
#endif
  else
    sprintf (tmp_filename, "%s/gnat-XXXXXX", tmpdir);

  close (mkstemp(tmp_filename));
  tmpnam (tmp_filename);
}

/*  Open directory and returns a DIR pointer.  */

DIR*
__gnat_opendir (uintptr_t name_p)
{
  char *name = (char *)name_p;
  return opendir (name);
}

/* Read the next entry in a directory.  The returned string points somewhere
   in the buffer.  */

#if defined (sun) && defined (__SVR4)
/* For Solaris, be sure to use the 64-bit version, otherwise NFS reads may
   fail with EOVERFLOW if the server uses 64-bit cookies.  */
#define dirent dirent64
#define readdir readdir64
#endif

char *
__gnat_readdir (uintptr_t dirp_p, uintptr_t buffer_p, uintptr_t len_p)
{
  DIR *dirp = (DIR *)dirp_p;
  char *buffer = (char *)buffer_p;
  int *len = (int *)len_p;
#if defined (HAVE_READDIR_R)
  /* If possible, try to use the thread-safe version.  */
  if (readdir_r (dirp, buffer) != NULL)
    {
      *len = strlen (((struct dirent*) buffer)->d_name);
      return ((struct dirent*) buffer)->d_name;
    }
  else
    return NULL;

#else
  struct dirent *dirent = (struct dirent *) readdir (dirp);

  if (dirent != NULL)
    {
      strcpy (buffer, dirent->d_name);
      *len = strlen (buffer);
      return buffer;
    }
  else
    return NULL;

#endif
}

/* Close a directory entry.  */

int
__gnat_closedir (uintptr_t dirp_p)
{
  DIR *dirp = (DIR *)dirp_p;
  return closedir (dirp);
}

/* Returns 1 if readdir is thread safe, 0 otherwise.  */

int
__gnat_readdir_is_thread_safe (void)
{
#ifdef HAVE_READDIR_R
  return 1;
#else
  return 0;
#endif
}

/* Return a GNAT time stamp given a file name.  */

static OS_Time
gnat_file_time_name_attr (char *name, struct file_attributes *attr)
{
  if (attr->timestamp == (OS_Time)-2) {
    gnat_stat_to_attr (-1, name, attr);
  }
  return attr->timestamp;
}

OS_Time
__gnat_file_time_name_attr (uintptr_t name_p, uintptr_t attr_p)
{
  char *name = (char *)name_p;
  struct file_attributes *attr = (struct file_attributes *) attr_p;
  return gnat_file_time_name_attr (name, attr);
}

OS_Time
__gnat_file_time_name (uintptr_t name_p)
{
  char *name = (char *)name_p;
   struct file_attributes attr;
   gnat_reset_attributes (&attr);
   return gnat_file_time_name_attr (name, &attr);
}

/* Return a GNAT time stamp given a file descriptor.  */

static OS_Time
__gnat_file_time_fd_attr (int fd, struct file_attributes *attr)
{
  if (attr->timestamp == (OS_Time)-2) {
    gnat_stat_to_attr (fd, NULL, attr);
  }

  return attr->timestamp;
}

OS_Time
__gnat_file_time_fd (int fd)
{
   struct file_attributes attr;
   gnat_reset_attributes (&attr);
   return __gnat_file_time_fd_attr (fd, &attr);
}

/* Set the file time stamp.  */

void
__gnat_set_file_time_name (uintptr_t name_p, time_t time_stamp)
{
  char *name = (char *)name_p;
  struct utimbuf utimbuf;
  time_t t;

  /* Set modification time to requested time.  */
  utimbuf.modtime = time_stamp;

  /* Set access time to now in local time.  */
  t = time ((time_t) 0);
  utimbuf.actime = mktime (localtime (&t));

  utime (name, &utimbuf);
}

/* Get the list of installed standard libraries from the
   HKEY_LOCAL_MACHINE\SOFTWARE\Ada Core Technologies\GNAT\Standard Libraries
   key.  */

char *
__gnat_get_libraries_from_registry (void)
{
  char *result = (char *) xmalloc (1);
  result[0] = '\0';
  return result;
}

/*************************************************************************
 ** Check whether a file exists
 *************************************************************************/

static int
gnat_file_exists_attr (char *name, struct file_attributes *attr)
{
  if (attr->exists == ATTR_UNSET)
    gnat_stat_to_attr (-1, name, attr);

  return attr->exists;
}

int
__gnat_file_exists_attr (uintptr_t name_p, uintptr_t attr_p)
{
  char *name = (char *)name_p;
  struct file_attributes *attr = (struct file_attributes *)attr_p;
  return gnat_file_exists_attr (name, attr);
}

int
__gnat_file_exists (uintptr_t name_p)
{
  char *name = (char *)name_p;
  struct file_attributes attr;
  gnat_reset_attributes (&attr);
  return gnat_file_exists_attr (name, &attr);
}

/**********************************************************************
 ** Whether name is an absolute path
 **********************************************************************/

static int
gnat_is_absolute_path (char *name, int length)
{
  return (length != 0) && (*name == '/' || *name == DIR_SEPARATOR);
}

int
__gnat_is_absolute_path (uintptr_t name_p, int length)
{
  char *name = (char *)name_p;
  return gnat_is_absolute_path (name, length);
}

static int
gnat_is_regular_file_attr (char *name, struct file_attributes *attr)
{
  if (attr->regular == ATTR_UNSET)
    gnat_stat_to_attr (-1, name, attr);
  return attr->regular;
}

int
__gnat_is_regular_file_attr (uintptr_t name_p, uintptr_t attr_p)
{
  char *name = (char *)name_p;
  struct file_attributes *attr = (struct file_attributes *) attr_p;
  return gnat_is_regular_file_attr (name, attr);
}

static int
gnat_is_regular_file (char *name)
{
  struct file_attributes attr;
  gnat_reset_attributes (&attr);
  return gnat_is_regular_file_attr (name, &attr);
}

int
__gnat_is_regular_file (uintptr_t name_p)
{
  char *name = (char *)name_p;
  return gnat_is_regular_file (name);
}

int
__gnat_is_regular_file_fd (int fd)
{
  int ret;
  GNAT_STRUCT_STAT statbuf;

  ret = GNAT_FSTAT (fd, &statbuf);
  return (!ret && S_ISREG (statbuf.st_mode));
}

static int
gnat_is_directory_attr (char *name, struct file_attributes *attr)
{
  if (attr->directory == ATTR_UNSET)
    gnat_stat_to_attr (-1, name, attr);
  return attr->directory;
}

int
__gnat_is_directory_attr (uintptr_t name_p, uintptr_t attr_p)
{
  char *name = (char *)name_p;
  struct file_attributes *attr = (struct file_attributes *) attr_p;
  return gnat_is_directory_attr (name, attr);
}

int
__gnat_is_directory (uintptr_t name_p)
{
  char *name = (char *)name_p;
  struct file_attributes attr;
  gnat_reset_attributes (&attr);
  return gnat_is_directory_attr (name, &attr);
}

static int
gnat_is_readable_file_attr (char *name, struct file_attributes *attr)
{
  if (attr->readable == ATTR_UNSET)
    gnat_stat_to_attr (-1, name, attr);
  return attr->readable;
}

int
__gnat_is_readable_file_attr (uintptr_t name_p, uintptr_t attr_p)
{
  char *name = (char *)name_p;
  struct file_attributes *attr = (struct file_attributes *) attr_p;
  return gnat_is_readable_file_attr (name, attr);
}

int
__gnat_is_readable_file (uintptr_t name_p)
{
  char *name = (char *)name_p;
  struct file_attributes attr;
  gnat_reset_attributes (&attr);
  return gnat_is_readable_file_attr (name, &attr);
}

static int
gnat_is_writable_file_attr (char *name, struct file_attributes *attr)
{
  if (attr->writable == ATTR_UNSET)
    gnat_stat_to_attr (-1, name, attr);
  return attr->writable;
}

int
__gnat_is_writable_file_attr (uintptr_t name_p, uintptr_t attr_p)
{
  char *name = (char *)name_p;
  struct file_attributes *attr = (struct file_attributes *) attr_p;
  return gnat_is_writable_file_attr (name, attr);
}

int
__gnat_is_writable_file (uintptr_t name_p)
{
  char *name = (char *)name_p;
  struct file_attributes attr;
  gnat_reset_attributes (&attr);
  return gnat_is_writable_file_attr (name, &attr);
}

static int
gnat_is_executable_file_attr (char *name, struct file_attributes *attr)
{
  if (attr->executable == ATTR_UNSET)
    gnat_stat_to_attr (-1, name, attr);
  return attr->regular && attr->executable;
}

int
__gnat_is_executable_file_attr (uintptr_t name_p, uintptr_t attr_p)
{
  char *name = (char *)name_p;
  struct file_attributes *attr = (struct file_attributes *) attr_p;
  return gnat_is_executable_file_attr (name, attr);
}

static int
gnat_is_executable_file (char *name)
{
  struct file_attributes attr;
  gnat_reset_attributes (&attr);
  return gnat_is_executable_file_attr (name, &attr);
}

int
__gnat_is_executable_file (uintptr_t name_p)
{
  char *name = (char *)name_p;
  return gnat_is_executable_file (name);
}

void
__gnat_set_writable (uintptr_t name_p)
{
  char *name = (char *)name_p;
  GNAT_STRUCT_STAT statbuf;
  if (GNAT_STAT (name, &statbuf) == 0)
    {
      statbuf.st_mode = statbuf.st_mode | S_IWUSR;
      chmod (name, statbuf.st_mode);
    }
}

/* must match definition in s-os_lib.ads */
#define S_OWNER  1
#define S_GROUP  2
#define S_OTHERS 4

void
__gnat_set_executable (uintptr_t name_p, int mode)
{
  char *name = (char *)name_p;
  GNAT_STRUCT_STAT statbuf;

  if (GNAT_STAT (name, &statbuf) == 0)
    {
      if (mode & S_OWNER)
        statbuf.st_mode = statbuf.st_mode | S_IXUSR;
      if (mode & S_GROUP)
        statbuf.st_mode = statbuf.st_mode | S_IXGRP;
      if (mode & S_OTHERS)
        statbuf.st_mode = statbuf.st_mode | S_IXOTH;
      chmod (name, statbuf.st_mode);
    }
}

void
__gnat_set_non_writable (uintptr_t name_p)
{
  char *name = (char *)name_p;
  GNAT_STRUCT_STAT statbuf;

  if (GNAT_STAT (name, &statbuf) == 0)
    {
      statbuf.st_mode = statbuf.st_mode & 07577;
      chmod (name, statbuf.st_mode);
    }
}

void
__gnat_set_readable (uintptr_t name_p)
{
  char *name = (char *)name_p;
  GNAT_STRUCT_STAT statbuf;

  if (GNAT_STAT (name, &statbuf) == 0)
    {
      chmod (name, statbuf.st_mode | S_IREAD);
    }
}

void
__gnat_set_non_readable (uintptr_t name_p)
{
  char *name = (char *)name_p;
  GNAT_STRUCT_STAT statbuf;

  if (GNAT_STAT (name, &statbuf) == 0)
    {
      chmod (name, statbuf.st_mode & (~S_IREAD));
    }
}

static int
gnat_is_symbolic_link_attr (char *name, struct file_attributes *attr)
{
  if (attr->symbolic_link == ATTR_UNSET)
    {
      int ret;
      GNAT_STRUCT_STAT statbuf;
      ret = GNAT_LSTAT (name, &statbuf);
      attr->symbolic_link = (!ret && S_ISLNK (statbuf.st_mode));
    }
  return attr->symbolic_link;
}

int
__gnat_is_symbolic_link_attr (uintptr_t name_p, uintptr_t attr_p)
{
  char *name = (char *)name_p;
  struct file_attributes *attr = (struct file_attributes *) attr_p;
  return gnat_is_symbolic_link_attr (name, attr);
}

int
__gnat_is_symbolic_link (uintptr_t name_p)
{
  char *name = (char *)name_p;
  struct file_attributes attr;
  gnat_reset_attributes (&attr);
  return gnat_is_symbolic_link_attr (name, &attr);
}

int
__gnat_portable_spawn (uintptr_t args_p)
{
  char **args = (char **)args_p;
  int status = 0;
  int finished;
  int pid;

  pid = fork ();
  if (pid < 0)
    return -1;

  if (pid == 0)
    {
      /* The child. */
      if (execv (args[0], MAYBE_TO_PTR32 (args)) != 0)
	_exit (1);
    }

  /* The parent.  */
  finished = waitpid (pid, &status, 0);

  if (finished != pid || WIFEXITED (status) == 0)
    return -1;

  return WEXITSTATUS (status);
}

/* Create a copy of the given file descriptor.
   Return -1 if an error occurred.  */

int
__gnat_dup (int oldfd)
{
  return dup (oldfd);
}

/* Make newfd be the copy of oldfd, closing newfd first if necessary.
   Return -1 if an error occurred.  */

int
__gnat_dup2 (int oldfd ATTRIBUTE_UNUSED, int newfd ATTRIBUTE_UNUSED)
{
  return dup2 (oldfd, newfd);
}

int
__gnat_number_of_cpus (void)
{
  int cores = (int) sysconf (_SC_NPROCESSORS_ONLN);
  return cores;
}

int
__gnat_portable_no_block_spawn (uintptr_t args_p)
{
  char **args = (char **)args_p;
  int pid = fork ();

  if (pid == 0)
    {
      /* The child.  */
      if (execv (args[0], MAYBE_TO_PTR32 (args)) != 0)
	_exit (1);
    }

  return pid;
}

int
__gnat_portable_wait (uintptr_t process_status_p)
{
  int *process_status = (int *)process_status_p;
  int status = 0;
  int pid = 0;

  pid = waitpid (-1, &status, 0);
  status = status & 0xffff;

  *process_status = status;
  return pid;
}

void
__gnat_os_exit (int status)
{
  exit (status);
}

/* Locate file on path, that matches a predicate */

static char *
gnat_locate_file_with_predicate (char *file_name,
				 char *path_val,
				 int (*predicate)(char *))
{
  char *ptr;
  char *file_path = (char *) alloca (strlen (file_name) + 1);
  int absolute;

  /* Return immediately if file_name is empty */

  if (*file_name == '\0')
    return 0;

  /* Remove quotes around file_name if present */

  ptr = file_name;
  if (*ptr == '"')
    ptr++;

  strcpy (file_path, ptr);

  ptr = file_path + strlen (file_path) - 1;

  if (*ptr == '"')
    *ptr = '\0';

  /* Handle absolute pathnames.  */

  absolute = gnat_is_absolute_path (file_path, strlen (file_name));

  if (absolute)
    {
     if (predicate (file_path))
       return xstrdup (file_path);

      return 0;
    }

  /* If file_name include directory separator(s), try it first as
     a path name relative to the current directory */
  for (ptr = file_name; *ptr && *ptr != '/' && *ptr != DIR_SEPARATOR; ptr++)
    ;

  if (*ptr != 0)
    {
      if (predicate (file_name))
        return xstrdup (file_name);
    }

  if (path_val == 0)
    return 0;

  {
    /* The result has to be smaller than path_val + file_name.  */
    char *file_path =
      (char *) alloca (strlen (path_val) + strlen (file_name) + 2);

    for (;;)
      {
      /* Skip the starting quote */

      if (*path_val == '"')
	path_val++;

      for (ptr = file_path; *path_val && *path_val != PATH_SEPARATOR; )
	*ptr++ = *path_val++;

      /* If directory is empty, it is the current directory*/

      if (ptr == file_path)
        {
         *ptr = '.';
        }
      else
        ptr--;

      /* Skip the ending quote */

      if (*ptr == '"')
	ptr--;

      if (*ptr != '/' && *ptr != DIR_SEPARATOR)
        *++ptr = DIR_SEPARATOR;

      strcpy (++ptr, file_name);

      if (predicate (file_path))
        return xstrdup (file_path);

      if (*path_val == 0)
        return 0;

      /* Skip path separator */

      path_val++;
      }
  }

  return 0;
}

/* Locate an executable file, give a Path value.  */

static char *
gnat_locate_executable_file (char *file_name, char *path_val)
{
  return gnat_locate_file_with_predicate
    (file_name, path_val, &gnat_is_executable_file);
}

/* Locate a regular file, give a Path value.  */

char *
__gnat_locate_regular_file (uintptr_t file_name_p, uintptr_t path_val_p)
{
  char *file_name = (char *)file_name_p;
  char *path_val = (char *)path_val_p;
  return gnat_locate_file_with_predicate
    (file_name, path_val, &gnat_is_regular_file);
}

/* Locate an executable given a Path argument. This routine is only used by
   gnatbl and should not be used otherwise.  Use locate_exec_on_path
   instead.  */

static char *
gnat_locate_exec (char *exec_name, char *path_val)
{
  char *ptr;
  if (!strstr (exec_name, HOST_EXECUTABLE_SUFFIX))
    {
      char *full_exec_name =
        (char *) alloca
	  (strlen (exec_name) + strlen (HOST_EXECUTABLE_SUFFIX) + 1);

      strcpy (full_exec_name, exec_name);
      strcat (full_exec_name, HOST_EXECUTABLE_SUFFIX);
      ptr = gnat_locate_executable_file (full_exec_name, path_val);

      if (ptr == 0)
         return gnat_locate_executable_file (exec_name, path_val);
      return ptr;
    }
  else
    return gnat_locate_executable_file (exec_name, path_val);
}

char *
__gnat_locate_exec (uintptr_t exec_name_p, uintptr_t path_val_p)
{
  char *exec_name = (char *)exec_name_p;
  char *path_val = (char *)path_val_p;
  return gnat_locate_exec (exec_name, path_val);
}

/* Locate an executable using the Systems default PATH.  */

char *
__gnat_locate_exec_on_path (uintptr_t exec_name_p)
{
  char *exec_name = (char *)exec_name_p;
  char *apath_val;

  char *path_val = getenv ("PATH");

  if (path_val == NULL) return NULL;
  apath_val = (char *) alloca (strlen (path_val) + 1);
  strcpy (apath_val, path_val);
  return gnat_locate_exec (exec_name, apath_val);
}

/* Dummy functions for Osint import for non-VMS systems.
   ??? To be removed.  */

int
__gnat_to_canonical_file_list_init (uintptr_t dirspec ATTRIBUTE_UNUSED,
				    int onlydirs ATTRIBUTE_UNUSED)
{
  return 0;
}

char *
__gnat_to_canonical_file_list_next (void)
{
  static char empty[] = "";
  return empty;
}

void
__gnat_to_canonical_file_list_free (void)
{
}

char *
__gnat_to_canonical_dir_spec (uintptr_t dirspec_p,
			      int prefixflag ATTRIBUTE_UNUSED)
{
  char *dirspec = (char *)dirspec_p;
  return dirspec;
}

char *
__gnat_to_canonical_file_spec (uintptr_t filespec_p)
{
  char *filespec = (char *)filespec_p;
  return filespec;
}

char *
__gnat_to_canonical_path_spec (uintptr_t pathspec_p)
{
  char *pathspec = (char *)pathspec_p;
  return pathspec;
}

char *
__gnat_to_host_dir_spec (uintptr_t dirspec_p,
			 int prefixflag ATTRIBUTE_UNUSED)
{
  char *dirspec = (char *)dirspec_p;
  return dirspec;
}

char *
__gnat_to_host_file_spec (uintptr_t filespec_p)
{
  char *filespec = (char *)filespec_p;
  return filespec;
}

void
__gnat_adjust_os_resource_limits (void)
{
}

int __gnat_argument_needs_quote = 0;

/* This option is used to enable/disable object files handling from the
   binder file by the GNAT Project module. For example, this is disabled on
   Windows (prior to GCC 3.4) as it is already done by the mdll module.
   Stating with GCC 3.4 the shared libraries are not based on mdll
   anymore as it uses the GCC's -shared option  */
int __gnat_prj_add_obj_files = 1;

/* char used as prefix/suffix for environment variables */
char __gnat_environment_char = '$';

/* This functions copy the file attributes from a source file to a
   destination file.

   mode = 0  : In this mode copy only the file time stamps (last access and
               last modification time stamps).

   mode = 1  : In this mode, time stamps and read/write/execute attributes are
               copied.

   Returns 0 if operation was successful and -1 in case of error. */

int
__gnat_copy_attribs (uintptr_t from_p, uintptr_t to_p, int mode)
{
  char *from = (char *)from_p;
  char *to = (char *)to_p;
  GNAT_STRUCT_STAT fbuf;
  struct utimbuf tbuf;

  if (GNAT_STAT (from, &fbuf) == -1)
    {
      return -1;
    }

  tbuf.actime = fbuf.st_atime;
  tbuf.modtime = fbuf.st_mtime;

  if (utime (to, &tbuf) == -1)
    {
      return -1;
    }

  if (mode == 1)
    {
      if (chmod (to, fbuf.st_mode) == -1)
	{
	  return -1;
	}
    }

  return 0;
}

int
__gnat_lseek (int fd, long offset, int whence)
{
  return (int) lseek (fd, offset, whence);
}

/* This function returns the major version number of GCC being used.  */
int
get_gcc_version (void)
{
#ifdef IN_RTS
  return __GNUC__;
#else
  return (int) (version_string[0] - '0');
#endif
}

/*
 * Set Close_On_Exec as indicated.
 * Note: this is used for both GNAT.OS_Lib and GNAT.Sockets.
 */

int
__gnat_set_close_on_exec (int fd ATTRIBUTE_UNUSED,
                          int close_on_exec_p ATTRIBUTE_UNUSED)
{
  int flags = fcntl (fd, F_GETFD, 0);
  if (flags < 0)
    return flags;
  if (close_on_exec_p)
    flags |= FD_CLOEXEC;
  else
    flags &= ~FD_CLOEXEC;
  return fcntl (fd, F_SETFD, flags);
}

/* Indicates if platforms supports automatic initialization through the
   constructor mechanism */
int
__gnat_binder_supports_auto_init (void)
{
  return 1;
}

/* Indicates that Stand-Alone Libraries are automatically initialized through
   the constructor mechanism */
int
__gnat_sals_init_using_constructors (void)
{
   return 1;
}

#if defined (__ANDROID__)

#include <pthread.h>

void *
__gnat_lwp_self (void)
{
   return (void *) pthread_self ();
}

#elif defined (linux)
/* There is no function in the glibc to retrieve the LWP of the current
   thread. We need to do a system call in order to retrieve this
   information. */
#include <sys/syscall.h>
void *
__gnat_lwp_self (void)
{
   return (void *) syscall (__NR_gettid);
}

#include <sched.h>

/* glibc versions earlier than 2.7 do not define the routines to handle
   dynamically allocated CPU sets. For these targets, we use the static
   versions. */

#ifdef CPU_ALLOC

/* Dynamic cpu sets */

cpu_set_t *
__gnat_cpu_alloc (size_t count)
{
  return CPU_ALLOC (count);
}

size_t
__gnat_cpu_alloc_size (size_t count)
{
  return CPU_ALLOC_SIZE (count);
}

void
__gnat_cpu_free (uintptr_t set_p)
{
  cpu_set_t *set = (cpu_set_t *)set_p;
  CPU_FREE (set);
}

void
__gnat_cpu_zero (size_t count, uintptr_t set_p)
{
  cpu_set_t *set = (cpu_set_t *)set_p;
  CPU_ZERO_S (count, set);
}

void
__gnat_cpu_set (int cpu, size_t count, uintptr_t set_p)
{
  cpu_set_t *set = (cpu_set_t *)set_p;
  /* Ada handles CPU numbers starting from 1, while C identifies the first
     CPU by a 0, so we need to adjust. */
  CPU_SET_S (cpu - 1, count, set);
}

#else /* !CPU_ALLOC */

/* Static cpu sets */

cpu_set_t *
__gnat_cpu_alloc (size_t count ATTRIBUTE_UNUSED)
{
  return (cpu_set_t *) xmalloc (sizeof (cpu_set_t));
}

size_t
__gnat_cpu_alloc_size (size_t count ATTRIBUTE_UNUSED)
{
  return sizeof (cpu_set_t);
}

void
__gnat_cpu_free (uintptr_t set_p)
{
  cpu_set_t *set = (cpu_set_t *)set_p;
  free (set);
}

void
__gnat_cpu_zero (size_t count ATTRIBUTE_UNUSED, uintptr_t set_p)
{
  cpu_set_t *set = (cpu_set_t *)set_p;
  CPU_ZERO (set);
}

void
__gnat_cpu_set (int cpu, size_t count ATTRIBUTE_UNUSED, uintptr_t set_p)
{
  cpu_set_t *set = (cpu_set_t *)set_p;
  /* Ada handles CPU numbers starting from 1, while C identifies the first
     CPU by a 0, so we need to adjust. */
  CPU_SET (cpu - 1, set);
}
#endif /* !CPU_ALLOC */
#endif /* linux */

/* Return the load address of the executable, or 0 if not known.  In the
   specific case of error, (void *)-1 can be returned. Beware: this unit may
   be in a shared library.  As low-level units are needed, we allow #include
   here.  */

#if defined (__APPLE__)
#include <mach-o/dyld.h>
#elif 0 && defined (__linux__)
#include <link.h>
#endif

const void *
__gnat_get_executable_load_address (void)
{
#if defined (__APPLE__)
  return _dyld_get_image_header (0);

#elif 0 && defined (__linux__)
  /* Currently disabled as it needs at least -ldl.  */
  struct link_map *map = _r_debug.r_map;

  return (const void *)map->l_addr;

#else
  return NULL;
#endif
}

size_t
__gnat_strlen (uintptr_t s)
{
  return strlen ((const char *)s);
}

char *
__gnat_strncpy (uintptr_t dest, uintptr_t src, size_t n)
{
  return strncpy ((char *)dest, (const char *)src, n);
}

void *
__gnat_memcpy (uintptr_t dest, uintptr_t src, size_t n)
{
  return memcpy ((void *)dest, (const void *)src, n);
}

void *
__gnat_memmove (uintptr_t dest, uintptr_t src, size_t n)
{
  return memmove ((void *)dest, (const void *)src, n);
}

int
__gnat_atoi (uintptr_t nptr)
{
  return atoi ((const char *)nptr);
}

ssize_t
__gnat_read (int fd, uintptr_t buf, size_t count)
{
  return read (fd, (void *)buf, count);
}

ssize_t
__gnat_write (int fd, uintptr_t buf, size_t count)
{
  return write (fd, (const void *)buf, count);
}

char *
__gnat_x32_getenv (uintptr_t name)
{
  return getenv ((const char *)name);
}

char *
__gnat_mktemp (uintptr_t template)
{
  return mktemp ((char *)template);
}

char *
__gnat_tmpnam (uintptr_t s)
{
  return tmpnam ((char *)s);
}

void
__gnat_clearerr (uintptr_t stream)
{
  clearerr ((FILE *)stream);
}

int
__gnat_fclose (uintptr_t stream)
{
  return fclose ((FILE *)stream);
}

FILE *
__gnat_fdopen (int fd, uintptr_t mode)
{
  return fdopen (fd, (const char *)mode);
}

int
__gnat_fflush (uintptr_t stream)
{
  return fflush ((FILE *)stream);
}

int
__gnat_fgetc (uintptr_t stream)
{
  return fgetc ((FILE *)stream);
}

char *
__gnat_fgets (uintptr_t s, int size, uintptr_t stream)
{
  return fgets ((char *)s, size, (FILE *)stream);
}

int
__gnat_fputc (int c, uintptr_t stream)
{
  return fputc (c, (FILE *)stream);
}

int
__gnat_fputs (uintptr_t s, uintptr_t stream)
{
  return fputs ((char *)s, (FILE *)stream);
}

int
__gnat_ungetc (int c, uintptr_t stream)
{
  return ungetc (c, (FILE *)stream);
}

int
__gnat_setvbuf (uintptr_t stream, uintptr_t buf, int mode, size_t size)
{
  return setvbuf ((FILE *)stream, (char *)buf, mode, size);
}

int
__gnat_fseek (uintptr_t stream, long offset, int whence)
{
  return fseek ((FILE *)stream, offset, whence);
}

void
__gnat_rewind (uintptr_t stream)
{
  rewind ((FILE *)stream);
}

int
__gnat_pclose (uintptr_t stream)
{
  return pclose ((FILE *)stream);
}

FILE *
__gnat_popen (uintptr_t command, uintptr_t type)
{
  return popen ((const char *)command, (const char *)type);
}

void *
__gnat_x32_realloc (uintptr_t ptr, size_t size)
{
  return realloc ((void *)ptr, size);
}

void
__gnat_x32_free (uintptr_t ptr)
{
  free ((void *)ptr);
}

#ifdef __cplusplus
}
#endif
