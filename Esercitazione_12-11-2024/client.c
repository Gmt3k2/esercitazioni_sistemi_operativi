#include "common.h"

void client(int msgq_guest)
{
	int i;
	/* Inizializzazione Messaggio del processo */

	msgRichiestaClient m;
	m.message_type = MSG_CLIENT_SERVER;
	m.msgProcesso = getpid();

	/* Invio dei 15 messaggi in ciclo, con sleep(1) tra un messaggio e l'altro*/
	
	for (i= 0; i<15; i++)
	{
		if(msgsnd(msgq_guest,(void*) &m, sizeof(msgRichiestaClient) - sizeof(long), 0) == -1){
			printf("Errore nella send\n");
		}
		else{
			printf("Client[%u] manda un messaggio\n", getpid());
		}	
		sleep(1);
	}
	return;
}
