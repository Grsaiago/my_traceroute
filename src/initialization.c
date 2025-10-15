#include "my_traceroute.h"
#include <string.h>

/**
 * @brief Initializes an ExecutionFlags struct with default values.
 *
 * This function prepares a @c ExecutionFlags structure for use by clearing its
 * memory and setting all execution flags to their default values, as
 * defined by the following macros:
 * - @ref DEFAULT_SO_DEBUG
 * - @ref DEFAULT_PORT
 * - @ref DEFAULT_FIRST_TTL
 * - @ref DEFAULT_MAX_PROBE_TTL
 * - @ref DEFAULT_PROBE_RESPONSE_TIMEOUT
 * - @ref DEFAULT_PROBE_SEND_INTERVAL
 * - @ref DEFAULT_PROBES_PER_HOP
 *
 * @param conf Pointer to the @c ExecutionFlags structure to be initialized.
 */
void initialize_execution_flags(ExecutionFlags *flags) {
	memset(flags, 0, sizeof(ExecutionFlags));

	flags->so_debug = DEFAULT_SO_DEBUG;
	flags->port = DEFAULT_PORT;
	flags->first_ttl = DEFAULT_FIRST_TTL;
	flags->max_probe_ttl = DEFAULT_MAX_PROBE_TTL;
	flags->probe_response_timeout = DEFAULT_PROBE_RESPONSE_TIMEOUT;
	flags->probe_send_interval = DEFAULT_PROBE_SEND_INTERVAL;
	flags->probes_per_hop = DEFAULT_PROBES_PER_HOP;
}

/**
 * @brief Initializes the ProgramConf structure.
 *
 * This function sets up the @c ProgramConf structure by initializing its
 * execution flags, creating the necessary sockets for sending and receiving
 * packets, and resolving the target host address. It ensures that all
 * components are properly configured for the traceroute operation.
 *
 * @param conf Pointer to the @c ProgramConf structure to be initialized.
 * @return Returns 0 on success, or -1 on failure.
 */
int initialize_program_conf(ProgramConf *conf) {
	conf->probes = malloc(sizeof(Probe) * conf->flags.probes_per_hop);
	if (conf->probes == NULL) {
		dprintf(
		    STDERR_FILENO, "error allocating memory for probes: %s\n",
		    strerror(errno)
		);
		return (-1);
	}
	memset(conf->probes, 0, sizeof(Probe) * conf->flags.probes_per_hop);

	conf->send_socket = create_send_socket(&conf->flags);
	if (conf->send_socket == NULL) {
		delete_program_conf(conf);
		return (-1);
	}

	conf->recv_socket = create_recv_socket(&conf->flags);
	if (conf->recv_socket == NULL) {
		delete_program_conf(conf);
		return (-1);
	}
	return (0);
}
