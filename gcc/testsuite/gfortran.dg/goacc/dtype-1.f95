! { dg-do compile }
! { dg-options "-fopenacc -fdump-tree-omplower" }

program dtype
  integer i1, i2, i3, i4, i5, i6

!! ACC PARALLEL DEVICE_TYPE:

!$acc parallel dtype (nVidia) async (1) num_gangs (100) &
!$acc&  num_workers (100) vector_length (32) wait (1)
!$acc end parallel

!$acc parallel async (1) num_gangs (1) num_workers (1) vector_length (1) &
!$acc& wait (1) device_type (nvidia) async (2) num_gangs (200) &
!$acc&  num_workers (200) vector_length (64) wait (2)
!$acc end parallel

!$acc parallel async (1) num_gangs (1) num_workers (1) vector_length (1) &
!$acc& wait (1) device_type (nvidia) async (3) num_gangs (300) &
!$acc& num_workers (300) vector_length (128) wait (3) dtype (*) &
!$acc& async (10) num_gangs (10) num_workers (10) vector_length (10) wait (10)
!$acc end parallel

!$acc parallel async (1) num_gangs (1) num_workers (1) vector_length (1) &
!$acc& wait (1) dtype (nvidia_ptx) async (3) num_gangs (300) &
!$acc& num_workers (300) vector_length (128) wait (3) device_type (*) &
!$acc& async (10) num_gangs (10) num_workers (10) vector_length (10) wait (10)
!$acc end parallel

!! ACC KERNELS DEVICE_TYPE:

!$acc kernels device_type (nvidia) async wait
!$acc end kernels

!$acc kernels async wait dtype (nvidia) async (1) wait (1)
!$acc end kernels

!$acc kernels async wait dtype (nvidia) async (2) wait (2) &
!$acc& device_type (*) async (0) wait (0)
!$acc end kernels

!$acc kernels async wait device_type (nvidia_ptx) async (1) wait (1) &
!$acc& dtype (*) async (0) wait (0)
!$acc end kernels

!! ACC LOOP DEVICE_TYPE:

!$acc parallel
!$acc loop device_type (nVidia) gang tile (1)
  do i1 = 1, 10
     !$acc loop dtype (nVidia) worker collapse (1)
     do i2 = 1, 10
        !$acc loop device_type (nVidia) vector
        do i3 = 1, 10
           !$acc loop device_type (nVidia) auto
           do i4 = 1, 10
              !$acc loop dtype (nVidia)
              do i5 = 1, 10
                 !$acc loop dtype (nVidia) seq
                 do i6 = 1, 10
                 end do
              end do
           end do
        end do
     end do
  end do
!$acc end parallel

!$acc parallel
!$acc loop dtype (nVidia) gang tile (1) dtype (*) seq
  do i1 = 1, 10
     !$acc loop device_type (nVidia) worker collapse (1) &
     !$acc& device_type (*) seq
     do i2 = 1, 10
        !$acc loop device_type (nVidia) vector dtype (*) seq
        do i3 = 1, 10
           !$acc loop device_type (nVidia) auto dtype (*) seq
           do i4 = 1, 10
              !$acc loop dtype (nVidia) &
              !$acc& dtype (*) seq
              do i5 = 1, 10
                 !$acc loop device_type (nVidia) seq
                 do i6 = 1, 10
                 end do
              end do
           end do
        end do
     end do
  end do
!$acc end parallel

!$acc parallel
!$acc loop dtype (nVidiaGPU) gang tile (1) dtype (*) seq
  do i1 = 1, 10
     !$acc loop dtype (nVidiaGPU) worker collapse (1) &
     !$acc& device_type (*) seq
     do i2 = 1, 10
        !$acc loop device_type (nVidiaGPU) vector device_type (*) seq
        do i3 = 1, 10
           !$acc loop dtype (nVidiaGPU) auto device_type (*) seq
           do i4 = 1, 10
              !$acc loop dtype (nVidiaGPU) &
              !$acc& dtype (*) seq
              do i5 = 1, 10
                 !$acc loop dtype (nVidiaGPU) seq device_type (*) seq
                 do i6 = 1, 10
                 end do
              end do
           end do
        end do
     end do
  end do
!$acc end parallel

!! ACC UPDATE:

!$acc update host(i1) async(1) wait (1)

!$acc update host(i1) device_type(nvidia) async(2) wait (2)

!$acc update host(i1) async(1) wait (1) dtype(nvidia) async(3) wait (3)

!$acc update host(i1) async(4) wait (4) device_type(nvidia) async(5) wait (5) &
!$acc& dtype (*) async (6) wait (6)

!$acc update host(i1) async(4) wait (4) dtype(nvidia1) async(5) &
!$acc& wait (5) device_type (*) async (6) wait (6)
end program dtype

!! ACC ROUTINE:

subroutine sr1 ()
  !$acc routine device_type (nvidia) gang
end subroutine sr1

subroutine sr2 ()
  !$acc routine dtype (nvidia) worker
end subroutine sr2

subroutine sr3 ()
  !$acc routine device_type (nvidia) vector
end subroutine sr3

subroutine sr4 ()
  !$acc routine device_type (nvidia) seq
end subroutine sr4

subroutine sr5 ()
  !$acc routine dtype (nvidia) bind (foo)
end subroutine sr5

subroutine sr1a ()
  !$acc routine device_type (nvidia) gang device_type (*) seq
end subroutine sr1a

subroutine sr2a ()
  !$acc routine dtype (nvidia) worker dtype (*) seq
end subroutine sr2a

subroutine sr3a ()
  !$acc routine dtype (nvidia) vector device_type (*) seq
end subroutine sr3a

subroutine sr4a ()
  !$acc routine device_type (nvidia) seq device_type (*) worker
end subroutine sr4a

subroutine sr5a ()
  !$acc routine device_type (nvidia) bind (foo) dtype (*) seq
end subroutine sr5a

subroutine sr1b ()
  !$acc routine dtype (gpu) gang dtype (*) seq
end subroutine sr1b

subroutine sr2b ()
  !$acc routine dtype (gpu) worker device_type (*) seq
end subroutine sr2b

subroutine sr3b ()
  !$acc routine device_type (gpu) vector device_type (*) seq
end subroutine sr3b

subroutine sr4b ()
  !$acc routine device_type (gpu) seq device_type (*) worker
end subroutine sr4b

subroutine sr5b ()
  !$acc routine dtype (gpu) bind (foo) device_type (*) seq
end subroutine sr5b

! { dg-final { scan-tree-dump-times "oacc_parallel async\\(1\\) wait\\(1\\) num_gangs\\(100\\) num_workers\\(100\\) vector_length\\(32\\)" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "oacc_parallel async\\(2\\) wait\\(2\\) num_gangs\\(200\\) num_workers\\(200\\) vector_length\\(64\\)" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "oacc_parallel async\\(3\\) wait\\(3\\) num_gangs\\(300\\) num_workers\\(300\\) vector_length\\(128\\)" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "oacc_parallel async\\(10\\) wait\\(10\\) num_gangs\\(10\\) num_workers\\(10\\) vector_length\\(10\\)" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "oacc_kernels async\\(-1\\)" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "oacc_kernels async\\(1\\) wait\\(1\\)" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "oacc_kernels async\\(2\\) wait\\(2\\)" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "oacc_kernels async\\(0\\) wait\\(0\\)" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "acc loop private\\(i1\\) tile\\(1\\) gang private\\(i1\\.1\\)" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "acc loop private\\(i1\\) tile\\(1\\) gang private\\(i1\\.2\\)" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "acc loop private\\(i1\\) seq private\\(i1\\.3\\)" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "acc loop private\\(i2\\) collapse\\(1\\) worker" 2 "omplower" } }

! { dg-final { scan-tree-dump-times "acc loop private\\(i3\\) vector" 2 "omplower" } }

! { dg-final { scan-tree-dump-times "acc loop private\\(i4\\) auto" 2 "omplower" } }

! { dg-final { scan-tree-dump-times "acc loop private\\(i4\\)" 3 "omplower" } }

! { dg-final { scan-tree-dump-times "acc loop private\\(i5\\)" 3 "omplower" } }

! { dg-final { scan-tree-dump-times "acc loop private\\(i6\\) seq" 3 "omplower" } }

! { dg-final { scan-tree-dump-times "acc loop private\\(i2\\) seq" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "acc loop private\\(i4\\) seq" 1 "omplower" } }

! { dg-final { scan-tree-dump-times "acc loop private\\(i5\\) seq" 1 "omplower" } }
