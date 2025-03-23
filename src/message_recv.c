#include "../include/my_ping.h"

static int		recv_icmp_message(Socket *sock, IcmpReply *message);


int recv_message(Socket *sock, IcmpReply *message) {
	return (recv_icmp_message(sock, message));
}

static int	recv_icmp_message(Socket *sock, IcmpReply *message) {
	// as per man raw: "For receiving, the IP header is always included in the packet."
	char		readbuff[sizeof(struct iphdr) + sizeof(IcmpPayload)] = {0};
	int		err_value;

	err_value = recvfrom(sock->fd, readbuff, sizeof(readbuff),
		      0, NULL, NULL);
	if (err_value < (int)sock->addr_struct_size) {
		if (err_value == -1) {
			dprintf(STDERR_FILENO, "erro no recvfrom: %s\n", strerror(errno));
			return (1);
		} else {
			dprintf(STDERR_FILENO, "erro no recvfrom: de %ld bytes, somente %d foram lidos", sizeof(readbuff), err_value);
			return (1);
		}
	}
	message->iphdr = *(struct iphdr *)readbuff;
	message->icmp = *(struct icmp *)(readbuff + (message->iphdr.ihl * 4));
	return (0);
}
