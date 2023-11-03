#include <stddef.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/mman.h>

#include "defs.h"

unsigned int fputs(int fd, const char *s)
{
	unsigned int ret, len;

	len = strlen(s);
	ret = write(fd, s, len);

	return ret;
}

unsigned int puts(const char *s)
{
	return fputs(stdout, s);
}

unsigned int eputs(const char *s)
{
	return fputs(stderr, s);
}

int handle_connection(int sockfd)
{
	struct sockaddr_in cliaddr;
	socklen_t addrlen, cliaddrlen;
	int clientfd, ret;
	char *mem;
	size_t recvlen;
	const char *response =
		"HTTP/1.0 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"\n"
		"<!doctype html>"
		"<html>"
		"<head>"
		"<title>Test</title>"
		"</head>"
		"<body>"
		"<p>Hi</p>"
		"</body>"
		"</html>\r\n";

	clientfd = accept(sockfd, (struct sockaddr *) &cliaddr, &addrlen);
	if(clientfd == -1) {
		eputs("Failed to accept on socket!\n");
		return -1;
	}
	puts("Connection established!\n");

	mem = mmap(NULL, 4096, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	recvlen = recvfrom(clientfd, mem, 4096, 0, (struct sockaddr *) &cliaddr, &cliaddrlen);
	if(recvlen == -1)
		return -1;

	write(stdout, mem, ret);

	ret = sendto(clientfd, response, strlen(response), 0,
			(struct sockaddr *) &cliaddr, sizeof(cliaddr));
	if((unsigned int) ret != strlen(response))
		eputs("Warning: didn't send all bytes");
	munmap(mem, 4096);
	shutdown(clientfd, SHUT_RDWR);
	close(clientfd);
	return 0;
}
	
void _start(void)
{
	struct sockaddr_in sockaddr;
	int sockfd;
	int ret;

	puts("Starting tinyhttpd HTTP server...\n");
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1) {
		fputs(stderr, "Failed to create socket!\n");
		goto err;
	}
	
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = 0x401f;
	sockaddr.sin_addr.s_addr = 0x0100007f;

	ret = bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(struct sockaddr));
	if(ret < 0) {
		eputs("Failed to bind to socket!\n");
		goto err;
	}

	ret = listen(sockfd, 1);
	if(ret == -1) {
		eputs("Failed to listen on socket!\n");
		goto err;
	}

	while(1) {
		ret = handle_connection(sockfd);
		if(ret != 0) {
			eputs("Failed to handle connection!\n");
			goto err;
		}
	}
	close(sockfd);
	exit(0);
err:
	exit(-1);
}
