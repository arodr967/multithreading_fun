// COP4610 – Principles of Operating Systems – Summer C 2017
// Prof. Jose F. Osorio
// Student: Alicia Rodriguez – 5162522
//
// Assignment#: 3
// Project: Multithreaded Programming
// Specs: Queue Scheduling Multi-Threaded Programming with OpenMP
// Due Date: 07/09/2017 by 11:55pm
//
// Module Name: Queue Scheduling Multi-Threaded Programming with OpenMP
// I Certify that this program code has been written by me
// and no part of it has been taken from any sources.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <omp.h>

// Function declarations
void validate_arguments(int argc, char *argv[]);
void validate_number_of_students();
void validate_office_capacity();
void initialize_locks();
void student(int id);
void professor();
void answer_start();
void answer_done();
void enter_office();
void leave_office();
void question_start();
void question_done();

// Global variables
int office_student_id, question_student_id = -1;
int office_capacity, number_of_students, current_number_of_students, current_office_capacity;

omp_lock_t question_queue;
omp_lock_t office_queue;
omp_lock_t student_speaker;
omp_lock_t professor_speaker;

int main(int argc, char *argv[]) {
  int i, tid, student_tid;

  validate_arguments(argc, argv);

  number_of_students = atoi(argv[1]);
  office_capacity = atoi(argv[2]);

  validate_number_of_students();
  validate_office_capacity();

	initialize_locks();

	omp_set_num_threads(number_of_students + 1);
	current_number_of_students = number_of_students;

  printf("\n*** PROFESSOR'S OFFICE HOURS HAVE BEGUN ***\n\n");

	#pragma omp parallel private(tid, student_tid)
	{
		tid = omp_get_thread_num();

		if (tid == 0) {
			professor();
		} else {
			student_tid = tid - 1;
			student(student_tid);
		}

		if (current_number_of_students == 0) {
			printf("\n*** PROFESSOR'S OFFICE HOURS HAVE CONCLUDED ***\n\n");
			exit(0);
		}
	}

  return 0;
}

void student(int id) {
	int number_of_questions = fmod(id, 4) + 1;
	int i;

	// wait if office is full
	omp_set_lock(&office_queue);
	while(current_office_capacity == office_capacity);
	omp_unset_lock(&office_queue);
	office_student_id = id;
	current_office_capacity++;
	enter_office();

	for (i = 0; i < number_of_questions; i++) {
		omp_set_lock(&question_queue);
		// wait until it's student turn to ask question
		omp_set_lock(&student_speaker);
		question_student_id = id;
		question_start();

		omp_unset_lock(&professor_speaker);
		omp_set_lock(&student_speaker);
		// wait until professor is done answering the question
		question_done();
		question_student_id = -1; // making sure there are no errors
		omp_unset_lock(&professor_speaker);
		omp_unset_lock(&question_queue);
	}

	// another student can enter the office now
	current_office_capacity--;
	office_student_id = id;
	current_number_of_students--;
	leave_office();
}

void professor() {
	while(1) {
    omp_unset_lock(&student_speaker);
    omp_set_lock(&professor_speaker);

    answer_start();
    answer_done();

    omp_unset_lock(&student_speaker);
		omp_set_lock(&professor_speaker);
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

void initialize_locks() {
	omp_init_lock(&office_queue);
	omp_init_lock(&question_queue);

	omp_init_lock(&student_speaker);
	omp_set_lock(&student_speaker); // initially set to wait

	omp_init_lock(&professor_speaker);
	omp_set_lock(&professor_speaker); // initially set to wait
}
