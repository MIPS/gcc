// We mark this XFAIL because we can't test for expected linker errors.
// If we get an XPASS for this testcase, that's a bug.
// (OK) excess errors test - XFAIL *-*-*

#line 1 "template18.h"
#pragma interface
#line 13 "template18.C"
  min (1, 1); 		// should produce an undefined symbol error.
}
