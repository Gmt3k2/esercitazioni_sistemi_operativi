#include "monitor_signal_continue.h"

/*---------- FUNZIONI DI UTILITA' ----------*/
void Wait_Sem(int id_sem, int num_sem){
    struct sembuf sem_buf;

    sem_buf.sem_num = num_sem;
    sem_buf.sem_flg = 0;
    sem_buf.sem_op = -1;

    semop(id_sem, &sem_buf, 1);
}
void Signal_Sem(int id_sem, int num_sem){
    struct sembuf sem_buf;

    sem_buf.sem_num = num_sem;
    sem_buf.sem_flg = 0;
    sem_buf.sem_op = 1;

    semop(id_sem, &sem_buf, 1);    
}
int Queue_sem(int id_sem, int num_sem){
    return semctl(num_sem, num_sem, GETNCNT, NULL);
}
/*------------------------------------------------------------*/

/*-------------------- Funzioni del monitor --------------------*/
void init_monitor(Monitor* m, int c){
    m->num_var_cond = c;
    m->mutex = semget(IPC_PRIVATE, 1, IPC_CREAT|0664);
    semctl(m->mutex, 0, SETVAL, 1);//mutex

    m->id_shared = shmget(IPC_PRIVATE, c*sizeof(int), IPC_CREAT|0664);
    m->cond_counts = (int*) shmat(m->id_shared, 0, 0);

    for (int i = 0; i < c; i++)
    {
        m->cond_counts[i] = 0;
    }
    

    m->id_conds = semget(IPC_PRIVATE, c, IPC_CREAT|0664);
    for (int i = 0; i < c; i++)
    {
        semctl(m->id_conds, i, SETVAL, 0);//counter
    }
}

void enter_monitor(Monitor* m){
    Wait_Sem(m->mutex, 0);
}

void leave_monitor(Monitor* m){
    Signal_Sem(m->mutex, 0);
}

void signal_condition(Monitor* m, int id_var){
    if(m->cond_counts[id_var] > 0){
        m->cond_counts[id_var]--;
        Signal_Sem(m->id_conds, id_var);
    }
}

void wait_condition(Monitor* m, int id_var){
    m->cond_counts[id_var]++;
    Signal_Sem(m->mutex, 0);
    Wait_Sem(m->id_conds, id_var);
    Wait_Sem(m->mutex, 0); //credo che appena dopo che viene risvegliato deve mettersi in attesa sul mutex
                             //poichè deve avere di nuovo l'accesso alle variable conditions per poterle controllare
}

int queue_condition(Monitor* m, int id_var){
    return m->cond_counts[id_var];
}

void remove_monitor(Monitor* m){
    semctl(m->mutex, 0, IPC_RMID);
    semctl(m->id_conds, 0, IPC_RMID);
    shmctl(m->id_shared, IPC_RMID, 0);
}