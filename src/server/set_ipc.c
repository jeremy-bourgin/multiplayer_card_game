#include <sys/types.h>
#include <sys/sem.h>

#include "../../include/server/ipc.h"

int op_sem_set(int int_sem, int index, int value)
{
	/* ------------------ variable declaration ------------------- */
	
	// int declaration
	int int_ctl;
	
	// Semun declaration
	Semun set;
	
	/* ------------------------ Semaphores ----------------------- */
	
	set.val = value;
	int_ctl = semctl(int_sem, index, SETVAL, set);
	
	if (int_ctl == -1)
	{
		return -1;
	}
	
	return 0;
}

int op_sem_wait_player_set(int int_sem, bool bool_unlocking)
{
	int value = (bool_unlocking) ? 0 : MIN_PLAYERS;
	
	return op_sem_set(int_sem, IPC_SEM_WAIT_PLAYER, value);
}

int op_sem_dcm_set_lock(int int_sem)
{
	return op_sem_set(int_sem, IPC_SEM_DCM, 1);
}

// > RDV : wait all player have "draw card message" before unlock "draw card"
// so, at the end, we need to set semaphore to "nb players"
// NB : when there is a new player, ne need to increment the semaphore (see count)
int op_sem_wait_dcm_set_lock(int int_sem, struct_nb_players *data)
{
	/* ------------------ variable declaration ------------------- */
	
	// int declaration
	int int_lock;
	int int_unlock;
	int int_ctl;
	
	/* ------------------------ Semaphores ----------------------- */
	
	int_lock = op_sem_nb_players_lock(int_sem);
	
	if (int_lock == -1)
	{
		return -1;
	}
	
	int_ctl = op_sem_set(int_sem, IPC_SEM_WAIT_DCM, data->count);
	
	int_unlock = op_sem_nb_players_unlock(int_sem);
	
	if (int_ctl == -1 || int_unlock == -1)
	{
		return -1;
	}
	
	return 0;
}

int op_sem_round_set_lock(int int_sem)
{
	return op_sem_set(int_sem, IPC_SEM_ROUND, 1);
}

int op_sem_wait_round_set_lock(int int_sem, size_t nb_players_round)
{
	// struct_nb_players is already locked
	return op_sem_set(int_sem, IPC_SEM_WAIT_ROUND, nb_players_round);
}

int op_sem_wait_round_end_set_lock(int int_sem)
{
	// struct_nb_players is already locked
	return op_sem_set(int_sem, IPC_SEM_WAIT_ROUND_END, 1);
}
