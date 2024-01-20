## Attempting to implement locks by the stdatomic library provided by C as well as the futex system calls

Some general observations:

1) __Futexes are great!__ Can be used anywhere but are not portable, in case you want to implement something similar, you would probably use either __pthreads__ (which are still not fully portable) and slower since you would need to use an internal mutex and a condition variable to signal

2) __Spinlocks are a straightforward implementation that can be used 99% of the time__, although the majority of the time they are spining, meaning that they utilize the CPU more

3) __Timedlocks are tricky__, based on benching them with various inputs from either number of threads, number of iterations and some timeouts, they really are case specific with a __big if__ on what concerns the performance improvement. __I guess the main concern is the thread contention__, when multiple threads are trying to acquire a timedlock, you will observe a lot of timeouts

4) __Sleeplocks__ in their simplest form (unlike futexes that the kernel is waking up a thread), do perform slower but they have reduced CPU cycles

5) __Trylocks__ are practically the basis of any other type of lock, one of the main issues is the __thread contention__ that is solved in the spinlocks by spinning to acquire and then lock. This makes things __messier on scalability__, meaning that they are not suitable for a big number of threads and a big number of iterations