% sntp(1) | A bare-bones SNTP client

# NAME

SNTP - A bare-bones SNTP client

# SYNOPSES

sntp server port?

# DESCRIPTION

- Project:    A simple SNTP client
- Author:     Richard James Howe
- License:    The Unlicense
- Email:      howe.r.j.89@gmail.com
- Repository: https://github.com/howerj/sntp

Super bare bones SNTP client. Implementing a full NTP client is quite complex,
just getting the time from an NTP server is not, most applications do not need
sub second accuracy, or even second accuracy. This program should be fine for
those use cases.

Most of the complexity of this solution is in dealing with the IPv4/IPv6
connection. They really did make it ugly.

To build and run you will require two things *make* and a C compiler. Type
'make' to build the example program called 'sntp' and type 'make run' to build
and run the 'sntp' program. It will attempt to connect to a NTP pool specified
in the makefile.

Building the program also produces a library, 'libsntp.a', the SNTP library is
incredibly simple, much like the program, containing just one function:

	int sntp(const char *server, unsigned port, unsigned long *seconds, unsigned long *fractional);

This function returns zero on success, and negative on failure. You must
provide a SNTP server to connect to (either a hostname, an IPv4, or an IPv6
address), a port (the default port for NTP is port 123, set the port to zero to
auto select this) and two variables into which the time will placed on success,
on failure both values will be set to zero. This function will block until the
client either returns success or failure.

A non-blocking version of the 'sntp' function would be trivial to make, it
would require only a file descriptor and a single variable describing the
program position to be stored and passed into the 'sntp' function between
calls.

References:

- NTPv4 <https://tools.ietf.org/html/rfc5905>
- NTPv3 <https://tools.ietf.org/html/rfc1305>
- <https://en.wikipedia.org/wiki/Network_Time_Protocol>

# LICENSE

All code and documentation is released into the public domain where this is
allowed, do what thou wilt.

# RETURN CODE

The 'sntp' program returns '1' on failure and '0' on success.


