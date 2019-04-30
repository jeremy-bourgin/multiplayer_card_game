#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../include/functions/constantes.h"
#include "../../include/functions/tcp.h"
#include "../../include/server/client.h"
#include "../../include/server/ipc.h"

// send a message to his client to inform that another client have drawn a card
void* draw_card_message(void* arg)
{
	struct_client_ressources* ressources = (struct_client_ressources*)arg;
    
	// int declaration
	int int_count;
    int int_lock;
	int int_vcond;
	int int_set;
	int tm_dcm;
    int int_send_tm;
	int int_send_message;
	
	// size_t declaration
	size_t sizet_tm_size;
	size_t sizet_dcm_size;

	// values
	tm_dcm = TMS_DCM;
	sizet_tm_size = sizeof(tm_dcm);
	sizet_dcm_size = sizeof(*(ressources->shm_drawn_card));
	int_count = op_sem_wait_dcm_count(ressources->semaphores);
	
    while (1)
	{
		// wait a person to 
		int_lock = op_sem_dcm_lock(ressources->semaphores);

		// send information to the client
	    int_send_tm = send_full(ressources->int_accept, &tm_dcm, sizet_tm_size);
		int_send_message = send_full(ressources->int_accept, ressources->shm_drawn_card, sizet_dcm_size);
		
		// if all DCM has finished, unlock draw card
		int_vcond = op_sem_wait_dcm_vcond(ressources->semaphores);
		
		// use SEM_UNDO doesn't work correcly
		// so, to set value at "1" again, we need to set manually the semaphore
		// it is essential before the lock
		int_set = op_sem_dcm_set_lock(ressources->semaphores);
		
		if(
			int_send_tm == -1
			|| int_send_message == -1
			|| int_count == -1
			|| int_lock == -1
			|| int_vcond ==-1
			|| int_set == -1
		) {
			client_error(NULL);
		}
	}
}
