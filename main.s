	.arch armv4t
	.eabi_attribute 27, 3
	.fpu neon
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"main.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #28
	ldr	r3, .L3
	fldd	d17, [r3, #0]
	ldr	r3, .L3+4
	fldd	d16, [r3, #0]
	fstd	d17, [fp, #-12]
	fstd	d16, [fp, #-20]
	fldd	d17, [fp, #-12]
	fldd	d16, [fp, #-20]
	vadd.i32	d16, d17, d16
	ldr	r3, .L3+8
	fstd	d16, [r3, #0]
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
.L4:
	.align	2
.L3:
	.word	aa
	.word	bb
	.word	ss
	.size	main, .-main
	.comm	aa,8,8
	.comm	bb,8,8
	.comm	ss,8,8
	.ident	"GCC: (Sourcery G++ Lite 2008q3-72) 4.3.2"
	.section	.note.GNU-stack,"",%progbits
