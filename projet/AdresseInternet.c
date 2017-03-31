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
#include "AdresseInternet.h"

#define MAXHOST      1025
#define FUN_SUCCESS  0
#define FUN_FAILURE  -1
#define PORT_ERROR   0

#define FNCT_T(comp)     \
    if (comp) {           \
      return FUN_FAILURE; \
    }


AdresseInternet *AdresseInternet_new (const char* addresse, uint16_t port) {
  
  AdresseInternet *addr = malloc (sizeof(*addr));  
   
   if (addr == NULL) {
     return NULL;
  }
  
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = 0;
  hints.ai_flags = AI_PASSIVE;
 
  
  struct addrinfo *result;  
  sprintf(addr -> service, "%u", port);
  
  int s;

  s = getaddrinfo( addresse, addr -> service, &hints, &result);
 

  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  if (result != NULL) {
    (addr -> sockAddr).ss_family = result -> ai_family;
  }  
  strcpy (addr -> nom, addresse);  
  free(addr);
  return addr;
}

AdresseInternet *AdresseInternet_any(uint16_t port) {
    
  AdresseInternet *addr = malloc (sizeof(*addr));  
   if (addr == NULL) {
     return NULL;
  }
  
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = 0;
  hints.ai_flags = 0;
 
   
  struct addrinfo *result;  
  sprintf(addr -> service, "%u", port);
  
  int s;
  s = getaddrinfo( NULL, addr -> service, &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  if (result != NULL) {
    (addr -> sockAddr).ss_family = result -> ai_family;
  }
  
  free(addr);
  return addr;
}

AdresseInternet *AdresseInternet_loopback(uint16_t port) {

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  addr.sin_addr.s_addr = INADDR_LOOPBACK;
  
  char host[MAXHOST];
  int errnum;
  if ((errnum = getnameinfo((struct sockaddr *) &addr, sizeof(addr), host, sizeof(host), NULL, 0, 0)) != 0) {
    fprintf(stderr, "getnameinfo: %s\n", gai_strerror(errnum));
    exit(EXIT_FAILURE);
  }  
  return AdresseInternet_new (host, port);
}

AdresseInternet *AdresseInternet_loopback(uint16_t port);
void AdresseInternet_free(AdresseInternet *addresse) {
  free(addresse);
}


int AdresseInternet_getinfo(AdresseInternet *addresse, char *nomDNS,
                                int tailleDNS, char *nomPort, int taillePort) {
  FNCT_T(addresse == NULL);
  if (nomDNS != NULL || nomPort != NULL) {
    strncpy(nomDNS, addresse -> nom, tailleDNS);
    strncpy(nomPort, addresse -> service, taillePort);
    nomDNS += '0';
    nomPort += '0';
    return FUN_SUCCESS;
  }  
  return FUN_FAILURE;
}
int AdresseInternet_getIP(const AdresseInternet *addresse, char *IP,
                                                                int tailleIP) {
  FNCT_T(addresse == NULL);
  strncpy( IP, addresse -> nom, tailleIP);
  return FUN_SUCCESS;
}

uint16_t AdresseInternet_getPort(const AdresseInternet *addresse) {
  if (addresse == NULL) {
    return PORT_ERROR;
  }
  uint16_t  port = (uint16_t) strtol(addresse -> service, NULL, 16);  
  return port;  
}

int AdresseInternet_getDomain(const AdresseInternet *addresse) {
  
  return addresse == NULL? FUN_FAILURE: (addresse -> sockAddr).ss_family;
}


int sockaddr_to_AdresseInternet(const struct sockaddr *addr,
                                                   AdresseInternet *addresse) {
  FNCT_T(addr == NULL);
  FNCT_T(addresse == NULL);
  addresse -> sockAddr.ss_family = addr -> sa_family; 
  strcpy(addresse -> service, addr -> sa_data);
  return FUN_SUCCESS;
}
                                                   
int AdresseInternet_to_sockaddr(const AdresseInternet *addresse,
                                                        struct sockaddr *addr) {
  FNCT_T(addresse == NULL);
  FNCT_T(addr == NULL);
  addr -> sa_family = addresse -> sockAddr.ss_family;
  strcpy(addr -> sa_data, addresse -> service);  
  return FUN_SUCCESS;
}

int AdresseInternet_compare(const AdresseInternet *addrese1, 
                                             const AdresseInternet *addrese2) {
  FNCT_T(addrese1 == NULL);
  FNCT_T(addrese2 == NULL);
  size_t lgnth = sizeof(addrese1);
  return memcmp(addrese1, addrese2, lgnth) == 0 ? FUN_SUCCESS : FUN_FAILURE;
}

int AdresseInternet_copy( AdresseInternet *addrdst, 
                                             const AdresseInternet *addsrc) {
  FNCT_T(addsrc == NULL);
  FNCT_T(addrdst == NULL);
  size_t lgn = sizeof(addsrc);
  memcpy(addrdst, addsrc, lgn);
  return FUN_SUCCESS;
}


