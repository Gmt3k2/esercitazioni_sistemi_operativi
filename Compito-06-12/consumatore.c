#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"


int main() {
	int pid;
	//TODO: completare l'inizializzazione code
    key_t msg_req_key = ftok("./", 'q');
	key_t msg_res_key = ftok("./", 'r');

	int queue_req = msgget(msg_req_key, 0|0664); //le code vengono già create nel processo main
	int queue_res = msgget(msg_res_key, 0|0664);

	int i;
	for (i=0; i < NUM_CONS; i++) {
		req msg;
		//TODO preparazione messaggio
		msg.type = MSG_TYPE_REQUEST;
		printf("CONSUMATORE %d: invio richiesta \n",getpid());
		//TODO invio richiesta
		msgsnd(queue_req, &msg, sizeof(long), 0);
		res risp;
		//TODO ricezione risposta
		msgrcv(queue_res, &risp, sizeof(res)-sizeof(long), MSG_TYPE_RESPONSE, 0);
		printf("CONSUMATORE %d: letto valore: %d\n",getpid(),risp.value);
		sleep(1);
	}
	//TODO rimozione code
	msgctl(queue_req, IPC_RMID, 0);
	msgctl(queue_res, IPC_RMID, 0);
	return 0;
}
