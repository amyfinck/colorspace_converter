	.arch armv4t
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 18, 4
	.file	"utilities.c"
	.text
	.align	2
	.global	check_height_width
	.type	check_height_width, %function
check_height_width:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	orr	r1, r1, r0
	tst	r1, #1
	stmfd	sp!, {r4, lr}
	beq	.L3
	ldr	r0, .L5
	bl	puts
	mov	r0, #1
	bl	exit
.L3:
	ldmfd	sp!, {r4, lr}
	bx	lr
.L6:
	.align	2
.L5:
	.word	.LC0
	.size	check_height_width, .-check_height_width
	.align	2
	.global	resize_file
	.type	resize_file, %function
resize_file:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, lr}
	sub	sp, sp, #12
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	mov	r1, #18
	mov	r2, #0
	mov	r4, r0
	bl	fseek
	subs	r5, r0, #0
	bne	.L11
	mov	r1, #4
	mov	r2, #1
	mov	r3, r4
	add	r0, sp, r1
	bl	fwrite
	mov	r0, r4
	mov	r2, r5
	mov	r1, #22
	bl	fseek
	cmp	r0, #0
	bne	.L12
	mov	r3, r4
	mov	r0, sp
	mov	r1, #4
	mov	r2, #1
	bl	fwrite
	add	sp, sp, #12
	ldmfd	sp!, {r4, r5, lr}
	bx	lr
.L11:
	ldr	r0, .L13
	bl	puts
	mov	r0, #1
	bl	exit
.L12:
	ldr	r0, .L13+4
	bl	puts
	mov	r0, #1
	bl	exit
.L14:
	.align	2
.L13:
	.word	.LC1
	.word	.LC2
	.size	resize_file, .-resize_file
	.align	2
	.global	write_header
	.type	write_header, %function
write_header:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, sl, fp, lr}
	add	r3, r0, #14
	bic	r3, r3, #7
	add	fp, sp, #28
	sub	sp, sp, r3
	mov	r8, r2
	mov	sl, r1
	mov	r2, sp
	mov	r1, #0
	mov	r3, r2, lsr #3
	mov	r6, r0
	mov	r2, r1
	mov	r0, r8
	mov	r7, r3, asl #3
	bl	fseek
	cmp	r6, #0
	movne	r5, #0
	bne	.L17
	b	.L20
.L19:
	mov	r0, r7
	mov	r1, #1
	mov	r2, r4
	mov	r3, sl
	bl	fwrite
	cmp	r4, r0
	bne	.L21
	add	r5, r5, r4
	cmp	r6, r5
	bls	.L20
.L17:
	mov	r0, r7
	mov	r1, #1
	rsb	r2, r5, r6
	mov	r3, r8
	bl	fread
	subs	r4, r0, #0
	bne	.L19
.L20:
	sub	sp, fp, #28
	ldmfd	sp!, {r4, r5, r6, r7, r8, sl, fp, lr}
	bx	lr
.L21:
	ldr	r0, .L22
	bl	puts
	mov	r0, #1
	bl	exit
.L23:
	.align	2
.L22:
	.word	.LC3
	.size	write_header, .-write_header
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"Error: width and height must be divisible by 2\000"
	.space	1
.LC1:
	.ascii	"Error seeking to width position\000"
.LC2:
	.ascii	"Error seeking to offset position\000"
	.space	3
.LC3:
	.ascii	"Error writing header\000"
	.ident	"GCC: (Sourcery G++ Lite 2008q3-72) 4.3.2"
	.section	.note.GNU-stack,"",%progbits