#include "../../include/client/client.h"
#include "../../include/client/input.h"

#include "../../include/client/struct_ressources.h"
#include "../../include/client/struct_client.h"
#include "../../include/functions/struct_put_card.h"

#include "../../include/functions/constantes.h"

#include "../../include/functions/tcp.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void* input (void* arg)
{

	/* ------------------ variable declaration ------------------- */

	// strecture that contains all the ressources
	struct_ressources* ressources = (struct_ressources*)arg;
	// strecture thtat contains player's game infos
	struct_client* struct_client_play = ressources->player;


	//socket
	int int_socket = struct_client_play->int_sock;

	//check if the request is local or is going to be sent to the server
	int int_tm = -1;
	//check if the user have some data to send to the server
	int data = -1;
	//management of terminal's display
	int int_error = 0;

	//command typed by the player
	int int_size_string_commande = 16;
	char string_commande[int_size_string_commande];

	/* --------------- listening to user's input ---------------- */
	

	while(1)
	{
		// read the command
		scanf("%s", string_commande);

		if(strcmp(string_commande, "show") == 0)
		{
			show_cards(struct_client_play);
		}
		else if(strcmp(string_commande, "play") == 0)
		{
			// check if the player can send a request to the server
			if(struct_client_play->state == ST_FREE)
			{
				//check if the player have at least one card in his hand
				if(struct_client_play->nb_cards == 0)
				{
					printf("> vous n'avez pas de cartes dans votre main, il faut piocher une\n");
					int_error = 1;
				}
				else
					// specify that the player have some data to send
					data = 1;
			}

			// specify that the player want to play a card
			int_tm = TMC_PUT_CARD;
		}
		else if(strcmp(string_commande, "draw") == 0)
		{	
			// check if the play can send a request a request to the server && his hand is not full
			if(struct_client_play->state == ST_FREE && struct_client_play->nb_cards == NB_CARDS)
			{
				printf("> vous avez déjà 10 cartes, vous ne pouvez pas piocher plus\n");
				int_error = 1;
			}

			// specify that the player want to draw a card
			int_tm = TMC_DRAW_CARD;
		}
		else if (strcmp(string_commande, "logout") == 0)
		{
			printf("> au revoir :)\n");
			disconnect(NULL);
		}
		else if (strcmp(string_commande, "help") == 0)
		{
			print_cmd();
		}
		else
		{
			printf("> entrez une commande valide \n");
		}


		// warn the player that there is not enough players
		if(struct_client_play->state == ST_WAIT_PLAYERS && int_tm != -1)
		{
			printf("***** il y a pas assez de joueurs, veuillez attendre svp *****\n");
			int_tm = -1;
		}

		// warn the player that there is a round that has not finished yet
		if(struct_client_play->state == ST_WAIT_ROUND && int_tm != -1)
		{
			printf("***** attendez la fin du tour svp *****\n");
			int_tm = -1;
		}

		// print "command>" to clearify to the player that he can enter a command
		if(int_tm == -1 || int_error == 1)
		{
			printf("command > ");
			int_error = 0;
			int_tm = -1;
			continue;
		}



		/* ------- sending the type of request to the server -------- */


		if (int_tm != -1)
		{
			// sending the type message
			if(send_full(int_socket, (int*)&int_tm, sizeof(int_tm)) < 0)
			{
				puts("Send input tm failed");
				return NULL;
			}

			int_tm = -1;
		}


		/* --------- sending the chosen card to the server ---------- */


		if (data == 1)
		{	
			// strecture that contains the chosen card
			struct_put_card chosen_card;

			// getting the chosen card as a result of play_card method
			chosen_card.int_index_card = play_card(struct_client_play);
			
			// sending the card 
			if(send_full(int_socket, &chosen_card, sizeof(chosen_card)) < 0)
			{
				puts("Send play_card failed");
				return NULL;
			}
			
			data = -1;
		}
	}
		

	/* ----------------------------------------------------------- */

	pthread_exit(NULL);
	return 0;
}