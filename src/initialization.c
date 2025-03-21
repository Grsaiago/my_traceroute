#include "../include/my_ping.h"

void	initialize_program_conf(ProgramConf *conf) {
	memset(conf, 0, sizeof(ProgramConf));
	conf->continue_execution = true;
	conf->msg_seq = -1;
	/* initialize packet metrics */
	conf->pkt_stats.rtt_max = DBL_MIN;
	conf->pkt_stats.rtt_min = DBL_MAX;
	/* initialize flags */
	conf->flags.resolve_ip_name = true;
	conf->flags.max_ttl = DEFAULT_MAX_TTL;
	conf->flags.first_ttl= DEFAULT_TTL;
	conf->flags.icmp = false;
	conf->flags.so_debug = false;
}
