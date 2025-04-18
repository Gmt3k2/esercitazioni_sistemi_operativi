#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include "header.h"
#include <pthread.h>

#define MAX_OFF 10

// id code
int mq_offerte; 
int mq_risposte;


void* client(void* par) {
    int my_id = *((int*) par);
    int base_asta = 0;
    int aggiudicata = 0;

    while(!aggiudicata) {
        int offerta = base_asta + rand()%MAX_OFF+1;
        offerta_msg off;

        //TODO: invio 'off' al server
        off.id_cliente = my_id;
        off.valore_offerto = offerta;
        msgsnd(mq_offerte, &off, sizeof(offerta_msg)-sizeof(long), 0);
        printf("CLIENT %d: invio offerta %d\n",my_id,offerta);
        
        //TODO: ricezione in 'risp' dello stato offerta corrente per my_id
        risposta_offerta_msg risp;
        msgrcv(mq_risposte, &risp, sizeof(risposta_offerta_msg)-sizeof(long), 0, 0);
        aggiudicata = risp.asta_aggiudicata;

        if(aggiudicata) {    
            printf("CLIENT %d: asta aggiudicata da ",my_id);
            if (risp.id_vincitore==my_id) {
                printf("ME, SONO IL VINCITORE!!\n");
            } else printf("%d\n",risp.id_vincitore);
        } else {
            //rialzo base d'asta in base al valore corrente
            base_asta = risp.max_valore_offerto;
            printf("CLIENT %d: max valore offerta %d, rialzo!\n",my_id,risp.max_valore_offerto);
        }    
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {

    //TODO: completare l'inizializzazione code
    int key_queue_off = ftok("./", 'o');
    int key_queue_risp = ftok("./", 'r');
    mq_offerte = msgget(key_queue_off, 0);
    mq_risposte = msgget(key_queue_risp, 0);

    pthread_t client_thr[MAX_CLIENTI];
    for (long i = 1; i < MAX_CLIENTI+1; i++) {
        srand(time(NULL)*getpid()*i);
        // TODO creazione thread client i-mo
        int* client_id = &i;
        if(pthread_create(&client_thr[i], NULL, client, (void *) client_id) == 0)
            printf("Thread client correttamente creato\n");
    }

    // TODO join dei thread 
    for (int i = 1; i < MAX_CLIENTI+1; i++){
        if(pthread_join(client_thr[i], NULL) == 0){
            printf("Thread client terminato correttamente\n");
        }
    }
    

    pthread_exit(NULL);
}
