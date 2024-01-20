#include "test_utils.h"

static void *
futex_thread_wrap(void *wrap)
{
	thread_ctx_t *w = wrap;
	void (*f)(void *) = w->f;
	void *arg = w->arg;
	futex_unlock(&w->futex);
	f(arg);
	return 0;
}

static void *
spinlock_thread_wrap(void *wrap)
{
	thread_ctx_t *w = wrap;
	void (*f)(void *) = w->f;
	void *arg = w->arg;
	bare_metal_lock_spinlock(&w->spin_lock);
	f(arg);
	return 0;
}

static void *
timedlock_thread_wrap(void *wrap)
{
	thread_ctx_t *w = wrap;
	void (*f)(void *) = w->f;
	void *arg = w->arg;
	bare_metal_lock_timedlock(&w->timed_lock, TIMEOUT);
	f(arg);
	return 0;
}

static void *
sleeplock_thread_wrap(void *wrap)
{
	thread_ctx_t *w = wrap;
	void (*f)(void *) = w->f;
	void *arg = w->arg;
	bare_metal_lock_sleeplock(&w->timed_lock, TIMEOUT, ZZZ);
	f(arg);
	return 0;
}

static void *
trylock_thread_wrap(void *wrap)
{
	thread_ctx_t *w = wrap;
	void (*f)(void *) = w->f;
	void *arg = w->arg;
	bare_metal_lock_trylock(&w->try_lock);
	f(arg);
	return 0;
}

pthread_t
futex_thread_create(void (*f)(void *), void *arg)
{
	pthread_t thr;
	thread_ctx_t w = { f, arg, { MUTEX_LOCKED }, { 0 }, { 0 }, { 0 },
		{ 0 } };
	pthread_create(&thr, 0, futex_thread_wrap, &w);
	futex_lock(&w.futex);
	return thr;
}

pthread_t
spinlock_thread_create(void (*f)(void *), void *arg)
{
	pthread_t thr;
	thread_ctx_t w = { f, arg, { 0 }, { MUTEX_LOCKED, 0 }, { 0 }, { 0 },
		{ 0 } };
	pthread_create(&thr, 0, spinlock_thread_wrap, &w);
	bare_metal_lock_unlock(&w.spin_lock);
	return thr;
}

pthread_t
timedlock_thread_create(void (*f)(void *), void *arg)
{
	pthread_t thr;
	thread_ctx_t w = { f, arg, { 0 }, { 0 }, { MUTEX_LOCKED, 0 }, { 0 },
		{ 0 } };
	pthread_create(&thr, 0, timedlock_thread_wrap, &w);
	bare_metal_lock_unlock(&w.timed_lock);
	return thr;
}

pthread_t
sleeplock_thread_create(void (*f)(void *), void *arg)
{
	pthread_t thr;
	thread_ctx_t w = { f, arg, { 0 }, { 0 }, { 0 }, { MUTEX_LOCKED, 0 },
		{ 0 } };
	pthread_create(&thr, 0, sleeplock_thread_wrap, &w);
	bare_metal_lock_unlock(&w.timed_lock);
	return thr;
}

pthread_t
trylock_thread_create(void (*f)(void *), void *arg)
{
	pthread_t thr;
	thread_ctx_t w = { f, arg, { 0 }, { 0 }, { 0 }, { MUTEX_LOCKED, 0 } };
	pthread_create(&thr, 0, trylock_thread_wrap, &w);
	bare_metal_lock_unlock(&w.try_lock);
	return thr;
}

void
thread_join(pthread_t t)
{
	pthread_join(t, 0);
}
