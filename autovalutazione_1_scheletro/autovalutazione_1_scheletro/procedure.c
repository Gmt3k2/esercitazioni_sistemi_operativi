			/*----IMPLEMENTAZIONE DELLE PROCEDURE----*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <math.h>

//Procedure per la sincronizzazione sui semafori

void Wait_Sem(int id_sem, int numsem)     {
	int err;
	struct sembuf sem_buf;
	sem_buf.sem_num = numsem;
	sem_buf.sem_op = -1;
	sem_buf.sem_flg = 0;
	err = semop(id_sem,&sem_buf,1);
	if(err < 0){
		perror("ERROR sem");
	}

}


void Signal_Sem (int id_sem,int numsem)     {
	int err;
	struct sembuf sem_buf;
	sem_buf.sem_num = numsem;
	sem_buf.sem_op = 1;
	sem_buf.sem_flg = 0;
	err = semop(id_sem,&sem_buf,1);
	if(err < 0){
		perror("ERROR sem");
	}

}

// Consumo
void consuma(BufferCircolare* buf, int chiave, int semid){
	printf("[Consumatore %d] iniziato con chiave %d \n",getpid(), chiave);
	int sem;
	int value;
	int index;

	for(int i=0; i<3; i++){
		Wait_Sem(semid, chiave);

		Wait_Sem(semid, MUTEX_C); //inizio sezione critica
		index = 0;
		while(buf->stati[index] != OCCUPATO || buf->elementi[index].chiave != chiave){
			index++;		
		}
		buf->stati[index] = IN_USO;
		Signal_Sem(semid, MUTEX_C); //fine sezione critica

		printf("[Consumatore %d] consumo elemento in posizione %d\n",getpid(), index);	

		value = buf->elementi[index].valore;
		buf->stati[index] = VUOTO;
		int val_sleep =  (1+rand()%3);
		sleep(val_sleep);

		printf("[Consumatore %d] valore letto: %d\n",getpid(), value);
		
		Signal_Sem(semid, SPAZIO_DISPONIBILE);
	}
}

//Produzione
void produci(BufferCircolare* buf, int chiave, int semid){
	printf("[Produttore %d] iniziato con chiave %d\n",getpid(), chiave);
	int val_sleep;
	int value;
	int sem;
	int index;

	for(int i=0; i<3; i++){
		Wait_Sem(semid, SPAZIO_DISPONIBILE);


		Wait_Sem(semid, MUTEX_P); //sezione critica
		index = 0;
		while(buf->stati[index] != LIBERO){
			index++;
		}
		buf->stati[index] = IN_USO;
		Signal_Sem(semid, MUTEX_P);  //fine sezione critica
		printf("[Produttore %d] entrato con chiave %d \n",getpid(), chiave);
		
		value = rand()%10;
		buf->elementi[index].chiave = chiave;
		buf->elementi[index].valore = value;
		buf->stati[index] = PIENO;
		val_sleep =  (1+rand()%3);
		sleep(val_sleep);
		
		printf("[Produttore %d] ho prodotto %d, con chiave %d, in posizione %d\n",getpid(),value, chiave, index);
	
		Signal_Sem(semid, chiave);
		
	}
}
