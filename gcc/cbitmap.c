/* Functions to support general ended compressed bitmaps.
   Contributed by Daniel Berlin <dberlin@redhat.com>
   Copyright (C) 1997, 1998, 1999, 2000 Free Software Foundation, Inc.
   
   
   This file is part of GNU CC.
   
   GNU CC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   GNU CC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with GNU CC; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "flags.h"
#include "cbitmap.h"

static void cbitmap_int_list_init PARAMS ((cbitmap_int_list *));
static void cbitmap_int_list_delete PARAMS ((cbitmap_int_list *));
static unsigned int cbitmap_int_list_size PARAMS ((cbitmap_int_list *));
static void cbitmap_int_list_add PARAMS ((cbitmap_int_list *, unsigned int));

static void
cbitmap_int_list_init (int_list)
     cbitmap_int_list *int_list;
{
  int_list->eod = 0;
  int_list->len = 0;
  int_list->data = 0;
}

static unsigned int
cbitmap_int_list_size (int_list)
     cbitmap_int_list *int_list;
{
  return int_list->eod;
}

static void
cbitmap_int_list_add (int_list, data)
     cbitmap_int_list *int_list;
     unsigned int data;
{
  if (int_list->eod >= int_list->len)
    {
      int new_size = int_list->eod + 8;
      int_list->data =
	(unsigned int *) xrealloc (int_list->data,
				   new_size * sizeof (*int_list->data));
      int_list->len = new_size;
    }
  int_list->data[int_list->eod++] = data;
}

static void
cbitmap_int_list_delete (int_list)
     cbitmap_int_list *int_list;
{
  free (int_list->data);
}

cbitmap
cbitmap_alloc (size)
     unsigned int size;
{
  cbitmap result = xmalloc (sizeof (struct cbitmap_def));
  result->blen = size;
  cbitmap_int_list_init (&result->blist);
  return result;
}

void
cbitmap_free (bmap)
     cbitmap bmap;
{
  cbitmap_int_list_delete (&bmap->blist);
  free (bmap);
}

int
cbitmap_a_or_b (a, b)
     cbitmap a;
     cbitmap b;
{
  cbitmap_negate (a);
  cbitmap_negate (b);
  cbitmap_a_and_b (a, b);
  cbitmap_negate (b);
  cbitmap_negate (a);
  return 1;
}


void
cbitmap_clear (a)
     cbitmap a;
{
  cbitmap_int_list_delete (&a->blist);
  cbitmap_int_list_init (&a->blist);
  cbitmap_int_list_add (&a->blist, 0);
}

void
cbitmap_copy (a, b)
     cbitmap a;
     cbitmap b;
{
  unsigned int len;
  unsigned int i;
  cbitmap_int_list_delete (&a->blist);
  cbitmap_int_list_init (&a->blist);
  len = cbitmap_int_list_size (&b->blist);
  for (i = 0; i < len; i++)
    cbitmap_int_list_add (&a->blist, b->blist.data[i]);
}

int
cbitmap_equal_p (a, b)
     cbitmap a;
     cbitmap b;
{
  unsigned int len = cbitmap_int_list_size (&b->blist);
  unsigned int i;
  if (cbitmap_int_list_size (&a->blist) != len)
    return 0;
  for (i = 0; i < len; i++)
    if (a->blist.data[i] != b->blist.data[i])
      return 0;
  return 1;
}

// Macros for logical 'and'.

#define CHANGE_B() \
{ \
         newb= b1&b2; \
         if(newb != q) \
         { \
            cbitmap_int_list_add (&result, rem); \
            rem= 0; \
            q= newb; \
         } \
}

#define PROCEED( inx , i_x , len_x , num_x , num_y ) \
{ \
         inx ++; \
         if(inx >= len_x) \
         { \
            if(inx > len_x) \
               break; \
            num_y= num_y-num_x; \
            num_x= blen - tot; \
         } \
         else \
         { \
            num_y= num_y-num_x; \
            num_x= i_x.data[inx]; \
         } \
}

#define PROCEED1( inx , len , num , i_x ) \
{ \
         inx++; \
         if(inx >= len) \
         { \
            if(inx > len) \
               break; \
            num= blen - tot; \
         } \
         else \
         { \
            num= i_x.data[inx]; \
         } \
}

int
cbitmap_a_and_b (a, b)
     cbitmap a;
     cbitmap b;
{
  unsigned int b1 = a->blist.data[0];
  unsigned int b2 = b->blist.data[0];
  unsigned int len;

  unsigned int len1 = cbitmap_int_list_size (&a->blist);
  unsigned int len2 = cbitmap_int_list_size (&b->blist);
  unsigned int q = b1 & b2;
  unsigned int newb;
  unsigned int x = 1, y = 1;
  unsigned int num1, num2;
  unsigned int rem = 0, tot = 0;
  unsigned int blen;
  unsigned int i;

  cbitmap_int_list result;

  cbitmap_int_list_init (&result);

  if (len1 <= 1)
    {
      int j;
      if (b1 == 0)
	return 0;
      cbitmap_int_list_delete (&a->blist);
      cbitmap_int_list_init (&a->blist);

      for (j = 0; j < len2; j++)
	cbitmap_int_list_add (&a->blist, b->blist.data[j]);
      return 1;
    }

  if (len2 <= 1)
    {

      if (b2 == 1)
	return 0;

      cbitmap_int_list_delete (&a->blist);
      cbitmap_int_list_init (&a->blist);

      cbitmap_int_list_add (&a->blist, 0);
      return 1;
    }
  cbitmap_int_list_add (&result, q);

  num1 = a->blist.data[1];
  num2 = b->blist.data[1];

  while (1)
    {
      if (num1 < num2)
	{
	  tot += num1;
	  rem += num1;

	  b1 = 1 - b1;
	  CHANGE_B ();
	  blen = a->blen;
	  PROCEED (x, a->blist, len1, num1, num2);
	}
      else if (num1 > num2)
	{
	  tot += num2;
	  rem += num2;

	  b2 = 1 - b2;
	  CHANGE_B ();
	  blen = a->blen;
	  PROCEED (y, b->blist, len2, num2, num1);
	}
      else
	{
	  tot += num1;
	  rem += num1;
	  blen = a->blen;

	  if (tot >= blen)
	    break;

	  b1 = 1 - b1;
	  b2 = 1 - b2;

	  CHANGE_B ();
	  PROCEED1 (x, len1, num1, a->blist);
	  PROCEED1 (y, len2, num2, b->blist);

	}
    }

  cbitmap_int_list_delete (&a->blist);
  cbitmap_int_list_init (&a->blist);

  len = cbitmap_int_list_size (&result);
  for (i = 0; i < len; i++)
    cbitmap_int_list_add (&a->blist, result.data[i]);

  return 1;
}

int
cbitmap_bit_p (a, num)
     cbitmap a;
     unsigned int num;
{
  unsigned int b = a->blist.data[0];
  unsigned int tot = 0, len = cbitmap_int_list_size (&a->blist);
  unsigned int i;

  for (i = 1; i < len; i++)
    {
      tot += a->blist.data[i];
      if (tot > num)
	return b;
      b = 1 - b;
    }
  return b;

}


void
cbitmap_set_bit (a, num)
     cbitmap a;
     unsigned int num;
{
  cbitmap tmp = cbitmap_alloc (a->blen);
  if (num == 0)
    {
      cbitmap_int_list_add (&tmp->blist, 0);
      cbitmap_int_list_add (&tmp->blist, 1);
    }
  else
    {
      cbitmap_int_list_add (&tmp->blist, 1);
      cbitmap_int_list_add (&tmp->blist, num);
      if (num != a->blen - 1)
	cbitmap_int_list_add (&tmp->blist, 1);
    }
  cbitmap_negate (a);
  cbitmap_a_and_b (a, tmp);
  cbitmap_negate (a);
  cbitmap_free (tmp);
}

void
cbitmap_clear_bit (a, num)
     cbitmap a;
     unsigned int num;
{
  cbitmap tmp = cbitmap_alloc (a->blen);
  if (num == 0)
    {
      cbitmap_int_list_add (&tmp->blist, 0);
      cbitmap_int_list_add (&tmp->blist, 1);
    }
  else
    {
      cbitmap_int_list_add (&tmp->blist, 1);
      cbitmap_int_list_add (&tmp->blist, num);
      if (num != a->blen - 1)
	cbitmap_int_list_add (&tmp->blist, 1);
    }
  cbitmap_a_and_b (a, tmp);
  cbitmap_free (tmp);
}
