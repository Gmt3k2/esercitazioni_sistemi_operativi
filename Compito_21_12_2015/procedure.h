#ifndef __PROCEDURE_H
#define __PROCEDURE_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/random.h>
#include <sys/wait.h>

#define DIM_BUF 10

typedef struct {
    int indirizzo;
    int dato;
} Buffer;

typedef struct {
    Buffer vettore[DIM_BUF];
    
    int count;
    int testa;
    int coda;

    pthread_mutex_t mutex;
    pthread_cond_t cv_produttore;

}GestioneIO;

void Inizializza(GestioneIO * g);
void Produci(GestioneIO * g, Buffer * b);
int Consuma(GestioneIO * g, Buffer * b);


#endif