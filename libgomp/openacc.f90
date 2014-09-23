!  OpenACC Runtime Library Definitions.

!  Copyright (C) 2013-2014 Free Software Foundation, Inc.

!  Contributed by Thomas Schwinge <thomas@codesourcery.com>.

!  This file is part of the GNU OpenMP Library (libgomp).

!  Libgomp is free software; you can redistribute it and/or modify it
!  under the terms of the GNU General Public License as published by
!  the Free Software Foundation; either version 3, or (at your option)
!  any later version.

!  Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
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

module openacc_kinds
  implicit none

  integer, parameter :: acc_device_kind = 4

end module openacc_kinds

module openacc
  use openacc_kinds
  implicit none

  integer, parameter :: openacc_version = 201306

  integer (acc_device_kind), parameter :: acc_device_none = 0
  integer (acc_device_kind), parameter :: acc_device_default = 1
  integer (acc_device_kind), parameter :: acc_device_host = 2
  integer (acc_device_kind), parameter :: acc_device_not_host = 3

  interface
     function acc_on_device (dev)
       use openacc_kinds
       logical (4) :: acc_on_device
       integer (acc_device_kind), intent (in) :: dev
     end function acc_on_device
  end interface

end module openacc
