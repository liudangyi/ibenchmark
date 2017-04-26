## CPU, Scheduling, and OS Services

### Measurement overhead

```bash
# overhead of reading time
make test_rdtsc
#=> cycles = 19.433602, time = 7.497532ns

# overhead of using a loop to measure many iterations of an operation
make test_loop
#=> cycles = 0.744678, time = 0.287299ns
```

### Procedure call overhead

```bash
make test_call0
#=> cycles = 3.596432, time = 1.387512ns

make test_call1
#=> cycles = 3.496094, time = 1.348802ns

make test_call2
#=> cycles = 4.058392, time = 1.565738ns

make test_call3
#=> cycles = 4.005494, time = 1.545329ns

make test_call4
#=> cycles = 3.918804, time = 1.511884ns

make test_call5
#=> cycles = 4.009165, time = 1.546746ns

make test_call6
#=> cycles = 4.089374, time = 1.577691ns

make test_call7
#=> cycles = 4.842510, time = 1.868252ns
```

### System call overhead

```bash
make test_getpid                             
#=> cycles = 4.624042, time = 1.783967ns
# because getpid is cached

make test_getuid                             
#=> cycles = 328.456131, time = 126.719186ns
```

### Task creation time

```bash
make test_create_process
#=> count = 100, cycles = 420318.680000, time = 162159.984568ns
#=> count = 200, cycles = 438598.140000, time = 169212.245370ns

make test_create_thread
#=> count = 100, cycles = 43824.380000, time = 16907.554012ns
#=> count = 200, cycles = 69031.940000, time = 26632.692901ns
#=> count = 300, cycles = 77512.806667, time = 29904.632202ns
```

Seems that XNU uses different mechanisms to implement thread and process.

### Context switch time

```bash
make test_switch_process
#=> count = 100, cycles = 1293272.720000, time = 472956.550926ns
#=> count = 200, cycles = 1301392.830000, time = 502080.567130ns

make test_switch_thread
#=> count = 100, cycles = 35579.260000, time = 13726.566358ns
#=> count = 200, cycles = 39589.840000, time = 15273.858025ns
#=> count = 300, cycles = 46020.093333, time = 17754.665638ns
```
