struct __attribute__((packed))
{
 char c;
 short s;
 int i;
} h __attribute__((aligned(4)));

void 
foo (void)
{
 h.s = 0;
 h.i = 0;
}
