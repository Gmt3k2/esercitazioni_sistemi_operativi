#include "monitor_hoare.h"


/* Funzioni di utilità*/
void Wait_Sem(int sem_id, int num_sem);
void Signal_Sem(int sem_id, int num_sem);
int Queue_Sem(int sem_id, int num_sem);



void init_monitor(Monitor* m, int num_cond){
    int i;
    m->mutex = semget(IPC_PRIVATE, 1, IPC_CREAT|0664);
    semctl(m->mutex, 0, SETVAL, 1);
    
    m->id_urgents = semget(IPC_PRIVATE, 1, IPC_CREAT|0664);
    semctl(m->id_urgents, 0, SETVAL, 0);
    
    m->id_conds = semget(IPC_PRIVATE, num_cond, IPC_CREAT|0664);
    for (i = 0; i < num_cond; i++){
        semctl(m->id_conds, i, SETVAL, 0);
    }
    
    m->id_shared = shmget(IPC_PRIVATE, (num_cond +1) * sizeof(int), IPC_CREAT|0664);
    m->cond_counts = (int*) shmat(m->id_shared, 0, 0);
    
    m->num_var_cond = num_cond;

    
    for (i = 0; i < num_cond; i++){
        m->cond_counts[i] = 0;
    }
    m->urgent_count = m->cond_counts + num_cond;
    *(m->urgent_count) = 0;
}

void remove_monitor(Monitor* m){
    semctl(m->mutex, 0, IPC_RMID, 0);
    semctl(m->id_conds, *(m->cond_counts), IPC_RMID, 0);
    semctl(m->id_urgents, 0, IPC_RMID, 0);
    shmctl(m->id_shared, 0, IPC_RMID);
}

void enter_monitor(Monitor* m){
    Wait_Sem(m->mutex, 0);    
}

void leave_monitor(Monitor* m){
    if(*(m->urgent_count) > 0){
        Signal_Sem(m->id_urgents, 0);
    }
    else{
        Signal_Sem(m->mutex, 0);
    }
}

void signal_cond(Monitor* m, int id_cond){
    (*(m->urgent_count))++;
    if(m->cond_counts[id_cond] > 0){
        Signal_Sem(m->id_conds, id_cond);
        Wait_Sem(m->id_urgents, 0);
    }    
    (*(m->urgent_count))--;
}

void wait_cond(Monitor* m, int id_cond){
    m->cond_counts[id_cond]++;
    if(*(m->urgent_count) > 0){
        Signal_Sem(m->id_urgents, 0);
    }
    else{
        Signal_Sem(m->mutex, 0);
    }
    Wait_Sem(m->id_conds, id_cond);
    m->cond_counts[id_cond]--;
}

int queue_cond(Monitor* m, int id_cond){
    return m->cond_counts[id_cond];
}


void Wait_Sem(int sem_id, int num_sem){
    struct sembuf sem_buf;
    sem_buf.sem_num = num_sem;
    sem_buf.sem_flg = 0;
    sem_buf.sem_op = -1;

    semop(sem_id,&sem_buf, 1);
}

void Signal_Sem(int sem_id, int num_sem){
    struct sembuf sem_buf;
    sem_buf.sem_num = num_sem;
    sem_buf.sem_flg = 0;
    sem_buf.sem_op = 1;

    semop(sem_id,&sem_buf, 1);
}

int Queue_Sem(int sem_id, int num_sem){
    return semctl(sem_id, num_sem, GETNCNT, NULL);
}