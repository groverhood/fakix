#ifndef POSIX_ARPA_INET
#define POSIX_ARPA_INET 1

#include <inttypes.h>
#include <netinet/in.h>

uint32_t htonl(uint32_t);
uint16_t htons(uint16_t);
uint32_t ntohl(uint32_t);
uint16_t ntohs(uint16_t);

in_addr_t inet_addr(const char *);
char *inet_ntoa(struct in_addr);
const char *inet_ntop(int, const void *__restrict__,
		      char *__restrict__, socklen_t);
int inet_pton(int, const char *__restrict__,
	      void *__restrict__);

#endif
