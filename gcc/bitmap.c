/* Functions to support general ended bitmaps.
   Copyright (C) 1997, 1998, 1999, 2000, 2001, 2003
   Free Software Foundation, Inc.

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
#include "tm.h"
#include "rtl.h"
#include "flags.h"
#include "obstack.h"
#include "ggc.h"
#include "bitmap.h"

/* Obstack to allocate bitmap elements from.  */
static struct obstack bitmap_obstack;
static int bitmap_obstack_init = FALSE;

#ifndef INLINE
#ifndef __GNUC__
#define INLINE
#else
#define INLINE __inline__
#endif
#endif

#define HIGHEST_INDEX (unsigned int) ~0

#undef VERIFY_BITMAP

/* Global data */
bitmap_element bitmap_zero_bits;	/* An element of all zero bits.  */
static bitmap_element *bitmap_free;	/* Freelist of bitmap elements.  */
static GTY((deletable (""))) bitmap_element *bitmap_ggc_free;

static void bitmap_elem_to_freelist	PARAMS ((bitmap, bitmap_element *));
static void bitmap_element_free		PARAMS ((bitmap, bitmap_element *));
static bitmap_element *bitmap_element_allocate PARAMS ((bitmap));
static int bitmap_element_zerop		PARAMS ((bitmap_element *));
static int bitmap_find_indx		PARAMS ((bitmap, unsigned int));
static void ensure_allocated_obstack	PARAMS ((void));
static void fill_freelist		PARAMS ((int));
#ifdef VERIFY_BITMAP
static void verify_bitmap		PARAMS ((bitmap));
static void slow_bitmap_operation	PARAMS ((bitmap, bitmap, bitmap,
						 enum bitmap_bits));
#endif

/* Add ELEM to the appropriate freelist.  */
static INLINE void
bitmap_elem_to_freelist (head, elt)
     bitmap head;
     bitmap_element *elt;
{
  elt->next = *head->freelist;
  *head->freelist = elt;
}

/* Free a bitmap element.  Since these are allocated off the
   bitmap_obstack, "free" actually means "put onto the freelist".  */

static INLINE void
bitmap_element_free (head, elt)
     bitmap head;
     bitmap_element *elt;
{
  bitmap_element *next = elt->next;
  bitmap_element *prev = elt->prev;

  if (prev)
    prev->next = next;

  if (next)
    next->prev = prev;

  if (head->first == elt)
    head->first = next;

  if (head->current == elt)
    {
      head->current = next ? next : prev;
      if (head->current)
	head->indx = head->current->indx;
      else
	head->indx = HIGHEST_INDEX;
    }
  bitmap_elem_to_freelist (head, elt);
}

/* Allocates an obstack.  */
static void
ensure_allocated_obstack ()
{
  bitmap_obstack_init = TRUE;
	      
  /* We can't use gcc_obstack_init to initialize the obstack since
     print-rtl.c now calls bitmap functions, and bitmap is linked
     into the gen* functions.  */

  /* Let particular systems override the size of a chunk.  */
#ifndef OBSTACK_CHUNK_SIZE
#define OBSTACK_CHUNK_SIZE 0
#endif
  /* Let them override the alloc and free routines too.  */
#ifndef OBSTACK_CHUNK_ALLOC
#define OBSTACK_CHUNK_ALLOC xmalloc
#endif
#ifndef OBSTACK_CHUNK_FREE
#define OBSTACK_CHUNK_FREE free
#endif
	      
#if !defined(__GNUC__) || (__GNUC__ < 2)
#define __alignof__(type) 0
#endif
	      
  obstack_specify_allocation (&bitmap_obstack, OBSTACK_CHUNK_SIZE,
			      __alignof__ (bitmap_element),
			      (void *(*) PARAMS ((long))) OBSTACK_CHUNK_ALLOC,
			      (void (*) PARAMS ((void *))) OBSTACK_CHUNK_FREE);

  /* Allocate a few elements to obstack.  */
  fill_freelist (1);
}

#define N_TO_ALLOCATE 16
/* Add a few elements to appropriate freelist.  */
static void
fill_freelist (using_obstack)
     int using_obstack;
{
  bitmap_element *element;
  int i;

  if (using_obstack)
    {
      for (i = 0; i < N_TO_ALLOCATE; i++)
	{
	  element = (bitmap_element *) obstack_alloc (&bitmap_obstack,
						      sizeof (bitmap_element));
	  element->next = bitmap_free;
	  bitmap_free = element;
	}
    }
  else
    {
      for (i = 0; i < N_TO_ALLOCATE; i++)
	{
	  element = ggc_alloc (sizeof (bitmap_element));
	  element->next = bitmap_ggc_free;
	  bitmap_ggc_free = element;
	}
    }
}

/* Allocate a bitmap element.  The bits are not cleared.  */

static INLINE bitmap_element *
bitmap_element_allocate (head)
     bitmap head;
{
  bitmap_element *element;

  element = *head->freelist;
  *head->freelist = element->next;

  if (!element->next)
    fill_freelist (head->using_obstack);

  return element;
}

/* Release any memory allocated by bitmaps.  */

void
bitmap_release_memory ()
{
  bitmap_free = 0;
  if (bitmap_obstack_init)
    {
      bitmap_obstack_init = FALSE;
      obstack_free (&bitmap_obstack, NULL);
    }
}

/* Initialize a bitmap header.  */

bitmap
bitmap_initialize (head, using_obstack)
     bitmap head;
     int using_obstack;
{
  if (head == NULL && ! using_obstack)
    head = ggc_alloc (sizeof (*head));
  
  if (using_obstack && !bitmap_obstack_init)
    ensure_allocated_obstack ();
  
  if (!using_obstack && !bitmap_ggc_free)
    fill_freelist (0);

  head->first = head->current = 0;
  head->indx = HIGHEST_INDEX;
  head->using_obstack = using_obstack;
  head->freelist = using_obstack ? &bitmap_free : &bitmap_ggc_free;

#ifdef VERIFY_BITMAP
  verify_bitmap (head);
#endif
  return head;
}
/* Return nonzero if all bits in an element are zero.  */

static INLINE int
bitmap_element_zerop (element)
     bitmap_element *element;
{
#if BITMAP_ELEMENT_WORDS == 2
  return (element->bits[0] | element->bits[1]) == 0;
#else
  int i;

  for (i = 0; i < BITMAP_ELEMENT_WORDS; i++)
    if (element->bits[i] != 0)
      return 0;

  return 1;
#endif
}

/* Clear a bitmap by freeing the linked list.  */

INLINE void
bitmap_clear (head)
     bitmap head;
{
  bitmap_element *element;

  if (head->first)
    {
      head->indx = HIGHEST_INDEX;

      for (element = head->current; element->next; element = element->next)
	continue;
      head->current = NULL;
      element->next = *head->freelist;
      *head->freelist = head->first;
      head->first = NULL;
    }
#ifdef VERIFY_BITMAP
  verify_bitmap (head);
#endif
}

/* Copy a bitmap to another bitmap.  */

void
bitmap_copy (to, from)
     bitmap to;
     bitmap from;
{
  bitmap_element *from_ptr, *to_ptr = 0;
#if BITMAP_ELEMENT_WORDS != 2
  int i;
#endif

  bitmap_clear (to);

  /* Copy elements in forward direction one at a time */
  for (from_ptr = from->first; from_ptr; from_ptr = from_ptr->next)
    {
      bitmap_element *to_elt = bitmap_element_allocate (to);

      to_elt->indx = from_ptr->indx;

#if BITMAP_ELEMENT_WORDS == 2
      to_elt->bits[0] = from_ptr->bits[0];
      to_elt->bits[1] = from_ptr->bits[1];
#else
      for (i = 0; i < BITMAP_ELEMENT_WORDS; i++)
	to_elt->bits[i] = from_ptr->bits[i];
#endif

      /* Here we have a special case of element link, for the case
	 where we know the links are being entered in sequence.  */
      if (to_ptr == 0)
	{
	  to->first = to->current = to_elt;
	  to->indx = from_ptr->indx;
	  to_elt->next = to_elt->prev = 0;
	}
      else
	{
	  to_elt->prev = to_ptr;
	  to_elt->next = 0;
	  to_ptr->next = to_elt;
	}

      to_ptr = to_elt;
    }
#ifdef VERIFY_BITMAP
  if (!bitmap_equal_p (to, from))
    abort ();
#endif
}

/* Checks whether an element with index INDX is in bitmap HEAD, and
   set HEAD->current to nearest element to it. */

static INLINE int 
bitmap_find_indx (head, indx)
     bitmap head;
     unsigned int indx;
{
  bitmap_element *element;

  if (head->indx == indx)
    return true;
  if (head->indx == HIGHEST_INDEX)
    return false;

  if (head->indx > indx)
    {
      for (element = head->current;
  	   element->prev != 0 && element->prev->indx >= indx;
  	   element = element->prev)
    	continue;
    }
  else
    {
      for (element = head->current;
	   element->next != 0 && element->next->indx <= indx;
	   element = element->next)
	continue;
    }
  head->current = element;
  head->indx = element->indx;
  return element->indx == indx;
}

/* Clear a single bit in a bitmap.  */

void
bitmap_clear_bit (head, bit)
     bitmap head;
     int bit;
{
  bitmap_element *ptr;
  unsigned delta = bit % BITMAP_ELEMENT_ALL_BITS;
  unsigned indx = bit - delta;

  if (bitmap_find_indx (head, indx))
    {
      unsigned bit_num  = delta % BITMAP_WORD_BITS;
      unsigned word_num = delta / BITMAP_WORD_BITS;

      ptr = head->current;
      ptr->bits[word_num] &= ~ (((BITMAP_WORD) 1) << bit_num);

      /* If we cleared the entire word, free up the element */
      if (!ptr->bits[word_num] /* Just to speed up things a bit.  */
	  && bitmap_element_zerop (ptr))
	bitmap_element_free (head, ptr);
    }
}

/* Set a single bit in a bitmap.  */

void
bitmap_set_bit (head, bit)
     bitmap head;
     int bit;
{
  bitmap_element *ptr, *nw;
  unsigned delta = bit % BITMAP_ELEMENT_ALL_BITS;
  unsigned indx = bit - delta;
  unsigned bit_num  = delta % BITMAP_WORD_BITS;
  unsigned word_num = delta / BITMAP_WORD_BITS;
  BITMAP_WORD bit_val = ((BITMAP_WORD) 1) << bit_num;

  if (bitmap_find_indx (head, indx))
    {
      head->current->bits[word_num] |= bit_val;
      return;
    }
  
  ptr = head->current;
  nw = bitmap_element_allocate (head);
  nw->indx = indx;
  memset (nw->bits, 0, sizeof (nw->bits));
  nw->bits[word_num] = bit_val;
  head->current = nw;
  head->indx = indx;

  if (!ptr)
    {
      head->first = nw;
      nw->prev = nw->next = NULL;
    }
  else if (ptr->indx > indx)
    {
      nw->next = ptr;
      nw->prev = ptr->prev;
      if (nw->prev)
	nw->prev->next = nw;
      else
	head->first = nw;
      ptr->prev = nw;
    }
  else
    {
      nw->prev = ptr;
      nw->next = ptr->next;
      if (nw->next)
	nw->next->prev = nw;
      ptr->next = nw;
    }
}

/* Return whether a bit is set within a bitmap.  */

int
bitmap_bit_p (head, bit)
     bitmap head;
     int bit;
{
  bitmap_element *ptr;
  unsigned bit_num;
  unsigned word_num;
  unsigned delta = bit % BITMAP_ELEMENT_ALL_BITS;
  unsigned indx = bit - delta;

  if (!bitmap_find_indx (head, indx))
    return 0;
  ptr = head->current;
  bit_num = delta % BITMAP_WORD_BITS;
  word_num = delta / BITMAP_WORD_BITS;

  return (ptr->bits[word_num] >> bit_num) & 1;
}

/* Return the bit number of the first set bit in the bitmap, or -1
   if the bitmap is empty.  */

int
bitmap_first_set_bit (a)
     bitmap a;
{
  bitmap_element *ptr = a->first;
  BITMAP_WORD word;
  unsigned word_num, bit_num;

  if (ptr == NULL)
    return -1;

#if BITMAP_ELEMENT_WORDS == 2
  word_num = 0, word = ptr->bits[0];
  if (word == 0)
    word_num = 1, word = ptr->bits[1];
#else
  for (word_num = 0; word_num < BITMAP_ELEMENT_WORDS; ++word_num)
    if ((word = ptr->bits[word_num]) != 0)
      break;
#endif

  /* Binary search for the first set bit.  */
  /* ??? It'd be nice to know if ffs or ffsl was available.  */

  bit_num = 0;
  word = word & -word;

#if nBITMAP_WORD_BITS > 64
 #error "Fill out the table."
#endif
#if nBITMAP_WORD_BITS > 32
  if ((word & 0xffffffff) == 0)
    word >>= 32, bit_num += 32;
#endif
  if ((word & 0xffff) == 0)
    word >>= 16, bit_num += 16;
  if ((word & 0xff) == 0)
    word >>= 8, bit_num += 8;
  if (word & 0xf0)
    bit_num += 4;
  if (word & 0xcc)
    bit_num += 2;
  if (word & 0xaa)
    bit_num += 1;

  return (ptr->indx
	  + word_num * BITMAP_WORD_BITS
	  + bit_num);
}

/* Return the bit number of the last set bit in the bitmap, or -1
   if the bitmap is empty.  */

int
bitmap_last_set_bit (a)
     bitmap a;
{
  bitmap_element *ptr = a->first;
  BITMAP_WORD word;
  unsigned word_num, bit_num;

  if (ptr == NULL)
    return -1;

  while (ptr->next != NULL)
    ptr = ptr->next;

#if BITMAP_ELEMENT_WORDS == 2
  word_num = 1, word = ptr->bits[1];
  if (word == 0)
    word_num = 0, word = ptr->bits[0];
#else
  for (word_num = BITMAP_ELEMENT_WORDS; word_num-- > 0; )
    if ((word = ptr->bits[word_num]) != 0)
      break;
#endif

  /* Binary search for the last set bit.  */

  bit_num = 0;
#if nBITMAP_WORD_BITS > 64
 #error "Fill out the table."
#endif
#if nBITMAP_WORD_BITS > 32
  if (word & ~(BITMAP_WORD)0xffffffff)
    word >>= 32, bit_num += 32;
#endif
  if (word & 0xffff0000)
    word >>= 16, bit_num += 16;
  if (word & 0xff00)
    word >>= 8, bit_num += 8;
  if (word & 0xf0)
    word >>= 4, bit_num += 4;
  if (word & 0xc)
    word >>= 2, bit_num += 2;
  if (word & 0x2)
    bit_num += 1;

  return (ptr->indx
	  + word_num * BITMAP_WORD_BITS
	  + bit_num);
}

#ifdef VERIFY_BITMAP
/* For verification purposes only, slow and stupid version of the
   bitmap_operation.  */
static void
slow_bitmap_operation (to, from1, from2, operation)
     bitmap to;
     bitmap from1;
     bitmap from2;
     enum bitmap_bits operation;
{
  unsigned bit;

  bitmap_clear (to);
  switch (operation)
    {
    default:
      abort ();

    case BITMAP_AND:
      EXECUTE_IF_AND_IN_BITMAP (from1, from2, 0, bit,
	{
	  bitmap_set_bit (to, bit);
	});
      break;

    case BITMAP_AND_COMPL:
      EXECUTE_IF_AND_COMPL_IN_BITMAP (from1, from2, 0, bit,
	{
	  bitmap_set_bit (to, bit);
	});
      break;

    case BITMAP_IOR:
      EXECUTE_IF_SET_IN_BITMAP (from1, 0, bit,
	{
	  bitmap_set_bit (to, bit);
	});
      EXECUTE_IF_SET_IN_BITMAP (from2, 0, bit,
	{
	  bitmap_set_bit (to, bit);
	});
      break;

    case BITMAP_XOR:
      EXECUTE_IF_AND_COMPL_IN_BITMAP (from1, from2, 0, bit,
	{
	  bitmap_set_bit (to, bit);
	});
      EXECUTE_IF_AND_COMPL_IN_BITMAP (from2, from1, 0, bit,
	{
	  bitmap_set_bit (to, bit);
	});
      break;
    }
}
#endif
			 
/* Store in bitmap TO the result of combining bitmap FROM1 and FROM2 using
   a specific bit manipulation.  Return true if TO changes.  */

int
bitmap_operation (to, from1, from2, operation)
     bitmap to;
     bitmap from1;
     bitmap from2;
     enum bitmap_bits operation;
{
  bitmap_element *from1_ptr = from1->first;
  bitmap_element *from2_ptr = from2->first;
  bitmap_element *to_ptr = to->first;
  bitmap_element *to_tmp, *last = NULL;
  int changed = false;
#if BITMAP_ELEMENT_WORDS != 2
  int i;
#endif

#ifdef VERIFY_BITMAP
  bitmap_head c;
  int c_changed;

  verify_bitmap (to);
  verify_bitmap (from1);
  verify_bitmap (from2);
  bitmap_initialize (&c, 0);
  slow_bitmap_operation (&c, from1, from2, operation);
  c_changed = !bitmap_equal_p (&c, to);
#endif

#if BITMAP_ELEMENT_WORDS == 2
#define DOIT1(VAL)					\
  do {							\
    to_tmp->bits[0] = (VAL)->bits[0];			\
    to_tmp->bits[1] = (VAL)->bits[1];			\
    bitmap_element_allocate (to);			\
    to_tmp->prev = last;				\
    to_tmp->indx = (VAL)->indx;				\
    last = to_tmp;					\
    to_tmp = *to->freelist;				\
  } while (0)
#define DOIT_MBZ(A, OP, B)				\
  do {							\
    BITMAP_WORD r0, r1;					\
    r0 = (A)->bits[0] OP (B)->bits[0];			\
    r1 = (A)->bits[1] OP (B)->bits[1];			\
    if (r0 | r1)					\
      {							\
	to_tmp->bits[0] = r0;				\
	to_tmp->bits[1] = r1;				\
	bitmap_element_allocate (to);			\
	to_tmp->indx = (A)->indx;			\
	to_tmp->prev = last;				\
	last = to_tmp;					\
	to_tmp = *to->freelist;				\
      }							\
  } while (0)
#define DOIT(A, OP, B)					\
  do {							\
    BITMAP_WORD r0, r1;					\
    r0 = (A)->bits[0] OP (B)->bits[0];			\
    r1 = (A)->bits[1] OP (B)->bits[1];			\
    to_tmp->bits[0] = r0;				\
    to_tmp->bits[1] = r1;				\
    bitmap_element_allocate (to);			\
    to_tmp->indx = (A)->indx;				\
    to_tmp->prev = last;				\
    last = to_tmp;					\
    to_tmp = *to->freelist;				\
  } while (0)
#else
#define DOIT1(VAL)					\
  do {							\
    for (i = 0; i < BITMAP_ELEMENT_WORDS; ++i)		\
      to_tmp->bits[i] = (VAL)->bits[i];			\
    bitmap_element_allocate (to);			\
    to_tmp->indx = (VAL)->indx;				\
    to_tmp->prev = last;				\
    last = to_tmp;					\
    to_tmp = *to->freelist;				\
  } while (0)
#define DOIT_MBZ(A, OP, B)				\
  do {							\
    BITMAP_WORD r, n = 0;				\
    for (i = 0; i < BITMAP_ELEMENT_WORDS; ++i)		\
      {							\
	r = (A)->bits[i] OP (B)->bits[i];		\
	n |= r;						\
	to_tmp->bits[i] = r;				\
      }							\
    if (n)						\
      {							\
	bitmap_element_allocate (to);			\
	to_tmp->prev = last;				\
	to_tmp->indx = (A)->indx;			\
	last = to_tmp;					\
	to_tmp = *to->freelist;				\
      }							\
  } while (0)
#define DOIT(A, OP, B)					\
  do {							\
    BITMAP_WORD r;					\
    for (i = 0; i < BITMAP_ELEMENT_WORDS; ++i)		\
      {							\
	r = (A)->bits[i] OP (B)->bits[i];		\
	to_tmp->bits[i] = r;				\
      }							\
    bitmap_element_allocate (to); /* Really allocate to_tmp. */	\
    to_tmp->indx = (A)->indx;				\
    to_tmp->prev = last;				\
    last = to_tmp;					\
    to_tmp = *to->freelist;				\
  } while (0)
#endif

  to_tmp = *to->freelist;
  switch (operation)
    {
    default:
      abort ();

    case BITMAP_AND:
      if (!from1_ptr || !from2_ptr)
	break;

      while (from1_ptr)
	{
	  while (from2_ptr && from1_ptr->indx > from2_ptr->indx)
	    from2_ptr = from2_ptr->next;
	  if (!from2_ptr)
	    break;
	  if (from1_ptr->indx < from2_ptr->indx)
	    {
	      from1_ptr = from1_ptr->next;
	      continue;
	    }
	  DOIT_MBZ (from1_ptr, &, from2_ptr);
	  from1_ptr = from1_ptr->next;
	  from2_ptr = from2_ptr->next;
	}
      break;

    case BITMAP_AND_COMPL:
      if (!from1_ptr)
	break;

      while (from1_ptr)
	{
	  while (from2_ptr && from1_ptr->indx > from2_ptr->indx)
	    from2_ptr = from2_ptr->next;
	  if (!from2_ptr)
	    break;
	  if (from1_ptr->indx < from2_ptr->indx)
	    {
	      DOIT1 (from1_ptr);
	      from1_ptr = from1_ptr->next;
	      continue;
	    }
	  DOIT_MBZ (from1_ptr, &~, from2_ptr);
	  from1_ptr = from1_ptr->next;
	  from2_ptr = from2_ptr->next;
	}
      while (from1_ptr)
	{
	  DOIT1 (from1_ptr);
	  from1_ptr = from1_ptr->next;
	}
      break;

    case BITMAP_IOR:
      if (!from1_ptr && !from2_ptr)
	break;

      while (from1_ptr && from2_ptr)
	{
	  if (from1_ptr->indx < from2_ptr->indx)
	    {
	      DOIT1 (from1_ptr);
	      from1_ptr = from1_ptr->next;
	      continue;
	    }
	  if (from1_ptr->indx > from2_ptr->indx)
	    {
	      DOIT1 (from2_ptr);
	      from2_ptr = from2_ptr->next;
	      continue;
	    }
	  DOIT (from1_ptr, |, from2_ptr);
	  from1_ptr = from1_ptr->next;
	  from2_ptr = from2_ptr->next;
	}

      while (from1_ptr)
	{
	  DOIT1 (from1_ptr);
	  from1_ptr = from1_ptr->next;
	}
      while (from2_ptr)
	{
	  DOIT1 (from2_ptr);
	  from2_ptr = from2_ptr->next;
	}
      break;

    case BITMAP_XOR:
      if (!from1_ptr && !from2_ptr)
	break;

      while (from1_ptr && from2_ptr)
	{
	  if (from1_ptr->indx < from2_ptr->indx)
	    {
	      DOIT1 (from1_ptr);
	      from1_ptr = from1_ptr->next;
	      continue;
	    }
	  if (from1_ptr->indx > from2_ptr->indx)
	    {
	      DOIT1 (from2_ptr);
	      from2_ptr = from2_ptr->next;
	      continue;
	    }
	  DOIT_MBZ (from1_ptr, ^, from2_ptr);
	  from1_ptr = from1_ptr->next;
	  from2_ptr = from2_ptr->next;
	}

      while (from1_ptr)
	{
	  DOIT1 (from1_ptr);
	  from1_ptr = from1_ptr->next;
	}
      while (from2_ptr)
	{
	  DOIT1 (from2_ptr);
	  from2_ptr = from2_ptr->next;
	}
      break;
    }

  to_tmp = last;
  if (to_tmp)
    {
      to_tmp->next = NULL;
      while (to_tmp->prev)
	{
	  to_tmp->prev->next = to_tmp;
	  to_tmp = to_tmp->prev;
	}
    }
  to->first = to_tmp;

  while (to_tmp)
    {
      if (!to_ptr || to_ptr->indx != to_tmp->indx)
	{
	  changed = true;
	  break;
	}      
#if BITMAP_ELEMENT_WORDS == 2
      if (to_ptr->bits[0] != to_tmp->bits[0]
	  || to_ptr->bits[1] != to_tmp->bits[1])
	{
	  changed = true;
	  break;
	}
#else
      for (i = 0; i < BITMAP_ELEMENT_WORDS; i++)
	if (to_ptr->bits[i] != to_tmp->bits[i])
	  break;
      if (i < BITMAP_ELEMENT_WORDS)
	{
	  changed = true;
	  break;
	}
#endif
      last = to_ptr->next;
      bitmap_elem_to_freelist (to, to_ptr);
      to_ptr = last;
      to_tmp = to_tmp->next;
    }
  if (to_ptr)
    {
      changed = true;
      do
	{
	  last = to_ptr->next;
	  bitmap_elem_to_freelist (to, to_ptr);
	  to_ptr = last;
	}
      while (to_ptr);
    }
  if (to->first)
    {
      to->current = to->first;
      to->indx = to->first->indx;
    }
  else
    {
      to->current = NULL;
      to->indx = HIGHEST_INDEX;
    }

#ifdef VERIFY_BITMAP
  verify_bitmap (to);
  verify_bitmap (from1);
  verify_bitmap (from2);
  if (!bitmap_equal_p (&c, to))
    abort ();
  if (changed && !c_changed)
    abort ();
  if (!changed && c_changed)
    abort ();
#endif
  
  return changed;
}

/* Return true if two bitmaps are identical.  */

int
bitmap_equal_p (a, b)
     bitmap a;
     bitmap b;
{
  bitmap_element *a_ptr, *b_ptr;
#if BITMAP_ELEMENT_WORDS != 2
  int i;
#endif

  a_ptr = a->first;
  b_ptr = b->first;

  while (1)
    {
      if (!a_ptr)
	return !b_ptr;
      if (!b_ptr)
	return false;

      if (a_ptr->indx != b_ptr->indx)
	return false;

#if BITMAP_ELEMENT_WORDS == 2
      if (a_ptr->bits[0] != b_ptr->bits[0])
	return false;
      if (a_ptr->bits[1] != b_ptr->bits[1])
	return false;
#else
      for (i = 0; i < BITMAP_ELEMENT_WORDS; i++)
	if (a_ptr->bits[i] != b_ptr->bits[i])
	  return false;
#endif
      a_ptr = a_ptr->next;
      b_ptr = b_ptr->next;
    }
}

/* Or into bitmap TO bitmap FROM1 and'ed with the complement of
   bitmap FROM2.  */

void
bitmap_ior_and_compl (to, from1, from2)
     bitmap to;
     bitmap from1;
     bitmap from2;
{
  bitmap_head tmp;

  bitmap_initialize (&tmp, 0);
  bitmap_operation (&tmp, from1, from2, BITMAP_AND_COMPL);
  bitmap_operation (to, to, &tmp, BITMAP_IOR);
  bitmap_clear (&tmp);
}

int
bitmap_union_of_diff (dst, a, b, c)
     bitmap dst;
     bitmap a;
     bitmap b;
     bitmap c;
{
  bitmap_head tmp;
  int changed;

  bitmap_initialize (&tmp, 0);
  bitmap_operation (&tmp, b, c, BITMAP_AND_COMPL);
  changed = bitmap_operation (dst, &tmp, a, BITMAP_IOR);
  bitmap_clear (&tmp);

  return changed;
}

/* Debugging function to print out the contents of a bitmap.  */

void
debug_bitmap_file (file, head)
     FILE *file;
     bitmap head;
{
  bitmap_element *ptr;

  fprintf (file, "\nfirst = ");
  fprintf (file, HOST_PTR_PRINTF, (PTR) head->first);
  fprintf (file, " current = ");
  fprintf (file, HOST_PTR_PRINTF, (PTR) head->current);
  fprintf (file, " indx = %u\n", head->indx);

  for (ptr = head->first; ptr; ptr = ptr->next)
    {
      unsigned int i, j, col = 26;

      fprintf (file, "\t");
      fprintf (file, HOST_PTR_PRINTF, (PTR) ptr);
      fprintf (file, " next = ");
      fprintf (file, HOST_PTR_PRINTF, (PTR) ptr->next);
      fprintf (file, " prev = ");
      fprintf (file, HOST_PTR_PRINTF, (PTR) ptr->prev);
      fprintf (file, " indx = %u\n\t\tbits = {", ptr->indx);

      for (i = 0; i < BITMAP_ELEMENT_WORDS; i++)
	for (j = 0; j < BITMAP_WORD_BITS; j++)
	  if ((ptr->bits[i] >> j) & 1)
	    {
	      if (col > 70)
		{
		  fprintf (file, "\n\t\t\t");
		  col = 24;
		}

	      fprintf (file, " %u", (ptr->indx
				     + i * BITMAP_WORD_BITS + j));
	      col += 4;
	    }

      fprintf (file, " }\n");
    }
}

/* Function to be called from the debugger to print the contents
   of a bitmap.  */

void
debug_bitmap (head)
     bitmap head;
{
  debug_bitmap_file (stdout, head);
}

/* Function to print out the contents of a bitmap.  Unlike debug_bitmap_file,
   it does not print anything but the bits.  */

void
bitmap_print (file, head, prefix, suffix)
     FILE *file;
     bitmap head;
     const char *prefix;
     const char *suffix;
{
  const char *comma = "";
  int i;

  fputs (prefix, file);
  EXECUTE_IF_SET_IN_BITMAP (head, 0, i,
			    {
			      fprintf (file, "%s%d", comma, i);
			      comma = ", ";
			    });
  fputs (suffix, file);
}


#ifdef VERIFY_BITMAP
/* Checks whether the state of bitmap is consistent.  */
static void
verify_bitmap (head)
     bitmap head;
{
  int l = 0, wc = 0, last = -1;
  bitmap_element *ptr;

  if (head->using_obstack && head->freelist != &bitmap_free)
    abort ();
  if (!head->using_obstack && head->freelist != &bitmap_ggc_free)
    abort ();
  if (!*head->freelist)
    abort ();

  if (!head->first)
    {
      if (head->current)
	abort ();
      if (head->indx != HIGHEST_INDEX)
	abort ();
      return;
    }
  if (!head->current)
    abort ();
  if (head->current->indx != head->indx)
    abort ();

  for (ptr = head->first; ptr->next; ptr = ptr->next)
    {
      if ((int) ptr->indx <= last)
	abort ();
      last = ptr->indx;
      if (ptr->next->prev != ptr)
	abort ();
      l++;
      if (ptr == head->current)
	wc = 1;
    }
  if ((int) ptr->indx <= last)
    abort ();
  if (ptr == head->current)
    wc = 1;
  if (!wc)
    abort ();

  for (; ptr->prev; ptr = ptr->prev)
    {
      if (ptr->prev->next != ptr)
	abort ();
      l--;
    }
  if (ptr != head->first)
    abort ();
  if (l)
    abort ();
}
#endif
#include "gt-bitmap.h"
