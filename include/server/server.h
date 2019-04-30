#ifndef HS_SERVER
#define HS_SERVER

#include "ressources.h"

void client(int int_accept);
void server_error(struct_server_ressources *r);
void* game_round(void* arg);
void* signup(void* arg);
void* ranking(void* arg);

#endif
