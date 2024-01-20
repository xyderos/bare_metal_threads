#pragma once

#include "../utilities/utilities.h"

typedef struct futex {
	volatile atomic_short l;
} futex_t;

void futex_lock(futex_t *);
void futex_unlock(futex_t *);
