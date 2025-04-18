/* PROCESSO PRODUTTORE */

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"


int main(){
	printf("[PRODUTTORE] <%d> \n",getpid());
	key_t key_shm = ftok("./", 'b');
	key_t key_sem = ftok("./", 's');

	int shm_id = shmget(key_shm, sizeof(BufferCircolare), 0);
	
	if(shm_id < 0)
		perror("Error:");

	BufferCircolare* b = shmat(shm_id, NULL, 0);
	if(b == (void *) 0)
		perror("Error:");
	
	int sem_id = semget(key_sem, 4, 0);
	
	produci_elemento(sem_id, b);
	
	return 1;
}
