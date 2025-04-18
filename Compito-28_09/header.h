#ifndef __HEADER
#define __HEADER
#include <pthread.h>

#define DIM_BUFFER 5

#define NUM_REQ 10

#define MUTEX_C 0
#define MUTEX_P 1
#define MESSAGGIO_DISP 2
#define SPAZIO_DISP 3

struct Coda {
    int buffer[DIM_BUFFER];
    int testa;
    int coda;
};

void produci(struct Coda *, int id_sem, int num_prenotazioni);
int consuma(struct Coda *, int id_sem);

// struttura Count con num_prenotazioni condivisa lato server
typedef struct{
	//TODO aggiungere variabile per la sincronizzazione
	int num_prenotazioni;
    pthread_mutex_t mutex;
}Count;

#endif
