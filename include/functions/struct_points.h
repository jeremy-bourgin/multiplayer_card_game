#ifndef HF_POINTS
#define HF_POINTS

#include <stdbool.h>
#include "constantes.h"

struct struct_points {
	char username[NAME_LENGTH + 1];
	bool is_winner;
	int int_pts;
}typedef struct_points;

#endif
