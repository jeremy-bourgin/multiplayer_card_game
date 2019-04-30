#include <sys/types.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../include/server/ipc.h"

int op_sem(int int_sem, unsigned short int_index, short int_op)
{
	/* ------------------ variable declaration ------------------- */
	
	// int declaration
	int int_sem_op;
	
	// sembuf declaration
	struct sembuf *sembuf;
	
	//size_t declaration
	size_t sizet_sembuf_size;
	
	/* --------------------------------------------------------- */
	
	// calc size
	sizet_sembuf_size = sizeof(*sembuf);
	
	// memory allocation
	sembuf = malloc(sizet_sembuf_size);
	
	/* ------------------------- IPC ---------------------------- */
	
	sembuf->sem_num = int_index;
	sembuf->sem_op = int_op;
	sembuf->sem_flg = 0;
	int_sem_op = semop(int_sem, sembuf, 1);
	
	free(sembuf);
	
	return int_sem_op;
}

/* ----------------------- NB PLAYERS SEMAPHORES/SHM -------------------------- */

int op_sem_nb_players_lock(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_NB_PLAYER, -1);
}

int op_sem_nb_players_unlock(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_NB_PLAYER, +1);
}

int op_shm_nd_players_add(int int_sem, struct_nb_players *data, size_t *sizet_nb_players)
{
	/* ------------------ variable declaration ------------------- */
	
	// int declaration
	int int_lock;
	int int_unlock;
	
	/* ---------------------------------------------------------- */

	int_lock = op_sem_nb_players_lock(int_sem);
	
	++(data->count);
	
	if (sizet_nb_players != NULL)
	{
		(*sizet_nb_players) = data->count;
	}

	int_unlock = op_sem_nb_players_unlock(int_sem);

	if (int_lock == -1 || int_unlock == -1)
	{
		return -1;
	}

	return 0;
}

int op_shm_nd_players_sub(int int_sem, struct_nb_players *data, size_t *sizet_nb_players)
{
	/* ------------------ variable declaration ------------------- */
	
	// int declaration
	int int_lock;
	int int_unlock;
	
	/* ---------------------------------------------------------- */
	
	int_lock = op_sem_nb_players_lock(int_sem);
	
	--(data->count);
	
	if (sizet_nb_players != NULL)
	{
		(*sizet_nb_players) = data->count;
	}
	
	int_unlock = op_sem_nb_players_unlock(int_sem);
	
	if (int_lock == -1 || int_unlock == -1)
	{
		return -1;
	}
	
	return 0;
}


/* ------------------------- WAIT PLAYERS SEMAPHORES ---------------------------- */

int op_sem_wait_player_zcond(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_WAIT_PLAYER, 0);
}

int op_sem_wait_player_vcond(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_WAIT_PLAYER, -1);
}

/* ------------------------- DRAW CARD SEMAPHORES ---------------------------- */

int op_sem_draw_card_lock(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_DRAW_CARD, -1);
}

int op_sem_draw_card_unlock(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_DRAW_CARD, +1);
}

/* ------------------------- DCM SEMAPHORES ---------------------------- */

int op_sem_dcm_lock(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_DCM, 0);
}

int op_sem_dcm_unlock_all(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_DCM, -1);
}

/* ------------------------- WAIT DCM SEMAPHORES ---------------------------- */

int op_sem_wait_dcm_zcond(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_WAIT_DCM, 0);
}

int op_sem_wait_dcm_vcond(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_WAIT_DCM, -1);
}

int op_sem_wait_dcm_count(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_WAIT_DCM, +1);
}

/* ------------------------- ROUND SEMAPHORES ---------------------------- */

int op_sem_round_lock(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_ROUND, 0);
}

int op_sem_round_unlock_all(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_ROUND, -1);
}

/* ----------------------- WAIT ROUND SEMAPHORES -------------------------- */

int op_sem_wait_round_cond(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_WAIT_ROUND, 0);
}

int op_sem_wait_round_zcond(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_WAIT_ROUND, 0);
}

int op_sem_wait_round_vcond(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_WAIT_ROUND, -1);
}

/* ----------------------- WAIT ROUND END SEMAPHORES -------------------------- */
int op_sem_wait_round_end_lock(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_WAIT_ROUND_END, 0);
}

int op_sem_wait_round_end_unlock_all(int int_sem)
{
	return op_sem(int_sem, IPC_SEM_WAIT_ROUND_END, -1);
}
