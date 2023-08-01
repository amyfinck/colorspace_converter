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
	.file	"bmp_parser.c"
	.text
	.align	2
	.global	get_row_byte_count
	.type	get_row_byte_count, %function
get_row_byte_count:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	add	r0, r0, r0, asl #1
	ands	r2, r0, #3
	addne	r3, r0, #4
	rsbne	r0, r2, r3
	bx	lr
	.size	get_row_byte_count, .-get_row_byte_count
	.align	2
	.global	get_buffer_row_bytes
	.type	get_buffer_row_bytes, %function
get_buffer_row_bytes:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	add	r0, r0, r0, asl #1
	ands	r0, r0, #3
	rsbne	r0, r0, #4
	bx	lr
	.size	get_buffer_row_bytes, .-get_buffer_row_bytes
	.align	2
	.global	write_pixels_cr
	.type	write_pixels_cr, %function
write_pixels_cr:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	mov	r4, r0
	sub	sp, sp, #28
	mov	r0, r2
	mov	r6, r2
	mov	r5, r1
	mov	r2, #0
	ldr	r1, [r4, #8]
	bl	fseek
	cmp	r0, #0
	bne	.L20
	ldmia	r4, {r2, ip}	@ phole ldm
	mov	r7, ip, lsr #1
	mov	r2, r2, lsr #1
	mov	r3, r7, asl #1
	str	r2, [sp, #0]
	add	r3, r3, r7
	ldr	r2, [sp, #0]
	ands	r1, r3, #3
	moveq	fp, r1
	rsbne	fp, r1, #4
	cmp	r2, #0
	beq	.L17
	mov	r3, ip, asl #3
	sub	r3, r3, ip, asl #1
	str	r3, [sp, #12]
	mov	r3, #0
	stmib	sp, {r3, r5}	@ phole stm
	sub	r9, r7, #1
	add	sl, sp, #23
.L13:
	cmp	r7, #0
	beq	.L16
	subs	r8, fp, #0
	movne	r8, #1
	ldr	r5, [sp, #8]
	mov	r4, #0
	b	.L15
.L14:
	add	r4, r4, #1
	cmp	r7, r4
	add	r5, r5, #6
	bls	.L16
.L15:
	mov	r1, #1
	mvn	ip, #127
	mov	r2, r1
	mov	r3, r6
	mov	r0, sl
	strb	ip, [sp, #23]
	bl	fwrite
	mov	r1, #1
	mov	r2, r1
	mov	r3, r6
	mov	r0, sl
	bl	fwrite
	mov	r1, #1
	mov	r3, r6
	add	r0, r5, #2
	mov	r2, r1
	bl	fwrite
	cmp	r9, r4
	movne	r3, #0
	andeq	r3, r8, #1
	cmp	r3, #0
	beq	.L14
	mov	r0, r6
	mov	r1, fp
	mov	r2, #1
	add	r4, r4, #1
	bl	fseek
	cmp	r7, r4
	add	r5, r5, #6
	bhi	.L15
.L16:
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #0]
	add	r2, r2, #1
	ldr	ip, [sp, #8]
	str	r2, [sp, #4]
	cmp	r3, r2
	ldr	r2, [sp, #12]
	add	ip, ip, r2
	str	ip, [sp, #8]
	bhi	.L13
.L17:
	add	sp, sp, #28
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.L20:
	ldr	r0, .L21
	bl	puts
	mov	r0, #1
	bl	exit
.L22:
	.align	2
.L21:
	.word	.LC0
	.size	write_pixels_cr, .-write_pixels_cr
	.align	2
	.global	write_pixels_cb
	.type	write_pixels_cb, %function
write_pixels_cb:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	mov	r4, r0
	sub	sp, sp, #28
	mov	r0, r2
	mov	r6, r2
	mov	r5, r1
	mov	r2, #0
	ldr	r1, [r4, #8]
	bl	fseek
	cmp	r0, #0
	bne	.L34
	ldmia	r4, {r2, ip}	@ phole ldm
	mov	r7, ip, lsr #1
	mov	r2, r2, lsr #1
	mov	r3, r7, asl #1
	str	r2, [sp, #0]
	add	r3, r3, r7
	ldr	r2, [sp, #0]
	ands	r1, r3, #3
	moveq	fp, r1
	rsbne	fp, r1, #4
	cmp	r2, #0
	beq	.L32
	mov	r3, ip, asl #3
	sub	r3, r3, ip, asl #1
	str	r3, [sp, #12]
	mov	r3, #0
	stmib	sp, {r3, r5}	@ phole stm
	sub	r9, r7, #1
	add	sl, sp, #23
.L28:
	cmp	r7, #0
	beq	.L31
	subs	r8, fp, #0
	movne	r8, #1
	ldr	r5, [sp, #8]
	mov	r4, #0
	b	.L30
.L29:
	add	r4, r4, #1
	cmp	r7, r4
	add	r5, r5, #6
	bls	.L31
.L30:
	mov	r1, #1
	mvn	ip, #127
	mov	r2, r1
	mov	r3, r6
	add	r0, r5, r1
	strb	ip, [sp, #23]
	bl	fwrite
	mov	r1, #1
	mov	r2, r1
	mov	r3, r6
	mov	r0, sl
	bl	fwrite
	mov	r1, #1
	mov	r3, r6
	mov	r0, sl
	mov	r2, r1
	bl	fwrite
	cmp	r9, r4
	movne	r3, #0
	andeq	r3, r8, #1
	cmp	r3, #0
	beq	.L29
	mov	r0, r6
	mov	r1, fp
	mov	r2, #1
	add	r4, r4, #1
	bl	fseek
	cmp	r7, r4
	add	r5, r5, #6
	bhi	.L30
.L31:
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #0]
	add	r2, r2, #1
	ldr	ip, [sp, #8]
	str	r2, [sp, #4]
	cmp	r3, r2
	ldr	r2, [sp, #12]
	add	ip, ip, r2
	str	ip, [sp, #8]
	bhi	.L28
.L32:
	add	sp, sp, #28
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.L34:
	ldr	r0, .L35
	bl	puts
	mov	r0, #1
	bl	exit
.L36:
	.align	2
.L35:
	.word	.LC0
	.size	write_pixels_cb, .-write_pixels_cb
	.align	2
	.global	write_pixels_luma
	.type	write_pixels_luma, %function
write_pixels_luma:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	mov	r4, r0
	sub	sp, sp, #28
	str	r1, [sp, #4]
	mov	r0, r2
	mov	r6, r2
	ldr	r1, [r4, #8]
	mov	r2, #0
	bl	fseek
	cmp	r0, #0
	bne	.L51
	ldr	r8, [r4, #4]
	mov	r3, r8, asl #1
	add	r3, r3, r8
	ldr	r4, [r4, #0]
	ands	r1, r3, #3
	rsbne	r1, r1, #4
	str	r3, [sp, #16]
	cmp	r4, #0
	and	r3, r8, #1
	str	r1, [sp, #20]
	str	r4, [sp, #8]
	str	r3, [sp, #12]
	beq	.L48
	mov	r9, #0
	ldr	sl, [sp, #4]
	mov	fp, r9
.L42:
	cmp	r8, #0
	moveq	r7, r8
	addne	r5, sl, #3
	movne	r4, sl
	movne	r7, #0
	beq	.L47
.L43:
	mov	r1, #1
	mov	r2, r1
	mov	r0, r4
	mov	r3, r6
	bl	fwrite
	mov	r1, #1
	mov	r2, r1
	mov	r0, r4
	mov	r3, r6
	bl	fwrite
	mov	r1, #1
	mov	r2, r1
	mov	r0, r4
	mov	r3, r6
	bl	fwrite
	mov	r1, #1
	mov	r2, r1
	mov	r0, r5
	mov	r3, r6
	bl	fwrite
	mov	r1, #1
	mov	r2, r1
	mov	r0, r5
	mov	r3, r6
	bl	fwrite
	mov	r1, #1
	mov	r0, r5
	add	r7, r7, #2
	mov	r2, r1
	mov	r3, r6
	bl	fwrite
	cmp	r8, r7
	add	r4, r4, #6
	add	r5, r5, #6
	bhi	.L43
.L47:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne	.L52
.L44:
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bne	.L53
.L45:
	ldr	r3, [sp, #8]
	add	r9, r9, #1
	cmp	r3, r9
	ldr	r3, [sp, #16]
	add	fp, fp, r8
	add	sl, sl, r3
	bhi	.L42
.L48:
	add	sp, sp, #28
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.L53:
	mov	r0, r6
	mov	r1, r3
	mov	r2, #1
	bl	fseek
	b	.L45
.L52:
	add	r3, r7, fp
	mov	r4, r3, asl #1
	add	r4, r4, r3
	ldr	r3, [sp, #4]
	mov	r1, #1
	add	r4, r3, r4
	mov	r2, r1
	mov	r3, r6
	mov	r0, r4
	bl	fwrite
	mov	r1, #1
	mov	r2, r1
	mov	r3, r6
	mov	r0, r4
	bl	fwrite
	mov	r1, #1
	mov	r0, r4
	mov	r2, r1
	mov	r3, r6
	bl	fwrite
	b	.L44
.L51:
	ldr	r0, .L54
	bl	puts
	mov	r0, #1
	bl	exit
.L55:
	.align	2
.L54:
	.word	.LC0
	.size	write_pixels_luma, .-write_pixels_luma
	.align	2
	.global	write_pixels_rgb
	.type	write_pixels_rgb, %function
write_pixels_rgb:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	mov	r4, r0
	sub	sp, sp, #28
	str	r1, [sp, #4]
	mov	r0, r2
	mov	r6, r2
	ldr	r1, [r4, #8]
	mov	r2, #0
	bl	fseek
	cmp	r0, #0
	bne	.L70
	ldr	r8, [r4, #4]
	mov	r3, r8, asl #1
	add	r3, r3, r8
	ldr	r4, [r4, #0]
	ands	r1, r3, #3
	rsbne	r1, r1, #4
	str	r3, [sp, #16]
	cmp	r4, #0
	and	r3, r8, #1
	str	r1, [sp, #20]
	str	r4, [sp, #8]
	str	r3, [sp, #12]
	beq	.L67
	mov	r9, #0
	ldr	sl, [sp, #4]
	mov	fp, r9
.L61:
	cmp	r8, #0
	moveq	r7, r8
	addne	r5, sl, #3
	movne	r4, sl
	movne	r7, #0
	beq	.L66
.L62:
	mov	r1, #1
	mov	r2, r1
	add	r0, r4, #2
	mov	r3, r6
	bl	fwrite
	mov	r1, #1
	mov	r2, r1
	add	r0, r4, r1
	mov	r3, r6
	bl	fwrite
	mov	r1, #1
	mov	r2, r1
	mov	r0, r4
	mov	r3, r6
	bl	fwrite
	mov	r1, #1
	mov	r2, r1
	add	r0, r5, #2
	mov	r3, r6
	bl	fwrite
	mov	r1, #1
	mov	r2, r1
	add	r0, r5, r1
	mov	r3, r6
	bl	fwrite
	mov	r1, #1
	mov	r0, r5
	add	r7, r7, #2
	mov	r2, r1
	mov	r3, r6
	bl	fwrite
	cmp	r8, r7
	add	r4, r4, #6
	add	r5, r5, #6
	bhi	.L62
.L66:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne	.L71
.L63:
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bne	.L72
.L64:
	ldr	r3, [sp, #8]
	add	r9, r9, #1
	cmp	r3, r9
	ldr	r3, [sp, #16]
	add	fp, fp, r8
	add	sl, sl, r3
	bhi	.L61
.L67:
	add	sp, sp, #28
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.L72:
	mov	r0, r6
	mov	r1, r3
	mov	r2, #1
	bl	fseek
	b	.L64
.L71:
	add	r3, r7, fp
	mov	r4, r3, asl #1
	add	r4, r4, r3
	ldr	r3, [sp, #4]
	mov	r1, #1
	add	r4, r3, r4
	mov	r2, r1
	mov	r3, r6
	add	r0, r4, #2
	bl	fwrite
	mov	r1, #1
	mov	r2, r1
	mov	r3, r6
	add	r0, r4, r1
	bl	fwrite
	mov	r1, #1
	mov	r0, r4
	mov	r2, r1
	mov	r3, r6
	bl	fwrite
	b	.L63
.L70:
	ldr	r0, .L73
	bl	puts
	mov	r0, #1
	bl	exit
.L74:
	.align	2
.L73:
	.word	.LC0
	.size	write_pixels_rgb, .-write_pixels_rgb
	.align	2
	.global	read_pixels_rgb
	.type	read_pixels_rgb, %function
read_pixels_rgb:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
	mov	sl, r0
	mov	r6, r2
	mov	r0, r2
	mov	r9, r1
	mov	r2, #0
	ldr	r1, [sl, #8]
	bl	fseek
	cmp	r0, #0
	bne	.L87
	ldr	r2, [sl, #4]
	mov	r3, r2, asl #1
	add	r3, r3, r2
	ands	r1, r3, #3
	ldr	r3, [sl, #0]
	moveq	fp, r1
	rsbne	fp, r1, #4
	cmp	r3, #0
	movne	r8, #0
	beq	.L85
.L80:
	cmp	r2, #0
	movne	r7, #0
	beq	.L84
.L81:
	mla	r4, r8, r2, r7
	mov	r5, r4, asl #1
	add	r5, r5, r4
	add	r5, r9, r5
	mov	r1, #1
	mov	r2, r1
	mov	r3, r6
	add	r0, r5, #2
	bl	fread
	mov	r1, #1
	mov	r2, r1
	add	r0, r5, r1
	mov	r3, r6
	add	r4, r4, #1
	bl	fread
	mov	r1, #1
	mov	r0, r5
	mov	r5, r4, asl #1
	mov	r2, r1
	mov	r3, r6
	add	r5, r5, r4
	bl	fread
	add	r5, r9, r5
	mov	r1, #1
	mov	r2, r1
	mov	r3, r6
	add	r0, r5, #2
	bl	fread
	mov	r1, #1
	mov	r2, r1
	mov	r3, r6
	add	r0, r5, r1
	bl	fread
	mov	r1, #1
	mov	r2, r1
	mov	r0, r5
	mov	r3, r6
	bl	fread
	ldr	r2, [sl, #4]
	add	r7, r7, #2
	cmp	r2, r7
	bhi	.L81
.L84:
	cmp	fp, #0
	bne	.L88
	ldr	r3, [sl, #0]
	add	r8, r8, #1
	cmp	r3, r8
	bls	.L85
.L89:
	ldr	r2, [sl, #4]
	b	.L80
.L88:
	mov	r0, r6
	mov	r1, fp
	mov	r2, #1
	bl	fseek
	ldr	r3, [sl, #0]
	add	r8, r8, #1
	cmp	r3, r8
	bhi	.L89
.L85:
	ldmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.L87:
	ldr	r0, .L90
	bl	puts
	mov	r0, #1
	bl	exit
.L91:
	.align	2
.L90:
	.word	.LC0
	.size	read_pixels_rgb, .-read_pixels_rgb
	.align	2
	.global	get_file_height
	.type	get_file_height, %function
get_file_height:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, lr}
	mov	r4, r1
	mov	r5, r0
	mov	r1, #22
	mov	r0, r4
	mov	r2, #0
	bl	fseek
	cmp	r0, #0
	bne	.L96
	mov	r0, r5
	mov	r3, r4
	mov	r1, #4
	mov	r2, #1
	bl	fread
	cmp	r0, #1
	bne	.L97
	ldmfd	sp!, {r4, r5, r6, lr}
	bx	lr
.L96:
	ldr	r0, .L98
	bl	puts
	mov	r0, #1
	bl	exit
.L97:
	ldr	r0, .L98+4
	bl	puts
	mov	r0, #1
	bl	exit
.L99:
	.align	2
.L98:
	.word	.LC1
	.word	.LC2
	.size	get_file_height, .-get_file_height
	.align	2
	.global	get_file_width
	.type	get_file_width, %function
get_file_width:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, lr}
	mov	r4, r1
	mov	r5, r0
	mov	r1, #18
	mov	r0, r4
	mov	r2, #0
	bl	fseek
	cmp	r0, #0
	bne	.L104
	add	r0, r5, #4
	mov	r3, r4
	mov	r1, #4
	mov	r2, #1
	bl	fread
	cmp	r0, #1
	bne	.L105
	ldmfd	sp!, {r4, r5, r6, lr}
	bx	lr
.L104:
	ldr	r0, .L106
	bl	puts
	mov	r0, #1
	bl	exit
.L105:
	ldr	r0, .L106+4
	bl	puts
	mov	r0, #1
	bl	exit
.L107:
	.align	2
.L106:
	.word	.LC3
	.word	.LC4
	.size	get_file_width, .-get_file_width
	.align	2
	.global	get_file_offset
	.type	get_file_offset, %function
get_file_offset:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, lr}
	mov	r4, r1
	mov	r5, r0
	mov	r1, #10
	mov	r0, r4
	mov	r2, #0
	bl	fseek
	cmp	r0, #0
	bne	.L112
	add	r0, r5, #8
	mov	r3, r4
	mov	r1, #4
	mov	r2, #1
	bl	fread
	cmp	r0, #1
	bne	.L113
	ldmfd	sp!, {r4, r5, r6, lr}
	bx	lr
.L112:
	ldr	r0, .L114
	bl	puts
	mov	r0, #1
	bl	exit
.L113:
	ldr	r0, .L114+4
	bl	puts
	mov	r0, #1
	bl	exit
.L115:
	.align	2
.L114:
	.word	.LC5
	.word	.LC6
	.size	get_file_offset, .-get_file_offset
	.align	2
	.global	get_file_size
	.type	get_file_size, %function
get_file_size:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, lr}
	mov	r4, r1
	mov	r5, r0
	mov	r1, #0
	mov	r2, #2
	mov	r0, r4
	bl	fseek
	mov	r0, r4
	bl	ftell
	str	r0, [r5, #16]
	ldmfd	sp!, {r4, r5, r6, lr}
	bx	lr
	.size	get_file_size, .-get_file_size
	.align	2
	.global	get_image_info
	.type	get_image_info, %function
get_image_info:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, lr}
	mov	r4, r1
	mov	r2, #2
	mov	r1, #0
	mov	r5, r0
	mov	r0, r4
	bl	fseek
	mov	r0, r4
	bl	ftell
	mov	r1, #10
	str	r0, [r5, #16]
	mov	r2, #0
	mov	r0, r4
	bl	fseek
	subs	r6, r0, #0
	bne	.L128
	add	r0, r5, #8
	mov	r1, #4
	mov	r2, #1
	mov	r3, r4
	bl	fread
	cmp	r0, #1
	mov	r7, r0
	bne	.L129
	mov	r2, r6
	mov	r0, r4
	mov	r1, #18
	bl	fseek
	subs	r8, r0, #0
	bne	.L130
	add	r0, r5, #4
	mov	r1, #4
	mov	r2, r7
	mov	r3, r4
	bl	fread
	cmp	r0, #1
	mov	r6, r0
	bne	.L131
	mov	r2, r8
	mov	r0, r4
	mov	r1, #22
	bl	fseek
	cmp	r0, #0
	bne	.L132
	mov	r3, r4
	mov	r0, r5
	mov	r1, #4
	mov	r2, r6
	bl	fread
	cmp	r0, #1
	bne	.L133
	ldr	r2, [r5, #4]
	mov	r3, r2, asl #1
	add	r3, r3, r2
	ands	r3, r3, #3
	rsbne	r3, r3, #4
	strne	r3, [r5, #12]
	streq	r3, [r5, #12]
	ldmfd	sp!, {r4, r5, r6, r7, r8, lr}
	bx	lr
.L128:
	ldr	r0, .L134
	bl	puts
	mov	r0, #1
	bl	exit
.L129:
	ldr	r0, .L134+4
	bl	puts
	mov	r0, #1
	bl	exit
.L130:
	ldr	r0, .L134+8
	bl	puts
	mov	r0, r7
	bl	exit
.L131:
	ldr	r0, .L134+12
	bl	puts
	mov	r0, r7
	bl	exit
.L132:
	ldr	r0, .L134+16
	bl	puts
	mov	r0, r6
	bl	exit
.L133:
	ldr	r0, .L134+20
	bl	puts
	mov	r0, r6
	bl	exit
.L135:
	.align	2
.L134:
	.word	.LC5
	.word	.LC6
	.word	.LC3
	.word	.LC4
	.word	.LC1
	.word	.LC2
	.size	get_image_info, .-get_image_info
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"Error: Seeking pixel start position failed\000"
	.space	1
.LC1:
	.ascii	"Error: Seeking height position failed\000"
	.space	2
.LC2:
	.ascii	"Error: Reading file height failed\000"
	.space	2
.LC3:
	.ascii	"Error: Seeking width position failed\000"
	.space	3
.LC4:
	.ascii	"Error: Reading file width failed\000"
	.space	3
.LC5:
	.ascii	"Error: Seeking offset position failed\000"
	.space	2
.LC6:
	.ascii	"Error: Reading file offset failed\000"
	.ident	"GCC: (Sourcery G++ Lite 2008q3-72) 4.3.2"
	.section	.note.GNU-stack,"",%progbits