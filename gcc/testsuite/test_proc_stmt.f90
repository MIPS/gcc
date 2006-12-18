! { dg-do run }
! { dg-additional-sources proc_stmt_main.c }
module testProcStmt
  interface 
     subroutine copy() bind(c)
     end subroutine copy

     subroutine copy2() 
     end subroutine copy2
     
     subroutine foo(x,y) bind(c)
       use, intrinsic :: iso_c_binding
       real(c_double) :: x
       real(c_double) :: y
     end subroutine foo

     subroutine foo2(x,y) bind(c)
       use, intrinsic :: iso_c_binding
       real(c_double), value :: x
       real(c_double), value :: y
     end subroutine foo2
  end interface

contains
    subroutine testProcDecls() bind(c, name="testProcDecls")
      use, intrinsic :: iso_c_binding
! this next line should work, and does as of 04.12.06
!      procedure(copy), bind(c, name="myCopy") :: myCopy
! this line should also work.  the binding label (since not given) is 
! the name of the sym in all lowercase.  the bind(c) is inherited from 
! the interface defn of copy().  this is dependent on the mycopy/myCopy
! routine in the file procDecls.f90, because the binding label.  
! this one works with the defn of myCopy that has no explicit label.
! the above proc decl of myCopy works with the explicitly labeled one.
! --Rickett, 04.12.06
      procedure(copy) :: myCopy
      ! this line should work.  both are bind(c) with same "signature".
      procedure(copy), bind(c) :: myCopy2
      ! this next one is by default bind(c), since foo is bind(c)
      procedure(foo) :: myFoo
      ! this is bind(c) also, with by-value params
      procedure(foo2) :: myFoo2
      real(c_double) :: myX
      real(c_double) :: myY

      call myCopy()

      call myCopy2()

      myX = 1.1d0
      myY = 1.2d0
      ! does not use myX and myY
      call myFoo(myX, myY)
      ! does not change myX and myY, because pass-by-value
      call myFoo2(myX, myY)
      if(myX .ne. 1.1d0) then
         call abort()
      endif
      if(myY .ne. 1.2d0) then 
         call abort()
      endif
    end subroutine testProcDecls
end module testProcStmt

subroutine myCopy() bind(c)
  use, intrinsic :: iso_c_binding
  interface
     subroutine copy() bind(c)
     end subroutine copy
  end interface

  ! copy() is a C routine
  call copy()
end subroutine myCopy

subroutine myCopy2() bind(c)
  use, intrinsic :: iso_c_binding
  interface
     subroutine copy() bind(c)
     end subroutine copy
  end interface

  ! copy() is a C routine
  call copy()
end subroutine myCopy2

subroutine myFoo(x, y) bind(c)
  use, intrinsic :: iso_c_binding
  real(c_double) :: x
  real(c_double) :: y

end subroutine myFoo

subroutine myFoo2(x, y) bind(c)
  use, intrinsic :: iso_c_binding
  real(c_double), value :: x
  real(c_double), value :: y

  ! x coming in is 1.1d0
  x = x + 0.1d0
  ! y coming in is 1.2d0
  y = y + 0.1d0
end subroutine myFoo2
