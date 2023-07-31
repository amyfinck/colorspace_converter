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
	.file	"colorspace_converter.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	cmp	r0, #1
	sub	sp, sp, #20
	mov	r4, r1
	movgt	r8, #0
	movgt	r2, #1
	bgt	.L7
	b	.L31
.L4:
	add	r2, r2, #1
	cmp	r0, r2
	ble	.L22
.L7:
	ldr	r1, [r4, r2, asl #2]
	ldrb	r3, [r1, #0]	@ zero_extendqisi2
	cmp	r3, #45
	bne	.L4
	ldrb	r3, [r1, #1]	@ zero_extendqisi2
	cmp	r3, #99
	beq	.L23
.L5:
	ldr	r0, .L32
	bl	printf
	ldr	r1, [r4, #0]
	ldr	r0, .L32+4
	bl	printf
	mov	r0, #1
.L3:
	add	sp, sp, #20
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.L23:
	ldrb	r3, [r1, #2]	@ zero_extendqisi2
	cmp	r3, #0
	bne	.L5
	add	r2, r2, #1
	cmp	r0, r2
	mov	r8, #1
	bgt	.L7
.L22:
	ldr	r0, .L32+8
	bl	chdir
	ldr	r0, [r4, #4]
	ldr	r1, .L32+12
	bl	fopen
	subs	sl, r0, #0
	beq	.L24
	ldr	r0, .L32+16
	bl	chdir
	ldr	r0, .L32+20
	bl	chdir
	cmp	r8, #1
	beq	.L25
.L9:
	ldr	r0, .L32+24
	bl	chdir
	ldr	r0, [r4, #4]
	ldr	r1, .L32+28
	bl	fopen
	subs	r9, r0, #0
	beq	.L26
	ldr	r0, .L32+16
	bl	chdir
	ldr	r0, .L32+16
	bl	chdir
	mov	r0, #24
	bl	malloc
	mov	r4, r0
	mov	r0, #4
	bl	malloc
	mov	r7, r0
	mov	r0, #4
	bl	malloc
	mov	r5, r0
	mov	r0, #4
	bl	malloc
	cmp	r7, #0
	cmpne	r4, #0
	mov	r6, r0
	beq	.L14
	cmp	r0, #0
	cmpne	r5, #0
	movne	fp, #0
	moveq	fp, #1
	bne	.L15
.L14:
	ldr	r0, .L32+32
	bl	puts
	mov	r0, #1
	bl	exit
.L25:
	ldr	r0, .L32+36
	bl	chdir
	ldr	r0, [r4, #4]
	ldr	r1, .L32+28
	bl	fopen
	cmp	r0, #0
	str	r0, [sp, #4]
	beq	.L27
	ldr	r0, .L32+16
	bl	chdir
	ldr	r0, .L32+40
	bl	chdir
	ldr	r0, [r4, #4]
	ldr	r1, .L32+28
	bl	fopen
	cmp	r0, #0
	str	r0, [sp, #8]
	beq	.L28
	ldr	r0, .L32+16
	bl	chdir
	ldr	r0, .L32+44
	bl	chdir
	ldr	r0, [r4, #4]
	ldr	r1, .L32+28
	bl	fopen
	cmp	r0, #0
	str	r0, [sp, #12]
	beq	.L29
	ldr	r0, .L32+16
	bl	chdir
	b	.L9
.L15:
	mov	r0, r4
	mov	r1, sl
	bl	get_image_info
	ldr	r1, [r4, #0]
	ldr	r0, [r4, #4]
	bl	check_height_width
	ldr	r2, [r4, #4]
	ldr	r3, [r4, #0]
	mul	r0, r3, r2
	mov	r0, r0, asl #2
	bl	malloc
	ldr	r2, [r4, #4]
	ldr	r3, [r4, #0]
	mul	r1, r3, r2
	str	r0, [r7, #0]
	mov	r0, r1, asl #2
	bl	malloc
	ldr	r3, [r4, #0]
	ldr	r2, [r4, #4]
	mul	r1, r3, r2
	str	r0, [r5, #0]
	mov	r0, r1, asl #2
	bl	malloc
	mov	r1, r7
	str	r0, [r6, #0]
	mov	r2, sl
	mov	r0, r4
	bl	read_pixels_rgb
	ldr	r0, [r4, #8]
	mov	r1, r9
	mov	r2, sl
	bl	write_header
	cmp	r8, #1
	beq	.L30
	ldr	r0, [r4, #16]
	mov	r1, r7
	mov	r2, r5
	bl	rgb_to_ycc
	ldmia	r4, {r0, r1}	@ phole ldm
	mov	r2, r5
	bl	downsample_chroma
	ldr	r0, [r4, #16]
	mov	r1, r6
	mov	r2, r5
	bl	ycc_to_rgb
	mov	r1, r6
	mov	r2, r9
	mov	r0, r4
	bl	write_pixels_rgb
	mov	r0, r4
	bl	free
	ldr	r0, [r7, #0]
	bl	free
	ldr	r0, [r5, #0]
	bl	free
	ldr	r0, [r6, #0]
	bl	free
	mov	r0, r7
	bl	free
	mov	r0, r5
	bl	free
	mov	r0, r6
	bl	free
	mov	r0, sl
	bl	fclose
	mov	r0, r9
	bl	fclose
	mov	r0, fp
	b	.L3
.L31:
	ldr	r0, .L32+48
	bl	puts
	ldr	r1, [r4, #0]
	ldr	r0, .L32+4
	bl	printf
	mov	r0, #1
	b	.L3
.L29:
	ldr	r1, [r4, #4]
	ldr	r0, .L32+52
	bl	printf
	mov	r0, r8
	bl	exit
.L30:
	ldr	r0, [r4, #8]
	ldr	r1, [sp, #4]
	mov	r2, sl
	bl	write_header
	ldr	r0, [r4, #8]
	ldr	r1, [sp, #12]
	mov	r2, sl
	bl	write_header
	ldr	r0, [r4, #8]
	ldr	r1, [sp, #8]
	mov	r2, sl
	bl	write_header
	ldr	r1, [r4, #4]
	ldr	r2, [r4, #0]
	mov	r1, r1, lsr #1
	mov	r2, r2, lsr #1
	ldr	r0, [sp, #8]
	bl	resize_file
	ldr	r1, [r4, #4]
	ldr	r2, [r4, #0]
	mov	r1, r1, lsr #1
	mov	r2, r2, lsr #1
	ldr	r0, [sp, #12]
	bl	resize_file
	ldr	r0, [r4, #16]
	mov	r1, r7
	mov	r2, r5
	bl	rgb_to_ycc
	ldmia	r4, {r0, r1}	@ phole ldm
	mov	r2, r5
	bl	downsample_chroma
	ldr	r0, [r4, #16]
	mov	r1, r6
	mov	r2, r5
	bl	ycc_to_rgb
	mov	r0, r4
	mov	r1, r6
	mov	r2, r9
	bl	write_pixels_rgb
	mov	r0, r4
	mov	r1, r5
	ldr	r2, [sp, #4]
	bl	write_pixels_luma
	mov	r0, r4
	mov	r1, r5
	ldr	r2, [sp, #8]
	bl	write_pixels_cb
	mov	r1, r5
	ldr	r2, [sp, #12]
	mov	r0, r4
	bl	write_pixels_cr
	mov	r0, r4
	bl	free
	ldr	r0, [r7, #0]
	bl	free
	ldr	r0, [r5, #0]
	bl	free
	ldr	r0, [r6, #0]
	bl	free
	mov	r0, r7
	bl	free
	mov	r0, r5
	bl	free
	mov	r0, r6
	bl	free
	mov	r0, sl
	bl	fclose
	mov	r0, r9
	bl	fclose
	ldr	r0, [sp, #4]
	bl	fclose
	ldr	r0, [sp, #8]
	bl	fclose
	ldr	r0, [sp, #12]
	bl	fclose
	mov	r0, fp
	b	.L3
.L26:
	ldr	r1, [r4, #4]
	ldr	r0, .L32+56
	bl	printf
	mov	r0, #1
	bl	exit
.L24:
	ldr	r1, [r4, #4]
	ldr	r0, .L32+60
	bl	printf
	mov	r0, #1
	bl	exit
.L27:
	mov	r0, sl
	bl	fclose
	ldr	r0, [sp, #4]
	bl	fclose
	ldr	r1, [r4, #4]
	ldr	r0, .L32+64
	bl	printf
	mov	r0, r8
	bl	exit
.L28:
	ldr	r1, [r4, #4]
	ldr	r0, .L32+68
	bl	printf
	mov	r0, r8
	bl	exit
.L33:
	.align	2
.L32:
	.word	.LC2
	.word	.LC1
	.word	.LC3
	.word	.LC4
	.word	.LC6
	.word	.LC7
	.word	.LC15
	.word	.LC9
	.word	.LC17
	.word	.LC8
	.word	.LC11
	.word	.LC13
	.word	.LC0
	.word	.LC14
	.word	.LC16
	.word	.LC5
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
