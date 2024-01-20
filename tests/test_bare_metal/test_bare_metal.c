#include "test_bare_metal.h"

Suite *
bare_metal_suite(void)
{
	Suite *s = NULL;
	TCase *tc_core = NULL;

	s = suite_create("test_bare_metal");
	tc_core = tcase_create("core");

	tcase_add_test(tc_core, BENCH_SPINLOCK);
	tcase_add_test(tc_core, BENCH_TIMEDLOCK);
	tcase_add_test(tc_core, BENCH_SLEEPLOCK);
	tcase_add_test(tc_core, BENCH_TRYLOCK);

	suite_add_tcase(s, tc_core);

	return s;
}
