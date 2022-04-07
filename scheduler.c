#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"
#include "policy.h"


tcb_t **task_list;
int morr_p = 0;

int main(int argc, char **argv) {

	int sched = -1;

	// Allocate memory for task control blocks
	task_list = malloc(MAX_TASKS * sizeof(tcb_t));
	for(int i = 0; i < MAX_TASKS; i++) {
		task_list[i] = (tcb_t *) malloc(sizeof(tcb_t));
	}

	while(sched != EXIT) {

		// Get input
		if(getArgs(&sched) != 1) return EXIT_FAILURE;

		// Process params for priority based
		int burstTime_p[NUM_TASKS] = {6, 1, 2, 3, 4};
		int arrivalTime_p[NUM_TASKS] = {4, 5, 1, 5, 3};
		int priorities_p[NUM_TASKS] = {2, 3, 4, 1, 5};

		// Process params for round robin based
		int arrivalTime_rr[NUM_TASKS] = {0, 5, 1, 6, 8};
		int burstTime_rr[NUM_TASKS] = {8, 2, 7, 3, 5};

		// Process params for modified round robin
		int burstTime_mrr[NUM_TASKS] = {22, 18, 9, 10, 4};
		int arrivalTime_mrr[NUM_TASKS] = {0, 0, 0, 0, 0};
		int priorities_mrr[NUM_TASKS] = {4, 2, 1, 3, 5};

		// Process params for modulo based round robin
		int burstTime_morr[NUM_TASKS] = {10, 20, 30, 40, 50};
		int arrivalTime_morr[NUM_TASKS] = {0, 0, 0, 0, 0};
		int priorities_morr[NUM_TASKS] = {2, 3, 5, 4, 1};

		// Process params for priority based round robin
		int burstTime_pbrr[NUM_TASKS] = {10, 3, 12, 25, 7};
		int arrivalTime_pbrr[NUM_TASKS] = {0, 0, 0, 0, 0};
		int priorities_pbrr[NUM_TASKS] = {2, 1, 3, 4, 5};

		int burstTime_prr[NUM_TASKS] = {13, 25, 33, 8, 2};
		int priorities_prr[NUM_TASKS] = {2, 4, 1, 3, 5};
		//int burstTime_morr[4] = {43, 32, 24, 17};
		//int arrivalTime_morr[4] = {0, 0, 0, 0};

		switch(sched) {
			case ROUND_ROBIN:
				// Round Robin Scheduling
				init_tasks(burstTime_rr, arrivalTime_rr, NULL, NUM_TASKS);
				rrScheduler(task_list, NUM_TASKS, TIME_QUANTUM_3);
				break;

			case PRIORITY:
				// Priority Based Scheduling
				init_tasks(burstTime_p, arrivalTime_p, priorities_p, NUM_TASKS);
				priorityScheduler(task_list, NUM_TASKS);
				break;

			case MODIFIED_RR:
				// Modified Round Robin Scheduling
				init_tasks(burstTime_mrr, arrivalTime_mrr, priorities_mrr, NUM_TASKS);
				mrrScheduler(task_list, NUM_TASKS, modified_quantum(burstTime_mrr));
				break;

			case MODULO_BASED_RR:
				//Modulo Based Round Robin Scheduling
				init_tasks(burstTime_morr, arrivalTime_morr, NULL, NUM_TASKS);
				morrScheduler(task_list, NUM_TASKS, morr_quantum(burstTime_morr));
				break;

			case PRIORITY_BASED_RR:
				// Priority Based Round Robin Scheduling
				init_tasks(burstTime_prr, NULL, priorities_prr, NUM_TASKS);
				priorityRoundRobinScheduler(task_list, NUM_TASKS, TIME_QUANTUM_5*2);
				break;

			case PBRR_PRIO:
				// Priority Based RR and Priority Based
				printf("Priority Based Round Robin and Priority Based\n");
				init_tasks(burstTime_p, arrivalTime_p, priorities_p, NUM_TASKS);
				priorityScheduler(task_list, NUM_TASKS);
				detach_tasks();

				init_tasks(burstTime_p, arrivalTime_p, priorities_p, NUM_TASKS);
				priorityRoundRobinScheduler(task_list, NUM_TASKS, TIME_QUANTUM_5);
				break;

			case PBRR_RR:
				// Priority Based RR and Round Robin
				printf("Priority Based Round Robin and Round Robin\n");
				init_tasks(burstTime_pbrr, arrivalTime_pbrr, NULL, NUM_TASKS);
				rrScheduler(task_list, NUM_TASKS, TIME_QUANTUM_5);
				detach_tasks();

				init_tasks(burstTime_pbrr, arrivalTime_pbrr, priorities_pbrr, NUM_TASKS);
				priorityRoundRobinScheduler(task_list, NUM_TASKS, TIME_QUANTUM_5);
				break;

			case PBRR_MRR:
				// Priority Based RR and Modified Round Robin
				printf("Priority Based Round Robin and Modified Round Robin\n");
				init_tasks(burstTime_mrr, arrivalTime_mrr, priorities_mrr, NUM_TASKS);
				mrrScheduler(task_list, NUM_TASKS, modified_quantum(burstTime_mrr));
				detach_tasks();

				init_tasks(burstTime_mrr, arrivalTime_mrr, priorities_mrr, NUM_TASKS);
				priorityRoundRobinScheduler(task_list, NUM_TASKS, TIME_QUANTUM_5);
				break;

			case PBRR_MBRR:
				// Priority Based RR and Modulo Based RR
				printf("Priority Based Round Robin and Modulo Based Round Robin\n");
				init_tasks(burstTime_morr, arrivalTime_morr, NULL, NUM_TASKS);
				morrScheduler(task_list, NUM_TASKS, morr_quantum(burstTime_morr));
				detach_tasks();

				init_tasks(burstTime_morr, arrivalTime_morr, priorities_morr, NUM_TASKS);
				priorityRoundRobinScheduler(task_list, NUM_TASKS, TIME_QUANTUM_3);
				break;

			case EXIT:
				printf("Exiting.\n");
				break;
			default:
				printf("invalid argument.\n");
				break;
		}

		// detach tcb blocks
		detach_tasks();
	}


	// Free task control blocks
	for(int i = 0; i < MAX_TASKS; i++) {
		free(task_list[i]);
	}

	free(task_list);

	return EXIT_SUCCESS;
}


/*
 * Create/attach or detach tasks to/from task list
 * task.attached = 1 when tcb block is in use and 0 otherwise
 */

int task_create(int burstTime, int arrivalTime, int priority) {
	int i = 0;

	while(i < MAX_TASKS) {
		if(task_list[i]->attached != 1) {
			task_list[i]->pid = i;
			asprintf(&task_list[i]->pname, "P%d", i+1);
			task_list[i]->state = STATE_INACTIVE;
			task_list[i]->priority = priority;
			task_list[i]->attached = 1;

			task_list[i]->params.arrivalTime = arrivalTime;
			task_list[i]->params.burstTime = burstTime;
			task_list[i]->params.responseTime = -1;
			task_list[i]->params.waitingTime = 0;
			task_list[i]->params.runTime = 0;
			return 0;
		}
		i++;
	}

	return -1;
}

void detach_tasks() {
	for(int i = 0; i < MAX_TASKS; i++) {
		task_list[i]->attached = 0;
	}
}

void init_tasks(int *burstTimes, int * arrivalTimes, int *priorities, int num_tasks) {
	for(int i = 0; i < num_tasks; i++) {
		task_create(burstTimes[i], (arrivalTimes != NULL ? arrivalTimes[i] : 0),
				(priorities != NULL ? priorities[i] : 0));
	}
}


/*
 * Functions to manage task state
 */

void start_task(int task_id) {
	task_list[task_id]->state = STATE_RUNNING;
}

void suspend_task(int task_id) {
	task_list[task_id]->state = STATE_WAITING;
}

void ready_task(int task_id) {
	task_list[task_id]->state = STATE_READY;
}

void terminate_task(int task_id) {
	task_list[task_id]->state = STATE_TERMINATED;

}


/*
 * Time quantum
 */

int morr_quantum(int *burstTime_morr) {
	int qn = 0;
	for(int i = 0; i < 4; i++){
		qn += burstTime_morr[i];
	}
	qn /= 4;

	morr_p = qn;
	return qn;
}

int modified_quantum(int *burstTime_mrr) {
	int qn = 0;
	if(NUM_TASKS%2 == 0){
		for(int i = 0; i < NUM_TASKS; i++){
			qn += burstTime_mrr[i];

		}
		qn /= NUM_TASKS;
	}else{
		qn = burstTime_mrr[NUM_TASKS/2];
	}
	return qn;
}


/*
 * Sort task array by priority
 */

int priorityComp(const void *elem1, const void *elem2) {
	int p1 = (*((tcb_t**)elem1))->priority;
	int p2 = (*((tcb_t**)elem2))->priority;
	if(p1 > p2) return 1;
	if(p1 < p2) return -1;
	return 0;
}

void prioritySort(tcb_t **task_array, int numReady) {
	qsort(task_array, numReady, sizeof(*task_array), priorityComp);
}


/*
* Sort task array by burst (or execution) modulo average of burst
*/

int moduloComp(const void *elem1, const void *elem2) {
	int m1 = (*((tcb_t**)elem1))->params.burstTime%morr_p;
	int m2 = (*((tcb_t**)elem2))->params.burstTime%morr_p;
	if(m1 > m2) return 1;
	if(m1 < m2) return -1;
	return 0;
}

void moduloSort(tcb_t **task_array, int numReady) {

	qsort(task_array, numReady, sizeof(*task_array), moduloComp);
}


/*
* Sort task array by remaining burst times
*/

int burstComp(const void *elem1, const void *elem2) {
	int b1 = (*((tcb_t**)elem1))->params.burstTime - (*((tcb_t**)elem1))->params.runTime;
	int b2 = (*((tcb_t**)elem2))->params.burstTime - (*((tcb_t**)elem2))->params.runTime;
	if(b1 > b2) return 1;
	if(b1 < b2) return -1;
	return 0;
}

void burstSort(tcb_t **task_array, int numReady) {

	qsort(task_array, numReady, sizeof(*task_array), burstComp);
}


/*
 * Insert to and remove first task from ready task queue
 */

void insertQueue(tcb_t **ready_queue, tcb_t *task, int *numReady) {
	int i = 0;
	while(ready_queue[i] != NULL) i++;
	ready_queue[i] = task;
	++(*numReady);
}

void insertQueuePrio(tcb_t **ready_queue, tcb_t *task, int *numReady) {
	int i = 0;
	while(ready_queue[i] != NULL) {
		if(task->priority < ready_queue[i]->priority) {
			for(int j = *numReady; j > i; j--) {
				ready_queue[j] = ready_queue[j-1];
			}
			break;
		}
		i++;
	}
	ready_queue[i] = task;
	++(*numReady);
}

tcb_t *popQueue(tcb_t **ready_queue, int *numReady) {
	if (*numReady > 0) {
		tcb_t *nextTask = ready_queue[0];
		for(int i = 0; i < *numReady-1; i++) {
			ready_queue[i] = ready_queue[i+1];
		}
		ready_queue[*numReady-1] = NULL;
		--(*numReady);
		return nextTask;
	}
	return NULL;
}

/*
 * Interval list functions to track schedule
 */

void insertInterval(Interval **schedule, int pid, int time) {
	Interval *i = (Interval*)malloc(sizeof(Interval));
	i->pid = pid;
	i->time = time;
	i->nextInterval = NULL;
	if((*schedule) == NULL) {
		(*schedule) = i;
		return;
	}
	while((*schedule)->nextInterval != NULL) {
		schedule = &((*schedule)->nextInterval);
	}
	(*schedule)->nextInterval = i;
}

void freeSchedule(Interval **schedule) {
	Interval* sched = *schedule;
	Interval* temp = *schedule;
	*schedule = NULL;

	while(sched != NULL) {
		sched = sched->nextInterval;
		free(temp);
		temp = sched;
	}
}


/*
 * Print Gantt Chart for the current task_list
 */

void printGanttChart(char *sched_name, Interval *schedule) {
	printf("\nGantt Chart - %s\n", sched_name);

	Interval *process_seq = schedule->nextInterval;
	while(process_seq != NULL) {
		printf("|  P%d  ", process_seq->pid+1);
		process_seq = process_seq->nextInterval;
	}
	printf("|\n");

	Interval *interval_seq = schedule;
	while(interval_seq != NULL) {
		printf("%-7d", interval_seq->time);
		interval_seq = interval_seq->nextInterval;
	}
	printf("\n");
}


/*
 * Print scheduling info
 */

void printSchedulingInfo() {
	printf("\nProcess\tPrio\tArrival_time\tBurst_Time\tWaiting_Time\tResponse_Time\tTurn_Around_Time\n");

	int i = 0;

	while(i < MAX_TASKS && task_list[i]->attached == 1) {
		sched_params params = task_list[i]->params;
		printf("%7s\t%4d\t%12d\t%10d\t%12d\t%13d\t%16d\t\n", task_list[i]->pname, task_list[i]->priority,
				params.arrivalTime, params.burstTime, params.waitingTime, params.responseTime,
				params.runTime+params.waitingTime);
		++i;
	}
}


/*
 * Print list of arguments
 */

int getArgs(int *sched) {
	printf("\nScheduling Arguments: \n"
			"%d. Round Robin\n"
			"%d. Priority Based\n"
			"%d. Modified Round Robin\n"
			"%d. Modulo Based Round Robin\n"
			"%d. Priority Based Round Robin\n"
			"%d. Priority Based RR and Priority Based\n"
			"%d. Priority Based RR and Round Robin\n"
			"%d. Priority Based RR and Modified RR\n"
			"%d. Priority Based RR and Modulo Based RR\n"
			"%d. Exit\n\nEnter sched param (0-9): \n",
			ROUND_ROBIN, PRIORITY, MODIFIED_RR, MODULO_BASED_RR, PRIORITY_BASED_RR,
			PBRR_PRIO, PBRR_RR, PBRR_MRR, PBRR_MBRR, EXIT);
	if (scanf("%d", sched) != 1)
	{
	  printf("invalid argument.\n");
	  return -1;
	}
	printf("\n");

	return 1;
}

