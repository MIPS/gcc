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

typedef unsigned long int ffewhere;

typdef struct _ffewhere_file_
{
  size_t ffewhere_file_length_;
  char ffewhere_file_text_[1];
} *ffewhereFile;

typedef struct _ffewhere_incl_
{
  /* The file that was included.  (Might be included more than once,
     so track its name info separately.)  */
  ffewhereFile ffewhere_incl_file_;

  /* ffelex_line_number() at time of creation.  */
  ffewhereLine ffewhere_incl_line_;

  /* User-desired offset (usually 1). */
  ffewhereLine ffewhere_incl_offset_;
} *ffewhereIncl;

extern inline ffewhere
ffewhere_new (ffewhereLine l, ffewhereCol c)
{
  if (l > FFEWHERE_lineMAX)
    l = FFEWHERE_lineMAX;
  if (c > FFEWHERE_colMAX)
    c = FFEWHERE_colMAX;
  return (l << CHAR_BIT) | c;
}

#define ffewhere_line(w) ((w) >> CHAR_BIT)
#define ffewhere_col(w) ((w) & UCHAR_MAX)

#define ffewhere_file_name(f) ((f)->ffewhere_file_text_)
#define ffewhere_file_namelen(f) ((f)->ffewhere_file_length_)
ffewhereFile ffewhere_file_new (char *name, size_t length);
void ffewhere_file_set (ffewhereFile wf, bool have_num, ffewhereLineNumber ln);

ffewhereIncl ffewhere_incl_line (ffewhereLine l);

void ffewhere_initialize (void);

#define ffewhere_line_file(l) ffewhere_file_incl (ffewhere_incl_line ((l)))
#define ffewhere_line_filelinenum(l) \
  ffewhere_line_incl (ffewhere_incl_line ((l)))

#define ffewhere_terminate()

#endif
