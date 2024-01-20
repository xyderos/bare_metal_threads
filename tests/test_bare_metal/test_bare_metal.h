#include <check.h>
#include <stdio.h>
#include <unistd.h>

#include "../test_utils/test_utils.h"

Suite *bare_metal_suite(void);

static volatile int bare_metal_count;

static void
spinlock_worker(void *m)
{
	for (int i = 0; i < M; i++) {
		while (bare_metal_lock_trylock(m) != 0) {
		}

		int c = bare_metal_count;
		bare_metal_count = c + 1;
		bare_metal_lock_unlock(m);
	}
}

static void
timedlock_worker(void *m)
{
	for (int i = 0; i < M; i++) {
		bare_metal_lock_timedlock(m, TIMEOUT);
		int c = bare_metal_count;
		bare_metal_count = c + 1;
		bare_metal_lock_unlock(m);
	}
}

static void
sleeplock_worker(void *m)
{
	for (int i = 0; i < M; i++) {
		bare_metal_lock_sleeplock(m, TIMEOUT, ZZZ);
		int c = bare_metal_count;
		bare_metal_count = c + 1;
		bare_metal_lock_unlock(m);
	}
}

static void
trylock_worker(void *m)
{
	for (int i = 0; i < Y; i++) {
		bare_metal_lock_trylock(m);
		int c = bare_metal_count;
		bare_metal_count = c + 1;
		bare_metal_lock_unlock(m);
	}
}

START_TEST(BENCH_SPINLOCK)
{
	futex_t m = { 0 };
	pthread_t thr[N];
	for (int i = 0; i < N; i++) {
		thr[i] = futex_thread_create(spinlock_worker, &m);
	}
	for (int i = 0; i < N; i++) {
		thread_join(thr[i]);
	}
	ck_assert_int_eq(bare_metal_count, N * M);
}
END_TEST

START_TEST(BENCH_TIMEDLOCK)
{
	futex_t m = { 0 };
	pthread_t thr[N];
	for (int i = 0; i < N; i++) {
		thr[i] = futex_thread_create(timedlock_worker, &m);
	}
	for (int i = 0; i < N; i++) {
		thread_join(thr[i]);
	}
	ck_assert_int_eq(bare_metal_count, N * M);
}
END_TEST

START_TEST(BENCH_SLEEPLOCK)
{
	futex_t m = { 0 };
	pthread_t thr[N];
	for (int i = 0; i < N; i++) {
		thr[i] = futex_thread_create(sleeplock_worker, &m);
	}
	for (int i = 0; i < N; i++) {
		thread_join(thr[i]);
	}
	ck_assert_int_eq(bare_metal_count, N * M);
}
END_TEST

START_TEST(BENCH_TRYLOCK)
{
	futex_t m = { 0 };
	pthread_t thr[X];
	for (int i = 0; i < X; i++) {
		thr[i] = futex_thread_create(trylock_worker, &m);
	}
	for (int i = 0; i < X; i++) {
		thread_join(thr[i]);
	}
	ck_assert_int_eq(bare_metal_count, X * Y);
}
END_TEST
