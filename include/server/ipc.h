#ifndef HS_IPC
#define HS_IPC

#include <stdbool.h>
#include "ressources.h"

// init
int init_shm_nb_players(struct_server_ressources *ressources);
int init_shm_deck(struct_server_ressources *ressources);
int init_shm_drawn_card(struct_server_ressources *ressources);
int init_shm_points(struct_server_ressources *ressources, size_t sizet_nb_players);
int init_msg_round(struct_server_ressources *ressources);
int init_semaphores(struct_server_ressources *ressources);

// get
int get_shm_nb_players(struct_nb_players **data);
int get_shm_deck(struct_deck **data);
int get_shm_drawn_card(struct_dcm **data);
int get_shm_points(int int_sem, struct_points **data, size_t size);
int get_msg_round(int *int_msg);
int get_semaphores(int *int_sem);

// free
void free_server_ressources(struct_server_ressources *ressources);
void free_client_ressources(struct_client_ressources *ressources);

// semaphores operations
int op_sem_nb_players_lock(int int_sem);
int op_sem_nb_players_unlock(int int_sem);

int op_sem_wait_player_zcond(int int_sem);
int op_sem_wait_player_vcond(int int_sem);
int op_sem_wait_player_set(int int_sem, bool bool_unlocking);

int op_sem_draw_card_lock(int int_sem);
int op_sem_draw_card_unlock(int int_sem);

int op_sem_dcm_set_lock(int int_sem);
int op_sem_dcm_lock(int int_sem);
int op_sem_dcm_unlock_all(int int_sem);

int op_sem_wait_dcm_set_lock(int int_sem, struct_nb_players *data);
int op_sem_wait_dcm_zcond(int int_sem);
int op_sem_wait_dcm_vcond(int int_sem);
int op_sem_wait_dcm_count(int int_sem);

int op_sem_round_set_lock(int int_sem);
int op_sem_round_lock(int int_sem);
int op_sem_round_unlock_all(int int_sem);

int op_sem_wait_round_set_lock(int int_sem, size_t data);
int op_sem_wait_round_zcond(int int_sem);
int op_sem_wait_round_vcond(int int_sem);

int op_sem_wait_round_end_set_lock(int int_sem);
int op_sem_wait_round_end_lock(int int_sem);
int op_sem_wait_round_end_unlock_all(int int_sem);

// SHM
int op_shm_nd_players_add(int int_sem, struct_nb_players *data, size_t *sizet_nb_players);
int op_shm_nd_players_sub(int int_sem, struct_nb_players *data, size_t *sizet_nb_players);

#endif
