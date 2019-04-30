#include <sys/shm.h>
#include <stdbool.h>
#include <stdio.h>

#include "../../include/functions/constantes.h"
#include "../../include/functions/tcp.h"
#include "../../include/server/client.h"
#include "../../include/server/ipc.h"

void end_round(int int_accept, int int_sem, struct_nb_players *nb_players)
{
	int get_points;
	int send;
	int send_tms;
	int send_size;
	int tms_points;
	int int_shmdt;

	struct_points *data;

	size_t sizet_nb_players;
	size_t tms_points_size;
	size_t sizet_data_size;

	sizet_nb_players = nb_players->count_round;
	tms_points = TMS_POINTS;
	
	sizet_data_size = sizeof(*data) * sizet_nb_players;
	tms_points_size = sizeof(tms_points);
	
	get_points = get_shm_points(int_sem, &data, sizet_nb_players);

	if (get_points == -1)
	{
		client_error(NULL);
	}
	
	send_tms = send_full(int_accept, &tms_points, tms_points_size);

	if (send_tms == -1)
	{
		client_error(NULL);
	} 

	send_size = send_full(int_accept, &sizet_nb_players, sizeof(sizet_nb_players));
	
	if (send_size == -1)
	{
		client_error(NULL);
	}
	
	send = send_full(int_accept, data, sizet_data_size);

	if (send == -1)
	{
		client_error(NULL);
	}
	
	int_shmdt = shmdt(data);
	
	if (int_shmdt == -1)
	{
		client_error(NULL);
	}
}