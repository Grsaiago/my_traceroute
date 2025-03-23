#include "../include/my_ping.h"
#include <netinet/in.h>
#include <netinet/udp.h>
#include <string.h>
#include <sys/socket.h>

bool my_ping_should_continue = true;

int main(int argc, char *argv[]) {
	struct sockaddr_in	socket_confs = {0};
	ProgramConf		conf;

	initialize_program_conf(&conf);
	parse_arguments(&conf, argc, argv);
	if (check_user_permission(&conf) != true) {
		dprintf(STDERR_FILENO, "error: you don't have the required permissions to run this program\n");
		return (EXIT_FAILURE);
	}
	/*install_signal_handlers();*/
	if (validate_or_resolve_address(&conf, (struct sockaddr *)&socket_confs) == -1) {
		return (-1);
	}
	if (new_socket(&conf.main_socket, &socket_confs, &conf.flags) == -1) {
		return (-1);
	}
	event_loop(&conf);
	return (EXIT_SUCCESS);
}

int	event_loop(ProgramConf *conf) {
	extern bool	my_ping_should_continue;
	IcmpMessage	message = {0};
	IcmpReply	last_message = {0};
	int		err_value = 0;
	int		recv_result = 0;
	struct iphdr	response_iphdr = {0};
	struct icmp	response_icmp = {0};
	(void)recv_result;
	(void)err_value;
	(void)message;
	(void)last_message;
	(void)response_icmp;

	print_header(conf);
	while(my_ping_should_continue == true) {
		char	sendbuff[sizeof(struct udphdr)] = {0};
		char	recvbuff[sizeof(struct iphdr) + sizeof(struct icmp)] = {0};

		struct udphdr udp = (struct udphdr){
			.len = sizeof(struct udphdr),
			.source = 0,
			.dest = 5500,
		};
		udp.check = calculate_checksum(&udp, sizeof(udp));
		memcpy(sendbuff, &udp, sizeof(struct udphdr));

		err_value = sendto(conf->main_socket.fd, sendbuff, sizeof(sendbuff), 0, (const struct sockaddr *)&conf->main_socket.remote_addr, conf->main_socket.addr_struct_size);
		if (err_value < (int)sizeof(struct udphdr)) {
			if (err_value == -1) {
				dprintf(STDERR_FILENO, "erro no sendto: %s\n", strerror(errno));
			} else {
				dprintf(STDERR_FILENO, "erro no sendto: de %ld bytes, somente %d foram enviados", sizeof(struct udphdr), err_value);
			}
			return (1);
		}
		recv_result = recvfrom(conf->main_socket.fd, recvbuff, sizeof(recvbuff), 0, (struct sockaddr *)&conf->main_socket.remote_addr, &conf->main_socket.addr_struct_size);
		response_iphdr = *(struct iphdr *)recvbuff;
		response_icmp = *(struct icmp *)(recvbuff + (response_iphdr.ihl * 4));
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
