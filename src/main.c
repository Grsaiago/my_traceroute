#include "../include/my_ping.h"

bool my_ping_should_continue = true;

int main(int argc, char *argv[]) {
	struct sockaddr_in	remote_addr = {0};
	struct sockaddr_in	local_addr;
	ProgramConf		conf;

	initialize_program_conf(&conf);
	parse_arguments(&conf, argc, argv);
	if (check_user_permission(&conf) != true) {
		dprintf(STDERR_FILENO, "error: you don't have the required permissions to run this program\n");
		return (EXIT_FAILURE);
	}
	/*install_signal_handlers();*/
	if (validate_or_resolve_address(&conf, (struct sockaddr *)&remote_addr) != 0) {
		return (-1);
	}
	local_addr = (struct sockaddr_in) {
		.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY,
		.sin_port = 0,
	};
	if (new_socket_pair(&remote_addr, &local_addr, &conf.sock_pair, &conf.flags) != 0) {
		return (-1);
	}
	event_loop(&conf);
	return (EXIT_SUCCESS);
}

int	event_loop(ProgramConf *conf) {
	extern bool	my_ping_should_continue;
	TracerouteMessage	message;
	IcmpReply		reply = {0};
	int		err_value = 0;
	int		recv_result = 0;

	(void)recv_result;
	print_header(conf);
	while(my_ping_should_continue == true) {
		message = new_message(conf, TR_ICMP_PAYLOAD);
		gettimeofday(&reply.sent_at, NULL);
		err_value = send_message(conf, &message);
		if (err_value != 0) {
			return(-1);
		}
		recv_result = recv_message(&conf->sock_pair.in_sock, &reply);
		gettimeofday(&reply.recv_at, NULL);
		/*// if it didn't error_create a new message.*/
		/*// If there was an error, we need to retry the same message.*/
		/*if (!err_value) {*/
		/*	message = new_icmp_echo_message(conf);*/
		/*}*/
		/*gettimeofday(&last_message.sent_at, NULL);*/
		/*err_value = send_icmp_message(&conf->main_socket, &message);*/
		/*if (err_value != 0) {*/
		/*	continue ;*/
		/*}*/
		/*recv_result = recv_icmp_message(&conf->main_socket, &last_message);*/
		/*if (recv_result == -1 && errno == EINTR) {*/
		/*	break;*/
		/*}*/
		/*gettimeofday(&last_message.recv_at, NULL);*/
		/*record_new_response(conf, &last_message);*/
		/*print_icmp_message(conf, &last_message);*/
	}
	print_footer(conf);
	return (0);
}

bool check_user_permission(ProgramConf *conf) {
	(void)conf;
	return (getuid() == 0);
}
