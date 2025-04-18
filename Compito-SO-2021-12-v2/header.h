#ifndef _HEADER_
#define _HEADER_

#include "monitor_hoare.h"
#include "semafori.h"

#define DIM 3

#define VARCOND_PRODUTTORE 0
#define VARCOND_CONSUMATORE 1

typedef struct {

    int buffer[DIM];

    /* TBD: Aggiungere le variabili per la sincronizzazione con l'algoritmo del vettore circolare di buffer*/
    
    int testa;
    int coda;
    int count;

    Monitor monitor;
} VettoreProdCons;

typedef struct {

    int buffer;

    int sem_id;  // Utilizzare questa variabile per la mutua esclusione

} BufferMutuaEsclusione;

void inizializza_vettore(VettoreProdCons * p);
void produci(VettoreProdCons * p, int valore);
int consuma(VettoreProdCons * p);
void rimuovi_vettore(VettoreProdCons * p);

void inizializza_buffer(BufferMutuaEsclusione * p);
void aggiorna(BufferMutuaEsclusione * p, int valore);
void rimuovi_buffer(BufferMutuaEsclusione * p);


#endif
