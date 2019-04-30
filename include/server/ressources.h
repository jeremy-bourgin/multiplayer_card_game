#ifndef HS_STRUCT_RESSOURCES
#define HS_STRUCT_RESSOURCES

#include <stdbool.h>
#include <stddef.h>
#include "../functions/constantes.h"
#include "../functions/struct_dcm.h"
#include "../functions/struct_points.h"
#include "struct_client.h"

union Semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
} typedef Semun;

// shm_nb_players
struct struct_nb_players {
	// count all players
	size_t count;
	// count all players in the current round
	size_t count_round;
} typedef struct_nb_players;

// shm_deck
struct struct_deck {
	int deck[NB_DECK];
	size_t cursor;
} typedef struct_deck;

// msg_round
struct struct_play {
	bool is_leaved;
	char username[NAME_LENGTH + 1];
	int int_card;
	size_t nb_cards;
} typedef struct_play;

struct struct_server_ressources {
	int int_sock;
	int shm_nb_players;
	int shm_deck;
	int shm_drawn_card;
	int shm_points;
	int msg_round;
	int semaphores;
} typedef struct_server_ressources;

struct struct_client_ressources {
	int int_accept;
	struct_client* cl;
	struct_nb_players* shm_nb_players;
	struct_deck* shm_deck;
	struct_dcm* shm_drawn_card;
	struct_points* shm_points;
	int msg_round;
	int semaphores;
} typedef struct_client_ressources;

#endif
