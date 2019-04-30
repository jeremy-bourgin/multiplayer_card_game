#include <sys/socket.h>
#include <sys/types.h>

#include "../../include/functions/tcp.h"

ssize_t full(int sock, void *buf, size_t len, ssize_t (*function)(int, void*, size_t, int))
{
	/* ------------------ variable declaration ------------------ */
	
	// int delcaration
	int int_f;
	int int_r;
	
	// size_t declaration
	size_t sizet_bytes_sended;
	
	/* -------------------- send all message -------------------- */
	
	sizet_bytes_sended = len;
	int_r = 0;
	
	do
	{
		// send a part of the message
		int_f = function(sock, buf, sizet_bytes_sended, 0);
		
		// if there is an error
		int_r = (int_f == -1) ? -1 : int_r + int_f;
		
		// move the cursor to the next part which will be sended
		buf += int_f;
		
		// it remain (len - bytes_sended) bytes to send
		sizet_bytes_sended -= int_f;
	} while (int_f != len && int_r != -1); // if the message isn't totally sended or if there is an error
	
	return int_r;
}

int send_full(int sock, void *buf, size_t len)
{
	return full(sock, buf, len, (ssize_t (*)(int, void*, size_t, int))send);
}

int recv_full(int sock, void *buf, size_t len)
{
	return full(sock, buf, len, recv);
}
