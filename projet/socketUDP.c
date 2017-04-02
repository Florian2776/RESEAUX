#include <stdlib.h>
#include "socketUDP.h"
#include <sys/types.h>

int initSocketUDP(SocketUDP *sock) {
  sock is_bound = 0;
  sock is_listenning = 0;
  sock -> fd = socket(AF_INET, SOCK_STREAM, 0);
  
  if (socket -> fd == -1) {
    return -1;
	printf("La creation de socket a echoue : %s\n", strerror(errno));
    exit(1);
  }  
  return 0;
}

int attacherSocketUDP(SocketUDP *sock, const char *adress, uint16_t port, int flags) {
  if (sock == NULL) {
      exit(EXIT_FAILURE);
  }
  
  //affectationd'une adresse
  memset((struct sockaddr_un*) &adress, '\0', sizeof(struct sockaddr_un));
  adress.sin_family = AF_INET;
  adress.sin_port = htons(port);
  strncpy(adress.sun_path, ADRESSE_SERVEUR, sizeof(addr.sun_path) - 1);

  if (bind(sock -> fd, (struct sockaddr*) &adress, sizeof(adress)) == -1) {
    perror("bind");
    return -1;
	exit(1);
  }
  
  // Déclarer la socket comme une socket d'écoute
  if (listen(sock -> fd, 10) == -1) {
    perror("listen");
    return -1;
	exit(1);
  }
  
  // Connexion
  if (connect(sock -> fd, (struct sockaddr *) adress, sizeof(adress) == SOCKET_ERROR) {
	  perror("connexion");
	  return -1;
	  exit(1);
  }
  
  return 0;  
}

