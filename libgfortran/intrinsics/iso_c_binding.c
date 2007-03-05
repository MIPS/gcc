/**
 * Implement the functions and subroutines provided by the intrinsic
 * iso_c_binding module.
 * --Rickett, 12.13.05
 */

#include <stdlib.h>
#include "libgfortran.h"

#include "iso_c_binding.h"


/**
 * Set the fields of a Fortran pointer descriptor to point to the
 * given C address.  It uses <code>c_f_pointer_u0</code> for the
 * common fields, and will set up the information necessary if this
 * C address is to an array (i.e., offset, type, element size).
 *
 * @param c_ptr_in The <code>c_ptr_t</code> representing the C address
 * to have Fortran point to.
 * @param f_ptr_out The Fortran pointer to have point to the given C
 * address.
 * @param shape One-dimensional array of integers specifying the upper
 * bound(s) of the array pointed to by the given C address, if applicable.
 * @note: this parameter is optional in Fortran, which will cause it to
 * come in here as <code>NULL</code>.
 * @param type The type of the data being pointed to.  @see libgfortran.h
 * where the <code>GFC_DTYPE_*</code> are defined.
 * @param elem_size The size, in bytes, of the data element being
 * pointed to.  If the address is for an array, then the size needs to
 * be the size of a single element (i.e., for an array of doubles, it
 * needs to be the number of bytes for the size of one double).
 * @return none
 */
void ISO_C_BINDING_PREFIX(c_f_pointer)(c_ptr_t c_ptr_in,
                                       gfc_array_void *f_ptr_out,
                                       const array_t *shape,
                                       int type, int elemSize)
{
   if(shape != NULL)
   {
      f_ptr_out->offset = 0;
      /* set the necessary dtype field for all pointers */
      f_ptr_out->dtype = 0;
      /* put in the element size */
      f_ptr_out->dtype = f_ptr_out->dtype |
         (elemSize << GFC_DTYPE_SIZE_SHIFT);
      /* set the data type (e.g., GFC_DTYPE_INTEGER) */
      f_ptr_out->dtype = f_ptr_out->dtype | (type << GFC_DTYPE_TYPE_SHIFT);
   }/* end if(shape arg was given) */

   /* use generic version of c_f_pointer to set common fields */
   ISO_C_BINDING_PREFIX(c_f_pointer_u0)(c_ptr_in, f_ptr_out, shape);

   return;
}/* end ISO_C_BINDING_PREFIX(c_f_pointer)() */


/**
 * A generic function to set the common fields of all descriptors,
 * no matter whether it's to a scalar or an array.  Fields set are:
 *
 * <code>data</code> 
 * <code>rank</code>          (if appropriate)
 * <code>offset</code>        (if appropriate)
 * <code>dim[*].lbound</code> (if appropriate)
 * <code>dim[*].ubound</code> (if appropriate)
 * <code>dim[*].stride</code> (if appropriate)
 *
 * All other fields are left unchanged.
 *
 * @param c_ptr_in  C address to associate <code>f_ptr_out</code> with.
 * @param f_ptr_out Fortran descriptor, as described in libgfortran.h,
 * representing a variable declared with the <code>pointer</code>
 * attribute.  Upon exit, it will be associated with the given
 * C address.
 * @param shape A rank 1 array of integers containing the upper bound
 * of each dimension of what <code>f_ptr_out</code> points to.  The
 * length of this array must be EXACTLY the rank of what
 * <code>f_ptr_out</code> points to, as required by the draft (J3/04-007).
 * If <code>f_ptr_out</code> points to a scalar, then this parameter will
 * be <code>NULL</code>.  If <code>f_ptr_out</code> does not point to a
 * scalar, then this parameter is required, as previously stated.
 * @return none
 */
void ISO_C_BINDING_PREFIX(c_f_pointer_u0)(c_ptr_t c_ptr_in,
                                          gfc_array_void *f_ptr_out,
                                          const array_t *shape)
{
   int i = 0;
   int shapeSize = 0;

   GFC_DESCRIPTOR_DATA(f_ptr_out) = c_ptr_in.c_address;

   if(shape != NULL)
   {
      f_ptr_out->offset = 0;
      shapeSize = 0;
      
      /* shape's length (rank of the output array) */
      shapeSize = shape->dim[0].ubound + 1 - shape->dim[0].lbound;
      for(i = 0; i < shapeSize; i++)
      {
         /* lower bound is 1, as specified by the draft */
         f_ptr_out->dim[i].lbound = 1;
         f_ptr_out->dim[i].ubound = ((int *)(shape->data))[i];
      }/* end for(rank of output array) */

      /* set the offset and strides */
      /* offset is (sum of (dim[i].lbound * dim[i].stride) for all dims)
       * the -1 means we'll back the data pointer up that much
       * perhaps we could just realign the data pointer and not
       * change the offset?  
       * --Rickett, 02.28.06
       */
      f_ptr_out->dim[0].stride = 1;
      f_ptr_out->offset = f_ptr_out->dim[0].lbound * f_ptr_out->dim[0].stride;
      for(i = 1; i < shapeSize; i++)
      {
         f_ptr_out->dim[i].stride = (f_ptr_out->dim[i-1].ubound+1)
            - f_ptr_out->dim[i-1].lbound;
         f_ptr_out->offset += f_ptr_out->dim[i].lbound
            * f_ptr_out->dim[i].stride;
      }/* end for(each dim of f_ptr_out) */

      f_ptr_out->offset *= -1;

      /* all we know is the rank, so set it, leaving the rest alone.
       * make NO assumptions about the state of dtype coming in!!
       * if we shift right by TYPE_SHIFT bits we'll throw away the
       * existing rank.  then, shift left by the same number to shift
       * in zeros and or with the new rank.
       */
      f_ptr_out->dtype = ((f_ptr_out->dtype >> GFC_DTYPE_TYPE_SHIFT)
                        << GFC_DTYPE_TYPE_SHIFT) | shapeSize;
   }/* end if(shape array not null) */

   return;
}/* end ISO_C_BINDING_PREFIX(c_f_pointer_u0)() */


/**
 * Sets the descriptor fields for a <code>pointer</code> to a derived
 * type, using <code>c_f_pointer_u0</code> for the majority of the
 * work.
 *
 * @param c_ptr_in The C address to point to.
 * @param f_ptr_out The Fortran pointer out.
 * @param shape One dimensional array of integers describing the bounds
 * of the array of derived types to point the Fortran pointer to, if
 * given.  @note: This is an optional parameter in Fortran, so the
 * paramater here may be NULL.
 */
void ISO_C_BINDING_PREFIX(c_f_pointer_d0)(c_ptr_t c_ptr_in,
                                          gfc_array_void *f_ptr_out,
                                          const array_t *shape)
{
   /* set the common fields */
   ISO_C_BINDING_PREFIX(c_f_pointer_u0)(c_ptr_in, f_ptr_out, shape);

   /* preserve the size and rank bits, but reset the type */
   if(shape != NULL)
   {
      f_ptr_out->dtype = f_ptr_out->dtype & (~GFC_DTYPE_TYPE_MASK);
      f_ptr_out->dtype = f_ptr_out->dtype |
         (GFC_DTYPE_DERIVED << GFC_DTYPE_TYPE_SHIFT);
   }

   return;
}/* end ISO_C_BINDING_PREFIX(c_f_pointer_d0)() */


/* this function will change, once there is an actual f90 type for the
 * proc pointer.  --Rickett, 01.04.06
 */
void ISO_C_BINDING_PREFIX(c_f_procpointer)(c_ptr_t c_ptr_in,
                                           gfc_array_void *f_ptr_out)
{
   GFC_DESCRIPTOR_DATA(f_ptr_out) = c_ptr_in.c_address;
   
   return;
}/* end ISO_C_BINDING_PREFIX(c_f_procpointer)() */


/**
 * Test if the given <code>c_ptr_t</code> is associated or not.  This
 * function is called if the user only supplied one <code>c_ptr</code>
 * parameter to the <code>c_associated</code> function.  The second
 * argument is optional, and the Fortran compiler will resolve the
 * function to this version if only one arg was given.  Associated
 * here simply means whether or not the <code>c_ptr_t</code> is
 * <code>NULL</code> or not.
 *
 * @param c_ptr_in_1 The <code>c_ptr_t</code> to test for association.
 * @return 1 if given pointer is associated; 0 if not.
 * @see #gfc_iso_c_func_interface() 
 */
GFC_LOGICAL_4 ISO_C_BINDING_PREFIX(c_associated_1)(c_ptr_t c_ptr_in_1)
{
   if(c_ptr_in_1.c_address != NULL)
      return 1;
   else
      return 0;
}/* end ISO_C_BINDING_PREFIX(c_associated_1)() */

/**
 * Test if the two <code>c_ptr_t</code> args are associated with
 * one another.  This version of the <code>c_associated</code> function
 * is called if the user supplied two <code>c_ptr</code> args in the
 * Fortran source.  According to the draft standard (J3/04-007), if 
 * <code>c_ptr_in_1</code> is <code>NULL</code>, the two pointers are
 * NOT associated.  If <code>c_ptr_in_1</code> is <code>non-NULL</code>
 * and it is not equal to <code>c_ptr_in_2</code>, then either
 * <code>c_ptr_in_2</code> is <code>NULL</code> or is associated with
 * another address; either way, the two pointers are not associated
 * with each other then.
 *
 * @param c_ptr_in_1 <code>c_ptr</code> to test for assocation.
 * @param c_ptr_in_2 <code>c_ptr</code> to test for assocation.
 * @return 1 if the two given <code>c_ptr</code> args are associated;
 * 0 if not.
 * @see #gfc_iso_c_func_interface() 
 */
GFC_LOGICAL_4 ISO_C_BINDING_PREFIX(c_associated_2)(c_ptr_t c_ptr_in_1,
                                                   c_ptr_t c_ptr_in_2)
{
   /* since we have the second arg, if it doesn't equal the first,
    * return false; true otherwise.  however, if the first one is null,
    * then return false; otherwise compare the two ptrs for equality.
    */
   if(c_ptr_in_1.c_address == NULL)
      return 0;
   else if(c_ptr_in_1.c_address != c_ptr_in_2.c_address)
      return 0;
   else
      return 1;
}/* end ISO_C_BINDING_PREFIX(c_associated_2)() */


/**
 * Return the C address of the given Fortran allocatable object.  This
 * routine is expected to return a derived type of type <code>C_PTR</code>,
 * which represents the C address of the given Fortran object.  
 * 
 * @param f90_obj The Fortran allocatable object that the user is
 * requesting the C address for.
 * @return C address for the given Fortran object.
 */
c_ptr_t ISO_C_BINDING_PREFIX(c_loc)(void *f90_obj)
{
   c_ptr_t my_c_ptr;
   
   if(f90_obj == NULL)
   {
      runtime_error("C_LOC: Attempt to get C address for Fortran object"
                    " that has not been allocated or associated");
      abort();
   }

   my_c_ptr.c_address = f90_obj;
   
   /* the "C" address should be the address of the object in Fortran */
   return my_c_ptr;
}


/*  Return the C address of the given Fortran procedure.  This
    routine is expected to return a derived type of type C_FUNPTR,
    which represents the C address of the given Fortran object.  */

c_funptr_t
ISO_C_BINDING_PREFIX (c_funloc) (void *f90_obj)
{
   c_funptr_t my_c_funptr;
   
   if (f90_obj == NULL)
     {
       runtime_error ("C_LOC: Attempt to get C address for Fortran object "
                      "that has not been allocated or associated");
       abort ();
     }

   my_c_funptr.c_address = f90_obj;
   
   /* The "C" address should be the address of the object in Fortran.  */
   return my_c_funptr;
}
