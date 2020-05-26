#include "sntp.h"
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {
	unsigned port = 0;
	if (argc != 2 && argc != 3) {
		(void)fprintf(stderr, "usage %s: ntp-server port?\n", argv[0]);
		return 1;
	}
	if (argc == 3) {
		if (sscanf(argv[2], "%u", &port) != 1) {
			(void)fprintf(stderr, "Invalid number %s\n", argv[2]);
			return 1;
		}
	}
	unsigned long seconds = 0, fractional = 0;
	int r = sntp(argv[1], port, &seconds, &fractional);
	if (r < 0) {
		(void)fprintf(stderr, "ntp failed %d\n", r);
		return 1;
	}
	time_t ts = seconds;
	if (fprintf(stdout, "%lu.%lu\n", ts, fractional) < 0)
		return 1;
	char buf[512] = { 0 };
	struct tm *timeinfo = gmtime(&ts);
	if (strftime(buf, sizeof buf, "%a %b %d %H:%M:%S %Z %Y", timeinfo) == 0)
		return 1;
	if (fprintf(stdout, "%s\n", buf) < 0)
		return 1;
	return 0;
}

