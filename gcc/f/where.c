/* where.c
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

#include "proj.h"
#include "where.h"
#include "lex.h"
#include "malloc.h"

typedef struct _ffewhere_ll_ *ffewhereLL_;

static ffewhereFile ffewhere_last_file_ = NULL;
static ffewhereIncl ffewhere_last_incl_ = NULL;

/* Obtain file object given name (and its length) of file.  */

ffewhereFile
ffewhere_file (char *name, size_t length)
{
  ffewhereFile wf;

  /* It might be counterproductive to search for the file,
     except that, in source code repeatedly including the same files
     (in each program unit), it might end up saving Dcache misses
     overall.  */
  for (wf = ffewhere_last_file_;
       wf;
       wf = wf->ffewhere_file_prev_)
    {
      if (wf->ffewhere_file_length_ == length
	  && ! memcmp (wf->ffewhere_file_name_, name, length))
	return wf;
    }

  wf = xmalloc (offsetof (struct _ffewhere_file_, name) + length + 1);
  wf->ffewhere_file_prev_ = ffewhere_file_last_;
  ffewhere_file_last_ = wf;

  wf->ffewhere_file_length_ = length;
  memcpy (&wf->ffewhere_file_name_[0], name, length);

  /* For convenience.  */
  wf->ffewhere_file_name_[length] = '\0';

  return wf;
}

/* Obtain inclusion encapsulation for inclusion of a file corresponding
   to a given global line number, along with first line number (none
   if have_num is FALSE).  */

ffewhereIncl
ffewhere_incl (ffewhereFile wf, ffewhereLine global_line, bool have_num,
	       ffewhereLine line_offset)
{
  ffewhereIncl incl;

  incl = xmalloc (sizeof (*incl));
  incl->ffewhere_incl_prev_ = ffewhere_incl_latest_;
  ffewhere_incl_latest_ = ll;

  if (wf)
    incl->ffewhere_incl_file_ = wf;
  else
    {
      /* Just adjust the line number for the current file.  */
      if (incl->ffewhere_incl_prev_)
	incl->ffewhere_incl_file_
	  = incl->ffewhere_incl_prev_->ffewhere_incl_file_;
      else
	ll->wf = NULL;
    }

  incl->ffewhere_incl_line_ = global_line;

  if (have_num)
    incl->ffewhere_incl_offset_ = line_offset;
  else
    {
      /* Just keep the current line numbering going.  */
      if (incl->ffewhere_incl_prev_)
	incl->ffewhere_incl_offset_
	  = incl->ffewhere_incl_line_
	  - incl->ffewhere_incl_prev_->ffewhere_incl_line_
	  + incl->ffewhere_incl_prev_->ffewhere_incl_offset_;
      else
	incl->ffewhere_incl_offset_ = 1;
    }
}

/* Do initializations.  */

void
ffewhere_initialize ()
{
  /* If these aren't true, we're being reinitialized.  If that's legit,
     we should really free() the lists.  (Silly to do that on termination
     at this point, since termination precedes the compiler exiting,
     but comes after lots of other memory might be needed anyway.)  */

  assert (ffewhere_last_file_ == NULL);
  assert (ffewhere_last_incl_ == NULL);
}

/* Find inclusion encapsulation given a global line number.  */

ffewhereIncl
ffewhere_incl_find (ffewhereLine wl)
{
  ffewhereIncl_ incl;

  for (incl = ffewhere_last_incl_;
       incl;
       incl = incl->prev)
    {
      if (incl->ffewhere_incl_line_ <= ln)
	return incl;
    }

  assert ("cannot find global line number in list of inclusions" == NULL);
  return NULL;
}

/* Lookup file object from line object, return line number.  */

ffewhereLineNumber
ffewhere_line_filelinenum (ffewhereLine wl)
{
  ffewhereLL_ ll;

  assert (wl != NULL);
  ll = ffewhere_ll_lookup_ (wl->line_num);
  return wl->line_num + ll->offset - ll->line_no;
}
