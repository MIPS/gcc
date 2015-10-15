void
noprop1 (int **x, int y, int z)
{
  int *ptr = *x + y * z;
  __asm__ __volatile__ ("noprop1 %0" : : "X" (*ptr));
}

void
noprop2 (int **x, int y, int z)
{
  int *ptr = *x + y * z;
  __asm__ __volatile__ ("noprop2 %0" : : "X" (ptr));
}

int *global_var;

void
const1 (void)
{
  __asm__ __volatile__ ("const1 %0" : : "X" (global_var));
}

void
const2 (void)
{
  __asm__ __volatile__ ("const2 %0" : : "X" (*global_var));
}
