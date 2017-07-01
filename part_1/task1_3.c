#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

// Function declarations
void validate_number_of_students(int number_of_students);
void validate_arguments(int argc);
void validate_office_capacity(int office_capacity);
void student(int id);
void *ask_questions(int *id);
void professor();
void *answer_questions();

// Global variables
int current_office_capacity = 0;
int office_capacity = -1;

int main(int argc, char *argv[]) {

  int number_of_students, i;

  validate_arguments(argc);

  number_of_students = atoi(argv[1]);
  office_capacity = atoi(argv[2]);

  validate_number_of_students(number_of_students);
  validate_office_capacity();

  pthread_t student_threads[number_of_students];
  pthread_t professor_thread;

  for (i = 0; i < number_of_students; i++) {
    student(i);
  }

  return 0;
}

void student(int id) {
  pthread_create(&student_threads[id], NULL, ask_questions, (int *)id);
  pthread_join(student_threads[id], NULL);
}

void *ask_questions(int *id) {
  int number_of_questions = (id % 4) + 1;
  int i;

  // If the office is already full
  if (current_office_capacity == office_capacity) {
    // student must wait
  } else {
    // student enters the professor's office
    // enter_office();

    for (i = 0; i < number_of_questions; i++) {
      // question_start();
      // question_done();
    }

    // leave_office();
  }
}

void professor() {
  pthread_create(&professor_thread, NULL, answer_questions, NULL);
  pthread_join(professor_thread, NULL);
}

void *answer_questions() {
  while (true) {
    // answer_start();
    // answer_done();
  }
}

void validate_arguments(int argc) {
  if (argc != 3) {
    printf("usage: %s <number_of_students> <office_capacity>\n", argv[0]);
    exit(-1);
  }
}

void validate_number_of_students(int number_of_students) {
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
