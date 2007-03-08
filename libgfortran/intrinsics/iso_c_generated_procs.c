/* Implementation of the ISO_C_BINDING library helper generated functions.
   Copyright (C) 2007 Free Software Foundation, Inc.
   Contributed by Christopher Rickett.

This file is part of the GNU Fortran 95 runtime library (libgfortran).

Libgfortran is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file into combinations with other programs,
and to distribute those combinations without any restriction coming
from the use of this file.  (The General Public License restrictions
do apply in other respects; for example, they cover modification of
the file, and distribution when not linked into a combine
executable.)

Libgfortran is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with libgfortran; see the file COPYING.  If not,
write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */


#include "libgfortran.h"
#include "iso_c_binding.h"


/**
 * TODO!! This file needs finished so a function is provided for all
 * possible type/kind combinations!!
 * 
 */

#ifdef HAVE_GFC_INTEGER_1
void ISO_C_BINDING_PREFIX(c_f_pointer_i1) (c_ptr_t c_ptr_in,
					   gfc_array_void *f_ptr_out,
					   const array_t *shape);
#endif
#ifdef HAVE_GFC_INTEGER_2
void ISO_C_BINDING_PREFIX(c_f_pointer_i2) (c_ptr_t c_ptr_in,
					   gfc_array_void *f_ptr_out,
					   const array_t *shape);
#endif
#ifdef HAVE_GFC_INTEGER_4
void ISO_C_BINDING_PREFIX(c_f_pointer_i4)(c_ptr_t c_ptr_in,
					  gfc_array_void *f_ptr_out,
					  const array_t *shape);
#endif
#ifdef HAVE_GFC_INTEGER_8
void ISO_C_BINDING_PREFIX(c_f_pointer_i8)(c_ptr_t c_ptr_in,
					  gfc_array_void *f_ptr_out,
					  const array_t *shape);
#endif
#ifdef HAVE_GFC_INTEGER_16
void ISO_C_BINDING_PREFIX(c_f_pointer_i16) (c_ptr_t c_ptr_in,
					    gfc_array_void *f_ptr_out,
					    const array_t *shape);
#endif
#ifdef HAVE_GFC_REAL_4
void ISO_C_BINDING_PREFIX(c_f_pointer_r4)(c_ptr_t c_ptr_in,
					  gfc_array_void *f_ptr_out,
					  const array_t *shape);
#endif
#ifdef HAVE_GFC_REAL_8
void ISO_C_BINDING_PREFIX(c_f_pointer_r8)(c_ptr_t c_ptr_in,
					  gfc_array_void *f_ptr_out,
					  const array_t *shape);
#endif
#ifdef HAVE_GFC_REAL_10
void ISO_C_BINDING_PREFIX(c_f_pointer_r10)(c_ptr_t c_ptr_in,
					   gfc_array_void *f_ptr_out,
					   const array_t *shape);
#endif
#ifdef HAVE_GFC_REAL_16
void ISO_C_BINDING_PREFIX(c_f_pointer_r16)(c_ptr_t c_ptr_in,
					   gfc_array_void *f_ptr_out,
					   const array_t *shape);
#endif


#ifdef HAVE_GFC_INTEGER_1
/* Set the given Fortran pointer, 'f_ptr_out', to point to the given C
   address, 'c_ptr_in'.  The Fortran pointer is of type integer and
   kind=1.  The function c_f_pointer is used to set up the pointer
   descriptor.  shape is a one-dimensional array of integers
   specifying the upper bounds of the array pointed to by the given C
   address, if applicable.  'shape' is an optional parameter in
   Fortran, so if the user does not provide it, it will come in here
   as NULL.  */
void
ISO_C_BINDING_PREFIX (c_f_pointer_i1) (c_ptr_t c_ptr_in,
				       gfc_array_void *f_ptr_out,
				       const array_t *shape)
{
  /* Here we have an integer(kind=1).  */
  ISO_C_BINDING_PREFIX (c_f_pointer) (c_ptr_in, f_ptr_out, shape,
				      (int) GFC_DTYPE_INTEGER,
				      (int) sizeof (GFC_INTEGER_1));
  return;
}
#endif


#ifdef HAVE_GFC_INTEGER_2
/* Set the given Fortran pointer, 'f_ptr_out', to point to the given C
   address, 'c_ptr_in'.  The Fortran pointer is of type integer and
   kind=2.  The function c_f_pointer is used to set up the pointer
   descriptor.  shape is a one-dimensional array of integers
   specifying the upper bounds of the array pointed to by the given C
   address, if applicable.  'shape' is an optional parameter in
   Fortran, so if the user does not provide it, it will come in here
   as NULL.  */
void
ISO_C_BINDING_PREFIX (c_f_pointer_i2) (c_ptr_t c_ptr_in,
				       gfc_array_void *f_ptr_out,
				       const array_t *shape)
{
  /* here we have an integer(kind=2) */
  ISO_C_BINDING_PREFIX(c_f_pointer) (c_ptr_in, f_ptr_out, shape,
				     (int) GFC_DTYPE_INTEGER,
				     (int) sizeof (GFC_INTEGER_2));
  return;
}
#endif


#ifdef HAVE_GFC_INTEGER_4
/**
 * Set the given Fortran pointer, <code>f_ptr_out</code>, to point
 * to the given C address, <code>c_ptr_in</code>.  The Fortran pointer
 * is of type <code>integer</code> and <code>kind=4</code>.  The
 * function <code>c_f_pointer</code> is used to set up the pointer
 * descriptor.
 *
 * @param c_ptr_in The C address to associate the Fortran pointer with.
 * @param f_ptr_out The Fortran pointer to associate with the given
 * C address.
 * @param shape One-dimensional array of integers specifying the upper
 * bounds of the array pointed to by the given C address, if applicable.
 * @note: This is an optional parameter in Fortran, so if the user does
 * not provide it, it will come in here as <code>NULL</code>.
 */
void
ISO_C_BINDING_PREFIX (c_f_pointer_i4) (c_ptr_t c_ptr_in,
				       gfc_array_void *f_ptr_out,
				       const array_t *shape)
{
  /* Here we have an integer(kind=4).  */
  ISO_C_BINDING_PREFIX (c_f_pointer) (c_ptr_in, f_ptr_out, shape,
				      (int) GFC_DTYPE_INTEGER,
				      (int) sizeof (GFC_INTEGER_4));
   return;
}
#endif


#ifdef HAVE_GFC_INTEGER_8
/**
 * Set the given Fortran pointer, <code>f_ptr_out</code>, to point
 * to the given C address, <code>c_ptr_in</code>.  The Fortran pointer
 * is of type <code>integer</code> and <code>kind=8</code>.  The
 * function <code>c_f_pointer</code> is used to set up the pointer
 * descriptor.
 *
 * @param c_ptr_in The C address to associate the Fortran pointer with.
 * @param f_ptr_out The Fortran pointer to associate with the given
 * C address.
 * @param shape One-dimensional array of integers specifying the upper
 * bounds of the array pointed to by the given C address, if applicable.
 * @note: This is an optional parameter in Fortran, so if the user does
 * not provide it, it will come in here as <code>NULL</code>.
 */
void
ISO_C_BINDING_PREFIX (c_f_pointer_i8) (c_ptr_t c_ptr_in,
				       gfc_array_void *f_ptr_out,
				       const array_t *shape)
{
  /* here we have an integer(kind=8) */
  ISO_C_BINDING_PREFIX (c_f_pointer) (c_ptr_in, f_ptr_out, shape,
				      (int) GFC_DTYPE_INTEGER,
				      (int) sizeof (GFC_INTEGER_8));
  return;
}
#endif


#ifdef HAVE_GFC_INTEGER_16
/* Set the given Fortran pointer, 'f_ptr_out', to point to the given C
   address, 'c_ptr_in'.  The Fortran pointer is of type integer and
   kind=16.  The function c_f_pointer is used to set up the pointer
   descriptor.  shape is a one-dimensional array of integers
   specifying the upper bounds of the array pointed to by the given C
   address, if applicable.  'shape' is an optional parameter in
   Fortran, so if the user does not provide it, it will come in here
   as NULL.  */
void
ISO_C_BINDING_PREFIX (c_f_pointer_i16) (c_ptr_t c_ptr_in,
				        gfc_array_void *f_ptr_out,
				        const array_t *shape)
{
  /* Here we have an integer(kind=16).  */
  ISO_C_BINDING_PREFIX(c_f_pointer) (c_ptr_in, f_ptr_out, shape,
				     (int) GFC_DTYPE_INTEGER,
				     (int) sizeof (GFC_INTEGER_16));
  return;
}
#endif


#ifdef HAVE_GFC_REAL_4
/**
 * Set the given Fortran pointer, <code>f_ptr_out</code>, to point
 * to the given C address, <code>c_ptr_in</code>.  The Fortran pointer
 * is of type <code>real</code> and <code>kind=4</code>.  The
 * function <code>c_f_pointer</code> is used to set up the pointer
 * descriptor.
 *
 * @param c_ptr_in The C address to associate the Fortran pointer with.
 * @param f_ptr_out The Fortran pointer to associate with the given
 * C address.
 * @param shape One-dimensional array of integers specifying the upper
 * bounds of the array pointed to by the given C address, if applicable.
 * @note: This is an optional parameter in Fortran, so if the user does
 * not provide it, it will come in here as <code>NULL</code>.
 */
void
ISO_C_BINDING_PREFIX (c_f_pointer_r4) (c_ptr_t c_ptr_in,
				       gfc_array_void *f_ptr_out,
				       const array_t *shape)
{
  /* Here we have an real(kind=4).  */
  ISO_C_BINDING_PREFIX (c_f_pointer) (c_ptr_in, f_ptr_out, shape,
				      (int) GFC_DTYPE_REAL,
				      (int) sizeof (GFC_REAL_4));
  return;
}
#endif


#ifdef HAVE_GFC_REAL_8
/**
 * Set the given Fortran pointer, <code>f_ptr_out</code>, to point
 * to the given C address, <code>c_ptr_in</code>.  The Fortran pointer
 * is of type <code>real</code> and <code>kind=8</code>.  The
 * function <code>c_f_pointer</code> is used to set up the pointer
 * descriptor.
 *
 * @param c_ptr_in The C address to associate the Fortran pointer with.
 * @param f_ptr_out The Fortran pointer to associate with the given
 * C address.
 * @param shape One-dimensional array of integers specifying the upper
 * bounds of the array pointed to by the given C address, if applicable.
 * @note: This is an optional parameter in Fortran, so if the user does
 * not provide it, it will come in here as <code>NULL</code>.
 */
void
ISO_C_BINDING_PREFIX (c_f_pointer_r8) (c_ptr_t c_ptr_in,
				       gfc_array_void *f_ptr_out,
				       const array_t *shape)
{
  /* here we have an real(kind=8) */
  ISO_C_BINDING_PREFIX (c_f_pointer) (c_ptr_in, f_ptr_out, shape,
				      (int) GFC_DTYPE_REAL,
				      (int) sizeof (GFC_REAL_8));
  return;
}
#endif


#ifdef HAVE_GFC_REAL_10
/**
 * Set the given Fortran pointer, <code>f_ptr_out</code>, to point
 * to the given C address, <code>c_ptr_in</code>.  The Fortran pointer
 * is of type <code>real</code> and <code>kind=10</code>.  The
 * function <code>c_f_pointer</code> is used to set up the pointer
 * descriptor.
 *
 * @param c_ptr_in The C address to associate the Fortran pointer with.
 * @param f_ptr_out The Fortran pointer to associate with the given
 * C address.
 * @param shape One-dimensional array of integers specifying the upper
 * bounds of the array pointed to by the given C address, if applicable.
 * @note: This is an optional parameter in Fortran, so if the user does
 * not provide it, it will come in here as <code>NULL</code>.
 */
void
ISO_C_BINDING_PREFIX (c_f_pointer_r10) (c_ptr_t c_ptr_in,
					gfc_array_void *f_ptr_out,
					const array_t *shape)
{
  /* Here we have an real(kind=10).  */
  ISO_C_BINDING_PREFIX (c_f_pointer) (c_ptr_in, f_ptr_out, shape,
				     (int) GFC_DTYPE_REAL,
				     (int) sizeof(GFC_REAL_10));
  return;
}
#endif


#ifdef HAVE_GFC_REAL_16
/**
 * Set the given Fortran pointer, <code>f_ptr_out</code>, to point
 * to the given C address, <code>c_ptr_in</code>.  The Fortran pointer
 * is of type <code>real</code> and <code>kind=16</code>.  The
 * function <code>c_f_pointer</code> is used to set up the pointer
 * descriptor.
 *
 * @param c_ptr_in The C address to associate the Fortran pointer with.
 * @param f_ptr_out The Fortran pointer to associate with the given
 * C address.
 * @param shape One-dimensional array of integers specifying the upper
 * bounds of the array pointed to by the given C address, if applicable.
 * @note: This is an optional parameter in Fortran, so if the user does
 * not provide it, it will come in here as <code>NULL</code>.
 */
void
ISO_C_BINDING_PREFIX (c_f_pointer_r16) (c_ptr_t c_ptr_in,
					gfc_array_void *f_ptr_out,
					const array_t *shape)
{
  /* Here we have an real(kind=16).  */
  ISO_C_BINDING_PREFIX (c_f_pointer) (c_ptr_in, f_ptr_out, shape,
				      (int) GFC_DTYPE_REAL,
				      (int) sizeof (GFC_REAL_16));
  return;
}
#endif
