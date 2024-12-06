#ifndef __COMMON
#define __COMMON

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/random.h>
#include "monitor_signal_continue.h"

#define N_PRODUTTORI_ALTI 1
#define N_PRODUTTORI_BASSI 3
#define N_CONSUMATORI 1

/*condition variable*/
#define SPAZIO_DISP 0
#define PRODUTTORI_ALTI 1
#define PRODUTTORI_BASSI 2

#define DIM 3

typedef struct { 
    int buffer[DIM]; 
    int testa; 
    int coda;
    int count;
    Monitor m;   // utilizzare la libreria di procedure allegate 
} PriorityProdCons;

void inizializza_prod_cons(PriorityProdCons * p); 
void produci_alta_prio(PriorityProdCons * p); 
void produci_bassa_prio(PriorityProdCons * p); 
void consuma(PriorityProdCons * p); 
void rimuovi_prod_cons(PriorityProdCons * p);


#endif