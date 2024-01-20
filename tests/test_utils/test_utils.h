#pragma once

#include <pthread.h>

#include "../../src/bare_metal/bare_metal.h"
#include "../../src/futex/futex.h"

#define TIMEOUT 400000
#define ZZZ 1

#define N (1 << 6)
#define M (1 << 16)
#define X (1 << 3)
#define Y (1 << 8)

typedef struct thread_ctx {
	void (*f)(void *);
	void *arg;
	futex_t futex;
	bare_metal_lock_t spin_lock;
	bare_metal_lock_t timed_lock;
	bare_metal_lock_t sleep_lock;
	bare_metal_lock_t try_lock;
} thread_ctx_t;

pthread_t futex_thread_create(void (*f)(void *), void *arg);
pthread_t spinlock_thread_create(void (*f)(void *), void *arg);
pthread_t timedlock_thread_create(void (*f)(void *), void *arg);
pthread_t sleeplock_thread_create(void (*f)(void *), void *arg);
pthread_t trylock_thread_create(void (*f)(void *), void *arg);
void thread_join(pthread_t t);
