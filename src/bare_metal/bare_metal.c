#include <unistd.h>

#include "bare_metal.h"

int
bare_metal_lock_spinlock(bare_metal_lock_t *lock)
{
	do {
		while (atomic_load_explicit(&lock->l, memory_order_relaxed))
			;
	} while (atomic_exchange_explicit(&lock->l, MUTEX_LOCKED,
	    memory_order_acquire));

	return 0;
}

int
bare_metal_lock_timedlock(bare_metal_lock_t *lock, ul_t timeout)
{
	ul_t te;

	te = get_time(&lock->t) + timeout;
	while (1) {
		if (get_time(&lock->t) > te)
			return -1;

		if (atomic_load_explicit(&lock->l, memory_order_relaxed))
			continue;

		if (!atomic_exchange_explicit(&lock->l, MUTEX_LOCKED,
			memory_order_acquire))
			return 0;
	}
}

int
bare_metal_lock_sleeplock(bare_metal_lock_t *lock, ul_t timeout,
    ui_t sleep_duration)
{
	ul_t start_time = get_time(&lock->t), end_time = start_time + timeout;

	while (1) {
		if (!atomic_exchange_explicit(&lock->l, MUTEX_LOCKED,
			memory_order_acquire)) {
			return 0;
		}

		if (get_time(&lock->t) >= end_time) {
			return -1;
		}

		usleep(sleep_duration);
	}
}

int
bare_metal_lock_trylock(bare_metal_lock_t *lock)
{
	if (!atomic_exchange_explicit(&lock->l, MUTEX_LOCKED,
		memory_order_acquire))
		return 0;

	return -1;
}

int
bare_metal_lock_unlock(bare_metal_lock_t *lock)
{
	atomic_store_explicit(&lock->l, MUTEX_UNLOCKED, memory_order_release);

	return 0;
}
