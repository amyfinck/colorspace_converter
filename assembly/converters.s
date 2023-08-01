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
	.file	"converters.c"
	.text
	.align	2
	.global	downsample_chroma
	.type	downsample_chroma, %function
downsample_chroma:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
	cmp	r0, #0
	sub	sp, sp, #24
	mov	r9, r1
	str	r0, [sp, #4]
	mov	r1, r2
	beq	.L6
	add	r3, r9, #1
	mov	r2, r3, asl #1
	mov	fp, r1
	mov	r0, r9, asl #1
	add	r2, r2, r3
	add	r2, fp, r2
	add	r1, r0, r9
	mov	r3, r9, asl #3
	rsb	r3, r0, r3
	add	r1, fp, r1
	str	r2, [sp, #16]
	mov	r2, #0
	str	r3, [sp, #20]
	str	r1, [sp, #12]
	str	r2, [sp, #8]
.L3:
	cmp	r9, #0
	beq	.L5
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #16]
	add	r8, fp, #1
	add	r7, fp, #4
	add	r6, r3, #1
	add	r5, r2, #1
	mov	sl, #0
.L4:
	ldrb	r0, [r8, #1]	@ zero_extendqisi2
	ldrb	r1, [r8, #0]	@ zero_extendqisi2
	ldrb	r2, [r7, #1]	@ zero_extendqisi2
	ldrb	r3, [r7, #0]	@ zero_extendqisi2
	ldrb	ip, [r6, #1]	@ zero_extendqisi2
	ldrb	r4, [r6, #0]	@ zero_extendqisi2
	add	r2, r2, r0
	add	r3, r3, r1
	ldrb	r0, [r5, #1]	@ zero_extendqisi2
	ldrb	r1, [r5, #0]	@ zero_extendqisi2
	add	r2, r2, ip
	add	r3, r3, r4
	add	r2, r2, r0
	add	r3, r3, r1
	mov	r2, r2, asr #2
	mov	r3, r3, asr #2
	add	sl, sl, #2
	and	r2, r2, #255
	and	r3, r3, #255
	cmp	r9, sl
	strb	r3, [r8, #0]
	strb	r2, [r8, #1]
	strb	r3, [r7, #0]
	strb	r2, [r7, #1]
	strb	r3, [r6, #0]
	strb	r2, [r6, #1]
	strb	r3, [r5, #0]
	strb	r2, [r5, #1]
	add	r8, r8, #6
	add	r7, r7, #6
	add	r6, r6, #6
	add	r5, r5, #6
	bhi	.L4
.L5:
	ldmib	sp, {r2, r3}	@ phole ldm
	add	r3, r3, #2
	cmp	r2, r3
	str	r3, [sp, #8]
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #20]
	add	r2, r2, r3
	str	r2, [sp, #12]
	ldr	r2, [sp, #16]
	add	r2, r2, r3
	add	fp, fp, r3
	str	r2, [sp, #16]
	bhi	.L3
.L6:
	add	sp, sp, #24
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
	bx	lr
	.size	downsample_chroma, .-downsample_chroma
	.align	2
	.global	ycc_to_rgb
	.type	ycc_to_rgb, %function
ycc_to_rgb:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
	subs	sl, r0, #1
	beq	.L12
	mov	r3, r0, asl #1
	add	r3, r3, r0
	add	r8, r1, r3
	add	r7, r2, r3
.L11:
	ldrb	r4, [r7, #-3]	@ zero_extendqisi2
	ldrb	r5, [r7, #-2]	@ zero_extendqisi2
	ldrb	r6, [r7, #-1]	@ zero_extendqisi2
	mov	r1, r5
	mov	r2, r6
	mov	r0, r4
	bl	compute_rgb_b
	mov	r1, r5
	strb	r0, [r8, #-1]
	mov	r2, r6
	mov	r0, r4
	bl	compute_rgb_g
	mov	r1, r5
	strb	r0, [r8, #-2]
	mov	r2, r6
	mov	r0, r4
	bl	compute_rgb_r
	subs	sl, sl, #1
	strb	r0, [r8, #-3]!
	sub	r7, r7, #3
	bne	.L11
.L12:
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
	bx	lr
	.size	ycc_to_rgb, .-ycc_to_rgb
	.align	2
	.global	rgb_to_ycc
	.type	rgb_to_ycc, %function
rgb_to_ycc:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	mov	r9, r0
	sub	sp, sp, #20
	mov	r0, r2
	mov	r6, r2
	mov	r8, r1
	mov	r2, #0
	ldr	r1, [r9, #8]
	bl	fseek
	cmp	r0, #0
	bne	.L24
	ldr	r3, [r9, #0]
	cmp	r3, #0
	str	r3, [sp, #4]
	ldr	r3, [r9, #12]
	ldr	fp, [r9, #4]
	str	r3, [sp, #0]
	movne	sl, r0
	beq	.L22
.L17:
	cmp	fp, #0
	movne	r7, #0
	beq	.L21
.L18:
	ldr	r3, [r9, #4]
	mov	r1, #1
	mla	r5, r3, sl, r7
	mov	r2, r1
	mov	r3, r6
	add	r0, sp, #13
	bl	fread
	mov	r1, #1
	mov	r2, r1
	mov	r3, r6
	add	r0, sp, #14
	bl	fread
	mov	r1, #1
	mov	r3, r6
	mov	r2, r1
	add	r0, sp, #15
	bl	fread
	ldrb	r1, [sp, #14]	@ zero_extendqisi2
	ldrb	r2, [sp, #13]	@ zero_extendqisi2
	ldrb	r0, [sp, #15]	@ zero_extendqisi2
	bl	compute_ycc_y
	mov	r4, r5, asl #1
	add	r4, r4, r5
	strb	r0, [r8, r4]
	ldrb	r1, [sp, #14]	@ zero_extendqisi2
	ldrb	r2, [sp, #13]	@ zero_extendqisi2
	ldrb	r0, [sp, #15]	@ zero_extendqisi2
	bl	compute_ycc_cb
	add	r4, r8, r4
	strb	r0, [r4, #1]
	ldrb	r1, [sp, #14]	@ zero_extendqisi2
	ldrb	r2, [sp, #13]	@ zero_extendqisi2
	ldrb	r0, [sp, #15]	@ zero_extendqisi2
	bl	compute_ycc_cr
	mov	r1, #1
	strb	r0, [r4, #2]
	mov	r2, r1
	mov	r3, r6
	add	r0, sp, #13
	bl	fread
	mov	r1, #1
	mov	r2, r1
	mov	r3, r6
	add	r0, sp, #14
	bl	fread
	mov	r1, #1
	mov	r3, r6
	mov	r2, r1
	add	r0, sp, #15
	bl	fread
	ldrb	r1, [sp, #14]	@ zero_extendqisi2
	ldrb	r2, [sp, #13]	@ zero_extendqisi2
	ldrb	r0, [sp, #15]	@ zero_extendqisi2
	bl	compute_ycc_y
	add	r5, r5, #1
	mov	r4, r5, asl #1
	add	r4, r4, r5
	strb	r0, [r8, r4]
	ldrb	r1, [sp, #14]	@ zero_extendqisi2
	ldrb	r2, [sp, #13]	@ zero_extendqisi2
	ldrb	r0, [sp, #15]	@ zero_extendqisi2
	bl	compute_ycc_cb
	add	r4, r8, r4
	strb	r0, [r4, #1]
	ldrb	r1, [sp, #14]	@ zero_extendqisi2
	ldrb	r0, [sp, #15]	@ zero_extendqisi2
	ldrb	r2, [sp, #13]	@ zero_extendqisi2
	bl	compute_ycc_cr
	add	r7, r7, #2
	cmp	fp, r7
	strb	r0, [r4, #2]
	bhi	.L18
.L21:
	ldr	r3, [sp, #0]
	cmp	r3, #0
	bne	.L25
.L19:
	ldr	r3, [sp, #4]
	add	sl, sl, #1
	cmp	r3, sl
	bhi	.L17
.L22:
	add	sp, sp, #20
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.L25:
	mov	r0, r6
	mov	r1, r3
	mov	r2, #1
	bl	fseek
	b	.L19
.L24:
	ldr	r0, .L26
	bl	puts
	mov	r0, #1
	bl	exit
.L27:
	.align	2
.L26:
	.word	.LC0
	.size	rgb_to_ycc, .-rgb_to_ycc
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"Error: Seeking pixel start position failed\000"
	.ident	"GCC: (Sourcery G++ Lite 2008q3-72) 4.3.2"
	.section	.note.GNU-stack,"",%progbits