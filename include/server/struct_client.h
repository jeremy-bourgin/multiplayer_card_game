/*  - int_accept > file descriptor (result of accept in server.c) for the client
 *  - int *cards > array of integer which represent the cards that have the client
 *	- size_t *nb_cards > represent the number of cards that the client have (max : NB_CARDS)
 */

#ifndef HF_STRUCT_CLIENT
#define HF_STRUCT_CLIENT

#include <stdbool.h>
#include <stddef.h>
#include "../functions/constantes.h"

struct struct_client {
	bool is_authentified;
	bool in_round;
	bool make_wait_round_vcond;
	char username[NAME_LENGTH + 1];
	int cards[NB_CARDS];
	size_t nb_cards;
} typedef struct_client;

#endif
