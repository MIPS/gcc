// { dg-do assemble  }

void x()
{
 int* p = 1==0;	// { dg-warning "converting 'false' to pointer type" "" { xfail { c++11 } } }
 // { dg-warning "cannot convert 'bool' to" "" 5 { xfail { ! c++11 } }  }
}
