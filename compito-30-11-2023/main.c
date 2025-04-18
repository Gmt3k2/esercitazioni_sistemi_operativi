#include <stdio.h>
#include <stdlib.h>

#include "prodcons.h"

#define NUM_THREAD_PROD 3
#define NUM_THREAD_CONS 3
#define NUM_PRODUZIONI 10
#define NUM_CONSUMAZIONI 10
#define CONTEGGIO 5

void * thread_produttore(void *);
void * thread_consumatore(void *);
void * thread_conteggio(void *);


int main() {

    pthread_t th_consumatore[NUM_THREAD_CONS];
    pthread_t th_produttore[NUM_THREAD_PROD];
    pthread_t th_conteggio;


    /* TBD: Creare un oggetto monitor di tipo "MonitorProdCons" */
    MonitorProdCons* buffer = (MonitorProdCons*) malloc(sizeof(MonitorProdCons));

    init_monitor(buffer);

    for(int i=0; i<NUM_THREAD_PROD; i++) {

        /* TBD: Creare un thread produttore, fargli eseguire la funzione "thread_produttore" */
        if(pthread_create(&th_produttore[i], NULL, thread_produttore, (void *) buffer) == 0)
            printf("Thread produttore creato correttamente\n");
        
    }

    for(int i=0; i<NUM_THREAD_CONS; i++) {

        /* TBD: Creare un thread consumatore, fargli eseguire la funzione "thread_consumatore" */
        if(pthread_create(&th_consumatore[i], NULL, thread_consumatore, (void *) buffer) == 0)
            printf("Thread consumatore creato correttamente\n");
        
    }

    /* TBD: Creare un thread di "conteggio", fargli eseguire la funzione "thread_conteggio" */
    pthread_create(&(th_conteggio), NULL, thread_conteggio, (void*) buffer);


    /* TBD: Attendere la terminazione dei thread */
    for (int i = 0; i < NUM_THREAD_CONS; i++)
    {
        if(pthread_join(th_consumatore[i], NULL) == 0)
            printf("Thread consumatore terminato correttamente\n");
    }
    for (int i = 0; i < NUM_THREAD_PROD; i++)
    {
        if(pthread_join(th_produttore[i], NULL) == 0)
            printf("Thread produttore terminato correttamente\n");
    }    
    if(pthread_join(th_conteggio, NULL) == 0)
        printf("Thread conteggio terminato correttamente\n");

    remove_monitor(buffer);

    /* TBD: Deallocare l'oggetto monitor */
    free(buffer);
    return 0;
}

void * thread_produttore(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    MonitorProdCons* buffer = (MonitorProdCons *) x;

    for(int i=0; i<NUM_PRODUZIONI; i++) {

        /* TBD: Completare la chiamata a produzione() */

        int val = rand() % 10;
        produzione(buffer, val);

        printf("[PRODUTTORE] Ho prodotto: %d\n", val);

    }

    return NULL;
}

void * thread_consumatore(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    MonitorProdCons* buffer = (MonitorProdCons*) x;
    for(int i=0; i<NUM_CONSUMAZIONI; i++) {

        /* TBD: Completare la chiamata a consumazione() */

        int val = consumazione(x);

        printf("[CONSUMATORE] Ho consumato: %d\n", val);
    }

    return NULL;
}


void * thread_conteggio(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    MonitorProdCons* buffer = (MonitorProdCons*) x;
    int consumazioni_effettuate = 0;

    int consumazioni_da_effettuare = NUM_CONSUMAZIONI * NUM_THREAD_CONS;

    while(consumazioni_da_effettuare > 0) {

        int conteggio;

        if( consumazioni_da_effettuare >= CONTEGGIO ) {

            // Vi sono 5 o più consumazioni ancora da effettuare,
            // si pone a 5 il conteggio da attendere
            conteggio = CONTEGGIO;
        }
        else {

            // Vi sono meno di 5 consumazioni ancora da effettuare,
            // si pone il conteggio da attendere a "consumazioni_da_effettuare"
            conteggio = consumazioni_da_effettuare;
        }

        /* TBD: Completare la chiamata ad "attendi_consumazioni()" */

        int n = attendi_consumazioni(buffer, conteggio);

        consumazioni_effettuate += n;

        consumazioni_da_effettuare -= n;

        printf("[CONTEGGIO] Sono stati consumati in totale %d elementi\n", consumazioni_effettuate);
    }

    return NULL;
}
