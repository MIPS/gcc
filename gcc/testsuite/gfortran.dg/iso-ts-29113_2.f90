! { dg-do compile }
! { dg-options "-fdump-tree-original" }
!
! Check whether the type files are properly set
!
subroutine test
  use iso_c_binding
  implicit none

  interface
    subroutine foo(x,i)
      type(*) :: x(..)
      integer(2), value :: i
    end subroutine foo
  end interface

! /* Array-descriptor basic types, see ISO_Fortran_binding.h.  */
! #define GFC_TYPE_INTEGER 1
! #define GFC_TYPE_LOGICAL 2
! #define GFC_TYPE_REAL 3
! #define GFC_TYPE_COMPLEX 4
! #define GFC_TYPE_CHARACTER 5
! #define GFC_TYPE_STRUCT 6
! #define GFC_TYPE_CPTR 7
! #define GFC_TYPE_CFUNPTR 8
! #define GFC_TYPE_OTHER -1

  integer(2), parameter :: CFI_Int   = 1
  integer(2), parameter :: CFI_Log   = 2
  integer(2), parameter :: CFI_Real  = 3
  integer(2), parameter :: CFI_Cmplx = 4
  integer(2), parameter :: CFI_Char = 5
  integer(2), parameter :: CFI_Struct = 6
  integer(2), parameter :: CFI_cptr = 7
  integer(2), parameter :: CFI_funcptr = 8
  integer(2), parameter :: CFI_other = -1


  integer(1), allocatable :: x_int1(:)
  integer(2), allocatable :: x_int2(:)
  integer(4), allocatable :: x_int4(:)
  integer(8), allocatable :: x_int8(:)

  logical(1), allocatable :: x_log1(:)
  logical(2), allocatable :: x_log2(:)
  logical(4), allocatable :: x_log4(:)
  logical(8), allocatable :: x_log8(:)

  real(4), allocatable :: x_real4(:)
  real(8), allocatable :: x_real8(:)

  complex(4), allocatable :: x_cmplx4(:)
  complex(8), allocatable :: x_cmplx8(:)

  character(kind=1,len=1), allocatable :: x_str1a(:)
  character(kind=1,len=:), allocatable :: x_str1b(:)
  character(kind=4,len=1), allocatable :: x_str4a(:)
  character(kind=4,len=:), allocatable :: x_str4b(:)

  type(c_ptr), allocatable :: x_cptr(:)
  type(c_funptr), allocatable :: x_funcptr(:)


  type t_seq
    sequence
    integer :: iii
  end type t_seq

  type, bind(C) :: t_bindc
    integer(c_int) :: iii
  end type t_bindc

  type :: t_ext
    integer :: iii
  end type t_ext

  type(t_seq), allocatable :: x_seq(:)
  type(t_bindc), allocatable :: x_bindc(:)
  type(t_ext), allocatable :: x_ext(:)
  class(t_ext), allocatable :: x_class(:)

  call foo(x_int1, CFI_Int + ishft (int(kind(x_int1),kind=2),8))
  call foo(x_int2, CFI_Int + ishft (int(kind(x_int2),kind=2),8))
  call foo(x_int4, CFI_Int + ishft (int(kind(x_int4),kind=2),8))
  call foo(x_int8, CFI_Int + ishft (int(kind(x_int8),kind=2),8))

  call foo(x_log1, CFI_Log + ishft (int(kind(x_log1),kind=2),8))
  call foo(x_log2, CFI_Log + ishft (int(kind(x_log2),kind=2),8))
  call foo(x_log4, CFI_Log + ishft (int(kind(x_log4),kind=2),8))
  call foo(x_log8, CFI_Log + ishft (int(kind(x_log8),kind=2),8))

  call foo(x_real4, CFI_Real + ishft (int(kind(x_real4),kind=2),8))
  call foo(x_real8, CFI_Real + ishft (int(kind(x_real8),kind=2),8))

  call foo(x_cmplx4, CFI_cmplx + ishft (int(kind(x_cmplx4),kind=2),8))
  call foo(x_cmplx8, CFI_cmplx + ishft (int(kind(x_cmplx8),kind=2),8))

  call foo(x_str1a, CFI_char + ishft (int(kind(x_str1a),kind=2),8))
  call foo(x_str1b, CFI_char + ishft (int(kind(x_str1a),kind=2),8))
  call foo(x_str4a, CFI_char + ishft (int(kind(x_str4a),kind=2),8))
  call foo(x_str4b, CFI_char + ishft (int(kind(x_str4a),kind=2),8))

  call foo(x_cptr, CFI_cptr)
  call foo(x_funcptr, CFI_funcptr)

  call foo(x_seq, CFI_struct)
  call foo(x_bindc, CFI_struct)
  call foo(x_ext, CFI_other)
  call foo(x_class, CFI_other)
end subroutine test

! { dg-final { scan-tree-dump-times "x_cmplx4.type = 1028;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_cmplx8.type = 2052;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_int1.type = 257;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_int2.type = 513;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_int4.type = 1025;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_int8.type = 2049;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_log1.type = 258;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_log2.type = 514;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_log4.type = 1026;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_log8.type = 2050;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_real4.type = 1027;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_real8.type = 2051;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_str1a.type = 261;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_str1b.type = 261;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_str4a.type = 1029;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_str4b.type = 1029;" 1 "original" } }
! { dg-final { scan-tree-dump       "x_cptr.type = (1025|2049);"  "original" } }
! { dg-final { scan-tree-dump       "x_funcptr.type = (1025|2049);" "original" } }
! { dg-final { scan-tree-dump-times "x_seq.type = 6;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_bindc.type = 6;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_ext.type = -1;" 1 "original" } }
! { dg-final { scan-tree-dump-times "x_class._data.type = -1;" 1 "original" } }

! { dg-final { scan-tree-dump-times "foo \\(&x_int1, 257\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_int2, 513\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_int4, 1025\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_int8, 2049\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_log1, 258\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_log2, 514\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_log4, 1026\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_log8, 2050\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_real4, 1027\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_real8, 2051\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_cmplx4, 1028\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_cmplx8, 2052\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_str1a, 261, 1\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_str1b, 261, .x_str1b\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_str4a, 1029, 1\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_str4b, 1029, .x_str4b\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_cptr, 7\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_funcptr, 8\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_seq, 6\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_bindc, 6\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_ext, -1\\);" 1 "original" } }
! { dg-final { scan-tree-dump-times "foo \\(&x_class._data, -1\\);" 1 "original" } }

