#include "my_traceroute.h"
#include <criterion/criterion.h>

Test(parse_arguments, simple_parsing) {
	ProgramConf conf;
	initialize_program_conf(&conf);
	char *argv[] = {
	    "my_traceroute",
	    "-d",
	    "-p",
	    "8080",
	    "-f",
	    "5",
	    "-m",
	    "30",
	    "-w",
	    "15",
	    "-z",
	    "2",
	    "-q",
	    "3",
	    "example.com"
	};
	int argc = sizeof(argv) / sizeof(*argv);
	int result = parse_cli_args(&conf, argc, argv);
	cr_assert_eq(
	    result, 0, "Expected parse_arguments to return 0, but got %d", result
	);
	cr_assert_eq(conf.flags.so_debug, true, "Expected so_debug to be true");
	cr_assert_eq(conf.flags.port, 8080, "Expected port to be 8080");
	cr_assert_eq(conf.flags.first_ttl, 5, "Expected first_ttl to be 5");
	cr_assert_eq(
	    conf.flags.max_probe_ttl, 30, "Expected max_probe_ttl to be 30"
	);
	cr_assert_eq(
	    conf.flags.probe_response_timeout, 15,
	    "Expected probe_response_timeout to be 15"
	);
	cr_assert_eq(
	    conf.flags.probe_send_interval, 2,
	    "Expected probe_send_interval to be 2"
	);
	cr_assert_eq(
	    conf.flags.probes_per_hop, 3, "Expected probes_per_hop to be 3"
	);
	cr_assert_str_eq(
	    conf.program_arg, "example.com",
	    "Expected program_arg to be 'example.com'"
	);
}
