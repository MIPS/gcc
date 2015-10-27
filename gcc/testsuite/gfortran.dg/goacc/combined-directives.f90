! Exercise combined OpenACC directives.

! { dg-do compile }
! { dg-options "-fopenacc -fdump-tree-omplower" }
! { dg-prune-output "sorry, unimplemented: device_type clause is not supported yet" }

subroutine test
  implicit none
  integer a(100), i, j, z

  ! PARALLEL
  
  !$acc parallel loop collapse (2)
  do i = 1, 100
     do j = 1, 10
     end do
  end do
  !$acc end parallel loop
  
  !$acc parallel loop gang
  do i = 1, 100
  end do
  !$acc end parallel loop

  !$acc parallel loop worker
  do i = 1, 100
     do j = 1, 10
     end do
  end do
  !$acc end parallel loop

  !$acc parallel loop vector
  do i = 1, 100
     do j = 1, 10
     end do
  end do
  !$acc end parallel loop

  !$acc parallel loop seq
  do i = 1, 100
     do j = 1, 10
     end do
  end do
  !$acc end parallel loop

  !$acc parallel loop auto
  do i = 1, 100
     do j = 1, 10
     end do
  end do
  !$acc end parallel loop

  !$acc parallel loop tile (2, 3)
  do i = 1, 100
     do j = 1, 10
     end do
  end do
  !$acc end parallel loop

  !$acc parallel loop private (z) copy (a) gang device_type (nvidia) worker async (3) wait
  do i = 1, 100
     a(i) = i
  end do
  !$acc end parallel loop

  !$acc parallel loop independent
  do i = 1, 100
  end do
  !$acc end parallel loop

  !$acc parallel loop private (z)
  do i = 1, 100
     z = 0
  end do
  !$acc end parallel loop

  !$acc parallel loop reduction (+:z) copy (z)
  do i = 1, 100
  end do
  !$acc end parallel loop

  ! KERNELS

  !$acc kernels loop collapse (2)
  do i = 1, 100
     do j = 1, 10
     end do
  end do
  !$acc end kernels loop
  
  !$acc kernels loop gang
  do i = 1, 100
  end do
  !$acc end kernels loop

  !$acc kernels loop worker
  do i = 1, 100
     do j = 1, 10
     end do
  end do
  !$acc end kernels loop

  !$acc kernels loop vector
  do i = 1, 100
     do j = 1, 10
     end do
  end do
  !$acc end kernels loop

  !$acc kernels loop seq
  do i = 1, 100
     do j = 1, 10
     end do
  end do
  !$acc end kernels loop

  !$acc kernels loop auto
  do i = 1, 100
     do j = 1, 10
     end do
  end do
  !$acc end kernels loop

  !$acc kernels loop tile (2, 3)
  do i = 1, 100
     do j = 1, 10
     end do
  end do
  !$acc end kernels loop

  !$acc kernels loop private (z) copy (a) gang device_type (nvidia) worker async (3) wait
  do i = 1, 100
     a(i) = i
  end do
  !$acc end kernels loop

  !$acc kernels loop independent
  do i = 1, 100
  end do
  !$acc end kernels loop

  !$acc kernels loop private (z)
  do i = 1, 100
     z = 0
  end do
  !$acc end kernels loop

  !$acc kernels loop reduction (+:z) copy (z)
  do i = 1, 100
  end do
  !$acc end kernels loop
end subroutine test

! { dg-final { scan-tree-dump-times "acc loop collapse.2. private.j. private.i" 2 "omplower" } }
! { dg-final { scan-tree-dump-times "acc loop gang" 2 "omplower" } }
! { dg-final { scan-tree-dump-times "acc loop worker" 2 "omplower" } }
! { dg-final { scan-tree-dump-times "acc loop vector" 2 "omplower" } }
! { dg-final { scan-tree-dump-times "acc loop seq" 2 "omplower" } }
! { dg-final { scan-tree-dump-times "acc loop auto" 2 "omplower" } }
! { dg-final { scan-tree-dump-times "acc loop tile.2, 3" 2 "omplower" } }
! { dg-final { scan-tree-dump-times "device_type.nvidia. . async.3. . map.force_tofrom:a" 2 "omplower" } }
! { dg-final { scan-tree-dump-times "acc loop device_type.nvidia. . worker . gang private.i" 2 "omplower" } }
! { dg-final { scan-tree-dump-times "acc loop independent private.i" 2 "omplower" } }
! { dg-final { scan-tree-dump-times "private.z" 2 "omplower" } }
! { dg-final { scan-tree-dump-times "map.force_tofrom:z .len: 4.. reduction..:z." 2 "omplower" } }
