
char * x;
int foo;
x = (char *) malloc (10);
strcpy (x, "123456789");
foo = strlen (x+10);
x [foo] = 1; /* we just just use foo to force execution of strlen */
