#include <sys/syscall.h>

#include <linux/futex.h>
#include <unistd.h>

#include "futex.h"

static void
wait(futex_t *m, short v)
{
	syscall(SYS_futex, m, FUTEX_WAIT, v, 0, 0, 0);
}

static void
wake(futex_t *m)
{
	syscall(SYS_futex, m, FUTEX_WAKE, 1, 0, 0, 0);
}

static int
exchange(futex_t *m, short v)
{
	return atomic_exchange_explicit(&m->l, v, memory_order_seq_cst);
}

static short
cas(futex_t *m, short old, short new)
{
	atomic_compare_exchange_strong_explicit(&m->l, &old, new,
	    memory_order_seq_cst, memory_order_seq_cst);
	return old;
}

void
futex_lock(futex_t *m)
{
	switch (cas(m, MUTEX_UNLOCKED, MUTEX_LOCKED)) {
	case MUTEX_UNLOCKED:
		return;
	case MUTEX_LOCKED:
		cas(m, MUTEX_LOCKED, MUTEX_SLEEPING);
		__attribute__((fallthrough));
	case MUTEX_SLEEPING:
		do {
			wait(m, MUTEX_SLEEPING);
		} while (exchange(m, MUTEX_SLEEPING) != MUTEX_UNLOCKED);
	}
}

void
futex_unlock(futex_t *m)
{
	if (exchange(m, MUTEX_UNLOCKED) == MUTEX_SLEEPING) {
		wake(m);
	}
}
