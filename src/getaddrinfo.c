/**
 * Récupération de l'adresse IP d'une machine à partir
 * de son nom (getaddrinfo).
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define BUF_SIZE 255

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s hostname\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_CANONNAME;

  int s;
  struct addrinfo *result;
  s = getaddrinfo(argv[1], NULL, &hints, &result);

  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  if (result != NULL) {
    printf("Canonical name: %s\n", result->ai_canonname);
  }

  for (struct addrinfo* rp = result; rp != NULL; rp = rp->ai_next) {
    struct sockaddr_in* sin = (struct sockaddr_in*)rp->ai_addr;
    char buffer[BUF_SIZE];
    // On devrait utiliser ntop et non ntoa
    printf("IP address: %s\n", inet_ntop(rp->ai_family, &sin->sin_addr, buffer, sizeof(buffer)));
  }

  freeaddrinfo(result);
  exit(EXIT_SUCCESS);
}
