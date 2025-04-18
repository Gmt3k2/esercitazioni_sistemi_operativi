#include <stdio.h>

#include "prodcons.h"


void init_monitor(MonitorProdCons * p) {

    /* TBD: Inizializzare l'oggetto monitor */
    pthread_cond_init(&(p->cv_consumatore), NULL);
    pthread_cond_init(&(p->cv_produttore), NULL);
    pthread_cond_init(&(p->cv_conteggio), NULL);

    p->conteggio_consumazioni = 0;
    p->testa = 0;
    p->coda = 0;
    p->count = 0;
}

void produzione(MonitorProdCons * p, int val) {

    /* TBD: Completare la sincronizzazione, in base allo schema
            del vettore di buffer circolare */
    pthread_mutex_lock(&(p->mutex));
    while(p->count == DIM){
        pthread_cond_wait(&(p->cv_produttore), &(p->mutex));
    }

    p->buffer[p->coda] = val;
    p->coda = (p->coda + 1)%DIM;
    p->count++;

    pthread_cond_signal(&(p->cv_consumatore));
    pthread_mutex_unlock(&(p->mutex));
}

int consumazione(MonitorProdCons * p) {

    int val;

    /* TBD: Completare la sincronizzazione, in base allo schema
            del vettore di buffer circolare */
    pthread_mutex_lock(&(p->mutex));
    while(p->count == 0){
        pthread_cond_wait(&(p->cv_consumatore), &(p->mutex));
    }

    val = p->buffer[p->testa];
    p->testa = (p->testa + 1)%DIM;
    p->count++;
    p->conteggio_consumazioni++;

    pthread_cond_signal(&(p->cv_produttore));
    pthread_cond_signal(&(p->cv_conteggio));
    pthread_mutex_unlock(&(p->mutex));
    return val;
}

int attendi_consumazioni(MonitorProdCons * p, int soglia) {

    int n;

    /* TBD: Sospendere il thread finché il "conteggio_consumazioni" non sia diventato
            maggiore o uguale a "soglia"
     */
    pthread_mutex_lock(&(p->mutex));
    while (p->conteggio_consumazioni < soglia)
    {
        pthread_cond_wait(&(p->cv_conteggio), &(p->mutex));
    }
    n = p->conteggio_consumazioni;

    p->conteggio_consumazioni = 0;
    
    pthread_mutex_unlock(&(p->mutex));
    return n;
}

void remove_monitor(MonitorProdCons * p) {

    /* TBD: Disattivare le variabili per la sincronizzazione */
    pthread_mutex_destroy(&(p->mutex));
    pthread_cond_destroy(&(p->cv_consumatore));
    pthread_cond_destroy(&(p->cv_produttore));
    pthread_cond_destroy(&(p->cv_conteggio));
}