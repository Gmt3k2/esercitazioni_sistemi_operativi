#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include "prodcons.h"

/* Dichiarazione di metodo privato, che sarà
 * eseguito da un thread "timer" */
static void * thread_timer(void *);


void inizializza_monitor(MonitorProdCons * m) {

    printf("[MONITOR] Inizializzazione monitor\n");

    pthread_mutex_init(&(m->mutex), NULL);
    pthread_cond_init(&m->cv_produttori, NULL);
    pthread_cond_init(&m->cv_consumatori, NULL);

    m->testa = 0;
    m->coda = 0;
    m->uscita_timer = 0;

    pthread_t t;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&t, &attr, thread_timer, m);
}

void distruggi_monitor(MonitorProdCons * m) {

    printf("[MONITOR] Distruzione monitor\n");


    pthread_mutex_lock(&m->mutex);

    printf("[MONITOR] Terminazione thread timer\n");
    m->uscita_timer = 1;

    pthread_mutex_unlock(&m->mutex);

    pthread_mutex_destroy(&m->mutex);
    pthread_cond_destroy(&m->cv_produttori);
    pthread_cond_destroy(&m->cv_consumatori);
}

int produci(MonitorProdCons * m, int val) {

    pthread_mutex_lock(&m->mutex);

    struct timeval t1;
	gettimeofday(&t1,NULL);
    long start_time = t1.tv_sec*1000000 + t1.tv_usec;


    while(m->count == DIM) {

        printf("[MONITOR] Produttore in attesa...\n");

        pthread_cond_wait(&m->cv_produttori, &m->mutex);

        struct timeval t2;
	    gettimeofday(&t2,NULL);
        long current_time = t2.tv_sec*1000000 + t2.tv_usec;

        if(current_time - start_time >= 3000000) {

            printf("[MONITOR] Produttore ha atteso oltre 3 secondi, esce\n");

            pthread_mutex_unlock(&m->mutex);
            return -1;
        }

    }


    m->buffer[m->testa] = val;
    m->testa = (m->testa + 1 ) % DIM;
    m->count++;

    printf("[MONITOR] Produzione %d\n", val);

    pthread_cond_signal(&m->cv_consumatori);

    pthread_mutex_unlock(&m->mutex);
    
    return 0;
}

int consuma(MonitorProdCons * m, int * val) {

    pthread_mutex_lock(&(m->mutex));

    struct timeval t1;
	gettimeofday(&t1,NULL);
    long start_time = t1.tv_sec*1000000 + t1.tv_usec;


    while(m->count == 0) {

        printf("[MONITOR] Consumatore in attesa...\n");

        pthread_cond_wait(&m->cv_consumatori, &m->mutex);

        struct timeval t2;
	    gettimeofday(&t2,NULL);
        long current_time = t2.tv_sec*1000000 + t2.tv_usec;

        if(current_time - start_time >= 3000000) {

            printf("[MONITOR] Consumatore ha atteso oltre 3 secondi, esce\n");

            pthread_mutex_unlock(&m->mutex);
            return -1;
        }

    }


    *val = m->buffer[m->coda];
    m->coda = (m->coda + 1 ) % DIM;
    m->count--;

    printf("[MONITOR] Consumazione %d\n", *val);

    pthread_cond_signal(&m->cv_produttori);

    pthread_mutex_unlock(&m->mutex);


    return 0;
}

void * thread_timer(void * p) {

    printf("[MONITOR] Avvio thread timer\n");

    MonitorProdCons * m = (MonitorProdCons *)p;

    while(1) {

        int uscita = 0;

        sleep(1);

        pthread_mutex_lock(&m->mutex);

        printf("[MONITOR] Thread timer\n");

        pthread_cond_broadcast(&m->cv_produttori);
        pthread_cond_broadcast(&m->cv_consumatori);

        uscita = m->uscita_timer;

        pthread_mutex_unlock(&m->mutex);


        if(uscita != 0) {
            break;
        }
    }

    printf("[MONITOR] Uscita thread timer\n");

    pthread_exit(NULL);
}