#ifndef HC_INPUT  
#define HC_INPUT

#include "struct_client.h"


#include <pthread.h>


int draw_card(struct_client* struct_client_play);
int play_card(struct_client* struct_client_play);
int ranking(char* username);
int show_cards(struct_client* struct_client_play);
int print_dcm(struct_client* struct_client_play);
#endif 