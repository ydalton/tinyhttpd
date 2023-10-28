.include "defs.S"

.section .text
	.extern strlen
	.global _start

_start:
	# write message
	write stdout, http_start
	# create socket
	socket AF_INET, SOCK_STREAM, 0

	# check for error
	cmp x0, 0
	b.lt _err_sock
	# store the return value into "sockfd"
	adr x1, sockfd
	str x0, [x1]
	write stderr, http_sock_ok

	# fill in the sockaddr_in struct
	adr x0, sockaddr
	# port 8000
	mov w1, 0x401f
	# some fancy bitshifting shenanigans
	lsl w1, w1, 16
	add w1, w1, AF_INET
	str w1, [x0]
	# 127.0.0.1
	mov w1, 0x7f
	movk w1, 0x0100, lsl 16
	str w1, [x0, 4]

	# bind
	mov x8, SYS_bind
	adr x3, sockfd
	ldr x0, [x3]
	adr x1, sockaddr
	mov x2, 16
	svc 0
	cmp x0, 0
	b.lt _err

	mov x8, SYS_listen
	adr x2, sockfd
	ldr x0, [x2]
	mov x1, 5
	cmp x0, 0
	b.lt _err

	mov x8, SYS_accept
	adr x3, sockfd
	ldr x0, [x3]
	adr x1, clisockaddr
	mov x2, 16
	svc 0
	cmp x0, 0
	b.lt _err

	adr x1, connfd
	str x0, [x1]

	# close the socket
	adr x0, sockfd
	ldr x1, [x0]
	close x1

	add sp, sp, 16
	ldp x29, x30, [sp], #16

	exit 0

_err_sock:
	write stderr, http_sock_err
	exit -1

_err:
	exit -1

.section .data
	http_start: 	.asciz "Starting tinyhttpd HTTP server...\n";
	http_sock_err: 	.asciz "Cannot create socket!\n";
	http_sock_ok: 	.asciz "Created socket...\n";

.section .bss
	sockfd:		.skip 8
	sockaddr:	.skip 16
	connfd:		.skip 4
	clisockaddr:	.skip 16
