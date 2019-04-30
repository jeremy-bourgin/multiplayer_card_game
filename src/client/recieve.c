#include "../../include/client/client.h"
#include "../../include/client/input.h"

#include "../../include/client/struct_ressources.h"
#include "../../include/client/struct_client.h"

#include "../../include/functions/constantes.h"

#include "../../include/functions/tcp.h"


void* recieve (void* arg)
{
	/* ------------------ variable declaration ------------------- */


	// strecture that contains all the ressources
	struct_ressources* ressources = (struct_ressources*)arg;
	// strecture thtat contains player's game infos
	struct_client* struct_client_play = ressources->player;


	//socket
	int int_socket = struct_client_play->int_sock;

	// message sent by the server 
	int int_server_reply;
	size_t server_reply_size;
	server_reply_size = sizeof(int_server_reply);
	

	// boolean for terminal's display management (if == 1 menas that this case is going to display something in the terminal)
	int int_terminal = 0;


	/* ------------- listening to server's messages -------------- */


	while(1)
	{
		// recieve server's message
		if(recv_full(int_socket, &int_server_reply, server_reply_size) < 0)
		{
			puts("Recieve failed");
			pthread_exit(NULL);
		}
		
		switch (int_server_reply)
		{

			case TMS_DRAW_CARD_ERROR :

				printf("\n> une erruer est survennue quand vous avez piocher, essayez une autre fois\n");

				//clearify that the program is gonna dispaly on the terminal
				int_terminal = 1;
				break;

			case TMS_DRAW_CARD_SUCCESS :

				// recieving from the server that i can draw a card and call the method draw_card to draw a card
				draw_card(struct_client_play);

				//clearify that the program is gonna dispaly on the terminal
				int_terminal = 1;
				break;

			case TMS_PUT_CARD_ERROR :

				printf("\n> une erreur est survenue quand vous avez joué  votre carte, essayez une autre fois\n");

				//clearify that the program is gonna dispaly on the terminal
				int_terminal = 1;
				break;

			case TMS_PUT_CARD_SUCCESS :

				printf("> carte joué");
				break;

			case TMS_WAIT_PLAYER :
				
				// change the players state, if he is FREE =>  change it it to WAIT_PLAYERS and vice versa
				if(struct_client_play->state == ST_FREE)
				{
					printf("\n> en attente de joueurs\n");
					struct_client_play->state = ST_WAIT_PLAYERS;
				}
				else
				{
					printf("\n> nombre minimum de joueurs atteint, vous pouvez jouer\n");
					struct_client_play->state = ST_FREE;
				}

				//clearify that the program is gonna dispaly on the terminal
				int_terminal = 1;
				break;

			case TMS_WAIT_ROUND :
				
				printf("\n> attente fin du tour\n");

				//change the player's state to wait for the end of the round
				struct_client_play->state = ST_WAIT_ROUND;

				//clearify that the program is gonna dispaly on the terminal
				int_terminal = 1;
				break;


			case TMS_DCM :

				//clearify that the program is gonna display on the terminal
				int_terminal = print_dcm(struct_client_play);
				break;

			case TMS_ROUND_END :

				printf("\n> tour terminé\n");

				// after the end of the round, change the player state so he can make requestes to the server
				struct_client_play->state = ST_FREE;


				break;

			case TMS_POINTS:
				if(print_pts(ressources) != 0)
					printf("recieve pts_points error\n");

				//clearify that the program is gonna dispaly on the terminal
				int_terminal = 1;
				break;

			case TMS_KICK :

				printf("\n> le serveur vous a sorti de la parite \n");

				// clear the used ressources 
				disconnect(NULL);
				break;

			case TMS_DISCONNECT :

				printf("\n> le serveur a déconnecté \n");

				// clear the used ressources 
				disconnect(NULL);
				break;
		}

		// print "command>" to clearify to the player that he can enter a command
		if(int_terminal == 1)
		{
			printf("command > ");
			int_terminal = 0;
			fflush(stdout);
		}

	}

	pthread_exit(NULL);
}
