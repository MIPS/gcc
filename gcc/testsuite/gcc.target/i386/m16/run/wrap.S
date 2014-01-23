/*
 * Code wrapper: set up a 16-bit protected mode environment with
 * paging enabled, so we can catch permission violations.
 *
 * This will be combined with the target program and fed as a single
 * 128K binary "BIOS" image to Qemu, which will load it at 0xe0000.
 *
 * The first 64K is the actual program, then immediately follows
 * PAE page tables set up so the low 1M is identity-mapped, with
 * 64K-128K as user mode with appropriate permissions and the balance
 * as RWX supervisor.
 *
 * This chunk is then loaded at 32K, and finally the reset vector at
 * -16 bytes with a jump to f000:8000.
 */
	.code16
	.text
	.globl _start
_start:
	cld

	/* Copy the program code to 0x10000, in Qemu "RAM" */
	movw $0xe000,%ax
	movw %ax,%ds
	movw $0x1000,%ax
	movw %ax,%es
	movw $0x4000,%cx
	rep; movsl

	movw %cs,%ax
	movw %ax,%ds
	xorw %ax,%ax
	movw %ax,%ss
	movl $0xfff0,%esp
	movw %ax,%es

	/* Set up GDT at 0:0x1000, followed by TSS */
	movw $gdt_master,%si
	movw $0x1000,%di
	movw $(gdt_len + 104),%cx
	shrw $2,%cx
	rep; movsl

	lgdtl gdt_desc
	lidtl idt_desc

	movl $0x30,%eax		/* Enable PAE and PSE */
	movl %eax,%cr4

	movl $0xc0000080,%ecx	/* IA32_EFER */
	rdmsr
	orl $(1 << 11),%eax	/* Enable NX */
	wrmsr

	movl $0xf0000,%eax	/* Page table base */
	movl %eax,%cr3

	movw $0x10,%dx		/* Data segment */
	movw $0x18,%cx		/* Stack segment */
	movw $0x30,%bx		/* TSS segment */
	xorw %si,%si		/* Null selector */

	movl $0x80010033,%eax
	movl %eax,%cr0
	ljmpw $0x08,$1f
1:
	movw %cx,%ss
	movw %dx,%ds
	movw %dx,%es
	movw %si,%fs		/* fs/gs should never be implicitly accessed */
	movw %si,%gs
	ltr %bx

	movw $0x2b,%dx		/* User data segment */
	pushw %dx		/* User SS */
	pushw $0xf000		/* User SP */
	pushw $0x3002		/* User FLAGS (IOPL=3) */
	pushw $0x23		/* User CS */
	movw %dx,%ds
	pushw 0xf000		/* sysseg:0xf000 holds the user IP */
	movw %dx,%es
	iretw			/* Go to user program */

	.balign 16
trap_handler:
	movb $'T',%al			/* T = trap */
	movw $0x501,%dx
	outb %al,%dx
	/* This should kill Qemu */
die:
	hlt
	jmp die

	.section ".rodata","a"
	.balign 16
gdt_desc:
	.word gdt_len
	.long 0x1000
	.word 0

gdt_master:
	.long 0,0

	/* Segment 0x08: 16-bit system code segment (in ROM) */
	.word 0xffff			/* Limit[15:0] */
	.long 0x0f0000 + 0x9b000000	/* Base[24:0] + code segment */
	.word 0x0000			/* Base[31:24] + 16-bit segment */

	/* Segment 0x10: 16-bit system data segment (in ROM) */
	.word 0xffff			/* Limit[15:0] */
	.long 0x0f0000 + 0x93000000	/* Base[24:0] + code segment */
	.word 0x0000			/* Base[31:24] + 16-bit segment */

	/* Segment 0x18: 16-bit system stack segment (in low memory) */
	.word 0xffff			/* Limit[15:0] */
	.long 0x000000 + 0x93000000	/* Base[24:0] + code segment */
	.word 0x0000			/* Base[31:24] + 16-bit segment */

	/* Segment 0x23: 16-bit user code segment */
	.word 0xffff			/* Limit[15:0] */
	.long 0x010000 + 0xfb000000	/* Base[24:0] + code segment */
	.word 0x0000			/* Base[31:24] + 16-bit segment */

	/* Segment 0x2b: 16-bit user data segment */
	.word 0xffff			/* Limit[15:0] */
	.long 0x010000 + 0xf3000000	/* Base[24:0] + code segment */
	.word 0x0000			/* Base[31:24] + 16-bit segment */

	/* Segment 0x30: TSS */
	.word 104			/* Limit */
	.long (0x001000	+ gdt_len) + 0x89000000
					/* Base[15:0] + available TSS */
	.word 0x0000			/* Base[31:24] + limit[19:16] */

gdt_len	= . - gdt_master

	/* Must immediately follow the GDT */
tss_master:
	.long 0				/* Task link */
	.long 0				/* ESP0 */
	.long 0x18			/* SS0 */
	.long 0				/* ESP1 */
	.long 0				/* SS1 */
	.long 0				/* ESP2 */
	.long 0				/* SS2 */
	.long 0xf0000			/* CR3 */
	.long 0				/* EIP */
	.long 0				/* EFLAGS */
	.long 0				/* EAX */
	.long 0				/* ECX */
	.long 0				/* EDX */
	.long 0				/* EBX */
	.long 0				/* ESP */
	.long 0				/* EBP */
	.long 0				/* ESI */
	.long 0				/* EDI */
	.long 0				/* ES */
	.long 0				/* CS */
	.long 0				/* SS */
	.long 0				/* DS */
	.long 0				/* FS */
	.long 0				/* GS */
	.long 0				/* LDTR */
	.long 0xffff0000		/* T + IObase */

idt_desc:
	.word idt_len
	.long idt + 0xf0000
	.word 0

	.balign 16
idt:
	.rept 256
	.word trap_handler		/* Offset[15:0] */
	.word 0x08			/* Code segment */
	.word 0x8600			/* Interrupt gate, DPL 0 */
	.word 0				/* Offset[31:16] */
	.endr

idt_len = . - idt
