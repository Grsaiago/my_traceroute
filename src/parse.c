#include "../include/my_ping.h"

static error_t	parser_func(int key, char *arg, struct argp_state *state);
static int32_t	set_first_ttl(ProgramConf *conf, char *str_nbr);
static int32_t	set_max_hops(ProgramConf *conf, char *str_nbr);

// argp global variables as per: https://www.gnu.org/software/libc/manual/html_node/Argp-Global-Variables.html
const char			*argp_program_version = "1.0";

int	parse_arguments(ProgramConf *conf, int argc, char *argv[]) {
	static struct argp_option	options[] = {
		{ .name = "no-ip-to-host-resolve", .key = 'n', .arg = NULL, .doc = "Do not try to map IP addresses to host names when displaying them." },
		{ .name = "max-hops", .key = 'm', .arg = "max-ttl", .doc = "Specifies the maximum number of hops (max time-to-live value) traceroute will probe. The default is 30." },
		{ .name = "first", .key = 'f', .arg = "first_ttl", .doc = "Specifies with what TTL to start. Defaults to 1." },
		{ .name = "icmp", .key = 'I', .arg = NULL, .doc = "Use ICMP ECHO for probes." },
		{ .name = "debug", .key = 'd', .arg = NULL, .doc = "Enable socket level debugging (when the Linux kernel supports it)." },
		{0}
	};

	const struct argp argp = {
		.options = options,
		.parser = parser_func,
		.args_doc = "[HOST]",
		.doc = "tracks the route packets taken from an IP network on their way to a given host.",
	};
	argp_parse(&argp, argc, argv, 0, 0, (void *)conf);
	return (0);
}

static error_t	parser_func(int key, char *arg, struct argp_state *state) {
	(void)arg;
	ProgramConf	*conf;

	conf = (ProgramConf *)state->input;
	switch (key) {
		case ('n'):
			conf->flags.resolve_ip_name = false;
			break;
		case ('m'):
			if (arg == NULL || set_max_hops(conf, arg) == -1) {
				argp_error(state, "Max hops cannot be less than 1\n");
			}
			break ;
		case ('f'):
			if (arg == NULL || set_first_ttl(conf, arg) == -1) {
				argp_error(state, "First cannot be less than 1\n");
			}
			break ;
		case ('I'):
			conf->flags.out_socket_type = ICMP_SOCK;
			break ;
		case ('d'):
			conf->flags.so_debug = true;
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

static int32_t	set_first_ttl(ProgramConf *conf, char *str_nbr) {
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
