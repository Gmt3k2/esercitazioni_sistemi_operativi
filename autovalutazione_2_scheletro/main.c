#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <time.h>

#include "procedure.h"

int main() {

	int id_queue, id_shm;
	key_t chiave_shm = IPC_PRIVATE;
	key_t chiave_mbx = IPC_PRIVATE;

	//memoria contenente i risultati
	id_shm = shmget(chiave_shm, sizeof(struct MonitorRisultati), IPC_CREAT|0664);
	//coda mailbox
	id_queue = msgget(chiave_mbx, IPC_CREAT|0664);

	if( id_shm < 0 || id_queue < 0) {
		perror("errore creazione risorse");
		exit(1);
	}

	struct MonitorRisultati * ls = shmat(id_shm, NULL, 0);

	init_monitor(&(ls->m), 2);

	ls->numero_lettori = 0;
	ls->numero_scrittori = 0;

	pid_t pid;

	int j;
	for(j=0; j < N_PRINTERS ; j++) {
		pid = fork();
		if(pid < 0){
			perror("Errore nella creazione processo figlio");
		}
		else if (pid == 0)
		{
			printer(ls);
			exit(0);
		}
	}
	
	for(j=0; j < N_CALCOLATORI ; j++) {
		pid = fork();
		if(pid < 0){
			perror("Errore nella creazione processo figlio");
		}
		else if (pid == 0)
		{
			calcolo(ls, j+1, id_queue);
			exit(0);
		}
	}

	for(j=0; j < N_GENERATORI ; j++) {
		pid = fork();
		if(pid < 0){
			perror("Errore nella creazione processo figlio");
		}
		else if (pid == 0)
		{
			generazione(j+1, id_queue);
			exit(0);
		}
		
	}





	for(j=0; j<N_CALCOLATORI+N_GENERATORI+N_PRINTERS; j++)
		wait(NULL);

	shmctl(id_shm, IPC_RMID, 0);
	msgctl(id_queue, IPC_RMID, 0);
	remove_monitor(&(ls->m));
	return 0;
}





