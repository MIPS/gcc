/* Simple binary heap.  
   Copyright (C) 2003 Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "heap.h"

/* Allocate heap for at most SIZE elements.  */
struct heap *
heap_alloc (size)
     unsigned size;
{
  struct heap *heap =
    xmalloc (sizeof (struct heap) + (size - 1) * sizeof (heap_elem));

  heap->n_elems = 0;
  return heap;
}

/* Return minimal element of HEAP.  */
heap_elem *
heap_min (heap)
     struct heap *heap;
{
  if (!heap->n_elems)
    abort ();

  return heap->heap;
}

/* Return number of elements of HEAP.  */
unsigned
heap_size (heap)
     struct heap *heap;
{
  return heap->n_elems;
}

/* Extracts and returns the minimal element of HEAP.  */
void *
heap_extract_min (heap)
     struct heap *heap;
{
  void *ret;
  unsigned empty = 0, son;
  int key;

  if (!heap->n_elems)
    abort ();
  ret = heap->heap->data;
  heap->n_elems--;

  if (heap->n_elems)
    {
      key = heap->heap[heap->n_elems].key;

      while (1)
	{
	  son = 2 * empty + 1;
	  if (son >= heap->n_elems)
	    break;
	  if (son + 1 < heap->n_elems
	      && heap->heap[son].key > heap->heap[son + 1].key)
	    son++;
	  if (heap->heap[son].key >= key)
	    break;
	  heap->heap[empty] = heap->heap[son];
	  empty = son;
	}
      heap->heap[empty] = heap->heap[heap->n_elems];
    }
  return ret;
}

/* Inserts DATA with KEY to HEAP.  */
void
heap_insert (heap, key, data)
     struct heap *heap;
     int key;
     void *data;
{
  int empty = heap->n_elems++, father;

  while (empty)
    {
      father = (empty - 1) / 2;

      if (heap->heap[father].key <= key)
	break;
      heap->heap[empty] = heap->heap[father];
      empty = father;
    }
  heap->heap[empty].key = key;
  heap->heap[empty].data = data;
}
