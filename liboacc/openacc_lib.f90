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

      module openacc_lib_kind
        implicit none
        integer, parameter :: acc_device_kind = 1
      end module
      
      module openacc_lib
        use openacc_lib_kind
        implicit none
        integer, parameter :: openacc_version = 201111
        integer, parameter :: acc_async_noval = -1 
        integer, parameter :: acc_async_sync = -2
        integer (acc_device_kind), parameter :: acc_device_none = 0
        integer (acc_device_kind), parameter :: acc_device_default = 1
        integer (acc_device_kind), parameter :: acc_device_host = 2
        integer (acc_device_kind), parameter :: acc_device_not_host = 3

        interface
          function acc_get_num_devices (devicetype)
            use openacc_lib_kind
            integer (4) :: acc_get_num_devices
            integer (acc_device_kind), intent (in) :: devicetype
          end function acc_get_num_devices
        end interface

        interface
          subroutine acc_set_device_type (devicetype)
            use openacc_lib_kind
            integer (acc_device_kind), intent (in) :: devicetype
          end subroutine acc_set_device_type
        end interface

        interface
          function acc_get_device_type ()
            use openacc_lib_kind
            integer (acc_device_kind) :: acc_get_device_type
          end function 
        end interface

        interface
          subroutine acc_set_device_num (devicenum, devicetype)
            use openacc_lib_kind
            integer (4), intent (in) :: devicenum
            integer (acc_device_kind), intent (in) :: devicetype
          end subroutine acc_set_device_num
        end interface

        interface
          function acc_get_device_num (devicetype)
            use openacc_lib_kind
            integer (4) :: acc_get_device_num
            integer (acc_device_kind), intent (in) :: devicetype
          end function acc_get_device_num
        end interface

        interface
          function acc_async_test (arg)
            logical :: acc_async_test
            integer (4), intent (in) :: arg
          end function acc_async_test
        end interface

        interface
          function acc_async_test_all ()
            logical :: acc_async_test_all
          end function acc_async_test_all
        end interface

        interface
          subroutine acc_async_wait (arg)
            integer (4), intent (in) :: arg
          end subroutine acc_async_wait
        end interface

        interface
          subroutine acc_async_wait_all ()
          end subroutine acc_async_wait_all
        end interface

        interface
          subroutine acc_init (devicetype)
            use openacc_lib_kind
            integer (acc_device_kind), intent (in) :: devicetype
          end subroutine acc_init
        end interface

        interface
          subroutine acc_shutdown (devicetype)
            use openacc_lib_kind
            integer (acc_device_kind), intent (in) :: devicetype
          end subroutine acc_shutdown
        end interface

        interface
          function acc_on_device (devicetype)
            use openacc_lib_kind
            logical :: acc_on_device
            integer (acc_device_kind), intent (in) :: devicetype
          end function acc_on_device
        end interface
      end module