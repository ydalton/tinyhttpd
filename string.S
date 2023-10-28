.global strlen

strlen:
	sub sp, sp, 16
	mov x2, #0
.L1:
	add x0, x0, #1
	add x2, x2, #1
	# load byte from address pointed to by x0
	ldrb w1, [x0]
	# check if loaded byte is \0 and jump if not equal
	cbnz w1, .L1
	# move the result into x0 which is the return value
	mov x0, x2

	add sp, sp, 16
	ret
