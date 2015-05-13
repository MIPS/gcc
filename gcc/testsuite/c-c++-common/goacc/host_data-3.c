/* { dg-do compile } */

int main (int argc, char* argv[])
{
  int x = 5, y;

  #pragma acc enter data copyin (x)
  /* It's not clear what attempts to use non-pointer variables "directly"
     (rather than merely taking their address) should do in host_data regions. 
     We choose to make it an error.  */
  #pragma acc host_data use_device (x) /* TODO { dg-error "" } */
  {
    y = x;
  }
  #pragma acc exit data delete (x)

  return y - 5;
}
