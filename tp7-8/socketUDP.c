#include <stdlib.h>
#include "socketUDP.h"


int initSocketUDP(SocketUDP *sock) {
  socket is_bound = 0;
  socket is_listenning = 0;
  socket -> fd = socket(AF_INET, SOCK_STREAM, 0);
  
  if (socket -> fd == -1) {
    return -1;
  }  
  return 0;
}

int attacherSocketUDP(SocketUDP *sock, const char *adress, uint_16 port, int flags) {
  if (sock == NULL) {
      exit(EXIT_FAILURE);
  }  
  
}

