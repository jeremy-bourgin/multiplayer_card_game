#ifndef HS_CLIENT
#define HS_CLIENT

#include <stddef.h>
#include "ressources.h"
#include "struct_client.h"

// error
void client_error(struct_client_ressources* ressources);

// threads
void* draw_card_message(void* arg);

// authentification
int login(int int_accept, struct_client* cl);

// actions
void leave_round(int int_msg_round, struct_client* cl);
void wait_players(int int_accept, int int_sem, size_t sizet_nb_players, bool is_init);
void mix_card(int *deck);
void draw_card(struct_client_ressources *ressources);
void wait_round(int int_accept, int int_sem);
void play_card(struct_client_ressources *ressources);
void end_round(int int_accept, int int_sem, struct_nb_players *nb_players);

#endif
