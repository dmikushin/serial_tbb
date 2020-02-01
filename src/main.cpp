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

	tbb::global_control control(
		tbb::global_control::max_allowed_parallelism, nthreads);

	arena.execute( [&] {
		group.run( [&] {
			tbb::parallel_for(tbb::blocked_range<int>(0, result.size()),
				[&](const tbb::blocked_range<int>& range)
			{
				for (int i = range.begin(); i != range.end(); i++)
					result[i] = sqrt((double)i);
			});
		});
	});

	arena.execute( [&] { group.wait(); });		

	return 0;
}
