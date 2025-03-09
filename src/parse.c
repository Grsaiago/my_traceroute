#include "../include/my_ping.h"

static error_t	parser_func(int key, char *arg, struct argp_state *state);
static int32_t	set_first(ProgramConf *conf, char *str_nbr);
static int32_t	set_max_hops(ProgramConf *conf, char *str_nbr);
static int32_t	set_interval(ProgramConf *conf, char *str_nbr);

// argp global variables as per: https://www.gnu.org/software/libc/manual/html_node/Argp-Global-Variables.html
const char			*argp_program_version = "1.0";

int	parse_arguments(ProgramConf *conf, int argc, char *argv[]) {
	static struct argp_option	options[] = {
		{ .name = "debug", .key = 'd', .arg = NULL, .doc = "Set the SO_DEBUG option." },
		{ .name = "first", .key = 'f', .arg = NULL, .doc = "Start from the first_ttl hop (instead from 1)." },
		{ .name = "max-hops", .key = 'm', .arg = "max_ttl", .doc = "specify max_ttl as the maximum possible ttl." },
		{ .name = "interval", .key = 'i', .arg = "first_ttl", .doc = "Minimal time interval between probes (default 0)." },
		{0}
	};
	bool		so_debug; // -d --debug
	uint32_t	linger; // -W --linger
	uint32_t	first_ttl; // -f --first
	uint32_t	max_ttl; // -m --max-hops
	uint32_t	packet_interval; // -i --interval

	const struct argp argp = {
		.options = options,
		.parser = parser_func,
		.args_doc = "[HOST]",
		.doc = "Send ICMP ECHO_REQUEST packets to network hosts.",
	};
	argp_parse(&argp, argc, argv, 0, 0, (void *)conf);
	return (0);
}

static error_t	parser_func(int key, char *arg, struct argp_state *state) {
	(void)arg;
	ProgramConf	*conf;

	conf = (ProgramConf *)state->input;
	switch (key) {
		case ('d'):
			conf->flags.so_debug= true;
			break ;
		case ('f'):
			if (arg == NULL || set_first(conf, arg) == -1) {
				argp_error(state, "First cannot be less than 1\n");
			}
			break ;
		case ('m'):
			if (arg == NULL || set_max_hops(conf, arg) == -1) {
				argp_error(state, "Max hops cannot be less than 1\n");
			}
			break ;
		case ('i'):
			if (arg == NULL || set_interval(conf, arg) == -1) {
				argp_error(state, "Interval cannot be less than 1\n");
			}
			break ;
		case (ARGP_KEY_ARG):
			if (state->arg_num >= 1) {
				argp_usage(state);
			}
			conf->program_arg = arg;
			break;
		case (ARGP_KEY_END):
			if (state->arg_num < 1) {
				argp_usage(state);
			}
			break;
		default:
			return (ARGP_ERR_UNKNOWN);
	}
	return (0);
}

static int32_t	set_interval(ProgramConf *conf, char *str_nbr) {
	int32_t	result_nbr;

	result_nbr = strtol(str_nbr, NULL, 0);
	if (result_nbr < 0 || errno == EINVAL || errno == ERANGE) {
		return (-1);
	}
	conf->flags.packet_interval = result_nbr;
	return (0);
}

static int32_t	set_first(ProgramConf *conf, char *str_nbr) {
	int32_t	result_nbr;

	result_nbr = strtol(str_nbr, NULL, 0);
	if (result_nbr < 0 || errno == EINVAL || errno == ERANGE) {
		return (-1);
	}
	conf->flags.first_ttl = result_nbr;
	return (0);
}

static int32_t	set_max_hops(ProgramConf *conf, char *str_nbr) {
	int32_t	result_nbr;

	result_nbr = strtol(str_nbr, NULL, 0);
	if (result_nbr < 0 || errno == EINVAL || errno == ERANGE) {
		return (-1);
	}
	conf->flags.max_ttl = result_nbr;
	return (0);
}
