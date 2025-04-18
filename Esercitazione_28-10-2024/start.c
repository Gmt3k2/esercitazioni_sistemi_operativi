#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>

#include "header.h"

#define N_PROC 11


int main(){
	pid_t pid_ex,pid;
	int st,i;
	//creare le risorse IPC necessarie
	key_t key_shm = ftok("./", 'b');
	key_t key_sem = ftok("./", 's');
	
	int shm_id = shmget(key_shm, sizeof(BufferCircolare), IPC_CREAT|0664);
	
	if(shm_id < 0)
		perror("Error:");

	BufferCircolare* b = shmat(shm_id, NULL, 0);
	if(b == (void *) 0)
		perror("Error:");

	b->count = 0;
	b->testa = 0;
	b->coda = 0;	


	int sem_id = semget(key_sem, 4, IPC_CREAT|0664);
	semctl(sem_id, MUTEXC, SETVAL, 1);
	semctl(sem_id, MUTEXP, SETVAL, 1);
	semctl(sem_id, SPAZIO_DISP, SETVAL, N);
	semctl(sem_id, BUFFER_PIENO, SETVAL, 0); //parte a zero perchè dovrebbe essere bloccato

	//istanziare i processi
	for(i = 0; i < N_PROC;i++){
		pid = fork();
		if(pid < 0){
			printf("Errore nella fork\n");
			exit(-1);
		}	
		else if(pid == 0){
			if(i == 0){
				//exec consumatore
				execl("./consumatore", "consumatore", NULL);
				exit(-1);
			}else{
				//exec produttore
				execl("./produttore", "produttore", NULL);
				exit(-1);
			}	
			_exit(0);			
		}
		sleep(1);
	}
	
	for(i = 0; i < N_PROC ; i++){
		pid_ex = wait(&st);
		printf("[MASTER] - Il processo %d ha terminato l'esecuzione\n",pid_ex);			
	}

	semctl(sem_id, 4, IPC_RMID);
	shmctl(shm_id, IPC_RMID, 0);
	return 0;
}
