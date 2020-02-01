This test shows how to impose static global limitations on the amount of threads spawned by TBB.

The problem of the well-known `task_scheduler_init`-based approach is that TBB in any case creates a large number of threads, reflecting the number of the available physical threads in the system. Even if the scheduler explicitly asks for less threads, the same maximum number of threads is always created.

The `global_control` class introduced in recent update of TBB copes with this problem on higher level. The `max_allowed_parallelism` property is able to limit the thread spawning strictly to the specified number of threads.

## Building

```
git clone --recurse https://github.com/dmikushin/python_tbb.git
mkdir build
cd build
cmake ..
make -j2
```

## Testing

```
$ TBB_NUM_THREADS=8 gdb ./serial_tbb
GNU gdb (Ubuntu 8.1-0ubuntu3) 8.1.0.20180409-git
(gdb) r 1000000
Starting program: /home/marcusmae/hypersolve.io/serial_tbb/build/serial_tbb 1000000
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
Using 8 threads
[New Thread 0x7ffff629b700 (LWP 25165)]
[New Thread 0x7ffff5e9a700 (LWP 25166)]
[New Thread 0x7ffff5a99700 (LWP 25167)]
[New Thread 0x7ffff5698700 (LWP 25168)]
[New Thread 0x7ffff4e96700 (LWP 25169)]
[New Thread 0x7ffff5297700 (LWP 25170)]
[New Thread 0x7ffff4a95700 (LWP 25171)]
[Thread 0x7ffff4a95700 (LWP 25171) exited]
[Thread 0x7ffff5297700 (LWP 25170) exited]
[Thread 0x7ffff4e96700 (LWP 25169) exited]
[Thread 0x7ffff5698700 (LWP 25168) exited]
[Thread 0x7ffff5e9a700 (LWP 25166) exited]
[Thread 0x7ffff629b700 (LWP 25165) exited]
[Thread 0x7ffff7f9c740 (LWP 25161) exited]
[Inferior 1 (process 25161) exited normally]
```

With `TBB_NUM_THREADS=1`, TBB does not spawn any additional threads, which is the expected behavior:

```
$ TBB_NUM_THREADS=1 gdb ./serial_tbb
GNU gdb (Ubuntu 8.1-0ubuntu3) 8.1.0.20180409-git
(gdb) r 1000000
Starting program: /home/marcusmae/hypersolve.io/serial_tbb/build/serial_tbb 1000000
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
Using 1 threads
[Inferior 1 (process 25218) exited normally]
```

Note: make sure to specify sufficiently large problem size. Not all threads might be created, if the problem size is too small.

