#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h> 
#include <unistd.h>
#include <time.h>

#include "../include/functions/constantes.h"
#include "../include/server/server.h"
#include "../include/server/ipc.h"

#define MAX_LISTEN 25

void display_ip_address()
{
	/* ------------------ variable declaration ------------------- */
	
	// int declaration
	int int_hostname;
	int int_addrinfo;
	int int_nameinfo;
	
	// string declaration
	char hostname[NI_MAXHOST];
	
	// struct addrinfo declaration
	struct addrinfo *hint;
	struct addrinfo *res;
	
	// struct sockaddr_in declaration
	struct sockaddr_in *in;
	
	/* ----------------------------------------------------------- */
	
	hint = malloc(sizeof(*hint));
	hint->ai_family = AF_INET;
	hint->ai_socktype = SOCK_STREAM;
	
	/* ----------------------------------------------------------- */
	
	int_hostname = gethostname(hostname, NI_MAXHOST);
	
	if (int_hostname == -1)
	{
		free(hint);
		perror("gethostname");
		return;
	}
	
	int_addrinfo = getaddrinfo(hostname, NULL, hint, &res);
	
	if (int_addrinfo == -1)
	{
		freeaddrinfo(hint);
		freeaddrinfo(res);
		perror("getaddrinfo");
		return;
	}
	
	int_nameinfo = getnameinfo(res->ai_addr, res->ai_addrlen, NULL, 0, NULL, 0, 0);
	
	if (int_nameinfo == -1)
	{
		freeaddrinfo(hint);
		freeaddrinfo(res);
		perror("getnameinfo");
		return;
	}
	
	printf("Adresse IP du serveur :\n");
		
	while (res != NULL)
	{
		in = (struct sockaddr_in*)res->ai_addr;
		printf(" - %s\n", inet_ntoa(in->sin_addr));
		res = res->ai_next;
	}
	
	freeaddrinfo(hint);
	freeaddrinfo(res);
}

void server_error(struct_server_ressources *r)
{
	static struct_server_ressources *ressources = NULL;
	
	/* --------------------- initialisation ---------------------- */
	
	if (r != NULL)
	{
		ressources = r;
		
		return;
	}
	
	/* ----------------------------------------------------------- */
	
	printf("\n");	
	printf("Arrêt du server\n");
	
	free_server_ressources(ressources);
	
	exit(EXIT_SUCCESS);
}

void sigint_signal_server(int s)
{
	server_error(NULL);
}

int main()
{
	/* ------------------ variable declaration ------------------- */
	
	// int declaration 
	int int_sock;
	int int_binded;
	int int_listen;
	int int_accept;
	int int_thread_signup;
	int int_thread_ranking;
	int int_thread_round;
	int int_ipc_init;
	
	// pthread_t declaration
	pthread_t pthread_signup;
	pthread_t pthread_ranking;
	pthread_t pthread_round;

	// struct struct_server_ressources declaration
	struct_server_ressources ressources;
	
	// struct sockaddr_in declaration
	struct sockaddr_in sockaddr_bind;
	struct sockaddr_in sockaddr_client;
	
	// pid_t declaration
	pid_t pidt_forked;
	
	// size_t declaration
	size_t sizet_ipc_ressources_size;
	
	// socklen_t declaration
	socklen_t socklent_bind_size;
	socklen_t socklent_client_size;
	
	/* ----------------------------------------------------------- */
	
	// rand
	srand(time(NULL));
	
	// calc size of variables
	socklent_bind_size = sizeof(sockaddr_bind);
	socklent_client_size = sizeof(sockaddr_client);
	sizet_ipc_ressources_size = sizeof(ressources);
	
	// memory allocation
	memset(&sockaddr_bind, 0, socklent_bind_size);
	memset(&sockaddr_client, 0, socklent_client_size);
	memset(&ressources, -1, sizet_ipc_ressources_size);
	
	/* ---------------------- server error ----------------------- */
	
	// just initialisation for static var (not an error)
	server_error(&ressources);
	signal(SIGINT, sigint_signal_server); // CTRL C management (free IPC before close)
	
	/* -------------------- creation of IPC -----------------------*/
	
	int_ipc_init = 0;
	
	int_ipc_init += init_shm_nb_players(&ressources);
	int_ipc_init += init_shm_deck(&ressources);
	int_ipc_init += init_shm_drawn_card(&ressources);
	int_ipc_init += init_msg_round(&ressources);
	int_ipc_init += init_semaphores(&ressources);
	
	if (int_ipc_init < 0)
	{
		server_error(NULL);
	}
	
	/* ---------------- creation of the threads -------------------*/
	
	int_thread_signup = pthread_create(&pthread_signup, NULL, signup, NULL);
	int_thread_ranking = pthread_create(&pthread_ranking, NULL, ranking, NULL);
	int_thread_round = pthread_create(&pthread_round, NULL, game_round, &ressources);
	
	if (int_thread_signup != 0 || int_thread_ranking != 0 || int_thread_round != 0)
	{
		perror("Thread error");
		server_error(NULL);
	}
	
	/* ----------------- creation of the socket ------------------ */
	
	/* begin :  */
	int_sock = socket(PF_INET, SOCK_STREAM, 0);
	
	// creation of the socket : check if there isn't error
	if (int_sock == -1)
	{
		perror("Socket error");
		server_error(NULL);
	}
	
	ressources.int_sock = int_sock;
	
	// params for binding
	sockaddr_bind.sin_family = AF_INET; // ipv4
	sockaddr_bind.sin_addr.s_addr = INADDR_ANY; // accept everybody
	sockaddr_bind.sin_port = htons(PLAY_PORT); // random free port
	
	int_binded = bind(int_sock, (struct sockaddr*)&sockaddr_bind, socklent_bind_size);
	
	if (int_binded == -1)
	{
		perror("Binding error");
		server_error(NULL);
	}

	display_ip_address();
	
	printf("Serveur connecte sur le port : %d\n",PLAY_PORT);
	
	/* ------------------- client management ---------------------- */
	
	int_listen = listen(int_sock, MAX_LISTEN);

	if (int_listen == -1)
	{
		perror("Listen error");
		server_error(NULL);
	}
	
	do
	{
		int_accept = accept(int_sock, (struct sockaddr*)&sockaddr_client, &socklent_client_size);
		
		if (int_accept == -1)
		{
			server_error(NULL);
		}
		
		pidt_forked = fork();
		
		switch(pidt_forked)
		{
			case 0:
				printf("Un joueur a été accepté sur le serveur\n");
				client(int_accept);
				break;
				
			case -1:
				close(int_accept);
				break;
		}
	} while (pidt_forked != 0);
	
	/* ----------------------------------------------------------- */
	
	return 0;
}
