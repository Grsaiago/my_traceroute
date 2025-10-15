#include "my_traceroute.h"

/**
 * @brief Prints the values of the ExecutionFlags structure for debugging
 * purposes.
 *
 * This function outputs the contents of the given ExecutionFlags struct to
 * stdout, displaying each field with its name and value. Useful for debugging
 * and verifying the state of execution flags.
 *
 * @param flags Pointer to an @c ExecutionFlags structure whose fields will be
 * printed.
 */
void debug_execution_flags(ExecutionFlags *flags) {
	printf("flags:\n");
	printf("\tso_debug:			%d\n", flags->so_debug);
	printf("\tport:				%d\n", flags->port);
	printf("\tfirst_ttl:			%d\n", flags->first_ttl);
	printf("\tmax_probe_ttl:			%d\n", flags->max_probe_ttl);
	printf(
	    "\tprobe_response_timeout:		%d\n", flags->probe_response_timeout
	);
	printf("\tprobe_send_interval:		%d\n", flags->probe_send_interval);
	printf("\tprobes_per_hop:			%d\n", flags->probes_per_hop);
}

/**
 * @brief Prints the values of the ProgramConf structure for debugging
 * purposes.
 *
 * This function outputs the contents of the given ProgramConf struct to
 * stdout, displaying each field with its name and value. Useful for debugging
 * and verifying the state of the program configuration.
 *
 * @param conf Pointer to a @c ProgramConf structure whose fields will be
 * printed.
 */
void debug_program_conf(ProgramConf *conf) {
	printf("program_conf:\n");
	printf("\tprogram_arg:			%s\n", conf->program_arg);
	printf("\tresolved_server_addr:		%s\n", conf->resolved_server_addr);
	printf("\tsend_socket.fd:			%d\n", conf->send_socket->fd);
	printf("\trecv_socket.fd:			%d\n", conf->recv_socket->fd);
	printf("\tprobes_per_hop:			%d\n", conf->flags.probes_per_hop);
	for (int i = 0; i < conf->flags.probes_per_hop; i++) {
		printf(
		    "\tprobe[%d].sender_addr_str:	%s\n", i,
		    conf->probes[i].sender_addr_str
		);
	}
}
