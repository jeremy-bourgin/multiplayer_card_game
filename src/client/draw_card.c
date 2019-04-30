#include "../../include/client/input.h"

#include "../../include/functions/struct_draw_card.h"
#include "../../include/client/struct_client.h"

#include "../../include/functions/constantes.h"

#include "../../include/functions/tcp.h"


#include <stdio.h>
#include <stdlib.h>


int draw_card(struct_client* struct_client_play)
{
	//getting the socket from ressources
	int int_sock = struct_client_play->int_sock;

	//structure that contains drawn card
	struct_draw_card recieved_card;

	//recieve the structure
	if(recv_full(int_sock, &recieved_card, sizeof(recieved_card)) < 0)
	{
		puts("Recv Draw card failed");
		return 1;
	}

	//add the card to the deck and increment the number of cards in player's hand
	struct_client_play->cards[struct_client_play->nb_cards] = recieved_card.int_card;
	struct_client_play->nb_cards++;
	
	printf("> vous avez pioché la carte : %d\n",recieved_card.int_card);
	return 0;
}
