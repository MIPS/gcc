/* To be complied together with iso-ts-29113_1.f90.

   Test whether accessing the array from C works using
   TS29113's ISO_Fortran_binding.h

   The examples are based on TS29113's.  */

#include <ISO_Fortran_binding.h>
#include <stdio.h>
#include <stdlib.h>

void
test_address (CFI_cdesc_t *dv)
{
  CFI_index_t subscripts[2];
  float *address;
  int i, j;

  if (dv->rank != 2) abort ();
  if (dv->version != CFI_VERSION) abort ();
  if (dv->elem_len != sizeof (float)/ sizeof (char)) abort ();
  if (dv->attribute != CFI_attribute_other) abort ();
  if (dv->type != CFI_type_float) abort ();

  /* FIXME: TS 29113 requires lower_bound == 0,
     currently, lower_bound == 1 is used.  */
  if (dv->dim[0].lower_bound != 1) abort ();
  if (dv->dim[1].lower_bound != 1) abort ();
  if (dv->dim[0].extent != 100) abort ();
  if (dv->dim[1].extent != 100) abort ();
  if (dv->dim[0].sm != 1*dv->elem_len) abort ();
  if (dv->dim[1].sm != 100*dv->elem_len) abort ();

  /* Fixme: LB should be 0.  */ 
  for (i = 1; i <= 100; i++)
    for (j = 1; j <= 100; j++)
      { 
	subscripts[0] = j;
	subscripts[1] = i;
	address = (float *) CFI_address (dv, subscripts);
	if (*address != j + 1000*i) abort ();
      }
}

void
test_allocate (CFI_cdesc_t *dv) {
  CFI_index_t lower[2], upper[2];
  int ind;
  size_t dummy = 0;

  if (dv->rank != 2) abort ();
  if (dv->version != CFI_VERSION) abort ();
  if (dv->attribute != CFI_attribute_allocatable) abort ();
  if (dv->base_addr != NULL) abort ();
  if (dv->elem_len != sizeof (float)/ sizeof (char)) abort ();

  lower[0] = -1;
  lower[1] = 5;
  upper[0] = 100;
  upper[1] = 500;
  ind = CFI_allocate (dv, lower, upper, dummy);
  if (ind != CFI_SUCCESS) abort ();
  if (dv->dim[0].lower_bound != -1) abort ();
  if (dv->dim[1].lower_bound != 5) abort ();
  if (dv->dim[0].extent != 100-(-1)+1) abort ();
  if (dv->dim[1].extent != 500-5+1) abort ();
  if (dv->dim[0].sm != 1*dv->elem_len) abort ();
  if (dv->dim[1].sm != 102*dv->elem_len) abort ();

  ind = CFI_allocate (dv, lower, upper, dummy);
  if (ind != CFI_ERROR_BASE_ADDR_NOT_NULL) abort ();
}

void
test_deallocate (CFI_cdesc_t *dv) {
  int ind;

  if (dv->rank != 2) abort ();
  if (dv->version != CFI_VERSION) abort ();
  if (dv->attribute != CFI_attribute_allocatable) abort ();
  if (dv->base_addr == NULL) abort ();
  if (dv->elem_len != sizeof (float)/ sizeof (char)) abort ();

  if (dv->dim[0].lower_bound != -1) abort ();
  if (dv->dim[1].lower_bound != 5) abort ();
  if (dv->dim[0].extent != 100-(-1)+1) abort ();
  if (dv->dim[1].extent != 500-5+1) abort ();
  if (dv->dim[0].sm != 1*dv->elem_len) abort ();
  if (dv->dim[1].sm != 102*dv->elem_len) abort ();

  ind = CFI_deallocate (dv);
  if (ind != CFI_SUCCESS) abort ();
  if (dv->base_addr != NULL) abort ();

  ind = CFI_deallocate (dv);
  if (ind != CFI_ERROR_BASE_ADDR_NULL) abort ();
}
