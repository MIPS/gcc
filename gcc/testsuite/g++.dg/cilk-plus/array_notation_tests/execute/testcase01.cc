
#define N 16

int main(int argc, char **argv) {
    
    int x = 0;
    float a[N];
    float b[N];

    a[:] = 1;
    b[:] = 2;

    if (a[:] > 0) {
	x++;
    }  

    // Some operations
    a[:] = 3;
    b[:] = a[:];
  
    if (b[0] != 3)
      return 1;
    else if (x != N)
      return 2;

    return 0;
}
