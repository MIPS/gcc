int A[256];
int func (int x) { 
  return x; 
}

/* There was an error that some call exprs were inserted twice.  This will
   check that issue.  */
int main (int argc, char **argv) 
{
  int x = 0;
  A[:] = 4;
  x = func (__sec_reduce_add(A[0:5:2])) + x;
  if (x != 20)
    return 1;
  else
    return 0;
}
