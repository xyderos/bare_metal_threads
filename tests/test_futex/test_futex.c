#include "test_futex.h"

Suite *
futex_suite(void)
{
	Suite *s = NULL;
	TCase *tc_core = NULL;

	s = suite_create("test_futex");
	tc_core = tcase_create("core");

	tcase_add_test(tc_core, BENCH_FUTEX);

	suite_add_tcase(s, tc_core);

	return s;
}
