!program to test nested forall
program forall2
  implicit none
  integer a(4,4,4)
  integer i, j, k, m, n
  do i=1,4
    do j=1,4
      do k=1,4
        a(k,j,i) = i+j+k
      enddo
    enddo
  enddo
  m=2
  n=4
  k=1
  forall (i=k+1:n)
   forall (j=k:i+1)
     a(i,j,1:4) = a(i-1,j,1:4)+2
   end forall
  end forall
  if (any (a.ne.reshape ((/3,5,6,7,4,6,7,8,5,7,8,9,6,7,9,10,4,6,7,8,&
     5,7,8,9,6,8,9,10,7,8,10,11,5,7,8,9,6,8,9,10,7,9,10,11,8,9,11,12,&
     6,8,9,10,7,9,10,11,8,10,11,12,9,10,12,13/),(/4,4,4/)))) call abort
end
