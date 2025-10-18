#include "my_traceroute.h"

/**
 * @brief Frees resources allocated in a ProgramConf structure.
 *
 * This function releases all dynamically allocated memory and closes any open
 * sockets associated with the provided @c ProgramConf structure. It ensures
 * that all resources are properly cleaned up to prevent memory leaks and
 * dangling file descriptors.
 *
 * @param conf Pointer to the @c ProgramConf structure whose resources are to be
 * freed.
 */
void delete_program_conf(ProgramConf *conf) {
	if (conf->probes != NULL) {
		for (int i = 0; i < conf->flags.probes_per_hop; i++) {
			delete_probe(&conf->probes[i]);
		}
		free(conf->probes);
		conf->probes = NULL;
	}
	if (conf->send_socket != NULL) {
		delete_udp_socket(conf->send_socket);
		free(conf->send_socket);
		conf->send_socket = NULL;
	}
	if (conf->recv_socket != NULL) {
		delete_icmp_socket(conf->recv_socket);
		free(conf->recv_socket);
		conf->recv_socket = NULL;
	}
	return;
}

/**
 * @brief Frees resources allocated in an IcmpSocket structure.
 *
 * This function releases all dynamically allocated memory and closes any open
 * sockets associated with the provided @c IcmpSocket structure. It ensures
 * that all resources are properly cleaned up to prevent memory leaks and
 * dangling file descriptors.
 *
 * @param socket Pointer to the @c IcmpSocket structure whose resources are to
 * be freed.
 */
void delete_icmp_socket(IcmpSocket *socket) {
	if (socket != NULL) {
		if (socket->fd >= 0) {
			close(socket->fd);
		}
	}
	return;
}

/**
 * @brief Frees resources allocated in a UdpSocket structure.
 *
 * This function releases all dynamically allocated memory and closes any open
 * sockets associated with the provided @c UdpSocket structure. It ensures
 * that all resources are properly cleaned up to prevent memory leaks and
 * dangling file descriptors.
 *
 * @param socket Pointer to the @c UdpSocket structure whose resources are to be
 * freed.
 */
void delete_udp_socket(UdpSocket *socket) {
	if (socket != NULL) {
		if (socket->fd >= 0) {
			close(socket->fd);
		}
	}
	return;
}

/**
 * @brief Frees resources allocated in a Probe structure.
 *
 * This function releases all dynamically allocated memory associated with the
 * provided @c Probe structure. It ensures that all resources are properly
 * cleaned up to prevent memory leaks.
 *
 * @param probe Pointer to the @c Probe structure whose resources are to be
 * freed.
 */
void delete_probe(Probe *probe) {
	if (probe != NULL) {
		if (probe->sender_addr_str != NULL) {
			free(probe->sender_addr_str);
		}
	}
	return;
}
