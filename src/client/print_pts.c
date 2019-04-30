#include "../../include/client/struct_ressources.h"
#include "../../include/functions/struct_points.h"

#include "../../include/functions/tcp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_pts(struct_ressources* ressources)
{
	struct_client* struct_client_play = ressources->player;
	int int_socket = struct_client_play->int_sock;


	size_t size_t_nb_players = -1;


	if(recv_full(int_socket, &size_t_nb_players, sizeof(size_t_nb_players)) < 0)
	{
		puts("Recieve failed");
	}

	struct_points* struct_point_joueurs = malloc(size_t_nb_players * sizeof(struct_points));
	struct_points struct_moi;
	if(recv_full(int_socket, struct_point_joueurs, size_t_nb_players * sizeof(struct_points)) < 0)
	{
		puts("Recieve failed");
	}

	printf("---------------------------------------------------\n");
	printf("scores du round précédent : \n\n");

	for (int i = 0; i < size_t_nb_players; ++i)
	{

		if(strcmp(struct_point_joueurs[i].username, struct_client_play->username) == 0)
		{
			struct_moi = struct_point_joueurs[i];
			continue;
		}

		if(struct_point_joueurs[i].int_pts >= 0)
			printf("  - le joueur %s a gagné %d\n", struct_point_joueurs[i].username, struct_point_joueurs[i].int_pts);
		else
			printf("  - le joueur %s a perdu %d\n", struct_point_joueurs[i].username, (struct_point_joueurs[i].int_pts * -1));
	}

	printf("\n");

	if(struct_moi.int_pts >= 0)
			printf("  - vous avez gagné %d\n", struct_moi.int_pts);
		else
			printf("  - vous avez perdu %d\n", (struct_moi.int_pts * -1));


	printf("---------------------------------------------------\n");	

	free(struct_point_joueurs);
	return 0;
}