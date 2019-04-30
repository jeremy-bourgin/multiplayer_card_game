#ifndef HF_SIGNUP
#define HF_SIGNUP

#include <stdbool.h>
#include "constantes.h"

struct struct_sign {
	char name[NAME_LENGTH + 1];
	char pwd[PWD_LENGTH + 1];
} typedef struct_sign;

void check_login(struct_sign *search, bool *bool_login_finded, bool *bool_password_checked);

#endif
