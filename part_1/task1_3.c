#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>

// Function declarations
void validate_arguments(int argc, char *argv[]);
void validate_number_of_students();
void validate_office_capacity();
void *student_actions(void *stud);
void *professor_actions();
void student(int id);
void professor();
void answer_start();
void answer_done();
void enter_office();
void leave_office();
void question_start();
void question_done();
void initialize_semaphores();

// Global variables
int office_student_id, question_student_id = -1;
int office_capacity, number_of_students;

sem_t office_queue;
sem_t question_queue;
sem_t student_speaker;
sem_t professor_speaker;
pthread_t *student_threads;

struct student_struct {
	int number_of_questions; //number of questions the student has
	int id; // students id
};

int main(int argc, char *argv[]) {
  int i;

  validate_arguments(argc, argv);

  number_of_students = atoi(argv[1]);
  office_capacity = atoi(argv[2]);

  validate_number_of_students();
  validate_office_capacity();

  initialize_semaphores();

  printf("\n*** PROFESSOR'S OFFICE HOURS HAVE BEGUN ***\n\n");

  // Create professor thread
  professor();

  student_threads = malloc(sizeof(struct student_struct) * number_of_students);

  // Create student thread
  for (i = 0; i < number_of_students; i++) {
    student(i);
  }

  // Join student threads
  for (i = 0; i < number_of_students; i++) {
    pthread_join(*(student_threads + i), NULL);
  }

  free(student_threads);

  printf("\n*** PROFESSOR'S OFFICE HOURS HAVE CONCLUDED ***\n\n");

  return 0;
}

void student(int id) {
  pthread_t student_thread;

  struct student_struct* stud = (struct student_struct*)malloc(sizeof(struct student_struct));

  stud->id = id;
  stud->number_of_questions = fmod(id, 4) + 1;

  pthread_create(&student_thread, NULL, student_actions, (void *)stud);

  *(student_threads + id) = student_thread;
}

void *student_actions(void *stud) {

  struct student_struct *student = (struct student_struct *) stud;
  int number_of_questions = student->number_of_questions;
  int i;
  int id = student->id;

  sem_wait(&office_queue); // wait if office is full
  office_student_id = id;
  enter_office();

  for (i = 0; i < number_of_questions; i++) {
    sem_wait(&question_queue);

    // wait until it's student turn to ask question
    sem_wait(&student_speaker);
    question_student_id = id;
    question_start();

    sem_post(&professor_speaker);
    sem_wait(&student_speaker);
    // wait until professor is done answering the question
    question_done();
    question_student_id = -1; // making sure there are no errors
    sem_post(&professor_speaker);

    sem_post(&question_queue);
  }

  sem_post(&office_queue); // another student can enter the office now
  office_student_id = id;
  leave_office();

}

void professor() {
  pthread_t professor_thread;

  pthread_create(&professor_thread, NULL, professor_actions, NULL);
}

void *professor_actions() {
  while(1) {
    sem_post(&student_speaker);
    sem_wait(&professor_speaker);

    answer_start();
    answer_done();

    sem_post(&student_speaker);
		sem_wait(&professor_speaker);
  }
}

void enter_office() {
  printf("Student %d enters the office.\n", office_student_id);
}

void leave_office() {
  printf("Student %d leaves the office.\n", office_student_id);
}

void question_start() {
  printf("Student %d asks a question.\n", question_student_id);
}

void question_done() {
  printf("Student %d is satisfied.\n", question_student_id);
}

void answer_start() {
  printf("Professor starts to answer question for student %d.\n", question_student_id);
}

void answer_done() {
  printf("Professor is done with answer for student %d.\n", question_student_id);
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

void initialize_semaphores() {
	sem_init(&office_queue, 0, office_capacity); // handle students in office
  sem_init(&question_queue, 0, 1); // handle students asking questions
  sem_init(&student_speaker, 0, 0); // handle when the student speaks
  sem_init(&professor_speaker, 0, 0); // handle when the professor speaks
}
