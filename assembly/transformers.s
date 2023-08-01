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
	.file	"transformers.c"
	.text
	.align	2
	.global	compute_ycc_y
	.type	compute_ycc_y, %function
compute_ycc_y:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	ip, r0, asl #6
	mov	r0, r0, asl #1
	add	r0, r0, ip
	mov	r3, r2, asl #5
	add	r1, r1, r1, asl #7
	sub	r3, r3, r2, asl #3
	add	r0, r0, #4224
	add	r0, r0, r1
	add	r3, r3, r2
	add	r0, r0, r3
	mov	r0, r0, lsr #8
	and	r0, r0, #255
	bx	lr
	.size	compute_ycc_y, .-compute_ycc_y
	.align	2
	.global	compute_ycc_cb
	.type	compute_ycc_cb, %function
compute_ycc_cb:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	ip, r0, asl #4
	mov	r3, r0, asl #2
	add	r3, r3, ip
	rsb	r0, r3, r0
	mov	ip, r1, asl #2
	mov	r3, r1, asl #5
	add	ip, ip, r3
	mov	r3, #32768
	add	r3, r3, #128
	mov	r0, r0, asl #1
	add	r0, r0, r3
	add	ip, ip, r1
	mov	r3, r2, asl #7
	sub	r0, r0, ip, asl #1
	sub	r3, r3, r2, asl #4
	add	r0, r0, r3
	mov	r0, r0, asr #8
	cmp	r0, #255
	movge	r0, #255
	and	r0, r0, #255
	bx	lr
	.size	compute_ycc_cb, .-compute_ycc_cb
	.align	2
	.global	compute_ycc_cr
	.type	compute_ycc_cr, %function
compute_ycc_cr:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r1, asl #6
	sub	r3, r3, r1, asl #4
	mov	ip, r0
	rsb	r1, r3, r1
	mov	r0, r0, asl #7
	mov	r3, #32768
	sub	r0, r0, ip, asl #4
	add	r3, r3, #128
	add	r0, r0, r3
	mov	r1, r1, asl #1
	mov	r3, r2, asl #4
	mov	r2, r2, asl #1
	add	r0, r0, r1
	add	r2, r2, r3
	rsb	r0, r2, r0
	mov	r0, r0, lsr #8
	and	r0, r0, #255
	bx	lr
	.size	compute_ycc_cr, .-compute_ycc_cr
	.align	2
	.global	compute_rgb_r
	.type	compute_rgb_r, %function
compute_rgb_r:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0, asl #2
	mov	r1, r0, asl #5
	mov	ip, r2, asl #3
	add	r3, r3, r1
	sub	ip, ip, r2, asl #1
	add	r3, r3, r0
	mov	r3, r3, asl #1
	mov	r2, ip, asl #4
	add	ip, ip, r2
	sub	r3, r3, #14208
	add	r3, r3, ip
	movs	r3, r3, asr #6
	movmi	r0, #0
	bxmi	lr
	cmp	r3, #255
	movge	r3, #255
	and	r0, r3, #255
	bx	lr
	.size	compute_rgb_r, .-compute_rgb_r
	.align	2
	.global	compute_rgb_g
	.type	compute_rgb_g, %function
compute_rgb_g:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	ip, r0, asl #5
	mov	r3, r0, asl #2
	add	r3, r3, ip
	add	r3, r3, r0
	mov	ip, r1, asl #5
	mov	r3, r3, asl #1
	sub	ip, ip, r1, asl #3
	mov	r0, r2, asl #4
	add	ip, ip, r1
	add	r3, r3, #8704
	sub	r0, r0, r2, asl #2
	add	r0, r0, r2
	rsb	r3, ip, r3
	sub	r3, r3, r0, asl #2
	movs	r3, r3, asr #6
	movmi	r0, #0
	bxmi	lr
	cmp	r3, #255
	movge	r3, #255
	and	r0, r3, #255
	bx	lr
	.size	compute_rgb_g, .-compute_rgb_g
	.align	2
	.global	compute_rgb_b
	.type	compute_rgb_b, %function
compute_rgb_b:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r3, r0, asl #2
	mov	r2, r0, asl #5
	add	r3, r3, r2
	add	r3, r3, r0
	mov	r3, r3, asl #1
	sub	r3, r3, #17664
	add	r1, r1, r1, asl #7
	add	r3, r3, r1
	movs	r3, r3, asr #6
	movmi	r0, #0
	bxmi	lr
	cmp	r3, #255
	movge	r3, #255
	and	r0, r3, #255
	bx	lr
	.size	compute_rgb_b, .-compute_rgb_b
	.ident	"GCC: (Sourcery G++ Lite 2008q3-72) 4.3.2"
	.section	.note.GNU-stack,"",%progbits