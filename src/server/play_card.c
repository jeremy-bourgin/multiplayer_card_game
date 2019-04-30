#include <sys/msg.h>
#include <stdbool.h>
#include <string.h>

#include "../../include/functions/constantes.h"
#include "../../include/functions/tcp.h"
#include "../../include/functions/struct_put_card.h"
#include "../../include/server/client.h"
#include "../../include/server/ipc.h"

void play_card(struct_client_ressources *ressources)
{
	/* ------------------ variable declaration ------------------- */
	
	// int declaration
	int int_accept;
	int int_sem;
	int int_msg_round;
	int int_tm;
	int int_recv;
	int int_send;
	int int_msg;
	int int_index_card;
	int int_card;
	int int_nbplayer_lock;
	int int_nbplayer_unlock;
	int int_wait_vcond;
	
	// bool declaration
	bool bool_pc_error;
	
	// struct_client declaration
	struct_client* cl;
	
	// struct_put_card declaration
	struct_put_card put_card;
	
	// struct_play declaration
	struct_play play;
	
	// size_t declaration
	size_t sizet_nb_card;
	size_t sizet_tm_size;
	size_t sizet_put_card_size;
	size_t sizet_play_size;
	size_t sizet_nb_players;
	
	/* ---------------------------------------------------------- */
	
	int_accept = ressources->int_accept;
	int_sem = ressources->semaphores;
	int_msg_round = ressources->msg_round;
	cl = ressources->cl;
	
	/* ---------------------------------------------------------- */
	
	sizet_nb_card = cl->nb_cards;
	sizet_tm_size = sizeof(int_tm);
	sizet_put_card_size = sizeof(put_card);
	sizet_play_size = sizeof(play);
	
	/* ------------------------ recieve ------------------------- */
	
	int_recv = recv_full(int_accept, &put_card, sizet_put_card_size);
	int_index_card = put_card.int_index_card;
	
	/* -------------------------- check ------------------------- */
	
	bool_pc_error = false;
	// if the player try to put card that he does'n have
	bool_pc_error = bool_pc_error || (int_index_card >= sizet_nb_card);
	
	/* -------------------------- send -------------------------- */
	
	// if the player has already played a card
	int_tm = (bool_pc_error) ? TMS_PUT_CARD_ERROR : TMS_PUT_CARD_SUCCESS;
	int_send = send_full(int_accept, &int_tm, sizet_tm_size);
	
	if (bool_pc_error)
	{
		if (int_send == -1 || int_recv == -1)
		{
			client_error(NULL);
		}
		
		return;
	}
	
	/* ------------------------- values ------------------------ */
	
	// get the card
	int_card = cl->cards[int_index_card];
	// set values to sent
	play.is_leaved = false;
	play.int_card = int_card;
	play.nb_cards = sizet_nb_card;
	strcpy(play.username, cl->username);
	
	/* ------------------------ put card ----------------------- */
	
	cl->cards[int_index_card] = cl->cards[sizet_nb_card - 1];
	--(cl->nb_cards);
	
	int_msg = msgsnd(int_msg_round, &play, sizet_play_size, 0);
	
	/* --------------------------------------------------------- */
	
	if (int_msg == -1)
	{
		client_error(NULL);
	}
	
	cl->in_round = false;
	// wait the end of the round
    wait_round(int_accept, int_sem);
	// when the round is over
	end_round(int_accept, int_sem, ressources->shm_nb_players);
	
	/* --------------------- wait players --------------------- */
	
	
	int_nbplayer_lock = op_sem_nb_players_lock(int_sem);
	sizet_nb_players = ressources->shm_nb_players->count;
	int_nbplayer_unlock = op_sem_nb_players_unlock(int_sem);
	
	if (int_nbplayer_lock == -1 || int_nbplayer_unlock == -1)
	{
		client_error(NULL);
	}
	
	// if there are less of 3 people, wait...
	wait_players(int_accept, int_sem, sizet_nb_players, false);
	

	/* --------------------- game round ---------------------- */
	
	// game round : wait after "wait players" 
	int_wait_vcond = op_sem_wait_round_vcond(int_sem);
	cl->make_wait_round_vcond = false;
	
	if (int_wait_vcond == -1)
	{
		client_error(NULL);
	}
}
