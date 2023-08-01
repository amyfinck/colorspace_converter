	.arch armv4t
	.eabi_attribute 27, 3
	.fpu neon
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 18, 4
	.file	"colorspace_converter.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	cmp	r0, #1
	sub	sp, sp, #36
	movgt	r3, #0
	mov	r4, r1
	strgt	r3, [sp, #8]
	movgt	r2, #1
	bgt	.L7
	b	.L32
.L4:
	add	r2, r2, #1
	cmp	r0, r2
	ble	.L23
.L7:
	ldr	r1, [r4, r2, asl #2]
	ldrb	r3, [r1, #0]	@ zero_extendqisi2
	cmp	r3, #45
	bne	.L4
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	cmp	r3, #99
	beq	.L24
.L5:
	ldr	r0, .L33
	bl	printf
	ldr	r1, [r4, #0]
	ldr	r0, .L33+4
	bl	printf
	mov	r0, #1
.L3:
	add	sp, sp, #36
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.L24:
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L5
	add	r2, r2, #1
	mov	r3, #1
	cmp	r0, r2
	str	r3, [sp, #8]
	bgt	.L7
.L23:
	ldr	r0, .L33+8
	bl	chdir
	ldr	r0, [r4, #4]
	ldr	r1, .L33+12
	bl	fopen
	cmp	r0, #0
	str	r0, [sp, #12]
	beq	.L25
	ldr	r0, .L33+16
	bl	chdir
	ldr	r0, .L33+20
	bl	chdir
	ldr	r3, [sp, #8]
	cmp	r3, #1
	beq	.L26
.L9:
	ldr	r0, .L33+24
	bl	chdir
	ldr	r0, [r4, #4]
	ldr	r1, .L33+28
	bl	fopen
	cmp	r0, #0
	str	r0, [sp, #28]
	beq	.L27
	ldr	r0, .L33+16
	bl	chdir
	ldr	r0, .L33+16
	bl	chdir
	mov	r0, #20
	bl	malloc
	subs	r7, r0, #0
	beq	.L20
	ldr	r1, [sp, #12]
	bl	get_image_info
	ldr	r9, [r7, #4]
	ldr	sl, [r7, #0]
	mul	fp, r9, sl
	mov	r4, fp, asl #1
	mov	r1, sl
	mov	r0, r9
	add	r4, r4, fp
	ldr	r5, [r7, #8]
	bl	check_height_width
	mov	r0, r4
	bl	malloc
	mov	r6, r0
	mov	r0, r4
	bl	malloc
	cmp	r0, #0
	cmpne	r6, #0
	movne	r3, #0
	moveq	r3, #1
	mov	r8, r0
	str	r3, [sp, #4]
	beq	.L20
	mov	r0, r5
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #12]
	bl	write_header
	ldr	r3, [sp, #8]
	cmp	r3, #1
	beq	.L28
	mov	r0, r7
	mov	r1, r6
	ldr	r2, [sp, #12]
	bl	rgb_to_ycc
	mov	r0, sl
	mov	r1, r9
	mov	r2, r6
	bl	downsample_chroma
	mov	r0, fp
	mov	r1, r8
	mov	r2, r6
	bl	ycc_to_rgb
	mov	r1, r8
	ldr	r2, [sp, #28]
	mov	r0, r7
	bl	write_pixels_rgb
	mov	r0, r7
	bl	free
	mov	r0, r6
	bl	free
	mov	r0, r8
	bl	free
	ldr	r0, [sp, #12]
	bl	fclose
	ldr	r0, [sp, #28]
	bl	fclose
	ldr	r0, [sp, #4]
	b	.L3
.L26:
	ldr	r0, .L33+32
	bl	chdir
	ldr	r0, [r4, #4]
	ldr	r1, .L33+28
	bl	fopen
	cmp	r0, #0
	str	r0, [sp, #16]
	beq	.L29
	ldr	r0, .L33+16
	bl	chdir
	ldr	r0, .L33+36
	bl	chdir
	ldr	r0, [r4, #4]
	ldr	r1, .L33+28
	bl	fopen
	cmp	r0, #0
	str	r0, [sp, #20]
	beq	.L30
	ldr	r0, .L33+16
	bl	chdir
	ldr	r0, .L33+40
	bl	chdir
	ldr	r0, [r4, #4]
	ldr	r1, .L33+28
	bl	fopen
	cmp	r0, #0
	str	r0, [sp, #24]
	beq	.L31
	ldr	r0, .L33+16
	bl	chdir
	b	.L9
.L32:
	ldr	r0, .L33+44
	bl	puts
	ldr	r1, [r4, #0]
	ldr	r0, .L33+4
	bl	printf
	mov	r0, #1
	b	.L3
.L28:
	mov	r0, r5
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	bl	write_header
	mov	r0, r5
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #12]
	bl	write_header
	mov	r4, r9, lsr #1
	mov	r0, r5
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #12]
	mov	r5, sl, lsr #1
	bl	write_header
	ldr	r0, [sp, #20]
	mov	r1, r4
	mov	r2, r5
	bl	resize_file
	ldr	r0, [sp, #24]
	mov	r1, r4
	mov	r2, r5
	bl	resize_file
	mov	r0, r7
	mov	r1, r6
	ldr	r2, [sp, #12]
	bl	rgb_to_ycc
	mov	r0, sl
	mov	r1, r9
	mov	r2, r6
	bl	downsample_chroma
	mov	r0, fp
	mov	r1, r8
	mov	r2, r6
	bl	ycc_to_rgb
	mov	r0, r7
	mov	r1, r8
	ldr	r2, [sp, #28]
	bl	write_pixels_rgb
	mov	r0, r7
	mov	r1, r6
	ldr	r2, [sp, #16]
	bl	write_pixels_luma
	mov	r0, r7
	mov	r1, r6
	ldr	r2, [sp, #20]
	bl	write_pixels_cb
	mov	r1, r6
	ldr	r2, [sp, #24]
	mov	r0, r7
	bl	write_pixels_cr
	mov	r0, r7
	bl	free
	mov	r0, r6
	bl	free
	mov	r0, r8
	bl	free
	ldr	r0, [sp, #12]
	bl	fclose
	ldr	r0, [sp, #28]
	bl	fclose
	ldr	r0, [sp, #16]
	bl	fclose
	ldr	r0, [sp, #20]
	bl	fclose
	ldr	r0, [sp, #24]
	bl	fclose
	ldr	r0, [sp, #4]
	b	.L3
.L31:
	ldr	r1, [r4, #4]
	ldr	r0, .L33+48
	bl	printf
	ldr	r0, [sp, #8]
	bl	exit
.L20:
	ldr	r0, .L33+52
	bl	puts
	mov	r0, #1
	bl	exit
.L25:
	ldr	r1, [r4, #4]
	ldr	r0, .L33+56
	bl	printf
	mov	r0, #1
	bl	exit
.L27:
	ldr	r1, [r4, #4]
	ldr	r0, .L33+60
	bl	printf
	mov	r0, #1
	bl	exit
.L29:
	ldr	r0, [sp, #12]
	bl	fclose
	ldr	r0, [sp, #16]
	bl	fclose
	ldr	r1, [r4, #4]
	ldr	r0, .L33+64
	bl	printf
	ldr	r0, [sp, #8]
	bl	exit
.L30:
	ldr	r1, [r4, #4]
	ldr	r0, .L33+68
	bl	printf
	ldr	r0, [sp, #8]
	bl	exit
.L34:
	.align	2
.L33:
	.word	.LC2
	.word	.LC1
	.word	.LC3
	.word	.LC4
	.word	.LC6
	.word	.LC7
	.word	.LC15
	.word	.LC9
	.word	.LC8
	.word	.LC11
	.word	.LC13
	.word	.LC0
	.word	.LC14
	.word	.LC17
	.word	.LC5
	.word	.LC16
	.word	.LC10
	.word	.LC12
	.size	main, .-main
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"Error: Missing mandatory input .BMP file.\000"
	.space	2
.LC1:
	.ascii	"Usage: %s <input_file.BMP> [-c]\012\000"
	.space	3
.LC2:
	.ascii	"Error: Unrecognized flag %s\012\000"
	.space	3
.LC3:
	.ascii	"input\000"
	.space	2
.LC4:
	.ascii	"r\000"
	.space	2
.LC5:
	.ascii	"Could not open %s\012\000"
	.space	1
.LC6:
	.ascii	"..\000"
	.space	1
.LC7:
	.ascii	"output\000"
	.space	1
.LC8:
	.ascii	"luma\000"
	.space	3
.LC9:
	.ascii	"w+\000"
	.space	1
.LC10:
	.ascii	"Error creating luma/%s\012\000"
.LC11:
	.ascii	"cb\000"
	.space	1
.LC12:
	.ascii	"Error creating cb/%s\012\000"
	.space	2
.LC13:
	.ascii	"cr\000"
	.space	1
.LC14:
	.ascii	"Error creating cr/%s\012\000"
	.space	2
.LC15:
	.ascii	"rgb\000"
.LC16:
	.ascii	"Error creating RBG/%s\012\000"
	.space	1
.LC17:
	.ascii	"Error: Malloc for structs failed\000"
	.ident	"GCC: (Sourcery G++ Lite 2008q3-72) 4.3.2"
	.section	.note.GNU-stack,"",%progbits