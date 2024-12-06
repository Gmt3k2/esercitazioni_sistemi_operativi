#ifndef __MONITOR_HOARE
#define __MONITOR_HOARE

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/shm.h>

typedef struct{
    int num_var_cond;
    int mutex;
    int id_urgents;
    int id_conds;
    int id_shared;
    int* cond_counts;
    int* urgent_count;
}Monitor;

void init_monitor(Monitor* m, int num_cond);
void remove_monitor(Monitor* m);
void enter_monitor(Monitor* m);
void leave_monitor(Monitor*);
void signal_cond(Monitor* m, int id_cond);
void wait_cond(Monitor* m, int id_cond);
int queue_cond(Monitor* m, int id_cond);

#endif