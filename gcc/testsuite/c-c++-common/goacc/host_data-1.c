/* Test valid use of host_data directive.  */
/* { dg-do compile } */

int v0;
int v1[3][3];

void
f (void)
{
  int v2 = 3;
#pragma acc host_data use_device(v2, v0, v1)
  ;
}
