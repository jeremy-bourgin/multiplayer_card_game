#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../include/functions/struct_sign.h"
#include "../../include/functions/constantes.h"

void check_login(struct_sign *search, bool *bool_login_finded, bool *bool_password_checked)
{
	/* ------------------ variable declaration ------------------ */
	
	// FILE declaration
	FILE* file_ropen;
	
	// int declaration
	int int_count;
	
	// boolean declaration
	bool bool_finded;
	
	// string declaration
	char *string_line;
	
	// size_t declaration
	size_t sizet_buff_size;
	
	/* ----------------------------------------------------------- */
	
	(*bool_login_finded) = false;
	
	if (bool_password_checked != NULL)
	{
		(*bool_password_checked) = false;
	}
	
	/* ----------------------------------------------------------- */
	
	string_line = malloc((NAME_LENGTH + 1) * sizeof(*string_line));
		
	/* ---------------------- treatment -------------------------- */
	
	// file where there are members informations
	file_ropen = fopen(FILE_PATH_MEMBERS, "r");
	bool_finded = false;
	
	while (!bool_finded && (int_count = getline(&string_line, &sizet_buff_size, file_ropen)) != -1)
	{
		string_line[int_count  - 1] = '\0';
		
		// check login exist
		if (strcmp(search->name, string_line) == 0)
		{
			(*bool_login_finded) = true;
			bool_finded = true;
		}
		
		// if login exist : check password
		int_count = getline(&string_line, &sizet_buff_size, file_ropen);
		string_line[int_count  - 1] = '\0';
		
		if (
			bool_password_checked != NULL
			&& bool_finded
			&& strcmp(search->pwd, string_line) == 0
		)
		{
			(*bool_password_checked) = true;
		}
	}
	
	fclose(file_ropen);
	free(string_line);
}
