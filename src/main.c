#include "my_traceroute.h"

bool my_traceroute_should_continue = true;

int main(int argc, char **argv) {
	ProgramConf conf;

	memset(&conf, 0, sizeof(ProgramConf));

	initialize_execution_flags(&conf.flags);
	parse_cli_args(&conf, argc, argv);
	initialize_program_conf(&conf);
	delete_program_conf(&conf);
}
