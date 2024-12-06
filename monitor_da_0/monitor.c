#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "monitor.h"

void init_monitor(Monitor * m, int num_var){
    m->num_var_cond = num_var;
    
    m->mutex = semget(IPC_PRIVATE, 1, IPC_CREAT|0664);  //inizializzo il mutex per il monitor
    m->id_cons = semget(IPC_PRIVATE, num_var, IPC_CREAT|0664);

    semctl(m->mutex, 0, SETVAL, 1);

    for (int i = 0; i < num_var; i++){
        semctl(m->id_cons, i, SETVAL, 0);
    }

    m->id_shared = shmget(IPC_PRIVATE, num_var*sizeof(int), IPC_CREAT|0664);
    m->cond_counts = (int*) shmat(m->id_shared, 0, 0);

    for (int i = 0; i < num_var; i++){
        m->cond_counts[i] = 0;
    }
    
}

void Wait_Sem(int id_sem, int num_sem){
    struct sembuf sem_buf;

    sem_buf.sem_num = num_sem;
    sem_buf.sem_op = -1;
    sem_buf.sem_flg = 0;

    semop(id_sem, &(sem_buf), 1);
}

void Signal_Sem(int id_sem, int num_sem){
    struct sembuf sem_buf;

    sem_buf.sem_num = num_sem;
    sem_buf.sem_op = 1;
    sem_buf.sem_flg = 0;

    semop(id_sem, &(sem_buf), 1);
}

int Queue_Sem(int id_sem, int num_sem){
    return semctl(id_sem, num_sem, GETNCNT, NULL);
}

void enter_monitor(Monitor* m){     //funzione che mi permette di acquisire il monitor
    Wait_Sem(m->mutex, 0);

}

void leave_monitor(Monitor* m){
    Signal_Sem(m->mutex, 0);
}


void remove_monitor(Monitor * m){
    
    semctl(m->mutex, 0, IPC_RMID);
    semctl(m->id_cons, 0, IPC_RMID);
    shmctl(m->id_shared, IPC_RMID, 0);
}

void wait_condition(Monitor* m,int num_sem){
    m->cond_counts[num_sem]++;
    Signal_Sem(m->mutex, 0);
    Wait_Sem(m->id_cons, num_sem);
    m->cond_counts[num_sem]--;
}

void signal_condition(Monitor* m,int num_sem){
    if(m->cond_counts > 0){
        Signal_Sem(m->id_cons, num_sem);
    }
    else{
        Signal_Sem(m->mutex, 0);
    }
}