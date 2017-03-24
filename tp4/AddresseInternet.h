
#ifndef _AddresseInternet_
#define _AddresseInternet_


#define _DNS_NAME_MAX_SIZE 256
#define _SERVICE_NAME_MAX_SIZE 20
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

typedef struct AddresseInternet {
  struct sockaddr_storage sockAddr;
  char nom[_DNS_NAME_MAX_SIZE];
  char service[_SERVICE_NAME_MAX_SIZE];
}AddresseInternet;

extern AddresseInternet *AddresseInternet_new (const char* addresse, 
                                                                 uint16_t port);
extern AddresseInternet *AddresseInternet_any(uint16_t port);
extern AddresseInternet *AddresseInternet_loopback(uint16_t port);
extern void AddresseInternet_free(AddresseInternet *addresse);
extern int AddresseInternet_getinfo(AddresseInternet *addresse, char *nomDNS,
                                  int tailleDNS, char *nomPort, int taillePort);
extern int AddresseInternet_getIP(const AddresseInternet *addresse, char *IP,
                                  int tailleIP);
extern uint16_t AddresseInternet_getPortt(const AddresseInternet *addresse);
extern int AddresseInternet_getDomain(const AddresseInternet *addresse);
extern int sockaddr_to_AddresseInternet(const struct sockaddr *addr,
                                                   AddresseInternet *addresse);
extern int AddresseInternet_to_sockaddr(const AddresseInternet *addresse,
                                         struct sockaddr *addr);

extern int AddresseInternet_compare(const AddresseInternet *addrese1, 
                                             const AddresseInternet *addrese2);
extern int AddresseInternet_copy(AddresseInternet *addrdst, 
                                             const AddresseInternet *addrsrc);

#endif
