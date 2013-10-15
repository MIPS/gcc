/* Copyright (C) 2013-2013 Free Software Foundation, Inc.
   Contributed by Ilmir Usmanov <i.usmanov@samsung.com>.

   This file is part of the GNU OpenACC Library (liboacc).

   Liboacc is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   Liboacc is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

/* This file contains Fortran wrapper routines.  */

#include "openacc.h"
#include <stdlib.h>
#include <limits.h>

int32_t
acc_get_num_devices_ (acc_device_t kind)
{
  return acc_get_num_devices (kind);
}

void
acc_set_device_type_ (acc_device_t kind)
{
  acc_set_device_type (kind);
}

acc_device_t
acc_get_device_type_ (void)
{
  return acc_get_device_type ();
}

void
acc_set_device_num_ (int32_t num, acc_device_t kind)
{
  acc_set_device_num (num, kind);
}

int32_t
acc_get_device_num_ (acc_device_t kind)
{
  return acc_get_device_num (kind);
}

int
acc_async_test_ (int arg)
{
  return acc_async_test (arg);
}

int
acc_async_test_all_ ()
{
  return acc_async_test_all ();
}

void
acc_async_wait_ (int arg)
{
  acc_async_wait (arg);
}

void
acc_async_wait_all_ ()
{
  acc_async_wait_all ();
}

void
acc_init_ (acc_device_t kind)
{
  acc_init (kind);
}

void
acc_shutdown_ (acc_device_t kind)
{
  acc_shutdown (kind);
}

int
acc_on_device_ (acc_device_t kind)
{
  return acc_on_device (kind);
}
