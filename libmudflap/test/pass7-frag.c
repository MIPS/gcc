char *foo;
char *bar;
foo = (char *)malloc (10);
bar = (char *)malloc (10);

memcpy(foo+1, bar+1, 9);
