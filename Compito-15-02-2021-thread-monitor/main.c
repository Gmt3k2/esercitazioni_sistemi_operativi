#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "prodcons.h"

void * produttore(void *);
void * consumatore(void *);

int main() {

    pthread_t t_produttori[5];
    pthread_t t_consumatore;

    printf("[MAIN] inizializza_monitor\n");

    MonitorProdCons * m = (MonitorProdCons *)malloc(sizeof(MonitorProdCons));

    inizializza_monitor(m);

    srand(time(NULL));


    printf("[MAIN] pthread_create\n");

    for(int i=0; i<5; i++) {

        pthread_create(&t_produttori[i], NULL, produttore, m);
    }

    pthread_create(&t_consumatore, NULL, consumatore, m);



    printf("[MAIN] pthread_join\n");

    for(int i=0; i<5; i++) {

        pthread_join(t_produttori[i], NULL);
    }

    pthread_join(t_consumatore, NULL);



    printf("[MAIN] distruzione_monitor\n");

    distruggi_monitor(m);

    free(m);

}


void * produttore(void * p) {

    printf("[MAIN] Avvio thread produttore\n");

    MonitorProdCons * m = (MonitorProdCons *)p;

    for(int i=0; i<4; i++) {

        int val = rand() % 10;

        printf("[MAIN] Produttore ha generato %d\n", val);

        int ret = produci(m, val);

        while(ret < 0) {

            sleep(1);

            printf("[MAIN] Produttore ritenta inserimento di %d\n", val);

            ret = produci(m, val);
        }

    }


    pthread_exit(NULL);
}

void * consumatore(void * p) {

    printf("[MAIN] Avvio thread consumatore\n");

    MonitorProdCons * m = (MonitorProdCons *)p;

    for(int i=0; i<20; i++) {

        int val;

        int ret = consuma(m, &val);

        printf("[MAIN] Consumatore ha prelevato %d\n", val);

        sleep(2);
    }


    pthread_exit(NULL);
}