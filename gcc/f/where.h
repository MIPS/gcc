/* where.h
   Copyright (C) 1995, 1999 Free Software Foundation, Inc.
   Contributed by James Craig Burley.

This file is part of GNU Fortran.

GNU Fortran is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Fortran is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Fortran; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#ifndef _H_f_where
#define _H_f_where

#include "glimits.j"
#include "top.h"

#define FFEWHERE_nameUNKNOWN ("(input file)")

/* Line number within all the source code read (in order)
   during lexing.  Starts with 1 for first line.  */
typedef unsigned long ffewhereLine;
#define ffewhereLine_f "l"
#define FFEWHERE_lineNONE 0
#define FFEWHERE_lineMAX (ULONG_MAX >> CHAR_BIT)

/* Column number within line.  Starts with 1 for first column.  */
typedef unsigned char ffewhereCol;
#define ffewhereCol_f ""
#define FFEWHERE_colNONE 0
#define FFEWHERE_colMAX UCHAR_MAX

/* Encapsulates all the info locating a given point in the source file(s)
   being compiled -- the file name, the line number within the file,
   and the column number within the line.  Currently implemented as an
   unsigned long integer, with a global (cross-source-file) line number
   starting in bit 8, and the column number in bits 7-0.  */
typedef unsigned long int ffewherePt;

/* Encapsulates information on a source file.  */
typdef struct _ffewhere_file_ *ffewhereFile
struct _ffewhere_file_
{
  /* Another file previous discovered.  */
  ffewhereFile ffewhere_file_prev_;

  /* Length of name of this file.  */
  size_t ffewhere_file_length_;

  /* Name of this file.  */
  char ffewhere_file_name_[1];
};

/* Encapsulates information on a particular inclusion of a source file.  */
typedef struct _ffewhere_incl_ *ffewhereIncl;
struct _ffewhere_incl_
{
  /* Previous inclusion.  */
  ffewhereIncl ffewhere_incl_prev_;

  /* The file that was included.  (Might be included more than once,
     so track its name info separately.)  */
  ffewhereFile ffewhere_incl_file_;

  /* Global line number at time of creation.  */
  ffewhereLine ffewhere_incl_line_;

  /* User-desired offset for within-file line numbers.  Usually 1. */
  ffewhereLine ffewhere_incl_offset_;
};

/* Return a point encapsulation of global line and column numbers.  */
static inline ffewherePt
ffewhere_pt (ffewhereLine l, ffewhereCol c)
{
  if (l > FFEWHERE_lineMAX)
    l = FFEWHERE_lineMAX;
  if (c > FFEWHERE_colMAX)
    c = FFEWHERE_colMAX;
  return (l << CHAR_BIT) | c;
}

/* Return the global line number given a point encapsulation.  */
#define ffewhere_line_pt(p) ((p) >> CHAR_BIT)

/* Return the within-line column number given a point encapsulation.  */
#define ffewhere_col_pt(p) ((p) & UCHAR_MAX)

/* Return a file encapsulation for a given file name/length.  */
ffewhereFile ffewhere_file (char *name, size_t length);

/* Return the file name given a file encapsulation.  */
#define ffewhere_file_name(f) ((f)->ffewhere_file_text_)

/* Return the length of the file name given a file encapsulation.  */
#define ffewhere_file_namelen(f) ((f)->ffewhere_file_length_)

/* Return an inclusion encapsulation given the file and global line
   number for the inclusion and, optionally, the line offset desired
   for within-file line numbers.  */
ffewhereIncl ffewhere_incl (ffewhereFile wf, ffewhereLine global_line,
			    bool have_num, ffewhereLine line_offset);

/* Find inclusion encapsulation given a global line number.  */
ffewhereIncl ffewhere_incl_find (ffewhereLine wl);

/* Return a file encapsulation given an inclusion encapsulation.  */
#define ffewhere_file_incl(i) ((i)->ffewhere_incl_file_)

/* Return the within-file line number of a global line number, given
   the corresponding inclusion encapsulation.  */
#define ffewhere_line_incl(i,l) \
  ((l) - (i)->ffewhere_incl_line_ + (i)->ffewhere_incl_offset_)

/* Initialize the module.  */
void ffewhere_initialize (void);

/* Terminate the module.  */
#define ffewhere_terminate()

#endif
