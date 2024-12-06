#ifndef __MONITOR_SIGNAL_CONTUNUE
#define __MONITOR_SIGNAL_CONTUNUE

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>

typedef struct{
    int num_var_cond;
    int mutex;
    int id_conds;
    int id_shared;
    int* cond_counts;
    
}Monitor;


void init_monitor(Monitor* m, int c);
void enter_monitor(Monitor* m);
void leave_monitor(Monitor* m);
void remove_monitor(Monitor* m);
void signal_condition(Monitor* m, int id_var);
void wait_condition(Monitor* m, int id_var);
int queue_condition(Monitor* m, int id_var);

#endif