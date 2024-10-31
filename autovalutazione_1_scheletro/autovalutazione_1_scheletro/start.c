#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

#include "header.h"


int main(){
	pid_t pid_ex,pid;
	int st,i;
	int shmid,semid;
	BufferCircolare* buf;

	key_t key_shm,key_sem;
	key_shm = ftok(".", 'm');
	key_sem = ftok(".", 's');
	
	shmid = shmget(key_shm,sizeof(BufferCircolare), IPC_CREAT | IPC_EXCL |0664);

	if(shmid <0){
		perror("Errore nella creazione della shared memory");
	}

	buf = shmat(shmid, NULL, 0);
	for (int i = 0; i <SIZE; i++)
	{
		buf->stati[i] = LIBERO;
	}
	

	if(buf == (void*) 0){
		perror("Errore");
	}

	semid = semget(key_sem,6,IPC_CREAT|0664);

	if(shmid < 0){
		perror("ERrore nella creazione della shared memory");
	}

	printf("[MASTER] - Shmid : %d, Semid: %d\n",shmid,semid);

	buf = (BufferCircolare *) shmat(shmid, NULL, 0);

	//inizializzazione semafori di cooperazione
	semctl(semid, SPAZIO_DISPONIBILE, SETVAL, NPROD);
	semctl(semid, DISPONIBILE_1, SETVAL, 0);
	semctl(semid, DISPONIBILE_2, SETVAL, 0);
	semctl(semid, DISPONIBILE_3, SETVAL, 0);

	//inizializzazione mutex
	semctl(semid, MUTEX_C, SETVAL, 1);
	semctl(semid, MUTEX_P, SETVAL, 1);


	srand(time(NULL));
	int val;

	for(i = 0; i < NCONS+NPROD; i++){
		pid = fork();

		if (pid == 0 && i < NPROD)
		{
			produci(buf, i+1, semid);
			exit(0);
		}

		if (pid == 0 && i >= NPROD)
		{
			consuma(buf, i-2, semid);
			exit(0);
		}
		val =  (1+rand()%3);
		sleep(val);

	}
	
	for(i = 0; i < NCONS+NPROD ; i++){
		pid_ex = wait(NULL);
		printf("[MASTER] - Il processo %d ha terminato l'esecuzione\n",pid_ex);			
	}

	shmctl(shmid, IPC_RMID, NULL);
	semctl(semid, 0, IPC_RMID);
	return 0;
}
