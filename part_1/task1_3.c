#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

// Function declarations
void validate_arguments(int argc, char *argv[]);
void validate_number_of_students();
void validate_office_capacity();
void student(int id);
void *ask_questions(int *student_id);
void professor(int student_id);
void *answer_questions(int *student_id);
void answer_start(int student_id);
void answer_done(int student_id);
void enter_office(int student_id);
void leave_office(int student_id);
void question_start(int student_id);
void question_done(int student_id);

// Global variables
int current_office_capacity = 0;
int is_question_started = 0;
int is_question_ended = 0;
int office_capacity, number_of_students;

// 0 false
// 1 true

// Sync
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

sem_t office_queue;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

int main(int argc, char *argv[]) {

  int i;

  validate_arguments(argc, argv);

  number_of_students = atoi(argv[1]);
  office_capacity = atoi(argv[2]);

  validate_number_of_students();
  validate_office_capacity();

  sem_init(&office_queue, 0, 1);

  for (i = 0; i < number_of_students; i++) {
    professor(i); // ??
    student(i);
  }

  return 0;
}

void student(int id) {
  pthread_t student_thread;

  pthread_create(&student_thread, NULL, ask_questions, (int *)id);
  pthread_join(student_thread, NULL);
}

void *ask_questions(int *id) {
  int number_of_questions = fmod((intptr_t)id, 4) + 1;
  int i;

  // // lock
  // pthread_mutex_lock(&mutex);
  // while (current_office_capacity == office_capacity) {
  //   // wait
  //   pthread_cond_wait(&cond, &mutex);
  // }
  // // unlock
  // pthread_mutex_unlock(&mutex);

  sem_wait(&office_queue);
  enter_office((intptr_t)id);

  for (i = 0; i < number_of_questions; i++) {
    // wait until it's student turn to ask question
    question_start((intptr_t)id);

    // wait until professor is done answering the question
    question_done((intptr_t)id);
  }


  leave_office((intptr_t)id);

}

void enter_office(int student_id) {
  printf("Student %d enters the office.\n", student_id);

  // less space in office now
  // pthread_mutex_lock(&mutex);
  // current_office_capacity++; // signal
  // pthread_cond_signal(&cond);
  // pthread_mutex_unlock(&mutex);
  sem_post(&office_queue);
}

void leave_office(int student_id) {
  printf("Student %d leaves the office.\n", student_id);

  // more space in office now
  // pthread_mutex_lock(&mutex);
  // current_office_capacity--; // signal
  // pthread_cond_signal(&cond);
  // pthread_mutex_unlock(&mutex);
  sem_post(&office_queue);
}

void question_start(int student_id) {
  printf("Student %d asks a question.\n", student_id);

  pthread_mutex_lock(&mutex);
  is_question_started = 1;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
}

void question_done(int student_id) {
  printf("Student %d is satisfied.\n", student_id);

  pthread_mutex_lock(&mutex);
  is_question_started = 0;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
}

void professor(int student_id) {
  pthread_t professor_thread;
  //
  pthread_create(&professor_thread, NULL, answer_questions, (int *)student_id);
  pthread_join(professor_thread, NULL);
  // answer_questions(student_id);
}

void *answer_questions(int *student_id) {
  pthread_mutex_lock(&mutex);
  while(is_question_started != 1) {
    pthread_cond_wait(&cond, &mutex);
  }
  pthread_mutex_unlock(&mutex);

  answer_start((intptr_t)student_id);
  answer_done((intptr_t)student_id);
}

void answer_start(int student_id) {
  // answer_start() blocks when there are no students around.

  // // lock
  // pthread_mutex_lock(&mutex);
  // while(current_office_capacity == 0) {
  //   // wait
  //   pthread_cond_wait(&cond, &mutex);
  // }
  // // unlock
  // pthread_mutex_unlock(&mutex);
  sem_wait(&office_queue);
  printf("Professor starts to answer question for student %d.\n", student_id);
}

void answer_done(int student_id) {
  printf("Professor is done with answer for student %d.\n", student_id);
}

void validate_arguments(int argc, char *argv[]) {
  if (argc != 3) {
    printf("usage: %s <number_of_students> <office_capacity>\n", argv[0]);
    exit(-1);
  }
}

void validate_number_of_students() {
  if (number_of_students < 0) {
    printf("ERROR: parameter <number_of_students> should be greater than or equal to 0.\n");
    exit(-1);
  }
}

void validate_office_capacity() {
  if (office_capacity <= 0) {
    printf("ERROR: parameter <office_capacity> should be greater than 0.\n");
    exit(-1);
  }
}
