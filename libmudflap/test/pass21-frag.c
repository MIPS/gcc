char *boo, *foo;
boo = alloca (100);
boo[99] = 'a';
foo = __builtin_alloca (200);
foo[44] = 'b';
