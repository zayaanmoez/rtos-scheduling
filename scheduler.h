#ifndef SCHEDULER_H
#define SCHEDULER_H

// Task state to track in task control block

#define STATE_INACTIVE 0
#define STATE_READY 1
#define STATE_WAITING 2
#define STATE_RUNNING 3
#define STATE_TERMINATED 4

#define MAX_TASKS 10
#define NUM_TASKS 5

#define TIME_QUANTUM_3 3
#define TIME_QUANTUM_4 4
#define TIME_QUANTUM_5 5

// Scheduling algorithms
#define ROUND_ROBIN 1
#define PRIORITY 2
#define MODIFIED_RR 3
#define MODULO_BASED_RR 4
#define PRIORITY_BASED_RR 5
#define PBRR_PRIO 6
#define PBRR_RR 7
#define PBRR_MRR 8
#define PBRR_MBRR 9
#define EXIT 0

// Task control block object to track task state
typedef struct sched_params {
	int burstTime;
	int arrivalTime;
	int responseTime;
	int waitingTime;
	int runTime;
} sched_params;

// Task control block object to track task state
typedef struct task_control_block {
	int pid;
	char *pname;
	unsigned short int state;
	int priority;
	sched_params params;
	int attached;
} tcb_t;

// Interval node to track scheduling sequence in linked list
typedef struct interval_node {
	int pid;
	int time;
	struct interval_node *nextInterval;
} Interval;

int task_create();
void detach_tasks();
void init_tasks(int *burstTimes, int * arrivalTimes, int *priorities, int num_tasks);

void start_task(int task_id);
void suspend_task(int task_id);
void ready_task(int task_id);
void terminate_task(int task_id);

int morr_quantum(int *burstTime_morr);
int modified_quantum(int *burstTime_morr);

void prioritySort(tcb_t **task_array, int numReady);
void moduloSort(tcb_t **task_array, int numReady);
void burstSort(tcb_t **task_array, int numReady);

void insertQueue(tcb_t **ready_queue, tcb_t *task, int *numReady);
void insertQueuePrio(tcb_t **ready_queue, tcb_t *task, int *numReady);
tcb_t *popQueue(tcb_t **ready_queue, int *numReady);

void insertInterval(Interval **schedule, int pid, int time);
void freeSchedule(Interval **schedule);
void printGanttChart(char *sched_name, Interval *schedule);

void printSchedulingInfo();

int getArgs(int *sched);

#endif
