! { dg-do run }
module threadprivate1
  double precision :: d
!$omp threadprivate (d)
end module threadprivate1

  use threadprivate1
  integer omp_get_thread_num
  logical :: l
  l = .false.
!$omp parallel num_threads (4) reduction (.or.:l)
  d = omp_get_thread_num () + 6.5
!$omp barrier
  if (d .ne. omp_get_thread_num () + 6.5) l = .true.
!$omp end parallel
  if (l) call abort ()
end
