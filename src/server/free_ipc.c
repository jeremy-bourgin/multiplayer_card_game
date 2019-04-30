#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>

#include "../../include/server/client.h"
#include "../../include/server/ipc.h"

void free_server_ressources(struct_server_ressources *ressources)
{
	// socket
	if (ressources->int_sock != -1)
	{
		close(ressources->int_sock);
	}
	
	// free SHM
	if (ressources->shm_nb_players != -1)
	{
		shmctl(ressources->shm_nb_players, IPC_RMID, NULL);
	}
	
	if (ressources->shm_deck != -1)
	{
		shmctl(ressources->shm_deck, IPC_RMID, NULL);
	}
	
	if (ressources->shm_drawn_card != -1)
	{
		shmctl(ressources->shm_drawn_card, IPC_RMID, NULL);
	}
	
	if (ressources->shm_points != -1)
	{
		shmctl(ressources->shm_points, IPC_RMID, NULL);
	}
	
	// free MSG
	if (ressources->msg_round != -1)
	{
		msgctl(ressources->msg_round, IPC_RMID, NULL);
	}
	
	// free SEMAPHORE
	if (ressources->semaphores != -1)
	{
		semctl(ressources->semaphores, 0, IPC_RMID);
	}
}

void free_client_ressources(struct_client_ressources *ressources)
{
	// Updates values before free ressources
	if (ressources->shm_nb_players != NULL && ressources->semaphores != -1)
	{
		op_shm_nd_players_sub(ressources->semaphores, ressources->shm_nb_players, NULL);
		// it depend on number of players
		op_sem_wait_dcm_vcond(ressources->semaphores);
	}
	
	if (ressources->cl->in_round && ressources->msg_round != -1)
	{
		leave_round(ressources->msg_round, ressources->cl);
	}
	
	if (ressources->cl->make_wait_round_vcond && ressources->semaphores != -1)
	{
		op_sem_wait_round_vcond(ressources->semaphores);
	}
	
	// socket
	if (ressources->int_accept != -1)
	{
		close(ressources->int_accept);
	}
	
	// free SHM
	if (ressources->shm_nb_players != NULL)
	{
		shmdt(ressources->shm_nb_players);
	}
	
	if (ressources->shm_deck != NULL)
	{
		shmdt(ressources->shm_deck);
	}
	
	if (ressources->shm_drawn_card != NULL)
	{
		shmdt(ressources->shm_drawn_card);
	}
	
	if (ressources->shm_points != NULL)
	{
		shmdt(ressources->shm_points);
	}
}
