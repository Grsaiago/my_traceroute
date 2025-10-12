#include "my_traceroute.h"

static error_t parser_func(int key, char *arg, struct argp_state *state);
static error_t parse_debug(ProgramConf *conf);
static error_t parse_port(ProgramConf *conf, char *arg);
static error_t parse_first_ttl(ProgramConf *conf, char *arg);
static error_t parse_max_ttl(ProgramConf *conf, char *arg);
static error_t parse_wait(ProgramConf *conf, char *arg);
static error_t parse_sendwait(ProgramConf *conf, char *arg);
static error_t parse_queries(ProgramConf *conf, char *arg);

// argp global variables as per:
// https://www.gnu.org/software/libc/manual/html_node/Argp-Global-Variables.html
const char *argp_program_version = "1.0";

int parse_arguments(ProgramConf *conf, int argc, char *argv[]) {

	static struct argp_option options[] = {
	    {.name = "debug",
	     .key = 'd',
	     .arg = NULL,
	     .doc = "turns on SO_DEBUG on the inner sockets"},
	    {.name = "port",
	     .key = 'p',
	     .arg = "<PORT_NUMBER>",
	     .doc = "specify the target port, between 1 and 65535, for the UDP "
	            "requests on the server, defaults to 33434"},
	    {.name = "first-ttl",
	     .key = 'f',
	     .arg = "<STARTING_TTL>",
	     .doc = "set the starting TTL for the ip packets, defualts to 0"},
	    {.name = "max-ttl",
	     .key = 'm',
	     .arg = "<MAX_TTL>",
	     .doc = "set the TTL limit, defaults none"},
	    {.name = "wait",
	     .key = 'w',
	     .arg = "<PROBE_TIMEOUT>",
	     .doc = "define the timeout for the probes in seconds, defaults to 10 "
	            "seconds"},
	    {.name = "sendwait",
	     .key = 'z',
	     .arg = "<INTERVAL>",
	     .doc = "set the interval, in seconds, between getting a response from "
	            "a probe and sending a new one"},
	    {.name = "queries",
	     .key = 'q',
	     .arg = "<PROBES_PER_HOP>",
	     .doc = "the number of probes to send at each hop"},
	    {0}
	};

	const struct argp argp = {
	    .options = options,
	    .parser = parser_func,
	    .args_doc = "<HOST | IPV4 | IPV6>",
	    .doc = "My own implementation of the traceroute tool",
	};

	return (argp_parse(&argp, argc, argv, 0, 0, (void *)conf));
}

static error_t parse_debug(ProgramConf *conf) {
	conf->flags.so_debug = true;
	return (0);
}

static error_t parse_port(ProgramConf *conf, char *arg) {
	char *endptr;

	long port = strtol(arg, &endptr, 10);
	if (*endptr != '\0' || port < 1 || port > USHRT_MAX) {
		return (EINVAL);
	}
	conf->flags.port = (uint16_t)port;
	return (0);
}

static error_t parse_first_ttl(ProgramConf *conf, char *arg) {
	char *endptr;

	long port = strtol(arg, &endptr, 10);
	if (*endptr != '\0' || port < 1 || port > USHRT_MAX) {
		return (EINVAL);
	}
	conf->flags.first_ttl = (uint16_t)port;
	return (0);
}

static error_t parse_max_ttl(ProgramConf *conf, char *arg) {
	char *endptr;

	long ttl = strtol(arg, &endptr, 10);
	if (*endptr != '\0' || ttl < 1 || ttl > SHRT_MAX)
		return (EINVAL);
	conf->flags.max_probe_ttl = (int16_t)ttl;
	return (0);
}

static error_t parse_wait(ProgramConf *conf, char *arg) {
	char *endptr;

	long wait = strtol(arg, &endptr, 10);
	if (*endptr != '\0' || wait < 0 || wait > USHRT_MAX)
		return (EINVAL);
	conf->flags.probe_response_timeout = (uint16_t)wait;
	return (0);
}

static error_t parse_sendwait(ProgramConf *conf, char *arg) {
	char *endptr;

	long sendwait = strtol(arg, &endptr, 10);
	if (*endptr != '\0' || sendwait < 0 || sendwait > USHRT_MAX)
		return (EINVAL);
	conf->flags.probe_send_interval = sendwait;
	return (0);
}

static error_t parse_queries(ProgramConf *conf, char *arg) {
	char *endptr;

	long queries = strtol(arg, &endptr, 10);
	if (*endptr != '\0' || queries < 1 || queries > USHRT_MAX)
		return (EINVAL);
	conf->flags.probes_per_hop = (uint16_t)queries;
	return (0);
}

static error_t parser_func(int key, char *arg, struct argp_state *state) {
	ProgramConf *conf = (ProgramConf *)state->input;

	switch (key) {
	case 'd':
		return parse_debug(conf);
	case 'p':
		return (parse_port(conf, arg));
	case 'f':
		return (parse_first_ttl(conf, arg));
	case 'm':
		return (parse_max_ttl(conf, arg));
	case 'w':
		return (parse_wait(conf, arg));
		break;
	case 'z':
		return (parse_sendwait(conf, arg));
	case 'q':
		return (parse_queries(conf, arg));
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
