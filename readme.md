# SNTP Client

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

References:

- NTPv4 <https://tools.ietf.org/html/rfc5905>
- NTPv3 <https://tools.ietf.org/html/rfc1305>
- <https://en.wikipedia.org/wiki/Network_Time_Protocol>

To Do:

- Port to Windows

