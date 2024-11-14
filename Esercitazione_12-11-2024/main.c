#include "common.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define NR_CLIENT  5

/* ID - Code di Messaggi */
int msgq_guest;
int msgq_print;

int main(int argc,char*argv[])
{
	int i;
	key_t chiave = IPC_PRIVATE;
	/*Richiesta code di messaggi IPC*/
	msgq_guest = msgget(chiave, IPC_CREAT|0664);
	if(msgq_guest < 0){
		perror("Errore nella creazione coda guest");
		exit(1);
	}
	
	msgq_print = msgget(chiave, IPC_CREAT|0664); 
	if (msgq_print < 0){
		perror("Errore nella creazione coda print");
		exit(1);
	}


	/*Creazione processo 'printer'*/
	pid_t pid = fork();
	if(pid == 0){
		printer(msgq_print);
		exit(0);
	}

	/*Creazione processo 'server'*/
	pid_t pid1 = fork();
	if(pid1 == 0){
		server(msgq_guest, msgq_print);
		exit(0);
	}

	/*Creazione processi 'client'*/
	for (int i = 0; i < NR_CLIENT; i++)
	{
		pid_t pid2 = fork();
		if (pid2 == 0)
		{
			client(msgq_guest);
			exit(0);
		}
	}
	

	/*Attesa terminazione dei client*/
	for (int i = 0; i < NR_CLIENT; i++)
	{
		wait(NULL);
	}
	

	/*Invio messaggio di terminazione al server*/
	msgRichiestaClient endMessage;
	endMessage.message_type = MSG_EXIT;
	endMessage.msgProcesso = -1;
	msgsnd(msgq_guest, &endMessage, sizeof(msgRichiestaClient) - sizeof(long), 0);
	
	/*Attesa terminazione processi 'server' e 'printer'*/
	for(i=0; i<2; i++){
		wait(0);
	}
	/*Rimozione code IPC e uscita*/
	msgctl(msgq_guest, IPC_RMID, 0);
	msgctl(msgq_print, IPC_RMID, 0);

	return 0;
}
