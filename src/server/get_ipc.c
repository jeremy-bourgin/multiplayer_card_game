#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../include/server/ipc.h"

/* ------------------------------------ SHM ------------------------------------ */

void* get_shm(int int_id, size_t sizet_data_size, int *int_r)
{
	/* ------------------ variable declaration ------------------- */
	
	// key_t declaration
	key_t key;
	
	// int declaration
	int int_shm;
	
	/* ------------------------- IPC ---------------------------- */
	
	key = ftok(IPC_FILE, int_id);
	
	if (key == -1)
	{
		(*int_r) = -1;
		return NULL;
	}
	
	int_shm = shmget(key, sizet_data_size, O_RDWR);
	
	if (int_shm == -1)
	{
		(*int_r) = -1;
		return NULL;
	}
	
	(*int_r) = 0;
	return shmat(int_shm, NULL, 0);	
}

int get_shm_nb_players(struct_nb_players **data)
{
	int int_r;
	size_t sizet_data_size = sizeof(**data);
	
	(*data) = get_shm(IPC_SHM_NB_PLAYERS, sizet_data_size, &int_r);
	
	return int_r;
}

int get_shm_deck(struct_deck **data)
{
	int int_r;
	size_t sizet_data_size = sizeof(**data);
	
	(*data) =  get_shm(IPC_SHM_DECK, sizet_data_size, &int_r);
	
	return int_r;
}

int get_shm_drawn_card(struct_dcm **data)
{
	int int_r;
	size_t sizet_data_size = sizeof(**data);
	
	(*data) =  get_shm(IPC_SHM_DRAWN_CARD, sizet_data_size, &int_r);
	
	return int_r;
}

int get_shm_points(int int_sem, struct_points **data, size_t size)
{
	int int_r;
	size_t sizet_data_size = sizeof(**data);
	
	(*data) =  get_shm(IPC_SHM_POINTS, sizet_data_size * size, &int_r);
	
	if (int_r == -1)
	{
		perror("get_shm_points");
		return -1;
	}
	
	return 0;
}

/* ------------------------------------ MSG ------------------------------------ */

int get_msg_round(int *int_msg)
{
	/* ------------------ variable declaration ------------------- */
	
	// key_t declaration
	key_t key;
	
	/* ------------------------- IPC ---------------------------- */
	
	key = ftok(IPC_FILE, IPC_MSG_ROUND);
	
	if (key == -1)
	{
		return -1;
	}
	
	(*int_msg) = msgget(key, O_RDWR);
	
	if ((*int_msg) == -1)
	{
		return -1;
	}
	
	return 0;
}

/* --------------------------------- SEMAPHORE --------------------------------- */

int get_semaphores(int *int_sem)
{
	/* ------------------ variable declaration ------------------- */
	
	// key_t declaration
	key_t key;
	
	/* ------------------------- IPC ---------------------------- */
	
	key = ftok(IPC_FILE, IPC_SEMAPHORES);
	
	if (key == -1)
	{
		return -1;
	}
	
	(*int_sem) = semget(key, IPC_NB_SEMAPHORES, O_RDWR);
	
	if ((*int_sem) == -1)
	{
		return -1;
	}
	
	return 0;
}
