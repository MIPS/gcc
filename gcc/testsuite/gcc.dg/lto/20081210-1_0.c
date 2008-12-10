extern void srand (unsigned int);

inline void
foo (unsigned int seed)
{
 srand (seed * seed);
}

int
main ()
{
 foo (0);
 srand ((unsigned int) (&foo));
 return 0;
}
