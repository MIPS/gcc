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

#ifndef _CBITMAP_H
#define _CBITMAP_H 1

/* Integer list for compressed bitmap */
typedef struct int_list
{
  unsigned int len;
  unsigned int eod;
  unsigned int *data;
}
cbitmap_int_list;

/* Actual cbitmap */
typedef struct cbitmap_def
{
  unsigned int blen;
  cbitmap_int_list blist;
}
 *cbitmap;


/* Global data */
extern cbitmap cbitmap_alloc PARAMS ((unsigned int));

/* Clear a cbitmap by freeing up the linked list.  */
extern void cbitmap_clear PARAMS ((cbitmap));

/* Copy a cbitmap to another cbitmap. */
extern void cbitmap_copy PARAMS ((cbitmap, cbitmap));

/* True if two cbitmaps are identical.  */
extern int cbitmap_equal_p PARAMS ((cbitmap, cbitmap));

/* OR two cbitmaps into the first */
extern int cbitmap_a_or_b PARAMS ((cbitmap, cbitmap));

/* AND two cbitmaps into the first */
extern int cbitmap_a_and_b PARAMS ((cbitmap, cbitmap));

/* Negate entire compressed bitmap in place */
#define cbitmap_negate(a) \
do { \
a->blist.data[0] = 1 - a->blist.data[0]; \
} while (0)

extern void cbitmap_free PARAMS ((cbitmap));

/* Clear a single register in a register set.  */
extern void cbitmap_clear_bit PARAMS ((cbitmap, unsigned int));

/* Set a single register in a register set.  */
extern void cbitmap_set_bit PARAMS ((cbitmap, unsigned int));

/* Return true if a register is set in a register set.  */
extern int cbitmap_bit_p PARAMS ((cbitmap, unsigned int));

/* Debug functions to print a cbitmap linked list.  */
extern void debug_cbitmap PARAMS ((cbitmap));
extern void debug_cbitmap_file PARAMS ((FILE *, cbitmap));


/* A few compatibility/functions macros for compatibility with scbitmaps */
#define dump_cbitmap(a, b) cbitmap_print (a,b,"","\n")
#define cbitmap_zero(a) cbitmap_clear (a)
extern int cbitmap_union_of_diff
PARAMS ((cbitmap, cbitmap, cbitmap, cbitmap));
extern int cbitmap_first_set_bit PARAMS ((cbitmap));
extern int cbitmap_last_set_bit PARAMS ((cbitmap));

#endif /* _CBITMAP_H */
