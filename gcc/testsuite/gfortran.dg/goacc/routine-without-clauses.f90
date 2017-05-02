! Test the OpenACC routine directive when it has no gang, worker
! vector, or seq partitioning clauses.

subroutine s1
  !$acc routine ! { dg-warning "Expected one of" }
end subroutine s1

integer function f1 ()
  !$acc routine ! { dg-warning "Expected one of" }
end function f1

module m
contains
  subroutine s2
    !$acc routine ! { dg-warning "Expected one of" }
  end subroutine s2

  integer function f2 ()
    !$acc routine ! { dg-warning "Expected one of" }
  end function f2
end module m

program t
  implicit none
  interface
     subroutine s3
       !$acc routine ! { dg-warning "Expected one of" }
     end subroutine s3

     integer function f3 ()
       !$acc routine ! { dg-warning "Expected one of" }
     end function f3
  end interface
end program t
