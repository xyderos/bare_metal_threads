#include <check.h>
#include <stdio.h>

#include "../test_utils/test_utils.h"

Suite *futex_suite(void);

static volatile int bare_metal_count;

static void
futex_worker(void *m)
{
	for (int i = 0; i < M; i++) {
		futex_lock(m);
		int c = bare_metal_count;
		bare_metal_count = c + 1;
		futex_unlock(m);
	}
}

START_TEST(BENCH_FUTEX)
{
	futex_t m = { 0 };
	pthread_t thr[N];
	for (int i = 0; i < N; i++) {
		thr[i] = futex_thread_create(futex_worker, &m);
	}
	for (int i = 0; i < N; i++) {
		thread_join(thr[i]);
	}
	ck_assert_int_eq(bare_metal_count, N * M);
}
END_TEST
