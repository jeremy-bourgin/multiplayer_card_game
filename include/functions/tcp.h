#ifndef HF_TCP
#define HF_TCP

#include <stddef.h>

int send_full(int sock, void *buf, size_t len);
int recv_full(int sock, void *buf, size_t len);

#endif
