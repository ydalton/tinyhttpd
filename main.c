#include <arpa/inet.h>
#include <sys/socket.h>

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
	
void _start(void)
{
	struct sockaddr_in sockaddr;
	struct sockaddr_in cliaddr;
	socklen_t addrlen;
	int sockfd, clientfd;
	int ret;

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
		clientfd = accept(sockfd, (struct sockaddr *) &cliaddr, &addrlen);
		if(clientfd == -1) {
			eputs("Failed to accept on socket!\n");
			goto err;
		}
		puts("Connection established!\n");
		ret = sendto(clientfd, response, strlen(response), 0,
				(struct sockaddr *) &cliaddr, sizeof(cliaddr));
		if((unsigned int) ret != strlen(response))
			eputs("Warning: didn't send all bytes");
		shutdown(clientfd, SHUT_RDWR);
		close(clientfd);
	}
	close(sockfd);
	exit(0);
err:
	exit(-1);
}
