#ifndef __MONITOR 
#define __MONITOR

#define CONSUMATORE 0
#define PRODUTTORE 1

typedef struct{
    int mutex;
    int num_var_cond;  //numero di variabili condition
    int id_cons;  //qui viene allocato l'ID vettore di semafori per le variabili condition
    int* cond_counts;
    int id_shared;
}Monitor;

void init_monitor(Monitor *, int);
void remove_monitor(Monitor *);
void enter_monitor(Monitor*);
void leave_monitor(Monitor*);
void wait_condition(Monitor*,int);
void signal_condition(Monitor*,int);

#endif