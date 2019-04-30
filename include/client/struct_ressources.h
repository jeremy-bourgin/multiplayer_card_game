#include "struct_client.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef HF_RESSOURCES
#define HF_RESSOURCES

struct struct_ressources {
	struct_client *player;
	FILE *file_key;
	char* string_line;
	const char* string_server_ip;
	
} typedef struct_ressources;

#endif
