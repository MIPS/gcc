! PR fortran/78866
! { dg-do compile }

subroutine pr78866(x)
  integer :: x(*)
! Regarding the XFAIL, see gcc/fortran/trans-openmp.c:gfc_omp_finish_clause.
!$omp target		! { dg-error "implicit mapping of assumed size array" "" { xfail *-*-* } }
  x(1) = 1
!$omp end target
end
