/* HSA traits.
   Copyright (C) 2015 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

////////////////////////////////////////////////////////////////////////////////

#ifndef HSA_TRAITS_H_
#define HSA_TRAITS_H_

/* HSA kernel dispatch is collection of informations needed for
   a kernel dispatch.  */

struct hsa_kernel_dispatch
{
  /* Pointer to a command queue associated with a kernel dispatch agent.  */
  void *queue;
  /* Pointer to reserved memory for OMP data struct copying.  */
  void *omp_data_memory;
  /* Pointer to a memory space used for kernel arguments passing.  */
  void *kernarg_address;
  /* Kernel object.  */
  uint64_t object;
  /* Synchronization signal used for dispatch synchronization.  */
  uint64_t signal;
  /* Private segment size.  */
  uint32_t private_segment_size;
  /* Group segment size.  */
  uint32_t group_segment_size;
  /* Number of children kernel dispatches.  */
  uint64_t kernel_dispatch_count;
  /* Debug purpose argument.  */
  uint64_t debug;
  /* Kernel dispatch structures created for children kernel dispatches.  */
  struct hsa_kernel_dispatch **children_dispatches;
};

#endif // HSA_TRAITS_H_
