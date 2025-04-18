#ifndef __PROCEDURE_H
#define __PROCEDURE_H

#include "monitor_hoare.h"
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>


#define DIM_BUFFER 5

typedef struct{
    int buffer[DIM_BUFFER];
    
    int testa;
    int coda;
    int count;

    Monitor monitor;
}BufferCircolare;

typedef struct{
    int value;
    int buffer_pieno;
    int buffer_vuoto;
    Monitor monitor;
}BufferSingolo;

#define VARCOND_PRODUTTORE 0
#define VARCOND_CONSUMATORE 1

#define SPAZIO_DISPONIBILE 0
#define MESSAGGIO_DISPONIBILE 1


void inizializza(BufferCircolare* bc, int num_cond);
void produttore(BufferCircolare * bc, BufferSingolo* bs);
void consumatore(BufferCircolare * bc, BufferSingolo* bs);

#endif