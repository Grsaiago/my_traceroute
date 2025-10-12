#include "my_traceroute.h"

static void initialize_program_flags(ExecutionFlags *flags);

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
	// TODO: Inintialize the probe array in here

	return;
}
