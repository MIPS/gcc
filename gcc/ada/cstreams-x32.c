/****************************************************************************
 *                                                                          *
 *                          GNAT RUN-TIME COMPONENTS                        *
 *                                                                          *
 *                              C S T R E A M S                             *
 *                                                                          *
 *              Auxiliary C functions for Interfaces.C.Streams              *
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

/* Routines required for implementing routines in Interfaces.C.Streams.  */

#if !defined __linux__ && !defined __x86_64__ && !defined __ILP32__
#error Unsupported platform.
#endif

#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#define _FILE_OFFSET_BITS 64
/* the define above will make off_t a 64bit type on GNU/Linux */

#include <stdio.h>
#include <sys/types.h>

#ifdef IN_RTS
#include "tconfig.h"
#include "tsystem.h"
#include <sys/stat.h>
#else
#include "config.h"
#include "system.h"
#endif

#include "adaint.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef linux
/* Don't use macros on GNU/Linux since they cause incompatible changes between
   glibc 2.0 and 2.1 */

#ifdef stderr
#  undef stderr
#endif
#ifdef stdin
#  undef stdin
#endif
#ifdef stdout
#  undef stdout
#endif

#endif

int
__gnat_feof (uintptr_t stream_p)
{
  FILE *stream = (FILE *)stream_p;
  return (feof (stream));
}

int
__gnat_ferror (uintptr_t stream_p)
{
  FILE *stream = (FILE *)stream_p;
  return (ferror (stream));
}

int
__gnat_fileno (uintptr_t stream_p)
{
  FILE *stream = (FILE *)stream_p;
  return (fileno (stream));
}

/* on some systems, the constants for seek are not defined, if so, then
   provide the conventional definitions */

#ifndef SEEK_SET
#define SEEK_SET 0  /* Set file pointer to offset                           */
#define SEEK_CUR 1  /* Set file pointer to its current value plus offset    */
#define SEEK_END 2  /* Set file pointer to the size of the file plus offset */
#endif

/* if L_tmpnam is not set, use a large number that should be safe */
#ifndef L_tmpnam
#define L_tmpnam 256
#endif

int    __gnat_constant_eof      = EOF;
int    __gnat_constant_iofbf    = _IOFBF;
int    __gnat_constant_iolbf    = _IOLBF;
int    __gnat_constant_ionbf    = _IONBF;
int    __gnat_constant_l_tmpnam = L_tmpnam;
int    __gnat_constant_seek_cur = SEEK_CUR;
int    __gnat_constant_seek_end = SEEK_END;
int    __gnat_constant_seek_set = SEEK_SET;

FILE *
__gnat_constant_stderr (void)
{
  return stderr;
}

FILE *
__gnat_constant_stdin (void)
{
  return stdin;
}

FILE *
__gnat_constant_stdout (void)
{
  return stdout;
}

char *
__gnat_full_name (uintptr_t nam_p, uintptr_t buffer_p)
{
  char *nam = (char *)nam_p;
  char *buffer = (char *)buffer_p;

  /* Use realpath function which resolves links and references to . and ..
     on those Unix systems that support it. Note that GNU/Linux provides it but
     cannot handle more than 5 symbolic links in a full name, so we use the
     getcwd approach instead. */
  realpath (nam, buffer);
  if (nam[0] != '/')
    {
      char *p = getcwd (buffer, __gnat_max_path_len);

      if (p == 0)
	{
	  buffer[0] = '\0';
	  return 0;
	}


      /* If the name returned is an absolute path, it is safe to append '/'
	 to the path and concatenate the name of the file. */
      if (buffer[0] == '/')
	strcat (buffer, "/");

      strcat (buffer, nam);
    }
  else
    strcpy (buffer, nam);

  return buffer;
}

#if defined(linux) || defined(sun) \
  || defined (__FreeBSD__) || defined(__APPLE__)
/* section for platforms having ftello/fseeko */

__int64
__gnat_ftell64 (uintptr_t stream_p)
{
  FILE *stream = (FILE *)stream_p;
  return (__int64)ftello (stream);
}

int
__gnat_fseek64 (uintptr_t stream_p, __int64 offset, int origin)
{
  FILE *stream = (FILE *)stream_p;
  /* make sure that the offset is not bigger than the OS off_t, if so return
     with error as this mean that we are trying to handle files larger than
     2Gb on a patform not supporting it. */
  if ((off_t)offset == offset)
    return fseeko (stream, (off_t) offset, origin);
  else
    return -1;
}

#else

__int64
__gnat_ftell64 (uintptr_t stream_p)
{
  FILE *stream = (FILE *)stream_p;
  return (__int64)ftell (stream);
}

int
__gnat_fseek64 (uintptr_t stream_p, __int64 offset, int origin)
{
  FILE *stream = (FILE *)stream_p;
  /* make sure that the offset is not bigger than the OS off_t, if so return
     with error as this mean that we are trying to handle files larger than
     2Gb on a patform not supporting it. */
  if ((off_t)offset == offset)
    return fseek (stream, (off_t) offset, origin);
  else
    return -1;
}
#endif

#ifdef __cplusplus
}
#endif
