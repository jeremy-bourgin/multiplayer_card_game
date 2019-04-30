#include "../../include/client/client.h"

#include "../../include/client/struct_ressources.h"
#include "../../include/functions/struct_sign.h"

#include "../../include/functions/constantes.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int signup(struct_ressources* ressources)
{

	/* ------------------ variable declaration ------------------- */

	// socket 
	int int_udp_sock; 

	// server message
	int int_server_reply;

	// error mangement
	int int_recvfrom_result;
	int int_sendto_result;

	// getting the server ip adress from the ressources file
	const char* string_server_ip = ressources->string_server_ip;

	// structure sent to server to the server in order to create an account
	struct_sign *struct_sign_s = malloc(sizeof(struct_sign));
	size_t struct_sign_size = sizeof(*struct_sign_s);

	// server's socket
	struct sockaddr_in  servaddr; 


	// open the file "Key.txt" conatins client login infos
	FILE *file_key = fopen(FILE_PATH_CLIENT, "w+"); 

	// check if the client specified the server address or else connect to a local server
	if(string_server_ip != NULL)
		servaddr.sin_addr.s_addr = inet_addr(string_server_ip); 
	else
		servaddr.sin_addr.s_addr = INADDR_ANY;

	// other socket parameters
	socklen_t len = sizeof(servaddr);
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(SIGNUP_PORT); 


	/* -------------------- socket creation ---------------------- */


	if ((int_udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
	    perror("socket creation failed"); 
	    exit(EXIT_FAILURE); 
	} 


	printf("> entrer votre pseudo :\n");
	scanf("%s", struct_sign_s->name); 


	/* ------------------ getting user's info -------------------- */

	// getting the name and checking if it is greater than 16 caracters
	while(strlen(struct_sign_s->name) > 16)
	{
		printf(">16 caractères max pour le pseudo ^^ :\n");
		scanf("%s", struct_sign_s->name); 
	}

	// generating a random password
	for (int i = 0; i < sizeof(struct_sign_s->pwd) - 1; i++) {
   		struct_sign_s->pwd[i] = (char) (rand() % 126) + 33;
	}
	struct_sign_s->pwd[sizeof(struct_sign_s->pwd)] = '\0';


	/* ---------------- communication with server ---------------- */

	// send the structure
	int_sendto_result = sendto(int_udp_sock, (struct_sign *)struct_sign_s, struct_sign_size, 0, (struct sockaddr *) &servaddr, len);

	// error managment
	if (int_sendto_result < 0)
	{
		perror("Sendto Signup error 1");
		exit(EXIT_FAILURE);
	} 
	 
	// recieve server's response
	int_recvfrom_result = recvfrom(int_udp_sock, (int*)&int_server_reply, sizeof(int_server_reply), 0, (struct sockaddr *) &servaddr, &len);

	// error management
	if(int_recvfrom_result < 0)
	{
		perror("Recvfrom Signup error 1");
		exit(EXIT_FAILURE);
	} 

	// while the pseudo entered already exist 
	while (int_server_reply == TMS_SIGNUP_ERROR)
	{  
		printf("> pseudo existe, entrer un autre pseudo :\n");
		scanf("%s", struct_sign_s->name);


		int_sendto_result = sendto(int_udp_sock, (struct_sign *)struct_sign_s, struct_sign_size, 0, (struct sockaddr *) &servaddr, len);

		if (int_sendto_result < 0)
		{
			perror("Sendto Signup error 2");
			exit(EXIT_FAILURE);
		} 
		     

		int_recvfrom_result = recvfrom(int_udp_sock, (int*)&int_server_reply, sizeof(int_server_reply), 0, (struct sockaddr *) &servaddr, &len);
		     

		if(int_recvfrom_result  < 0)
		{
			perror("Recvfrom Signup error 2");
			exit(EXIT_FAILURE);
		} 

	}


	printf("> compte crée\n");
	printf("> bienvenue dans notre jeu\n"); 

	/* ---------------------------------------------------------- */ 
	
	// save user's info in "Key.txt"
	fprintf(file_key,"%s\n",struct_sign_s->name);
	fprintf(file_key,"%s\n",struct_sign_s->pwd);
  
	/* ---------------------------------------------------------- */ 

	if(close(int_udp_sock)==-1)
    	perror("close");

    fclose(file_key);
	return 0;
}