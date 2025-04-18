#include "procedure.h"

void Inizializza(GestioneIO * g){
    pthread_mutex_init(&(g->mutex), NULL);
    pthread_cond_init(&(g->cv_produttore), NULL);

    g->coda = 0;
    g->testa = 0;
    g->count = 0;
}
void Produci(GestioneIO * g, Buffer * b){
    pthread_mutex_lock(&g->mutex);
    while(g->count == DIM_BUF){
        pthread_cond_wait(&g->cv_produttore, &g->mutex);
    }
    
    g->vettore[g->testa].indirizzo = b->indirizzo;
    g->vettore[g->testa].dato = b->dato;
    printf("[PRODUTTORE] Ho prodotto il dato (%d,%d) in posizione %d\n", b->indirizzo, b->dato, g->testa);

    g->testa = (g->testa + 1)%DIM_BUF;
    g->count++;

    pthread_mutex_unlock(&g->mutex);
}
int Consuma(GestioneIO * g, Buffer * b){
    pthread_mutex_lock(&g->mutex);
    
    if(g->count == 0){
        pthread_mutex_unlock(&g->mutex);
        return 1;
    }
    else
    {
        b->indirizzo = g->vettore[g->coda].indirizzo;
        b->dato = g->vettore[g->coda].dato;
        g->coda = (g->coda + 1)%DIM_BUF;
        g->count--;
        pthread_cond_signal(&g->cv_produttore);
        pthread_mutex_unlock(&g->mutex);
        return 0;

    }
    
}