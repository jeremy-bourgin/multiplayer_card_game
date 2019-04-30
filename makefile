CC := gcc
FLAGS := -Wall
LDFLAGS := -Wall -lpthread
PRE := -c
COMPILE := -o

SRC_EXT := c
OBJ_EXT := o

BIN_DIR := ./bin
OBJ_DIR := ./obj
SRC_DIR := ./src

CLIENT_SRC_DIR := $(SRC_DIR)/client
CLIENT_SRC_FILE := $(SRC_DIR)/client.$(SRC_EXT)
CLIENT_OBJ_DIR := $(OBJ_DIR)/client

SERVER_SRC_DIR := $(SRC_DIR)/server
SERVER_SRC_FILE := $(SRC_DIR)/server.$(SRC_EXT)
SERVER_OBJ_DIR := $(OBJ_DIR)/server

FUNCTIONS_SRC_DIR := $(SRC_DIR)/functions
FUNCTIONS_OBJ_DIR := $(OBJ_DIR)/functions

CLIENT_SRC := $(wildcard $(CLIENT_SRC_DIR)/*.$(SRC_EXT))
CLIENT_OBJ := $(patsubst $(CLIENT_SRC_DIR)/%.$(SRC_EXT), $(CLIENT_OBJ_DIR)/%.$(OBJ_EXT), $(CLIENT_SRC))

SERVER_SRC := $(wildcard $(SERVER_SRC_DIR)/*.$(SRC_EXT))
SERVER_OBJ := $(patsubst $(SERVER_SRC_DIR)/%.$(SRC_EXT), $(SERVER_OBJ_DIR)/%.$(OBJ_EXT), $(SERVER_SRC))
	
FUNCTIONS_SRC := $(wildcard $(FUNCTIONS_SRC_DIR)/*.$(SRC_EXT))
FUNCTIONS_OBJ := $(patsubst $(FUNCTIONS_SRC_DIR)/%.$(SRC_EXT), $(FUNCTIONS_OBJ_DIR)/%.$(OBJ_EXT), $(FUNCTIONS_SRC))

CLIENT_BIN := $(BIN_DIR)/client
SERVER_BIN := $(BIN_DIR)/server

$(CLIENT_OBJ_DIR)/%.$(OBJ_EXT): $(CLIENT_SRC_DIR)/%.$(SRC_EXT)
	$(CC) $(COMPILE) $@ $(PRE) $< $(FLAGS) 

$(SERVER_OBJ_DIR)/%.$(OBJ_EXT): $(SERVER_SRC_DIR)/%.$(SRC_EXT)
	$(CC) $(COMPILE) $@ $(PRE) $< $(FLAGS) 
	
$(FUNCTIONS_OBJ_DIR)/%.$(OBJ_EXT): $(FUNCTIONS_SRC_DIR)/%.$(SRC_EXT)
	$(CC) $(COMPILE) $@ $(PRE) $< $(FLAGS) 

client: $(FUNCTIONS_OBJ) $(CLIENT_OBJ) $(CLIENT_SRC_FILE)
	$(CC) $^ $(COMPILE) $(CLIENT_BIN) $(LDFLAGS)

server: $(FUNCTIONS_OBJ) $(SERVER_OBJ) $(SERVER_SRC_FILE)
	$(CC) $^ $(COMPILE) $(SERVER_BIN) $(LDFLAGS)
	touch ./bin/server_data/ipc_taj_game

clean:
	find . -name *.o -delete
	find ./bin -name server -delete
	find ./bin -name client -delete

all: client server