/*
This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef OPENACC_H
#define OPENACC_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
OpenACC device type
see OpenACC_1.0 3.1
*/
typedef enum {
  acc_device_none,
  acc_device_default,
  acc_device_host,
  acc_device_not_host
}
acc_device_t;

/*
get number of devices of specified type
see OpenACC_1.0 3.2.1
*/
int acc_get_num_devices(acc_device_t);
/*
set device type for next kernels
see OpenACC_1.0 3.2.2
*/
void acc_set_device_type(acc_device_t);
/*
get type of current device
see OpenACC_1.0 3.2.3
*/
acc_device_t acc_get_device_type(void);
/*
set current device number
see OpenACC_1.0 3.2.4
*/
void acc_set_device_num(int, acc_device_t);
/*
get current device number of specified type
see OpenACC_1.0 3.2.5
*/
int acc_get_device_num(acc_device_t);

/*
test for async execution
see OpenACC_1.0 3.2.6
*/
int acc_async_test(int);
/*
test all async execution
see OpenACC_1.0 3.2.7
*/
int acc_async_test_all(void);
/*
wait for async execution
see OpenACC_1.0 3.2.8
*/
void acc_async_wait(int);
/*
wait all async execution
see OpenACC_1.0 3.2.9
*/
void acc_async_wait_all(void);

/*
initialize device
see OpenACC_1.0 3.2.10
*/
void acc_init(acc_device_t);
/*
finalize device
see OpenACC_1.0 3.2.11
*/
void acc_shutdown(acc_device_t);
/*
execution device
see OpenACC_1.0 3.2.12
*/
int acc_on_device(acc_device_t);

/*
allocate memory on device
see OpenACC_1.0 3.2.13
*/
void* acc_malloc(size_t);
/*
free device memory
see OpenACC_1.0 3.2.14
*/
void acc_free(void*);

#ifdef __cplusplus
}
#endif


#endif
