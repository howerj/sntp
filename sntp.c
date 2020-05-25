/*- Project:    A simple SNTP client
  - Author:     Richard James Howe
  - License:    The Unlicense
  - Email:      howe.r.j.89@gmail.com
  - Repository: https://github.com/howerj/sntp */
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define DELTA (2208988800ull)

static inline unsigned long unpack32(unsigned char *p) {
	assert(p);
	unsigned long l = 0;
	l |= ((unsigned long)p[0]) << 24;
	l |= ((unsigned long)p[1]) << 16;
	l |= ((unsigned long)p[2]) <<  8;
	l |= ((unsigned long)p[3]) <<  0;
	return l;
}

/* see https://beej.us/guide/bgnet/html/#cb46-22 */
static void *get_in_addr(struct sockaddr *sa) {
	assert(sa);
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

static int ntp_connect(const char *host_or_ip, unsigned port) {
	assert(host_or_ip);
	struct addrinfo *servinfo = NULL, *p = NULL;
       	struct addrinfo hints = { .ai_family   = AF_UNSPEC, .ai_socktype = SOCK_DGRAM, };
	int fd = -1;
	port = port ? port : 123;

	char sport[32] = { 0 };
	snprintf(sport, sizeof sport, "%u", port);
	if (getaddrinfo(host_or_ip, sport, &hints, &servinfo) != 0)
		goto fail;

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;
		if (connect(fd, p->ai_addr, p->ai_addrlen) == -1) {
			close(fd);
			fd = -1;
			continue;
		}
		break;
	}

	if (p == NULL) {
		freeaddrinfo(servinfo);
		servinfo = NULL;
		goto fail;
	}

	char ip[INET6_ADDRSTRLEN] = { 0 };
	if (NULL == inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), ip, sizeof ip))
		goto fail;

	freeaddrinfo(servinfo);
	servinfo = NULL;

	struct timeval tx_tv = { .tv_sec = 30 }, rx_tv = { .tv_sec = 30 };
	if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tx_tv, sizeof tx_tv) < 0)
		goto fail;
	if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &rx_tv, sizeof rx_tv) < 0)
		goto fail;

	return fd;
fail:
	close(fd);
	if (servinfo) {
		freeaddrinfo(servinfo);
		servinfo = NULL;
	}
	return -1;
}

static int ntp(const char *server, unsigned port, unsigned long *seconds, unsigned long *fractional) {
	assert(server);
	assert(seconds);
	assert(fractional);
	*seconds = 0;
	*fractional = 0;
	unsigned char h[48] = { 0x1b, };
	int fd = ntp_connect(server, port);
	if (fd < 0)
		return -1;
	if (write(fd, h, sizeof h) != sizeof h)
		return -2;
	if (read(fd, h, sizeof h) != sizeof h)
		return -3;
	if (close(fd) < 0)
		return -4;
	*seconds = unpack32(&h[40]) - DELTA;
	*fractional = unpack32(&h[44]);
	return 0;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage %s: ntp-server\n", argv[0]);
		return 1;
	}
	unsigned long seconds = 0, fractional = 0;
	int r = ntp(argv[1], 0, &seconds, &fractional);
	if (r < 0) {
		fprintf(stderr, "ntp failed %d\n", r);
		return 1;
	}
	time_t ts = seconds;
	fprintf(stdout, "%lu.%lu\n", ts, fractional);
	char buf[512] = { 0 };
	struct tm *timeinfo = gmtime(&ts);
	strftime(buf, sizeof buf, "%a %b %d %H:%M:%S %Z %Y", timeinfo);
	fprintf(stdout, "%s\n", buf);
	return 0;
}

