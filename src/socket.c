#include "my_traceroute.h"

static int set_send_socket_flags(int sock, ExecutionFlags *flags);
static int set_recv_socket_flags(int sock, ExecutionFlags *flags);

int create_send_socket(ExecutionFlags *flags) {
	int sockfd;

	sockfd = socket(AF_INET | SOCK_NONBLOCK | SOCK_CLOEXEC, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		dprintf(STDERR_FILENO, "error creating socket: %s", strerror(errno));
		return (-1);
	}
	if (set_send_socket_flags(sockfd, flags) != 0) {
		close(sockfd);
		return (-1);
	}
	return (sockfd);
}

static int set_send_socket_flags(int sock, ExecutionFlags *flags) {
	int value;
	if (flags->so_debug == true) {
		value = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_DEBUG, &value, sizeof(int)) != 0) {
			dprintf(
			    STDERR_FILENO, "error on setsockopt SO_DEBUG: %s\n",
			    strerror(errno)
			);
			return (-1);
		}
	}
	return (0);
}

int create_recv_socket(ExecutionFlags *flags) {
	int sockfd;

	sockfd =
	    socket(AF_INET, SOCK_RAW | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_ICMP);
	if (sockfd < 0) {
		dprintf(STDERR_FILENO, "error creating socket: %s", strerror(errno));
		return (-1);
	}
	if (set_recv_socket_flags(sockfd, flags) != 0) {
		close(sockfd);
		return (-1);
	}
	return (0);
}

static int set_recv_socket_flags(int sock, ExecutionFlags *flags) {
	(void)sock;
	(void)flags;
	return (0);
}
