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

struct _ffewhere_ll_
  {
    ffewhereLL_ next;
    ffewhereLL_ previous;
    ffewhereFile wf;

    /* ffelex_line_number() at time of creation.  */
    ffewhereLine line_no;

    /* User-desired offset (usually 1). */
    ffewhereLine offset;
  };

struct _ffewhere_root_ll_
  {
    ffewhereLL_ first;
    ffewhereLL_ last;
  };

static struct _ffewhere_root_ll_ ffewhere_root_ll_;

static ffewhereLL_ ffewhere_ll_lookup_ (ffewhereLineNumber ln);

/* Look up line-to-line object from absolute line num.  */

static ffewhereLL_
ffewhere_ll_lookup_ (ffewhereLineNumber ln)
{
  ffewhereLL_ ll;

  if (ln == 0)
    return ffewhere_root_ll_.first;

  for (ll = ffewhere_root_ll_.last;
       ll != (ffewhereLL_) &ffewhere_root_ll_.first;
       ll = ll->previous)
    {
      if (ll->line_no <= ln)
	return ll;
    }

  assert ("no line num" == NULL);
  return NULL;
}

/* Kill file object.

   Note that this object must not have been passed in a call
   to any other ffewhere function except ffewhere_file_name and
   ffewhere_file_namelen.  */

void
ffewhere_file_kill (ffewhereFile wf)
{
  malloc_kill_ks (ffe_pool_file (), wf,
		  offsetof (struct _ffewhere_file_, text)
		  + wf->length + 1);
}

/* Create file object.  */

ffewhereFile
ffewhere_file_new (char *name, size_t length)
{
  ffewhereFile wf;

  wf = malloc_new_ks (ffe_pool_file (), "ffewhereFile",
		      offsetof (struct _ffewhere_file_, text)
		      + length + 1);
  wf->length = length;
  memcpy (&wf->text[0], name, length);
  wf->text[length] = '\0';

  return wf;
}

/* Set file and first line number.

   Pass FALSE if no line number is specified.  */

void
ffewhere_file_set (ffewhereFile wf, bool have_num, ffewhereLineNumber ln)
{
  ffewhereLL_ ll;

  ll = malloc_new_kp (ffe_pool_file (), "ffewhereLL_", sizeof (*ll));
  ll->next = (ffewhereLL_) &ffewhere_root_ll_.first;
  ll->previous = ffewhere_root_ll_.last;
  ll->next->previous = ll;
  ll->previous->next = ll;
  if (wf == NULL)
    {
      if (ll->previous == ll->next)
	ll->wf = NULL;
      else
	ll->wf = ll->previous->wf;
    }
  else
    ll->wf = wf;
  ll->line_no = ffelex_line_number ();
  if (have_num)
    ll->offset = ln;
  else
    {
      if (ll->previous == ll->next)
	ll->offset = 1;
      else
	ll->offset
	  = ll->line_no - ll->previous->line_no + ll->previous->offset;
    }
}

/* Do initializations.  */

void
ffewhere_initialize ()
{
  ffewhere_root_ll_.first = ffewhere_root_ll_.last
    = (ffewhereLL_) &ffewhere_root_ll_.first;
}

/* Look up file object from line object.  */

ffewhereFile
ffewhere_line_file (ffewhereLine wl)
{
  ffewhereLL_ ll;

  assert (wl != NULL);
  ll = ffewhere_ll_lookup_ (wl->line_num);
  return ll->wf;
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
