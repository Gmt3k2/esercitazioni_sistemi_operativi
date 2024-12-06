#ifndef __MONITOR_HOARE
#define __MONITOR_HOARE

#include <sys/sem.h>
#include <sys/shm.h>

typedef struct
{
    int mutex;
    int id_conds;
    int id_urgents;
    int num_cond_var;
    int id_shared;
    int* cond_counts;
    int* urgent_counts;
}Monitor;

// typedef struct {
//   int mutex;
//   int urgent_sem;
//   int num_var_cond;
//   int id_conds;
//   int id_shared;
//   int *cond_counts;
//   int *urgent_count;

// } Monitor;

void init_monitor(Monitor *, int);
void enter_monitor(Monitor *);
void leave_monitor(Monitor *);
void wait_condition(Monitor*, int);
void signal_condition(Monitor*, int);
void remove_monitor(Monitor *);
int queue_condition(Monitor *,int);


#endif