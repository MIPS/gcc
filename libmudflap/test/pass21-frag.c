char *boo, *foo;
boo = (char *) alloca (100);
boo[99] = 'a';
foo = (char *) __builtin_alloca (200);
foo[44] = 'b';
