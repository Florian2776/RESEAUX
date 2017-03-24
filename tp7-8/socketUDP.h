#ifndef SOCKETUDP_H_
#define SOCKETUDP_H_

#include "AdresseInternet.h"
#include <sys/socket.h>

typedef struct  {
  int fd;
  struct AdresseInternet *local_addr;
  struct AdresseInternet *distant_addr;
  int is_connected;
  int is_bound;
  int is_listenning;
} SocketUDP;

typedef SocketUDP SocketUDP;

int initSocketUDP(SocketUDP *sock);

int attacherSocketUDP(SocketUDP *sock, const char *adress, uint_16 port, int flags);


#endif // SOCKETUDP_H_
