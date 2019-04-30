#ifndef HC_CLIENT
#define HC_CLIENT


#include "struct_client.h"
#include "struct_ressources.h"

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>



int login(struct_ressources* ressources);
int signup(struct_ressources* ressources);
void* input(void* arg);
void* recieve(void* arg);
void disconnect();
void print_cmd();
int print_pts(struct_ressources* ressources);

#endif 