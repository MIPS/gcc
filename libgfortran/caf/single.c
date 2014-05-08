/* Single-image implementation of GNU Fortran Coarray Library
   Copyright (C) 2011-2014 Free Software Foundation, Inc.
   Contributed by Tobias Burnus <burnus@net-b.de>

This file is part of the GNU Fortran Coarray Runtime Library (libcaf).

Libcaf is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

Libcaf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  */

#include "libcaf.h"
#include <stdio.h>  /* For fputs and fprintf.  */
#include <stdlib.h> /* For exit and malloc.  */
#include <string.h> /* For memcpy and memset.  */
#include <stdarg.h> /* For variadic arguments.  */

/* Define GFC_CAF_CHECK to enable run-time checking.  */
/* #define GFC_CAF_CHECK  1  */

typedef void* single_token_t;
#define TOKEN(X) ((single_token_t) (X))

/* Single-image implementation of the CAF library.
   Note: For performance reasons -fcoarry=single should be used
   rather than this library.  */

/* Global variables.  */
caf_static_t *caf_static_list = NULL;


/* Keep in sync with mpi.c.  */
static void
caf_runtime_error (const char *message, ...)
{
  va_list ap;
  fprintf (stderr, "Fortran runtime error: ");
  va_start (ap, message);
  vfprintf (stderr, message, ap);
  va_end (ap);
  fprintf (stderr, "\n");

  /* FIXME: Shutdown the Fortran RTL to flush the buffer.  PR 43849.  */
  exit (EXIT_FAILURE);
}

void
_gfortran_caf_init (int *argc __attribute__ ((unused)),
		    char ***argv __attribute__ ((unused)))
{
}


void
_gfortran_caf_finalize (void)
{
  while (caf_static_list != NULL)
    {
      caf_static_t *tmp = caf_static_list->prev;
      free (caf_static_list->token);
      free (caf_static_list);
      caf_static_list = tmp;
    }
}


int
_gfortran_caf_this_image (int distance __attribute__ ((unused)))
{
  return 1;
}


int
_gfortran_caf_num_images (int distance __attribute__ ((unused)),
			  int failed __attribute__ ((unused)))
{
  return 1;
}


void *
_gfortran_caf_register (size_t size, caf_register_t type, caf_token_t *token,
			int *stat, char *errmsg, int errmsg_len)
{
  void *local;

  local = malloc (size);
  *token = malloc (sizeof (single_token_t));

  if (unlikely (local == NULL || token == NULL))
    {
      const char msg[] = "Failed to allocate coarray";
      if (stat)
	{
	  *stat = 1;
	  if (errmsg_len > 0)
	    {
	      int len = ((int) sizeof (msg) > errmsg_len) ? errmsg_len
							  : (int) sizeof (msg);
	      memcpy (errmsg, msg, len);
	      if (errmsg_len > len)
		memset (&errmsg[len], ' ', errmsg_len-len);
	    }
	  return NULL;
	}
      else
	  caf_runtime_error (msg);
    }

  *token = local;

  if (stat)
    *stat = 0;

  if (type == CAF_REGTYPE_COARRAY_STATIC)
    {
      caf_static_t *tmp = malloc (sizeof (caf_static_t));
      tmp->prev  = caf_static_list;
      tmp->token = *token;
      caf_static_list = tmp;
    }
  return local;
}


void
_gfortran_caf_deregister (caf_token_t *token, int *stat,
			  char *errmsg __attribute__ ((unused)),
			  int errmsg_len __attribute__ ((unused)))
{
  free (TOKEN(*token));

  if (stat)
    *stat = 0;
}


/* Get a scalar (or contiguous) data from remote image into a buffer.  */

void
_gfortran_caf_get (caf_token_t token, size_t offset,
		   int image_id __attribute__ ((unused)),
		   void *buffer, size_t size, bool async __attribute__ ((unused)))
{
    void *src = (void *) ((char *) TOKEN (token) + offset);
    memmove (buffer, src, size);
}


/* Get array data from a remote src to a local dest.  */

void
_gfortran_caf_get_desc (caf_token_t token, size_t offset,
			int image_id __attribute__ ((unused)),
			gfc_descriptor_t *src, gfc_descriptor_t *dest,
			bool asyn __attribute__ ((unused)))
{
  size_t i, size;
  int j;
  int rank = GFC_DESCRIPTOR_RANK (dest);

  size = 1;
  for (j = 0; j < rank; j++)
    {
      ptrdiff_t dimextent = dest->dim[j]._ubound - dest->dim[j].lower_bound + 1;
      if (dimextent < 0)
	dimextent = 0;
      size *= dimextent;
    }

  if (size == 0)
    return;

  for (i = 0; i < size; i++)
    {
      ptrdiff_t array_offset_dst = 0;
      ptrdiff_t stride = 1;
      ptrdiff_t extent = 1;
      for (j = 0; j < rank-1; j++)
	{
	  array_offset_dst += ((i / (extent*stride))
			       % (dest->dim[j]._ubound
				  - dest->dim[j].lower_bound + 1))
			      * dest->dim[j]._stride;
	  extent = (dest->dim[j]._ubound - dest->dim[j].lower_bound + 1);
	  stride = dest->dim[j]._stride;
	}
      array_offset_dst += (i / extent) * dest->dim[rank-1]._stride;

      ptrdiff_t array_offset_sr = 0;
      stride = 1;
      extent = 1;
      for (j = 0; j < GFC_DESCRIPTOR_RANK (src)-1; j++)
	{
	  array_offset_sr += ((i / (extent*stride))
			   % (src->dim[j]._ubound
			      - src->dim[j].lower_bound + 1))
			  * src->dim[j]._stride;
	  extent = (src->dim[j]._ubound - src->dim[j].lower_bound + 1);
	  stride = src->dim[j]._stride;
	}
      array_offset_sr += (i / extent) * dest->dim[rank-1]._stride;

      void *sr = (void *)((char *) TOKEN (token) + offset
			   + array_offset_sr*GFC_DESCRIPTOR_SIZE (src));
      void *dst = (void *)((char *) dest->base_addr
			  + array_offset_dst*GFC_DESCRIPTOR_SIZE (dest));
      memmove (dst, sr, GFC_DESCRIPTOR_SIZE (dest));
    }
}


/* Send scalar (or contiguous) data from buffer to a remote image.  */

void
_gfortran_caf_send (caf_token_t token, size_t offset,
		    int image_id __attribute__ ((unused)),
		    void *buffer, size_t size,
		    bool asyn __attribute__ ((unused)))
{
    void *dest = (void *)((char *) TOKEN(token) + offset);
    memmove (dest, buffer, size);
}


/* Send array data from src to dest on a remote image.  */

void
_gfortran_caf_send_desc (caf_token_t token, size_t offset,
			 int image_id __attribute__ ((unused)),
			 gfc_descriptor_t *dest, gfc_descriptor_t *src,
			 bool asyn __attribute__ ((unused)))
{
  size_t i, size;
  int j;
  int rank = GFC_DESCRIPTOR_RANK (dest);

  size = 1;
  for (j = 0; j < rank; j++)
    {
      ptrdiff_t dimextent = dest->dim[j]._ubound - dest->dim[j].lower_bound + 1;
      if (dimextent < 0)
	dimextent = 0;
      size *= dimextent;
    }

  if (size == 0)
    return;

  for (i = 0; i < size; i++)
    {
      ptrdiff_t array_offset_dst = 0;
      ptrdiff_t stride = 1;
      ptrdiff_t extent = 1;
      for (j = 0; j < rank-1; j++)
	{
	  array_offset_dst += ((i / (extent*stride))
			       % (dest->dim[j]._ubound
				  - dest->dim[j].lower_bound + 1))
			      * dest->dim[j]._stride;
	  extent = (dest->dim[j]._ubound - dest->dim[j].lower_bound + 1);
	  stride = dest->dim[j]._stride;
	}
      array_offset_dst += (i / extent) * dest->dim[rank-1]._stride;

      ptrdiff_t array_offset_sr = 0;
      stride = 1;
      extent = 1;
      for (j = 0; j < GFC_DESCRIPTOR_RANK (src)-1; j++)
	{
	  array_offset_sr += ((i / (extent*stride))
			   % (src->dim[j]._ubound
			      - src->dim[j].lower_bound + 1))
			  * src->dim[j]._stride;
	  extent = (src->dim[j]._ubound - src->dim[j].lower_bound + 1);
	  stride = src->dim[j]._stride;
	}
      array_offset_sr += (i / extent) * dest->dim[rank-1]._stride;

      void *dst = (void *)((char *) TOKEN (token) + offset
			   + array_offset_dst*GFC_DESCRIPTOR_SIZE (dest));
      void *sr = (void *)((char *) src->base_addr
			  + array_offset_sr*GFC_DESCRIPTOR_SIZE (src));
      memmove (dst, sr, GFC_DESCRIPTOR_SIZE (dest));
    }
}


/* Send scalar data from src to array dest on a remote image.  */

void
_gfortran_caf_send_desc_scalar (caf_token_t token, size_t offset,
				int image_id __attribute__ ((unused)),
				gfc_descriptor_t *dest, void *buffer,
				bool asyn __attribute__ ((unused)))
{
  size_t i, size;
  int j;
  int rank = GFC_DESCRIPTOR_RANK (dest);

  size = 1;
  for (j = 0; j < rank; j++)
    {
      ptrdiff_t dimextent = dest->dim[j]._ubound - dest->dim[j].lower_bound + 1;
      if (dimextent < 0)
	dimextent = 0;
      size *= dimextent;
    }

  for (i = 0; i < size; i++)
    {
      ptrdiff_t array_offset = 0;
      ptrdiff_t stride = 1;
      ptrdiff_t extent = 1;
      for (j = 0; j < rank-1; j++)
	{
	  array_offset += ((i / (extent*stride))
			   % (dest->dim[j]._ubound
			      - dest->dim[j].lower_bound + 1))
			  * dest->dim[j]._stride;
	  extent = (dest->dim[j]._ubound - dest->dim[j].lower_bound + 1);
	  stride = dest->dim[j]._stride;
	}
      array_offset += (i / extent) * dest->dim[rank-1]._stride;
      void *dst = (void *)((char *) TOKEN (token) + offset
			   + array_offset*GFC_DESCRIPTOR_SIZE (dest));
      memmove (dst, buffer, GFC_DESCRIPTOR_SIZE (dest));
    }
}


void
_gfortran_caf_co_sum (gfc_descriptor_t *source __attribute__ ((unused)),
		      int result_image __attribute__ ((unused)),
		      int *stat, char *errmsg __attribute__ ((unused)),
		      int errmsg_len __attribute__ ((unused)))
{
  if (stat)
    stat = 0;
}

void
_gfortran_caf_co_min (gfc_descriptor_t *source __attribute__ ((unused)),
		      int result_image __attribute__ ((unused)),
		      int *stat, char *errmsg __attribute__ ((unused)),
		      int src_len __attribute__ ((unused)),
		      int errmsg_len __attribute__ ((unused)))
{
  if (stat)
    stat = 0;
}

void
_gfortran_caf_co_max (gfc_descriptor_t *source __attribute__ ((unused)),
		      int result_image __attribute__ ((unused)),
		      int *stat, char *errmsg __attribute__ ((unused)),
		      int src_len __attribute__ ((unused)),
		      int errmsg_len __attribute__ ((unused)))
{
  if (stat)
    stat = 0;
}


void
_gfortran_caf_sync_all (int *stat,
			char *errmsg __attribute__ ((unused)),
			int errmsg_len __attribute__ ((unused)))
{
  if (stat)
    *stat = 0;
}


void
_gfortran_caf_sync_images (int count __attribute__ ((unused)),
			   int images[] __attribute__ ((unused)),
			   int *stat,
			   char *errmsg __attribute__ ((unused)),
			   int errmsg_len __attribute__ ((unused)))
{
#ifdef GFC_CAF_CHECK
  int i;

  for (i = 0; i < count; i++)
    if (images[i] != 1)
      {
	fprintf (stderr, "COARRAY ERROR: Invalid image index %d to SYNC "
		 "IMAGES", images[i]);
	exit (EXIT_FAILURE);
      }
#endif

  if (stat)
    *stat = 0;
}


void
_gfortran_caf_error_stop_str (const char *string, int32_t len)
{
  fputs ("ERROR STOP ", stderr);
  while (len--)
    fputc (*(string++), stderr);
  fputs ("\n", stderr);

  exit (1);
}


void
_gfortran_caf_error_stop (int32_t error)
{
  fprintf (stderr, "ERROR STOP %d\n", error);
  exit (error);
}


void
_gfortran_caf_co_sum (void *a __attribute__ ((unused)),
		      caf_vector_t vector[] __attribute__ ((unused)),
		      int result_image __attribute__ ((unused)),
		      int *stat, char *errmsg __attribute__ ((unused)),
		      int errmsg_len __attribute__ ((unused)))
{
  if (stat)
    stat = 0;
}

void
_gfortran_caf_co_min (void *a __attribute__ ((unused)),
		      caf_vector_t vector[] __attribute__ ((unused)),
		      int result_image __attribute__ ((unused)),
		      int *stat, char *errmsg __attribute__ ((unused)),
		      int src_len __attribute__ ((unused)),
		      int errmsg_len __attribute__ ((unused)))
{
  if (stat)
    stat = 0;
}

void
_gfortran_caf_co_max (void *a __attribute__ ((unused)),
		      caf_vector_t vector[] __attribute__ ((unused)),
		      int result_image __attribute__ ((unused)),
		      int *stat, char *errmsg __attribute__ ((unused)),
		      int src_len __attribute__ ((unused)),
		      int errmsg_len __attribute__ ((unused)))
{
  if (stat)
    stat = 0;
}
