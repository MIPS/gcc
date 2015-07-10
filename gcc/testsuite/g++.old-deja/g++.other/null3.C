// { dg-do assemble  }

void x()
{
 int* p = 1==0;	// { dg-warning "converting 'false' to pointer type" "" { target { ! c++11 } } }
 // { dg-error "cannot convert" "convert" { target { c++11 } } 5 }
}
