/* { dg-do compile } */

int main (int argc, char* argv[])
{
  int x[100];

  #pragma acc enter data copyin (x)
  /* Specifying an array index is not valid for host_data/use_device.  */
  #pragma acc host_data use_device (x[4]) /* { dg-error "expected '\\\)' before '\\\[' token" } */
    ;
  #pragma acc exit data delete (x)

  return 0;
}
