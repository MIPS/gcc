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

/* Element of a heap.  */
typedef struct
{
  int key;
  void *data;
} heap_elem;

/* The heap type.  */
struct heap
{
  unsigned n_elems;		/* Number of elements in heap.  */
  heap_elem heap[1];	/* The heap itself.  */
};

extern struct heap *heap_alloc		PARAMS ((unsigned));
extern heap_elem *heap_min		PARAMS ((struct heap *));
extern unsigned heap_size		PARAMS ((struct heap *));
extern void *heap_extract_min		PARAMS ((struct heap *));
extern void heap_insert			PARAMS ((struct heap *, int, void *));

