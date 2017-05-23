/*
BSD 3-Clause License

Copyright (c) 2017, John Ventura
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

int sendudp(uint32_t s_ip, uint32_t d_ip, uint16_t sport, uint16_t dport,
	    uint8_t * buf, int buflen) {
    int sock;
    struct sockaddr_in daddr;
    struct sockaddr_in saddr;
    int one = 1;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock <= 0) {
	perror("Can't open socket");
	exit(1);
    }

    daddr.sin_family = AF_INET;
    daddr.sin_addr.s_addr = htonl(d_ip);
    daddr.sin_port = htons(dport);

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(s_ip);
    saddr.sin_port = htons(sport);

    if (bind(sock, (const struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
	perror("can't bind socket");
	exit(1);
    }

    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &one, sizeof(one));
    sendto(sock, buf, buflen, 0, (const struct sockaddr *) &daddr,
	   sizeof(struct sockaddr_in));

    close(sock);

    return (0);
}