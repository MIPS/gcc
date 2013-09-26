! { dg-do compile } 
! { dg-options "-fopenacc" } 

module test
	implicit none
contains
	elemental subroutine test1
		!$acc parallel ! { dg-error "may not appear in PURE or ELEMENTAL procedures" }
	end subroutine test1

	pure subroutine test2
		!$acc parallel ! { dg-error "may not appear in PURE or ELEMENTAL procedures" }
	end subroutine test2

	elemental real function test3(x)
		real, intent(in) :: x
		!$acc parallel ! { dg-error "may not appear in PURE or ELEMENTAL procedures" }
		test3 = x
	end function test3

	pure real function test4(x)
		real, intent(in) :: x
		!$acc parallel ! { dg-error "may not appear in PURE or ELEMENTAL procedures" }
		test4 = x
	end function test4

	subroutine test5
		real :: x = 0.0
		integer :: i
		!$acc parallel loop collapse(1) reduction(+:x)
		do i = 1,10
			x = x + 0.3
		enddo
		print *, x
	end subroutine test5

	real function test6(x)
		real :: x
		integer :: i
		!$acc parallel loop collapse(1) reduction(+:x)
		do i = 1,10
			x = x + 0.3
		enddo
		test6 = x
	end function test6
end module test