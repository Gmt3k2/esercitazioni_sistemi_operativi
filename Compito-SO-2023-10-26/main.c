#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "header.h"

void * produttore(void *);
void * consumatore(void *);

int main() {

    pthread_t thread_consumatore;
    pthread_t thread_produttori[3];

    /* TBD: Creare un oggetto monitor, e inizializzarlo con init_monitor() */
    MonitorIO* p = (MonitorIO*) malloc(sizeof(MonitorIO));
    init_monitor(p);

    /* TBD: Creare 3 thread produttori, chiamando la funzione produttore() */
    for (int i = 0; i < 3; i++)
    {
        if(pthread_create(&thread_produttori[i], NULL, produttore, (void*) p) == 0)
            printf("Thread PRODUTTORE creato correttamente\n");
    }
    
    /* TBD: Creare 1 thread consumatore, chiamando la funzione consumatore() */
    if(pthread_create(&thread_consumatore, NULL, consumatore, (void*) p) == 0)
        printf("Thread CONSUMATORE creato correttamente\n");

    /* TBD: Attendere la terminazione dei 3 thread produttori */
    for (int i = 0; i < 3; i++){
        if(pthread_join(thread_produttori[i], NULL) == 0)
            printf("Thread PRODUTTORE terminato correttamente\n");
    }
    

    /* TBD: Attendere la terminazione del thread consumatore */
    if(pthread_join(thread_consumatore, NULL) == 0)
        printf("Thread CONSUMATORE terminato correttamente\n");

    /* TBD: Disattivare e deallocare l'oggetto monitor */
    end_monitor(p);
    free(p);
    return 0;
}


void * produttore(void * p) {

    printf("Avvio thread produttore...\n");
    MonitorIO* m = (MonitorIO*) p;

    for(int i=0; i<5; i++) {

        sleep(1);

        // genera carattere casuale tra "a" e "z"
        char carattere = 97 + (rand() % 26);

        /* TBD: Chiamare il metodo "produzione()", passando l'oggetto monitor e
         *      la variabile "carattere" */
        produzione(m, carattere);
    }

    return NULL;
}

void * consumatore(void * p) {

    printf("Avvio thread consumatore...\n");
    MonitorIO* m = (MonitorIO*) p;


    int totale_consumazioni = 0;

    while(totale_consumazioni < 15) {

        int occupati = 1;
        
        if(totale_consumazioni < 13) {

            printf("CONSUMATORE: In attesa di elementi disponibili...\n");

            occupati = attesa(m);

            printf("CONSUMATORE: %d elementi disponibili\n", occupati);
        }

        for(int j=0; j<occupati; j++) {

            char carattere;
            
            carattere = consumazione(m);
        }

        totale_consumazioni += occupati;
    }

    return NULL;
}
