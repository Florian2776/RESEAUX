#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "socket_tools.h"

int main(int argc, char* argv[]) {
  int s;

  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  struct sockaddr_in addr;
  build_inet_address(&addr, "localhost", "44400");

  if (connect(s, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
    perror("connect");
    return EXIT_FAILURE;
  }
  display_inet_address(s);

  if (write(s, "bonjour ", 9) == -1) {
    perror("write");
    return EXIT_FAILURE;
  }

  if (write(s, "tout le monde", 14) == -1) {
    perror("write");
    return EXIT_FAILURE;
  }

  sleep(1);

  if (close(s) == -1) {
    perror("close");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
