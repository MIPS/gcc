/*

   Copyright (C) 2007-2008 Free Software Foundation, Inc.
   Contributed by STMicroelectronics

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
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

/* If we are compiling this, we are sure this is to build C-runtime*/
#define __LIBSTD_CRT_BUILD

#include <stdlib.h>

/* Swapping item on a char by char basis to avoid alignment issues */
#define QSORT_SWAP(_a, _b, _size) \
  do {                            \
      size_t __size = (_size);    \
      char *__src = (_a);         \
      char *__dst = (_b);         \
      do {                        \
          char __tmp = *__src;    \
          *__src++ = *__dst;      \
          *__dst++ = __tmp;       \
      } while (--__size > 0);     \
  } while (0)


LIBSTD_LPROTO_IMPL(void, qsort, void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *) )
{
    if (nmemb > 1 &&  size > 0) {
        char* first  = (char *)base;
        char* last = first + (nmemb - 1) * size;

        char* left  = first;
        char* right = last;
        char* mid   = first + (nmemb / 2) * size;

        if ((*compar) (mid, left) < 0)
            QSORT_SWAP (mid, left, size);
        if ((*compar) (right, mid) < 0) {
            QSORT_SWAP (mid, right, size);
            if ((*compar) (mid, left) < 0)
                QSORT_SWAP (mid, left, size);
        }

        while (left < right) {

            while ((*compar) (left, mid) < 0)
                left += size;
            while ((*compar) (right, mid) > 0)
                right -= size;

            if (left < right) {
                QSORT_SWAP (left, right, size);
                if (mid == left)  mid = right;
                if (mid == right) mid = left;
                left  += size;
                right -= size;
            }
            else if (left == right) {
                left  += size;
                right -= size;
            }
        }

        if (right > first)
            LIBSTD_LNAME(qsort)(first, (right - first) / size + 1, size, compar);
        if (left < last)
            LIBSTD_LNAME(qsort)(left,  (last - left)   / size + 1, size, compar);
    }
}
