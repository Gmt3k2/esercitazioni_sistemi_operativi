#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>

#define MESSAGGIO_TERMINAZIONE 1
#define NUM_CLIENT 3
#define DIM_MESSAGGIO sizeof(Messaggio) - sizeof(long)
#define NUM_WORKER 2

#define DISPONIBILE 0
#define IN_USO 1
#define OCCUPATO 2

typedef struct{
    long type;
    int num1;
    int num2;
}Messaggio;

typedef struct{
    Messaggio messaggi[NUM_CLIENT];
    int buffer_stati[NUM_CLIENT];

    int numero_liberi;
    int numero_occupati;

    pthread_mutex_t mutex;
    pthread_cond_t cv_consumatore;
    pthread_cond_t cv_produttore;
    
}BufferManagerWorker;
