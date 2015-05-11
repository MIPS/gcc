/* { dg-do compile } */

int main (int argc, char* argv[])
{
  int x = 5, y;

  #pragma acc enter data copyin (x)
  #pragma acc host_data use_device (x)
  {
    y = x;
  }
  #pragma acc exit data delete (x)

  return y - 5;
}
/* { dg-bogus "sorry, unimplemented: directive not yet implemented" "host_data" { xfail *-*-* } 8 } */
