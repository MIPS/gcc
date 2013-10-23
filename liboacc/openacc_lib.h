!  Copyright (C) 2013-2013 Free Software Foundation, Inc.
!  Contributed by Ilmir Usmanov <i.usmanov@samsung.com>.

!  This file is part of the GNU OpenACC Library (liboacc).

!  Liboacc is free software; you can redistribute it and/or modify it
!  under the terms of the GNU General Public License as published by
!  the Free Software Foundation; either version 3, or (at your option)
!  any later version.

!  Liboacc is distributed in the hope that it will be useful, but WITHOUT ANY
!  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
!  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
!  more details.

!  Under Section 7 of GPL version 3, you are granted additional
!  permissions described in the GCC Runtime Library Exception, version
!  3.1, as published by the Free Software Foundation.

!  You should have received a copy of the GNU General Public License and
!  a copy of the GCC Runtime Library Exception along with this program;
!  see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
!  <http://www.gnu.org/licenses/>.
!
! OpenACC device type
! see OpenACC_1.0 3.1 
!
      integer openacc_version
      parameter ( openacc_version = 201111 )
      integer acc_async_noval
      parameter ( acc_async_noval = -1 )
      integer acc_async_sync
      parameter ( acc_async_sync = -2 )

      integer acc_device_none
      parameter ( acc_device_none = 0 )
      integer acc_device_default
      parameter ( acc_device_default = 1 )
      integer acc_device_host
      parameter ( acc_device_host = 2 )
      integer acc_device_not_host
      parameter ( acc_device_not_host = 3 )
!      
! get number of devices of specified type 
! see OpenACC_1.0 3.2.1
      external acc_get_num_devices
      integer acc_get_num_devices
!
! set device type for next kernels 
! see OpenACC_1.0 3.2.2
      external acc_set_device_type
!
! set current device number
! see OpenACC_1.0 3.2.4
      external acc_get_device_type
      integer acc_get_device_type
!
! set current device number
! see OpenACC_1.0 3.2.4
      external acc_set_device_num
!
! get current device number of specified type
! see OpenACC_1.0 3.2.5
      external acc_get_device_num
      integer acc_get_device_num
! 
! test for async execution
! see OpenACC_1.0 3.2.6
      external acc_async_test
      logical acc_async_test
! 
! test all async execution
! see OpenACC_1.0 3.2.7
      external acc_async_test_all
      logical acc_async_test_all
! 
! wait for async execution
! see OpenACC_1.0 3.2.8
      external acc_async_wait
! 
! wait all async execution
! see OpenACC_1.0 3.2.9
      external acc_async_wait_all
!
! initialize device
! see OpenACC_1.0 3.2.10
      external acc_init
! 
! finalize device
! see OpenACC_1.0 3.2.11
      external acc_shutdown
!
! execution device
! see OpenACC_1.0 3.2.12
      external acc_on_device
      logical acc_on_device
