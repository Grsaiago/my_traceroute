#include "my_traceroute.h"

static void initialize_program_flags(ExecutionFlags *flags);

/**
 * @brief Initializes the program configuration with default values.
 *
 * This function prepares a @c ProgramConf structure for use by clearing its
 * memory and setting all execution parameters to their default values, as
 * defined by the following macros:
 * - @ref DEFAULT_SO_DEBUG
 * - @ref DEFAULT_PORT
 * - @ref DEFAULT_FIRST_TTL
 * - @ref DEFAULT_MAX_PROBE_TTL
 * - @ref DEFAULT_PROBE_RESPONSE_TIMEOUT
 * - @ref DEFAULT_PROBE_SEND_INTERVAL
 * - @ref DEFAULT_PROBES_PER_HOP
 *
 * @param conf Pointer to the @c ProgramConf structure to be initialized.
 */
void initialize_program_conf(ProgramConf *conf) {
	memset(conf, 0, sizeof(ProgramConf));
	initialize_program_flags(&conf->flags);
}

static void initialize_program_flags(ExecutionFlags *flags) {
	flags->so_debug = DEFAULT_SO_DEBUG;
	flags->port = DEFAULT_PORT;
	flags->first_ttl = DEFAULT_FIRST_TTL;
	flags->max_probe_ttl = DEFAULT_MAX_PROBE_TTL;
	flags->probe_response_timeout = DEFAULT_PROBE_RESPONSE_TIMEOUT;
	flags->probe_send_interval = DEFAULT_PROBE_SEND_INTERVAL;
	flags->probes_per_hop = DEFAULT_PROBES_PER_HOP;
	flags->ip_protocol = IPV4;
	// TODO: Inintialize the probe array in here

	return;
}
