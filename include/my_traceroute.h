/** \file my_traceroute.h
    \brief The project's main header file

    It contains all includes, defines, struct declarations and function
   prototypes used by the project.
*/

#ifndef MY_TRACEROUTE_H_
#define MY_TRACEROUTE_H_

#include <argp.h>
#include <errno.h>
#include <error.h>
#include <limits.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/**
 *  @brief Anm enumeration representing the IP protocol version (IPv4 or IPv6)
 */
typedef enum e_IpProtocol { IPV4 = AF_INET, IPV6 = AF_INET6 } IpProtocol;

/**
 *  @brief A structure representing a network socket, be it ICMP ot UDP
 */
typedef struct s_Socket {
	/**
	 * @brief the socket file descriptior
	 */
	int                     fd;
	/**
	 * @brief the parsed address of the remote host, be it ipv4 or ipv6
	 */
	struct sockaddr_storage remote_addr;
	/**
	 * @brief a pointer to the internal sockstorage if it's ipv4
	 */
	struct sockaddr_in     *ipv4_addr;
	/**
	 * @brief a pointer to the internal sockstorage if it's ipv6
	 */
	struct sockaddr_in6    *ipv6_addr;
	/**
	 * @brief the size of the underlying sockaddr_storage in use
	 */
	socklen_t               addr_struct_size;
	/**
	 * @brief if it's an IPv4 or IPv6 socket
	 */
	IpProtocol              ip_protocol;
} Socket;

/**
 * @brief Default debug mode flag.
 *
 * Set to `0` to disable debug mode by default.
 */
#define DEFAULT_SO_DEBUG 0

/**
 * @brief Default destination UDP port.
 *
 * Standard starting port used for traceroute probes.
 */
#define DEFAULT_PORT 33434

/**
 * @brief Default initial Time-To-Live (TTL) value.
 *
 * The TTL used for the first probe sent.
 */
#define DEFAULT_FIRST_TTL 0

/**
 * @brief Default maximum Time-To-Live (TTL) value.
 *
 * The maximum TTL value before traceroute stops probing.
 */
#define DEFAULT_MAX_PROBE_TTL -1

/**
 * @brief Default probe response timeout in milliseconds.
 *
 * Time (in ms) to wait for an ICMP reply before marking the probe as lost.
 */
#define DEFAULT_PROBE_RESPONSE_TIMEOUT 10

/**
 * @brief Default interval between sending probes in milliseconds.
 *
 * Defines the delay between consecutive probe packets.
 */
#define DEFAULT_PROBE_SEND_INTERVAL 0

/**
 * @brief Default number of probes sent per hop.
 *
 * Determines how many probe packets are sent for each hop in the route.
 */
#define DEFAULT_PROBES_PER_HOP 3

/**
 * @brief Holds configuration flags for the traceroute execution.
 *
 * This structure stores all user-provided runtime parameters,
 * such as debugging mode, port selection, TTL configuration,
 * and probe timing options.
 */
typedef struct s_ExecutionFlags {
	/**
	 * @brief '-d`, `--debug`
	 *
	 * Enables the SO_DEBUG on the underlying socket
	 */
	bool     so_debug;
	/**
	 * @brief '-p`, `--port`
	 *
	 * Destination port used for probes.
	 */
	uint16_t port;
	/**
	 * @brief `-f`, `--first-ttl`
	 *
	 * Initial Time-To-Live value for the request sent by the probes.
	 */
	uint16_t first_ttl;
	/**
	 * @brief `-m`, `--max-ttl`
	 *
	 * Maximum Time-To-Live that the program will probe.
	 */
	int16_t  max_probe_ttl;
	/**
	 * @brief `-w`, `--wait`
	 *
	 * Timeout (in milliseconds) to wait for probe responses
	 */
	uint16_t probe_response_timeout;
	/**
	 * @brief `-z`, `--sendwait`
	 *
	 * Interval (in milliseconds) between sending probes
	 */
	uint16_t probe_send_interval; /**< -z --sendwait */
	/**
	 * @brief `-q`, `--queries`
	 *
	 * Number of probes sent per hop
	 */
	uint8_t  probes_per_hop;

	/**
	 * @brief `-P`, `--protocol`
	 *
	 * The IP protocol version to use (IPv4 or IPv6)
	 */
	IpProtocol ip_protocol;

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
