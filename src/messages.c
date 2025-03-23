#include "../include/my_ping.h"

double	calculate_rtt_in_ms(const struct timeval *start, const struct timeval *end) {
	double	latency;

	latency = ((end->tv_sec * (double)1000) + ((double)end->tv_usec / 1000))
		- ((start->tv_sec * (double)1000) + ((double)start->tv_usec / 1000));
	return (latency);
}
