
	.set	nomips16
	.set	nomicromips
	.set	noreorder

	.section	.text.__epi, "ax", @progbits
	.align	2
	.globl	__mips_epi_9
	.globl	__mips_epi_8
	.globl	__mips_epi_7
	.globl	__mips_epi_6
	.globl	__mips_epi_5
	.globl	__mips_epi_4
	.globl	__mips_epi_3
__mips_epi_9:
	lw	$30,32($sp)
__mips_epi_8:
	lw	$23,28($sp)
__mips_epi_7:
	lw	$22,24($sp)
__mips_epi_6:
	lw	$21,20($sp)
__mips_epi_5:
	lw	$20,16($sp)
__mips_epi_4:
	lw	$19,12($sp)
__mips_epi_3:
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	jr	$31
	addu	$sp,$sp,$9

	.section	.text.__epi_9_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_9_ra
__mips_epi_9_ra:
	lw	$31,36($sp)
	lw	$30,32($sp)
	lw	$23,28($sp)
	lw	$22,24($sp)
	lw	$21,20($sp)
	lw	$20,16($sp)
	lw	$19,12($sp)
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	jr	$31
	addiu	$sp,$sp,40

	.section	.text.__epi_8_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_8_ra
__mips_epi_8_ra:
	lw	$31,32($sp)
	lw	$23,28($sp)
	lw	$22,24($sp)
	lw	$21,20($sp)
	lw	$20,16($sp)
	lw	$19,12($sp)
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	jr	$31
	addiu	$sp,$sp,36

	.section	.text.__epi_7_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_7_ra
__mips_epi_7_ra:
	lw	$31,28($sp)
	lw	$22,24($sp)
	lw	$21,20($sp)
	lw	$20,16($sp)
	lw	$19,12($sp)
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	jr	$31
	addiu	$sp,$sp,32

	.section	.text.__epi_6_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_6_ra
__mips_epi_6_ra:
	lw	$31,24($sp)
	lw	$21,20($sp)
	lw	$20,16($sp)
	lw	$19,12($sp)
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	jr	$31
	addiu	$sp,$sp,28

	.section	.text.__epi_5_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_5_ra
__mips_epi_5_ra:
	lw	$31,20($sp)
	lw	$20,16($sp)
	lw	$19,12($sp)
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	jr	$31
	addiu	$sp,$sp,24

	.section	.text.__epi_4_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_4_ra
__mips_epi_4_ra:
	lw	$31,16($sp)
	lw	$19,12($sp)
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	jr	$31
	addiu	$sp,$sp,20

	.section	.text.__epi_3_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_3_ra
__mips_epi_3_ra:
	lw	$31,12($sp)
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	jr	$31
	addiu	$sp,$sp,16

	.set	reorder
