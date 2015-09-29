// Test for [[optimize_for_synchronized]] clone.
// { dg-do compile { target c++11 } }
// { dg-options "-fgnu-tm" }

[[optimize_for_synchronized]] void f() { }

// { dg-final { scan-assembler "_ZGTt1fv" } }
