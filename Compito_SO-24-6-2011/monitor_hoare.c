#include "monitor_hoare.h"

#include <sys/sem.h>

/*-------Procedure di utilità-------*/

void Signal_sem(int, int);
void Wait_sem(int, int);
int queue_sem(int, int);

/*-------Procedure per i monitor-------*/
void init_monitor(Monitor * m, int num_cond){
    m->num_cond_var = num_cond;
    m->mutex = semget(IPC_PRIVATE, 1, IPC_CREAT|0664);
    semctl(m->mutex,0,SETVAL, 1);

    m->id_urgents = semget(IPC_PRIVATE, 1, IPC_CREAT|0664);
    semctl(m->id_urgents, 0, SETVAL, 0);

    m->id_conds = semget(IPC_PRIVATE, num_cond, IPC_CREAT|0664);
    for (int i = 0; i < num_cond; i++){
        semctl(m->id_conds, i, SETVAL, 0);
    }
    
    m->id_shared = shmget(IPC_PRIVATE, (num_cond+1)*num_cond*sizeof(int), IPC_CREAT|0664);
    m->cond_counts = (int*)shmat(m->id_shared, 0, 0);

    m->urgent_counts = m->cond_counts + num_cond;

    for (int i = 0; i < num_cond; i++){
        m->cond_counts[i] = 0;
    }
    *(m->urgent_counts) = 0;
}

void enter_monitor(Monitor * m){
    Wait_sem(m->mutex,0);
}

//Quando esco dal monitor se ho sospeso un processo lo faccio riprendere
void leave_monitor(Monitor * m){
    if (*(m->urgent_counts) > 0){
        Signal_sem(m->id_urgents, 0);
    }
    else{
        Signal_sem(m->mutex, 0);
    }
}

void wait_condition(Monitor* m, int num_cond){
    m->cond_counts[num_cond]++;
    //se ho un processo in coda urgente faccio riprendere prima lui 
    if(*(m->urgent_counts)>0){
        Signal_sem(m->id_urgents, 0);
    }
    else{
        Signal_sem(m->mutex, 0);
    }
    Wait_sem(m->id_conds, num_cond);
    m->cond_counts[num_cond]--;
}

void signal_condition(Monitor* m, int num_cond){
    (*(m->urgent_counts))++;
    if(m->cond_counts[num_cond] > 0){
        Signal_sem(m->id_conds, num_cond);
        Wait_sem(m->id_urgents, 0);
    }
    (*(m->urgent_counts))--;
}

void remove_monitor(Monitor * m){
    semctl(m->mutex, 0, IPC_RMID, 0);
    semctl(m->id_urgents, 0, IPC_RMID, 0);
    semctl(m->id_conds, m->num_cond_var, IPC_RMID, 0);
    shmctl(m->id_shared, IPC_RMID, 0);
}

int queue_condition(Monitor * m, int num_sem){
    return m->cond_counts[num_sem];
}

/* Implementazione procedure */
void Signal_sem(int id_sem, int num_sem){
    struct sembuf sem_buf;
    
    sem_buf.sem_num = num_sem;
    sem_buf.sem_flg = 0;
    sem_buf.sem_op = 1;

    semop(id_sem, &sem_buf, 1);
}
void Wait_sem(int id_sem, int num_sem){
    struct sembuf sem_buf;
    
    sem_buf.sem_num = num_sem;
    sem_buf.sem_flg = 0;
    sem_buf.sem_op = -1;

    semop(id_sem, &sem_buf, 1);
}
int queue_sem(int id_sem, int num_sem){
    return semctl(id_sem, num_sem, GETNCNT, 0);
}

