#include <stdlib.h>
#include "socketUDP.h"
#include <sys/types.h>

int initSocketUDP(SocketUDP *sock) {
  socket is_bound = 0;
  socket is_listenning = 0;
  socket -> fd = socket(AF_INET, SOCK_STREAM, 0);
  
  if (socket -> fd == -1) {
    return -1;
  }  
  return 0;
}

int attacherSocketUDP(SocketUDP *sock, const char *adress, uint16_t port, int flags) {
  sock->fd = socket(AF_INET, SOCK_DGRAM,0);
  
  if (sock == NULL) {
      exit(EXIT_FAILURE);
  }  
  if (sock->fd == -1) {
	  printf("La creation de socket a echoue : %s\n", strerror(errno));
      exit(1);
  }
  // TODO
  
  return 0;  
}

