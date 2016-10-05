! { dg-do compile }
! { dg-additional-options "-fdump-tree-original" }

subroutine test
  integer i, j, k

  !$acc parallel
  !$acc loop tile (1)
  do i = 1, 10
  end do

  !$acc loop tile (*)
  do i = 1, 10
  end do
  
  !$acc loop tile (1,2)
  do i = 1, 10
     do j = 1, 10
     end do
  end do

  !$acc loop tile (*,2)
  do i = 1, 10
     do j = 1, 10
     end do
  end do

  !$acc loop tile (1,*)
  do i = 1, 10
     do j = 1, 10
     end do
  end do

  !$acc loop tile (*,*)
  do i = 1, 10
     do j = 1, 10
     end do
  end do

  
  !$acc loop tile (1,2,3)
  do i = 1, 10
     do j = 1, 10
        do k = 1, 10
        end do
     end do
  end do

  !$acc loop tile (*,2,3)
  do i = 1, 10
     do j = 1, 10
        do k = 1, 10
        end do
     end do
  end do

  !$acc loop tile (1,*,3)
  do i = 1, 10
     do j = 1, 10
        do k = 1, 10
        end do
     end do
  end do

  !$acc loop tile (1,2,*)
  do i = 1, 10
     do j = 1, 10
        do k = 1, 10
        end do
     end do
  end do
  !$acc end parallel
end subroutine test

! { dg-final { scan-tree-dump-times "tile\\(1\\)" 1 "original" } }
! { dg-final { scan-tree-dump-times "tile\\(0\\)" 1 "original" } }
! { dg-final { scan-tree-dump-times "tile\\(1, 2\\)" 1 "original" } }
! { dg-final { scan-tree-dump-times "tile\\(0, 2\\)" 1 "original" } }
! { dg-final { scan-tree-dump-times "tile\\(1, 0\\)" 1 "original" } }
! { dg-final { scan-tree-dump-times "tile\\(0, 0\\)" 1 "original" } }
! { dg-final { scan-tree-dump-times "tile\\(1, 2, 3\\)" 1 "original" } }
! { dg-final { scan-tree-dump-times "tile\\(0, 2, 3\\)" 1 "original" } }
! { dg-final { scan-tree-dump-times "tile\\(1, 0, 3\\)" 1 "original" } }
! { dg-final { scan-tree-dump-times "tile\\(1, 2, 0\\)" 1 "original" } }
! { dg-final { scan-tree-dump-times "for \\(" 22 "original" } }

