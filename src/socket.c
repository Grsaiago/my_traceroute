#include "../include/my_ping.h"
#include <netinet/in.h>
#include <sys/socket.h>

static int	new_in_socket(struct sockaddr_in *local_addr, Socket *sock, ExecutionFlags *flags);
static int	new_out_socket(struct sockaddr_in *remote_addr, Socket *sock, ExecutionFlags *flags);
static int	set_in_socket_flags(int sock, ExecutionFlags *flags);
static int	set_out_socket_flags(int sock, ExecutionFlags *flags);

int	new_socket_pair(struct sockaddr_in *remote_addr, struct sockaddr_in *local_addr, SocketPair *sock_pair, ExecutionFlags *flags) {


	if (new_out_socket(remote_addr, &sock_pair->out_sock, flags) != 0) {
		dprintf(STDERR_FILENO, "error on out_sock creation: %s\n", strerror(errno));
		return (-1);
	}
	if (new_in_socket(local_addr, &sock_pair->in_sock, flags) != 0) {
		dprintf(STDERR_FILENO, "error on in_sock creation: %s\n", strerror(errno));
		return (-1);
	}
	return (0);
}

static int	new_out_socket(struct sockaddr_in *remote_addr, Socket *sock, ExecutionFlags *flags) {
	int	new_sock;

	new_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (new_sock == -1) {
		dprintf(STDERR_FILENO, "socket() failed for the out_socket: %s\n", strerror(errno));
		return (-1);
	}
	if (set_out_socket_flags(new_sock, flags) != 0) {
		return (-1);
	}
	sock->fd = new_sock;
	sock->type = flags->out_socket_type;
	sock->remote_addr = *remote_addr;
	sock->addr_struct_size = sizeof(struct sockaddr_in);
	return (0);
}

static int	new_in_socket(struct sockaddr_in *local_addr, Socket *sock, ExecutionFlags *flags) {
	int	new_sock;

	new_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (new_sock == -1) {
		dprintf(STDERR_FILENO, "socket() failed for the in_socket: %s\n", strerror(errno));
		return (-1);
	}
	if (bind(new_sock, (struct sockaddr *)local_addr, sizeof(struct sockaddr_in)) != 0) {
		dprintf(STDERR_FILENO, "bind() failed for the in_socket: %s\n", strerror(errno));
		return (-1);
	}
	if (set_in_socket_flags(new_sock, flags) != 0) {
		return (-1);
	}
	sock->fd = new_sock;
	sock->type = ICMP_SOCK;
	sock->remote_addr = *local_addr;
	sock->addr_struct_size = sizeof(struct sockaddr_in);
	return (0);
}

static int	set_in_socket_flags(int sock, ExecutionFlags *flags) {
	int value;

	if (flags->so_debug) {
		value = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_DEBUG, &value, sizeof(int)) != 0) {
			dprintf(STDERR_FILENO, "error on in_sock setsockopt SO_DEBUG: %s\n", strerror(errno));
			return (-1);
		}
	}
	return (0);
}

static int	set_out_socket_flags(int sock, ExecutionFlags *flags) {
	int value;

	if (flags->so_debug) {
		value = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_DEBUG, &value, sizeof(int)) != 0) {
			dprintf(STDERR_FILENO, "error on out_sock setsockopt SO_DEBUG: %s\n", strerror(errno));
			return (-1);
		}
	}
	return (0);
}
