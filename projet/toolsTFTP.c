#include <sys/types.h>
#include <stdint.h>
#include "AdresseInternet.h"
#include "socketUDP.h"
#include "toolsTFTP.h"


/**
 * Crée un paquet tftp de type ack en mettant à jour la variable length 
 * avec la taille des données écrites.
 * 
 * Valeur de retour : 0 si tout s'est bien passé, -1 sinon. 
 */
 // uint16_t is an unsigned short
int tftp_make_ack(char *buffer, size_t *length, uint16_t block) {
    // TODO
	//copie les carateres de block pour les mettre dans le buffer
	memcpy(block, buffer+sizeof(uint16_t), sizeof(uint16_t));
	//length stock la taille actuelle du buffer
	*length = sizeof(*buffer);
	// ntohs converti les unsigned short integer netshort 
	// d'un ordre network byte à un ordre host byte
	block = ntohs(block);
	
    return 0;
}

/**
 * Crée un paquet tftp de type rrq en mettant à jour la variable length 
 * avec la taille des données écrites.
 * 
 * Valeur de retour : 0 si tout s'est bien passé, -1 sinon. 
 */
int tftp_make_rrq(char *buffer, size_t *length, const char *fichier) {
    // TODO
	//copie le str pointé de fichier vers le buffer de taille du buffer plus unsigned short
	strcpy(fichier, buffer+sizeof(uint16_t));
	//length stock la taille actuelle du buffer
	*length = sizeof(*buffer);
    return 0;
}

/**
 * Crée un paquet tftp de type data en mettant à jour la variable length 
 * avec la taille des données écrites.
 * 
 * Valeur de retour : 0 si tout s'est bien passé, -1 sinon. 
 */
int tftp_make_data(char *buffer, size_t *length, uint16_t block, const
                                                        char *data, size_t n) {
    memcpy(block, buffer+sizeof(uint16_t), sizeof(uint16_t));
	block = ntohs(block);
	// n est la taille des donnees
	//on copie les donnes dans le buffer
	memcpy(data, buffer+2*sizeof(uint16_t), n);
	//mise a jour de length
	*length = sizeof(*buffer);
    return 0;
}

/**
 * Crée un paquet tftp de type error en mettant à jour la variable length 
 * avec la taille des données écrites.
 * 
 * Valeur de retour : 0 si tout s'est bien passé, -1 sinon. 
 */
int tftp_make_error(char *buffer, size_t *length, 
                    uint16_t errorcode, const char *message) {
    memcpy(errorcode, buffer+sizeof(uint16_t), sizeof(errorcode));
	errorcode = ntohs(errorcode);
	memcpy(message, buffer+2*sizeof(uint16_t), sizeof(message));
	*length = sizeof(*buffer);
    return 0;
}

/**
 * Envoie un message d'erreur à l'adresse dst.
 */ 
void tftp_send_error(SocketUDP *socket, const AdresseInternet *dst,
                                              uint16_t code, const char *msg) {
    // htons : host to network short
	code = htons(CODEOPE_ERROR);
	int octet = sendto(*socket, *msg, sizeof(*msg), code /*flag*/, (struct sockaddr*)dst, sizeof(*dst));
    
	if(octet == -1){
		printf("Les donnees n ont pas ete envoyees via le socket : %s\n", strerror(errno));
		exit(1);
	}
	return 0;
}

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
int tftp_send_RRQ_wait_DATA_with_timeout(SocketUDP *socket,
                const AdresseInternet *dst, const char *fichier,
                AdresseInternet *connexion, char *reponse, size_t *replength) {
    // TODO
    return 0;
}
 
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
int tftp_send_RRQ_wait_DATA(SocketUDP *socket,
                  const AdresseInternet *dst, const char *fichier,
                AdresseInternet *connexion, char *reponse, size_t *replength) {
    // TODO
    return 0;
}
                  
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
int tftp_send_DATA_wait_ACK(SocketUDP *socket,
            const AdresseInternet *dst, const char *paquet, size_t paquetlen) {
    // TODO
    return 0;
}

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
int tftp_send_ACK_wait_DATA(SocketUDP *socket,
            const AdresseInternet *dst, const char *paquet, size_t paquetlen) {
    // TODO
    return 0;
}

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
int tftp_send_last_ACK(SocketUDP *socket,
            const AdresseInternet *dst, const char *paquet, size_t paquetlen) {
    // TODO
    return 0;
}

