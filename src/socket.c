#include "my_traceroute.h"

static int set_send_socket_flags(int sock, ExecutionFlags *flags);
static int set_recv_socket_flags(int sock, ExecutionFlags *flags);

UdpSocket *create_send_socket(ExecutionFlags *flags) {
	int        sockfd;
	UdpSocket *udpSock;

	sockfd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
	if (sockfd < 0) {
		dprintf(
		    STDERR_FILENO, "error creating UdpSocket socket: %s",
		    strerror(errno)
		);
		return (NULL);
	}
	if (set_send_socket_flags(sockfd, flags) != 0) {
		close(sockfd);
		return (NULL);
	}

	udpSock = malloc(sizeof(UdpSocket));
	if (udpSock == NULL) {
		dprintf(
		    STDERR_FILENO, "error allocating memory for UdpSocket: %s\n",
		    strerror(errno)
		);
		close(sockfd);
		return (NULL);
	}

	*udpSock = (UdpSocket){
	    .fd = sockfd,
	};
	return (udpSock);
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

IcmpSocket *create_recv_socket(ExecutionFlags *flags) {
	int         sockfd;
	IcmpSocket *icmpSock;

	sockfd =
	    socket(AF_INET, SOCK_RAW | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_ICMP);
	if (sockfd < 0) {
		dprintf(
		    STDERR_FILENO, "error creating IcmpSocket socket: %s",
		    strerror(errno)
		);
		return NULL;
	}
	if (set_recv_socket_flags(sockfd, flags) != 0) {
		close(sockfd);
		return NULL;
	}

	icmpSock = malloc(sizeof(UdpSocket));
	if (icmpSock == NULL) {
		dprintf(
		    STDERR_FILENO, "error allocating memory for IcmpSocket: %s\n",
		    strerror(errno)
		);
		close(sockfd);
		return (NULL);
	}

	*icmpSock = (IcmpSocket){
	    .fd = sockfd,
	};
	return (icmpSock);
}

static int set_recv_socket_flags(int sock, ExecutionFlags *flags) {
	(void)sock;
	(void)flags;
	return (0);
}
