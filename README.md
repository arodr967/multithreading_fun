# Multithreaded Programming and Synchronization

- [Part 1: Simple Multi-threaded Programming using Pthreads](#part-1-simple-multi-threaded-programming-using-pthreads)
- Part 2: Multi-threaded/Parallel Programming using OpenMP

## Part 1: Simple Multi-threaded Programming using Pthreads

Files found for part 1 are in the `part_1/` directory.

### Tasks 1 & 2

Tasks 1 and 2 can be found in `task1_2.c`.

#### Compile & Run _without_ Synchronization

1. `gcc -pthread task1_2.c -o task1_2`
2. `./task1_2 <number_of_threads>`

Place output into a file by:

`./task1_2 <number_of_threads> > task1_1_results/<number_of_threads>_threads_results.txt`

> Example: `./task1_2 200 > task1_1_results/200_threads_results.txt`

_Some output files have already been added from previous executions._

#### Compile & Run _with_ Synchronization

1. `gcc -pthread -Wall -D PTHREAD_SYNC task1_2.c -o task1_2_sync`
2. `./task1_2_sync <number_of_threads>`

> NOTE: Some warnings might appear but they will not prevent the program from executing.

Place output into a file by:

`./task1_2_sync <number_of_threads> > task1_2_results/<number_of_threads>_threads_results.txt`

> Example: `./task1_2_sync 200 > task1_2_results/200_threads_results.txt`

_Some output files have already been added from previous executions._

### Task 3

Task 3 can be found in `task1_3.c`

#### Compile & Run

1. `gcc -pthread -lm task1_3.c -o task1_3`
2. `./task1_3 <number_of_students> <office_capacity>`
