#include "../../include/client/struct_ressources.h"

#include "../../include/functions/constantes.h"

#include "../../include/functions/tcp.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void disconnect (struct_ressources* r)
{
	static struct_ressources* ressources = NULL;

	// initialize ressources pointer 
	if (r != NULL)
	{
		ressources = r;

		return;
	}

	// getting the message from constantes.h
	int int_tm = TMC_DISCONNECT;

	//getting the socket
	int int_sock = ressources->player->int_sock;

	//sending to server that the slient has disconnected
	if (ressources->player->is_authentified == true)
	{
		send_full(int_sock, (int*)&int_tm, sizeof(int_tm));
	}
		
	//free different sources
	if(int_sock != -1)
		close(int_sock);

	if(ressources->file_key != NULL)
		free(ressources->file_key);

	if(ressources->string_line != NULL)
		free(ressources->string_line);


	printf("arrêt du Client\n");

	exit(0);
}