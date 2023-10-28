.equ	SYS_close,	57
.equ	SYS_write,	64
.equ	SYS_exit,	93
.equ	SYS_socket,	198
.equ	SYS_bind,	200
.equ	SYS_listen,	201
.equ 	SYS_accept,	202

.equ 	AF_INET,	2
.equ 	SOCK_STREAM,	1

.equ 	stdin, 		0
.equ 	stdout, 	1	
.equ 	stderr, 	2

.macro write fd, buf
	adr x0, \buf
	bl strlen
	mov x2, x0
	
	mov x8, SYS_write
	mov x0, \fd
	adr x1, \buf
	svc #0
.endm

.macro close fd
	mov x8, SYS_close
	mov x0, \fd
	svc 0
.endm

.macro socket domain, type, protocol
	mov x8, SYS_socket
	mov x0, \domain
	mov x1, \type
	mov x2, \protocol
	svc 0
.endm

.macro bind sockfd, addr, len
	adr x3, \sockfd
	ldr x0, [x3]
	mov x3, 0

	mov x8, SYS_bind
	adr x1, \addr
	mov x2, \len
	svc 0
.endm

.macro listen sockfd, backlog
	adr x2, \sockfd
	mov x8, SYS_listen
	ldr x0, [x2]
	mov x1, \backlog
	svc 0
.endm

.macro exit status
	mov x8, SYS_exit
	mov x0, \status
	svc 0
.endm
