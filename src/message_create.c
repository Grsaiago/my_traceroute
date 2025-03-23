#include "../include/my_ping.h"
#include <netinet/in.h>

static UdpPayload	new_udp_message(ProgramConf *conf);
static IcmpPayload	new_icmp_echo_message(ProgramConf *conf);
static void		set_iphdr(ProgramConf *conf, struct iphdr *iphdr);
static uint16_t		calculate_udp_checksum(struct udphdr *udphdr);
static uint16_t		calculate_icmp_checksum(void *b, int len);

TracerouteMessage new_message(ProgramConf *conf, TracerouteMessagePayloadType message_type) {
	TracerouteMessage	new_message = {0};

	set_iphdr(conf, &new_message.iphdr);
	new_message.message_type = message_type;
	switch (new_message.message_type) {
		case (TR_UDP_PAYLOAD):
			new_message.payload.udp = new_udp_message(conf);
			break;
		case (TR_ICMP_PAYLOAD):
			new_message.payload.icmp = new_icmp_echo_message(conf);
			break;
	}
	return new_message;
}

static UdpPayload	new_udp_message(ProgramConf *conf) {
	UdpPayload message;

	message.udp = (struct udphdr) {
		.len = sizeof(struct udphdr),
		.dest = conf->sock_pair.out_sock.remote_addr.sin_addr.s_addr,
	};
	message.udp.check = calculate_udp_checksum(&message.udp);
	return (message);
}

static IcmpPayload	new_icmp_echo_message(ProgramConf *conf) {
	IcmpPayload	message;

	message.icmp = (struct icmp) {
		.icmp_type = ICMP_ECHO,
		.icmp_code = 0,
		.icmp_hun.ih_idseq = {
			.icd_id = getpid(),
			.icd_seq = ++conf->msg_seq,
		}
	};
	message.icmp.icmp_cksum = calculate_icmp_checksum(&message, sizeof(message));
	return (message);
}

static uint16_t	calculate_udp_checksum(struct udphdr *udphdr) {
	// TODO: Tem que fazer essa porra em
	(void)udphdr;
	return (42);
}

static uint16_t	calculate_icmp_checksum(void *b, int len) {
	unsigned short	*buf = b;
	unsigned int	sum = 0;
	uint16_t	result;

	for (sum = 0; len > 1; len -= 2) {
		sum += *buf++;
	}
	if (len == 1) {
		sum += *(unsigned char *)buf;
	}
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

static void set_iphdr(ProgramConf *conf, struct iphdr *iphdr) {
	/* https://man7.org/linux/man-pages/man7/raw.7.html */
	(void)conf;
	*iphdr = (struct iphdr) {
		/* .check is always filled by the kernel */
		/* .tot_len is always filled by the kernel */
		.version = 4,           // IPv4
		.ihl = 5,               // Header length in 32-bit words (5 * 4 = 20 bytes)
		.tos = 0,               // Type of service (default)
		.tot_len = 0,           // Will be calculated or filled by the kernel
		.id = htons(54321),     // Identification field (use a unique value)
		.frag_off = 0,          // No fragmentation
		.ttl = 1,               // Time-to-live (set to 1 to trigger ICMP Time Exceeded)
		.check = 0,             // Will be calculated or filled by the kernel
		.saddr = 0,             // Source IP (kernel will fill this in)
		.daddr = conf->sock_pair.out_sock.remote_addr.sin_addr.s_addr
	};
	switch (conf->flags.out_socket_type) {
		case (ICMP_SOCK):
			iphdr->protocol = IPPROTO_ICMP;
			break;
		case (UDP_SOCK):
			iphdr->protocol = IPPROTO_UDP;
			break;
	}
	return ;
}
