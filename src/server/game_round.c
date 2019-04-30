#include <sys/msg.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/functions/constantes.h"
#include "../../include/server/server.h"
#include "../../include/server/ipc.h"

void leave_round(int int_msg_round, struct_client* cl)
{
	/* ------------------ variable declaration ------------------ */
	
	// struct_play declaration
	struct_play leave_play;
	
	// size_t declaration
	size_t sizet_leave_play_size;
	
	/* --------------------------------------------------------- */
	
	sizet_leave_play_size = sizeof(leave_play);
	
	// to finish a round, all player must play a card
	// so if a player leave while a round
	// we need to put a "false" card 
	leave_play.is_leaved = true;
	strcpy(leave_play.username, cl->username);
	msgsnd(int_msg_round, &leave_play, sizet_leave_play_size, 0);
}

void* game_round(void* arg)
{
	/* ------------------ variable declaration ------------------ */
	
	// int declaration
	int int_msg;
	int int_wait_players;
	int int_unlock_wait_players;
	int int_lock_nbplayers;
	int int_unlock_nbplayers;
	int int_wait_player_set;
	int int_wait_round_set;
	int int_get_nb_players;
	int int_init_get_points;
	int int_get_points;
	int int_wait_set;
	int int_unlock_all;
	int int_zcond;
	int int_end_lock;
	int int_end_unlock;
	int int_shmdt;
	int int_rctl;
	
	// struct_server_ressources declaration
	struct_server_ressources *ressources;
	
	// struct_play declaration
	struct_play play;
	
	// struct_nb_players declaration
	struct_nb_players *nb_players;
	
	// struct_points array declaration
	struct_points client_points;
	struct_points *all_points;
	
	// size_t declaration
	size_t sizet_nb_players;
	size_t sizet_count;
	size_t sizet_count_left;
	size_t sizet_play_size;
	
	/* ------------------------- values ------------------------ */
	
	// get arg
	ressources = (struct_server_ressources*)arg;
	
	// get ipc
	int_get_nb_players = get_shm_nb_players(&nb_players);
	
	// calc size
	sizet_play_size = sizeof(play);
	
	sizet_count = 0;
	sizet_count_left = 0;
	
	/* --------------------------------------------------------- */
	
	if (int_get_nb_players == -1)
	{
		server_error(NULL);
	}
	
	/* --------------------------------------------------------- */
	
	while (true)
	{
		/* -------------------- wait player -------------------- */

		if (sizet_count_left == sizet_count)
		{
			// init
			int_wait_players = op_sem_wait_player_zcond(ressources->semaphores);

			if (int_wait_players == -1)
			{
				server_error(NULL);
			}
		}
		
		/* --------------------- nb players -------------------- */
		
		sizet_count = 0;
		sizet_count_left = 0;
		
		int_lock_nbplayers = op_sem_nb_players_lock(ressources->semaphores);
		sizet_nb_players = nb_players->count;
		int_wait_round_set = op_sem_round_set_lock(ressources->semaphores);
		
		if (sizet_nb_players >= MIN_PLAYERS)
		{
			int_unlock_wait_players = op_sem_wait_player_set(ressources->semaphores, true);
		}
		
		int_unlock_nbplayers = op_sem_nb_players_unlock(ressources->semaphores);
		
		// nb players in round
		nb_players->count_round = sizet_nb_players;
		
		if (
			int_unlock_wait_players == -1 
			|| int_wait_round_set == -1
			|| int_lock_nbplayers == -1
			|| int_unlock_nbplayers == -1
		) {
			server_error(NULL);
		}
		
		/* ------------------------ init ---------------------- */
		// init ipc
		int_init_get_points = init_shm_points(ressources, sizet_nb_players);
		// get ipc
		int_get_points = get_shm_points(ressources->semaphores, &all_points, sizet_nb_players);
		// set lock (nb player in the next round)
		int_wait_set = op_sem_wait_round_set_lock(ressources->semaphores, sizet_nb_players);
		// the round can begin
		int_end_unlock = op_sem_wait_round_end_unlock_all(ressources->semaphores);
		
		if (int_init_get_points == -1 || int_get_points == -1 || int_wait_set == -1 || int_end_unlock == -1)
		{
			server_error(NULL);
		}
		
		/* ----------------- get card played  ---------------- */
		
		while (sizet_count < sizet_nb_players)
		{
			// client play card
			int_msg = msgrcv(ressources->msg_round, &play, sizet_play_size, 0, 0);

			if (int_msg == -1)
			{
				server_error(NULL);
			}
						
			/* ------------------ points calc  ------------------ */

			client_points.int_pts = (play.is_leaved) ? 0 : (play.int_card - play.nb_cards);
			client_points.is_winner = true;
			strcpy(client_points.username, play.username);

			/* ----------------------- SHM ---------------------- */

			all_points[sizet_count] = client_points;
			// count nb played cards
			++sizet_count;
			sizet_count_left += (play.is_leaved) ? 1 : 0;
		}
		
		// set wait players semaphore (we need MIN_PLAYERS on server before lauching the round)
		int_wait_player_set = op_sem_wait_player_set(ressources->semaphores, false);
		// set lock to wait before the begin of the next round
		int_end_lock = op_sem_wait_round_end_set_lock(ressources->semaphores);
		// unlock client process for send points
		int_unlock_all = op_sem_round_unlock_all(ressources->semaphores);
		// wait that all client process send points
		int_zcond = op_sem_wait_round_zcond(ressources->semaphores);
		
		// free SHM
		int_shmdt = shmdt(all_points);
		int_rctl = shmctl(ressources->shm_points, IPC_RMID, NULL);
		
		if (
			int_wait_player_set == -1
			|| int_unlock_all == -1
			|| int_zcond == -1
			|| int_end_lock == -1
			|| int_shmdt == -1
			|| int_rctl == -1
		) {
			server_error(NULL);
		}
	}
	
	return NULL;
}
