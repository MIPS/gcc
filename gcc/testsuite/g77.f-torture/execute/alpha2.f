c     This was originally a compile test.
     5                7./
C     Data values were once mis-compiled on (OSF/1 ?) Alpha with -O2
c     such that, for instance, `7.' appeared as `4.' in the assembler
c     output.
      call test(a(9), 7)
      subroutine test(r, i)
      double precision r
      if (nint(r)/=i) call abort
      end

