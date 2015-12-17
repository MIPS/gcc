
	.set	nomips16
	.set	nomicromips
	.set	noreorder

	.section	.text.__mips_epi, "ax", @progbits
	.align	2
	.globl	__mips_epi_9
	.ent	__mips_epi_9
	.type   __mips_epi_9, @function
	.globl	__mips_epi_8
	.type   __mips_epi_8, @function
	.globl	__mips_epi_7
	.type   __mips_epi_7, @function
	.globl	__mips_epi_6
	.type   __mips_epi_6, @function
	.globl	__mips_epi_5
	.type   __mips_epi_5, @function
	.globl	__mips_epi_4
	.type   __mips_epi_4, @function
	.globl	__mips_epi_3
	.type   __mips_epi_3, @function
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
	.end	__mips_epi_9
	.size	__mips_epi_9, __mips_epi_9-.

	.section	.text.__mips_epi_9_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_9_ra
	.ent	__mips_epi_9_ra
	.type   __mips_epi_9_ra, @function
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
	.end	__mips_epi_9_ra
	.size	__mips_epi_9_ra, __mips_epi_9_ra-.

	.section	.text.__mips_epi_8_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_8_ra
	.ent	__mips_epi_8_ra
	.type   __mips_epi_8_ra, @function
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
	.end	__mips_epi_8_ra
	.size	__mips_epi_8_ra, __mips_epi_8_ra-.

	.section	.text.__mips_epi_7_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_7_ra
	.ent	__mips_epi_7_ra
	.type   __mips_epi_7_ra, @function
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
	.end	__mips_epi_7_ra
	.size	__mips_epi_7_ra, __mips_epi_7_ra-.

	.section	.text.__mips_epi_6_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_6_ra
	.ent	__mips_epi_6_ra
	.type   __mips_epi_6_ra, @function
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
	.end	__mips_epi_6_ra
	.size	__mips_epi_6_ra, __mips_epi_6_ra-.

	.section	.text.__mips_epi_5_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_5_ra
	.ent	__mips_epi_5_ra
	.type   __mips_epi_5_ra, @function
__mips_epi_5_ra:
	lw	$31,20($sp)
	lw	$20,16($sp)
	lw	$19,12($sp)
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	jr	$31
	addiu	$sp,$sp,24
	.end	__mips_epi_5_ra
	.size	__mips_epi_5_ra, __mips_epi_5_ra-.

	.section	.text.__mips_epi_4_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_4_ra
	.ent	__mips_epi_4_ra
	.type   __mips_epi_4_ra, @function
__mips_epi_4_ra:
	lw	$31,16($sp)
	lw	$19,12($sp)
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	jr	$31
	addiu	$sp,$sp,20
	.end	__mips_epi_4_ra
	.size	__mips_epi_4_ra, __mips_epi_4_ra-.

	.section	.text.__mips_epi_3_ra, "ax", @progbits
	.align	2
	.globl	__mips_epi_3_ra
	.ent	__mips_epi_3_ra
	.type   __mips_epi_3_ra, @function
__mips_epi_3_ra:
	lw	$31,12($sp)
	lw	$18,8($sp)
	lw	$17,4($sp)
	lw	$16,0($sp)
	jr	$31
	addiu	$sp,$sp,16
	.end	__mips_epi_3_ra
	.size	__mips_epi_3_ra, __mips_epi_3_ra-.

	.set	reorder
