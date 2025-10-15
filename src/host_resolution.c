#include "my_traceroute.h"

int validate_or_resolve_address(ProgramConf *conf, struct sockaddr *res) {
	struct addrinfo *getaddr_result;
	struct addrinfo  getaddr_hints;
	int              error_value;

	getaddr_result = NULL;
	// fill hints struct
	memset(&getaddr_hints, 0, sizeof(struct addrinfo));
	getaddr_hints.ai_family = AF_INET;      // IPV4 address
	getaddr_hints.ai_socktype = SOCK_DGRAM; // UDP sockets
	getaddr_hints.ai_protocol = 0;          // UDP protocol

	// call getaddrinfo and get the found/validated address and feed it to
	// program_conf
	error_value =
	    getaddrinfo(conf->program_arg, NULL, &getaddr_hints, &getaddr_result);
	if (error_value != 0) {
		dprintf(STDERR_FILENO, "error: %s\n", gai_strerror(error_value));
		return (-1);
	}
	memcpy(res, getaddr_result->ai_addr, getaddr_result->ai_addrlen);
	struct sockaddr_in *ipv4_addr =
	    (struct sockaddr_in *)getaddr_result->ai_addr;
	// TODO: Change this to inet_aton as per the pdf.
	inet_ntop(
	    AF_INET, &(ipv4_addr->sin_addr), conf->resolved_server_addr,
	    sizeof(conf->resolved_server_addr)
	);
	freeaddrinfo(getaddr_result);
	return (0);
}
