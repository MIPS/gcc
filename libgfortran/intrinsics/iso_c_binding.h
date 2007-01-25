#ifndef __iso_c_binding_h_
#define __iso_c_binding_h_

#include "libgfortran.h"

typedef struct c_ptr
{
   void *c_address;
}c_ptr_t;

typedef struct c_funptr
{
   void *c_address;
}c_funptr_t;

#define ISO_C_BINDING_PREFIX(a) __iso_c_binding_##a

void ISO_C_BINDING_PREFIX(c_f_pointer)(c_ptr_t c_ptr_in,
                                       gfc_array_void *f_ptr_out,
                                       const array_t *shape,
                                       int type, int elemSize);

/* The second param here may change, once procedure pointers are
   implemented.  */
void ISO_C_BINDING_PREFIX(c_f_procpointer)(c_ptr_t c_ptr_in,
                                           gfc_array_void *f_ptr_out);

GFC_LOGICAL_4 ISO_C_BINDING_PREFIX(c_associated_1)(c_ptr_t c_ptr_in_1);
GFC_LOGICAL_4 ISO_C_BINDING_PREFIX(c_associated_2)(c_ptr_t c_ptr_in_1,
                                                   c_ptr_t c_ptr_in_2);

void ISO_C_BINDING_PREFIX(c_f_pointer_u0)(c_ptr_t c_ptr_in,
                                          gfc_array_void *f_ptr_out,
                                          const array_t *shape);

void ISO_C_BINDING_PREFIX(c_f_pointer_d0)(c_ptr_t c_ptr_in,
                                          gfc_array_void *f_ptr_out,
                                          const array_t *shape);

c_ptr_t ISO_C_BINDING_PREFIX(c_loc)(void *f90_obj);


#endif
