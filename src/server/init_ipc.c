#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../include/server/client.h"
#include "../../include/server/ipc.h"

int init_shm_nb_players(struct_server_ressources *ressources)
{
	/* ------------------ variable declaration ------------------- */
	
	// key_t declaration
	key_t key;
	
	// int declaration
	int int_shm;
	int int_dt;
	
	// struct declaration
	struct_nb_players *data;
	
	// size_t declaration
	size_t sizet_data_size;
	
	/* ------------------------- IPC ---------------------------- */

	key = ftok(IPC_FILE, IPC_SHM_NB_PLAYERS);
	
	if (key == -1)
	{
		perror("init_shm_nb_players ftok");
		return -1;
	}
	
	sizet_data_size = sizeof(*data);
	int_shm = shmget(key, sizet_data_size, IPC_CREAT|0666);
	
	if (int_shm == -1)
	{
		perror("init_shm_nb_players shmget");
		return -1;
	}
	
	ressources->shm_nb_players = int_shm;
	
	/* ----------------------- Values -------------------------- */
	
	data = (struct_nb_players*)shmat(int_shm, NULL, 0);
	data->count = 0;
	data->count_round = 0;
	
	/* ------------------------- IPC ---------------------------- */
	
	int_dt = shmdt(data);
	
	if (int_dt == -1)
	{
		perror("init_shm_deck shmdt");
		return -1;
	}
	
	return 0;
}

int init_shm_deck(struct_server_ressources *ressources)
{
	/* ------------------ variable declaration ------------------- */
	
	// key_t declaration
	key_t key;
	
	// int declaration
	int int_shm;
	int int_dt;
	
	// struct declaration
	struct_deck *data;
	
	// size_t declaration
	size_t sizet_data_size;
	
	/* ------------------------- IPC ---------------------------- */

	key = ftok(IPC_FILE, IPC_SHM_DECK);
	
	if (key == -1)
	{
		perror("init_shm_deck ftok");
		return -1;
	}
	
	sizet_data_size = sizeof(*data);
	int_shm = shmget(key, sizet_data_size, IPC_CREAT|0666);
	
	if (int_shm == -1)
	{
		perror("init_shm_deck shmget");
		return -1;
	}
	
	ressources->shm_deck = int_shm;
	
	/* ----------------------- Values -------------------------- */
	
	data = (struct_deck*)shmat(int_shm, NULL, 0);
	data->cursor = NB_DECK - 1;
	
	/* ------------------ creation of deck  -------------------- */
	
	data->deck[0] = 1;
	data->deck[1] = 2;
	data->deck[2] = 3;
	data->deck[3] = 4;
	data->deck[4] = 5;
	data->deck[5] = 6;
	data->deck[6] = 7;
	data->deck[7] = 8;
	data->deck[8] = 9;
	data->deck[9] = -2;
	data->deck[10] = -4;
	data->deck[11] = -8;
	data->deck[12] = 16;
	
	mix_card(data->deck);
	
	/* ------------------------- IPC ---------------------------- */
	
	int_dt = shmdt(data);
	
	if (int_dt == -1)
	{
		perror("init_shm_deck shmdt");
		return -1;
	}
	
	return 0;
}

int init_shm_drawn_card(struct_server_ressources *ressources)
{
	/* ------------------ variable declaration ------------------- */
	
	// key_t declaration
	key_t key;
	
	// int declaration
	int int_shm;
	
	// size_t declaration
	size_t sizet_data_size;
	
	/* ------------------------- IPC ---------------------------- */

	key = ftok(IPC_FILE, IPC_SHM_DRAWN_CARD);
	
	if (key == -1)
	{
		perror("init_shm_drawn_card ftok");
		return -1;
	}
	
	sizet_data_size = sizeof(struct_dcm);
	int_shm = shmget(key, sizet_data_size, IPC_CREAT|0666);
	
	if (int_shm == -1)
	{
		perror("init_shm_drawn_card shmget");
		return -1;
	}
	
	ressources->shm_drawn_card = int_shm;
	
	return 0;
}

int init_shm_points(struct_server_ressources *ressources, size_t sizet_nb_players)
{
	/* ------------------ variable declaration ------------------- */
	
	// key_t declaration
	key_t key;
	
	// int declaration
	int int_shm;
	
	// size_t declaration
	size_t sizet_data_size;
	
	/* ------------------------- IPC ---------------------------- */

	key = ftok(IPC_FILE, IPC_SHM_POINTS);
	
	if (key == -1)
	{
		perror("init_shm_points ftok");
		return -1;
	}
	
	sizet_data_size = sizeof(struct_points) * sizet_nb_players;
	int_shm = shmget(key, sizet_data_size, IPC_CREAT|0666);
	
	if (int_shm == -1)
	{
		perror("init_shm_points shmget");
		return -1;
	}
	
	ressources->shm_points = int_shm;
	
	return 0;
}

int init_msg_round(struct_server_ressources *ressources)
{
	/* ------------------ variable declaration ------------------- */
	
	// key_t declaration
	key_t key;
	
	// int declaration
	int int_msg;
	
	/* ------------------------- IPC ---------------------------- */

	key = ftok(IPC_FILE, IPC_MSG_ROUND);
	
	if (key == -1)
	{
		perror("init_msg_round ftok");
		return -1;
	}
	
	int_msg = msgget(key, IPC_CREAT|0666);
	
	if (int_msg == -1)
	{
		perror("init_msg_round msgget");
		return -1;
	}
	
	ressources->msg_round = int_msg;
	
	return 0;
}

int init_semaphores(struct_server_ressources *ressources)
{
	/* ------------------ variable declaration ------------------- */
	
	// key_t declaration
	key_t key;
	
	// int declaration
	int int_sem;
	int int_ctl;
	int int_err;
	
	// Semun declaration
	Semun init_nb_player;
	Semun init_wait_player;
	Semun init_draw_card;
	Semun init_dcm;
	Semun init_wait_dcm;
	Semun init_round;
	Semun init_wait_round;
	Semun init_wait_round_end;

	/* ------------------------- IPC ---------------------------- */
	
	key = ftok(IPC_FILE, IPC_SEMAPHORES);
	
	if (key == -1)
	{
		perror("init_sem_dcm ftok");
		return -1;
	}

	int_sem = semget(key, IPC_NB_SEMAPHORES, IPC_CREAT|0666);
	
	if (int_sem == -1)
	{
		perror("init_sem_dcm semget");
		return -1;
	}

	ressources->semaphores = int_sem;
	
	/* ----------------------- Values -------------------------- */
	
	// nb player lock
	init_nb_player.val = 1;
	// wait player cond
	init_wait_player.val = MIN_PLAYERS;
	// draw card lock
	init_draw_card.val = 1;
	// draw card message lock
	init_dcm.val = 1;
	// wait draw card message cond
	init_wait_dcm.val = 0;
	// round
	init_round.val = 1;
	// wait that all clients process send message
	init_wait_round.val = 0;
	init_wait_round_end.val = 1;

	/* ---------------------- SETVAL ------------------------- */
	
	int_err = 0;
	
	// nb player
	int_ctl = semctl(int_sem, IPC_SEM_NB_PLAYER, SETVAL, init_nb_player);
	int_err = (int_ctl == -1 || int_err == -1) ? -1 : 0;
	
	// wait player
	int_ctl = semctl(int_sem, IPC_SEM_WAIT_PLAYER, SETVAL, init_wait_player);
	int_err = (int_ctl == -1 || int_err == -1) ? -1 : 0;
	
	// draw card
	int_ctl = semctl(int_sem, IPC_SEM_DRAW_CARD, SETVAL, init_draw_card);
	int_err = (int_ctl == -1 || int_err == -1) ? -1 : 0;
	
	// draw card message
	int_ctl = semctl(int_sem, IPC_SEM_DCM, SETVAL, init_dcm);
	int_err = (int_ctl == -1 || int_err == -1) ? -1 : 0;
	
	// wait draw card message
	int_ctl = semctl(int_sem, IPC_SEM_WAIT_DCM, SETVAL, init_wait_dcm);
	int_err = (int_ctl == -1 || int_err == -1) ? -1 : 0;
	
	// round
	int_ctl = semctl(int_sem, IPC_SEM_ROUND, SETVAL, init_round);
	int_err = (int_ctl == -1 || int_err == -1) ? -1 : 0;
	
	// wait that all clients process send message
	int_ctl = semctl(int_sem, IPC_SEM_WAIT_ROUND, SETVAL, init_wait_round);
	int_err = (int_ctl == -1 || int_err == -1) ? -1 : 0;
	
	// wait that all clients process send message
	int_ctl = semctl(int_sem, IPC_SEM_WAIT_ROUND_END, SETVAL, init_wait_round_end);
	int_err = (int_ctl == -1 || int_err == -1) ? -1 : 0;

	if (int_err == -1)
	{
		perror("init_semaphores semctl");
		return -1;
	}
	
	return 0;
}
