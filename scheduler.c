#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"


tcb_t task_list[MAX_TASKS+1];
int t_curr;

void scheduler();


int main(void) {

	// Initialize array of task pointers
	void (*tasks[MAX_TASKS]) (void *p) = {task1, task2, task3, task4, task5, task6};

	int num_tasks = sizeof(tasks)/sizeof(*tasks);
	for (int i = 0; i < num_tasks; i++) {
		task_create(tasks[i], NULL, 0);
	}

	while(1) {
		scheduler();
	}

	return EXIT_SUCCESS;
}



void scheduler() {

}


/*
 * Create/attach task to task list
 */

int task_create(void (*fptr)(void *p), void * args, int priority) {
	int i;

	while(i < MAX_TASKS) {
		if(task_list[i].attached != 1) {
			task_list[i].pid = i;
			task_list[i].fptr = fptr;
			task_list[i].args = args;
			task_list[i].state = STATE_READY;
			task_list[i].delay = -1;
			task_list[i].priority = priority;
			task_list[i].attached = 1;
			return 0;
		}
		i++;
	}

	return -1;
}


/*
 * Functions to manage task state
 */

void start_task(int task_id) {
	task_list[task_id].state = STATE_READY;
}

void delay(unsigned int d) {
	task_list[t_curr].delay = d;
	task_list[t_curr].state = STATE_WAITING;
}

void suspend_task() {
	task_list[t_curr].state = STATE_INACTIVE;
}

/*
 * Test processes
 */

void task1(void *p) {
	printf("task1/n");
}

void task2(void *p) {
	printf("task2/n");
}

void task3(void *p) {
	printf("task3/n");
}

void task4(void *p) {
	printf("task4/n");
}

void task5(void *p) {
	printf("task5/n");
}

void task6(void *p) {
	printf("task6/n");
}



