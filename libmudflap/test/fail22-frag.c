struct boo { int a; };
int c;
struct boo *b = malloc (sizeof (struct boo));
__mf_set_options ("-check-initialization");
c = b->a;
(void) malloc (c); /* some dummy use of c */
