#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>
#include "header.h"

#define NUM_WORKER 3

static struct Coda * coda;
static int ds_sem;
static pthread_t threads[NUM_WORKER];
static int end = 0;

// struttura per passaggio parametri ai thread
typedef struct {
	int id;
	Count* count;
} parametri;

void init_buffer(Count* c){
	//TODO aggiungere codice per inizializzare Count
	c->num_prenotazioni = 0;
	pthread_mutex_init(&(c->mutex), NULL);
}

void remove_buffer(Count* c){
	//TODO aggiungere codice per rimuovere Count
	pthread_mutex_destroy(&(c->mutex));
}


void *worker(void* arg){

	printf("[WORKER] - start del thread...\n");

	parametri* par = (parametri*) arg;
	//TODO: completare il passaggio dei parametri
	
	while (!end) {
		//TODO completare il codice del worker
		// worker preleva dalla coda con funzione "consuma"
		// aggiornamento (eventuale) variabile end se valore ricevuto = -1
		// aggiornamento Count in mutua esclusione
		int valore = consuma(coda, ds_sem);
		if(valore == -1){
			printf("Terminazione server...\n");
			end = 1;
		}
		else{
			pthread_mutex_lock(&par->count->mutex);
			par->count += valore;
			pthread_mutex_unlock(&par->count->mutex);
			printf("[WORKER] Aggiornato il valore del contatore di %d\n", valore);
		}
		
	}
    //TODO uscita dal thread
	pthread_exit(NULL);
}



int main(){

	int i,k;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

	//ottenimento e attach shared memory
	key_t key_shm;
	key_shm = ftok("./", 'm');

	int ds_shm = shmget(key_shm, sizeof(struct Coda), 0);

	coda = (struct Coda*) shmat(ds_shm, NULL, 0);
	
	//ottenimento semafori
	key_t key_sem;
	key_sem = ftok("./", 's');

	ds_sem = semget(key_sem, 4, 0);

	//TODO Creazione e Inizializzazione struttura locale
	Count* count = (Count*) malloc(sizeof(Count));
	init_buffer(count);

	parametri param[NUM_WORKER];

	//Avvio dei thread
	for(i = 0; i < NUM_WORKER;i++){
		printf("[SERVER] - creo thread %d...\n",i);
		param[i].id = i;
		param[i].count = count;
		pthread_create(&(threads[i]), &attr, worker, (void*) param);
		//TODO creazione threads	
	}
	
	for(k=0; k < NUM_WORKER; k++) {
	  pthread_join(threads[k], NULL);
	  printf("Thread worker terminato correttamente!\n"); 
	}
	
	//TODO rimozione struttura locale
	printf("[SERVER] - bye bye...\n");
	
	pthread_attr_destroy(&attr);
	remove_buffer(count);
	free(count);
	pthread_exit(NULL);
}

