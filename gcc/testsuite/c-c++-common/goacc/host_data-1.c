/* Test valid use of host_data directive.  */
/* { dg-do compile } */

int v1[3][3];

void
f (void)
{
#pragma acc host_data use_device(v1)
  ;
}
