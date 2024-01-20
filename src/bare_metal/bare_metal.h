#pragma once

#include "../utilities/utilities.h"

typedef struct bare_metal_lock {
	volatile atomic_short l;
	ul_t t;
} bare_metal_lock_t;

int bare_metal_lock_spinlock(bare_metal_lock_t *lock);

int bare_metal_lock_timedlock(bare_metal_lock_t *lock, ul_t timeout);

int bare_metal_lock_sleeplock(bare_metal_lock_t *lock, ul_t timeout,
    ui_t sleep_duration);

int bare_metal_lock_trylock(bare_metal_lock_t *lock);

int bare_metal_lock_unlock(bare_metal_lock_t *lock);
