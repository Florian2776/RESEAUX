#ifndef TOOLS_TFTP_H
#define TOOLS_TFTP_H

#include <sys/types.h>
#include <stdint.h>
#include "AdresseInternet.h"
#include "socketUDP.h"

/* definition du code operation */
#define CODEOPE_RRQ		1	//demande de lecture
#define	CODEOPE_WRQ		2	//demande d ecriture
#define	CODEOPE_DATA	3	//donnees
#define	CODEOPE_ACK		4	//acquittement
#define	CODEOPE_ERROR	5	//erreur

/**
 * Crée un paquet tftp de type ack en mettant à jour la variable length 
 * avec la taille des données écrites.
 * 
 * Valeur de retour : 0 si tout s'est bien passé, -1 sinon. 
 */
extern int tftp_make_ack(char *buffer, size_t *length, uint16_t block);

/**
 * Crée un paquet tftp de type rrq en mettant à jour la variable length 
 * avec la taille des données écrites.
 * 
 * Valeur de retour : 0 si tout s'est bien passé, -1 sinon. 
 */
extern int tftp_make_rrq(char *buffer, size_t *length, const char *fichier);

/**
 * Crée un paquet tftp de type data en mettant à jour la variable length 
 * avec la taille des données écrites.
 * 
 * Valeur de retour : 0 si tout s'est bien passé, -1 sinon. 
 */
extern int tftp_make_data(char *buffer, size_t *length, uint16_t block, const
                                                          char *data, size_t n);

/**
 * Crée un paquet tftp de type error en mettant à jour la variable length 
 * avec la taille des données écrites.
 * 
 * Valeur de retour : 0 si tout s'est bien passé, -1 sinon. 
 */
extern int tftp_make_error(char *buffer, size_t *length, 
                    uint16_t errorcode, const char *message);

/**
 * Envoie un message d'erreur à l'adresse dst.
 */ 
extern void tftp_send_error(SocketUDP *socket, const AdresseInternet *dst,
                                                uint16_t code, const char *msg);

/**
 * Envoie une demande de connexion RRQ pour lire le fichier fichier, 
 * sur la socket socket à l'adresse dst.
 * La fonction se place en attente de réponse au plus TIMEOUT secondes.
 * Si elle reçoit une réponse valide (i.e. de type DATA 1), 
 * l’adresse de l’expéditeur est renseignée dans la variable
 * connexion, le paquet reçu dans reponse et sa taille dans replength.
 * Si elle reçoit une réponse non valide, un paquet d’erreur tftp est envoyé 
 * à l’expéditeur et les variables connexion, reponse, 
 * replength sont indéterminées.
 * 
 * Valeur de retour : 1 en cas de succès, 
 * 0 si le timeout est dépassé et -1 en cas d’erreur.
 */
extern int tftp_send_RRQ_wait_DATA_with_timeout(SocketUDP *socket,
                  const AdresseInternet *dst, const char *fichier,
                  AdresseInternet *connexion, char *reponse, size_t *replength);
 
/**
 * Envoie une demande de connexion RRQ pour lire le fichier fichier, 
 * sur la socket socket à l'adresse dst.
 * En cas de dépassement du timeout renvoie la demande de connexion, 
 * au plus NB_MAX_ENVOI fois.
 * Si elle reçoit une réponse valide (i.e. de type DATA 1), 
 * l’adresse de l’expéditeur est renseignée dans la variable
 * connexion, le paquet reçu dans reponse et sa taille dans replength.
 * Si elle reçoit une réponse non valide, un paquet d’erreur tftp est envoyé 
 * à l’expéditeur et les variables connexion, reponse, 
 * replength sont indéterminées.
 * 
 * Valeur de retour : 1 en cas de succès, 
 * 0 si le timeout est dépassé et -1 en cas d’erreur.
 */ 
extern int tftp_send_RRQ_wait_DATA(SocketUDP *socket,
                  const AdresseInternet *dst, const char *fichier,
                  AdresseInternet *connexion, char *reponse, size_t *replength);
                  
/**
 * Envoie un paquet de taille paquetlen de type DATA à l'adresse dst, 
 * et attend de recevoir en réponse depuis dst un paquet de type ACK 
 * correspondant (même numéro de bloc). Les paquets ACK reçus de numéro 
 * de blocs inférieur sont ignorés, tous les autres paquets reçus
 * génèrent l'envoi d'un paquet d'erreur.
 * Si le paquet attendu n'est pas reçu au bout de TIMEOUT, le paquet DATA
 * est renvoyé (au NB_MAX_ENVOI fois).
 * 
 * Valeur de retour : 1 en cas de succès, 0 si le timeout est dépassé 
 * et -1 en cas d'erreur.
 */
extern int tftp_send_DATA_wait_ACK(SocketUDP *socket,
              const AdresseInternet *dst, const char *paquet, size_t paquetlen);

/**
 * Envoie un paquet de taille paquetlen de type ACK à l'adresse dst, 
 * et attend de recevoir en réponse depuis dst un paquet de type ACK 
 * correspondant (même numéro de bloc). Les paquets DATA reçus de numéro 
 * de blocs inférieur sont ignorés, tous les autres paquets reçus
 * génèrent l'envoi d'un paquet d'erreur.
 * Si le paquet attendu n'est pas reçu au bout de TIMEOUT, le paquet ACK
 * est renvoyé (au NB_MAX_ENVOI fois).
 * 
 * Valeur de retour : 1 en cas de succès, 0 si le timeout est dépassé 
 * et -1 en cas d'erreur.
 */
extern int tftp_send_ACK_wait_DATA(SocketUDP *socket,
              const AdresseInternet *dst, const char *paquet, size_t paquetlen);

/**
 * Envoie un dernier paquet de taille paquetlen de type ACK à l'adresse dst, 
 * et attend de recevoir en réponse depuis dst un paquet de type ACK 
 * correspondant (même numéro de bloc). Les paquets DATA reçus de numéro 
 * de blocs inférieur sont ignorés, tous les autres paquets reçus
 * génèrent l'envoi d'un paquet d'erreur.
 * Si le paquet attendu n'est pas reçu au bout de TIMEOUT, le paquet ACK
 * est renvoyé (au NB_MAX_ENVOI fois).
 * 
 * Valeur de retour : 1 en cas de succès, 0 si le timeout est dépassé 
 * et -1 en cas d'erreur.
 */
extern int tftp_send_last_ACK(SocketUDP *socket,
              const AdresseInternet *dst, const char *paquet, size_t paquetlen);

#endif // TOOLS_TFTP_H
