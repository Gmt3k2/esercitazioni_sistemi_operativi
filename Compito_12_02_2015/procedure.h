#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

#define LIBERO 0
#define IN_USO 1
#define OCCUPATO 2


typedef struct { 
    int operandi[4];   // il buffer contiene un array di operandi (da 2 a 4) 
    int totale_operandi; // il numero di operandi presenti nell'array 
} Buffer;

typedef struct { 
    Buffer elaborazioni[5]; 
    pthread_mutex_t mutex; 
    pthread_cond_t cv_consumatore;
    pthread_cond_t cv_produttore;
    int stato[5];
    int numero_liberi;
    int numero_occupati;
} MonitorElaborazioni;


void* richiedente(void*);
void* elaboratore(void*);