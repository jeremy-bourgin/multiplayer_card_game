#include <sys/types.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include "../../include/functions/constantes.h"
#include "../../include/functions/tcp.h"
#include "../../include/server/server.h"
#include "../../include/server/client.h"
#include "../../include/server/ipc.h"

void client_error(struct_client_ressources* r)
{
	static struct_client_ressources *ressources = NULL;
	
	/* --------------------- initialisation ---------------------- */
	
	if (r != NULL)
	{
		ressources = r;
		
		return;
	}
	
	/* ----------------------------------------------------------- */
	
	int int_tm = TMS_DISCONNECT;
	
	if (ressources->int_accept != -1)
	{
		// inform the client that there is an error
		send_full(ressources->int_accept, &int_tm, sizeof(int_tm));
	}
	
	free_client_ressources(ressources);
	
	printf("Un client s'est déconnecté\n");
	_Exit(EXIT_SUCCESS);
}

void sigint_signal_client(int s)
{
	client_error(NULL);
}

void sigsegv_signal_client(int s)
{
	printf("Problème de mémoire\n");
	client_error(NULL);
}

void client(int int_accept)
{
	/* ------------------ variable declaration ------------------ */
	
	// int declaration
	int int_input;
	int int_ipc_init;
	int int_login;
	int int_thread;
	int int_wait_end;
	int int_recv;
	
	// pthread_t declaration
	pthread_t pthread_draw_card_message;
	
	// struct_client declaration
	struct_client cl;
	
	// struct_server_ressources declaration
	struct_client_ressources ressources;
	
	// struct sigaction declaration
	struct sigaction sig;
	
	// size_t declaration
	size_t sizet_nb_players;
	size_t sizet_input_size;
	size_t sizet_cl_size;
	size_t sizet_ipc_ressources_size;
	
	/* ----------------------------------------------------------- */
	
	// rand
	// have different srand for each fork (getpid()<<16)
	srand(time(NULL) ^ (getpid()<<16));
	
	// calc size of variables
	sizet_input_size = sizeof(int_input);
	sizet_cl_size = sizeof(cl);
	sizet_ipc_ressources_size = sizeof(ressources);
	
	// memory allocation
	memset(&cl, 0, sizet_cl_size);
	memset(&ressources, -1, sizet_ipc_ressources_size);
	memset(&sig, 0, sizet_cl_size);
	
	/* ----------------------- init struct ---------------------- */
	
	// ressources struct
	ressources.int_accept = int_accept;
	ressources.cl = &cl;
	ressources.shm_deck = NULL;
	ressources.shm_drawn_card = NULL;
	ressources.shm_nb_players = NULL;
	ressources.shm_points = NULL;
	
	// client struct
	cl.is_authentified = false;
	cl.in_round = false;
	cl.nb_cards = 0;
	
	/* -------------------------- error  ------------------------ */
	
	client_error(&ressources);
	
	sig.sa_handler = sigint_signal_client;
	sigaction(SIGINT, &sig, NULL);
	signal(SIGSEGV, sigsegv_signal_client);

	/* -------------------------- Login ------------------------- */
	
	int_login = login(ressources.int_accept, &cl);

	if (int_login == -1 || !cl.is_authentified)
	{
		client_error(NULL);
	}
	
	/* --------------------------- IPC  ------------------------- */
		
	int_ipc_init = 0;
	
	int_ipc_init += get_shm_nb_players(&(ressources.shm_nb_players));
	int_ipc_init += get_shm_deck(&(ressources.shm_deck));
	int_ipc_init += get_shm_drawn_card(&(ressources.shm_drawn_card));
	int_ipc_init += get_msg_round(&ressources.msg_round);
	int_ipc_init += get_semaphores(&ressources.semaphores);
		
	if (ressources.semaphores != -1 && ressources.shm_nb_players != NULL)
	{
		// count the new player
		int_ipc_init += op_shm_nd_players_add(ressources.semaphores, ressources.shm_nb_players, &sizet_nb_players);
	}
		
	if (int_ipc_init < 0)
	{
		client_error(NULL);
	}
	
	/* ---------------- creation of the threads ----------------- */
	
	int_thread = pthread_create(&pthread_draw_card_message, NULL, draw_card_message, &ressources);
	
	if (int_thread != 0)
	{
		free_client_ressources(&ressources);
		return;
	}
		
	/* ---------------------------------------------------------- */
	
	wait_players(ressources.int_accept, ressources.semaphores, sizet_nb_players, true);
	
	// round : ask of client
	do
	{
		// wait begin of the next round
		int_wait_end = op_sem_wait_round_end_lock(ressources.semaphores);
		
		if (int_wait_end == -1)
		{
			client_error(NULL);
		}
		
		cl.in_round = true;
		cl.make_wait_round_vcond = true;
		
		int_recv = recv_full(ressources.int_accept, &int_input, sizet_input_size);

		if (int_recv == -1)
		{
			client_error(NULL);
		}

		switch(int_input)
		{
			case TMC_DRAW_CARD:
				draw_card(&ressources);
				break;

			case TMC_PUT_CARD:
				play_card(&ressources);
				break;
		}
	} while(int_input != TMC_DISCONNECT);
			
	free_client_ressources(&ressources);
}
