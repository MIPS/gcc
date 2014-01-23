void exit(int c)
{
	unsigned char x = c ? 'F' : 'S';
	unsigned short port = 0x501;

	asm volatile("outb %0,%1" : : "a" (x), "d" (port));
	for(;;)
		asm volatile("hlt");
}
