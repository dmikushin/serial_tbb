#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/task_arena.h>
#include <tbb/task_group.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/global_control.h>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <length>\n", argv[0]);
		return 0;
	}
	int length = atoi(argv[1]);
	vector<double> result(length);

	int nthreads = tbb::task_scheduler_init::default_num_threads();
	const char* cnthreads = getenv("TBB_NUM_THREADS");
	if (cnthreads) nthreads = std::max(1, atoi(cnthreads));
	printf("Using %d threads\n", nthreads);

	tbb::task_group group;
	tbb::task_arena arena(nthreads, 1);
#if 0
	// Uncomment this to get things even worse
	tbb::global_control control(tbb::global_control::max_allowed_parallelism, nthreads);
#else
	// When only 1 thread is requested, it actually creates
	// the full house of threads, which is unexpected and
	// is a subject of the StackOverflow question:
	// https://stackoverflow.com/questions/59736661/how-to-completely-switch-off-threading-in-tbb-code/59740476
	//
	// TBB_NUM_THREADS=1 gdb ./serial_tbb
	// (gdb) r 16
	// Starting program: /home/marcusmae/hypersolve.io/serial_tbb/build/serial_tbb 16
	// [Thread debugging using libthread_db enabled]
	// Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
	// Using 1 threads
	// [New Thread 0x7ffff6a3d700 (LWP 8630)]
	// [New Thread 0x7fffeffff700 (LWP 8631)]
	// [New Thread 0x7ffff663c700 (LWP 8632)]
	// [New Thread 0x7ffff623b700 (LWP 8633)]
	// [New Thread 0x7ffff5a39700 (LWP 8635)]
	// [New Thread 0x7ffff5e3a700 (LWP 8634)]
	// [Thread 0x7ffff5e3a700 (LWP 8634) exited]
	// [Thread 0x7ffff623b700 (LWP 8633) exited]
	// [Thread 0x7ffff663c700 (LWP 8632) exited]
	// [Thread 0x7fffeffff700 (LWP 8631) exited]
	// [Thread 0x7ffff6a3d700 (LWP 8630) exited]
	// [Thread 0x7ffff7f9c740 (LWP 8626) exited]
	// [Inferior 1 (process 8626) exited normally]
	tbb::task_scheduler_init init(nthreads);
#endif

	group.run( [&] {
		tbb::parallel_for(tbb::blocked_range<int>(0, result.size()),
			[&](const tbb::blocked_range<int>& range)
		{
			for (int i = range.begin(); i != range.end(); i++)
				result[i] = sqrt((double)i);
		});
	});

	arena.execute( [&] { group.wait(); });		

	return 0;
}
