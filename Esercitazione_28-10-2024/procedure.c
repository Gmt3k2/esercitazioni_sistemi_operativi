			/*----IMPLEMENTAZIONE DELLE PROCEDURE----*/


#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <math.h>

//Procedure per la sincronizzazione sui semafori

void Wait_Sem(int id_sem, int numsem){
	struct sembuf sem_buf;
	sem_buf.sem_num = numsem;
	sem_buf.sem_op = -1;
	sem_buf.sem_flg = 0;

	semop(id_sem, &sem_buf, 1);
}


void Signal_Sem (int id_sem,int numsem){
	struct sembuf sem_buf;
	sem_buf.sem_num = numsem;
	sem_buf.sem_op = 1;
	sem_buf.sem_flg = 0;

	semop(id_sem, &sem_buf, 1);
}

//Calcolo statistiche
double media(int* v){
	int i;
	double somma;
	somma = 0.0f;
	for(i = 0; i < N;i++){
		somma += v[i];
	}
	return (double)(somma/N);
}

double var(int* v,double media){
	int i;
	double somma_par,temp;
	somma_par = 0.0f;
	for(i = 0; i < N;i++){
		temp = v[i]-media;
		somma_par += (temp * temp);
	}
	return (double)(somma_par/N-1);
}

// Consumo
void consuma_elementi(int sem, BufferCircolare* buf){
	int val[N];
	double m;
	//Calcola media e varianza
	/*Qua va il codice della sincronizzazione*/	
	Wait_Sem(sem, BUFFER_PIENO);
	Wait_Sem(sem, MUTEXC);
	
	printf("[Consumatore %d] consumo elementi\n",getpid());	
	for (int i = 0; i < N; i++){
		val[i] = buf->elementi[i];
	}
	
	m = media(val);

	printf("[Consumatore %d] media %f, varianza %f\n",getpid(),m,var(val,m));
	
	//ripristino le condizioni per una seconda esecuzione
	buf->count = 0;
	buf->testa = 0;
	buf->coda = 0;

	Signal_Sem(sem, MUTEXC);
	for (int i = 0; i < 5; i++)
		Signal_Sem(sem, SPAZIO_DISP);
	
}

//Produzione
void produci_elemento(int sem,BufferCircolare* buf){
	int val;
	srand(time(NULL) * getpid());
	Wait_Sem(sem, SPAZIO_DISP);
	Wait_Sem(sem, MUTEXP);

	val =  (1+rand()%11);
	buf->elementi[buf->testa] = val;
	buf->count++;
	printf("[Produttore %d] ho prodotto %d in posizione %d\n",getpid(),val,buf->testa);
	buf->testa = (buf->testa + 1)%N;

	Signal_Sem(sem, MUTEXP);
	if(buf->count == N)
		Signal_Sem(sem, BUFFER_PIENO);
}
