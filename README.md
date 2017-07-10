# Multithreaded Programming and Synchronization

- [Part 1: Simple Multi-threaded Programming using Pthreads](#part-1-simple-multi-threaded-programming-using-pthreads)
- [Part 2: Multi-threaded/Parallel Programming using OpenMP](#part-2-multi-threadedparallel-programming-using-openmp)
- [Part 3: OpenMP Solution for Queue Scheduling Problem in Task 1.3](#part-3-openmp-solution-for-queue-scheduling-problem-in-task-13)


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



## Part 2: Multi-threaded/Parallel Programming using OpenMP

Files found for part 2 are in the `part_2/` directory.

### Tasks 1, 2, and 3

- Task 1 can be found in `OpenMP/helloworld.c`
- Task 2 can be found in `task2_2.c`
- Task 3 can be found in `task2_3.c`

#### Compile & Run

Any of the files above an be compiled and run the following way:

1. `gcc -fopenmp task2_2.c -o task2_2`
2. `./task2_2`

### Task 4

Task 4 can be found separated as 4 different files.

- `task2_4_original.c` is the original code that is given on the assignment.
- `task2_4_outer.c` is the parallelized code with only the outer for loop parallelized.
- `task2_4_middle.c` is the parallelized code with only the middle for loop parallelized.
- `task2_4_both.c` is the parallelized code with both the outer and middle loops parralelized.

#### Compile & Run

Any of the files above an be compiled and run the following way:

1. `gcc -fopenmp task2_4_original.c -o task2_4_original`
2. `./task2_4_original`



## Part 3: OpenMP Solution for Queue Scheduling Problem in Task 1.3

File found for part 3 are in `part_3/task1_3_OpenMP.c`

#### Compile & Run

1. `gcc -fopenmp -lm task1_3_OpenMP.c -o task1_3_OpenMP`
2. `./task1_3_OpenMP <number_of_students> <office_capacity>`
