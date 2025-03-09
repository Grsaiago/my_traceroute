#ifndef MY_TRACEROUTE
# define MY_TRACEROUTE

# include <stdint.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
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

typedef enum e_IpVersion {
	IPV4 = AF_INET,
	IPV6 = AF_INET6,
}	IpVersion;

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

/* socket */
typedef struct s_Socket {
	int			fd;
	struct sockaddr_storage	remote_addr;
	struct sockaddr_in	*ipv4_addr;
	struct sockaddr_in6	*ipv6_addr;
	socklen_t		addr_struct_size;
}	Socket;

/* ping program */
#define DEFAULT_TTL 200
typedef struct s_ExecutionFlags {
	bool		so_debug; // -d --debug
	uint32_t	first_ttl; // -f --first
	uint32_t	max_ttl; // -m --max-hops
	uint32_t	packet_interval; // -i --interval
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
	IpVersion	ip_version;
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
void	install_signal_handlers(void);
void	initialize_program_conf(ProgramConf *conf);
int	new_raw_socket(Socket *res, struct sockaddr_storage *remote_addr, ExecutionFlags *flags);
/* parse functions */
int		parse_arguments(ProgramConf *conf, int argc, char *argv[]);
// Performs dns lookup/ip validation for address using getaddrinfo and places found sockaddr_* in struct res
int	validate_or_resolve_address(ProgramConf *conf, struct sockaddr *res);
/* icmp messaging */
unsigned short	calculate_checksum(void *b, int len);
IcmpMessage	new_icmp_echo_message(ProgramConf *conf);
int		send_icmp_message(Socket *sock, IcmpMessage *message);
int		recv_icmp_message(Socket *sock, IcmpReply *message);
double		calculate_rtt_in_ms(const struct timeval *start, const struct timeval *end);
/* record messages */
void	record_new_response(ProgramConf *conf, IcmpReply *message);
/* printing */
void	print_header(ProgramConf *conf);
void	print_icmp_message(ProgramConf *conf, IcmpReply *message);
void	print_footer(ProgramConf *conf);
/* main event loop */
int		event_loop(ProgramConf *conf);

#endif // !MY_TRACEROUTE
