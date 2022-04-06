#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "scheduler.h"
#include "policy.h"

#define SCHED_NAME "Round Robin Scheduler"

void schedule_task_rr(tcb_t **ready_queue, tcb_t **running_task, Interval **schedule,
		int *numReady, int currentTime, int quantum_number) {

	if(*running_task != NULL) {
		(*running_task)->params.runTime += 1;
		if((*running_task)->params.runTime == (*running_task)->params.burstTime) {
			terminate_task((*running_task)->pid);
			insertInterval(schedule, (*running_task)->pid, currentTime);
			*running_task = NULL;
		} else if(*numReady > 0 && (*running_task)->params.runTime%quantum_number == 0) {
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
		printf("running process %s - current time = %d \n", (*running_task)->pname, currentTime);
	}

}
//Fifo ready queue
void rrScheduler(tcb_t **task_list, int num_tasks, int quantum_number) {
	int totalWaitingtime = 0, totalBursttime = 0,
		totalResponsetime = 0, currentTime = 0,
		contextSwitches = 0, numProcesses = 0, numReady = 0;

	tcb_t **ready_queue = calloc(MAX_TASKS, sizeof(tcb_t *));
	tcb_t *running_task = NULL;
	Interval *schedule = NULL;

	printf("\n<---------------------------------->\n");
	printf("\t%s\n\n", SCHED_NAME);
	printf("Time Quantum = %d\n", quantum_number);

	do {
		for(int i = 0; i < num_tasks; i++) {
			if(task_list[i]->state == STATE_WAITING) {
				ready_task(i);
				insertQueue(ready_queue, task_list[i], &numReady);
				task_list[i]->params.waitingTime += 1;
			} else if (task_list[i]->state == STATE_INACTIVE &&
					task_list[i]->params.arrivalTime == currentTime) {
				ready_task(i);
				insertQueue(ready_queue, task_list[i], &numReady);
				++numProcesses;
			} else if (task_list[i]->state == STATE_READY) {
				task_list[i]->params.waitingTime += 1;
			}
		}
		schedule_task_rr(ready_queue, &running_task, &schedule, &numReady, currentTime, quantum_number);
		//delay(500);
		++currentTime;
	} while(numProcesses != num_tasks || numReady != 0 || running_task != NULL);

	for (int i = 0; i < num_tasks; i++) {
		totalResponsetime += task_list[i]->params.responseTime;
		totalWaitingtime += task_list[i]->params.waitingTime;
		totalBursttime += task_list[i]->params.burstTime;
	}

	Interval *s = schedule;
	while(s != NULL) {
		++contextSwitches;
		s = s->nextInterval;
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
	printf("# of Context Switches = %d\n", contextSwitches > 0 ? contextSwitches - 2 : 0);
	printGanttChart(SCHED_NAME, schedule);
	printf("\n<---------------------------------->\n");

	freeSchedule(&schedule);
	free(ready_queue);
}
