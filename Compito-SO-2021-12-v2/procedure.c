#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"
#include "semafori.h"

void inizializza_vettore(VettoreProdCons * p) {

    /* TBD: Inizializzare la struttura dati "VettoreProdCons" */
    init_monitor(&p->monitor, 2);
    p->testa = 0;
    p->coda = 0;
    p->count = 0;
}

void produci(VettoreProdCons * p, int valore) {

    /* TBD: Implementare la sincronizzazione, mediante l'algoritmo del vettore circolare di buffer*/
    enter_monitor(&p->monitor);
    if(p->count == DIM){
        wait_condition(&p->monitor, VARCOND_PRODUTTORE);
    }

    p->buffer[p->testa] = valore;
    p->testa = (p->testa + 1)%DIM;
    p->count++;

    signal_condition(&p->monitor, VARCOND_CONSUMATORE);
    leave_monitor(&p->monitor);
}

int consuma(VettoreProdCons * p) {

    int valore;

    /* TBD: Implementare la sincronizzazione, mediante l'algoritmo del vettore circolare di buffer*/
    enter_monitor(&p->monitor);
    if(p->count == 0){
        wait_condition(&p->monitor, VARCOND_CONSUMATORE);
    }

    valore = p->buffer[p->coda];
    p->coda = (p->coda + 1)%DIM;
    p->count--;

    signal_condition(&p->monitor, VARCOND_PRODUTTORE);
    leave_monitor(&p->monitor);
    return valore;

}

void rimuovi_vettore(VettoreProdCons * p) {
    /* TBD: De-inizializzare la struttura dati "VettoreProdCons" */
}


void inizializza_buffer(BufferMutuaEsclusione * p) {

    /* TBD: Inizializzare la struttura dati "BufferMutuaEsclusione" */
    p->buffer = 0;
    p->sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT|0664);
    semctl(p->sem_id, MUTEX, SETVAL, 1);
}

void aggiorna(BufferMutuaEsclusione * p, int valore) {

    /* TBD: Aggiungere la sincronizzazione per la mutua esclusione */
    Wait_Sem(p->sem_id, MUTEX);
    p->buffer += valore;
    printf("[AGGIORNA] Nuovo valore del buffer: %d\n", p->buffer);
    Signal_Sem(p->sem_id, MUTEX);
}

void rimuovi_buffer(BufferMutuaEsclusione * p) {

    /* TBD: De-inizializzare la struttura dati "BufferMutuaEsclusione" */
    semctl(p->sem_id, MUTEX, IPC_RMID);
}


