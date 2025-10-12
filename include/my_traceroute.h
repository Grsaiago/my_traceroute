#ifndef MY_TRACEROUTE_H_
#define MY_TRACEROUTE_H_

#include <argp.h>
#include <error.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_Socket {
	int                     fd;
	struct sockaddr_storage remote_addr;
	struct sockaddr_in     *ipv4_addr;
	struct sockaddr_in6    *ipv6_addr;
	socklen_t               addr_struct_size;
} Socket;

#define DEFAULT_SO_DEBUG 0;
#define DEFAULT_PORT 33434;
#define DEFAULT_FIRST_TTL 0;
#define DEFAULT_MAX_PROBE_TTL -1;
#define DEFAULT_PROBE_RESPONSE_TIMEOUT 10
#define DEFAULT_PROBE_SEND_INTERVAL 0;
#define DEFAULT_PROBES_PER_HOP 3;

typedef struct s_ExecutionFlags {
	bool     so_debug;               // -d --debug
	uint16_t port;                   // -p --port
	uint16_t first_ttl;              // -f --first-ttl
	int16_t  max_probe_ttl;          // -m --max-ttl
	uint16_t probe_response_timeout; // -w --wait
	uint16_t probe_send_interval;    // -z --sendwait
	uint8_t  probes_per_hop;         // -q --queries
} ExecutionFlags;

// typedef struct s_Probe {
// 	// TODO: Define the fields
// } Probe;

typedef struct s_ProgramConf {
	ExecutionFlags flags;
	char          *program_arg;
	uint64_t       icmp_msg_seq;
	// TODO: Probe array in here
	Socket         send_socket;
	Socket         recv_socket;
	char           resolved_server_addr[INET6_ADDRSTRLEN];
} ProgramConf;

// initialize functions
void initialize_program_conf(ProgramConf *conf);

// parse functions
int parse_arguments(ProgramConf *conf, int argc, char *argv[]);

// debug functions
void debug_execution_flags(ExecutionFlags *flags);

#endif // MY_TRACEROUTE_H_
