#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "../../include/functions/tcp.h"
#include "../../include/functions/constantes.h"
#include "../../include/functions/struct_draw_card.h"
#include "../../include/server/client.h"
#include "../../include/server/ipc.h"

/* this function mix the cards of the deck for new tour*/

void mix_card(int *deck)
{
	int r, temp;
	
	for (int i = 0; i < NB_DECK; ++i)
	{
		r = rand () % NB_DECK;
		temp = deck[i];
		deck[i] = deck[r];
		deck[r] = temp;
	}
}

/* 
 * (see include/functions/constantes.h)
 * (see include/functions/struct_client.h)
 * 
 * parameter : 
 *	- int_accept > file descriptor (result of accept in server.c) for the client
 *  - int_pipe > file descriptor to send a message to the parent to indicate that a card was drawn
 *  - int *deck
 *		> array of integer (each integer represent a card) to represent the deck
 *		> the deck is a stack
 *  - size_t *cursor_deck
 *		> pointer to an integer value (isn't an array, just a pointer for a value)
 *		> represent the index of the top of the stack (max : NB_DECK)
 * 
 * description :
 *  - check if client can take a card (if *(cursor_cards) < NB_CARDS)
 *	- if he can't draw a card inform the client (send TM_INFORMATION then the message), and return
 *	- take a card at the top of the deck
 *  - put the card in the cards of client
 *	- send to the client that he will receive a card (TM_DRAW_CARD) then send the card
 * 
 * return :
 *	- 0 if there isn't error
 *  - -1 if there are an error (WARNING : if you return -1, the client will be disconnected)
 */
void draw_card(struct_client_ressources *ressources)
{
	/* ------------------ variable declaration ------------------ */
	
	// int declaration
	int tm_success;
	int tm_error;
	int int_drawn_card;
	int int_send_tm;
	int int_send_draw_card;
	int int_lock;
	int int_unlock;
	int int_unlock_all;
	int int_zcond;
	int int_set;
	
	// int array declaration
	int *deck;
	
	// struct_client declaration
	struct_client* cl;

	// struct_draw_card declaration
	struct_draw_card draw_card;
	
	// size_t declaration
	size_t sizet_success_size;
	size_t sizet_error_size;
	size_t sizet_card_size;
	size_t sizet_deck_cursor;
	size_t sizet_card_cursor;
	
	/* ------------------------- lock --------------------------- */
	
	// just one player at the time
	int_lock = op_sem_draw_card_lock(ressources->semaphores);
		
	/* ------------------------ values -------------------------- */
	
	cl = ressources->cl;
	
	// Type Message
	tm_success = TMS_DRAW_CARD_SUCCESS;
	tm_error = TMS_DRAW_CARD_ERROR;
	
	// calc size
	sizet_success_size = sizeof(tm_success);
	sizet_error_size = sizeof(tm_error);
	sizet_card_size = sizeof(draw_card);
	
	// get values
	deck = ressources->shm_deck->deck;
	sizet_deck_cursor = ressources->shm_deck->cursor;
	sizet_card_cursor = cl->nb_cards;
	
	/* ----------------------- draw card ------------------------ */
	
	// mix deck
	if (ressources->shm_deck->cursor == 0)
	{
		ressources->shm_deck->cursor = NB_DECK;
		mix_card(deck);
	}
	
	if (cl->nb_cards < NB_CARDS) // if he've less 10 cards
	{		
		// draw card in deck
		int_drawn_card = deck[sizet_deck_cursor];
		--(ressources->shm_deck->cursor);
		
		// put card 
		cl->cards[sizet_card_cursor] = int_drawn_card;
		++(cl->nb_cards);
		draw_card.int_card = int_drawn_card;
		
		// send TM
		int_send_tm = send_full(ressources->int_accept, &tm_success, sizet_success_size);
		// send card
		int_send_draw_card = send_full(ressources->int_accept, &draw_card, sizet_card_size);

		// write in SHM who has drawn a card
		strcpy(ressources->shm_drawn_card->username, cl->username);

		// unlock draw card message
		int_unlock_all = op_sem_dcm_unlock_all(ressources->semaphores);

		// wait that all "draw card message" has finished
		int_zcond = op_sem_wait_dcm_zcond(ressources->semaphores);
		
		// use SEM_UNDO doesn't work correcly
		// so, to set value at "nb players", we need to set manually the semaphore
		// it is essential to drawn a card again
		int_set = op_sem_wait_dcm_set_lock(ressources->semaphores, ressources->shm_nb_players);
	}
	else
	{
		// send error : "impossible to draw card, you have already 10 cards"
		int_send_draw_card = send_full(ressources->int_accept, &tm_error, sizet_error_size);
	}
	
	int_unlock = op_sem_draw_card_unlock(ressources->semaphores);
	
	if (
		int_send_tm == -1
		|| int_send_draw_card == -1
		|| int_unlock_all ==-1
		|| int_zcond == -1
		|| int_set == -1
		|| int_lock == -1
		|| int_unlock == -1
	) {
		client_error(NULL);
	}
}
