#include <sys/syscall.h>

.macro SYSCALL sysname, sysno
.global \sysname
\sysname:
	mov x8, \sysno
	svc #0
	ret
.endm

SYSCALL write, SYS_write
SYSCALL exit, SYS_exit
SYSCALL socket, SYS_socket
SYSCALL close, SYS_close
SYSCALL bind, SYS_bind
SYSCALL listen, SYS_listen
SYSCALL accept, SYS_accept
SYSCALL sendto, SYS_sendto
SYSCALL shutdown, SYS_shutdown
