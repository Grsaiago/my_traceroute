#include "../include/my_ping.h"

static int	send_udp_message(Socket *sock, UdpPayload *message, struct iphdr *iphdr);
static int	send_icmp_message(Socket *sock, IcmpPayload *message, struct iphdr *iphdr);

int send_message(ProgramConf *conf, TracerouteMessage *message) {
	switch (message->message_type) {
		case (TR_UDP_PAYLOAD):
			if (send_udp_message(&conf->sock_pair.out_sock,
				&message->payload.udp,
				&message->iphdr) == -1) {
				return (-1);
			}
			break;
		case (TR_ICMP_PAYLOAD):
			if (send_icmp_message(&conf->sock_pair.out_sock,
				&message->payload.icmp,
				&message->iphdr) == -1) {
				return (-1);
			}
			break;
	}
	return (0);
}

static int	send_udp_message(Socket *sock, UdpPayload *message, struct iphdr *iphdr) {
	int		err_value;
	char		sendbuff[sizeof(struct iphdr) + sizeof(UdpPayload)] = {0};

	memmove(sendbuff, iphdr, sizeof(struct iphdr));
	memmove(sendbuff + sizeof(struct iphdr), message, sizeof(UdpPayload));
	err_value = sendto(sock->fd, (void *)sendbuff, sizeof(sendbuff), 0,
		    (const struct sockaddr *)&sock->remote_addr,
		    sock->addr_struct_size);
	if (err_value < (int)sizeof(sendbuff)) {
		if (err_value == -1) {
			dprintf(STDERR_FILENO, "error on sendto: %s\n", strerror(errno));
		} else {
			dprintf(STDERR_FILENO, "error on sendto: out of %ld bytes, only %d were sent", sizeof(sendbuff), err_value);
		}
		return (1);
	}
	return (0);
}

static int	send_icmp_message(Socket *sock, IcmpPayload *message, struct iphdr *iphdr) {
	int		err_value;
	char		sendbuff[sizeof(struct iphdr) + sizeof(IcmpPayload)] = {0};

	memmove(sendbuff, iphdr, sizeof(struct iphdr));
	memmove(sendbuff + sizeof(struct iphdr), message, sizeof(IcmpPayload));
	err_value = sendto(sock->fd, (void *)sendbuff, sizeof(sendbuff), 0,
		    (const struct sockaddr *)&sock->remote_addr,
		    sock->addr_struct_size);
	if (err_value < (int)sizeof(sendbuff)) {
		if (err_value == -1) {
			dprintf(STDERR_FILENO, "error on sendto: %s\n", strerror(errno));
		} else {
			dprintf(STDERR_FILENO, "error on sendto: out of %ld bytes, only %d were sent", sizeof(sendbuff), err_value);
		}
		return (1);
	}
	return (0);
}
