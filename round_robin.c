#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "policy.h"

#define SCHED_NAME "Round Robin Scheduler"


void schedule_task_rr(tcb_t **ready_queue, tcb_t **running_task, Interval **schedule,
		int *numReady, int currentTime) {

	if(*running_task != NULL) {
		(*running_task)->params.runTime += 1;
		if((*running_task)->params.runTime == (*running_task)->params.burstTime) {
			terminate_task((*running_task)->pid);
			insertInterval(schedule, (*running_task)->pid, currentTime);
			*running_task = NULL;
		} else if(*numReady > 0 && (*running_task)->params.burstTime > 3) {
			suspend_task((*running_task)->pid);
			insertInterval(schedule, (*running_task)->pid, currentTime);
			*running_task = NULL;
		}
	}

	if(*running_task == NULL && *numReady > 0) {
		tcb_t *task = popQueue(ready_queue, numReady);
		if(task->params.responseTime == -1) {
			task->params.responseTime = currentTime - task->params.arrivalTime;
		}
		start_task(task->pid);
		*running_task = task;
		if(*schedule == NULL) insertInterval(schedule, -1, currentTime);
	}

	if(*running_task == NULL) {
		printf("No task running - current time = %d\n", currentTime);
	} else {
		printf("running process %s - current time = %d\n", (*running_task)->pname, currentTime);
	}

}

void rrScheduler(tcb_t **task_list, int num_tasks) {
	int totalWaitingtime = 0, totalBursttime = 0,
		totalResponsetime = 0, currentTime = 0,
		numProcesses = 0, numReady = 0;

	tcb_t **ready_queue = calloc(MAX_TASKS, sizeof(tcb_t *));
	tcb_t *running_task = NULL;
	Interval *schedule = NULL;
	int time_quantum = 3;

	printf("\n<---------------------------------->\n");
	printf("\t%s\n\n", SCHED_NAME);

	do {
		for(int i = 0; i < num_tasks; i++) {
			if(task_list[i]->state == STATE_WAITING) {
				ready_task(i);
				insertQueue(ready_queue, task_list[i], &numReady);
				rrSort(ready_queue, numReady);
				task_list[i]->params.waitingTime += 1;
			} else if (task_list[i]->state == STATE_INACTIVE &&
					task_list[i]->params.arrivalTime == currentTime) {
				ready_task(i);
				insertQueue(ready_queue, task_list[i], &numReady);
				rrSort(ready_queue, numReady);
				++numProcesses;
			} else if (task_list[i]->state == STATE_READY) {  // TODO: Double check average waiting time
				task_list[i]->params.waitingTime += 1;
			}
		}
		schedule_task_rr(ready_queue, &running_task, &schedule, &numReady, currentTime);
		++currentTime;
	} while(numProcesses != num_tasks || numReady != 0 || running_task != NULL);

	for (int i = 0; i < num_tasks; i++) {
		totalResponsetime += task_list[i]->params.responseTime;
		totalWaitingtime += task_list[i]->params.waitingTime;
		totalBursttime += task_list[i]->params.burstTime;
	}

	printSchedulingInfo();

	printf("Average waiting time = %.4f\n",
	           ((float)totalWaitingtime / (float)num_tasks));
	printf("Average response time = %.4f\n",
		   ((float)totalResponsetime / (float)num_tasks));
	printf("Average turn around time = %.4f\n",
		   ((float)(totalWaitingtime + totalBursttime) / (float)num_tasks));
	printf("Throughput = %.4f\n",
			   ((float)num_tasks) / (float)(totalWaitingtime + totalBursttime));
	printGanttChart(SCHED_NAME, schedule);
	printf("\n<---------------------------------->\n");

	freeSchedule(&schedule);
	free(ready_queue);
}
