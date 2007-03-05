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

