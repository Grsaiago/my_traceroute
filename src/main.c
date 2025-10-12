#include "my_traceroute.h"

bool my_traceroute_should_continue = true;

int main(int argc, char **argv) {
	ProgramConf conf;

	initialize_program_conf(&conf);

	parse_arguments(&conf, argc, argv);
	debug_execution_flags(&conf.flags);
}
