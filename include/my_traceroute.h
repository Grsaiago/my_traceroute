/** \file my_traceroute.h
    \brief A Documented file.

    Details.
*/

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

/**
 * @struct Socket struct documentation
 */
typedef struct s_Socket {
	int                     fd;
	struct sockaddr_storage remote_addr;
	struct sockaddr_in     *ipv4_addr;
	struct sockaddr_in6    *ipv6_addr;
	socklen_t               addr_struct_size;
} Socket;

/**
 * @def DEFAULT_SO_DEBUG
 * @brief Default debug mode flag.
 *
 * Set to `0` to disable debug mode by default.
 */
#define DEFAULT_SO_DEBUG 0

/**
 * @def DEFAULT_PORT
 * @brief Default destination UDP port.
 *
 * Standard starting port used for traceroute probes.
 */
#define DEFAULT_PORT 33434

/**
 * @def DEFAULT_FIRST_TTL
 * @brief Default initial Time-To-Live (TTL) value.
 *
 * The TTL used for the first probe sent.
 */
#define DEFAULT_FIRST_TTL 0

/**
 * @def DEFAULT_MAX_PROBE_TTL
 * @brief Default maximum Time-To-Live (TTL) value.
 *
 * The maximum TTL value before traceroute stops probing.
 */
#define DEFAULT_MAX_PROBE_TTL -1

/**
 * @def DEFAULT_PROBE_RESPONSE_TIMEOUT
 * @brief Default probe response timeout in milliseconds.
 *
 * Time (in ms) to wait for an ICMP reply before marking the probe as lost.
 */
#define DEFAULT_PROBE_RESPONSE_TIMEOUT 10

/**
 * @def DEFAULT_PROBE_SEND_INTERVAL
 * @brief Default interval between sending probes in milliseconds.
 *
 * Defines the delay between consecutive probe packets.
 */
#define DEFAULT_PROBE_SEND_INTERVAL 0

/**
 * @def DEFAULT_PROBES_PER_HOP
 * @brief Default number of probes sent per hop.
 *
 * Determines how many probe packets are sent for each hop in the route.
 */
#define DEFAULT_PROBES_PER_HOP 3

/**
 * @struct ExecutionFlags
 * @brief Holds configuration flags for the traceroute execution.
 *
 * This structure stores all user-provided runtime parameters,
 * such as debugging mode, port selection, TTL configuration,
 * and probe timing options.
 *
 * @typedef ExecutionFlags
 *
 * @var ExecutionFlags::so_debug
 *    Enables debug mode (`-d`, `--debug`).
 * @var ExecutionFlags::port
 *    Destination port used for probes (`-p`, `--port`).
 * @var ExecutionFlags::first_ttl
 *    Initial Time-To-Live value (`-f`, `--first-ttl`).
 * @var ExecutionFlags::max_probe_ttl
 *    Maximum Time-To-Live value (`-m`, `--max-ttl`).
 * @var ExecutionFlags::probe_response_timeout
 *    Timeout (in milliseconds) to wait for probe responses (`-w`, `--wait`).
 * @var ExecutionFlags::probe_send_interval
 *    Interval (in milliseconds) between sending probes (`-z`, `--sendwait`).
 * @var ExecutionFlags::probes_per_hop
 *    Number of probes sent per hop (`-q`, `--queries`).
 */
typedef struct s_ExecutionFlags {
	bool     so_debug;               /** -d --debug */
	uint16_t port;                   /** -p --port */
	uint16_t first_ttl;              /** -f --first-ttl */
	int16_t  max_probe_ttl;          /** -m --max-ttl */
	uint16_t probe_response_timeout; /** -w --wait */
	uint16_t probe_send_interval;    /** -z --sendwait */
	uint8_t  probes_per_hop;         /** -q --queries */
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
