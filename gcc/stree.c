/* S-tree representation manipulation.
   Copyright (C) 2004 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "stree.h"
#include "tree.h"
#include "ggc.h"

/* These are chosen so that tab_chunksize is a power of 2 for
   efficient lookup, and so that they fill a typical machine's
   page.  */
enum {
  tab_chunksize = 4096,
  initial_tabs = 1024
};

struct st_chunk GTY(()) {
  unsigned char d[1];
};

static GTY(()) s_tree_i last_s_tree;
static GTY(()) size_t num_tabs;
static GTY((length ("num_tabs"))) struct st_chunk ** tabs;

void
get_s_tree_iter (s_tree_iter * si_p, s_tree_i st)
{
  st--;
  *si_p = tabs[st / tab_chunksize]->d + (st % tab_chunksize);
}

tree
sti_const_int_tree (s_tree_iter * si_p)
{
  unsigned HOST_WIDE_INT l = 0, h = 0, c;
  unsigned char last;
  int i = 0;
  
  do {
    last = **si_p;
    c = last & 0x7F;
    if (i < HOST_BITS_PER_WIDE_INT)
      l |= c << i;
    if (i >= HOST_BITS_PER_WIDE_INT)
      h |= c << (i - HOST_BITS_PER_WIDE_INT);
    else if (i > HOST_BITS_PER_WIDE_INT - 7)
      h |= c >> (HOST_BITS_PER_WIDE_INT - i);
    i += 7;
    (*si_p)++;
  } while (last & 0x80);
  return build_int_2 (l, h);
}

s_tree_i
build_s_tree (unsigned char type, ...)
{
  va_list v;
  int kind;
  s_tree_i result;

  unsigned char stagea[256];
  unsigned char *stage = stagea;
  unsigned char *stagep = stage;
  size_t stage_size;

  /* FIXME this macro needs to be enhanced to deal with large strees.  */
#define require(n) do {						\
    if (stagep - stage + (size_t)(n) >= sizeof (stagea))  abort ();	\
  } while (0)

#define pb(b) do {				\
    require (1);				\
    *stagep++ = (b);				\
  } while (0)
  
  va_start (v, type);

  pb (type);
  while (1) {
    kind = va_arg (v, int);
    switch (kind)
      {
      case ST_LAST:
	goto last_item;
	
      case ST_TREE:
	{
	  tree t = va_arg (v, tree);
	  require (sizeof (tree));
	  memcpy (stagep, &t, sizeof (tree));
	  stagep += sizeof (tree);
	  break;
	}

      case ST_TREE_INT:
	{
	  tree t = va_arg (v, tree);
	  unsigned HOST_WIDE_INT l = TREE_INT_CST_LOW (t);
	  unsigned HOST_WIDE_INT h = TREE_INT_CST_HIGH (t);
	  require ((HOST_BITS_PER_WIDE_INT * 2 + 6) / 7);
	  while (l > 0x7F || h != 0)
	    {
	      *stagep++ = l | 0x80;
	      l = (l >> 7) | (h << (HOST_BITS_PER_WIDE_INT - 7));
	      h >>= 7;
	    }
	  *stagep++ = l;
	  break;
	}

      default:
	abort ();
      }
  }
 last_item:
    
  stage_size = stagep - stage;
  if (last_s_tree % tab_chunksize > 0
      && last_s_tree % tab_chunksize + stage_size > tab_chunksize)
    last_s_tree += tab_chunksize - last_s_tree % tab_chunksize;

  if (num_tabs <= last_s_tree / tab_chunksize)
    {
      if (num_tabs == 0)
	{
	  num_tabs = initial_tabs;
	  tabs = ggc_calloc (initial_tabs, sizeof (*tabs));
	}
      else
	{
	  size_t i;
	  
	  num_tabs += initial_tabs;
	  tabs = ggc_realloc (tabs, num_tabs * sizeof (*tabs));
	  for (i = num_tabs - initial_tabs; i < num_tabs; i++)
	    tabs[i] = NULL;
	}
    }
  
  if (tabs[last_s_tree / tab_chunksize] == NULL)
    {
      size_t sz = stage_size > tab_chunksize ? stage_size : tab_chunksize;
      tabs[last_s_tree / tab_chunksize] = ggc_alloc (sz);
    }
  
  memcpy (tabs[last_s_tree / tab_chunksize]->d + last_s_tree % tab_chunksize,
	  stage, stage_size);
  result = last_s_tree;
  last_s_tree += stage_size > tab_chunksize ? tab_chunksize : stage_size;
  return result + 1;
}

#include "gt-stree.h"
