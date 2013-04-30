/* To be complied together with iso-ts-29113_2.f90.

   Test whether accessing the array from C works using
   TS29113's ISO_Fortran_binding.h

   The examples are based on TS29113's.  */

#include <ISO_Fortran_binding.h>
#include <stdio.h>
#include <stdlib.h>


void check_section1 (CFI_cdesc_t *);
void check_section2 (CFI_cdesc_t *);


void
test_establish1 (void)
{
  int ind;

  /* For establish */
  CFI_rank_t rank;
  CFI_CDESC_T(1) field;

  /* For allocate */
  CFI_index_t lower[1], upper[1];
  size_t dummy = 0;

  rank = 1;
  ind = CFI_establish ((CFI_cdesc_t *) &field, NULL, CFI_attribute_allocatable,
		       CFI_type_double, 0, rank, NULL);

  if (ind != CFI_SUCCESS) abort ();
  if (field.base_addr != NULL) abort ();
  if (field.rank != 1) abort();
  if (field.version != CFI_VERSION) abort ();
  if (field.type != CFI_type_double) abort ();
  if (field.attribute != CFI_attribute_allocatable) abort ();

  lower[0] = -1;
  upper[0] = 100;
  ind = CFI_allocate ((CFI_cdesc_t *) &field, lower, upper, dummy);
  if (ind != CFI_SUCCESS) abort ();
  if (field.elem_len != sizeof (double)) abort ();
  if (field.dim[0].lower_bound != -1) abort ();
  if (field.dim[0].extent != 100-(-1)+1) abort ();
  if (field.dim[0].sm != 1*field.elem_len) abort ();

  ind = CFI_allocate ((CFI_cdesc_t *) &field, lower, upper, dummy);
  if (ind != CFI_ERROR_BASE_ADDR_NOT_NULL) abort ();

  ind = CFI_deallocate ((CFI_cdesc_t *) &field);
  if (ind != CFI_SUCCESS) abort ();
}


void
test_establish2 (void)
{
  int ind;

  /* For establish */
  typedef struct {double x; double _Complex y;} t;
  t a_c[100];
  CFI_CDESC_T(1) a_fortran;
  CFI_index_t extent[1];

  /* For allocate */
  CFI_index_t lower[2], upper[2];
  size_t dummy = 0;

  extent[0] = 100;
  ind = CFI_establish((CFI_cdesc_t *) &a_fortran, a_c, CFI_attribute_other,
		      CFI_type_struct, sizeof(t), 1, extent);

  if (ind != CFI_SUCCESS) abort ();
  if (a_fortran.base_addr != a_c) abort ();
  if (a_fortran.rank != 1) abort();
  if (a_fortran.version != CFI_VERSION) abort ();
  if (a_fortran.type != CFI_type_struct) abort ();
  if (a_fortran.elem_len != sizeof(t)) abort ();
  if (a_fortran.attribute != CFI_attribute_other) abort ();
  if (a_fortran.dim[0].lower_bound != 0) abort ();
  if (a_fortran.dim[0].extent != 100) abort ();
  if (a_fortran.dim[0].sm != a_fortran.elem_len) abort ();

  lower[0] = -1;
  upper[0] = 100;
  ind = CFI_allocate ((CFI_cdesc_t *) &a_fortran, lower, upper, dummy);
  if (ind != CFI_INVALID_ATTRIBUTE) abort ();

  if (a_fortran.base_addr != a_c) abort ();
  if (a_fortran.rank != 1) abort();
  if (a_fortran.version != CFI_VERSION) abort ();
  if (a_fortran.type != CFI_type_struct) abort ();
  if (a_fortran.elem_len != sizeof(t)) abort ();
  if (a_fortran.attribute != CFI_attribute_other) abort ();
  if (a_fortran.dim[0].lower_bound != 0) abort ();
  if (a_fortran.dim[0].extent != 100) abort ();
  if (a_fortran.dim[0].sm != a_fortran.elem_len) abort ();
}


void
test_section1 (CFI_cdesc_t *source)
{
  int ind;

  CFI_index_t lower_bounds[] = {2}, strides[] = {5};
  CFI_CDESC_T(1) section;

  CFI_rank_t rank = 1;
  ind = CFI_establish ((CFI_cdesc_t *) &section, NULL,
                       CFI_attribute_other, CFI_type_float, 0, rank, NULL);
  if (ind != CFI_SUCCESS) abort ();
  if (section.base_addr != NULL) abort ();
  if (section.rank != 1) abort();
  if (section.version != CFI_VERSION) abort ();
  if (section.type != CFI_type_float) abort ();
  if (section.attribute != CFI_attribute_other) abort ();

  if (source->base_addr == NULL) abort ();
  if (source->rank != 1) abort();
  if (source->version != CFI_VERSION) abort ();
  if (source->type != CFI_type_float) abort ();
  if (source->attribute != CFI_attribute_other) abort ();
  if (source->elem_len != sizeof(float)) abort ();
  /* FIXME: lower_bound should be 0. */
  if (source->dim[0].lower_bound != 1) abort ();
  if (source->dim[0].extent != 100) abort ();
  if (source->dim[0].sm != source->elem_len) abort (); 

  for (ind = 0; ind < 100; ind++)
    if (((float *)source->base_addr)[ind] != 100 + ind) abort();

  ind = CFI_section ((CFI_cdesc_t *) &section, source, lower_bounds,
		     NULL, strides);
  if (ind != CFI_SUCCESS) abort ();
  /* FIXME: Off by one due to 0<->1 lower_bound issue.  */
  if (section.base_addr != source->base_addr+1*source->dim[0].sm) abort ();
  if (section.dim[0].lower_bound != 2) abort (); /* FIXME: Is this correct? */
  if (section.dim[0].extent != 20) abort ();
  if (section.dim[0].sm != source->elem_len*5) abort ();
  if (section.rank != 1) abort();
  if (section.version != CFI_VERSION) abort ();
  if (section.type != CFI_type_float) abort ();
  if (section.attribute != CFI_attribute_other) abort ();
  if (section.elem_len != sizeof(float)) abort ();

  check_section1 ((CFI_cdesc_t *) &section);
}


void
test_section2 (CFI_cdesc_t *source)
{
  int ind;
  CFI_index_t lower_bounds[] = {source->dim[0].lower_bound, 41},
	      upper_bounds[] = {source->dim[0].lower_bound+source->dim[0].extent-1, 41},
	      strides[] = {1, 0};
  CFI_CDESC_T(1) section;


  CFI_rank_t rank = 1 ;
  ind = CFI_establish ((CFI_cdesc_t *) &section, NULL,
		       CFI_attribute_other, CFI_type_float, 0, rank, NULL);

  if (ind != CFI_SUCCESS) abort ();
  if (section.base_addr != NULL) abort ();
  if (section.rank != 1) abort();
  if (section.version != CFI_VERSION) abort ();
  if (section.type != CFI_type_float) abort ();
  if (section.attribute != CFI_attribute_other) abort ();

  if (source->base_addr == NULL) abort ();
  if (source->rank != 2) abort();
  if (source->version != CFI_VERSION) abort ();
  if (source->type != CFI_type_float) abort ();
  if (source->attribute != CFI_attribute_other) abort ();
  if (source->elem_len != sizeof(float)) abort ();
  /* FIXME: Off by one due to 0<->1 lower_bound issue.  */
  if (source->dim[0].lower_bound != 1) abort ();
  if (source->dim[1].lower_bound != 1) abort ();
  if (source->dim[0].extent != 100) abort ();
  if (source->dim[1].extent != 100) abort ();
  if (source->dim[0].sm != source->elem_len) abort (); 
  if (source->dim[1].sm != 100*source->elem_len) abort ();

  ind = CFI_section ((CFI_cdesc_t *) &section, source,
		     lower_bounds, upper_bounds, strides );
  if (ind != CFI_SUCCESS) abort ();
  /* FIXME: Off by one due to 0<->1 lower_bound issue.  */
  if (section.dim[0].lower_bound != 1) abort ();
  if (section.dim[0].extent != 100) abort ();
  if (section.dim[0].sm != source->elem_len) abort ();
  /* FIXME: Off by one due to 0<->1 lower_bound issue.  */
  if (section.base_addr != source->base_addr+40*100*source->dim[0].sm) abort ();
  if (section.rank != 1) abort();
  if (section.version != CFI_VERSION) abort ();
  if (section.type != CFI_type_float) abort ();
  if (section.attribute != CFI_attribute_other) abort ();
  if (section.elem_len != sizeof(float)) abort ();

  check_section2 ((CFI_cdesc_t *) &section);
}
