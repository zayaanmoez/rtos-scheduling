#ifndef POLICY_H
#define POLICY_H

void priorityScheduler(tcb_t **task_list, int num_tasks);
void rrScheduler(tcb_t **task_list, int num_tasks, int quantum_number);
void mrrScheduler(tcb_t **task_list, int num_tasks, int quantum_number);
void morrScheduler(tcb_t **task_list, int num_tasks, int quantum_number);
void priorityRoundRobinScheduler(tcb_t **task_list, int num_tasks, int quantum);

#endif
