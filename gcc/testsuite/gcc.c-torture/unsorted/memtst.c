#ifdef STACK_SIZE
#define SIZE STACK_SIZE / 8
#else
#define SIZE 65536
#endif

  int a[SIZE];
  bzero (a, SIZE * 4);
      memtst (a, SIZE);
