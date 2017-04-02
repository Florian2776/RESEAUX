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
 *
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
    int time = 0;
	// demande de connexion RRQ 
	if (sendto(*socket, CODEOPE_RRQ, 0, (struct sockaddr*)dst, sizeof(*dst)) == -1){
			perror("demande RRQ");
			return -1;
			exit(1);
	}
    //attente de connexion
	int service;
	while ((service = accept(socket, NULL, NULL)) != -1) {
		process_connexion(service);
		delay(1000);
		time += 1;
		if(time >= TIMEOUT){
			perror("Invalide TIMEOUT");
			exit(1);
		}
	}
	
	// si c'est différent de CODEOPE_DATA par exemple
	if(recv(socket, *reponse, *replength, 0) == (-1 || CODEOPE_ERROR)){
		perror("reponse");
		return -1;
		exit(1);
	}
	//récupération de l'adresse de l'expéduteur dans connexion
	//connexion = 
	
	//...
	
	return 0;
}
 
/**
 * Envoie une demande de connexion RRQ pour lire le fichier fichier, 
 * sur la socket socket à l'adresse dst.
 * En cas de dépassement du timeout renvoie la demande de connexion, 
 * au plus NB_MAX_ENVOI fois.
 *
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
    
	int time = 0;
	int demandes = 0;
	// demande de connexion RRQ 
	if (sendto(*socket, CODEOPE_RRQ, 0, (struct sockaddr*)dst, sizeof(*dst)) == -1){
			perror("demande RRQ");
			return -1;
			exit(1);
	}
    //attente de connexion
	int service;
	while ((service = accept(socket, NULL, NULL)) != -1) {
		process_connexion(service);
		delay(1000);
		time += 1;
		if(time >= TIMEOUT){
			process_connexion(service);
			demandes += 1;
			if(demandes > NB_MAX_ENVOI){
				perror("trop d envoi de demandes RRQ");
				return -1;
				exit(1);
			}
		}
	}
	
	// si c'est différent de CODEOPE_DATA par exemple
	if(recv(socket, *reponse, *replength, 0) == (-1 || CODEOPE_ERROR)){
		perror("reponse");
		return -1;
		exit(1);
	}
	//récupération de l'adresse de l'expéduteur dans connexion
	//connexion = 
	
	//...
	
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
    int time = 0;
	int demandes = 0;
	// envoi
	if (sendto(*socket, *paquet, CODEOPE_DATA, (struct sockaddr*)dst, paquetlen)) == -1){
			perror("envoi DATA");
			return -1;
			exit(1);
	}
	
	while(recv(socket, *paquet, *paquetlen, 0)!=CODEOPE_ACK){
		delay(1000);
		time += 1;
		if(time > TIMEOUT){
			sendto(*socket, *paquet, CODEOPE_DATA, (struct sockaddr*)dst, paquetlen);
			demandes += 1;
			return 0;
			if(demandes > NB_MAX_ENVOI){
				perror("demandes sans reponses");
				return -1;
				exit(1);
			}
		}
		
	}
	// attente de reception d un paquet de type ACK
	if(recv(socket, *paquet, *paquetlen, 0) > CODEOPE_ACK){
		perror("reponse");
		return -1;
		exit(1);
	}
	if(recv(socket, *paquet, *paquetlen, 0) < CODEOPE_ACK){
		printf("paquets ignores");
	}
	
    return 1;
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
    int time = 0;
	int demandes = 0;
	// envoi
	if (sendto(*socket, *paquet, CODEOPE_ACK, (struct sockaddr*)dst, paquetlen)) == -1){
			perror("envoi ACK");
			return -1;
			exit(1);
	}
	
	while(recv(socket, *paquet, *paquetlen, 0)!=CODEOPE_DATA){
		delay(1000);
		time += 1;
		if(time > TIMEOUT){
			sendto(*socket, *paquet, CODEOPE_ACK, (struct sockaddr*)dst, paquetlen);
			demandes += 1;
			return 0;
			if(demandes > NB_MAX_ENVOI){
				perror("demandes sans reponses");
				return -1;
				exit(1);
			}
		}
		
	}
	// attente de reception d un paquet de type ACK
	if(recv(socket, *paquet, *paquetlen, 0) > CODEOPE_DATA){
		perror("reponse");
		return -1;
		exit(1);
	}
	if(recv(socket, *paquet, *paquetlen, 0) < CODEOPE_DATA){
		printf("paquets ignores");
	}
	
    return 1;
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
    
	int time = 0;
	int demandes = 0;
	// envoi
	if (sendto(*socket, *paquet, CODEOPE_ACK, (struct sockaddr*)dst, paquetlen)) == -1){
			perror("envoi ACK");
			return -1;
			exit(1);
	}
	
	while(recv(socket, *paquet, *paquetlen, 0)!=CODEOPE_DATA){
		delay(1000);
		time += 1;
		if(time > TIMEOUT){
			sendto(*socket, *paquet, CODEOPE_ACK, (struct sockaddr*)dst, paquetlen);
			demandes += 1;
			return 0;
			if(demandes > NB_MAX_ENVOI){
				perror("demandes sans reponses");
				return -1;
				exit(1);
			}
		}
		
	}
	// attente de reception d un paquet de type ACK
	if(recv(socket, *paquet, *paquetlen, 0) > CODEOPE_DATA){
		perror("reponse");
		return -1;
		exit(1);
	}
	if(recv(socket, *paquet, *paquetlen, 0) < CODEOPE_DATA){
		printf("paquets ignores");
	}
	
	return 1;
}

