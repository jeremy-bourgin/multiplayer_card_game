#include "../../include/client/client.h"

#include <stdio.h>

void print_cmd ()
{
	printf("> voici la liste de commande : \n");
    printf("   - pour voir vos cartes tapez    : show\n");
    printf("   - pour jouer une carte tapez    : play\n");
    printf("   - pour piocher une carte tapez  : draw\n");
    printf("   - pour voir le classement tapez : rank\n");
    printf("   - pour voir les commandes tapez : help\n");
    printf("   - pour déconnecter de la partie : logout\n\n");
}