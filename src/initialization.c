#include "../include/my_traceroute.h"

void initialize_program_conf(ProgramConf *conf) {
	memset(conf, 0, sizeof(ProgramConf));
	conf->continue_execution = true;
	conf->flags.so_debug = DEFAULT_SO_DEBUG;
	conf->flags.port = DEFAULT_PORT;
	conf->flags.probe_timeout = DEFAULT_PROBE_TIMEOUT;
	conf->flags.first_ttl = DEFAULT_FIRST_TTL;
	conf->flags.max_probe_ttl = DEFAULT_MAX_PROBE_TTL;
	conf->flags.max_probe_wait = DEFAULT_MAX_PROBE_WAIT;
	conf->flags.probe_send_interval = DEFAULT_PROBE_SEND_INTERVAL;
	conf->flags.probes_per_hop = DEFAULT_PROBES_PER_HOP;
	// TODO: Inintialize the probe array in here
}
