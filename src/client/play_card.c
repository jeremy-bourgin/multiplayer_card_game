#include "../../include/functions/constantes.h"
#include "../../include/client/struct_client.h"
#include "../../include/functions/struct_put_card.h"
#include "../../include/client/input.h"
#include "../../include/functions/tcp.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int play_card(struct_client* struct_client_play)
{

	/* ------------------ variable declaration ------------------- */


	//boolean to check if the card chosen is in the player's hand
	int exist = 0;

	//chosen card by the player
	int int_choice;

	//structure that we use to send the chosen card
	struct_put_card chosen_card;

	//buffer that read from the console
	char* temp_c = malloc(sizeof(char) * 50);

	// show the player's hand in the terminal
	show_cards(struct_client_play);


	/* ------------------- play card procedure ------------------- */


	//while the player has chosen a card that does not exist in his hand
	while (exist == 0)
	{

		printf("choisissez une carte valide : ");

		scanf("%s",temp_c);

		// convert the ward to int
		int_choice = atoi(temp_c);

		//check if the player entered a thing other than an int
		if(int_choice == 0)
			continue;

		//serach for the card and remove it from player's hand
		for (int i = 0; i < struct_client_play->nb_cards; ++i)
		{
			if (struct_client_play->cards[i] != int_choice)
				continue;

			exist = 1;
			struct_client_play->nb_cards--;
			struct_client_play->cards[i] = struct_client_play->cards[struct_client_play->nb_cards];
			chosen_card.int_index_card = i;
			break;
		}
	}

	printf("> vous avez choisi la carte : %d\n",int_choice);

	free(temp_c);

	return chosen_card.int_index_card;
}