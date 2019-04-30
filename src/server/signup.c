#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../include/functions/constantes.h"
#include "../../include/functions/struct_sign.h"
#include "../../include/server/server.h"

void* signup(void* arg)
{
	/* ------------------ variable declaration ------------------ */
	
	// boolean declaration
	bool bool_finded;
	bool bool_error;
	
	// int declaration
	int int_sock;
	int int_binded;
	int int_recv;
	int int_value;
	int int_send;
	
	// FILE declaration
	FILE *file_wopen;
	
	// struct struct_signup declaration
	struct_sign signup_info;
	
	// struct sockaddr_in declaration
	struct sockaddr_in sockaddr_bind;
	struct sockaddr_in sockaddr_client;
	
	// size_t declaration
	size_t sizet_signup_size;
	size_t sizet_value;
	size_t sizet_char_size;
	size_t sizet_name_len;
	size_t sizet_pwd_len;
	
	// socklen_t declaration
	socklen_t socklent_bind_size;
	socklen_t socklent_client_size;
	
	/* ----------------------------------------------------------- */
	
	// calc size of variables
	sizet_value = sizeof(int_value);
	sizet_signup_size = sizeof(signup_info);
	socklent_bind_size = sizeof(sockaddr_bind);
	socklent_client_size = sizeof(sockaddr_client);
	sizet_char_size = sizeof(char);
	
	// memory allocation
	memset(&signup_info, 0, sizet_signup_size);
	memset(&sockaddr_bind, 0, socklent_bind_size);
	memset(&sockaddr_client, 0, socklent_client_size);
	
	/* ----------------- creation of the socket ------------------ */
	
	int_sock = socket(PF_INET, SOCK_DGRAM, 0);
	
	if (int_sock == -1)
	{
		perror("Socket error : ");
		close(int_sock);
		return NULL;
	}
	
	// params for binding
	sockaddr_bind.sin_family = AF_INET; // ipv4
	sockaddr_bind.sin_addr.s_addr = INADDR_ANY; // accept everybody
	sockaddr_bind.sin_port = htons(SIGNUP_PORT); // random free port
	
	int_binded = bind(int_sock, (struct sockaddr *)&sockaddr_bind, socklent_bind_size);
	
	if (int_binded == -1)
	{
		perror("Binding error : ");
		close(int_sock);
		return NULL;
	}
	
	/* ---------------------- write file ----------------------- */
	
	file_wopen = fopen(FILE_PATH_MEMBERS, "a");
	
	if (file_wopen == NULL)
	{
		perror("File error");
		close(int_sock);
		return NULL;
	}
	
	/* ------------------ client treatment -------------------- */
	
	bool_error = false;
	
	do
	{

		int_recv = recvfrom(int_sock, &signup_info, sizet_signup_size, 0, (struct sockaddr*)&sockaddr_client, &socklent_client_size);

		if (int_recv == -1)
		{
			bool_error = true;
		}

		check_login(&signup_info, &bool_finded, NULL);

		if (bool_finded)
		{
			// if identifiant already exist send an error
			int_value = TMS_SIGNUP_ERROR;
		}
		else
		{
			sizet_name_len = strlen(signup_info.name);
			sizet_pwd_len = strlen(signup_info.pwd);
			
			fwrite(signup_info.name, sizet_char_size, sizet_name_len, file_wopen);
			fwrite("\n", sizet_char_size, 1, file_wopen);
			fwrite(signup_info.pwd, sizet_char_size, sizet_pwd_len, file_wopen);
			fwrite("\n", sizet_char_size, 1, file_wopen);
			fflush(file_wopen);
			
			int_value = TMS_SIGNUP_SUCCESS;
		}

		int_send = sendto(int_sock, &int_value, sizet_value, 0, (struct sockaddr*)&sockaddr_client, socklent_client_size);

		if (int_send == -1)
		{
			bool_error = true;
		}
	} while (!bool_error);

	fclose(file_wopen);
	close(int_sock);
	perror("SignUP : ");
	
	return NULL;
}
