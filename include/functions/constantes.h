#ifndef HF_CONSTANTES
#define HF_CONSTANTES

// FILES
#define FILE_PATH_CLIENT "client_data/Key.txt"
#define FILE_PATH_MEMBERS "server_data/members.txt"
#define FILE_PATH_RANKING "server_data/ranking.txt"

// GAME INFO
#define MIN_PLAYERS 3
#define NB_DECK 13
#define NB_CARDS 10

// USER
#define NAME_LENGTH 16
#define PWD_LENGTH 16

// SERVER
#define IP_LENGTH 16

// PORT
#define PLAY_PORT 34080
#define SIGNUP_PORT 34081
#define RANKING_PORT 34082

// TMS > TYPE MESSAGE SERVER (sent by server)
#define TMS_SIGNUP_ERROR 0
#define TMS_SIGNUP_SUCCESS 1
#define TMS_LOGIN_ERROR 2
#define TMS_LOGIN_SUCCESS 3
#define TMS_DRAW_CARD_ERROR 4
#define TMS_DRAW_CARD_SUCCESS 5
#define TMS_PUT_CARD_ERROR 6
#define TMS_PUT_CARD_SUCCESS 7
#define TMS_POINTS 8
#define TMS_WAIT_PLAYER 9
#define TMS_WAIT_ROUND 10
#define TMS_DCM 11
#define TMS_ROUND_END 12
#define TMS_KICK 13
#define TMS_DISCONNECT 14

// TMC > TYPE MESSAGE CLIENT (sent by client)
#define TMC_DISCONNECT 0
#define TMC_DRAW_CARD 1
#define TMC_PUT_CARD 2

// IPC
#define IPC_FILE "server_data/ipc_taj_game"
#define IPC_NB_SEMAPHORES 8
// IPC KEYS
#define IPC_SHM_NB_PLAYERS 1
#define IPC_SHM_DECK 2
#define IPC_SHM_DRAWN_CARD 3 // client who drawn card
#define IPC_SHM_POINTS 4
#define IPC_MSG_ROUND 5
#define IPC_SEMAPHORES 7
// SEMAPHORES INDEX
#define IPC_SEM_NB_PLAYER 0
#define IPC_SEM_WAIT_PLAYER 1
#define IPC_SEM_DRAW_CARD 2
#define IPC_SEM_DCM 3 // draw card message
#define IPC_SEM_WAIT_DCM 4 // wait draw card message
#define IPC_SEM_ROUND 5
#define IPC_SEM_WAIT_ROUND 6
#define IPC_SEM_WAIT_ROUND_END 7

//CLIENT STATES
#define ST_FREE 0
#define ST_WAIT_PLAYERS 1 
#define ST_WAIT_ROUND 2

#endif
