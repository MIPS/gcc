/* Test invalid use of host_data directive.  */
/* { dg-do compile } */

int v0;
#pragma acc host_data use_device(v0) /* { dg-error "expected" } */

void
f (void)
{
  int v2 = 3;
#pragma acc host_data copy(v2) /* { dg-error "not valid for" } */
  ;

#pragma acc host_data use_device(v2)
  ;
  /* { dg-error ".use_device. variable is neither a pointer nor an array" "" { target c } 14 } */
  /* { dg-error ".use_device. variable is neither a pointer, nor an arraynor reference to pointer or array" "" { target c++ } 14 } */
  
#pragma acc host_data use_device(v0)
  ;
  /* { dg-error ".use_device. variable is neither a pointer nor an array" "" { target c } 19 } */
  /* { dg-error ".use_device. variable is neither a pointer, nor an arraynor reference to pointer or array" "" { target c++ } 19 } */
}
