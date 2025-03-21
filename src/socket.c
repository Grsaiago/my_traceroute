#include "../include/my_ping.h"

static int set_socket_flags(int sock, ExecutionFlags *flags);

int	new_raw_socket(Socket *res, struct sockaddr_storage *remote_addr, ExecutionFlags *flags) {
	int	sock;

	// we don't need a raw socket because:
	// https://sturmflut.github.io/linux/ubuntu/2015/01/17/unprivileged-icmp-sockets-on-linux/
	// We actually do, because without it the kernel sets the ttl of the iphdr to 0
	// to have it run without sudo, we need to run 'sudo setcap cap_net_raw+ep ./my_ping'
	if (flags->icmp) {
		sock = socket(AF_INET, SOCK_DGRAM, 0);
	} else {
		sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	}
	if (sock == -1) {
		dprintf(STDERR_FILENO, "error on socket creation: %s\n", strerror(errno));
		return (-1);
	}
	if (set_socket_flags(sock, flags) != 0) {
		return (-1);
	}
	res->fd = sock;
	res->remote_addr = *remote_addr;
	res->ipv4_addr = (struct sockaddr_in *)&res->remote_addr;
	res->addr_struct_size = sizeof(struct sockaddr_in);
	return (0);
}

static int set_socket_flags(int sock, ExecutionFlags *flags) {
	int	value;

	if (flags->so_debug == true) {
		value = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_DEBUG, &value, sizeof(int)) != 0) {
			dprintf(STDERR_FILENO, "error on setsockopt SO_DEBUG: %s\n", strerror(errno));
			return (-1);
		}
	}
	if (flags->max_ttl != DEFAULT_TTL) {
		value = flags->max_ttl;
		// TODO: Add an option for ipv6
		if (setsockopt(sock, IPPROTO_IP, IP_TTL, &value, sizeof(int)) != 0) {
			dprintf(STDERR_FILENO, "error on setsockopt IP_TTL: %s\n", strerror(errno));
			return (-1);
		}
	}
	return (0);
}
