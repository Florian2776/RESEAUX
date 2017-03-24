#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "AddresseInternet.h"

int main(void) {
  
  AddresseInternet *addr  = AddresseInternet_new ("www.univ-rouen.fr", 26);
  AddresseInternet *addst = NULL;
  if ( addr == NULL ) {
    fprintf(stderr,"AddresseInternet_new");
  }
  
  
  if (AddresseInternet_copy(addst ,addr) == 0) {
    printf("it works");
    //~ printf("Nom %s/n", addst -> nom);
  }
  
  return EXIT_SUCCESS;
}
