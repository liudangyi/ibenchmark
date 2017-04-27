## Introduction

### Objective

The objective of our project is to systematically determine the performance characteristic of the underlying hardware, such as CPU, RAM and disk, on Mac OS X. Though there are substantial specifications and documentations about relative performance quantities online, many must be determined experimentally. Through this project, we will obtain a further understanding about the influence imposed by hardware performance on operating system and the relation between hardware performance and user’s subjective sense of application’s “responsiveness”.

### Methodology

All the experiments will be conducted on Mac OS X by using C programming language with compiler Apple LLVM version 8.1.0 (clang-802.0.42). We set optimization setting of –O2 to eliminate the useless code, like function prologue / epilogue and unnecessary memory visiting. Since our tested CPU supports turbo boost, which means it has a variable frequency, we must disable this feature during our test. 

Throughout the whole experiments, we use function rdtsc() to count the number of cycles of CPU. Since most of the operations we're going to measure is very quick, we amptify the time by repeatedly execute the same operations many times. However, this will mostly demonstrate a better result because loop has a much better locality and greatly reduces the memory visit cost.

In order to reduce the software overhead, we use the following method. To measure the running time of 1000 some operation, we first execute it 1000 times, get an executing time `t1`, then we execute it 2000 times and get an executing time `t2`. We use `t2 - t1` as the final executing time, which ideally counteracts all software overhead.

The measurement operations contain procedure call overhead, RAM access time, file read time, etc. which will be illustrated in experiment section.

## Machine Description




## CPU

### Measurement Overhead

#### Methodology

For reading time overhead, we repeatedly read the tsc for 1000000 times and calculate the
average running time. To read the tsc, we use the predefined function `__rdtsc()` provided in `x86intrin.h`.

As for looping overhead, we measure the running time of an empty loop. Since we are using O2 optimization, to prevent the empty loop from being optimized away, we add an empty inline assembly in the body of the loop. This produce nothing but the loop is preserved. 

#### Prediction

For reading time, according to Intel's [optimization manual](http://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-optimization-manual.html), the throughput of RDTSC should be 13 cycles. The loop overhead could be 2, then the overall cost should be 15 cycles.

For overhead Loop, since there are only 2 instructions in the loop and the branch prediction should be quite accurate, the loop overhead should be under 2 cycles.

#### Results

|               | Prediction | Experiment Results    |
| ------------- | ---------- | --------------------- |
| Reading time  | 15 cycles  | 25.29 cycles, 9.76 ns |
| Loop overhead | 2 cycles   | 1.00 cycles, 0.38 ns  |

#### Discussion

The `rdtsc` overhead is much more than what's expected, we notice that the serializing version of `rdtsc`, `rdtscp`, has a throughput of 20, which is closer to our result. A explanation is that adjacent `rdtsc`s affect each other. Since Intel didn't provide the latency of `rdtsc`, throughput is only the lower bound of the executing time.

What interests us most is the loop overhead: it's as low as 1.00 cycles. The assembly code shows as below.

```
LBB1_2:
	incl  %eax
	jne   LBB1_2
```

Here, `%eax` stores the counter which is initialized as -10000000. According to the developer's manual from Intel, `inc` should take 1 cycle, which means there's nearly no overhead for the `jne` instruction. This is reasonable because if CPU predicts every `jne` branch correctly, the whole executing procedure will be

```
incl %eax
incl %eax
incl %eax
...
```

In this way, the cost is exactly 1.00 cycle because every two instructions are data dependent and cannot be issued simultaneously.

### Procedure call overhead

#### Methodology

In order to measure the procedure call overhead, we repeatedly call an empty function for 10000000 times. To make the code simple, we utilize the variable arguments feature of C. Other things worth noticing are that

1. We must use `__attribute__((noinline))` in order to prevent the compiler from inlining the function call.
2. We must add an empty assembly in the body of the function in order to prevent the compiler from optimizing the function call away.
3. There's a difference between x86 call convention and x64 call convention. Since we're using x64 system, the first 6 arguments will be past using registers rather than stack.

#### Prediction

The cost of the procedure call mainly consists of three parts.

1. Loop overhead. As described above, it's 1 cycle.
2. Argument passing overhead. According to different number of arguments, it should be between 0-10 cycles.
3. Callee overhead. Even the function body is empty, it need to setup the frame pointer. Since this part of memory is definitely in cache, it should take 3-5 cycles.

Thus we predict the procedure call overhead about 4-16 cycles, varying from the number of arguments.

#### Results

| \# of args | Prediction | Experiment Results   |
| ---------- | ---------- | -------------------- |
| 0          | 5          | 4.99 cycles, 1.92 ns |
| 1          | 6          | 4.95 cycles, 1.91 ns |
| 2          | 7          | 4.19 cycles, 1.62 ns |
| 3          | 8          | 4.40 cycles, 1.70 ns |
| 4          | 9          | 4.93 cycles, 1.90 ns |
| 5          | 10         | 4.03 cycles, 1.56 ns |
| 6          | 11         | 4.62 cycles, 1.78 ns |
| 7          | 12         | 4.19 cycles, 1.62 ns |

![call](graph/call.svg)

#### Discussion

The results show that the number of arguments has nearly no impact on the executing time. We can take a look at the assembly code.

```
LBB10_1:
	movl	$1, %esi
	movl	$2, %edx
	movl	$3, %ecx
	movl	$4, %r8d
	xorl	%eax, %eax
	callq	_func
	decl	%ebx
	jne	LBB10_1

_func:
	pushq	%rbp
	movq	%rsp, %rbp
	popq	%rbp
	retq
```

An explanation for this is that, the `mov` instructions above are independent from each other, thus can be issued simultaneously. Since the throughput of ALU operations is 0.25, and we have 4 ALU units (according to Intel's manual). We can support up-to 16 parallel assignments. So the number of arguments won't affect our running time. 

### System call overhead

#### Methodology

In order to measure the system call overhead, we need to use the simplest system call: getpid or getuid, since they are simply read operations on PCB. We measure the cost of 10000000 such call, and calculate the mean.

#### Prediction

System call requires a transition from user space to kernel space then user space. Although we have fast system call, this action is not trivial since we need to change the CS and SS registers, which also involves a lot of memory access. Based on this, we predict the cost of system call between 100-300 cycles.

#### Results

| System call | Prediction | Results                  |
| ----------- | ---------- | ------------------------ |
| getpid()    | 300 cycles | 5.87 cycles, 2.26 ns     |
| getuid()    | 300 cycles | 414.67 cycles, 159.98 ns |

#### Discussion

There's a huge difference between `getpid` and `getuid`. This is because `getpid` actually caches the result: a process's pid can't change once it's determined. `getuid` shows a result roughly close to our prediction.

### Task creation time

#### Methodology

For process creation, we use the `fork` system call to create a new process. In the new process we simply call `exit`.

For thread creation, we use the `pthread_create` system call. Since thread execution is based on a function, we provide it with an empty function.

#### Prediction

Process creation should take quite a lot of time: it'll need to allocate a PCB and setup the page table. The cost should be around 10000 cycles or more.

Thread is much lighter than process. We predict the cost around 3000 cycles.

#### Results

|         | Prediction | Results                        |
| ------- | ---------- | ------------------------------ |
| Process | 10000      | 538733.93 cycles, 207844.88 ns |
| Thread  | 3000       | 82363.93 cycles, 31776.21 ns   |

#### Discussion

Apparently we underestimate the cost greatly. It takes nearly 0.2 ms for OS to create a new process. We think that a possible explanation is that it not only involves process creation, but also involves context switching. Creating thread is much faster than process, which means that XNU kernel uses different structure to handle process and thread.

Another interesting observation is that, the creation cost of a thread is proportional to the number of threads created, but for process the cost is a constant. This means that in XNU kernel, threads are handled in a linear-time data structure such as linked list.

### Context switch time

#### Methodology

To measure the context switch time, we should first create one process / thread and then make CPU switch between the parent process / thread and child process / thread. To finish this, we use pipes: first parent writes on the pipe and child reads, then child writes on the pipe and parent reads.

Another thing worth noticing is that we need to disable the multicore feature of our CPU: otherwise it's most likely that two processes / threads are running on different cores and produces a much better result.

#### Prediction

Since context switch between processes will change the page table and evacuate the TLB, it'll be very time-consuming. We predict it around 10000 cycles.

The switch between threads should be quicker since they share the page table. We predict it around 5000 cycles.

#### Results

|         | Prediction   | Results                     |
| ------- | ------------ | --------------------------- |
| Process | 10000 cycles | 12304.54 cycles, 4747.12 ns |
| Thread  | 5000 cycles  | 7612.02 cycles, 2936.74 ns  |

#### Discussion

The experiment result is quite close to our prediction. But the interesting part is that, what if we enable multicore and do the experiment again? The result shows

|         | Single core                 | Multicore                  |
| ------- | --------------------------- | -------------------------- |
| Process | 12304.54 cycles, 4747.12 ns | 5982.74 cycles, 2308.16 ns |
| Thread  | 7612.02 cycles, 2936.74 ns  | 9532.40 cycles, 3677.62 ns |

The process switch has a much better performance! But the thread not. 