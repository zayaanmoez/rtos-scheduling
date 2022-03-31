
// Task state to track in task control block
#define STATE_RUNNING 0;
#define STATE_READY 1;
#define STATE_WAITING 2;
#define STATE_INACTIVE 3;

#define MAX_TASKS 10

// Task control block object to track task state
typedef struct task_control_block {
	int pid;
	void (*fptr)(void *p);
	void *args;
	unsigned short int state;
	unsigned int delay;
	int priority;
	int attached;
} tcb_t;

int task_create(void (*fptr)(void *p), void * args, int priority);

void start_task(int task_id);
void delay(unsigned int d);
void suspend_task();


void task1(void *p);
void task2(void *p);
void task3(void *p);
void task4(void *p);
void task5(void *p);
void task6(void *p);
