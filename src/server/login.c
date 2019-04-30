#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../../include/functions/constantes.h"
#include "../../include/functions/struct_sign.h"
#include "../../include/functions/tcp.h"
#include "../../include/server/client.h"

int login(int int_accept, struct_client* cl)
{
	/* ------------------ variable declaration ------------------ */
	// int declaration
	int int_recv;
	int int_send;
	int value;
	
	// bool declaration
	bool bool_login_finded;
	bool bool_password_checked;
	
	// struct_signup declaration
	struct_sign sign_info;
	
	// size_t declaration
	size_t sizet_sign_size;
	size_t sizet_value_size;
	
	/* ----------------------------------------------------------- */
	
	// calc size of variables
	sizet_sign_size = sizeof(sign_info);
	sizet_value_size = sizeof(value);
	
	// memory allocation
	memset(&sign_info, 0, sizet_sign_size);
	
	/* ---------------------- treatment -------------------------- */
	
	int_recv = recv_full(int_accept, &sign_info, sizet_sign_size);
	
	if (int_recv == -1)
	{
		
		return -1;
	}
	
	check_login(&sign_info, &bool_login_finded, &bool_password_checked);
	
	value = (bool_login_finded && bool_password_checked) ? TMS_LOGIN_SUCCESS : TMS_LOGIN_ERROR;
			
	int_send = send_full(int_accept, &value, sizet_value_size);
	
	if (int_send == -1)
	{
		return -1;
	}
	
	if (value == TMS_LOGIN_SUCCESS)
	{
		cl->is_authentified = true;
		strncpy(cl->username, sign_info.name, NAME_LENGTH + 1);
	}
	
	return 0;
}
