#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "header.h"

void updater(int queue_req) {
	int i;
	for (i=0; i < NUM_UPDATES; i++) {
		int val = rand()%100 - 40;
		req msg;
		/*TODO completare con preparazione e invio messaggio di 
		richiesta aggiornamento 'msg' con il valore 'val' */
		msg.type = MSG_UPDATE_REQUEST;
		msg.num = val;
		msg.pid = getpid();
		msgsnd(queue_req, &msg, sizeof(req)- sizeof(long), 0);
		printf("UPDATER: invio richiesta aggiornamento a valore %d\n",val);
	
		sleep(2);
	}
}

void reader(int queue_req, int queue_res) {
	int i;
	for (i=0; i < NUM_READS; i++) {
		req msg;
		/*TODO completare con preparazione e invio messaggio di 
		richiesta di lettura 'msg' */
		msg.type = MSG_READ_REQUEST;
		msg.pid = getpid();
		msg.num = -1;
		msgsnd(queue_req, &msg, sizeof(req)- sizeof(long), 0);
		printf("READER %d: invio richiesta lettura\n",getpid());
	
		 

		/*TODO completare con ricezione risposta */
		res msg_res;
		msgrcv(queue_res, &msg_res, sizeof(res)-sizeof(long), getpid(), 0);
		int value = msg_res.num;
		printf("READER %d: letto valore: %d\n",getpid(),value);
		sleep(1);
	}
}

int main() {
	
	key_t msg_req_key = ftok("./", 'q');
	key_t msg_res_key = ftok("./", 'r');

	int queue_req = msgget(msg_req_key, 0);
	int queue_res = msgget(msg_res_key, 0);

	srand(time(NULL));
	
	// TODO creare un processo figlio che esegue la funzione 'updater'
	pid_t pid = fork();
	if(pid < 0){
		perror("Error");
		exit(-1);
	}
	else if(pid == 0){
		updater(queue_req);
		exit(0);
	}

	// TODO creare NUM_READERS processi figlio che eseguono la funzione 'reader'
	int i;
	for (int i = 0; i < NUM_READERS; i++) {
		// TODO creazione del processo reader
		pid = fork();
		if(pid < 0){
			perror("Error:");
			exit(-1);
		}
		else if(pid == 0){
			reader(queue_req, queue_res);
			exit(0);
		}
	}

	// TODO mettere in attesa il padre dei processi figli
	for (int i = 0; i < NUM_READERS +1; i++){
		wait(NULL);
		printf("Processo terminato correttamente!\n");

	}
	
	// TODO completare con rimozione delle code
	msgctl(msg_req_key, IPC_RMID, 0);
	msgctl(msg_res_key, IPC_RMID, 0);

	return 0;
}