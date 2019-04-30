#include "../include/client/client.h"
#include "../include/client/input.h"

#include "../include/client/struct_ressources.h"
#include "../include/client/struct_client.h"

#include "../include/functions/constantes.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>


void sigint_signal(int s)
{
	disconnect(NULL);
}

int main(int argc, char const *argv[])
{


	srand(time(NULL));

	
	/* ------------------ variable declaration ------------------- */


	// strcture that contains all the pointers
	struct_ressources ressources;
	// structure that contains the client infos
	struct_client struct_client_play;
	// contains server address
	struct sockaddr_in server_login;


	/* ----------------------------------------------------------- */


	// socket 
	int int_sock = -1;


	/* ----------------------------------------------------------- */


	// open the file "Key.txt" conatins client login infos
	FILE *file_key = fopen (FILE_PATH_CLIENT, "r+");
	ressources.file_key = file_key;
	
	// buffer that store lines that we read from "Key.txt" file
 	char *string_line = NULL;
 	ressources.string_line = string_line;

 	//variables for the buffer that reading from "Key.txt" file
 	ssize_t ssize_t_read;
	size_t size_t_len = 0;

 	//number of lines in Key.txt
	int int_nb_lignes = 0;


	/* ----------------------------------------------------------- */


	// threads
	pthread_t pthread_recieve;
	pthread_t pthread_input;


	/* -------------------- error management --------------------- */


	// initialize client info values
	ressources.player = &struct_client_play;
	ressources.player->int_sock = -1;
	ressources.player->state = ST_FREE;
	ressources.player->nb_cards = 0;
	ressources.player->is_authentified = false;

	// initialize other values
	ressources.string_line = NULL;
	ressources.string_server_ip = NULL;

	// call disconnect in case of an error to free all the ressources
	disconnect(&ressources); 
	signal(SIGINT, sigint_signal);


	/* -------------------- socket creation ---------------------- */


	// create socket
	int_sock = socket(AF_INET, SOCK_STREAM , 0);
	struct_client_play.int_sock = int_sock;

	// error management
	if (int_sock == -1)
	{
		printf("Could not create socket \n");
	}

	printf("> tentative de connexion au serveur...\n");
	
	// intialize server address
	if(argc == 2)
	{
		printf("> connexion à l'adresse : %s \n", argv[1]);
		ressources.string_server_ip = argv[1];
		server_login.sin_addr.s_addr = inet_addr(ressources.string_server_ip);

	}
	else
		server_login.sin_addr.s_addr = INADDR_ANY;
	

	server_login.sin_family = AF_INET;
	server_login.sin_port = htons(PLAY_PORT);

	/* -------------------- socket creation ---------------------- */

	if (connect(int_sock, (struct sockaddr *)&server_login , sizeof(server_login)) < 0)
	{
		perror("Connexion error");
		disconnect(NULL);
		return 1;
	}

	printf("> connecté au serveur \n");


	/* -------------------- signup & login ---------------------- */


   	if(!file_key)

   	{	// file "Key.txt" does not exist, so we proced directly to signup
   		if(signup(&ressources) == -1)
   		{
   			printf("probleme de signup \n");
   			disconnect(NULL);
	   		return 1;
   		}
   	}
   	else
	{	
		// check if the file "Key.txt" is corruptesd
	    while ((ssize_t_read = getline(&string_line, &size_t_len, file_key)) != -1) 
	    {
	    	if( ++int_nb_lignes > 2)
	    		break;
	    }

	    free(string_line);

	    // file corrupted and we do a signup
		if (int_nb_lignes != 2 && signup(&ressources) == -1)
		{
			disconnect(NULL);
			return 1;
		}
	}

	// login call and error management
	if(login(&ressources) == -1)
	{
		printf("probleme de login : si le problème persiste, supprimer client_data/Key.txt\n");
		disconnect(NULL);
		return 1;
	}

	if(file_key != NULL)
		fclose(file_key);


	/* -------------------- thread creation and launching ---------------------- */


	ressources.player->is_authentified = true;

    printf("> bienvenue dans la partie\n");
    
    // print diffeernt command of the game
    print_cmd();


	if((pthread_create(&pthread_recieve, NULL, recieve, &ressources)) != 0)
		return 1;

	if((pthread_create(&pthread_input, NULL, input, &ressources)) != 0)
		return 1;



	if((pthread_join(pthread_recieve, NULL)) < 0)
		return 1;

	if((pthread_join(pthread_input, NULL)) < 0)
		return 1;

	
	disconnect(NULL);
	return 0;
}