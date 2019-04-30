#include "../../include/functions/struct_dcm.h"

#include "../../include/client/struct_client.h"

#include "../../include/functions/tcp.h"


#include <stdio.h>
#include <string.h>

int print_dcm (struct_client* struct_client_play)
{
	//geting the socket
	int int_sock = struct_client_play->int_sock;

	//structure that contains the drawing card message
	struct_dcm drawing_client;

	// recieve the structure
	if(recv_full(int_sock, &drawing_client, sizeof(drawing_client)) < 0)
	{
		puts("Recv Draw card failed");
	 	return 1;
	 }
	
	 //check if this player is the one who wctually draw the card or else we didnt show the message
	if(strcmp(struct_client_play->username,drawing_client.username) != 0)
	{
		printf("\n> Le joueur %s a pioché une carte \n",drawing_client.username);
		return 1;
	}
	
	return 0;
}