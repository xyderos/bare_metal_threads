#pragma once
#include <sys/time.h>

#include <stdatomic.h>
#include <stdlib.h>

typedef unsigned long ul_t;
typedef unsigned int ui_t;

enum { MUTEX_UNLOCKED, MUTEX_LOCKED, MUTEX_SLEEPING };

static inline ul_t
get_time(ul_t *old_t)
{
	ul_t t;
	struct timeval tv;

	if (gettimeofday(&tv, NULL) != 0)
		return 0;

	t = tv.tv_sec * 1000000 + tv.tv_usec;
	if (t < *old_t)
		return *old_t;

	*old_t = t;
	return t;
}
