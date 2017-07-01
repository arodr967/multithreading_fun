#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

int shared_variable = 0;

void *simple_thread(void *param);

int main(int argc, char *argv[]) {
  long thread;

  if (argc != 2) {
    printf("usage: %s <number_of_threads>\n", argv[0]);
  } else {
    int number_of_threads = atoi(argv[1]);
    printf("\ncreating %d thread(s)...\n\n", number_of_threads);

    pthread_t threads[number_of_threads];

    for (thread = 0; thread < number_of_threads; thread++) {
      pthread_create(&threads[thread], NULL, simple_thread, (void *)thread);
    }

    for (thread = 0; thread < number_of_threads; thread++) {
      pthread_join(threads[thread], NULL);
    }
  }

  return 0;
}

void *simple_thread(void *param) {
  int num, val = 0;
  int thread_num = (intptr_t)param + 1;

  for (num = 0; num < 20; num++) {
    if (random() > RAND_MAX/2) {
      usleep(10);
    }
    val = shared_variable;

    printf("*** thread #%d sees value %d\n", thread_num, val);
    shared_variable = val + 1;
  }
  val = shared_variable;
  printf("thread #%d sees final value %d\n\n", thread_num, val);
}
