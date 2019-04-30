#include "../../include/client/input.h"

#include "../../include/client/struct_client.h"

#include "../../include/functions/constantes.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int show_cards(struct_client* struct_client_play)
{
	size_t nb_cards = struct_client_play->nb_cards;

	printf("---------------------------------------------------\n");
	if(nb_cards == 0)
		printf("vous n'avez pas de cartes dans la main\n");
	else
	{
		printf("vos cartes : \n");
		for (int i = 0; i < nb_cards; ++i)
		{
			printf("| %d ",struct_client_play->cards[i]);
		}
		printf("|\n");
		printf("---------------------------------------------------\n");
		printf("nombre de cartes que vous avez dans la main : %zu \n", nb_cards);
	}	
	
	printf("---------------------------------------------------\n");
	return 0;
}