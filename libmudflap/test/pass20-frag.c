struct bar {int stuff; int array[10]; };

struct bar *foo = malloc (sizeof (struct bar));
foo->array[5] = 4;
free (foo);

