! Ensure that GFortran doesn't ICE with incomplete function
! definitions.

integer function f1 ! { dg-error "Expected formal argument list in function definition" }
  !$acc routine ! { dg-error "Unclassifiable OpenACC directive" }
end function f1 ! { dg-error "Expecting END PROGRAM statement" }
