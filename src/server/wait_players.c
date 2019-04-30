#include "../../include/server/ipc.h"
#include "../../include/functions/tcp.h"
#include "../../include/server/client.h"
#include <stdbool.h>
#include <stdio.h>

void wait_players(int int_accept, int int_sem, size_t sizet_nb_players, bool is_init)
{
	/* ------------------ variable declaration ------------------- */
	
	// int declaration
	int int_vcond;
	int int_zcond;
	int int_send;
	int int_send2;
	int int_message;
	
	// size_t declaration
	size_t sizet_message_size;
	
	/* ---------------------------------------------------------- */
	
	int_send = 0;
	int_send2 = 0;
	int_vcond = 0;
	int_zcond = 0;
	
	/* ---------------------------------------------------------- */
	
	int_message = TMS_WAIT_PLAYER;
	sizet_message_size = sizeof(int_message);
	
	/* ---------------------------------------------------------- */
	
	if (!is_init && sizet_nb_players == MIN_PLAYERS)
	{
		return;
	}
	
	if (sizet_nb_players <= MIN_PLAYERS)
	{
		// inform the client that he's bloqued
		int_send = send_full(int_accept, &int_message, sizet_message_size);
		// wait other players
		int_vcond = op_sem_wait_player_vcond(int_sem);
		int_zcond = op_sem_wait_player_zcond(int_sem);
		// inform the client that he's already not bloqued
		int_send2 = send_full(int_accept, &int_message, sizet_message_size);
	}
	else if (is_init)
	{
		// wait end of the round before begin to play
		wait_round(int_accept, int_sem);
	}
	
	if (
		int_vcond == -1
		|| int_zcond == -1
		|| int_send == -1
		|| int_send2 == -1
	) {
		client_error(NULL);
	}
}
