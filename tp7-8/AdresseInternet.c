#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <netinet/ip.h> // Pour in_port_t
#include "/usr/include/netdb.h"

#include "AdresseInternetType.h"

/** 
 * construit une adresse internet à partir d’un éventuel nom (sous
 * forme DNS ou IP) et d’un numéro de port. L’argument adresse est un
 * pointeur vers une variable de type AdresseInternet, qui est remplie
 * avec l’adresse internet construite et allouée. Valeur de retour :
 * en cas de succès, la fonction renvoie le pointeur. En cas d’erreur
 * la fonction renvoie NULL.
 */
AdresseInternet * AdresseInternet_new(const char* nom, uint16_t port) {
  AdresseInternet *adress = malloc(sizeof *adress);
  if (nom == NULL || adress == NULL) {
    return NULL;
  }
  adress -> sockAddr = malloc(sizeof(struct sockadrr_storage));
  strcpy((*adress).nom, nom);
  uint16_t p = htons(port);
  sprintf((*adress).service, "%d", p);
  
  struct addrinfo hints;
  struct addrinfo *result;
  int s;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = 0;
  hints.ai_flags = 0;
  
  s = getaddrinfo(NULL, NULL, &hints, &result);
  if (s != 0) {
       fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
       exit(EXIT_FAILURE);
  }

  return adress;
}

/**
 * Idem, mais construit une adresse internet correspondant à toutes
 * les interfaces réseau à partir d’un numéro de port.
 */
AdresseInternet * AdresseInternet_any(uint16_t port) {    
  AdresseInternet *adress = malloc(sizeof *adress);
  if (adress == NULL) {
    return NULL;
  }
  strcpy((*adress).nom, "0.0.0.0");
  uint16_t p = htons(port);
  sprintf((*adress).service, "%d", p);
  return adress;
}

/**
 * Idem, mais construit une adresse internet correspondant à
 * l'interface loopback à partir d’un numéro de port.
 */
AdresseInternet * AdresseInternet_loopback(uint16_t port) {
  AdresseInternet *adress = malloc(sizeof *adress);
  if (adress == NULL) {
    return NULL;
  }
  strcpy((*adress).nom, "127.0.0.0");
  uint16_t p = htons(port);
  sprintf((*adress).service, "%d", p);
  return adress;  
}

/** 
 * libère la mémoire allouée par AdresseInternet, any et loopback
 */
void AdresseInternet_free(AdresseInternet *adresse) {
    free(&(*adresse).sockAddr);
    free(adresse);
    adresse = NULL;
}

/**
 * extrait d’une adresse internet l’adresse IP et le port
 * correspondants. L’argument adresse pointe vers un buffer 
 * contenant une adresse. L’argument nomDNS (resp. nomPort)
 * pointe vers un buffer alloué de taille au moins tailleDNS
 * (resp. taillePort) dans lequel la fonction va écrire une chaîne
 * de caractère (terminant par un 0) contenant le nom (resp. le port)
 * associé à l’adresse fournie. Lorsque cela est possible, la
 * résolution de nom est faite.  Si nomDNS ou nomPort est NULL,
 * l’extraction correspondante ne sera pas effectuée. Les deux ne
 * peuvent pas être NULL en même temps.  Valeur de retour : rend 0 en
 * cas de succès, et -1 en cas d’erreur.
 */
int AdresseInternet_getinfo(AdresseInternet *adresse,
			    char *nomDNS, int tailleDNS, char *nomPort, int taillePort) {
  if (adresse == NULL) {
    return -1;
  }  
  nomDNS = adresse -> nom;
  tailleDNS = sizeof(adresse -> nom);
  nomPort = adresse -> service;
  if (nomDNS == NULL || nomPort == NULL) {
    return -1;
  }   
  return 0;
}

int AdresseInternet_getIP(const AdresseInternet *adresse, char *IP, int tailleIP);

/**
 * extrait le numéro de port d’une adresse internet. L'argument
 * adresse se comporte comme dans la fonction
 * précédente.  Valeur de retour : renvoie le port en cas de succès,
 * et 0 en cas d’erreur (par exemple si adresse n’est pas
 * initialisée).
*/
uint16_t AdresseInternet_getPort(const AdresseInternet *adresse);

/**
 * rend le domaine internet de l'adresse (AF_INET ou AF_INET6) ou -1
 * si l'adresse n'est pas initialisée.
 */
int AdresseInternet_getDomain(const AdresseInternet *adresse);

/**
 * Construit une adresse internet à partir d'une structure sockaddr La
 * structure addresse doit être suffisamment grande pour pouvoir
 * accueillir l’adresse.  Valeur de retour : 0 en cas de succès, -1 en
 * cas d’échec.
 */
int sockaddr_to_AdresseInternet(const struct sockaddr *addr, AdresseInternet *adresse);
/**
 * Construit une structure sockaddr à partir d'une adresse
 * internet. La structure addr doit être suffisamment grande pour
 * pouvoir accueillir l’adresse.  Valeur de retour : 0 en cas de
 * succès, -1 en cas d’échec.
 */
int AdresseInternet_to_sockaddr(const AdresseInternet *adresse, struct sockaddr *addr);
/**
 * compare deux adresse internet adresse1 et adresse2.  Valeur de
 * retour : rend 0 si les adresses sont différentes, 1 si elles sont
 * identiques (même IP et même port), et -1 en cas d’erreur.
 *
 */
int AdresseInternet_compare(const AdresseInternet *adresse11, const AdresseInternet *adresse22);

/* copie une adresse internet dans une autre. Les variables doivent
   être allouées. */
int AdresseInternet_copy(AdresseInternet *adrdst, const AdresseInternet *adrsrc);





