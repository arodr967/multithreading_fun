#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#ifdef PTHREAD_SYNC
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_barrier_t barrier;
#endif

int shared_variable = 0;

void *simple_thread(void *param);

int main(int argc, char *argv[]) {
  long thread;

  if (argc != 2) {
    printf("usage: %s <number_of_threads>\n", argv[0]);
  } else {
    int number_of_threads = atoi(argv[1]);

    if (number_of_threads > 0) {
      #ifdef PTHREAD_SYNC
        pthread_barrier_init(&barrier, NULL, number_of_threads);
      #endif

      printf("\ncreating %d thread(s)...\n\n", number_of_threads);

      pthread_t threads[number_of_threads];

      for (thread = 0; thread < number_of_threads; thread++) {
        pthread_create(&threads[thread], NULL, simple_thread, (void *)thread);
      }

      for (thread = 0; thread < number_of_threads; thread++) {
        pthread_join(threads[thread], NULL);
      }
    } else {
      printf("ERROR: parameter <number_of_threads> should be greater than 0.\n");
      exit(-1);
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
    #ifdef PTHREAD_SYNC
      pthread_mutex_lock(&mutex);
    #endif

    val = shared_variable;

    printf("*** thread #%d sees value %d\n", thread_num, val);
    shared_variable = val + 1;

    #ifdef PTHREAD_SYNC
      pthread_mutex_unlock(&mutex);
    #endif
  }

  #ifdef PTHREAD_SYNC
    // Allow all threads to wait for the last thread to exit the loop.
    pthread_barrier_wait(&barrier);
  #endif

  val = shared_variable;
  printf("thread #%d sees final value %d\n\n", thread_num, val);
}
