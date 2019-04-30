#include "../../include/client/client.h"

#include "../../include/client/struct_client.h"
#include "../../include/functions/struct_sign.h"

#include "../../include/functions/constantes.h"

#include "../../include/functions/tcp.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int login(struct_ressources* ressources)
{

	/* ------------------ variable declaration ------------------- */

	// structure that contains user's infos
	struct_client* struct_client_play = ressources->player;

	struct_sign struct_sign_s;

	/* ----------------------------------------------------------- */

	// open the file "Key.txt" conatins client login infos
	FILE* file_key = fopen (FILE_PATH_CLIENT, "r+");

	// parameters for the buffer that read "Key.txt"
	char *string_line = NULL;
	size_t ssize_t_read;
    size_t size_t_len = 0;


	/* ----------------------------------------------------------- */


	// socket
	int int_sock = struct_client_play->int_sock;
	// server reply
	int int_server_reply;


    /* ------- retrieving user's login info from "Key.txt" ------- */

	// getting user's username
    ssize_t_read = getline(&string_line, &size_t_len, file_key); 
    strcpy(struct_sign_s.name, string_line);
    struct_sign_s.name[ssize_t_read - 1] = '\0';
    strcpy(struct_client_play->username, struct_sign_s.name);

    //getting user's password
    ssize_t_read = getline(&string_line, &size_t_len, file_key);
    strcpy(struct_sign_s.pwd, string_line);
    struct_sign_s.pwd[ssize_t_read - 1] = '\0';


    /* -------------- communication with server --------------- */

    // sending the strcuture that contains user's login info
	if(send_full(int_sock , &struct_sign_s , sizeof(struct_sign_s)) < 0)
	{
		free(string_line);
		puts("Send_full Login Error");
		return 1;
	}
	
	// reciving resonse from the server
	if(recv_full(int_sock, &int_server_reply, sizeof(int_server_reply)) < 0)
	{
		free(string_line);
		puts("Recv_full Login Error");
		exit(1);
	}


    /* ------------------- error management -------------------- */


	if (int_server_reply == TMS_LOGIN_ERROR)
	{
		printf(">Votre compte n'existe pas, relancez le programme pour créer un nouveau compte\n");
		file_key = fopen (FILE_PATH_CLIENT, "w+");
		disconnect(NULL);
		exit(1);
	}	


	/* --------------------------------------------------------- */

	//intializing player's card number and state
	struct_client_play->nb_cards = 0;
	struct_client_play->state = ST_FREE;
	

	fclose(file_key);
	return 0;
}