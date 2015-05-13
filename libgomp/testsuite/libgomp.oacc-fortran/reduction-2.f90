! { dg-do run }

! real reductions

program reduction_2
  implicit none

  integer, parameter    :: n = 10, gangs = 20
  integer               :: i
  real, parameter       :: e = .001
  real                  :: vresult, result
  logical               :: lresult, lvresult
  real, dimension (n) :: array

  do i = 1, n
     array(i) = i
  end do

  result = 0
  vresult = 0

  ! '+' reductions

  !$acc parallel num_gangs(gangs) copy(result)
  !$acc loop reduction(+:result) gang
  do i = 1, n
     result = result + array(i)
  end do
  !$acc end parallel

  ! Verify the results
  do i = 1, n
     vresult = vresult + array(i)
  end do

  if (abs (result - vresult) .ge. e) call abort

  result = 1
  vresult = 1

  ! '*' reductions

  !$acc parallel num_gangs(gangs) copy(result)
  !$acc loop reduction(*:result) gang
  do i = 1, n
     result = result * array(i)
  end do
  !$acc end parallel

  ! Verify the results
  do i = 1, n
     vresult = vresult * array(i)
  end do

  if (result.ne.vresult) call abort

  result = 0
  vresult = 0

  ! 'max' reductions

  !$acc parallel num_gangs(gangs) copy(result)
  !$acc loop reduction(max:result) gang
  do i = 1, n
     result = max (result, array(i))
  end do
  !$acc end parallel

  ! Verify the results
  do i = 1, n
     vresult = max (vresult, array(i))
  end do

  if (result.ne.vresult) call abort

  result = 1
  vresult = 1

  ! 'min' reductions

  !$acc parallel num_gangs(gangs) copy(result)
  !$acc loop reduction(min:result) gang
  do i = 1, n
     result = min (result, array(i))
  end do
  !$acc end parallel

  ! Verify the results
  do i = 1, n
     vresult = min (vresult, array(i))
  end do

  if (result.ne.vresult) call abort

  lresult = .false.
  lvresult = .false.

  ! '.and.' reductions

  !$acc parallel num_gangs(gangs) copy(lresult)
  !$acc loop reduction(.and.:lresult) gang
  do i = 1, n
     lresult = lresult .and. (array(i) .ge. 5)
  end do
  !$acc end parallel

  ! Verify the results
  do i = 1, n
     lvresult = lvresult .and. (array(i) .ge. 5)
  end do

  if (lresult .neqv. lvresult) call abort

  lresult = .false.
  lvresult = .false.

  ! '.or.' reductions

  !$acc parallel num_gangs(gangs) copy(lresult)
  !$acc loop reduction(.or.:lresult) gang
  do i = 1, n
     lresult = lresult .or. (array(i) .ge. 5)
  end do
  !$acc end parallel

  ! Verify the results
  do i = 1, n
     lvresult = lvresult .or. (array(i) .ge. 5)
  end do

  if (lresult .neqv. lvresult) call abort

  lresult = .false.
  lvresult = .false.

  ! '.eqv.' reductions

  !$acc parallel num_gangs(gangs) copy(lresult)
  !$acc loop reduction(.eqv.:lresult) gang
  do i = 1, n
     lresult = lresult .eqv. (array(i) .ge. 5)
  end do
  !$acc end parallel

  ! Verify the results
  do i = 1, n
     lvresult = lvresult .eqv. (array(i) .ge. 5)
  end do

  if (lresult .neqv. lvresult) call abort

  lresult = .false.
  lvresult = .false.

  ! '.neqv.' reductions

  !$acc parallel num_gangs(gangs) copy(lresult)
  !$acc loop reduction(.neqv.:lresult) gang
  do i = 1, n
     lresult = lresult .neqv. (array(i) .ge. 5)
  end do
  !$acc end parallel

  ! Verify the results
  do i = 1, n
     lvresult = lvresult .neqv. (array(i) .ge. 5)
  end do

  if (lresult .neqv. lvresult) call abort
end program reduction_2
