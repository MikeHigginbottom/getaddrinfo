#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>

void printFlags(int flags) {
  printf("Flags:    ");
  if (flags & AI_PASSIVE)
    printf(" AI_PASSIVE");
  if (flags & AI_CANONNAME)
    printf(" AI_CANONNAME");
  if (flags & AI_NUMERICHOST)
    printf(" AI_NUMERICHOST");
  if (flags & AI_V4MAPPED)
    printf(" AI_V4MAPPED");
  if (flags & AI_ALL)
    printf(" AI_ALL");
  if (flags & AI_ADDRCONFIG)
    printf(" AI_ADDRCONFIG");
  if (flags & AI_NUMERICSERV)
    printf(" AI_NUMERICSERV");
#ifdef __USE_GNU
  if (flags & AI_IDN)
    printf(" AI_IDN");
  if (flags & AI_CANONIDN)
    printf(" AI_CANONIDN");
  if (flags & AI_IDN_ALLOW_UNASSIGNED)
    printf(" AI_IDN_ALLOW_UNASSIGNED");
  if (flags & AI_IDN_USE_STD3_ASCII_RULES)
    printf(" AI_IDN_USE_STD3_ASCII_RULES");
#endif
  printf("\n");
  return;
}

void printFamily(int family) {
  printf("Family:    ");
  if (family == AF_INET)
    printf("AF_INET");
  else if (family == AF_INET6)
    printf("AF_INET6");
  else if (family == AF_UNSPEC)
    printf("AF_UNSPEC");
  printf("\n");
  return;
}

void printSockType(int sockType) {
  printf("Socktype:  ");
  if (sockType == SOCK_STREAM)
    printf("SOCK_STREAM");
  else if (sockType == SOCK_DGRAM)
    printf("SOCK_DGRAM");
  printf("\n");
  return;
}

void printProtocol(int protocol) {
  struct protoent *entry = getprotobynumber(protocol);
  printf("Protocol:\n");
  printf("  Name:    %s\n", entry->p_name);
  printf("  Aliases:");
  char **ptr = entry->p_aliases;
  for(char *alias = *ptr; alias != NULL; alias=*++ptr)
    printf(" %s", alias);
  printf("\n");
  printf("  Number:  %d\n", entry->p_proto);
  return;
}

void printAddr(struct sockaddr *addr) {
  printf("Addr:\n");
  if (addr->sa_family == AF_INET) {
    char ip4[INET_ADDRSTRLEN];
    struct sockaddr_in *ipv4addr = (struct sockaddr_in *)(addr);
    if (ipv4addr->sin_family == AF_INET)
      printf("  Family:  AF_INET\n");
    printf("  Port:    %d\n", ipv4addr->sin_port);
    inet_ntop(AF_INET, &(ipv4addr->sin_addr), ip4, INET_ADDRSTRLEN);
    printf("  Address: %s\n", ip4);
  }
  else if (addr->sa_family == AF_INET6) {
    char ip6[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *ipv6addr = (struct sockaddr_in6 *)(addr);
    if (ipv6addr->sin6_family == AF_INET6)
      printf("  Family:  AF_INET6\n");
    printf("  Port:    %d\n", ipv6addr->sin6_port);
    inet_ntop(AF_INET6, &(ipv6addr->sin6_addr), ip6, INET6_ADDRSTRLEN);
    printf("  Address: %s\n", ip6);
  }
  return;
}

void printAddrInfo(struct addrinfo *ai) {
  printFlags(ai->ai_flags);
  printFamily(ai->ai_family);
  printSockType(ai->ai_socktype);
  printProtocol(ai->ai_protocol);
  printf("AddrLen:   %d\n", ai->ai_addrlen);
  printAddr(ai->ai_addr);
  printf("CanonName: %s\n", ai->ai_canonname);
  printf("Next:      %p\n", ai->ai_next);
  return;
}

int main (int argc, char **argv) {
  struct addrinfo ai;
  memset(&ai, 0, sizeof(struct addrinfo));
  ai.ai_family = AF_INET;
  ai.ai_socktype = SOCK_DGRAM;
  ai.ai_flags = AI_PASSIVE;
  ai.ai_protocol = 0;
  ai.ai_canonname = NULL;
  ai.ai_addr = NULL;
  ai.ai_next = NULL;

  clock_t t; 
  t = clock(); 
  struct addrinfo *res;
  int ret;
  ret = getaddrinfo("localhost", NULL, &ai, &res);
  t = clock() - t; 
  printf("Elapsed time: %f seconds\n", ((double)t)/CLOCKS_PER_SEC); 

  if (ret != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
    exit(EXIT_FAILURE);
  }
  struct addrinfo *rp;
  for (rp = res; rp != NULL; rp = rp->ai_next) {
    printAddrInfo(rp);
  }

  freeaddrinfo(res);
  exit(0);
}
