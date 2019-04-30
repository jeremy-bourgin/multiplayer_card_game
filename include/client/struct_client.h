/*  - int_sock > the socket
 *  - int cards[] > array of integer which represent the cards that have the client
 *	- size_t *nb_cards > represent the number of cards that the client have (max : NB_CARDS)
 *  - state > tell if the client is free to play, already played a card and waiting for 
              the end of the round or waiting for enough players to connect
 */

#ifndef HF_STRUCT_CLIENT
#define HF_STRUCT_CLIENT

#include <stdbool.h>
#include <stddef.h>
#include "../functions/constantes.h"

struct struct_client {
	int int_sock;
	char username[NAME_LENGTH];
	size_t nb_cards;
	int state;
	int cards[NB_CARDS];
	bool is_authentified;
} typedef struct_client;

#endif
