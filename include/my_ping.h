#ifndef MY_TRACEROUTE
# define MY_TRACEROUTE

# include <stdint.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <netinet/udp.h>
# include <string.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <netdb.h>
# include <unistd.h>
# include <argp.h>
# include <errno.h>
# include <sys/time.h>
# include <signal.h>
# include <float.h>
# include <math.h>

typedef struct s_IcmpReply {
	struct iphdr	iphdr;
	struct icmp	icmp;
	struct timeval	sent_at;
	struct timeval	recv_at;
}	IcmpReply;

// As specified by 'man ping'
typedef struct s_IcmpMessage {
	struct icmp	icmp;
	struct timeval	timestamp;
}	IcmpMessage;

typedef enum e_TracerouteMessagePayloadType {
	TR_ICMP_PAYLOAD,
	TR_UDP_PAYLOAD,
}	TracerouteMessagePayloadType;

/*this timestamp will be used included in the packet's return
 * as per rfc 792 (since it's within the 8 bytes limit)*/
typedef struct s_IcmpPayload {
	struct icmp	icmp;
	struct timeval	timestamp;
}	IcmpPayload;

/*this timestamp will be used included in the packet's return
 * as per rfc 792 (since it's within the 8 bytes limit)*/
typedef struct s_UdpPayload {
	struct udphdr	udp;
	struct timeval	timestamp;
}	UdpPayload;

typedef union s_TracerouteMessagePayload {
	IcmpPayload	icmp;
	UdpPayload	udp;
}	TracerouteMessagePayload;

typedef struct s_TracerouteMessage {
	struct iphdr			iphdr;
	TracerouteMessagePayloadType	message_type;
	TracerouteMessagePayload	payload;
}	TracerouteMessage;

/* socket */
typedef enum e_SocketType {
	ICMP_SOCK,
	UDP_SOCK,
}	SocketType;

typedef struct s_Socket {
	int			fd;
	SocketType		type;
	struct sockaddr_in	remote_addr;
	socklen_t		addr_struct_size;
}	Socket;

typedef struct s_SocketPair {
	Socket	in_sock;
	Socket	out_sock;
}	SocketPair;

/* traceroute program */
#define DEFAULT_MAX_TTL 30
#define TR_STARTING_TTL 1
typedef struct s_ExecutionFlags {
	bool		resolve_ip_name; // -n --no-ip-to-host-resolve
	uint32_t	max_ttl; // -m --max-hops
	uint32_t	first_ttl; // -f --first
	SocketType	out_socket_type; // -I --icmp
	bool		so_debug; // -d --debug
}	ExecutionFlags;

typedef struct s_PingPacketStats {
	uint32_t	oks;
	uint32_t	errors;
	float		rtt_min;
	float		rtt_max;
	double		rtt_sum;
	double		rtt_sum_square;
}	PingPacketStats;

typedef struct s_ProgramConf {
	uint64_t	msg_seq;
	Socket		main_socket;
	ExecutionFlags	flags;
	PingPacketStats	pkt_stats;
	char		*program_arg;
	char		resolved_addr[INET6_ADDRSTRLEN];
	bool		continue_execution;
}	ProgramConf;

typedef struct s_PrintMetrcis {
	double		avg;
	double		stddev;
	uint32_t	loss_percent;
}	PrintMetrics;


/* functions */
bool	check_user_permission(ProgramConf *conf);
void	install_signal_handlers(void);
void	initialize_program_conf(ProgramConf *conf);
int	new_socket(Socket *res, struct sockaddr_in *remote_addr, ExecutionFlags *flags);
/* parse functions */
int		parse_arguments(ProgramConf *conf, int argc, char *argv[]);
// Performs dns lookup/ip validation for address using getaddrinfo and places found sockaddr_* in struct res
int	validate_or_resolve_address(ProgramConf *conf, struct sockaddr *res);
/* messaging */
TracerouteMessage	new_message(ProgramConf *conf, TracerouteMessagePayloadType message_type);
int			send_message(ProgramConf *conf, TracerouteMessage *message);
int			recv_message(Socket *sock, IcmpReply *message);
double			calculate_rtt_in_ms(const struct timeval *start, const struct timeval *end);
/* record messages */
void	record_new_response(ProgramConf *conf, IcmpReply *message);
/* printing */
void	print_header(ProgramConf *conf);
void	print_icmp_message(ProgramConf *conf, IcmpReply *message);
void	print_footer(ProgramConf *conf);
/* main event loop */
int		event_loop(ProgramConf *conf);

#endif // !MY_TRACEROUTE
