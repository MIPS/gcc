char *foo;
char *bar;
foo = (char *)malloc (10);
bar = (char *)malloc (10);

free(foo);

memcpy(foo, bar, 10);
