#include "../../include/functions/constantes.h"
#include "../../include/functions/tcp.h"
#include "../../include/server/client.h"
#include "../../include/server/ipc.h"

void wait_round(int int_accept, int int_sem)
{
	/* ------------------ variable declaration ------------------- */
	
	// int declaration
	int int_send1;
	int int_send2;
	int int_lock;
	int int_tm_wait;
	int int_tm_end;
	
	// size_t declaration
	size_t sizet_tm_wait_size;
	size_t sizet_tm_end_size;
	
	/* ---------------------------------------------------------- */
	
	int_tm_wait = TMS_WAIT_ROUND;
	int_tm_end = TMS_ROUND_END;
	
	sizet_tm_wait_size = sizeof(int_tm_wait);
	sizet_tm_end_size = sizeof(int_tm_end);
	
	/* ------------------------- wait -------------------------- */

	// inform client that he need to wait the end of the round
	int_send1 = send_full(int_accept, &int_tm_wait, sizet_tm_wait_size);
	// lock
	int_lock = op_sem_round_lock(int_sem);
	// inform client that the round is over
	int_send2 = send_full(int_accept, &int_tm_end, sizet_tm_end_size);
	
	if (
		int_send1 == -1
		|| int_send2 == -1
		|| int_lock == -1
	) {
		client_error(NULL);
	}
}
